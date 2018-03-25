#include "Arduino.h"

#define BOOTLOADER_VERSION    "Bootloader v1.0"
#define FIRMWARE_VERSION      "RemoteControl v1.0"
#define USER_NAME             "aurelien"

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


#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF
#define DARKGREY 0x2084
#define DARKGREEN 0x0200
#define DARKRED  0x4000

// Bootloader
void bootloader_init(void);
void bootloader_main(void);

// Web
void web_main(void);
void web_erase_event(int event_index);
void web_send_interrest(int event_index);
int web_get_nb_event(void);
void web_init(void);
