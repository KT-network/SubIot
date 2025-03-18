//
// Created by Immortal Jreey on 2025/3/18.
//

#include "SubIot.h"


bool SubIot::isFactory() {
    LittleFS.begin();
    File system_serial = LittleFS.open("/system/serial.conf", "r");
    if (system_serial) {
        this->subIotSystemSerialNum = system_serial.readString();
        system_serial.close();
    }

    File system_debug = LittleFS.open("/system/debug.conf", "r");
    if (system_debug) {
        system_debug.read(&this->subIotSystemDebugMode, 1);
        system_debug.close();
    }

    LittleFS.end();

    if (this->subIotSystemSerialNum == "") {
        return true;
    }
    return false;

}


void SubIot::begin() {


}


void SubIot::run() {

    switch (this->workState) {
        case WORK_STATE_INIT:
            // 判断是否为出厂模式
            if (this->isFactory()) {
                if (this->subIotSystemDebugMode) {
                    Serial.println("[System] Factory mode");
                }
                this->workState = WORK_STATE_FACTORY_CONFIG;
            }

            break;
        case WORK_STATE_FACTORY_CONFIG:
            break;
        case WORK_STATE_BLE_CONFIG:
            break;
        case WORK_STATE_WIFI_CONNECTED:
            break;
        case WORK_STATE_MQTT_CONNECTED:
            break;
        case WORK_STATE_WORKING:
            break;
        case WORK_STATE_WIFI_ERROR:
            break;
        case WORK_STATE_MQTT_ERROR:
            break;
    }


}