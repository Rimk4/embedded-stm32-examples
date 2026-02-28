#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

void delay(volatile uint32_t count);
void delay_ms(uint32_t ms);

#endif // DELAY_H
