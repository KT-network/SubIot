//
// Created by Immortal Jreey on 2025/3/18.
//

#include "SubIotBle.h"


void SubIotBle::begin(WorkState state) {

    if (state == WORK_STATE_FACTORY_CONFIG) {

        BLEDevice::init(SUB_IOT_FACTORY_BLE_NAME);
        BLEServer *pBleServer = BLEDevice::createServer();
        BLEService *pBleService = pBleServer->createService();


        BLEDevice::startAdvertising();
    }


}
