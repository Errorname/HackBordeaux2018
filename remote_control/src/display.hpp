#if 0
#ifndef display_h
#define display_h

#include "global.hpp"
#include <string>
#include <Button.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <brzo_i2c.h> // I2C
#include "SSD1306Brzo.h" //I2C for OLED Display

#define PULLUP            true
#define NO_PULLUP         false
#define INVERT            false
#define DEBOUNCE_MS       10

#define LEAP_YEAR(_year) ((_year%4)==0)

struct event_t {
  String id;
  String comment;
  uint32_t createdAt;
  String location;
  String owner;
  uint32_t timestamp;
  String type;
};

struct menu_t {
  uint8_t curIndex;
  uint8_t nbMenu;
  uint8_t menuSelected;
};

void display_init(void);
void display_main(void);


#endif /* display_h */
#endif
