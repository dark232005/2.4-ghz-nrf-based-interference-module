/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Description:   ESP32 Wireless Protocol, Spectrum & Network Analysis Suite
 * Author:        Swarnim Pratap Singh
 * Version:       1.0.0
 * Date:          October 2026
 * License:       MIT
 * ============================================================================
 */

#include "config.h"
#include "input_manager.h"
#include "display_ui.h"
#include "rf_analyzer.h"
#include "wifi_monitor.h"
#include "ble_explorer.h"
#include "protocol_engine.h"

// ============================================================================
// GLOBAL OBJECT DEFINITIONS
// ============================================================================
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);

// ============================================================================
// MENU CONFIGURATION
// ============================================================================
const int MAIN_MENU_ITEMS = 7;
const char* menuItems[MAIN_MENU_ITEMS] = {
  "1. 2.4GHz Spectrum",
  "2. Wi-Fi AP Scanner",
  "3. Wi-Fi Diagnostics",
  "4. BLE Explorer",
  "5. Radio Transceiver",
  "6. CN Protocol Lab",
  "7. About System"
};

int currentSelection = 0;

// ============================================================================
// SETUP
// ============================================================================
void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n==========================================");
  Serial.println("  RF-NetToolkit v1.0.0");
  Serial.println("  Developed by Swarnim Pratap Singh");
  Serial.println("==========================================\n");

  // 1. Initialize Display
  DisplayUI::init();
  DisplayUI::showBootSplash();

  // 2. Initialize Push Buttons
  InputManager::init();

  // 3. Initialize SPI bus for NRF24L01+
  SPI.begin(SPI_PIN_SCK, SPI_PIN_MISO, SPI_PIN_MOSI);

  // 4. Initialize Radio
  Serial.print("Initializing NRF24L01+ on SPI (SCK=14, MISO=12, MOSI=13)... ");
  if (RFAnalyzer::init()) {
    Serial.println("ONLINE");
  } else {
    Serial.println("WARNING (Not responding / check pins)");
  }

  // 5. Initialize BLE
  BLEExplorer::init();

  Serial.println("System initialization complete. Entering main menu.");
}

// ============================================================================
// MAIN LOOP
// ============================================================================
void loop() {
  DisplayUI::drawMenu("RF-NETTOOLKIT", menuItems, MAIN_MENU_ITEMS, currentSelection);

  ButtonEvent evt = InputManager::readEvent();

  if (evt == BTN_EVT_UP) {
    currentSelection = max(0, currentSelection - 1);
  } else if (evt == BTN_EVT_DOWN) {
    currentSelection = min(MAIN_MENU_ITEMS - 1, currentSelection + 1);
  } else if (evt == BTN_EVT_SELECT) {
    switch (currentSelection) {
      case 0:
        RFAnalyzer::run();
        break;
      case 1:
        WiFiMonitor::runScanner();
        break;
      case 2:
        WiFiMonitor::runChannelDiagnostics();
        break;
      case 3:
        BLEExplorer::run();
        break;
      case 4:
        ProtocolEngine::runPacketTransceiver();
        break;
      case 5:
        ProtocolEngine::runInteractiveLab();
        break;
      case 6:
        DisplayUI::showAboutScreen();
        while (InputManager::readEvent() != BTN_EVT_SELECT) {
          delay(20);
        }
        break;
      default:
        break;
    }
  }

  delay(20);
}
