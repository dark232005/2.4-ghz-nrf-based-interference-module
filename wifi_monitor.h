/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        Wi-Fi Environmental Monitor & Diagnostics (Header)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#ifndef WIFI_MONITOR_H
#define WIFI_MONITOR_H

#include <Arduino.h>
#include <WiFi.h>

struct AccessPointInfo {
  char ssid[33];
  char bssidStr[18];
  int32_t rssi;
  uint8_t channel;
  wifi_auth_mode_t authMode;
};

class WiFiMonitor {
public:
  static void runScanner();
  static void runChannelDiagnostics();

private:
  static void performScan();
  static const char* getAuthName(wifi_auth_mode_t auth);
  static void showNetworkDetails(int index);
  
  static const int MAX_NETWORKS = 30;
  static AccessPointInfo networks[MAX_NETWORKS];
  static int networkCount;
};

#endif // WIFI_MONITOR_H
