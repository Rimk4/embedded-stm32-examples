#ifndef UART_H
#define UART_H

#include <stdint.h>

// Инициализация UART3 (подключен к ST-Link VCP)
void uart_init(void);

// Отправка одного символа
void uart_send_char(char c);

// Отправка строки
void uart_send_string(const char* str);

// Отправка буфера данных
void uart_send_data(const uint8_t* data, uint32_t len);

// Получение символа (если доступен)
int uart_receive_char(void);

// Проверка, есть ли данные для чтения
uint8_t uart_data_available(void);

// Собственная реализация printf
void uart_printf(const char* format, ...);

#endif // UART_H
