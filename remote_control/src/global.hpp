#include "Arduino.h"

#define BOOTLOADER_VERSION    "Bootloader v1.0"
#define FIRMWARE_VERSION      "RemoteControl v1.0"
#define USER_NAME             "david"

#define WIFI_SSID             "iPhone de David"
#define WIFI_PWD              "david-pi"
#define OTA_PWD               "macintosh"
#define OTA_PORT              8266
#define REFRESH_PERIODE       10000

// Touch screen (SPI)
#define TOUCH_SC_SPI_SCK      0

// Buttons
#define BTN_PLUS_PIN          10
#define BTN_ACTION_PIN        5
#define BTN_MOINS_PIN         16

// RGB LED
#define LED_R_PIN             12
#define LED_G_PIN             15
#define LED_B_PIN             13

// Square screen (I2C)
#define SQUARE_SC_RESET_PIN     4
#define SQUARE_SC_ADDR          0x3C
#define SQUARE_SC_W             128
#define SQUARE_SC_H             96

// Long screen (I2C)
#define LONG_SC_RESET_PIN      4
#define LONG_SC_ADDR           0x3C
#define LONG_SC_W              128
#define LONG_SC_H              32

// Bootloader
void bootloader_init(void);
void bootloader_main(void);

// Web
void web_init(void);
void web_main(void);
