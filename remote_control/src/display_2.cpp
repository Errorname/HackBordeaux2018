#if 1
#include "display_2.hpp"

extern std::vector<struct event_t> eventsTab;

unsigned long displayNextTick;

TFT_eSPI ucg = TFT_eSPI();       // Invoke custom library
XPT2046 touch(/*cs=*/ PIN_D2, /*irq=*/ PIN_D1);

button_t ButtonPlus;
button_t ButtonMoins;
button_t ButtonAccept;
button_t ButtonRefuse;
button_t ButtonText;

static  byte monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char monthNames_P[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

struct menu_t menu;
struct event_t event;

void display_init(void)
{
  menu.curIndex = 0;
  menu.nbMenu = 0;

  ucg.begin();
  touch.begin(240, 320);  // Must be done before setting rotation
  ucg.setRotation(3);
  touch.setRotation(touch.ROT270);
  ucg.fillScreen(ILI9341_BLACK);

  // Replace these for your screen module
  touch.setCalibration(209, 1759, 1775, 273);

  ButtonPlus.x = 320 - 20 + 5;
  ButtonPlus.y = 5;
  ButtonPlus.h = 240/2;
  ButtonPlus.w = 20;
  ButtonPlus.defColor = DARKGREEN;
  ButtonPlus.pressedColor = BLUE;
  ButtonPlus.label = ">";

  ButtonMoins.x = 5;
  ButtonMoins.y = 5;
  ButtonMoins.h = 240/2;
  ButtonMoins.w = 20;
  ButtonMoins.defColor = DARKGREEN;
  ButtonMoins.pressedColor = BLUE;
  ButtonMoins.label = "<";

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

  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  digitalWrite(LED_R_PIN, 0);
  digitalWrite(LED_G_PIN, 0);
  digitalWrite(LED_B_PIN, 0);
  Serial.println("[display_2.cpp] Big Screen");
}

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

void display_2_draw_event(struct event_t event)
{
  static int i = 0, sens = 1;
  char sevent[20];
  char own[20];
  char location[20];
  sprintf(sevent, "event: %s", event.id.c_str());
  sprintf(own, "own: %s", event.owner.c_str());
  strcpy(location, event.location.c_str());

  Serial.println("Bonjour1");
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

void display_2_btn_manage(void)
{
  static int PlusWasPressed = 0;
  static int MoinsWasPressed = 0;
  static int AcceptWasPressed = 0;
  static int RefuseWasPressed = 0;

  int PlusIsPressed = isButtonPressed(&ButtonPlus);
  int MoinsIsPressed = isButtonPressed(&ButtonMoins);
  int AcceptIsPressed = isButtonPressed(&ButtonAccept);
  int RefuseIsPressed = isButtonPressed(&ButtonRefuse);
  menu.nbMenu = web_get_nb_event();

  if(PlusIsPressed != PlusWasPressed) { // PLUS
    drawButton(&ButtonPlus, PlusIsPressed);
    if(AcceptIsPressed) {
      web_send_interrest(menu.curIndex);
      if(menu.curIndex < (menu.nbMenu - 1)) {
        menu.curIndex++;
      } else {
        menu.curIndex = 0;
      }
    }
  }
  if(MoinsIsPressed != MoinsWasPressed) { // PLUS
    drawButton(&ButtonMoins, MoinsIsPressed);
    if(MoinsIsPressed) {
      if(menu.curIndex < (menu.nbMenu - 1)) {
        menu.curIndex++;
      } else {
        menu.curIndex = 0;
      }
    }
  }
  if(AcceptIsPressed != AcceptWasPressed) { // Yope
    drawButton(&ButtonAccept, AcceptIsPressed);
    if(AcceptIsPressed) {
      web_send_interrest(menu.curIndex);
    }
  }
  if(RefuseIsPressed != RefuseWasPressed) { // Nope
    drawButton(&ButtonRefuse, RefuseIsPressed);
    if(RefuseIsPressed) {
      web_erase_event(menu.curIndex);
    }
  }

  AcceptWasPressed = AcceptIsPressed;
  RefuseWasPressed = RefuseIsPressed;
  MoinsWasPressed = MoinsIsPressed;
  PlusWasPressed = PlusIsPressed;
}

void display_2_main(void)
{
  display_2_btn_manage();

  if (millis() > displayNextTick) {
    //if(eventsTab.size() > 0) {
      //display_2_draw_event(eventsTab.at(menu.curIndex));
    //}

    displayNextTick = millis() + 3000;
  }
}
#endif
