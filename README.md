# 2.4 GHz NRF Based Interference Module 📡
### ESP32 Wireless Protocol, Spectrum & Interference Analysis Suite

**Developer**: **Swarnim Pratap Singh**  
**Field**: Computer Networks & Wireless Communications  
**Platform**: ESP32 (WROOM-32) + NRF24L01+ + SSD1306 OLED (128x64)  
**License**: MIT License  

---

## 📌 Overview

**2.4 GHz NRF Based Interference Module** is an embedded wireless diagnostic and protocol analysis suite built from scratch for the ESP32 microcontroller. The system combines software-defined 2.4 GHz spectrum scanning, 802.11 b/g/n Wi-Fi environment mapping, Bluetooth Low Energy (BLE) discovery, and a complete Computer Networks Data Link Layer protocol verification engine (CRC-16, Internet Checksum, and Hamming Codes).

```
                      ┌─────────────────────────────────┐
                      │ 2.4 GHz NRF Interference Module │
                      │    By Swarnim Pratap Singh      │
                      └────────────────┬────────────────┘
                                       │
        ┌──────────────────────────────┼──────────────────────────────┐
        ▼                              ▼                              ▼
┌──────────────────┐          ┌──────────────────┐          ┌──────────────────┐
│  2.4 GHz RF      │          │  Wi-Fi 802.11    │          │  Bluetooth LE    │
│  Spectrum Sweep  │          │  Diagnostics     │          │  Beacon Explorer │
│  (NRF24L01+)     │          │  (ESP32 Radio)   │          │  (Proximity RSSI)│
└──────────────────┘          └──────────────────┘          └──────────────────┘
                                       │
                                       ▼
                      ┌─────────────────────────────────┐
                      │    Computer Networks Lab        │
                      │  - CRC-16 (CCITT 0x1021)        │
                      │  - Internet Checksum (RFC 1071) │
                      │  - Hamming (7,4) Single-Error   │
                      │  - Wireless Packet Transceiver  │
                      └─────────────────────────────────┘
```

---

## 🚀 Key Modules & Capabilities

### 1. 2.4 GHz RF Spectrum Analyzer
* **Frequency Coverage**: 2.400 GHz to 2.527 GHz across 128 discrete channels.
* **Carrier Detection**: High-speed carrier energy sampling (`testCarrier()` / `testRPD()`).
* **Visual Graph**: Real-time spectrum bar chart on the 128x64 OLED display.
* **Peak Channel Tracking**: Dynamically tracks peak interference frequency and power percentage.
* **Wi-Fi Overlays**: Visual indicators for standard non-overlapping Wi-Fi channels (1, 6, 11).

### 2. Wi-Fi Environmental Scanner & Channel Analyzer
* **Access Point Discovery**: Scans 802.11 b/g/n networks, capturing SSID, BSSID (MAC), Channel, and RSSI.
* **Security Classification**: Classifies Open, WEP, WPA, WPA2, WPA3, and Enterprise authentication.
* **Channel Congestion Analysis**: Computes network density across the 2.4 GHz band and recommends the optimal router channel.

### 3. Bluetooth Low Energy (BLE) Explorer
* **Active Scanning**: Discovers advertising BLE peripherals and beacons.
* **Device Inspection**: Displays peripheral name, MAC address, and measured RSSI.
* **Proximity Estimation**: Categorizes device proximity into *Immediate* (<1m), *Near* (1–3m), or *Far* (>3m).

### 4. Computer Networks Protocol & Error Lab
* **CRC-16 (Cyclic Redundancy Check)**:
  * Uses standard CCITT-FALSE polynomial (`0x1021`).
  * Real-time calculation and integrity validation.
* **Internet Checksum**:
  * Implements RFC 1071 16-bit one's complement algorithm used in IPv4, TCP, and UDP headers.
* **Hamming (7,4) Code**:
  * Systematic linear block code encoding 4 data bits into 7 code bits.
  * Syndrome decoding detects bit corruption and automatically corrects single-bit errors.

### 5. Over-the-Air Packet Transceiver (NRF24)
* **Custom Packet Framing**:
  * `[Preamble: 0xAA55]` + `[Seq Number]` + `[Length]` + `[Payload]` + `[CRC-16]`
* **Transmitter (TX) Mode**: Broadcasts framed telemetry packets with dynamic CRC-16 computation.
* **Receiver (RX) Mode**: Validates packet preamble, verifies CRC checksum, and flags corrupted transmissions.

---

## 🛠 Hardware Configuration & Wiring

| Component | Pin Function | ESP32 GPIO | Description |
| :--- | :--- | :--- | :--- |
| **OLED (SSD1306)** | I2C SDA | **GPIO 21** | Serial Data |
| **OLED (SSD1306)** | I2C SCL | **GPIO 22** | Serial Clock |
| **NRF24L01+** | SPI SCK | **GPIO 14** | Serial Clock |
| **NRF24L01+** | SPI MISO | **GPIO 12** | Master In Slave Out |
| **NRF24L01+** | SPI MOSI | **GPIO 13** | Master Out Slave In |
| **NRF24L01+** | CE | **GPIO 16** | Chip Enable |
| **NRF24L01+** | CSN | **GPIO 15** | Chip Select Not |
| **Push Button** | BTN UP | **GPIO 26** | Active LOW (Internal Pull-Up) |
| **Push Button** | BTN DOWN | **GPIO 32** | Active LOW (Internal Pull-Up) |
| **Push Button** | BTN LEFT | **GPIO 25** | Active LOW (Internal Pull-Up) |
| **Push Button** | BTN RIGHT | **GPIO 27** | Active LOW (Internal Pull-Up) |
| **Push Button** | BTN SELECT | **GPIO 33** | Active LOW (Internal Pull-Up) |

---

## 📦 Required Arduino Libraries

Install the following libraries via the Arduino IDE Library Manager:
1. **RF24** (by TMRh20) - High performance driver for nRF24L01+.
2. **U8g2** (by oliver) - Monochrome graphics library for OLED displays.
3. **ESP32 Core** (by Espressif) - Built-in `WiFi.h`, `BLEDevice.h`, `SPI.h`, `Wire.h`.

---

## ⚡ Build & Upload Instructions

1. Open `RF-NetToolkit.ino` in **Arduino IDE**.
2. Select your board: **Tools > Board > ESP32 Arduino > ESP32 Dev Module**.
3. Select the COM Port corresponding to your ESP32 board.
4. Set Flash Frequency to **80MHz** and Upload Speed to **921600**.
5. Click **Upload**.
6. On startup, the OLED displays the splash screen with **Swarnim Pratap Singh** before loading the main diagnostic menu.

---

## 📄 License

This project is authored by **Swarnim Pratap Singh** and licensed under the **MIT License**. See [`LICENSE`](LICENSE) for complete details.
