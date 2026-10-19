/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        Display & UI Engine (Implementation)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#include "display_ui.h"
#include "config.h"

void DisplayUI::init() {
  display.begin();
  display.setFontMode(1);
  display.setDrawColor(1);
}

void DisplayUI::clear() {
  display.clearBuffer();
}

void DisplayUI::update() {
  display.sendBuffer();
}

void DisplayUI::showBootSplash() {
  display.clearBuffer();

  // Outer decorative frame
  display.drawFrame(0, 0, 128, 64);
  display.drawFrame(2, 2, 124, 60);

  // Title
  display.setFont(u8g2_font_helvB10_tr);
  int titleW = display.getStrWidth(PROJECT_NAME);
  display.drawStr((128 - titleW) / 2, 18, PROJECT_NAME);

  // Subtitle
  display.setFont(u8g2_font_5x8_tr);
  int subW = display.getStrWidth(PROJECT_SUBTITLE);
  display.drawStr((128 - subW) / 2, 28, PROJECT_SUBTITLE);

  // Divider
  display.drawHLine(10, 32, 108);

  // Attribution
  display.setFont(u8g2_font_5x8_tr);
  display.drawStr(34, 42, "Developed by:");
  
  display.setFont(u8g2_font_6x10_tf);
  int authorW = display.getStrWidth(PROJECT_AUTHOR);
  display.drawStr((128 - authorW) / 2, 54, PROJECT_AUTHOR);

  display.sendBuffer();
  delay(2500);
}

void DisplayUI::drawHeader(const char* title) {
  display.setFont(u8g2_font_6x10_tr);
  display.drawStr(2, 9, title);
  display.drawHLine(0, 11, 128);
}

void DisplayUI::drawFooter(const char* hint) {
  display.drawHLine(0, 54, 128);
  display.setFont(u8g2_font_4x6_tr);
  display.drawStr(2, 62, hint);
}

void DisplayUI::drawMenu(const char* menuTitle, const char* items[], int itemCount, int selectedIndex) {
  display.clearBuffer();
  drawHeader(menuTitle);

  // Calculate visible window (up to 4 items on screen)
  const int maxVisible = 4;
  int topIndex = 0;
  if (selectedIndex >= maxVisible) {
    topIndex = selectedIndex - maxVisible + 1;
  }

  int y = 22;
  for (int i = 0; i < maxVisible && (topIndex + i) < itemCount; i++) {
    int itemIdx = topIndex + i;
    if (itemIdx == selectedIndex) {
      // Highlight bar
      display.drawBox(2, y - 9, 124, 11);
      display.setDrawColor(0); // Inverted text
      display.setFont(u8g2_font_6x10_tf);
      display.drawStr(6, y, items[itemIdx]);
      display.setDrawColor(1); // Restore color
    } else {
      display.setFont(u8g2_font_6x10_tr);
      display.drawStr(6, y, items[itemIdx]);
    }
    y += 11;
  }

  // Scroll indicator if needed
  if (itemCount > maxVisible) {
    int barHeight = max(6, (maxVisible * 36) / itemCount);
    int barY = 13 + ((selectedIndex * (36 - barHeight)) / (itemCount - 1));
    display.drawVLine(127, 13, 36);
    display.drawBox(126, barY, 2, barHeight);
  }

  drawFooter("[UP/DN] Move   [SEL] Open");
  display.sendBuffer();
}

void DisplayUI::drawProgressBar(int x, int y, int w, int h, int percent) {
  percent = constrain(percent, 0, 100);
  display.drawFrame(x, y, w, h);
  int fillW = ((w - 4) * percent) / 100;
  if (fillW > 0) {
    display.drawBox(x + 2, y + 2, fillW, h - 4);
  }
}

void DisplayUI::showAboutScreen() {
  display.clearBuffer();
  drawHeader("ABOUT SYSTEM");

  display.setFont(u8g2_font_5x8_tr);
  display.drawStr(2, 21, "Suite:   " PROJECT_NAME);
  display.drawStr(2, 30, "Version: " PROJECT_VERSION " (" PROJECT_BUILD_YEAR ")");
  display.drawStr(2, 39, "Author:  " PROJECT_AUTHOR);
  display.drawStr(2, 48, "Subject: Comp. Networks");

  drawFooter("[SEL] Return to Main Menu");
  display.sendBuffer();
}

void DisplayUI::showStatusPopup(const char* title, const char* msg, int durationMs) {
  display.clearBuffer();
  display.drawFrame(6, 12, 116, 40);
  display.drawFrame(8, 14, 112, 36);

  display.setFont(u8g2_font_6x10_tf);
  int tW = display.getStrWidth(title);
  display.drawStr((128 - tW) / 2, 28, title);

  display.setFont(u8g2_font_5x8_tr);
  int mW = display.getStrWidth(msg);
  display.drawStr((128 - mW) / 2, 42, msg);

  display.sendBuffer();
  if (durationMs > 0) {
    delay(durationMs);
  }
}
