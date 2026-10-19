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

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <RF24.h>
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// ============================================================================
// PROJECT METADATA
// ============================================================================
#define PROJECT_NAME        "RF-NetToolkit"
#define PROJECT_SUBTITLE    "Wireless Diagnostic Suite"
#define PROJECT_VERSION     "v1.0.0"
#define PROJECT_AUTHOR      "Swarnim Pratap Singh"
#define PROJECT_BUILD_YEAR  "2026"

// ============================================================================
// HARDWARE PIN CONFIGURATION
// ============================================================================

// --- Push Buttons (Active LOW with internal pull-ups) ---
#define PIN_BTN_UP         26
#define PIN_BTN_DOWN       32
#define PIN_BTN_LEFT       25
#define PIN_BTN_RIGHT      27
#define PIN_BTN_SELECT     33

// Debounce timing (milliseconds)
#define BTN_DEBOUNCE_MS    160

// --- I2C OLED Display (SSD1306 128x64) ---
#define OLED_I2C_SDA       21
#define OLED_I2C_SCL       22
#define SCREEN_WIDTH       128
#define SCREEN_HEIGHT      64

// --- SPI Bus Pins (Default custom mapping from test bench) ---
#define SPI_PIN_SCK        14
#define SPI_PIN_MISO       12
#define SPI_PIN_MOSI       13

// --- NRF24L01+ Transceiver 1 (Primary Radio) ---
#define NRF_CE_PIN         16
#define NRF_CSN_PIN        15

// Optional secondary NRF24 radios (if equipped on hardware)
#define NRF2_CE_PIN        18
#define NRF2_CSN_PIN       17
#define NRF3_CE_PIN        23
#define NRF3_CSN_PIN       19

// --- Optional Status LED / NeoPixel ---
#define PIN_STATUS_LED     2

// ============================================================================
// RF SPECTRUM PARAMETERS
// ============================================================================
#define RF_CHANNELS_COUNT  128
#define RF_SWEEP_SAMPLES   50

// ============================================================================
// EXTERNAL SHARED OBJECTS
// ============================================================================
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C display;
extern RF24 radio;

#endif // CONFIG_H
