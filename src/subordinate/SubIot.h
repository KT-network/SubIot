//
// Created by Immortal Jreey on 2025/3/18.
//

#ifndef ESP32_TEST_SUBIOT_H
#define ESP32_TEST_SUBIOT_H

#include "Arduino.h"
#include "Preferences.h"

#include "Core/SubIotBle.h"
#include "Core/SubIotConfig.h"


SubIotBle subIotBle;

class SubIot{
private:
    WorkState workState = WORK_STATE_INIT;
    String subIotSystemSerialNum = "";
    bool subIotSystemDebugMode = true;
    Preferences pNvs;

    bool isFactory();

public:
    void begin();
    void run();
    bool getDebug();
    WorkState getWorkState();
    Preferences getPreferencesObject();

};

SubIot subIot;

#endif //ESP32_TEST_SUBIOT_H
