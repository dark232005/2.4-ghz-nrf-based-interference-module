/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        2.4 GHz RF Spectrum Analyzer (Header)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#ifndef RF_ANALYZER_H
#define RF_ANALYZER_H

#include <Arduino.h>
#include <RF24.h>

class RFAnalyzer {
public:
  static bool init();
  static void run();

private:
  static void sweepSpectrum();
  static void drawSpectrumGraph();
  static uint8_t channelHits[128];
  static uint8_t peakChannel;
  static uint8_t peakValue;
  static bool radioReady;
};

#endif // RF_ANALYZER_H
