/**
 * @brief Минимальный проект мигания светодиодами для NUCLEO-H723ZG
 * 
 * Светодиоды:
 * - LED1 (Green)  - PB0
 * - LED2 (Yellow) - PE1  
 * - LED3 (Red)    - PB14
 * 
 * Мигают по очереди с интервалом ~500 мс
 */

#include <stdint.h>

// ==================== АДРЕСА ПЕРИФЕРИИ ====================
#define RCC_BASE        0x58024400UL
#define GPIOB_BASE      0x58020400UL
#define GPIOE_BASE      0x58021000UL

// ==================== РЕГИСТРЫ RCC ====================
#define RCC_AHB4ENR     (*(volatile uint32_t *)(RCC_BASE + 0xE0UL))

// ==================== РЕГИСТРЫ GPIO ====================
#define GPIO_MODER      0x00
#define GPIO_OTYPER     0x04
#define GPIO_OSPEEDR    0x08
#define GPIO_PUPDR      0x0C
#define GPIO_ODR        0x14
#define GPIO_BSRR       0x18

// GPIOB
#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + GPIO_MODER))
#define GPIOB_OTYPER    (*(volatile uint32_t *)(GPIOB_BASE + GPIO_OTYPER))
#define GPIOB_OSPEEDR   (*(volatile uint32_t *)(GPIOB_BASE + GPIO_OSPEEDR))
#define GPIOB_PUPDR     (*(volatile uint32_t *)(GPIOB_BASE + GPIO_PUPDR))
#define GPIOB_ODR       (*(volatile uint32_t *)(GPIOB_BASE + GPIO_ODR))
#define GPIOB_BSRR      (*(volatile uint32_t *)(GPIOB_BASE + GPIO_BSRR))

// GPIOE
#define GPIOE_MODER     (*(volatile uint32_t *)(GPIOE_BASE + GPIO_MODER))
#define GPIOE_OTYPER    (*(volatile uint32_t *)(GPIOE_BASE + GPIO_OTYPER))
#define GPIOE_OSPEEDR   (*(volatile uint32_t *)(GPIOE_BASE + GPIO_OSPEEDR))
#define GPIOE_PUPDR     (*(volatile uint32_t *)(GPIOE_BASE + GPIO_PUPDR))
#define GPIOE_ODR       (*(volatile uint32_t *)(GPIOE_BASE + GPIO_ODR))
#define GPIOE_BSRR      (*(volatile uint32_t *)(GPIOE_BASE + GPIO_BSRR))

// ==================== БИТОВЫЕ МАСКИ ====================
#define RCC_AHB4ENR_GPIOBEN   (1UL << 1)
#define RCC_AHB4ENR_GPIOEEN   (1UL << 4)

// Пины светодиодов
#define PIN_LED_GREEN   0   // PB0
#define PIN_LED_RED     14  // PB14
#define PIN_LED_YELLOW  1   // PE1

// ==================== ПРОТОТИПЫ ====================
void init_leds(void);
void led_green_on(void);
void led_green_off(void);
void led_red_on(void);
void led_red_off(void);
void led_yellow_on(void);
void led_yellow_off(void);
void delay_ms(uint32_t ms);

// ==================== ОСНОВНАЯ ПРОГРАММА ====================
int main(void) {
    // Инициализация светодиодов
    init_leds();
    
    // Все светодиоды выключены
    led_green_off();
    led_red_off();
    led_yellow_off();
    
    while(1) {
        // Green ON
        led_green_on();
        delay_ms(500);
        
        // Green OFF, Yellow ON
        led_green_off();
        led_yellow_on();
        delay_ms(500);
        
        // Yellow OFF, Red ON
        led_yellow_off();
        led_red_on();
        delay_ms(500);
        
        // Red OFF
        led_red_off();
        delay_ms(500);
    }
    
    return 0;
}

// ==================== LED ФУНКЦИИ ====================
void init_leds(void) {
    // Включаем тактирование GPIOB и GPIOE
    RCC_AHB4ENR |= RCC_AHB4ENR_GPIOBEN | RCC_AHB4ENR_GPIOEEN;
    
    // Небольшая задержка для стабилизации
    for(volatile int i = 0; i < 100; i++);
    
    // Настройка PB0 (Green) как выход
    GPIOB_MODER &= ~(3 << (PIN_LED_GREEN * 2));
    GPIOB_MODER |= (1 << (PIN_LED_GREEN * 2));   // 01 = output
    GPIOB_OTYPER &= ~(1 << PIN_LED_GREEN);       // Push-pull
    GPIOB_OSPEEDR |= (3 << (PIN_LED_GREEN * 2)); // High speed
    GPIOB_PUPDR &= ~(3 << (PIN_LED_GREEN * 2));  // No pull
    
    // Настройка PB14 (Red) как выход
    GPIOB_MODER &= ~(3 << (PIN_LED_RED * 2));
    GPIOB_MODER |= (1 << (PIN_LED_RED * 2));     // 01 = output
    GPIOB_OTYPER &= ~(1 << PIN_LED_RED);         // Push-pull
    GPIOB_OSPEEDR |= (3 << (PIN_LED_RED * 2));   // High speed
    GPIOB_PUPDR &= ~(3 << (PIN_LED_RED * 2));    // No pull
    
    // Настройка PE1 (Yellow) как выход
    GPIOE_MODER &= ~(3 << (PIN_LED_YELLOW * 2));
    GPIOE_MODER |= (1 << (PIN_LED_YELLOW * 2));  // 01 = output
    GPIOE_OTYPER &= ~(1 << PIN_LED_YELLOW);      // Push-pull
    GPIOE_OSPEEDR |= (3 << (PIN_LED_YELLOW * 2)); // High speed
    GPIOE_PUPDR &= ~(3 << (PIN_LED_YELLOW * 2)); // No pull
}

void led_green_on(void) {
    GPIOB_BSRR = (1 << PIN_LED_GREEN);  // Set PB0
}

void led_green_off(void) {
    GPIOB_BSRR = (1 << (PIN_LED_GREEN + 16));  // Reset PB0
}

void led_red_on(void) {
    GPIOB_BSRR = (1 << PIN_LED_RED);  // Set PB14
}

void led_red_off(void) {
    GPIOB_BSRR = (1 << (PIN_LED_RED + 16));  // Reset PB14
}

void led_yellow_on(void) {
    GPIOE_BSRR = (1 << PIN_LED_YELLOW);  // Set PE1
}

void led_yellow_off(void) {
    GPIOE_BSRR = (1 << (PIN_LED_YELLOW + 16));  // Reset PE1
}

// ==================== ФУНКЦИИ ЗАДЕРЖКИ ====================
void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}

void delay_ms(uint32_t ms) {
    // Приблизительно 4000 итераций = 1 мс при 64MHz
    delay(ms * 4000);
}

// ==================== ОБРАБОТЧИКИ ПРЕРЫВАНИЙ ====================
void Default_Handler(void) {
    while(1);
}

void Reset_Handler(void);
void NMI_Handler(void)          __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)          __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)      __attribute__((weak, alias("Default_Handler")));

// Символы из linker script
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

// Таблица векторов прерываний
__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))(&_estack),    // 0: Stack pointer
    Reset_Handler,                   // 1: Reset
    NMI_Handler,                      // 2: NMI
    HardFault_Handler,                 // 3: Hard Fault
    MemManage_Handler,                  // 4: MemManage
    BusFault_Handler,                    // 5: Bus Fault
    UsageFault_Handler,                   // 6: Usage Fault
    0, 0, 0, 0,                           // 7-10: Reserved
    SVC_Handler,                           // 11: SVCall
    DebugMon_Handler,                       // 12: Debug Monitor
    0,                                       // 13: Reserved
    PendSV_Handler,                           // 14: PendSV
    SysTick_Handler,                            // 15: SysTick
    // Остальные векторы (можно оставить 0)
};

void Reset_Handler(void) {
    // Копируем .data из flash в RAM
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while(dst < &_edata) {
        *dst++ = *src++;
    }
    
    // Обнуляем .bss
    dst = &_sbss;
    while(dst < &_ebss) {
        *dst++ = 0;
    }
    
    // Переходим в main
    main();
    
    // Бесконечный цикл на случай возврата
    while(1);
}
