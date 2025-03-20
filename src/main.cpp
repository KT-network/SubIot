#include <Arduino.h>

#include "subordinate/SubIot.h"
// #include "Preferences.h"


#define SUB_IOT_FACTORY_BLE_NAME "ESP32_Factory_1"
#define SUB_IOT_SYSTEM_STATE_LED_PIN 2

Preferences preferences;

void setup() {
    Serial.begin(115200);

    pinMode(2,OUTPUT);
    digitalWrite(2,1);

    // subIot.begin();/

}

void loop() {
// write your code here
    subIot.run();
    delay(100);
}