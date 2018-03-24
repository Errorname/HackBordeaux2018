#include "global.hpp"
#include "display.hpp"

void setup() {
	Serial.begin(115200);
	bootloader_init();
	Serial.print("[main.cpp] ");
	Serial.println(FIRMWARE_VERSION);
	display_init();
	web_init();
}
void loop() {
	bootloader_main();
	display_main();
	web_main();
}
