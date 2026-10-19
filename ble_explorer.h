/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        Bluetooth Low Energy (BLE) Explorer (Header)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#ifndef BLE_EXPLORER_H
#define BLE_EXPLORER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

struct BleDeviceInfo {
  char name[24];
  char address[18];
  int rssi;
};

class BLEExplorer {
public:
  static void init();
  static void run();

private:
  static void performScan(uint32_t scanDurationSec = 3);
  static void showDeviceDetails(int index);

  static const int MAX_BLE_DEVICES = 25;
  static BleDeviceInfo devices[MAX_BLE_DEVICES];
  static int deviceCount;
  static bool bleInitialized;
};

#endif // BLE_EXPLORER_H
