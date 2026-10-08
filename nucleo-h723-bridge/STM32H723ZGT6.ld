ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM   (xrw)     : ORIGIN = 0x20000000, LENGTH = 128K  /* DTCM: всегда доступна на старте! */
    SRAM_D2 (xrw)   : ORIGIN = 0x30000000, LENGTH = 32K   /* Для дескрипторов Ethernet DMA */
}

SECTIONS
{
    .isr_vector : {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    .text : {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    _sidata = LOADADDR(.data);

    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT> FLASH

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
    } > RAM

    ._user_heap_stack : {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + 0x1000; /* Heap */
        . = . + 0x1000; /* Stack */
        . = ALIGN(8);
    } > RAM

    .eth_dma (NOLOAD) : {
        . = ALIGN(4);
        *(.eth_dma*)
        . = ALIGN(4);
    } > SRAM_D2

    /DISCARD/ : {
        libc.a (*)
        libm.a (*)
        libgcc.a (*)
    }
}
