#include "tusb.h"

// VID/PID (тестовый Realtek-совместимый или ST)
#define USB_VID   0xcafe
#define USB_PID   0x4002

tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = TUSB_CLASS_CDC,
    .bDeviceSubClass    = CDC_COMM_SUBCLASS_ETHERNET_CONTROL_MODEL,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = USB_VID,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

uint8_t const * tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device;
}

enum {
    ITF_NUM_CDC_CONTROL = 0,
    ITF_NUM_CDC_DATA,
    ITF_NUM_TOTAL
};

#define EPNUM_NET_NOTIF   0x81
#define EPNUM_NET_OUT     0x02
#define EPNUM_NET_IN      0x82

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_CDC_ECM_DESC_LEN)

uint8_t const desc_configuration[] = {
    // Конфигурационный дескриптор
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0, 100),

    // CDC-ECM дескриптор: (itfnum, desc_stridx, mac_stridx, ep_notif, ep_notif_size, epout, epin, epsize, maxsegmentsize)
    TUD_CDC_ECM_DESCRIPTOR(ITF_NUM_CDC_CONTROL, 0, 4, EPNUM_NET_NOTIF, 64, EPNUM_NET_OUT, EPNUM_NET_IN, CFG_TUD_NET_ENDPOINT_SIZE, CFG_TUD_NET_MTU)
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    (void) index;
    return desc_configuration;
}

// Строковые дескрипторы (включая виртуальный MAC-адрес)
char const* string_desc_arr[] = {
    (const char[]) { 0x09, 0x04 }, // 0: English
    "TinyUSB",                     // 1: Manufacturer
    "STM32H7 Bridge",              // 2: Product
    "1234567890AB",                // 3: Serial
    "001A2B3C4D5E",                // 4: MAC Address для Ubuntu (12 hex символов)
};

static uint16_t _desc_str[32];
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void) langid;
    uint8_t count;

    if (index == 0) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        count = 1;
    } else {
        if (index >= sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) return NULL;
        const char* str = string_desc_arr[index];
        count = strlen(str);
        for(uint8_t i=0; i<count; i++) _desc_str[1+i] = str[i];
    }

    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * count + 2);
    return _desc_str;
}

// MAC-адрес сетевого интерфейса (без const)
uint8_t tud_network_mac_address[6] = { 0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E };
