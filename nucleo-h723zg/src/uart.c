#include "uart.h"
#include "stm32h7_regs.h"

// Определения для UART3
#define USART3_BASE     0x40004800UL

// Регистры USART (для STM32H7)
#define USART3_CR1      (*(volatile uint32_t *)(USART3_BASE + 0x00))
#define USART3_CR2      (*(volatile uint32_t *)(USART3_BASE + 0x04))
#define USART3_CR3      (*(volatile uint32_t *)(USART3_BASE + 0x08))
#define USART3_BRR      (*(volatile uint32_t *)(USART3_BASE + 0x0C))
#define USART3_ISR      (*(volatile uint32_t *)(USART3_BASE + 0x1C))
#define USART3_RDR      (*(volatile uint32_t *)(USART3_BASE + 0x24))
#define USART3_TDR      (*(volatile uint32_t *)(USART3_BASE + 0x28))

// Битовые маски USART
#define USART_CR1_UE     (1UL << 0)
#define USART_CR1_RE     (1UL << 2)
#define USART_CR1_TE     (1UL << 3)
#define USART_ISR_TXE    (1UL << 7)
#define USART_ISR_TC     (1UL << 6)
#define USART_ISR_RXNE   (1UL << 5)

// Пины для USART3: PD8 (TX), PD9 (RX)
#define PIN_UART_TX     8   // PD8
#define PIN_UART_RX     9   // PD9

// RCC регистры для STM32H723
#define RCC_AHB4ENR     (*(volatile uint32_t *)(RCC_BASE + 0xE0))  // AHB4 peripheral clock enable


// Альтернативная функция для USART3 - AF7
#define GPIO_AF7        7UL

// Макросы для работы с AFR (Alternate Function Register)
// Для пинов 0-7 используется AFRL, для пинов 8-15 используется AFRH
#define AFR_PIN_OFFSET(pin) (((pin) & 0x07) * 4)  // Смещение в регистре для пина (0, 4, 8, 12, 16, 20, 24, 28)
#define AFR_PIN_MASK(pin)   (0xFUL << AFR_PIN_OFFSET(pin))

void uart_init(void) {
    // 1. Включаем тактирование
    RCC_AHB4ENR |= RCC_AHB4ENR_GPIODEN;
    RCC_APB1LENR |= RCC_APB1LENR_USART3EN;
    
    // 2. Небольшая задержка для стабилизации тактирования
    for(volatile int i = 0; i < 1000; i++);
    
    // 3. Настройка пинов PD8 (TX) и PD9 (RX)
    
    // Сбрасываем режим для PD8 и PD9
    GPIOD_MODER &= ~(3UL << (PIN_UART_TX * 2));
    GPIOD_MODER &= ~(3UL << (PIN_UART_RX * 2));
    
    // Устанавливаем режим альтернативной функции (10) для PD8 и PD9
    GPIOD_MODER |= (2UL << (PIN_UART_TX * 2));  // 10 = alternate function
    GPIOD_MODER |= (2UL << (PIN_UART_RX * 2));  // 10 = alternate function
    
    // Высокая скорость
    GPIOD_OSPEEDR |= (3UL << (PIN_UART_TX * 2));
    GPIOD_OSPEEDR |= (3UL << (PIN_UART_RX * 2));
    
    // Без подтяжки (push-pull)
    GPIOD_PUPDR &= ~(3UL << (PIN_UART_TX * 2));
    GPIOD_PUPDR &= ~(3UL << (PIN_UART_RX * 2));
    
    // 4. Настройка альтернативной функции AF7 для PD8 и PD9
    // Так как PD8 и PD9 > 7, используем AFRH
    
    // Для PD8 (пин 8) - биты 3-0 в AFRH
    GPIOD_AFRH &= ~AFR_PIN_MASK(PIN_UART_TX);        // Очищаем биты для PD8
    GPIOD_AFRH |= (GPIO_AF7 << AFR_PIN_OFFSET(PIN_UART_TX));  // Устанавливаем AF7
    
    // Для PD9 (пин 9) - биты 7-4 в AFRH
    GPIOD_AFRH &= ~AFR_PIN_MASK(PIN_UART_RX);        // Очищаем биты для PD9
    GPIOD_AFRH |= (GPIO_AF7 << AFR_PIN_OFFSET(PIN_UART_RX));  // Устанавливаем AF7
    
    // 5. Настройка USART3
    USART3_CR1 = 0;  // Сброс
    
    // BRR для 115200 бод при 64MHz
    // USARTDIV = 64,000,000 / 115200 = 555.55
    // BRR = 555 = 0x22B
    USART3_BRR = 555;
    
    // Включаем передатчик и приемник
    USART3_CR1 = USART_CR1_TE | USART_CR1_RE;
    
    // Включаем USART
    USART3_CR1 |= USART_CR1_UE;
    
    // 6. Ждем готовности
    for(volatile int i = 0; i < 1000; i++);
}

void uart_send_char(char c) {
    // Ждем освобождения буфера передачи
    while (!(USART3_ISR & USART_ISR_TXE)) {
        __asm__("nop");
    }
    
    // Отправляем символ
    USART3_TDR = c;
    
    // Ждем завершения передачи
    while (!(USART3_ISR & USART_ISR_TC)) {
        __asm__("nop");
    }
}

void uart_send_string(const char* str) {
    while (*str) {
        if (*str == '\n') {
            uart_send_char('\r');  // Добавляем CR перед LF
        }
        uart_send_char(*str++);
    }
}

void uart_send_data(const uint8_t* data, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        uart_send_char(data[i]);
    }
}

int uart_receive_char(void) {
    if (USART3_ISR & USART_ISR_RXNE) {
        return USART3_RDR;
    }
    return -1;
}

uint8_t uart_data_available(void) {
    return (USART3_ISR & USART_ISR_RXNE) ? 1 : 0;
}

// Вспомогательная функция для преобразования числа в строку
static void uart_itoa(int value, char* str, int base) {
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    int tmp_value;
    int negative = 0;
    
    if (value < 0 && base == 10) {
        negative = 1;
        value = -value;
    }
    
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789ABCDEF"[tmp_value - value * base];
    } while (value);
    
    if (negative) {
        *ptr++ = '-';
    }
    
    *ptr-- = '\0';
    
    // Reverse string
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

// Вспомогательная функция для преобразования unsigned в строку
static void uart_uitoa(unsigned int value, char* str, int base) {
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    unsigned int tmp_value;
    
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789ABCDEF"[tmp_value - value * base];
    } while (value);
    
    *ptr-- = '\0';
    
    // Reverse string
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

// Вспомогательная функция для преобразования hex в строку
static void uart_tohex(unsigned int value, char* str, int uppercase) {
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    unsigned int tmp_value;
    const char* digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    
    *ptr++ = '0';
    *ptr++ = 'x';
    
    do {
        tmp_value = value;
        value /= 16;
        *ptr++ = digits[tmp_value - value * 16];
    } while (value);
    
    *ptr-- = '\0';
    
    // Reverse string (skip "0x" prefix)
    ptr1 = str + 2;
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

// Собственная реализация printf
void uart_printf(const char* format, ...) {
    char buffer[32];
    char* arg_ptr = (char*)&format + sizeof(format);
    
    while (*format) {
        if (*format != '%') {
            uart_send_char(*format++);
            continue;
        }
        
        format++; // Переходим к спецификатору
        
        switch (*format) {
            case 'd':  // Знаковое десятичное
            case 'i': {
                int value = *(int*)arg_ptr;
                arg_ptr += sizeof(int);
                uart_itoa(value, buffer, 10);
                uart_send_string(buffer);
                break;
            }
            
            case 'u': {  // Беззнаковое десятичное
                unsigned int value = *(unsigned int*)arg_ptr;
                arg_ptr += sizeof(unsigned int);
                uart_uitoa(value, buffer, 10);
                uart_send_string(buffer);
                break;
            }
            
            case 'x':  // Шестнадцатеричное (нижний регистр)
            case 'X': {  // Шестнадцатеричное (верхний регистр)
                unsigned int value = *(unsigned int*)arg_ptr;
                arg_ptr += sizeof(unsigned int);
                uart_tohex(value, buffer, (*format == 'X'));
                uart_send_string(buffer);
                break;
            }
            
            case 'c': {  // Символ
                char c = *(char*)arg_ptr;
                arg_ptr += sizeof(char);
                uart_send_char(c);
                break;
            }
            
            case 's': {  // Строка
                char* str = *(char**)arg_ptr;
                arg_ptr += sizeof(char*);
                uart_send_string(str);
                break;
            }
            
            case '%': {  // Символ '%'
                uart_send_char('%');
                break;
            }
            
            default:  // Неизвестный спецификатор
                uart_send_char('%');
                uart_send_char(*format);
                break;
        }
        
        format++;
    }
}
