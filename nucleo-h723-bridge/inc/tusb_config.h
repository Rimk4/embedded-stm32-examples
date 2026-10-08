#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#define CFG_TUSB_MCU                OPT_MCU_STM32H7
#define CFG_TUSB_OS                 OPT_OS_NONE

// Разрешаем стек устройства
#define CFG_TUD_ENABLED             1

// Разрешаем сетевой класс CDC-ECM
#define CFG_TUD_NET                 1
#define CFG_TUD_ECM_RNDIS           1
#define CFG_TUD_RNDIS_TEMPLATE      0
#define CFG_TUD_NCM                 0

// КРИТИЧНО ДЛЯ H723: переключаем с несуществующего FS (порт 0) на контроллер OTG_HS (порт 1)
#define BOARD_DEVICE_RHPORT_NUM     1
#define CFG_TUSB_RHPORT1_MODE       (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)

#define CFG_TUD_ENDPOINT0_SIZE      64
#define CFG_TUD_NET_ENDPOINT_SIZE   64
#define CFG_TUD_NET_MTU             1514

#endif
