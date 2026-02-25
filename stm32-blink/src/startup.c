/**
 * @brief Модуль векторов прерываний и стартовой инициализации
 */

// Обработчики прерываний (все ведут на бесконечный цикл)
void Default_Handler(void) {
    while (1);
}

// Определяем все необходимые обработчики
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

void Reset_Handler(void);
int main(void);

// Полная таблица векторов для STM32F4
__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))0x20020000,  // 0: Initial SP value
    Reset_Handler,                // 1: Reset
    NMI_Handler,                  // 2: NMI
    HardFault_Handler,            // 3: Hard Fault
    MemManage_Handler,            // 4: MemManage Fault
    BusFault_Handler,             // 5: Bus Fault
    UsageFault_Handler,           // 6: Usage Fault
    0, 0, 0, 0,                   // 7-10: Reserved
    SVC_Handler,                  // 11: SVCall
    DebugMon_Handler,             // 12: Debug Monitor
    0,                            // 13: Reserved
    PendSV_Handler,               // 14: PendSV
    SysTick_Handler,              // 15: SysTick
    // Далее идут внешние прерывания (можно оставить 0)
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 16-25
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 26-35
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 36-45
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 46-55
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 56-65
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 66-75
    0, 0, 0, 0, 0, 0              // 76-81
};

void Reset_Handler(void) {
    main();
}
