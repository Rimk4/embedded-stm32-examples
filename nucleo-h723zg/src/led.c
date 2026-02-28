#include "led.h"
#include "stm32h7_regs.h"

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
