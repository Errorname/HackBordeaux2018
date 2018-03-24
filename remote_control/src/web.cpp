#include "global.hpp"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <string.h>

unsigned long webNextTick;

void web_init(void)
{

}

void web_https_request()
{
  String host = "https://us-central1-hackbordeaux2018.cloudfunctions.net";
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, 443)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/helloworld";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println(line);
      break;
    }
  }
  String line = client.readStringUntil('\n');

}

void web_request(void)
{
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println("[web.cpp] HTTP Request...");
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("https://us-central1-hackbordeaux2018.cloudfunctions.net/helloWorld");
    //http.begin("http://jsonplaceholder.typicode.com/users");
    http.addHeader("Content-Type", "text/plain");
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) { //Check the returning code
      Serial.println("[web.cpp] Server's awnser : " + http.getString());
    } else {
      Serial.println("[web.cpp] Request failed (error: " + http.errorToString(httpCode) + ") !");
    }
    http.end();   //Close connection
  } else {
    Serial.println("[web.cpp] Unable to connect to server.");
  }
}

void web_main(void)
{
  if (millis() > webNextTick) {
    //web_request();
    web_https_request();
    webNextTick = millis() + 3000;
  }
}
