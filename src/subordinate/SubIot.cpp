//
// Created by Immortal Jreey on 2025/3/18.
//

#include "SubIot.h"


void SubIotBleCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    if (subIot.getWorkState() != WORK_STATE_BLE_CONFIG)
        return;
    if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_UUID) {
        subIot.getPreferencesObject().begin("config", false);
        pCharacteristic->setValue(subIot.getPreferencesObject().getString("wifi_ssid","").c_str());
        subIot.getPreferencesObject().end();
    } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_PWD_UUID) {

        subIot.getPreferencesObject().begin("config", false);
        pCharacteristic->setValue(subIot.getPreferencesObject().getString("wifi_pwd","").c_str());
        subIot.getPreferencesObject().end();
    } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_SERIAL_NUM_UUID) {

        subIot.getPreferencesObject().begin("system", false);
        pCharacteristic->setValue(subIot.getPreferencesObject().getString("serial","").c_str());
        subIot.getPreferencesObject().end();

    }


}

void SubIotBleCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {

    if (subIot.getWorkState() == WORK_STATE_FACTORY_CONFIG) {
        if (pCharacteristic->getValue().empty())
            return;

        if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_SERIAL_NUM_UUID) {
            subIot.getPreferencesObject().begin("system", true);
            subIot.getPreferencesObject().putString("serial",pCharacteristic->getValue().c_str());
            subIot.getPreferencesObject().end();
        } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_DEBUG_MODE_UUID) {
            subIot.getPreferencesObject().begin("system", true);
            subIot.getPreferencesObject().putUChar("debug",pCharacteristic->getData()[0]);
            subIot.getPreferencesObject().end();
        }

    } else {
        if (pCharacteristic->getValue().empty())
            return;
        if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_UUID) {

            subIot.getPreferencesObject().begin("config", true);
            subIot.getPreferencesObject().putString("wifi_ssid",pCharacteristic->getValue().c_str());
            subIot.getPreferencesObject().end();

        } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_PWD_UUID) {
            subIot.getPreferencesObject().begin("config", true);
            subIot.getPreferencesObject().putString("wifi_pwd",pCharacteristic->getValue().c_str());
            subIot.getPreferencesObject().end();
        }

    }

}


bool SubIot::isFactory() {

    this->pNvs.begin("system", false);
    this->subIotSystemSerialNum = this->pNvs.getString("serial","");
    this->subIotSystemDebugMode = this->pNvs.getBool("debug", true);

    this->pNvs.end();

    if (this->subIotSystemSerialNum == "") {
        return true;
    }
    return false;
}

WorkState SubIot::getWorkState() {
    return this->workState;
}

bool SubIot::getDebug() {
    return this->subIotSystemDebugMode;
}

Preferences SubIot:: getPreferencesObject(){
    return this->pNvs;
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
            subIotBle.begin(this->workState);
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