#include "global.hpp"
#include "display.hpp"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <string.h>
#include <vector>
#include <ArduinoJson.h>

unsigned long webNextTick;
StaticJsonBuffer<3000> jsonBuffer;
std::vector<struct event_t> eventsTab;
uint32_t lastCreatedAt = 0;

void web_init(void)
{

}

int web_send_request(String msg, String data, String *awnser)
{
  int ret = 0;
  Serial.println("[web.cpp] HTTP Request : [" + msg + "] " + data);
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;
    String url = "http://us-central1-hackbordeaux2018.cloudfunctions.net/" + msg;
    http.begin(url);
    http.addHeader("Content-Type", "text/plain");
    int httpCode = http.POST(data);
    if (httpCode == HTTP_CODE_OK) { //Check the returning code
      (*awnser) = http.getString();
      //Serial.println("[web.cpp] Server's awnser : " + (*awnser));
      ret = 0;
    } else {
      Serial.println("[web.cpp] Request failed (error: " + http.errorToString(httpCode) + ") !");
      (*awnser) = "";
      ret = -1;
    }
    http.end();   //Close connection
  } else {
    Serial.println("[web.cpp] Wifi disconnected.");
    ret = -1;
  }
  return ret;
}

int web_update_events(String json)
{
  struct event_t event;

  if(json.length() < 5) {
    Serial.println("[web.cpp] JSON is not wide enought : " + json);
    return -1;
  }

  jsonBuffer.clear();
  JsonArray& root = jsonBuffer.parseArray(json);
  if (!root.success()) {
    Serial.println("[web.cpp] parseObject() failed");
    return -1;
  }

  for (int i = 0; i < root.size(); i++) {
    event.id = root[i]["id"].as<char*>();
    event.comment = root[i]["comment"].as<char*>();
    event.createdAt = root[i]["createdAt"].as<uint32_t>();
    event.location = root[i]["location"].as<char*>();
    event.owner = root[i]["owner"].as<char*>();
    event.timestamp = root[i]["timestamp"].as<uint32_t>();
    event.type = root[i]["type"].as<char*>();
    eventsTab.push_back(event);
  }
  return 0;
}

void web_send_interrest(int event_index)
{
  String trash, data;

  if((event_index + 1) > eventsTab.size()) {
    Serial.println("[web.cpp] Trying to send inexisting interrest");
    return;
  }
  data = "idEvent=" + eventsTab.at(event_index).id + "&name=" + USER_NAME;
  if(web_send_request("acceptEvent", data, &trash) == 0) {
    Serial.println("[web.cpp] Interrest sent");
  } else {
    Serial.println("[web.cpp] Failed to send the interrest");
  }
  eventsTab.erase(eventsTab.begin() + event_index);
}

void web_print_memory(void) {
  Serial.println("Print Memory :");
  for(int i = 0; i < eventsTab.size(); i++) {
    Serial.println("> " + String(i) + " - " + String(eventsTab.at(i).id) + " - t=" + String(eventsTab.at(i).createdAt));
  }
}

void web_main(void)
{
  if (millis() > webNextTick) {
    String awnser, last_id;
    last_id = "createdAt=";
    if (eventsTab.size() == 0) {
      last_id += String(lastCreatedAt);
    } else {
      last_id += String(eventsTab.back().createdAt);
    }
    // Ask fo new events
    if(web_send_request("getEvents", last_id, &awnser) == 0) {
      if(web_update_events(awnser) == 0) {
        lastCreatedAt = eventsTab.back().createdAt;
      }
      web_send_interrest(0); // Accept the first event
    }
    webNextTick = millis() + REFRESH_PERIODE;
  }
}
