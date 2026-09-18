#include <Arduino.h>
#include "webPage.h"
#include "sensors.h"

//This project was divided into two modules: hardware (Sensors.h) and the webPage (webPage.h)

/* 
  1. Initialize the webSockets server
  2. Start reading sensors
  3. If a connection, open the webSocket and stream the HTML, CSS, JS files
  4. Broadcast the sensor data, and listen to messages
*/
const unsigned long interval = 300;
unsigned long lastSend = 0;
void setup(){
  Serial.begin(115200);
  sensorSetup();
  pageBegin();
}


void loop(){
  loopPage();
  //Trying to not flood the page with data
  if(millis() - lastSend > interval){
    lastSend = millis();
     transmission(sensorData());
  }
 
}