# Makefile для STM32 двухэтапного загрузчика
# Использование: make [DEBUG=1] [FLASH=1]

# ============ КОНФИГУРАЦИЯ ============
# Модель процессора
MCPU = cortex-m4
FPU = fpv4-sp-d16
FLOAT_ABI = hard

# Пути
BUILD_DIR = build
SRC_DIR = src
INC_DIR = include
LINKER_SCRIPT = stm32f437xx.ld

# Целевой файл
TARGET = firmware
ELF_FILE = $(BUILD_DIR)/$(TARGET).elf
BIN_FILE = $(BUILD_DIR)/$(TARGET).bin
HEX_FILE = $(BUILD_DIR)/$(TARGET).hex

# ============ ИСХОДНЫЕ ФАЙЛЫ ============
# Ассемблерные файлы
AS_SOURCES = \
    $(SRC_DIR)/libs/libktrboot/stm32/starter.S \
    $(SRC_DIR)/libs/libktrboot/stm32/startup_stm32f437xx.S

# C файлы
C_SOURCES = \
    $(SRC_DIR)/apps/bootloader/main.c \
    $(SRC_DIR)/libs/cmsis/system_stm32f4xx.c \
    $(SRC_DIR)/drivers/stm32f4xx_hal.c

# ============ КОМПИЛЯТОРЫ ============
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
LD = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
SIZE = $(PREFIX)size

# ============ ФЛАГИ КОМПИЛЯЦИИ ============
# Общие флаги
COMMON_FLAGS = -mcpu=$(MCPU) -mthumb -mfloat-abi=$(FLOAT_ABI) -mfpu=$(FPU)
COMMON_FLAGS += -ffunction-sections -fdata-sections
COMMON_FLAGS += -I$(INC_DIR) -I$(SRC_DIR)/libs/cmsis

# Флаги отладки
ifeq ($(DEBUG),1)
    COMMON_FLAGS += -O0 -g3 -gdwarf-2
else
    COMMON_FLAGS += -O2
endif

# Специфичные флаги
CFLAGS = $(COMMON_FLAGS) -std=c11 -Wall -Wextra
ASFLAGS = $(COMMON_FLAGS) -x assembler-with-cpp
LDFLAGS = $(COMMON_FLAGS) -T$(LINKER_SCRIPT) -Wl,--gc-sections
LDFLAGS += -Wl,--entry=Starter_Handler -nostdlib -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

# ============ ПРАВИЛА СБОРКИ ============
# Объектные файлы
AS_OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(AS_SOURCES:.S=.o)))
C_OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
OBJECTS = $(AS_OBJECTS) $(C_OBJECTS)

# VPATH для поиска исходников
vpath %.S $(sort $(dir $(AS_SOURCES)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# Основная цель
all: $(BUILD_DIR) $(ELF_FILE) $(BIN_FILE) $(HEX_FILE) size

# Создание директории сборки
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Компиляция ассемблерных файлов
$(BUILD_DIR)/%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@

# Компиляция C файлов
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Линковка
$(ELF_FILE): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

# Генерация бинарного файла
$(BIN_FILE): $(ELF_FILE)
	$(OBJCOPY) -O binary $< $@

# Генерация HEX файла
$(HEX_FILE): $(ELF_FILE)
	$(OBJCOPY) -O ihex $< $@

# Просмотр размера
size: $(ELF_FILE)
	$(SIZE) --format=berkeley $<

# Диссекция
disasm: $(ELF_FILE)
	$(OBJDUMP) -D $< > $(BUILD_DIR)/$(TARGET).disasm

# Очистка
clean:
	rm -rf $(BUILD_DIR)

# Прошивка через ST-Link
flash: $(BIN_FILE)
	st-flash --reset write $< 0x08000000

# Отладка через GDB
debug: $(ELF_FILE)
	arm-none-eabi-gdb -ex "target remote :4242" \
	                  -ex "monitor reset halt" \
	                  -ex "load" \
	                  -ex "break main" \
	                  $<

# Список символов
symbols: $(ELF_FILE)
	$(PREFIX)nm -S -n $< | grep -E "(Starter_Handler|Reset_Handler|main)"

# Справка
help:
	@echo "Доступные цели:"
	@echo "  make all       - полная сборка"
	@echo "  make DEBUG=1   - сборка с отладочной информацией"
	@echo "  make clean     - очистка"
	@echo "  make flash     - прошивка через ST-Link"
	@echo "  make debug     - запуск отладки"
	@echo "  make disasm    - генерация листинга ассемблера"
	@echo "  make symbols   - список символов"

.PHONY: all clean flash debug disasm size help symbols
