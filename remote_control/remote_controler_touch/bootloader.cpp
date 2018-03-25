#include "global.hpp"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

void bootloader_init(void)
{
  Serial.print("\n[bootloader.cpp] ");
  Serial.println(BOOTLOADER_VERSION);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);

  Serial.print("[bootloader.cpp] Trying to connect to ");
  Serial.print(WIFI_SSID);
  Serial.println("...");
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("[bootloader.cpp] Connection Failed! Rebooting in 5s...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setPort(OTA_PORT);
  ArduinoOTA.setPassword(OTA_PWD);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("[bootloader.cpp] Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\n[bootloader.cpp] Update completed !");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("[bootloader.cpp] Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("[bootloader.cpp] Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.print("[bootloader.cpp] OTA Ready ! IP address: ");
  Serial.println(WiFi.localIP());
}

void bootloader_main(void)
{
  ArduinoOTA.handle();
}
