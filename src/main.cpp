#include <Arduino.h>

#include "subordinate/SubIot.h"

#define SUB_IOT_FACTORY_BLE_NAME "ESP32_Factory_1"
#define SUB_IOT_SYSTEM_STATE_LED_PIN 2


void setup() {
    Serial.begin(115200);

// write your initialization code here
    subIot.begin();

}

void loop() {
// write your code here
    subIot.run();
    delay(100);
}