/**
 * @brief Мигание светодиодами и вывод Hello World через UART для NUCLEO-H723ZG
 * 
 * Светодиоды:
 * - LED1 (Green)  - PB0
 * - LED2 (Yellow) - PE1  
 * - LED3 (Red)    - PB14
 * 
 * UART3 на PD8(TX), PD9(RX) подключен к ST-Link Virtual COM порт
 */

#include "led.h"
#include "delay.h"
#include "uart.h"

int main(void) {
    // Инициализация периферии
    init_leds();
    uart_init();
    
    // Все светодиоды выключены
    led_green_off();
    led_red_off();
    led_yellow_off();
    
    // Приветственное сообщение
    uart_send_string("\r\n========================================\r\n");
    uart_send_string("NUCLEO-H723ZG Hello World!\r\n");
    uart_send_string("========================================\r\n");
    
    uart_printf("System initialized with custom printf!\r\n");
    uart_printf("LED Blinking started...\r\n");
    
    unsigned int counter = 0;
    
    while(1) {
        // Green ON
        led_green_on();
        uart_printf("[%u] Green LED ON\r\n", counter++);
        delay_ms(500);
        
        // Green OFF, Yellow ON
        led_green_off();
        led_yellow_on();
        uart_printf("[%u] Yellow LED ON\r\n", counter++);
        delay_ms(500);
        
        // Yellow OFF, Red ON
        led_yellow_off();
        led_red_on();
        uart_printf("[%u] Red LED ON\r\n", counter++);
        delay_ms(500);
        
        // Red OFF
        led_red_off();
        uart_printf("[%u] All LEDs OFF\r\n", counter++);
        delay_ms(500);
    }
    
    return 0;
}
