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

#if defined(SUB_IOT_FACTORY_BLE_NAME)

    this.bleNameString = SUB_IOT_FACTORY_BLE_NAME;
#else
    this->bleNameString = "ESP32_Factory_1";
#endif

    if (state == WORK_STATE_FACTORY_CONFIG) {

        BLEDevice::init(this->bleNameString.c_str());
        BLEServer *pBleServer = BLEDevice::createServer();
        BLEService *pBleService = pBleServer->createService(SUB_IOT_BLE_SERVICE_SYSTEM_FACTORY_UUID);
        BLECharacteristic *pBleCharacteristicSerial = pBleService->createCharacteristic(
                SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_SERIAL_NUM_UUID,
                BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

        pBleCharacteristicSerial->setCallbacks(new SubIotBleCharacteristicCallback());

        pBleService->start();
        BLEDevice::startAdvertising();
        this->taskWait = true;
    } else if (state == WORK_STATE_BLE_CONFIG) {
        BLEDevice::init(this->bleNameString.c_str());
        BLEServer *pBleServer = BLEDevice::createServer();
        BLEService *pBleService = pBleServer->createService(SUB_IOT_BLE_SERVICE_SYSTEM_CONFIGS_UUID);
        BLECharacteristic *pBleCharacteristicSsid = pBleService->createCharacteristic(
                SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_UUID,
                BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

        BLECharacteristic *pBleCharacteristicSsidPwd = pBleService->createCharacteristic(
                SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_PWD_UUID,
                BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

        pBleCharacteristicSsid->setCallbacks(new SubIotBleCharacteristicCallback());
        pBleCharacteristicSsidPwd->setCallbacks(new SubIotBleCharacteristicCallback());


        pBleService->start();
        BLEDevice::startAdvertising();
        this->taskWait = true;

    } else {
        this->taskWait = false;
    }

    while (this->taskWait) {


        delay(1000);
        Serial.println("[Ble] Start Config Wifi");
    } else {
    }


}
