/**
 * ============================================================================
 * Project:       RF-NetToolkit
 * Module:        Input Manager (Implementation)
 * Author:        Swarnim Pratap Singh
 * ============================================================================
 */

#include "input_manager.h"
#include "config.h"

unsigned long InputManager::lastActionTime = 0;

void InputManager::init() {
  pinMode(PIN_BTN_UP, INPUT_PULLUP);
  pinMode(PIN_BTN_DOWN, INPUT_PULLUP);
  pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
  pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_BTN_SELECT, INPUT_PULLUP);
}

bool InputManager::isUpPressed() {
  return digitalRead(PIN_BTN_UP) == LOW;
}

bool InputManager::isDownPressed() {
  return digitalRead(PIN_BTN_DOWN) == LOW;
}

bool InputManager::isLeftPressed() {
  return digitalRead(PIN_BTN_LEFT) == LOW;
}

bool InputManager::isRightPressed() {
  return digitalRead(PIN_BTN_RIGHT) == LOW;
}

bool InputManager::isSelectPressed() {
  return digitalRead(PIN_BTN_SELECT) == LOW;
}

ButtonEvent InputManager::readEvent() {
  unsigned long now = millis();
  if (now - lastActionTime < BTN_DEBOUNCE_MS) {
    return BTN_NONE;
  }

  if (isSelectPressed()) {
    lastActionTime = now;
    return BTN_EVT_SELECT;
  }
  if (isUpPressed()) {
    lastActionTime = now;
    return BTN_EVT_UP;
  }
  if (isDownPressed()) {
    lastActionTime = now;
    return BTN_EVT_DOWN;
  }
  if (isLeftPressed()) {
    lastActionTime = now;
    return BTN_EVT_LEFT;
  }
  if (isRightPressed()) {
    lastActionTime = now;
    return BTN_EVT_RIGHT;
  }

  return BTN_NONE;
}
