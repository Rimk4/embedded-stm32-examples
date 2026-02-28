# Простой Makefile для NUCLEO-H723ZG

TARGET = nucleo-blink
MCU = cortex-m7
FPU = fpv5-d16

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
SIZE = $(PREFIX)size
STFLASH = st-flash
RM = rm -f
MKDIR = mkdir -p

BUILD_DIR = build

CFLAGS = -mcpu=$(MCU) -mthumb
CFLAGS += -mfloat-abi=hard -mfpu=$(FPU)
CFLAGS += -Os -Wall -Wextra
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -nostdlib -nostartfiles
CFLAGS += -DSTM32H723xx
CFLAGS += -I.
CFLAGS += -g3 -ggdb3  # Отладочная информация для GDB

LDFLAGS = -mcpu=$(MCU) -mthumb
LDFLAGS += -mfloat-abi=hard -mfpu=$(FPU)
LDFLAGS += -Tlinker.ld
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -nostdlib -nostartfiles

SOURCES = main.c
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.c=.o))

# Цвета
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
	@$(SIZE) $(BUILD_DIR)/$(TARGET).elf
	@echo "$(GREEN)✅ Build complete!$(NC)"

$(BUILD_DIR):
	$(MKDIR) $@

$(BUILD_DIR)/%.o: %.c
	@echo "$(YELLOW)🔨 Compiling $<...$(NC)"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@echo "$(YELLOW)🔗 Linking...$(NC)"
	$(CC) $^ $(LDFLAGS) -o $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	@echo "$(GREEN)✅ Binary size: $$(stat -c%s $@) bytes$(NC)"

clean:
	$(RM) -r $(BUILD_DIR)
	@echo "$(GREEN)✅ Clean complete!$(NC)"

# Прошивка через st-flash
stflash: $(BUILD_DIR)/$(TARGET).bin
	@echo "$(YELLOW)🚀 Programming with st-flash...$(NC)"
	$(STFLASH) --reset write $< 0x08000000
	@echo "$(GREEN)✅ Programming complete!$(NC)"

# Отладка: подключение GDB к запущенному st-util серверу
# Предварительно запустите в отдельном окне: st-util
debug:
	@echo "$(YELLOW)🐛 Connecting GDB to st-util server...$(NC)"
	@echo "$(GREEN)📝 Make sure st-util is running in another terminal: st-util$(NC)\n"
	$(PREFIX)gdb $(BUILD_DIR)/$(TARGET).elf -ex "target remote localhost:4242" \
	                                          -ex "monitor reset halt" \
	                                          -ex "break main" \
	                                          -ex "continue"

.PHONY: all clean stflash debug
