#!/usr/bin/env bash
set -e

echo "=== Загрузка CMSIS, HAL и TinyUSB ==="
mkdir -p Drivers/CMSIS Drivers/STM32H7xx_HAL_Driver

# CMSIS Core (ARM)
if [ ! -d "Drivers/CMSIS/Core" ]; then
    git clone --depth 1 https://github.com/ARM-software/CMSIS_5.git /tmp/cmsis5
    mv /tmp/cmsis5/CMSIS/Core Drivers/CMSIS/Core
    rm -rf /tmp/cmsis5
fi

# CMSIS Device STM32H7
if [ ! -d "Drivers/CMSIS/Device" ]; then
    git clone --depth 1 https://github.com/STMicroelectronics/cmsis_device_h7.git Drivers/CMSIS/Device
fi

# STM32H7xx HAL Driver
if [ ! -d "Drivers/STM32H7xx_HAL_Driver/Src" ]; then
    git clone --depth 1 https://github.com/STMicroelectronics/stm32h7xx_hal_driver.git Drivers/STM32H7xx_HAL_Driver
fi

# TinyUSB stack
if [ ! -d "tinyusb" ]; then
    git clone --depth 1 https://github.com/hathach/tinyusb.git tinyusb
fi

echo "=== Все библиотеки успешно загружены! ==="
