/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        Input Manager (Header)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>

enum ButtonEvent {
  BTN_NONE = 0,
  BTN_EVT_UP,
  BTN_EVT_DOWN,
  BTN_EVT_LEFT,
  BTN_EVT_RIGHT,
  BTN_EVT_SELECT
};

class InputManager {
public:
  static void init();
  static ButtonEvent readEvent();
  static bool isUpPressed();
  static bool isDownPressed();
  static bool isLeftPressed();
  static bool isRightPressed();
  static bool isSelectPressed();

private:
  static unsigned long lastActionTime;
};

#endif // INPUT_MANAGER_H
