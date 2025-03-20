//
// Created by Immortal Jreey on 2025/3/18.
//

#ifndef ESP32_TEST_SUBIOT_H
#define ESP32_TEST_SUBIOT_H

#include "Arduino.h"
#include "LittleFS.h"

#include "Core/SubIotBle.h"
#include "Core/SubIotConfig.h"


extern SubIotBle pSubIotBle;

class SubIot{
private:
    WorkState workState = WORK_STATE_INIT;
    String subIotSystemSerialNum = "";
    uint8_t subIotSystemDebugMode = 1;
    bool wait = true;


    bool isFactory();

public:
    void begin();
    void run();

    bool getDebug();
    WorkState getWorkState();
    void setWorkState(WorkState state);
    void setWaitState(bool state);
    bool getWaitState();

};

extern SubIot subIot;

#endif //ESP32_TEST_SUBIOT_H
