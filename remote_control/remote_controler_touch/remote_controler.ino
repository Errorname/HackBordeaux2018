#include "global.hpp"
//#include "display.hpp"
#include "display_2.hpp"

void setup() {
	Serial.begin(115200);
	bootloader_init();
	Serial.print("[main.cpp] ");
	Serial.println(FIRMWARE_VERSION);
	display_2_init();
	web_init();
}

void loop() {
	bootloader_main();
	display_2_main();
	web_main();
}
