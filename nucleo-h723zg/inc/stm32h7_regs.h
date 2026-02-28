#ifndef STM32H7_REGS_H
#define STM32H7_REGS_H

#include <stdint.h>

// ==================== АДРЕСА ПЕРИФЕРИИ ====================
#define RCC_BASE        0x58024400UL
#define GPIOB_BASE      0x58020400UL
#define GPIOE_BASE      0x58021000UL
#define GPIOD_BASE      0x58020C00UL

// ==================== ПРАВИЛЬНЫЕ РЕГИСТРЫ RCC ИЗ RM0468 ====================
// Таблица 65: RCC_AHB4ENR - AHB4 clock enable register (offset 0xE0)
#define RCC_AHB4ENR     (*(volatile uint32_t *)(RCC_BASE + 0xE0))

// RCC_AHB4RSTR - AHB4 peripheral reset register (offset 0x88)
#define RCC_AHB4RSTR    (*(volatile uint32_t *)(RCC_BASE + 0x88))

// Таблица 74: RCC_AHB4LPENR - AHB4 Sleep clock register (offset 0x108)
#define RCC_AHB4LPENR   (*(volatile uint32_t *)(RCC_BASE + 0x108))

// APB1L clock enable register (offset 0xE8)
#define RCC_APB1LENR    (*(volatile uint32_t *)(RCC_BASE + 0xE8))

// ==================== РЕГИСТРЫ GPIO ====================
#define GPIO_MODER      0x00
#define GPIO_OTYPER     0x04
#define GPIO_OSPEEDR    0x08
#define GPIO_PUPDR      0x0C
#define GPIO_ODR        0x14
#define GPIO_BSRR       0x18
#define GPIO_AFRL       0x20
#define GPIO_AFRH       0x24

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

// GPIOD
#define GPIOD_MODER     (*(volatile uint32_t *)(GPIOD_BASE + GPIO_MODER))
#define GPIOD_OTYPER    (*(volatile uint32_t *)(GPIOD_BASE + GPIO_OTYPER))
#define GPIOD_OSPEEDR   (*(volatile uint32_t *)(GPIOD_BASE + GPIO_OSPEEDR))
#define GPIOD_PUPDR     (*(volatile uint32_t *)(GPIOD_BASE + GPIO_PUPDR))
#define GPIOD_ODR       (*(volatile uint32_t *)(GPIOD_BASE + GPIO_ODR))
#define GPIOD_BSRR      (*(volatile uint32_t *)(GPIOD_BASE + GPIO_BSRR))
#define GPIOD_AFRL      (*(volatile uint32_t *)(GPIOD_BASE + GPIO_AFRL))
#define GPIOD_AFRH      (*(volatile uint32_t *)(GPIOD_BASE + GPIO_AFRH))

// ==================== БИТОВЫЕ МАСКИ ====================
#define RCC_AHB4ENR_GPIOBEN   (1UL << 1)   // Bit 1: GPIOB clock enable
#define RCC_AHB4ENR_GPIOEEN   (1UL << 4)   // Bit 4: GPIOE clock enable
#define RCC_AHB4ENR_GPIODEN   (1UL << 3)   // Bit 3: GPIOD clock enable

#define RCC_APB1LENR_USART3EN (1UL << 18)  // Bit 18: USART3 clock enable

#endif // STM32H7_REGS_H
