/**
 * @brief Модуль управления GPIO для светодиодов
 */

#include "gpio.h"
#include "registers.h"

// Конфигурация пинов
#define PIN_LED1        15  // PB15 - Красный
#define PIN_LED2        7   // PC7 - Зелёный

void gpio_init(void) {
    // Включение тактирования GPIOB и GPIOC
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
    
    // Небольшая задержка для стабилизации тактирования
    for (volatile int i = 0; i < 10; i++);
    
    // Настройка PB15 как выход с подтяжкой к питанию
    GPIOB_MODER &= ~(3 << (PIN_LED1 * 2));
    GPIOB_MODER |= (1 << (PIN_LED1 * 2));   // 01 = output
    GPIOB_PUPDR |= (1 << (PIN_LED1 * 2));   // Pull-up
    
    // Настройка PC7 как выход с подтяжкой к питанию
    GPIOC_MODER &= ~(3 << (PIN_LED2 * 2));
    GPIOC_MODER |= (1 << (PIN_LED2 * 2));   // 01 = output
    GPIOC_PUPDR |= (1 << (PIN_LED2 * 2));   // Pull-up
}

void gpio_led_on(LedId led) {
    switch(led) {
        case LED_RED:
            GPIOB_BSRR = (1 << PIN_LED1);  // Set PB15
            break;
        case LED_GREEN:
            GPIOC_BSRR = (1 << PIN_LED2);  // Set PC7
            break;
    }
}

void gpio_led_off(LedId led) {
    switch(led) {
        case LED_RED:
            GPIOB_BSRR = (1 << (PIN_LED1 + 16));  // Reset PB15
            break;
        case LED_GREEN:
            GPIOC_BSRR = (1 << (PIN_LED2 + 16));  // Reset PC7
            break;
    }
}
