/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        Computer Networks Protocol & Error Engine (Header)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#ifndef PROTOCOL_ENGINE_H
#define PROTOCOL_ENGINE_H

#include <Arduino.h>
#include <RF24.h>

struct PacketFrame {
  uint16_t preamble; // 0xAA55
  uint8_t seqNum;
  uint8_t payloadLen;
  char payload[16];
  uint16_t crc16;
};

class ProtocolEngine {
public:
  // Mathematical Error Detection/Correction algorithms
  static uint16_t calculateCRC16(const uint8_t* data, size_t len);
  static uint16_t calculateChecksum(const uint8_t* data, size_t len);
  static uint8_t encodeHamming74(uint8_t nibble);
  static uint8_t decodeHamming74(uint8_t code7, bool* errorDetected, uint8_t* errorBitPos);

  // Interactive UI modules
  static void runInteractiveLab();
  static void runPacketTransceiver();

private:
  static void runCrcDemo();
  static void runChecksumDemo();
  static void runHammingDemo();
  
  static void runPacketTx();
  static void runPacketRx();
};

#endif // PROTOCOL_ENGINE_H
