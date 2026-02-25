#ifndef LOG_H
#define LOG_H

#include <stdint.h>

void log_init(void);
void log_str(const char *str);
void log_int(uint32_t val);
void log_hex(uint32_t val);

// Удобные макросы
#define LOG(msg) log_str(msg "\n")
#define LOG_VAL(msg, val) do { log_str(msg); log_int(val); log_str("\n"); } while(0)

#endif