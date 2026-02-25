#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// Идентификаторы светодиодов
typedef enum {
    LED_RED,
    LED_GREEN
} LedId;

// Инициализация GPIO для светодиодов
void gpio_init(void);

// Управление светодиодами
void gpio_led_on(LedId led);
void gpio_led_off(LedId led);

#endif // GPIO_H
