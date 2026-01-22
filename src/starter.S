/* 
 * starter.S - двухэтапный загрузчик для STM32
 * 1. Копирует код из защищенной области Flash в SRAM
 * 2. Передает управление в SRAM
 */

.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

/* ============ ВЕКТОР СБРОСА ============ */
.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
.size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
    .word  _estack          /* Указатель стека */
    .word  Starter_Handler  /* Обработчик сброса (наш загрузчик) */
    .word  NMI_Handler
    .word  HardFault_Handler
    .word  MemManage_Handler
    .word  BusFault_Handler
    .word  UsageFault_Handler
    .word  0, 0, 0, 0       /* Reserved */
    .word  SVC_Handler
    .word  DebugMon_Handler
    .word  0                 /* Reserved */
    .word  PendSV_Handler
    .word  SysTick_Handler
    
    /* External interrupts */
    .word  WWDG_IRQHandler
    /* ... добавьте остальные векторы по необходимости */

/* ============ STARTER HANDLER ============ */
.section .text.Starter_Handler
.global Starter_Handler
.type Starter_Handler, %function
.thumb_func

Starter_Handler:
    /* Сохраняем LR (может понадобиться для возврата) */
    push {lr}
    
    /* 1. Копирование .data секции из Flash в RAM */
    ldr r0, =_sidata        /* Начало .data в Flash (LMA) */
    ldr r1, =_sdata         /* Начало .data в RAM (VMA) */
    ldr r2, =_edata         /* Конец .data в RAM */
    
data_copy_loop:
    cmp r1, r2
    bge data_copy_done
    ldr r3, [r0], #4
    str r3, [r1], #4
    b data_copy_loop
data_copy_done:

    /* 2. Очистка .bss секции (заполнение нулями) */
    ldr r0, =_sbss          /* Начало .bss */
    ldr r1, =_ebss          /* Конец .bss */
    movs r2, #0
    
bss_clear_loop:
    cmp r0, r1
    bge bss_clear_done
    str r2, [r0], #4
    b bss_clear_loop
bss_clear_done:

    /* 3. Копирование основного кода из Flash в SRAM */
    ldr r0, =__stext_lma    /* Начало кода в Flash */
    ldr r1, =__stext        /* Начало кода в SRAM */
    ldr r2, =__etext        /* Конец кода в SRAM */
    
code_copy_loop:
    cmp r1, r2
    bge code_copy_done
    ldr r3, [r0], #4
    str r3, [r1], #4
    b code_copy_loop
code_copy_done:

    /* 4. Настройка указателя стека */
    ldr r0, =_estack
    mov sp, r0

    /* 5. Отключение Watchdog для отладки */
    ldr r0, =0x40002C00     /* WWDG_CR */
    movs r1, #0
    str r1, [r0]

    /* 6. Переход на Reset_Handler в SRAM */
    ldr r0, =Reset_Handler
    bx r0

.size Starter_Handler, .-Starter_Handler

/* ============ RESET HANDLER (в SRAM) ============ */
.section .sram.text
.global Reset_Handler
.type Reset_Handler, %function
.thumb_func

Reset_Handler:
    /* 1. Вызов инициализации системы */
    bl SystemInit

    /* 2. Вызов конструкторов C++ (если нужно) */
    bl __libc_init_array

    /* 3. Переход в main() */
    bl main

    /* 4. Если main вернулся - бесконечный цикл */
Infinite_Loop:
    b Infinite_Loop

.size Reset_Handler, .-Reset_Handler

/* ============ ОБРАБОТЧИКИ ПРЕРЫВАНИЙ ============ */
/* Слабые обработчики по умолчанию */
.macro DEFAULT_HANDLER handler_name
    .weak \handler_name
    .thumb_set \handler_name, Default_Handler
.endm

DEFAULT_HANDLER NMI_Handler
DEFAULT_HANDLER HardFault_Handler
DEFAULT_HANDLER MemManage_Handler
DEFAULT_HANDLER BusFault_Handler
DEFAULT_HANDLER UsageFault_Handler
DEFAULT_HANDLER SVC_Handler
DEFAULT_HANDLER DebugMon_Handler
DEFAULT_HANDLER PendSV_Handler
DEFAULT_HANDLER SysTick_Handler
DEFAULT_HANDLER WWDG_IRQHandler
/* ... добавьте остальные обработчики */

/* Обработчик по умолчанию */
.section .text.Default_Handler
.weak Default_Handler
.type Default_Handler, %function
.thumb_func

Default_Handler:
    b .  /* Бесконечный цикл */

.size Default_Handler, .-Default_Handler
