#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <SPI.h>

#include <TFT_eSPI.h> // LCD lib for ESP
#include <XPT2046.h>  // Touch lib for ESP

#define WHITE 0xFFFF

// Modify the following two lines to match your hardware
// Also, update calibration parameters below, as necessary
TFT_eSPI ucg = TFT_eSPI();       // Invoke custom library
XPT2046 touch(/*cs=*/ PIN_D2, /*irq=*/ PIN_D1);

void setup() {
  delay(1000);
  ucg.begin();
  touch.begin(240, 320);  // Must be done before setting rotation
  ucg.setRotation(3);
  touch.setRotation(touch.ROT270);
  ucg.fillScreen(ILI9341_BLACK);

  // Replace these for your screen module
  touch.setCalibration(209, 1759, 1775, 273);
}

static uint16_t prev_x = 0xffff, prev_y = 0xffff;

void loop() {
  static int i = 0;
  ucg.setCursor(0, 0);
  ucg.print(i++);
  if (touch.isTouching()) {
    uint16_t x, y;
    touch.getPosition(x, y);
    ucg.setCursor(0, 20);
    ucg.print(x);
    ucg.setCursor(0, 28);
    ucg.print(y);
    ucg.drawPixel(x, y, 0xFFFF);
    
    if (prev_x == 0xffff) {
      ucg.drawPixel(x, y, WHITE);
    } else {
      ucg.drawLine(prev_x, prev_y, x, y, WHITE);
    }
    prev_x = x;
    prev_y = y;
  } else {
    prev_x = prev_y = 0xffff;
  }
  delay(20);
}
