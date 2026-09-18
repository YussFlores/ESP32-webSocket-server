#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

//GPIO ESP32 pins
const uint8_t lightPin = 32;
const uint8_t soilPin = 34;
const uint8_t dhtPin = 14;


const uint8_t fanPin = 21;
const uint8_t ledPinOne = 23;
const uint8_t ledPinTwo = 26;
const uint8_t ledPinThree = 27;
const uint8_t waterPin = 25;

//Every sensor has their own timer
//They take: "interval", read after some time and "threshold", to limit small variations
DHT dht(dhtPin, DHT11);
JsonDocument data; 
int readSoil(uint8_t pin, unsigned long interval, int threshold){
    static unsigned long lastTime = 0;
    unsigned long timer = millis();                      
    int value = 0;
    static int lastValue = 0;
    if((timer - lastTime) >= interval){ 
        lastTime = timer;
        value = analogRead(pin);
        if(abs(value - lastValue) >= threshold){ 
            lastValue = value;
            return value;

        }
    }
    return lastValue;
}
int readLight(uint8_t pin, unsigned long interval, int threshold){
    static unsigned long lastTime = 0;
    unsigned long timer = millis();
    int value = 0;
    static int lastValue = 0;
    if((timer - lastTime) >= interval){ 
        lastTime = timer;
        value = analogRead(pin);
        if(abs(value - lastValue) >= threshold){ 
            lastValue = value;
            return value;

        }
    }
    return lastValue;
}
 // Using the STL (std::pair) here was a bit overkill, but hey, it's what i knew.
 // It doesn't make much difference in performance for this case.
std::pair<float, float> readDHT(){ 
    static std::pair<float, float> lastValue = {0, 0};
    static unsigned long lasTime = 0;
    unsigned long timer = millis();
    if(timer - lasTime >= 2000){
        lasTime = timer;
        float hum = dht.readHumidity();
        float temp = dht.readTemperature();
        if(!isnan(hum) && !isnan(temp)){
            lastValue.first = hum;
            lastValue.second = temp;
        }
       
    }
    return lastValue;
}

void sensorSetup(){
    pinMode(soilPin, INPUT);
    pinMode(lightPin, INPUT);
    pinMode(ledPinOne, OUTPUT);
    pinMode(ledPinTwo, OUTPUT);
    pinMode(ledPinThree, OUTPUT);
    pinMode(waterPin, OUTPUT);
    pinMode(fanPin, OUTPUT);
    digitalWrite(waterPin, LOW);
    digitalWrite(fanPin, LOW);

    dht.begin();
}

String sensorData(){
    //JSON that is going directly to the webPage
    std::pair<float, float> dhtVal = readDHT();
    int lightVal = readLight(lightPin, 200, 50);
    int soilVal = readSoil(soilPin, 200, 50);
    data["temperature"] = dhtVal.second;
    data["humidity"] = dhtVal.first;
    data["light"] = map(lightVal, 0, 4095, 100, 0);
    data["soil"] = map(soilVal, 0, 4095, 0, 100);
    String json;
    serializeJson(data, json);
    return json;
}

void switchCases(String text){
    //In the webPage module, we stated that if we received a message, we go into this function
    //This compares all the possibly messages to match the received one
    if(text == "LED_ON"){
        digitalWrite(ledPinOne, HIGH);
        digitalWrite(ledPinTwo, HIGH);
        digitalWrite(ledPinThree, HIGH);
        data["ledState"] = true;
        }
    else if(text == "LED_OFF"){
        digitalWrite(ledPinOne, LOW);
        digitalWrite(ledPinTwo, LOW);
        digitalWrite(ledPinThree, LOW);
        data["ledState"] = false;
        }

    if(text == "FAN_ON"){
        digitalWrite(fanPin, HIGH);
        data["fanState"] = true;
    }
    else if(text == "FAN_OFF"){
        digitalWrite(fanPin, LOW);
        data["fanState"] = false;
    }

    if(text == "WATER_ON"){
        digitalWrite(waterPin, HIGH);
        data["waterState"] = true;
    }
    else if(text == "WATER_OFF"){
        digitalWrite(waterPin, LOW);
        data["waterState"] = false;
    }
    
}