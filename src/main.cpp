#include <Arduino.h>
#include "webPage.h"
#include "sensors.h"

const unsigned long interval = 300;
unsigned long lastSend = 0;
void setup(){
  Serial.begin(115200);
  sensorSetup();
  pageBegin();
}


void loop(){
  loopPage();
  if(millis() - lastSend > interval){
    lastSend = millis();
     transmission(sensorData());
  }
 
}