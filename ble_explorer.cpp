/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        Bluetooth Low Energy (BLE) Explorer (Implementation)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#include "ble_explorer.h"
#include "config.h"
#include "display_ui.h"
#include "input_manager.h"

BleDeviceInfo BLEExplorer::devices[MAX_BLE_DEVICES];
int BLEExplorer::deviceCount = 0;
bool BLEExplorer::bleInitialized = false;

void BLEExplorer::init() {
  if (!bleInitialized) {
    BLEDevice::init(PROJECT_NAME);
    bleInitialized = true;
  }
}

void BLEExplorer::performScan(uint32_t scanDurationSec) {
  init();
  DisplayUI::showStatusPopup("BLE Explorer", "Scanning BLE Beacons...", 0);

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);

  BLEScanResults foundDevices = pBLEScan->start(scanDurationSec, false);
  deviceCount = min((int)foundDevices.getCount(), MAX_BLE_DEVICES);

  for (int i = 0; i < deviceCount; i++) {
    BLEAdvertisedDevice d = foundDevices.getDevice(i);
    String name = d.getName().c_str();
    if (name.length() == 0) {
      strncpy(devices[i].name, "<Unnamed BLE>", sizeof(devices[i].name) - 1);
    } else {
      strncpy(devices[i].name, name.c_str(), sizeof(devices[i].name) - 1);
    }
    devices[i].name[sizeof(devices[i].name) - 1] = '\0';

    strncpy(devices[i].address, d.getAddress().toString().c_str(), sizeof(devices[i].address) - 1);
    devices[i].address[sizeof(devices[i].address) - 1] = '\0';

    devices[i].rssi = d.getRSSI();
  }

  pBLEScan->clearResults();
}

void BLEExplorer::showDeviceDetails(int index) {
  if (index < 0 || index >= deviceCount) return;
  BleDeviceInfo& dev = devices[index];

  const char* proximityStr = "Far (>3m)";
  if (dev.rssi > -60) proximityStr = "Immediate (<1m)";
  else if (dev.rssi > -80) proximityStr = "Near (1-3m)";

  while (true) {
    display.clearBuffer();
    DisplayUI::drawHeader("BLE DEVICE");

    display.setFont(u8g2_font_5x8_tr);
    char buf[36];
    
    snprintf(buf, sizeof(buf), "Name: %.18s", dev.name);
    display.drawStr(2, 21, buf);

    snprintf(buf, sizeof(buf), "MAC:  %s", dev.address);
    display.drawStr(2, 31, buf);

    snprintf(buf, sizeof(buf), "RSSI: %d dBm", dev.rssi);
    display.drawStr(2, 41, buf);

    snprintf(buf, sizeof(buf), "Prox: %s", proximityStr);
    display.drawStr(2, 50, buf);

    DisplayUI::drawFooter("[SEL] Return to List");
    display.sendBuffer();

    if (InputManager::readEvent() == BTN_EVT_SELECT) {
      break;
    }
    delay(20);
  }
}

void BLEExplorer::run() {
  performScan(3);

  if (deviceCount == 0) {
    DisplayUI::showStatusPopup("BLE Explorer", "No BLE devices found!", 1500);
    return;
  }

  int selected = 0;
  while (true) {
    display.clearBuffer();
    char hdr[32];
    snprintf(hdr, sizeof(hdr), "BLE Devices (%d)", deviceCount);
    DisplayUI::drawHeader(hdr);

    const int maxVisible = 4;
    int topIndex = 0;
    if (selected >= maxVisible) {
      topIndex = selected - maxVisible + 1;
    }

    int y = 22;
    for (int i = 0; i < maxVisible && (topIndex + i) < deviceCount; i++) {
      int idx = topIndex + i;
      char lineBuf[32];
      snprintf(lineBuf, sizeof(lineBuf), "%-12.12s %3ddB", devices[idx].name, devices[idx].rssi);

      if (idx == selected) {
        display.drawBox(2, y - 9, 124, 11);
        display.setDrawColor(0);
        display.setFont(u8g2_font_6x10_tf);
        display.drawStr(6, y, lineBuf);
        display.setDrawColor(1);
      } else {
        display.setFont(u8g2_font_6x10_tr);
        display.drawStr(6, y, lineBuf);
      }
      y += 11;
    }

    DisplayUI::drawFooter("[SEL] Details  [LEFT] Rescan");
    display.sendBuffer();

    ButtonEvent evt = InputManager::readEvent();
    if (evt == BTN_EVT_UP) {
      selected = max(0, selected - 1);
    } else if (evt == BTN_EVT_DOWN) {
      selected = min(deviceCount - 1, selected + 1);
    } else if (evt == BTN_EVT_SELECT) {
      showDeviceDetails(selected);
    } else if (evt == BTN_EVT_LEFT) {
      performScan(3);
      selected = 0;
    } else if (evt == BTN_EVT_RIGHT) {
      break;
    }
    delay(20);
  }
}
