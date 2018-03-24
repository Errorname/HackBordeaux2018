#include "Arduino.h"

#define BOOTLOADER_VERSION    "Bootloader v1.0"
#define FIRMWARE_VERSION      "RemoteControl v1.0"

#define WIFI_SSID             "iPhone de David"
#define WIFI_PWD              "david-pi"
#define OTA_PWD               "macintosh"
#define OTA_PORT              8266
#define SPI_SS_PIN            4
#define SPI_RST_PIN           5

// Bootloader
void bootloader_init(void);
void bootloader_main(void);

// Web Server
void web_server_init(void);
void web_server_main(void);
String getContentType(String filename);
bool handle_file_read(String path);
bool handle_clear_picc(void);
bool handle_ask_updates(void);
