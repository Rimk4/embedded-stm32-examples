#include "main.h"

ETH_HandleTypeDef heth;

// Дескрипторы и кольцевые буферы Ethernet DMA в памяти SRAM D2 (0x30000000)
__attribute__((section(".eth_dma"))) ETH_DMADescTypeDef DMARxDscrTab[4];
__attribute__((section(".eth_dma"))) ETH_DMADescTypeDef DMATxDscrTab[4];
__attribute__((section(".eth_dma"))) uint8_t Rx_Buff[4][1536];

__attribute__((section(".eth_dma"))) static uint8_t eth_tx_buf[1536];

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MPU_Config(void);

void SysTick_Handler(void) {
    HAL_IncTick();
}

void OTG_HS_IRQHandler(void) {
    tud_int_handler(1);
}

void ETH_IRQHandler(void) {
    HAL_ETH_IRQHandler(&heth);
}

// Заглушки newlib
void _close(void) {}
void _lseek(void) {}
void _read(void) {}
void _write(void) {}

// Регистры PHY LAN8742A
#define PHY_ADDR                0x00U
#define PHY_BCR                 0x00U
#define PHY_RESET               (1U << 15)
#define PHY_AUTONEGOTIATION     (1U << 12)
#define PHY_RESTART_AUTONEG     (1U << 9)
#define PHY_FULLDUPLEX_100M     ((1U << 13) | (1U << 8))

static void LAN8742_Init(void) {
    HAL_ETH_WritePHYRegister(&heth, PHY_ADDR, PHY_BCR, PHY_RESET);
    HAL_Delay(50);
    uint32_t reg_val = PHY_AUTONEGOTIATION | PHY_RESTART_AUTONEG | PHY_FULLDUPLEX_100M;
    HAL_ETH_WritePHYRegister(&heth, PHY_ADDR, PHY_BCR, reg_val);
    HAL_Delay(50);
}

static volatile uint16_t eth_tx_len = 0;

int main(void) {
    // Аппаратный запуск светодиодов
    *((volatile uint32_t *)(0x58024400UL + 0xE0)) |= (1UL << 1) | (1UL << 4);
    for (volatile int i = 0; i < 500; i++);
    *((volatile uint32_t *)(0x58020400UL + 0x00)) &= ~((3UL << 0) | (3UL << 28));
    *((volatile uint32_t *)(0x58020400UL + 0x00)) |= ((1UL << 0) | (1UL << 28));
    *((volatile uint32_t *)(0x58021000UL + 0x00)) &= ~(3UL << 2);
    *((volatile uint32_t *)(0x58021000UL + 0x00)) |= (1UL << 2);

    // Зажигаем Зеленый (PB0)
    *((volatile uint32_t *)(0x58020400UL + 0x18)) = (1UL << 0);
    // Гасим Красный (PB14)
    *((volatile uint32_t *)(0x58020400UL + 0x18)) = (1UL << (14 + 16));

    MPU_Config();
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ETH_Init();

    LAN8742_Init();
    HAL_ETH_Start(&heth);

    tusb_init();

    uint32_t last_blink = 0;

    while (1) {
        tud_task();

        // Сердцебиение: мигаем Желтым LED (PE1)
        if (HAL_GetTick() - last_blink > 250) {
            last_blink = HAL_GetTick();
            *((volatile uint32_t *)(0x58021000UL + 0x14)) ^= (1UL << 1);
        }

        // 1. ПОТОК: Из USB от Ubuntu -> в физический кабель Ethernet
        if (eth_tx_len > 0) {
            // Проверяем: освободил ли DMA предыдущую передачу (бит OWN должен быть 0)
            if ((DMATxDscrTab[0].DESC3 & ETH_DMATXNDESCWBF_OWN) == 0) {
                // Адрес буфера данных (в SRAM D2)
                DMATxDscrTab[0].DESC0 = (uint32_t)eth_tx_buf;
                DMATxDscrTab[0].DESC1 = 0;
                // Длина пакета в байтах
                DMATxDscrTab[0].DESC2 = eth_tx_len;
                // FD (First Descriptor) | LD (Last Descriptor) | CIC (Checksum Insert) | OWN (Владение DMA)
                DMATxDscrTab[0].DESC3 = ETH_DMATXNDESCRF_FD | ETH_DMATXNDESCRF_LD | ETH_DMATXNDESCRF_OWN;

                // Указываем DMA на хвост очереди дескрипторов (триггер немедленной отправки)
                heth.Instance->DMACTDTPR = (uint32_t)&DMATxDscrTab[1];

                eth_tx_len = 0;
                tud_network_recv_renew(); // Готовы принимать следующий пакет из USB
            }
        }

        // ПОТОК 2: Из кабеля Ethernet -> в USB хосту
        void *p_rx = NULL;
        if (HAL_ETH_ReadData(&heth, &p_rx) == HAL_OK && p_rx != NULL) {
            uint32_t rx_len = heth.RxDescList.RxDataLength;
            if (rx_len > 0 && tud_network_can_xmit(rx_len)) {
                tud_network_xmit(p_rx, rx_len);
            }
        }
    }
}

bool tud_network_recv_cb(const uint8_t *src, uint16_t size) {
    if (eth_tx_len == 0 && size > 0 && size <= 1514) {
        memcpy(eth_tx_buf, src, size);
        eth_tx_len = size;
        return true;
    }
    return false;
}

uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg) {
    uint16_t len = arg;
    memcpy(dst, ref, len);
    return len;
}

void tud_network_init_cb(void) {}

void rndis_class_set_handler(uint8_t *data, int size) {
    (void) data;
    (void) size;
}

void HAL_ETH_MspInit(ETH_HandleTypeDef *heth_inst) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    __HAL_RCC_ETH1MAC_CLK_ENABLE();
    __HAL_RCC_ETH1TX_CLK_ENABLE();
    __HAL_RCC_ETH1RX_CLK_ENABLE();
    __HAL_RCC_D2SRAM1_CLK_ENABLE();

    HAL_SYSCFG_ETHInterfaceSelect(SYSCFG_ETH_RMII);

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;

    // PA1: REF_CLK, PA2: MDIO, PA7: CRS_DV
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PB13: TXD1
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // PC1: MDC, PC4: RXD0, PC5: RXD1
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // PG11: TX_EN, PG13: TXD0
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_13;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

static void MX_ETH_Init(void) {
    static uint8_t mac_addr[6] = {0x00, 0x80, 0xE1, 0x00, 0x00, 0x01};

    memset(DMATxDscrTab, 0, sizeof(DMATxDscrTab));
    memset(DMARxDscrTab, 0, sizeof(DMARxDscrTab));

    heth.Instance = ETH;
    heth.Init.MACAddr = mac_addr;
    heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
    heth.Init.TxDesc = DMATxDscrTab;
    heth.Init.RxDesc = DMARxDscrTab;
    heth.Init.RxBuffLen = 1536;

    HAL_ETH_Init(&heth);

    // Привязка кольца RX
    for (uint32_t idx = 0; idx < 4; idx++) {
        DMARxDscrTab[idx].DESC0 = (uint32_t)Rx_Buff[idx];
        DMARxDscrTab[idx].DESC1 = 0;
        DMARxDscrTab[idx].DESC2 = 0;
        DMARxDscrTab[idx].DESC3 = ETH_DMARXNDESCRF_BUF1V | ETH_DMARXNDESCRF_OWN;
    }

    // Привязка кольца TX: ядро владеет дескрипторами (OWN = 0)
    for (uint32_t idx = 0; idx < 4; idx++) {
        DMATxDscrTab[idx].DESC0 = 0;
        DMATxDscrTab[idx].DESC1 = 0;
        DMATxDscrTab[idx].DESC2 = 0;
        DMATxDscrTab[idx].DESC3 = 0;
    }

    // Режим Promiscuous (принимать абсолютно все сетевые пакеты)
    ETH_MACFilterConfigTypeDef filterConfig = {0};
    filterConfig.PromiscuousMode = ENABLE;
    filterConfig.PassAllMulticast = ENABLE;
    HAL_ETH_SetMACFilterConfig(&heth, &filterConfig);
}

static void MPU_Config(void) {
    MPU_Region_InitTypeDef MPU_InitStruct = {0};
    HAL_MPU_Disable();

    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress = 0x30000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
    MPU_InitStruct.SubRegionDisable = 0x0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    for (volatile int i = 0; i < 50000; i++) {
        if (__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) break;
    }

    HAL_PWREx_EnableUSBVoltageDetector();

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 2;
    RCC_OscInitStruct.PLL.PLLN = 200;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    RCC_OscInitStruct.PLL.PLLR = 2;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                  RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

static void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    __HAL_RCC_USB1_OTG_HS_CLK_ENABLE();

    USB_OTG_HS->GUSBCFG |= USB_OTG_GUSBCFG_PHYSEL;
    USB_OTG_HS->GCCFG |= USB_OTG_GCCFG_PWRDWN;
    USB_OTG_HS->GCCFG &= ~USB_OTG_GCCFG_VBDEN;

    HAL_NVIC_SetPriority(OTG_HS_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}
