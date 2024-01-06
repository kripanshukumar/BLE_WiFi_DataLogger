//-------------------------------Include Libraries--------------------------------
#include "NimBLEDevice.h"

//-----------------------------Object Instantiation-------------------------------
NimBLEScan* pBLEScan;

char lookup[17] = "0123456789ABCDEF";
uint32_t ble_timer = 0;
Linked_List<BLE_DEVICE> BLE_LIST;

//-------------------------------Methods/Functions---------------------------------
String convertToHex(NimBLEAdvertisedDevice& dev) {
  uint8_t temp = 0;
  String hex = "";
  for (int i = 0; i < dev.getManufacturerData().length(); i++) {
    temp = (uint8_t)dev.getManufacturerData().data()[i];
    hex += lookup[temp / 16];
    hex += lookup[temp % 16];
  }
  return hex;
}

class MyAdvertisedDeviceCallbacks: public NimBLEAdvertisedDeviceCallbacks {
    void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      /*Serial.println("\n-------------------------------------------------------------------");
        Serial.print("Name          : ");Serial.println(advertisedDevice.getName().c_str());
        Serial.print("Address       : ");Serial.println(advertisedDevice.getAddress().toString().c_str());
        Serial.print("Mfg Data      : ");Serial.println(convertToHex(advertisedDevice));
        for(int i = 0; i < advertisedDevice.getServiceUUIDCount(); i++){
        Serial.print("Service UUID  : ");Serial.println(advertisedDevice.getServiceUUID(i).toString().c_str());
        }
        Serial.print("RSSI          : ");Serial.println(advertisedDevice.getRSSI());
        Serial.println("-------------------------------------------------------------------");
      */
      String serviceUUID;
      for (int i = 0; i < advertisedDevice->getServiceUUIDCount(); i++) {
        serviceUUID = serviceUUID + advertisedDevice->getServiceUUID(i).toString().c_str();
        if ((i != advertisedDevice->getServiceUUIDCount() - 1) && (advertisedDevice->getServiceUUIDCount() > 1)) {
          serviceUUID = serviceUUID + ',';
        }
      }
      BLE_DEVICE temp(advertisedDevice->getName().c_str(),
                      advertisedDevice->getAddress().toString().c_str(),
                      convertToHex(*advertisedDevice).c_str(),
                      serviceUUID.c_str(),
                      advertisedDevice->getRSSI());
      if (BLE_LIST.find(temp) == -1) {
        BLE_LIST.add(temp);
        //BLE_LIST.print();
      }
    }
};

void BLE_INIT() {
  NimBLEDevice::setScanFilterMode(CONFIG_BTDM_SCAN_DUPL_TYPE_DATA_DEVICE);
  NimBLEDevice::setScanDuplicateCacheSize(200);
  NimBLEDevice::init("");
  pBLEScan = NimBLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), false);
  pBLEScan->setActiveScan(true); // Set active scanning, this will get more data from the advertiser.
  pBLEScan->setInterval(100); // How often the scan occurs / switches channels; in milliseconds,
  pBLEScan->setWindow(99);  // How long to scan during the interval; in milliseconds.
  pBLEScan->setMaxResults(0); // do not store the scan results, use callback only.
}

void BLE_SCAN(void* parameter) {
  while (1) {
    if (millis() - ble_timer >= BLE_DATA_ACCUMULATION_INTERVAL) {
      if (pBLEScan->isScanning() == false) {
        pBLEScan->start(BLE_DATA_ACCUMULATION_INTERVAL / 1000, nullptr, false);
      }
      ble_timer = millis();
    }
    delay(100);
  }
}
