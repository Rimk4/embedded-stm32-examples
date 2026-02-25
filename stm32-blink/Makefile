# Makefile для сборки STM32 проекта

# Настройки проекта
TARGET = stm32-blink
MCU = cortex-m4
FPU = fpv4-sp-d16

# Инструменты
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
SIZE = $(PREFIX)size
GDB = $(PREFIX)gdb
OPENOCD = openocd
STFLASH = st-flash
RM = rm -f
MKDIR = mkdir -p

# Директории
SRC_DIR = src
LINKER_DIR = linker
BUILD_DIR = build

# Исходные файлы
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/gpio.c \
          $(SRC_DIR)/delay.c \
          $(SRC_DIR)/startup.c

OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Базовые флаги для Cortex-M4 с FPU
BASE_FLAGS = -mcpu=$(MCU) -mthumb
BASE_FLAGS += -mfloat-abi=hard -mfpu=$(FPU)

# Флаги компиляции - ДОБАВЛЕН -g для отладки
CFLAGS = $(BASE_FLAGS)
CFLAGS += -Og -Wall -Wextra  # -Og для оптимизации с сохранением отладки
CFLAGS += -g3                # Максимальная отладочная информация
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -nostdlib -nostartfiles
CFLAGS += -I$(SRC_DIR)       # Для поиска заголовочных файлов
CFLAGS += -MMD -MP

# Флаги линковки
LDFLAGS = $(BASE_FLAGS)
LDFLAGS += -T $(LINKER_DIR)/stm32f407.ld
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -specs=nano.specs
LDFLAGS += -Wl,--print-memory-usage

# Цвета
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
NC = \033[0m

.PHONY: all clean flash stflash openocd debug info check help

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
	@echo "$(GREEN)✅ Сборка завершена$(NC)"

help:
	@echo "$(BLUE)Доступные цели:$(NC)"
	@echo "  make all     - собрать проект"
	@echo "  make clean   - очистить"
	@echo "  make flash   - прошить через OpenOCD"
	@echo "  make stflash - прошить через st-flash"
	@echo "  make debug   - отладка через GDB"
	@echo "  make info    - информация о ELF"

$(BUILD_DIR):
	@$(MKDIR) $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "$(YELLOW)🔨 Компиляция: $<$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@echo "$(YELLOW)🔗 Линковка: $@$(NC)"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(SIZE) $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	@$(OBJCOPY) -O binary $< $@
	@echo "$(GREEN)✅ BIN: $$(stat -c%s $@) байт$(NC)"

clean:
	@$(RM) -r $(BUILD_DIR)
	@echo "$(GREEN)✅ Очищено$(NC)"

stflash: $(BUILD_DIR)/$(TARGET).bin
	@$(STFLASH) --reset write $(BUILD_DIR)/$(TARGET).bin 0x08000000

flash: $(BUILD_DIR)/$(TARGET).bin
	openocd -f interface/stlink.cfg \
	        -f target/stm32f4x.cfg \
	        -c "program $(BUILD_DIR)/$(TARGET).bin 0x08000000 verify reset exit"

debug: $(BUILD_DIR)/$(TARGET).elf
	@echo "$(YELLOW)Запуск GDB...$(NC)"
	$(GDB) -ex "target remote localhost:4242" \
	       -ex "monitor reset halt" \
	       -ex "load" \
	       -ex "b main" \
	       -ex "continue" \
	       $(BUILD_DIR)/$(TARGET).elf

info: $(BUILD_DIR)/$(TARGET).elf
	$(PREFIX)readelf -h $<
	$(OBJDUMP) -h $<

-include $(OBJECTS:.o=.d)
