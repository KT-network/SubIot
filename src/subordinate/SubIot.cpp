//
// Created by Immortal Jreey on 2025/3/18.
//

#include "SubIot.h"

SubIotBle pSubIotBle;
SubIot subIot;

void SubIotBleServerCallback::onConnect(BLEServer *pServer) {



    this->connectState = true;


}


void SubIotBleServerCallback::onDisconnect(BLEServer *pServer) {
    this->connectState = false;
    if (subIot.getWorkState() == WORK_STATE_BLE_CONFIG || subIot.getWorkState() == WORK_STATE_FACTORY_CONFIG){
        pServer->startAdvertising();
    }

}

void SubIotBleCharacteristicCallback::onRead(BLECharacteristic *pCharacteristic) {
    if (subIot.getWorkState() != WORK_STATE_BLE_CONFIG)
        return;
    if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_UUID) {
        Preferences preferences;
        preferences.begin("config", true);
        pCharacteristic->setValue(preferences.getString("wifi_ssid","").c_str());
        preferences.end();
    } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_PWD_UUID) {
        Preferences preferences;
        preferences.begin("config", true);
        pCharacteristic->setValue(preferences.getString("wifi_pwd","").c_str());
        preferences.end();
    } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_SERIAL_NUM_UUID) {
        Preferences preferences;
        preferences.begin("system", true);
        pCharacteristic->setValue(preferences.getString("serial","").c_str());
        preferences.end();

    }


}

void SubIotBleCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic) {

    if (subIot.getWorkState() == WORK_STATE_FACTORY_CONFIG) {
        if (pCharacteristic->getValue().empty())
            return;

        if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_SERIAL_NUM_UUID) {
            Preferences preferences;

            preferences.begin("system", false);
            preferences.putString("serial",pCharacteristic->getValue().c_str());
            Serial.println("[Ble] Write Factory Config Done");
            preferences.end();

            pSubIotBle.setTaskWaitState(false);
            subIot.setWorkState(WORK_STATE_BLE_CONFIG);

        } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_DEBUG_MODE_UUID) {
            Preferences preferences;
            preferences.begin("system", false);
            preferences.putUChar("debug",pCharacteristic->getData()[0]);
            preferences.end();
        }

    } else {
        if (pCharacteristic->getValue().empty())
            return;

        if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_UUID) {
            Preferences preferences;
            preferences.begin("config", false);
            preferences.putString("wifi_ssid",pCharacteristic->getValue().c_str());
            preferences.end();
            this->ssidWriteCount =1;

        } else if (pCharacteristic->getUUID().toString() == SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_PWD_UUID) {
            Preferences preferences;
            preferences.begin("config", false);
            preferences.putString("wifi_pwd",pCharacteristic->getValue().c_str());
            preferences.end();
            this->ssidWriteCount=2;
        }

        if (this->ssidWriteCount >= 2){
            pSubIotBle.setTaskWaitState(false);
            subIot.setWorkState(WORK_STATE_WIFI_CONNECTED);

        }

    }

}


bool SubIot::isFactory() {

    Preferences preferences;

    preferences.begin("system", true);

    this->subIotSystemSerialNum = preferences.getString("serial","");
    this->subIotSystemDebugMode = preferences.getBool("debug", true);
    preferences.end();
    if (this->subIotSystemSerialNum == ""){
        return true;
    }
    Serial.println("[System] Serial Value:" + this->subIotSystemSerialNum);

    return false;

}

void SubIot::setWorkState(WorkState state) {

    this->workState = state;
}


WorkState SubIot::getWorkState() {
    return this->workState;
}

bool SubIot::getDebug() {
    return this->subIotSystemDebugMode;
}

//Preferences SubIot:: getPreferencesObject(){
//    return this->pNvs;
//}

void SubIot::begin() {
    pinMode(SUB_IOT_SYSTEM_STATE_LED_PIN,OUTPUT);
    digitalWrite(SUB_IOT_SYSTEM_STATE_LED_PIN,1);
    Preferences preferences;
    preferences.begin("config", true);
    Serial.println("ssid:"+preferences.getString("wifi_ssid","123")) ;
    Serial.println("pwd:"+preferences.getString("wifi_pwd","456"));
    preferences.end();

//    Preferences preferences;
//    preferences.begin("system", false);
//    preferences.remove("serial");
//    preferences.end();
//
//    preferences.begin("config", false);
//    preferences.remove("wifi_ssid");
//    preferences.remove("wifi_pwd");
//    preferences.end();

}


void SubIot::run() {

    switch (this->workState) {
        case WORK_STATE_INIT:
            // 判断是否为出厂模式
            if (this->isFactory()) {
                Serial.println("[System] Factory mode");
                this->workState = WORK_STATE_FACTORY_CONFIG;
            } else{
                Serial.println("[System] User mode");
                Preferences preferences;
                preferences.begin("config", true);
                if (preferences.getString("wifi_ssid","") == "" || preferences.getString("wifi_pwd","") == "")
                {
                    Serial.println("[System] Wifi No Config");
                    this->workState = WORK_STATE_BLE_CONFIG;
                } else{
                    Serial.println("[System] Prepare wifi Connection");
                    this->workState = WORK_STATE_WIFI_CONNECTED;
                }
                preferences.end();

            }

            break;
        case WORK_STATE_FACTORY_CONFIG:
            pSubIotBle.begin(this->workState);
            break;
        case WORK_STATE_BLE_CONFIG:
            pSubIotBle.begin(this->workState);
            break;
        case WORK_STATE_WIFI_CONNECTED:
//            Serial.println("[WIFI] WIFI Connect...");
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