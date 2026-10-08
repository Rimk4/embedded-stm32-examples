#include <stdint.h>

// Глобальные переменные CMSIS для расчета частоты в HAL и TinyUSB
uint32_t SystemCoreClock = 64000000UL;
uint32_t SystemD2Clock = 64000000UL;
const uint8_t D1CorePrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

void SystemInit(void) {
    // Пустая функция для совместимости
}

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
void SysTick_Handler(void);
void OTG_HS_IRQHandler(void);

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

int main(void);

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
    
    // Внешние прерывания (до 77-го номера: OTG_HS)
    [16 + 77] = OTG_HS_IRQHandler               // Прерывание USB OTG HS
};

void Reset_Handler(void) {
    // Включаем FPU (плавающую точку), чтобы компилятор не падал на FPU-инструкциях
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
      SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    #else
      *((volatile uint32_t *)0xE000ED88) |= ((3UL << 20) | (3UL << 22));
    #endif

    // Копируем .data из Flash в RAM
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
    
    // СРАЗУ переходим в main! Без __libc_init_array и без SystemInit
    main();
    
    while(1);
}

void Default_Handler(void) {
    while(1);
}
