#include "display.hpp"

Button btnAction(BTN_ACTION_PIN, NO_PULLUP, INVERT, DEBOUNCE_MS);
Button btnPlus(BTN_PLUS_PIN, NO_PULLUP, INVERT, DEBOUNCE_MS);
Button btnMoins(BTN_MOINS_PIN, NO_PULLUP, INVERT, DEBOUNCE_MS);
//Adafruit_TFTLCD tft_disp(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
Adafruit_SSD1306 display(SQUARE_SC_RESET_PIN);
unsigned long displayNextTick;
//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

struct menu_t menu;
struct event_t event;

void display_init(void)
{
  menu.menuSelected = 0;
  menu.curIndex = 0;
  menu.nbMenu = 10;
  display.begin(SSD1306_SWITCHCAPVCC, SQUARE_SC_ADDR);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  digitalWrite(LED_R_PIN, 0);
  digitalWrite(LED_G_PIN, 0);
  digitalWrite(LED_B_PIN, 0);
  Serial.println("Display " + String(SQUARE_SC_W) + "x" + String(SQUARE_SC_H));
}

void display_btn_manage(void)
{
  btnPlus.read();
  btnAction.read();
  btnMoins.read();

  if(btnPlus.wasPressed()) {
    btnPlus.isPressed(); // Clear flag
    if(menu.menuSelected) {
      menu.menuSelected = 0;
      // YES
    } else {
      if(menu.curIndex < (menu.nbMenu - 1)) {
        menu.curIndex++;
      } else {
        menu.curIndex = 0;
      }
    }
  }
  if(btnMoins.wasPressed()) {
    btnMoins.isPressed(); // Clear flag
    if(menu.menuSelected) {
      menu.menuSelected = 0;
      // NO
    } else {
      if(menu.curIndex > 0) {
        menu.curIndex--;
      } else {
        menu.curIndex = (menu.nbMenu - 1);
      }
    }
  }
  if(btnAction.wasPressed()) {
    btnAction.isPressed(); // Clear flag
    Serial.print("menu_index: ");
    Serial.println(menu.curIndex);
    menu.menuSelected = 1;
  }
}

void display_draw_event(struct event_t event)
{
  Serial.println("[display.cpp] Printing event " + String(event.ID) + " : " + event.name);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(event.name);
  display.fillRect(0, 0, SQUARE_SC_W-1, SQUARE_SC_H-1, WHITE);
  display.setCursor(1,1);

  display.display();
}

void display_main(void)
{
  display_btn_manage();

  event.ID = 1;
  event.name = "BBQ";

  if (millis() > displayNextTick) {
    display_draw_event(event);
    displayNextTick = millis() + 3000;
  }

}
