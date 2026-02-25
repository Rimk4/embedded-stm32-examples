/**
 * @brief Адреса периферии и регистров STM32F4
 */

#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

// Адреса периферии
#define RCC_BASE        0x40023800
#define GPIOB_BASE      0x40020400
#define GPIOC_BASE      0x40020800

// Регистры RCC
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))

// Регистры GPIO
#define GPIO_MODER_OFFSET   0x00
#define GPIO_ODR_OFFSET     0x14
#define GPIO_BSRR_OFFSET    0x18
#define GPIO_PUPDR_OFFSET   0x0C

#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + GPIO_MODER_OFFSET))
#define GPIOB_ODR       (*(volatile uint32_t *)(GPIOB_BASE + GPIO_ODR_OFFSET))
#define GPIOB_BSRR      (*(volatile uint32_t *)(GPIOB_BASE + GPIO_BSRR_OFFSET))
#define GPIOB_PUPDR     (*(volatile uint32_t *)(GPIOB_BASE + GPIO_PUPDR_OFFSET))

#define GPIOC_MODER     (*(volatile uint32_t *)(GPIOC_BASE + GPIO_MODER_OFFSET))
#define GPIOC_ODR       (*(volatile uint32_t *)(GPIOC_BASE + GPIO_ODR_OFFSET))
#define GPIOC_BSRR      (*(volatile uint32_t *)(GPIOC_BASE + GPIO_BSRR_OFFSET))
#define GPIOC_PUPDR     (*(volatile uint32_t *)(GPIOC_BASE + GPIO_PUPDR_OFFSET))

// Битовые маски
#define RCC_AHB1ENR_GPIOBEN   (1 << 1)  // Бит 1 для GPIOB
#define RCC_AHB1ENR_GPIOCEN   (1 << 2)  // Бит 2 для GPIOC

#endif // REGISTERS_H
