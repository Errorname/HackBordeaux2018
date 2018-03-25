#if 0
#include "display.hpp"

SSD1306Brzo  display(0x3c, 0, 14); // SDA, SCL
extern std::vector<struct event_t> eventsTab;

Button btnAction(BTN_ACTION_PIN, NO_PULLUP, INVERT, DEBOUNCE_MS);
Button btnPlus(BTN_PLUS_PIN, NO_PULLUP, INVERT, DEBOUNCE_MS);
Button btnMoins(BTN_MOINS_PIN, NO_PULLUP, INVERT, DEBOUNCE_MS);
unsigned long displayNextTick;

static  byte monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char monthNames_P[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

struct menu_t menu;
struct event_t event;

void display_init(void)
{
  menu.menuSelected = 0;
  menu.curIndex = 0;
  menu.nbMenu = 10;

  display.init();
  display.setContrast(255);
  display.setFont(ArialMT_Plain_16);
  display.setLogBuffer(3, 20);
  display.clear();
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  digitalWrite(LED_R_PIN, 0);
  digitalWrite(LED_G_PIN, 0);
  digitalWrite(LED_B_PIN, 0);

  Serial.println("[display_2.cpp] Small Screen");
}

void getTime(uint32_t *timep, int *pmin, int *phour, int *pday, int *pmonth) {
  // convert the given time_t to time components
  // this is a more compact version of the C library localtime function
  time_t epoch = *timep;
  byte year;
  byte month, monthLength;
  unsigned long days;
  byte pwday;
  byte pyear;
  *pmin = epoch % 60;
  epoch /= 60; // now it is hours
  *phour = epoch % 24;
  epoch /= 24; // now it is days

  pwday = (epoch + 4) % 7;

  year = 70;
  days = 0;
  while ((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= epoch) {
    year++;
  }
  pyear = year; // *pyear is returned as years from 1900

  days -= LEAP_YEAR(year) ? 366 : 365;
  epoch -= days; // now it is days in this year, starting at 0
  //*pdayofyear=epoch;  // days since jan 1 this year

  days = 0;
  month = 0;
  monthLength = 0;
  for (month = 0; month < 12; month++) {
    if (month == 1) { // february
      if (LEAP_YEAR(year)) {
        monthLength = 29;
      } else {
        monthLength = 28;
      }
    } else {
      monthLength = monthDays[month];
    }

    if (epoch >= monthLength) {
      epoch -= monthLength;
    } else {
      break;
    }
  }
  *pmonth = month; // jan is month 0
  *pday = epoch + 1; // day of month
}

void display_draw_event(struct event_t event)
{
  static int i = 0, sens = 1;
  char sevent[20];
  char own[20];
  char location[20];
  sprintf(sevent, "event: %s", event.id.c_str());
  sprintf(own, "own: %s", event.owner.c_str());
  strcpy(location, event.location.c_str());

  int minu, hours, day, month;
  event.timestamp += 3600;
  getTime(&event.timestamp, &minu, &hours, &day, &month);

  char dateTime[20];

  sprintf(dateTime, "%s %d %d:%d", monthNames_P[month], day, hours, minu);

  char comment[100];
  strcpy(comment, event.comment.c_str());

  char* buffer[] = {
    sevent,
    own,
    dateTime,
    location,
    comment,
    "accept?"
  };

  display.clear();
  for (int j = 0; j < 3; j++) {
    display.println(buffer[i + j]);
    display.drawLogBuffer(0, 0);
    display.display();
  }
  i += sens;
  if (i >= 4) {
    sens = -1;
    i = 1;
  } else if (i <= -1) {
    sens = 1;
    i = 1;
  }
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
      web_send_interrest(menu.curIndex);
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
      web_erase_event(menu.curIndex);
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

void display_main(void)
{
  /*display_btn_manage();

  if (millis() > displayNextTick) {
    display_draw_event(eventsTab.at(menu.curIndex));
    displayNextTick = millis() + 3000;
  }*/
}
#endif
