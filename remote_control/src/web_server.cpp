#include "global.hpp"
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server(80); //Server on port 80

void web_server_init(void)
{
  server.begin();

  server.on("/clear_picc", HTTP_GET, [](){
    handle_clear_picc();
  });

  server.on("/ask_for_updates", HTTP_GET, [](){
    handle_ask_updates();
  });

  server.onNotFound([](){
    if(!handle_file_read(server.uri())) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  Serial.println("[web_server.cpp] HTTP server started");
}

void web_server_main(void)
{
  server.handleClient();
}

String getContentType(String filename)
{
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handle_file_read(String path)
{
  Serial.println("[web_server.cpp] Received : " + path);
  if(path.endsWith("/")) {
    path += "index.html";
  }
  String contentType = getContentType(path);
  if(SPIFFS.exists(path)){
    File file = SPIFFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

bool handle_clear_picc(void)
{
  Serial.println("[web_server.cpp] Received : /clear_picc");
  fs_clear_picc();
  if(fs_get_last_id() == 0) {
    server.send(200, "text/plain", "PICC Folder Cleared !");
  } else {
    server.send(200, "text/plain", "ERROR: Folder /PICC was not cleared !");
    Serial.println("[web_server.cpp] ERROR: Folder /PICC was not cleared !");
  }

  return true;
}

bool handle_ask_updates(void)
{
  String xml_to_send;
  Serial.println("[web_server.cpp] Received : /ask_for_updates");
  if(rfid_get_new_picc_available()) {
    xml_to_send = "<ask_for_updates>";
    xml_to_send += "<status>1</status>";
    xml_to_send += "<data_content>";
    xml_to_send += "/PICC/" + String(rfid_get_new_picc_available()) + ".txt";
    xml_to_send += "</data_content>";
    xml_to_send += "</ask_for_updates>";
    rfid_reset_flag_new_picc_available();
  } else {
    xml_to_send = "<ask_for_updates><status>0</status></ask_for_updates>";
  }
  server.send(200, "text/xml", xml_to_send);
  return true;
}
