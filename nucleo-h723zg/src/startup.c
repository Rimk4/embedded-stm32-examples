/**
 * @brief Startup code и таблица векторов прерываний для STM32H723ZGT6
 */

#include <stdint.h>

// Обработчики прерываний
void Default_Handler(void);
void Reset_Handler(void);
void NMI_Handler(void)          __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)          __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)      __attribute__((weak, alias("Default_Handler")));

// Символы из linker script
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

// Прототип main
int main(void);

// Таблица векторов прерываний
__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))(&_estack),    // 0: Stack pointer
    Reset_Handler,                   // 1: Reset
    NMI_Handler,                      // 2: NMI
    HardFault_Handler,                 // 3: Hard Fault
    MemManage_Handler,                  // 4: MemManage
    BusFault_Handler,                    // 5: Bus Fault
    UsageFault_Handler,                   // 6: Usage Fault
    0, 0, 0, 0,                           // 7-10: Reserved
    SVC_Handler,                           // 11: SVCall
    DebugMon_Handler,                       // 12: Debug Monitor
    0,                                       // 13: Reserved
    PendSV_Handler,                           // 14: PendSV
    SysTick_Handler,                            // 15: SysTick
    // Остальные векторы можно добавить при необходимости
};

void Reset_Handler(void) {
    // Копируем .data из flash в RAM
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while(dst < &_edata) {
        *dst++ = *src++;
    }
    
    // Обнуляем .bss
    dst = &_sbss;
    while(dst < &_ebss) {
        *dst++ = 0;
    }
    
    // Переходим в main
    main();
    
    // Бесконечный цикл на случай возврата
    while(1);
}

void Default_Handler(void) {
    while(1);
}
