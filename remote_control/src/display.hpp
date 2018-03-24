#ifndef display_h
#define display_h

#include "global.hpp"
#include <string>
#include <Button.h>
#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <Adafruit_SSD1306.h>
//#include <TouchScreen.h>

#define PULLUP            true
#define NO_PULLUP         false
#define INVERT            false
#define DEBOUNCE_MS       10

struct event_t {
  uint8_t ID;
  String name;
};

struct menu_t {
  uint8_t curIndex;
  uint8_t nbMenu;
  uint8_t menuSelected;
};

void display_init(void);
void display_main(void);


#endif /* display_h */
