/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        2.4 GHz RF Spectrum Analyzer (Implementation)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#include "rf_analyzer.h"
#include "config.h"
#include "display_ui.h"
#include "input_manager.h"

uint8_t RFAnalyzer::channelHits[128] = {0};
uint8_t RFAnalyzer::peakChannel = 0;
uint8_t RFAnalyzer::peakValue = 0;
bool RFAnalyzer::radioReady = false;

bool RFAnalyzer::init() {
  if (radio.begin()) {
    radio.setAutoAck(false);
    radio.startListening();
    radio.stopListening();
    radioReady = radio.isChipConnected();
  } else {
    radioReady = false;
  }
  return radioReady;
}

void RFAnalyzer::sweepSpectrum() {
  if (!radioReady) return;

  // Zero out counters for fresh sweep sample
  memset(channelHits, 0, sizeof(channelHits));
  peakValue = 0;

  // Sample across 128 channels
  for (int sample = 0; sample < RF_SWEEP_SAMPLES; sample++) {
    for (uint8_t ch = 0; ch < 128; ch++) {
      radio.setChannel(ch);
      radio.startListening();
      delayMicroseconds(130);
      if (radio.testCarrier() || radio.testRPD()) {
        channelHits[ch]++;
        if (channelHits[ch] > peakValue) {
          peakValue = channelHits[ch];
          peakChannel = ch;
        }
      }
      radio.stopListening();
    }
  }
}

void RFAnalyzer::drawSpectrumGraph() {
  display.clearBuffer();
  
  // Header with Peak Channel
  char headerBuf[32];
  snprintf(headerBuf, sizeof(headerBuf), "2.4GHz RF | Peak: %d", peakChannel);
  DisplayUI::drawHeader(headerBuf);

  if (!radioReady) {
    display.setFont(u8g2_font_5x8_tr);
    display.drawStr(10, 30, "[NRF24 Not Responding]");
    display.drawStr(10, 42, "Check wiring & SPI pins");
    DisplayUI::drawFooter("[SEL] Back to Menu");
    display.sendBuffer();
    return;
  }

  // Baseline at y=52
  const int baseY = 52;
  const int maxBarH = 38; // y=14 to y=52

  for (int x = 0; x < 128; x++) {
    int barHeight = (channelHits[x] * maxBarH) / RF_SWEEP_SAMPLES;
    if (barHeight > 0) {
      display.drawVLine(x, baseY - barHeight, barHeight);
    }
  }

  // Draw Wi-Fi Channel Center Markers: Ch1 (12), Ch6 (37), Ch11 (62)
  display.drawPixel(12, baseY + 1);
  display.drawPixel(37, baseY + 1);
  display.drawPixel(62, baseY + 1);

  // Footer status
  char statusBuf[32];
  snprintf(statusBuf, sizeof(statusBuf), "Pk:%dMHz Pwr:%d%% [SEL]Back", 2400 + peakChannel, (peakValue * 100) / RF_SWEEP_SAMPLES);
  DisplayUI::drawFooter(statusBuf);

  display.sendBuffer();
}

void RFAnalyzer::run() {
  if (!radioReady) {
    init();
  }

  DisplayUI::showStatusPopup("RF Analyzer", "Starting 2.4GHz Sweep...", 800);

  while (true) {
    if (InputManager::readEvent() == BTN_EVT_SELECT) {
      break;
    }

    sweepSpectrum();
    drawSpectrumGraph();
    delay(10);
  }

  // Restore neutral radio state
  if (radioReady) {
    radio.stopListening();
  }
}
