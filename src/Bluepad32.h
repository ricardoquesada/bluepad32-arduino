// Copyright 2021 - 2022, Ricardo Quesada, http://retro.moe
// SPDX-License-Identifier: Apache 2.0 or LGPL-2.1-or-later

#ifndef BP32_BLUEPAD32_H
#define BP32_BLUEPAD32_H

#include <inttypes.h>

#include "Controller.h"
#include "ControllerData.h"
#include "ControllerProperties.h"
#include "Gamepad.h"
#include "GamepadProperties.h"
#include "constants.h"

// Using C callbacks since AVR-GCC (needed for Arduino UNO WiFi) doesn't support
// STL
typedef void (*ControllerCallback)(ControllerPtr controller);
using GamepadCallback = ControllerCallback;

class Bluepad32 {
  // This is what the user receives
  Controller _controllers[BP32_MAX_CONTROLLERS];

  // This is used internally by SPI, and then copied into the Controller::State
  // of each controller
  int _prevConnectedControllers;

  ControllerCallback _onConnect;
  ControllerCallback _onDisconnect;

 public:
  Bluepad32();
  /*
   * Get the firmware version
   * result: version as string with this format a.b.c
   */
  const char* firmwareVersion();
  void setDebug(uint8_t on);

  // GPIOs: This is from NINA fw, and might be needed on some boards.
  // E.g.: Arduino Nano RP2040 Connect requires them to turn on/off the RGB LED,
  // that is controlled from the ESP32.
  void pinMode(uint8_t pin, uint8_t mode);
  int digitalRead(uint8_t pin);
  void digitalWrite(uint8_t pin, uint8_t value);

  // Controller
  void update();

  // When a controller connects to the ESP32, the ESP32 stores keys to make it
  // easier the reconnection.
  // If you want to "forget" (delete) the keys from ESP32, you should call this
  // function.
  void forgetBluetoothKeys();

  // Enable / Disable new Bluetooth connections.
  // When enabled, the device is put in Discovery mode, and new pairs are
  // accepted. When disabled, only devices that have paired before can connect.
  // Established connections are not affected.
  void enableNewBluetoothConnections(bool enabled);

  //
  // Get the local Bluetooth Address.
  // return: pointer to uint8_t array with length 6.
  //
  const uint8_t* localBdAddress();

  void setup(const ControllerCallback& onConnect,
             const ControllerCallback& onDisconnect);

 private:
  void checkProtocol();

  uint8_t _protocolVersionHi;
  uint8_t _protocolVersionLow;

  friend class Controller;
};

extern Bluepad32 BP32;

#endif  // BP32_BLUEPAD32_H
