/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        Wi-Fi Environmental Monitor & Diagnostics (Implementation)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#include "wifi_monitor.h"
#include "config.h"
#include "display_ui.h"
#include "input_manager.h"

AccessPointInfo WiFiMonitor::networks[MAX_NETWORKS];
int WiFiMonitor::networkCount = 0;

const char* WiFiMonitor::getAuthName(wifi_auth_mode_t auth) {
  switch (auth) {
    case WIFI_AUTH_OPEN: return "OPEN";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA";
    case WIFI_AUTH_WPA2_PSK: return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/2";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "ENTERPRISE";
    case WIFI_AUTH_WPA3_PSK: return "WPA3";
    case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/3";
    default: return "OTHER";
  }
}

void WiFiMonitor::performScan() {
  DisplayUI::showStatusPopup("Wi-Fi Scanner", "Scanning 802.11 b/g/n...", 0);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int found = WiFi.scanNetworks(false, true); // synchronous scan including hidden
  networkCount = min(found, MAX_NETWORKS);

  for (int i = 0; i < networkCount; i++) {
    String s = WiFi.SSID(i);
    if (s.length() == 0) {
      strncpy(networks[i].ssid, "<Hidden SSID>", sizeof(networks[i].ssid) - 1);
    } else {
      strncpy(networks[i].ssid, s.c_str(), sizeof(networks[i].ssid) - 1);
    }
    networks[i].ssid[sizeof(networks[i].ssid) - 1] = '\0';

    strncpy(networks[i].bssidStr, WiFi.BSSIDstr(i).c_str(), sizeof(networks[i].bssidStr) - 1);
    networks[i].bssidStr[sizeof(networks[i].bssidStr) - 1] = '\0';

    networks[i].rssi = WiFi.RSSI(i);
    networks[i].channel = WiFi.channel(i);
    networks[i].authMode = WiFi.encryptionType(i);
  }

  WiFi.scanDelete();
}

void WiFiMonitor::showNetworkDetails(int index) {
  if (index < 0 || index >= networkCount) return;
  AccessPointInfo& ap = networks[index];

  while (true) {
    display.clearBuffer();
    DisplayUI::drawHeader("AP DETAILS");

    display.setFont(u8g2_font_5x8_tr);
    char buf[36];
    
    // SSID
    snprintf(buf, sizeof(buf), "SSID: %.18s", ap.ssid);
    display.drawStr(2, 20, buf);

    // MAC
    snprintf(buf, sizeof(buf), "MAC:  %s", ap.bssidStr);
    display.drawStr(2, 29, buf);

    // Channel & Frequency
    snprintf(buf, sizeof(buf), "Ch: %d  (%d MHz)", ap.channel, 2407 + (ap.channel * 5));
    display.drawStr(2, 38, buf);

    // RSSI & Auth
    snprintf(buf, sizeof(buf), "RSSI: %d dBm | %s", ap.rssi, getAuthName(ap.authMode));
    display.drawStr(2, 47, buf);

    // Graphical RSSI meter (mapped from -100 dBm to -30 dBm)
    int pct = map(constrain(ap.rssi, -100, -30), -100, -30, 0, 100);
    DisplayUI::drawProgressBar(2, 50, 124, 4, pct);

    DisplayUI::drawFooter("[SEL] Back   [UP/DN] AP List");
    display.sendBuffer();

    ButtonEvent evt = InputManager::readEvent();
    if (evt == BTN_EVT_SELECT) {
      break;
    }
    delay(20);
  }
}

void WiFiMonitor::runScanner() {
  performScan();

  if (networkCount == 0) {
    DisplayUI::showStatusPopup("Wi-Fi Scanner", "No networks detected!", 1500);
    return;
  }

  int selected = 0;
  while (true) {
    display.clearBuffer();
    char hdr[32];
    snprintf(hdr, sizeof(hdr), "Wi-Fi APs (%d Found)", networkCount);
    DisplayUI::drawHeader(hdr);

    const int maxVisible = 4;
    int topIndex = 0;
    if (selected >= maxVisible) {
      topIndex = selected - maxVisible + 1;
    }

    int y = 22;
    for (int i = 0; i < maxVisible && (topIndex + i) < networkCount; i++) {
      int idx = topIndex + i;
      char lineBuf[32];
      snprintf(lineBuf, sizeof(lineBuf), "%-12.12s %3ddB", networks[idx].ssid, networks[idx].rssi);

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

    DisplayUI::drawFooter("[SEL] Inspect   [LEFT] Rescan");
    display.sendBuffer();

    ButtonEvent evt = InputManager::readEvent();
    if (evt == BTN_EVT_UP) {
      selected = max(0, selected - 1);
    } else if (evt == BTN_EVT_DOWN) {
      selected = min(networkCount - 1, selected + 1);
    } else if (evt == BTN_EVT_SELECT) {
      showNetworkDetails(selected);
    } else if (evt == BTN_EVT_LEFT) {
      performScan();
      selected = 0;
    } else if (evt == BTN_EVT_RIGHT) {
      break;
    }
    delay(20);
  }
}

void WiFiMonitor::runChannelDiagnostics() {
  performScan();

  int channelCounts[14] = {0};
  for (int i = 0; i < networkCount; i++) {
    if (networks[i].channel >= 1 && networks[i].channel <= 13) {
      channelCounts[networks[i].channel]++;
    }
  }

  // Determine cleanest non-overlapping channel among 1, 6, 11
  int bestCh = 1;
  int minCongestion = channelCounts[1];
  if (channelCounts[6] < minCongestion) {
    minCongestion = channelCounts[6];
    bestCh = 6;
  }
  if (channelCounts[11] < minCongestion) {
    minCongestion = channelCounts[11];
    bestCh = 11;
  }

  while (true) {
    display.clearBuffer();
    DisplayUI::drawHeader("CHANNEL DIAGNOSTICS");

    display.setFont(u8g2_font_5x8_tr);
    char buf[36];
    snprintf(buf, sizeof(buf), "Ch 1:  %d APs", channelCounts[1]);
    display.drawStr(4, 21, buf);

    snprintf(buf, sizeof(buf), "Ch 6:  %d APs", channelCounts[6]);
    display.drawStr(4, 30, buf);

    snprintf(buf, sizeof(buf), "Ch 11: %d APs", channelCounts[11]);
    display.drawStr(4, 39, buf);

    snprintf(buf, sizeof(buf), "Recommended: Ch %d (Cleanest)", bestCh);
    display.drawStr(4, 50, buf);

    DisplayUI::drawFooter("[SEL] Return to Menu");
    display.sendBuffer();

    if (InputManager::readEvent() == BTN_EVT_SELECT) {
      break;
    }
    delay(30);
  }
}
