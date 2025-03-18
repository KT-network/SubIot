//
// Created by Immortal Jreey on 2025/3/18.
//

#ifndef ESP32_TEST_SUBIOT_H
#define ESP32_TEST_SUBIOT_H

#include "Arduino.h"
#include "LittleFS.h"

#include "Core/SubIotConfig.h"


class SubIot{
private:
    WorkState workState = WORK_STATE_INIT;
    String subIotSystemSerialNum = "";
    uint8_t subIotSystemDebugMode = 1;


    bool isFactory();

public:
    void begin();
    void run();

    bool getDebug();

};

#endif //ESP32_TEST_SUBIOT_H
