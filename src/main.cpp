#include <Arduino.h>

//#include "subordinate/Core/SubIotConfig.h"
#include "subordinate/SubIot.h"
// #include "Preferences.h"


//#undef SUB_IOT_SYSTEM_FACTORY_BLE_NAME "ESP32_Factory_2"
//#define SUB_IOT_SYSTEM_STATE_LED_PIN 2


void setup() {
    Serial.begin(115200);


     subIot.begin();

}

void loop() {
    subIot.run();
    delay(100);
}