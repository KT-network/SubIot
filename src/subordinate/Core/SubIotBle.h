//
// Created by Immortal Jreey on 2025/3/18.
//

#ifndef ESP32_TEST_SUBIOTBLE_H
#define ESP32_TEST_SUBIOTBLE_H

#include "Arduino.h"
#include "BLEDevice.h"

#include "SubIotConfig.h"

#define SUB_IOT_BLE_SERVICE_SYSTEM_FACTORY_UUID "e27c4956-e897-49e9-b37a-41dfe70eaf2c"
#define SUB_IOT_BLE_SERVICE_SYSTEM_CONFIGS_UUID "7b826284-bcaf-49df-ac2c-67561352a83f"

#define SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_SERIAL_NUM_UUID "00002A23-8e5e-4002-85bc-83ec49af3ae9"
#define SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_FACTORY_DEBUG_MODE_UUID "ec85df61-9ab8-4dfd-b550-d5f1158d3fcd"
#define SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_UUID "14c39381-2c4b-4100-9f55-463a72c66000"
#define SUB_IOT_BLE_CHARACTERISTIC_SYSTEM_CONFIG_SSID_PWD_UUID "da7dae9e-d5a0-4f2b-92cd-1e375aa9321f"


class SubIotBle {

private:
    String bleNameString = "";

public:
    void begin(WorkState state);


};


class SubIotBleCharacteristicCallback : public BLECharacteristicCallbacks {

    virtual void onRead(BLECharacteristic *pCharacteristic);

    virtual void onWrite(BLECharacteristic *pCharacteristic);

};


#endif //ESP32_TEST_SUBIOTBLE_H
