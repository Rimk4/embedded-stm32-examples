#ifndef __STM32H7xx_HAL_CONF_H
#define __STM32H7xx_HAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

#define HAL_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_ETH_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED

// Опорные частоты генераторов
#if !defined (HSE_VALUE)
  #define HSE_VALUE            ((uint32_t)8000000UL) 
#endif
#if !defined (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT  ((uint32_t)5000U)
#endif
#if !defined (HSI_VALUE)
  #define HSI_VALUE            ((uint32_t)64000000UL)
#endif
#if !defined (CSI_VALUE)
  #define CSI_VALUE            ((uint32_t)4000000UL) 
#endif
#if !defined (LSI_VALUE)
  #define LSI_VALUE            ((uint32_t)32000UL)   
#endif
#if !defined (LSE_VALUE)
  #define LSE_VALUE            ((uint32_t)32768UL)   
#endif
#if !defined (LSE_STARTUP_TIMEOUT)
  #define LSE_STARTUP_TIMEOUT  ((uint32_t)5000U)
#endif
#if !defined (EXTERNAL_CLOCK_VALUE)
  #define EXTERNAL_CLOCK_VALUE ((uint32_t)12288000UL)
#endif
#if !defined (VDD_VALUE)
  #define VDD_VALUE            ((uint32_t)3300U)     
#endif

#define TICK_INT_PRIORITY      ((uint32_t)0U) 
#define USE_RTOS               0U
#define PREFETCH_ENABLE        0U
#define USE_HAL_ETH_REGISTER_CALLBACKS  0U

#include "stm32h7xx_hal_rcc.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_dma.h"
#include "stm32h7xx_hal_cortex.h"
#include "stm32h7xx_hal_pwr.h"
#include "stm32h7xx_hal_eth.h"
#include "stm32h7xx_hal_flash.h"

#define assert_param(expr) ((void)0U)

#ifdef __cplusplus
}
#endif

#endif /* __STM32H7xx_HAL_CONF_H */
