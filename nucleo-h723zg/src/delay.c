#include "delay.h"

void delay(volatile uint32_t count) {
    while(count--) {
        __asm__("nop");
    }
}

void delay_ms(uint32_t ms) {
    // Приблизительно 4000 итераций = 1 мс при 64MHz
    delay(ms * 4000);
}
