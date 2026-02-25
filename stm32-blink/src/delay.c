/**
 * @brief Модуль задержек
 */

#include "delay.h"

void delay(volatile uint32_t count) {
    while (count--) {
        __asm__("nop");  // Пустая инструкция, чтобы оптимизатор не удалил цикл
    }
}
