TARGET = nucleo_h723_bridge
BUILD_DIR = build

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

MCU = -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard

C_DEFS = \
    -DSTM32H723xx \
    -DUSE_HAL_DRIVER \
    -DCFG_TUSB_MCU=OPT_MCU_STM32H7

C_INCLUDES = \
    -Iinc \
    -IDrivers/CMSIS/Core/Include \
    -IDrivers/CMSIS/Device/Include \
    -IDrivers/STM32H7xx_HAL_Driver/Inc \
    -Itinyusb/src \
    -Itinyusb/lib/networking

# Исходные файлы C (добавлен src/startup.c, убран system_stm32h7xx.c)
C_SOURCES = \
    src/startup.c \
    src/main.c \
    src/usb_descriptors.c \
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c \
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c \
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c \
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
    Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c \
    tinyusb/src/tusb.c \
    tinyusb/src/common/tusb_fifo.c \
    tinyusb/src/device/usbd.c \
    tinyusb/src/class/net/ecm_rndis_device.c \
    tinyusb/src/portable/synopsys/dwc2/dwc2_common.c \
    tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.c

# Ассемблерные файлы больше не нужны
ASM_SOURCES = 

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) -O2 -g3 -Wall -fdata-sections -ffunction-sections

LDSCRIPT = STM32H723ZGT6.ld
LDFLAGS = $(MCU) -T$(LDSCRIPT) --specs=nano.specs --specs=nosys.specs \
          -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -lc -lm

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@echo "CC $<"
	@$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo "LD $@"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(CP) -O binary -S $< $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(CP) -O ihex $< $@

$(BUILD_DIR):
	mkdir $@

clean:
	rm -rf $(BUILD_DIR)

flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash --connect-under-reset --reset write $(BUILD_DIR)/$(TARGET).bin 0x08000000

.PHONY: all clean flash
