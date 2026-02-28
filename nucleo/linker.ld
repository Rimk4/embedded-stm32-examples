/**
 * Простой linker script для STM32H723ZG
 * 1MB FLASH, 128KB DTCM RAM
 */

MEMORY
{
    FLASH    (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
    DTCMRAM  (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
}

ENTRY(Reset_Handler)

SECTIONS
{
    /* Vector table */
    .isr_vector : {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Program code */
    .text : {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    /* Constants */
    .rodata : {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* Data to copy to RAM */
    _sidata = LOADADDR(.data);
    
    /* Initialized data */
    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > DTCMRAM AT > FLASH

    /* Uninitialized data */
    .bss : {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } > DTCMRAM

    /* Stack and heap */
    ._user_heap_stack : {
        . = ALIGN(8);
        . = . + 0x400;   /* 1KB heap */
        . = ALIGN(8);
        . = . + 0x400;   /* 1KB stack */
        . = ALIGN(8);
        _estack = .;
    } > DTCMRAM

    /* Discard unused sections */
    /DISCARD/ : {
        libc.a (*)
        libm.a (*)
        libgcc.a (*)
    }
}
