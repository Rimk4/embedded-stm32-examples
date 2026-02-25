/**
 * @brief Основной модуль программы
 */

#include "gpio.h"
#include "delay.h"
#include "log.h"

int main(void) {
    log_init();
    log_str("System started\n");
    
    gpio_init();
    
    int counter = 0;
    
    while (1) {
        gpio_led_on(LED_RED);
        gpio_led_off(LED_GREEN);
        log_str("RED on, GREEN off, count: ");
        log_int(counter++);
        log_str("\n");
        delay(1000000);
        
        gpio_led_off(LED_RED);
        gpio_led_on(LED_GREEN);
        log_str("RED off, GREEN on\n");
        delay(1000000);
    }
}
