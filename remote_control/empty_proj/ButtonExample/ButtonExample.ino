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

static uint16_t prev_x = 0xffff, prev_y = 0xffff;

/* ######### Partie à copier coller */
struct button_t {
  int x;
  int y;
  int h;
  int w;
  char* label;
  uint16_t defColor;
  uint16_t pressedColor;
};

int isButtonPressed(button_t* b) {
  if (!touch.isTouching()) return 0;
  
  uint16_t x, y;
  touch.getPosition(x, y);

  // Si on est hors du cadre, on retourne 0, sinon 1
  return !(x < b->x || y < b->y || x > b->x + b->w || y > b->y + b->h);
}

void drawButton(button_t* b, int pressed) {
  ucg.fillRect(b->x, b->y, b->w, b->h, pressed ? b->pressedColor : b->defColor);
  ucg.drawRect(b->x, b->y, b->w, b->h, WHITE);
  ucg.setTextSize(2);
  ucg.setTextColor(WHITE);
  ucg.setCursor(b->x + b->w/2 - strlen(b->label) * 12/2, b->y + (b->h/2) - 10); 
  ucg.print(b->label);
}

/* ######### Fin de partie à copier coller */

button_t ButtonAccept;
button_t ButtonRefuse;
button_t ButtonText;
  
void loop() {
  static int wasPressed = 0;
  int isPressed = isButtonPressed(&ButtonAccept);
  
  if(isPressed != wasPressed) { 
    drawButton(&ButtonAccept, isPressed);
  }
  wasPressed = isPressed;

  static int RefuseWasPressed = 0;
  int RefuseIsPressed = isButtonPressed(&ButtonRefuse);
  
  if(RefuseIsPressed != RefuseWasPressed) { 
    drawButton(&ButtonRefuse, RefuseIsPressed);
  }
  RefuseWasPressed = RefuseIsPressed;
  delay(10);
}


void setup() {
  delay(1000);
  ucg.begin();
  touch.begin(240, 320);  // Must be done before setting rotation
  ucg.setRotation(3);
  touch.setRotation(touch.ROT270);
  ucg.fillScreen(ILI9341_BLACK);

  // Replace these for your screen module
  touch.setCalibration(209, 1759, 1775, 273);

  ButtonAccept.x = 320/2 + 5;
  ButtonAccept.y = 240/2+5;
  ButtonAccept.h = 240/2-10;
  ButtonAccept.w = 320/2-10;
  ButtonAccept.defColor = DARKGREEN;
  ButtonAccept.pressedColor = BLUE;
  ButtonAccept.label = "Accept";
  
  ButtonRefuse.x = 5;
  ButtonRefuse.y = 240/2+5;
  ButtonRefuse.h = 240/2-10;
  ButtonRefuse.w = 320/2-10;
  ButtonRefuse.defColor = DARKRED;
  ButtonRefuse.pressedColor = BLUE;
  ButtonRefuse.label = "Refuse";
}

