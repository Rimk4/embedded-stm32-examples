#include "main.h"

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void SysTick_Handler(void) {
    HAL_IncTick();
}

void OTG_HS_IRQHandler(void) {
    tud_int_handler(1);
}

// Заглушки newlib
void _close(void) {}
void _lseek(void) {}
void _read(void) {}
void _write(void) {}

int main(void) {
    // 1. Аппаратный запуск тактов GPIOB и GPIOE напрямую
    *((volatile uint32_t *)(0x58024400UL + 0xE0)) |= (1UL << 1) | (1UL << 4); // GPIOBEN | GPIOEEN
    for (volatile int i = 0; i < 500; i++);

    // PB0 (Green) и PB14 (Red) на выход
    *((volatile uint32_t *)(0x58020400UL + 0x00)) &= ~((3UL << 0) | (3UL << 28));
    *((volatile uint32_t *)(0x58020400UL + 0x00)) |= ((1UL << 0) | (1UL << 28));

    // PE1 (Yellow) на выход
    *((volatile uint32_t *)(0x58021000UL + 0x00)) &= ~(3UL << 2);
    *((volatile uint32_t *)(0x58021000UL + 0x00)) |= (1UL << 2);

    // СТАРТ: Включаем Зеленый (PB0)
    *((volatile uint32_t *)(0x58020400UL + 0x18)) = (1UL << 0);

    // 2. Инициализация HAL
    HAL_Init();

    // 3. Тактирование
    SystemClock_Config();

    // ШАГ 2 ПРОЙДЕН: Включаем Желтый (PE1)
    *((volatile uint32_t *)(0x58021000UL + 0x18)) = (1UL << 1);

    // 4. Пины USB
    MX_GPIO_Init();

    // ШАГ 3 ПРОЙДЕН: Включаем Красный (PB14)
    *((volatile uint32_t *)(0x58020400UL + 0x18)) = (1UL << 14);

    // 5. Запуск TinyUSB
    tusb_init();

    uint32_t last_blink = 0;
    while (1) {
        tud_task();

        // Если мы в цикле — мигаем Желтым (PE1)
        if (HAL_GetTick() - last_blink > 250) {
            last_blink = HAL_GetTick();
            *((volatile uint32_t *)(0x58021000UL + 0x14)) ^= (1UL << 1);
        }
    }
}

// Сетевые коллбэки TinyUSB
bool tud_network_recv_cb(const uint8_t *src, uint16_t size) {
    (void) src;
    (void) size;
    tud_network_recv_renew();
    return true;
}

uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg) {
    (void) dst;
    (void) ref;
    return arg;
}

void tud_network_init_cb(void) {}

void rndis_class_set_handler(uint8_t *data, int size) {
    (void) data;
    (void) size;
}

// Тактирование под H723
static void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    // 1. Настраиваем Voltage Scale 1 с безопасным таймаутом
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    for (volatile int i = 0; i < 50000; i++) {
        if (__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) break;
    }

    // 2. Включаем питание трансивера USB (VDD33USB)
    HAL_PWREx_EnableUSBVoltageDetector();

    // 3. Запускаем HSE (8 МГц) и встроенный высокоточный генератор HSI48 для USB
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 2;
    RCC_OscInitStruct.PLL.PLLN = 200; // VCO = 800 MHz
    RCC_OscInitStruct.PLL.PLLP = 2;   // CPU = 400 MHz
    RCC_OscInitStruct.PLL.PLLQ = 4;
    RCC_OscInitStruct.PLL.PLLR = 2;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    // 4. Делители системных шин под 400 МГц
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                  RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2; // 200 MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

    // 5. USB питается напрямую от стабильного внутреннего HSI48 (48 МГц)
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

static void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    // 1. Включаем питание разъема USB CN13 (ножка PG10 на Nucleo-144)
    GPIO_InitTypeDef GPIO_Pwr = {0};
    GPIO_Pwr.Pin = GPIO_PIN_10;
    GPIO_Pwr.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Pwr.Pull = GPIO_NOPULL;
    GPIO_Pwr.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_Pwr);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET); // Активный уровень для подачи VBUS

    // 2. Настройка пинов PA11 (DM) и PA12 (DP)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. Тактирование USB OTG HS
    __HAL_RCC_USB1_OTG_HS_CLK_ENABLE();

    // 4. Принудительно выбираем ВСТРОЕННЫЙ FS PHY (а не ULPI)
    USB_OTG_HS->GUSBCFG |= USB_OTG_GUSBCFG_PHYSEL;

    // 5. Включаем питание трансивера и отключаем детектор VBUS (Force B-device)
    USB_OTG_HS->GCCFG |= USB_OTG_GCCFG_PWRDWN;
    USB_OTG_HS->GCCFG &= ~USB_OTG_GCCFG_VBDEN;

    HAL_NVIC_SetPriority(OTG_HS_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}
