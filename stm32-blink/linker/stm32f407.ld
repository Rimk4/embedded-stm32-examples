/**
 * Linker script для STM32F407VG
 * Flash: 1MB (0x08000000 - 0x08100000)
 * RAM:   192KB (0x20000000 - 0x20030000)
 */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1M
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 192K
}

SECTIONS
{
    /* Таблица векторов в начале flash */
    .isr_vector : {
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH
    
    /* Код программы */
    .text : {
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } > FLASH
    
    /* Инициализированные данные (копируются в RAM при старте) */
    .data : AT (_etext) {
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM
    
    /* Неинициализированные данные */
    .bss : {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM
    
    /* Стек */
    .stack : {
        . = ALIGN(8);
        _sstack = .;
        . = . + 0x1000;  /* 4KB stack */
        _estack = .;
    } > RAM
    
    /* Конец образа */
    _end = .;
}
