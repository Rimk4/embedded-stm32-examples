/* 
 * stm32f437xx.ld - линкерный скрипт для двухэтапного загрузчика
 * Память: Flash 2M, SRAM 192K, CCMRAM 64K
 */

/* Размеры памяти */
MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 2048K
    SRAM (rwx)      : ORIGIN = 0x20000000, LENGTH = 192K
    CCMRAM (rw)     : ORIGIN = 0x10000000, LENGTH = 64K
}

/* Точка входа */
ENTRY(Starter_Handler)

/* Размер стека */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

/* Адреса секций */
_sidata = LOADADDR(.data);  /* LMA .data в Flash */

SECTIONS
{
    /* ============ ВЕКТОРЫ ПРЕРЫВАНИЙ ============ */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))      /* Вектора прерываний */
        . = ALIGN(4);
    } >FLASH

    /* ============ STARTER HANDLER (во Flash) ============ */
    .starter :
    {
        . = ALIGN(4);
        KEEP(*(.text.Starter_Handler))  /* Код загрузчика */
        . = ALIGN(4);
    } >FLASH

    /* ============ ТЕКСТ (копируется в SRAM) ============ */
    .text : AT (__etext_lma)   /* LMA указываем явно */
    {
        . = ALIGN(4);
        __stext = .;           /* Начало кода в SRAM */
        
        *(.text)               /* Код */
        *(.text*)              /* Код */
        *(.glue_7)             /* glue arm to thumb code */
        *(.glue_7t)            /* glue thumb to arm code */
        *(.eh_frame)
        
        KEEP(*(.init))
        KEEP(*(.fini))
        
        . = ALIGN(4);
        __etext = .;           /* Конец кода в SRAM */
    } >SRAM
    
    /* Адрес кода в Flash */
    __stext_lma = LOADADDR(.text);
    __etext_lma = LOADADDR(.text) + SIZEOF(.text);

    /* ============ КОНСТАНТЫ ============ */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    /* ============ ARM EXIDX ============ */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH

    .ARM :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    /* ============ ПРЕДИНИЦИАЛИЗИРОВАННЫЕ ДАННЫЕ ============ */
    .data : 
    {
        . = ALIGN(4);
        __sdata = .;           /* Начало .data в RAM */
        
        *(.data)
        *(.data*)
        
        . = ALIGN(4);
        __edata = .;           /* Конец .data в RAM */
    } >SRAM AT>FLASH

    /* ============ НЕИНИЦИАЛИЗИРОВАННЫЕ ДАННЫЕ ============ */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __sbss = .;            /* Начало .bss */
        
        *(.bss)
        *(.bss*)
        *(COMMON)
        
        . = ALIGN(4);
        __ebss = .;            /* Конец .bss */
    } >SRAM

    /* ============ SRAM КОД (Reset_Handler и т.д.) ============ */
    .sram.text :
    {
        . = ALIGN(4);
        *(.sram.text)
        *(.sram.text*)
        . = ALIGN(4);
    } >SRAM

    /* ============ ОПЕРАТИВНАЯ ПАМЯТЬ (HEAP) ============ */
    .heap (NOLOAD) :
    {
        . = ALIGN(4);
        __heap_start = .;
        . = . + 0x2000;        /* 8K heap */
        __heap_end = .;
    } >SRAM

    /* ============ СТЕК ============ */
    .stack (NOLOAD) :
    {
        . = ALIGN(4);
        __stack_start = .;
        . = . + 0x1000;        /* 4K stack */
        __stack_end = .;
        . = ALIGN(4);
    } >SRAM

    /* ============ CCMRAM ============ */
    .ccmram (NOLOAD) :
    {
        . = ALIGN(4);
        *(.ccmram)
        *(.ccmram*)
        . = ALIGN(4);
    } >CCMRAM

    /* ============ ОТЛАДОЧНАЯ ИНФОРМАЦИЯ ============ */
    /* Удаляем отладочную информацию из релизной сборки */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }
}
