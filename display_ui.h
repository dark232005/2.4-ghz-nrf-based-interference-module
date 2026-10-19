/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        Display & UI Engine (Header)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <Arduino.h>
#include <U8g2lib.h>

class DisplayUI {
public:
  static void init();
  static void showBootSplash();
  static void drawHeader(const char* title);
  static void drawFooter(const char* hint);
  static void drawMenu(const char* menuTitle, const char* items[], int itemCount, int selectedIndex);
  static void drawProgressBar(int x, int y, int w, int h, int percent);
  static void showAboutScreen();
  static void showStatusPopup(const char* title, const char* msg, int durationMs = 1200);
  static void clear();
  static void update();
};

#endif // DISPLAY_UI_H
