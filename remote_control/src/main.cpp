#include "global.hpp"

void setup() {
	Serial.begin(115200);
	bootloader_init();
	Serial.print("[main.cpp] ");
	Serial.println(FIRMWARE_VERSION);
	file_sys_init();
	web_server_init();
	rfid_init();
	fs_print_flash_info();
	fs_list_dir();
}
void loop() {
	bootloader_main();
	rfid_main();
	web_server_main();
}
