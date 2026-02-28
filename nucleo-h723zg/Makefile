# Makefile для сборки проекта STM32H723ZGT6

# Настройки проекта
PROJECT_NAME = nucleo-h723-led-blink
MCU = cortex-m7
FPU = fpv5-d16

# Директории
SRC_DIR = src
INC_DIR = inc
LD_DIR = ld
BUILD_DIR = build

# Инструменты
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
SIZE = $(PREFIX)size
GDB = $(PREFIX)gdb
OPENOCD = openocd

# Флаги компиляции
CFLAGS = -mcpu=$(MCU) -mthumb -mfloat-abi=hard -mfpu=$(FPU)
CFLAGS += -DSTM32H723xx -DUSE_HAL_DRIVER
CFLAGS += -O0 -g3 -Wall -fmessage-length=0
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += --specs=nano.specs --specs=nosys.specs
CFLAGS += -I$(INC_DIR)

# Флаги линковки
LDFLAGS = -mcpu=$(MCU) -mthumb -mfloat-abi=hard -mfpu=$(FPU)
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(PROJECT_NAME).map
LDFLAGS += -T$(LD_DIR)/STM32H723ZGT6.ld --specs=nano.specs

# Исходные файлы
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/led.c \
       $(SRC_DIR)/delay.c \
       $(SRC_DIR)/uart.c \
       $(SRC_DIR)/startup.c

# Объектные файлы (в build директории)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Создание директории build если её нет
$(shell mkdir -p $(BUILD_DIR))

# Цели по умолчанию
all: $(BUILD_DIR)/$(PROJECT_NAME).elf $(BUILD_DIR)/$(PROJECT_NAME).bin $(BUILD_DIR)/$(PROJECT_NAME).hex

# Компиляция .c в .o в build директории
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Линковка
$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

# Создание .bin файла
$(BUILD_DIR)/$(PROJECT_NAME).bin: $(BUILD_DIR)/$(PROJECT_NAME).elf
	$(OBJCOPY) -O binary $< $@

# Создание .hex файла
$(BUILD_DIR)/$(PROJECT_NAME).hex: $(BUILD_DIR)/$(PROJECT_NAME).elf
	$(OBJCOPY) -O ihex $< $@

# Прошивка через OpenOCD (ST-Link)
flash: $(BUILD_DIR)/$(PROJECT_NAME).elf
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32h7x.cfg -c "program $(BUILD_DIR)/$(PROJECT_NAME).elf verify reset exit"

# Очистка
clean:
	rm -rf $(BUILD_DIR)

# Полная пересборка
rebuild: clean all

# Отладка через GDB
debug: $(BUILD_DIR)/$(PROJECT_NAME).elf
	$(GDB) $< -ex "target remote localhost:4242" \
	-ex "monitor reset halt" \
	-ex "break main" \
	-ex "continue"

# Показать размеры файлов
size: $(BUILD_DIR)/$(PROJECT_NAME).elf
	$(SIZE) $<

# Монитор последовательного порта (для Linux)
monitor:
	picocom /dev/ttyACM0 -b 115200

# Справка
help:
	@echo "Доступные цели:"
	@echo "  all      - Собрать проект (по умолчанию)"
	@echo "  clean    - Очистить сборку"
	@echo "  rebuild  - Полная пересборка"
	@echo "  flash    - Прошить устройство"
	@echo "  debug    - Запустить отладку"
	@echo "  size     - Показать размеры"
	@echo "  monitor  - Открыть монитор порта (Linux)"

.PHONY: all clean rebuild flash debug size help monitor
