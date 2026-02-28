#ifndef LED_H
#define LED_H

#include <stdint.h>

// Пины светодиодов
#define PIN_LED_GREEN   0   // PB0
#define PIN_LED_RED     14  // PB14
#define PIN_LED_YELLOW  1   // PE1

// Прототипы функций
void init_leds(void);
void led_green_on(void);
void led_green_off(void);
void led_red_on(void);
void led_red_off(void);
void led_yellow_on(void);
void led_yellow_off(void);

#endif // LED_H
