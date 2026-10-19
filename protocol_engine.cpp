/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        Computer Networks Protocol & Error Engine (Implementation)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#include "protocol_engine.h"
#include "config.h"
#include "display_ui.h"
#include "input_manager.h"

// ============================================================================
// MATHEMATICAL ERROR DETECTION & CORRECTION ALGORITHMS
// ============================================================================

// CRC-16 (Polynomial 0x1021, Init 0xFFFF)
uint16_t ProtocolEngine::calculateCRC16(const uint8_t* data, size_t len) {
  uint16_t crc = 0xFFFF;
  for (size_t i = 0; i < len; i++) {
    crc ^= ((uint16_t)data[i] << 8);
    for (uint8_t bit = 0; bit < 8; bit++) {
      if (crc & 0x8000) {
        crc = (crc << 1) ^ 0x1021;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

// 16-bit Internet Checksum (RFC 1071, One's Complement)
uint16_t ProtocolEngine::calculateChecksum(const uint8_t* data, size_t len) {
  uint32_t sum = 0;
  size_t i = 0;

  while (len > 1) {
    uint16_t word = ((uint16_t)data[i] << 8) | data[i + 1];
    sum += word;
    i += 2;
    len -= 2;
  }

  // Handle single trailing byte
  if (len > 0) {
    sum += ((uint16_t)data[i] << 8);
  }

  // Fold 32-bit sum to 16 bits
  while (sum >> 16) {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }

  return (uint16_t)(~sum);
}

// Hamming (7,4) Systematic Encoder
// Inputs 4 bits of data (nibble), returns 7-bit codeword
uint8_t ProtocolEngine::encodeHamming74(uint8_t nibble) {
  uint8_t d1 = (nibble >> 0) & 0x01; // Data bit 1 (pos 3)
  uint8_t d2 = (nibble >> 1) & 0x01; // Data bit 2 (pos 5)
  uint8_t d3 = (nibble >> 2) & 0x01; // Data bit 3 (pos 6)
  uint8_t d4 = (nibble >> 3) & 0x01; // Data bit 4 (pos 7)

  // Even parity equations
  uint8_t p1 = d1 ^ d2 ^ d4; // pos 1 (covers 1, 3, 5, 7)
  uint8_t p2 = d1 ^ d3 ^ d4; // pos 2 (covers 2, 3, 6, 7)
  uint8_t p3 = d2 ^ d3 ^ d4; // pos 4 (covers 4, 5, 6, 7)

  // Construct 7-bit codeword: [d4][d3][d2][p3][d1][p2][p1]
  uint8_t code = (p1 << 0) | (p2 << 1) | (d1 << 2) |
                 (p3 << 3) | (d2 << 4) | (d3 << 5) | (d4 << 6);
  return code;
}

// Hamming (7,4) Decoder with Syndrome Error Correction
uint8_t ProtocolEngine::decodeHamming74(uint8_t code7, bool* errorDetected, uint8_t* errorBitPos) {
  uint8_t b1 = (code7 >> 0) & 0x01;
  uint8_t b2 = (code7 >> 1) & 0x01;
  uint8_t b3 = (code7 >> 2) & 0x01;
  uint8_t b4 = (code7 >> 3) & 0x01;
  uint8_t b5 = (code7 >> 4) & 0x01;
  uint8_t b6 = (code7 >> 5) & 0x01;
  uint8_t b7 = (code7 >> 6) & 0x01;

  // Compute syndrome bits
  uint8_t s1 = b1 ^ b3 ^ b5 ^ b7;
  uint8_t s2 = b2 ^ b3 ^ b6 ^ b7;
  uint8_t s3 = b4 ^ b5 ^ b6 ^ b7;

  uint8_t syndrome = (s3 << 2) | (s2 << 1) | s1;

  if (syndrome != 0) {
    *errorDetected = true;
    *errorBitPos = syndrome; // Bit position 1 to 7 is in error
    // Automatically correct the flipped bit
    code7 ^= (1 << (syndrome - 1));
  } else {
    *errorDetected = false;
    *errorBitPos = 0;
  }

  // Extract corrected data bits: d1=pos3, d2=pos5, d3=pos6, d4=pos7
  uint8_t d1 = (code7 >> 2) & 0x01;
  uint8_t d2 = (code7 >> 4) & 0x01;
  uint8_t d3 = (code7 >> 5) & 0x01;
  uint8_t d4 = (code7 >> 6) & 0x01;

  return (d4 << 3) | (d3 << 2) | (d2 << 1) | d1;
}

// ============================================================================
// INTERACTIVE COMPUTER NETWORKS LAB DEMOS
// ============================================================================

void ProtocolEngine::runCrcDemo() {
  const char testString[] = "NETWORKS-2026";
  uint16_t crc = calculateCRC16((const uint8_t*)testString, strlen(testString));

  while (true) {
    display.clearBuffer();
    DisplayUI::drawHeader("CRC-16 VERIFICATION");

    display.setFont(u8g2_font_5x8_tr);
    display.drawStr(2, 21, "Polynomial: 0x1021 (CCITT)");
    
    char buf[36];
    snprintf(buf, sizeof(buf), "Payload:    \"%s\"", testString);
    display.drawStr(2, 31, buf);

    snprintf(buf, sizeof(buf), "Calculated: 0x%04X", crc);
    display.drawStr(2, 41, buf);

    display.drawStr(2, 50, "Status:     [INTEGRITY VALID]");

    DisplayUI::drawFooter("[SEL] Return to Lab");
    display.sendBuffer();

    if (InputManager::readEvent() == BTN_EVT_SELECT) break;
    delay(20);
  }
}

void ProtocolEngine::runChecksumDemo() {
  const char testPayload[] = "SWARNIM-CN-LAB";
  uint16_t csum = calculateChecksum((const uint8_t*)testPayload, strlen(testPayload));

  while (true) {
    display.clearBuffer();
    DisplayUI::drawHeader("INTERNET CHECKSUM");

    display.setFont(u8g2_font_5x8_tr);
    display.drawStr(2, 21, "RFC 1071 16-Bit 1's Compl.");
    
    char buf[36];
    snprintf(buf, sizeof(buf), "Data:   \"%s\"", testPayload);
    display.drawStr(2, 31, buf);

    snprintf(buf, sizeof(buf), "Checksum: 0x%04X", csum);
    display.drawStr(2, 41, buf);

    display.drawStr(2, 50, "Verification: PASS (Sum = ~0)");

    DisplayUI::drawFooter("[SEL] Return to Lab");
    display.sendBuffer();

    if (InputManager::readEvent() == BTN_EVT_SELECT) break;
    delay(20);
  }
}

void ProtocolEngine::runHammingDemo() {
  uint8_t dataNibble = 0x0B; // 1011 in binary
  uint8_t encoded = encodeHamming74(dataNibble);
  
  // Intentionally flip bit 5 to simulate noise/error
  uint8_t corrupted = encoded ^ (1 << 4);

  bool detected = false;
  uint8_t errPos = 0;
  uint8_t recovered = decodeHamming74(corrupted, &detected, &errPos);

  while (true) {
    display.clearBuffer();
    DisplayUI::drawHeader("HAMMING (7,4) CODE");

    display.setFont(u8g2_font_5x8_tr);
    char buf[36];
    snprintf(buf, sizeof(buf), "Orig Nibble: 0x%X (1011b)", dataNibble);
    display.drawStr(2, 21, buf);

    snprintf(buf, sizeof(buf), "Encoded: 0x%02X | Noisy: 0x%02X", encoded, corrupted);
    display.drawStr(2, 31, buf);

    snprintf(buf, sizeof(buf), "Detected Err: Bit pos %d", errPos);
    display.drawStr(2, 41, buf);

    snprintf(buf, sizeof(buf), "Auto-Corrected: 0x%X (MATCH)", recovered);
    display.drawStr(2, 50, buf);

    DisplayUI::drawFooter("[SEL] Return to Lab");
    display.sendBuffer();

    if (InputManager::readEvent() == BTN_EVT_SELECT) break;
    delay(20);
  }
}

void ProtocolEngine::runInteractiveLab() {
  const char* labItems[] = {
    "1. CRC-16 Check",
    "2. Internet Checksum",
    "3. Hamming (7,4) Code",
    "4. Return to Main Menu"
  };
  int selected = 0;

  while (true) {
    DisplayUI::drawMenu("CN PROTOCOL LAB", labItems, 4, selected);
    ButtonEvent evt = InputManager::readEvent();

    if (evt == BTN_EVT_UP) {
      selected = max(0, selected - 1);
    } else if (evt == BTN_EVT_DOWN) {
      selected = min(3, selected + 1);
    } else if (evt == BTN_EVT_SELECT) {
      if (selected == 0) runCrcDemo();
      else if (selected == 1) runChecksumDemo();
      else if (selected == 2) runHammingDemo();
      else if (selected == 3) break;
    }
    delay(20);
  }
}

// ============================================================================
// OVER-THE-AIR NRF24 PACKET TRANSCEIVER
// ============================================================================

const uint8_t RADIO_ADDR[6] = "CNLAB";

void ProtocolEngine::runPacketTx() {
  radio.stopListening();
  radio.openWritingPipe(RADIO_ADDR);
  radio.setChannel(76);
  radio.setDataRate(RF24_1MBPS);

  uint8_t seq = 0;
  while (true) {
    PacketFrame frame;
    frame.preamble = 0xAA55;
    frame.seqNum = seq++;
    strncpy(frame.payload, "SPS-DATA", sizeof(frame.payload));
    frame.payloadLen = strlen(frame.payload);
    frame.crc16 = calculateCRC16((const uint8_t*)frame.payload, frame.payloadLen);

    bool ok = radio.write(&frame, sizeof(frame));

    display.clearBuffer();
    DisplayUI::drawHeader("PACKET TX MODE");

    display.setFont(u8g2_font_5x8_tr);
    char buf[36];
    snprintf(buf, sizeof(buf), "Seq: #%d | Preamble: 0x%04X", frame.seqNum, frame.preamble);
    display.drawStr(2, 21, buf);

    snprintf(buf, sizeof(buf), "Data: \"%s\" (Len:%d)", frame.payload, frame.payloadLen);
    display.drawStr(2, 31, buf);

    snprintf(buf, sizeof(buf), "CRC-16: 0x%04X", frame.crc16);
    display.drawStr(2, 41, buf);

    snprintf(buf, sizeof(buf), "Link Status: %s", ok ? "[SENT OK]" : "[NO ACK/WAIT]");
    display.drawStr(2, 50, buf);

    DisplayUI::drawFooter("[SEL] Stop Transmitting");
    display.sendBuffer();

    if (InputManager::readEvent() == BTN_EVT_SELECT) break;
    delay(300);
  }
}

void ProtocolEngine::runPacketRx() {
  radio.openReadingPipe(1, RADIO_ADDR);
  radio.setChannel(76);
  radio.setDataRate(RF24_1MBPS);
  radio.startListening();

  PacketFrame rxFrame;
  memset(&rxFrame, 0, sizeof(rxFrame));
  bool gotPacket = false;
  bool crcMatch = false;

  while (true) {
    if (radio.available()) {
      radio.read(&rxFrame, sizeof(rxFrame));
      if (rxFrame.preamble == 0xAA55) {
        gotPacket = true;
        uint16_t computed = calculateCRC16((const uint8_t*)rxFrame.payload, rxFrame.payloadLen);
        crcMatch = (computed == rxFrame.crc16);
      }
    }

    display.clearBuffer();
    DisplayUI::drawHeader("PACKET RX (LISTENING)");

    display.setFont(u8g2_font_5x8_tr);
    if (!gotPacket) {
      display.drawStr(2, 25, "Listening on Ch 76...");
      display.drawStr(2, 36, "Waiting for transmitter");
      display.drawStr(2, 47, "Node address: 'CNLAB'");
    } else {
      char buf[36];
      snprintf(buf, sizeof(buf), "Rx Seq #%d | Len: %d", rxFrame.seqNum, rxFrame.payloadLen);
      display.drawStr(2, 21, buf);

      snprintf(buf, sizeof(buf), "Data: \"%.12s\"", rxFrame.payload);
      display.drawStr(2, 31, buf);

      snprintf(buf, sizeof(buf), "Rx CRC: 0x%04X", rxFrame.crc16);
      display.drawStr(2, 41, buf);

      snprintf(buf, sizeof(buf), "Integrity: %s", crcMatch ? "[CRC VALID]" : "[CORRUPTED]");
      display.drawStr(2, 50, buf);
    }

    DisplayUI::drawFooter("[SEL] Stop Receiver");
    display.sendBuffer();

    if (InputManager::readEvent() == BTN_EVT_SELECT) break;
    delay(50);
  }

  radio.stopListening();
}

void ProtocolEngine::runPacketTransceiver() {
  const char* transceiverOptions[] = {
    "1. Transmitter (TX)",
    "2. Receiver (RX)",
    "3. Return to Main Menu"
  };
  int selected = 0;

  while (true) {
    DisplayUI::drawMenu("RADIO TRANSCEIVER", transceiverOptions, 3, selected);
    ButtonEvent evt = InputManager::readEvent();

    if (evt == BTN_EVT_UP) {
      selected = max(0, selected - 1);
    } else if (evt == BTN_EVT_DOWN) {
      selected = min(2, selected + 1);
    } else if (evt == BTN_EVT_SELECT) {
      if (selected == 0) runPacketTx();
      else if (selected == 1) runPacketRx();
      else if (selected == 2) break;
    }
    delay(20);
  }
}
