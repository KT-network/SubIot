//
// Created by Immortal Jreey on 2025/3/18.
//

#include "SubIot.h"


void SubIotBleCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    if (subIot.getWorkState() != WORK_STATE_BLE_CONFIG)
        return;
    if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_UUID) {

        LittleFS.begin();
        File ssid = LittleFS.open("/config/wifi_ssid.conf", "w");
        if (ssid) {
            pCharacteristic->setValue(ssid.readString().c_str());
            ssid.close();
        }
        LittleFS.end();
    } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_PWD_UUID) {

        LittleFS.begin();
        File ssid_pwd = LittleFS.open("/config/wifi_ssid_pwd.conf", "w");
        if (ssid_pwd) {
            pCharacteristic->setValue(ssid_pwd.readString().c_str());
            ssid_pwd.close();
        }
        LittleFS.end();
    } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_SERIAL_NUM_UUID) {

        LittleFS.begin();
        File system_serial = LittleFS.open("/system/serial.conf", "w");
        if (system_serial) {
            pCharacteristic->setValue(system_serial.readString().c_str());
            system_serial.close();
        }
        LittleFS.end();
    }


}

void SubIotBleCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {

    if (subIot.getWorkState() == WORK_STATE_FACTORY_CONFIG) {
        if (pCharacteristic->getValue().empty())
            return;

        if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_SERIAL_NUM_UUID) {

            LittleFS.begin();
            File system_serial = LittleFS.open("/system/serial.conf", "w");
            if (system_serial) {
                system_serial.print(pCharacteristic->getValue().c_str());
                system_serial.close();
            }
            LittleFS.end();

        } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_DEBUG_MODE_UUID) {

            LittleFS.begin();
            File system_debug = LittleFS.open("/system/debug.conf", "w");
            if (system_debug) {
                system_debug.write(pCharacteristic->getData(), 1);
                system_debug.close();
            }
            LittleFS.end();
        }

    } else {
        if (pCharacteristic->getValue().empty())
            return;

        if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_UUID) {

            LittleFS.begin();
            File ssid = LittleFS.open("/config/wifi_ssid.conf", "w");
            if (ssid) {
                ssid.print(pCharacteristic->getValue().c_str());
                ssid.close();
            }
            LittleFS.end();

        } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_PWD_UUID) {
            LittleFS.begin();
            File ssid_pwd = LittleFS.open("/config/wifi_ssid_pwd.conf", "w");
            if (ssid_pwd) {
                ssid_pwd.print(pCharacteristic->getValue().c_str());
                ssid_pwd.close();
            }
            LittleFS.end();
        }

    }

}


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

WorkState SubIot::getWorkState() {
    return this->workState;
}

bool SubIot::getDebug() {
    if (this->subIotSystemDebugMode == 0)
        return false;
    return true;
}

void SubIot::begin() {


}


void SubIot::run() {

    switch (this->workState) {
        case WORK_STATE_INIT:
            // 判断是否为出厂模式
            if (this->isFactory()) {
                if (this->getDebug()) {
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