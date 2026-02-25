```
stm32-blink/
├── src/
│   ├── main.c
│   ├── gpio.c
│   ├── gpio.h
│   ├── delay.c
│   ├── delay.h
│   ├── stm32f4_hal.h
│   └── startup.c
├── linker/
│   └── stm32f407.ld
├── Makefile
└── README.md
```

# Проект мигания светодиодом для STM32

## Требования
- ARM GCC toolchain (`arm-none-eabi-gcc`)
- OpenOCD
- ST-Link программатор
- Плата STM32F4-Discovery или аналогичная


# Отладка через GDB с st-flash:
```bash
# В терминале 1: Запустите GDB сервер от st-flash
st-util

# В терминале 2: Подключитесь через GDB
arm-none-eabi-gdb build/stm32-blink.elf \
    -ex "target remote localhost:4242" \
    -ex "monitor reset halt" \
    -ex "load" \
    -ex "b main" \
    -ex "continue"
```