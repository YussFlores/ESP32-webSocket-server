
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <LittleFS.h>
#include <sensors.h>
#include "credentials.h"


//WebServer and WebSocketStart

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

//When receive a pairing connection, we do multiple cases depending on the data: Connected/disconnected/TextMessage/Error
void connectionEvent(uint8_t connection, WStype_t type, uint8_t* payload, size_t length){
  IPAddress ip = webSocket.remoteIP(connection);
  switch (type)
  {
  case WStype_CONNECTED:
      Serial.printf("%u, connection detected from ip: %d, %d, %d, %d", connection, ip[0], ip[1], ip[2], ip[3]);
    break;
  
  case WStype_DISCONNECTED:
      Serial.printf("Connection disconnected: %u", connection);
      break;
  case WStype_ERROR:
      break;
  case WStype_TEXT:
        String text = String((char*) payload);
        switchCases(text);
    break;
  }
}

//The handle files work for the connections to have the web page files required for webSockets and HTML
void handleHTML(){
    if(LittleFS.exists("/index.html")){
        File file = LittleFS.open("/index.html", "r");
        server.streamFile(file, "text/html");
        file.close();
    }
    else{
        server.send(404, "text/plain", "html not found");
    }
}

void handleJS(){
    if(LittleFS.exists("/script.js")){
        File file = LittleFS.open("/script.js", "r");
        server.streamFile(file, "text/javascript");
        file.close();
    }
    else{
        server.send(404, "text/plain", "script not found");
    }
}


void handleCSS(){
    if(LittleFS.exists("/styles.css")){
        File file = LittleFS.open("/styles.css", "r");
        server.streamFile(file, "text/css");
        file.close();
    }
    else{
        server.send(404, "text/plain", "styles not found");
    }
}

void pageBegin(){
  WiFi.begin(SSID, PASSWORD); //STATION MODE: Esp32 lives on your LAN network (Wifi of your house basically)
  //Local SSID and PASSWORD come from another file for privacy
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  LittleFS.begin(true); 
  Serial.println(WiFi.localIP()); 
  Serial.println("Begin succesfull");
  //server.on serves the html, css and js when navigator asks for it
  server.on("/", handleHTML);
  server.on("/script.js", handleJS);
  server.on("/styles.css", handleCSS);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(connectionEvent);
}

void loopPage(){
  webSocket.loop();
  server.handleClient();
}

void transmission(String msg){ //Same function but we save another import 
    webSocket.broadcastTXT(msg);
}