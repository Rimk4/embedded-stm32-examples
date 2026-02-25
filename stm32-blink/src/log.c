#include "log.h"
#include "registers.h"

// Добавьте в registers.h если нет:
// #define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))

void log_init(void) {
    // Включить тактирование USART3 и GPIOC
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;    // GPIOC EN (бит 2)
    RCC_APB1ENR |= (1 << 18);               // USART3 EN (бит 18)
    
    // PC10 (TX) - AF7, PC11 (RX) - AF7
    GPIOC_MODER &= ~((3 << 20) | (3 << 22)); // PC10, PC11 (биты 20-23)
    GPIOC_MODER |= (2 << 20) | (2 << 22);    // Alternate function
    
    // Настройка AF (альтернативной функции)
    // AF7 = USART3 для PC10 и PC11
    GPIOC_AFRH &= ~((0xF << 8) | (0xF << 12)); // Очистка для PC10 (биты 8-11) и PC11 (биты 12-15)
    GPIOC_AFRH |= (7 << 8)  | (7 << 12);       // AF7 для PC10 и PC11
    
    // Небольшая задержка для стабилизации
    for (volatile int i = 0; i < 100; i++);
    
    // USART3 настройки: 115200 8N1
    // Если APB1 = 16MHz (проверьте частоту вашей платы)
    USART3_BRR = 16000000 / 115200; // 16MHz / 115200 ≈ 139
    
    USART3_CR1 = (1 << 13) | (1 << 3); // UE | TE (включить USART, включить передатчик)
    // Если нужен и прием, добавьте (1 << 2) для RE
}

void log_putc(char c) {
    // Ждем пока буфер передачи освободится
    while (!(USART3_SR & (1 << 7))); // Ждем TXE (бит 7)
    USART3_DR = c;
    
    // Автоматический перевод строки
    if (c == '\n') {
        while (!(USART3_SR & (1 << 7)));
        USART3_DR = '\r';
    }
}

void log_str(const char *str) {
    while (*str) {
        log_putc(*str++);
    }
}

void log_int(uint32_t val) {
    char buf[16];
    int i = 0;
    
    if (val == 0) {
        log_putc('0');
        return;
    }
    
    // Обработка отрицательных чисел (если нужны)
    // if (val < 0) {
    //     log_putc('-');
    //     val = -val;
    // }
    
    while (val > 0 && i < 15) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }
    
    while (i > 0) {
        log_putc(buf[--i]);
    }
}

void log_hex(uint32_t val) {
    const char hex[] = "0123456789ABCDEF";
    int i;
    
    log_str("0x");
    for (i = 28; i >= 0; i -= 4) {
        log_putc(hex[(val >> i) & 0xF]);
    }
}
