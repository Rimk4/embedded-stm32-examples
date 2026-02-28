#ifndef STM32H7_REGS_H
#define STM32H7_REGS_H

#include <stdint.h>

// ==================== АДРЕСА ПЕРИФЕРИИ ====================
#define RCC_BASE        0x58024400UL
#define GPIOB_BASE      0x58020400UL
#define GPIOE_BASE      0x58021000UL

// ==================== РЕГИСТРЫ RCC ====================
#define RCC_AHB4ENR     (*(volatile uint32_t *)(RCC_BASE + 0xE0UL))

// ==================== РЕГИСТРЫ GPIO ====================
#define GPIO_MODER      0x00
#define GPIO_OTYPER     0x04
#define GPIO_OSPEEDR    0x08
#define GPIO_PUPDR      0x0C
#define GPIO_ODR        0x14
#define GPIO_BSRR       0x18

// GPIOB
#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + GPIO_MODER))
#define GPIOB_OTYPER    (*(volatile uint32_t *)(GPIOB_BASE + GPIO_OTYPER))
#define GPIOB_OSPEEDR   (*(volatile uint32_t *)(GPIOB_BASE + GPIO_OSPEEDR))
#define GPIOB_PUPDR     (*(volatile uint32_t *)(GPIOB_BASE + GPIO_PUPDR))
#define GPIOB_ODR       (*(volatile uint32_t *)(GPIOB_BASE + GPIO_ODR))
#define GPIOB_BSRR      (*(volatile uint32_t *)(GPIOB_BASE + GPIO_BSRR))

// GPIOE
#define GPIOE_MODER     (*(volatile uint32_t *)(GPIOE_BASE + GPIO_MODER))
#define GPIOE_OTYPER    (*(volatile uint32_t *)(GPIOE_BASE + GPIO_OTYPER))
#define GPIOE_OSPEEDR   (*(volatile uint32_t *)(GPIOE_BASE + GPIO_OSPEEDR))
#define GPIOE_PUPDR     (*(volatile uint32_t *)(GPIOE_BASE + GPIO_PUPDR))
#define GPIOE_ODR       (*(volatile uint32_t *)(GPIOE_BASE + GPIO_ODR))
#define GPIOE_BSRR      (*(volatile uint32_t *)(GPIOE_BASE + GPIO_BSRR))

// ==================== БИТОВЫЕ МАСКИ ====================
#define RCC_AHB4ENR_GPIOBEN   (1UL << 1)
#define RCC_AHB4ENR_GPIOEEN   (1UL << 4)

#endif // STM32H7_REGS_H
