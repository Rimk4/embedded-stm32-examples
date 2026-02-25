/**
 * @brief Основной модуль программы
 */

#include "gpio.h"
#include "delay.h"

int main(void) {
    // Инициализация
    gpio_init();
    
    // Убедимся, что оба светодиода выключены
    gpio_led_off(LED_RED);
    gpio_led_off(LED_GREEN);
    
    while (1) {
        // Включить красный, выключить зелёный
        gpio_led_on(LED_RED);
        gpio_led_off(LED_GREEN);
        delay(1000000);
        
        // Выключить красный, включить зелёный
        gpio_led_off(LED_RED);
        gpio_led_on(LED_GREEN);
        delay(1000000);
    }
    
    return 0;
}
