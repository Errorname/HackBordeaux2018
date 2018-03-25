
#ifndef display_2_h
#define display_2_h

#include "global.hpp"
#include <string>
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <SPI.h>

#include <TFT_eSPI.h> // LCD lib for ESP
#include <XPT2046.h>  // Touch lib for ESP

#define LEAP_YEAR(_year) ((_year%4)==0)

struct button_t {
  int x;
  int y;
  int h;
  int w;
  char* label;
  uint16_t defColor;
  uint16_t pressedColor;
};

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

void display_2_init(void);
void display_2_main(void);


#endif /* display_2_h */

