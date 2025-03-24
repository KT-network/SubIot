//
// Created by Immortal Jreey on 2025/3/18.
//

#include "SubIotBle.h"

bool SubIotBle::getTaskWaitState() {
    return this->taskWait;
}

void SubIotBle::setTaskWaitState(bool state) {
    this->taskWait = state;
}

void SubIotBle::begin(WorkState state) {



    if (state == WORK_STATE_FACTORY_CONFIG) {

//        BLEDevice::init(this->bleNameString.c_str());
        BLEDevice::init(SUB_IOT_SYSTEM_FACTORY_BLE_NAME);
        BLEServer *pBleServer = BLEDevice::createServer();
        BLEService *pBleService = pBleServer->createService(SUB_IOT_BLE_SERVICE_SYSTEM_FACTORY_UUID);
        BLECharacteristic *pBleCharacteristicSerial = pBleService->createCharacteristic(
                SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_SERIAL_NUM_UUID,
                BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

        pBleCharacteristicSerial->setCallbacks(new SubIotBleCharacteristicCallback());
        pBleServer->setCallbacks(new SubIotBleServerCallback());


        pBleService->start();
        BLEDevice::startAdvertising();
        Serial.println("[Ble] Start FACTORY Config");

        this->taskWait = true;
    } else if (state == WORK_STATE_BLE_CONFIG) {
        BLEDevice::init(this->bleNameString.c_str());
        BLEServer *pBleServer = BLEDevice::createServer();

        BLEService *pBleServiceSystem = pBleServer->createService(SUB_IOT_BLE_SERVICE_SYSTEM_FACTORY_UUID);
        BLECharacteristic *pBleCharacteristicSerial = pBleServiceSystem->createCharacteristic(
                SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_SERIAL_NUM_UUID,
                BLECharacteristic::PROPERTY_READ);


        BLEService *pBleService = pBleServer->createService(SUB_IOT_BLE_SERVICE_SYSTEM_CONFIGS_UUID);
        BLECharacteristic *pBleCharacteristicSsid = pBleService->createCharacteristic(
                SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_UUID,
                BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

        BLECharacteristic *pBleCharacteristicSsidPwd = pBleService->createCharacteristic(
                SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_PWD_UUID,
                BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

        pBleCharacteristicSsid->setCallbacks(new SubIotBleCharacteristicCallback());
        pBleCharacteristicSsidPwd->setCallbacks(new SubIotBleCharacteristicCallback());
        pBleCharacteristicSerial->setCallbacks(new SubIotBleCharacteristicCallback());

        pBleServer->setCallbacks(new SubIotBleServerCallback());

        pBleServiceSystem->start();
        pBleService->start();
        BLEDevice::startAdvertising();
        Serial.println("[Ble] Start Config Wifi");

        this->taskWait = true;

    } else {
        this->taskWait = false;
    }

    while (this->taskWait) {
        if (state == WORK_STATE_FACTORY_CONFIG){
            digitalWrite(SUB_IOT_SYSTEM_STATE_LED_PIN, digitalRead(SUB_IOT_SYSTEM_STATE_LED_PIN) == 0 ? 1 : 0);
            delay(800);
        }else if (state == WORK_STATE_BLE_CONFIG){
            digitalWrite(SUB_IOT_SYSTEM_STATE_LED_PIN, digitalRead(SUB_IOT_SYSTEM_STATE_LED_PIN) == 0 ? 1 : 0);
            delay(1300);
        } else{
            delay(1000);
        }

    }


}
