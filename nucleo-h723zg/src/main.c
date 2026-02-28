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

#include "led.h"
#include "delay.h"

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
