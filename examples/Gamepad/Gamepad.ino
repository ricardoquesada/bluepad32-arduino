// Copyright 2021 - 2021, Ricardo Quesada
// SPDX-License-Identifier: Apache 2.0 or LGPL-2.1-or-later

/*
 * This example shows how to use the Gamepad API.
 *
 * Supported on boards with NINA W10x like:
 *  - Arduino MKR WiFi 1010,
 *  - UNO WiFi Rev.2,
 *  - Nano RP2040 Connect,
 *  - Nano 33 IoT,
 *  - etc.
 */
#include <BP32.h>

GamepadPtr myGamepad;

void setup() {
  // Initialize serial
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect.
  }

  String fv = BP32.firmwareVersion();
  String latestFv;
  Serial.print("Firmware version installed: ");
  Serial.println(fv);

  // BP32.pinMode(27, OUTPUT);
  // BP32.digitalWrite(27, 0);

  // Print required firmware version
  Serial.print("Latest firmware version available : ");
  Serial.println(BLUEPAD32_LATEST_FIRMWARE_VERSION);

  BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
}

void onConnectedGamepad(GamepadPtr gp) {
  myGamepad = gp;
  Serial.println("CALLBACK: Gamepad is connected!");
}

void onDisconnectedGamepad(GamepadPtr gp) {
  Serial.println("CALLBACK: Gamepad is disconnected!");
}

void loop() {
  // This call will fetch all the gamepad info from the ESP32 module.
  // Just call this function once in your game loop.
  BP32.update();

  if (myGamepad && myGamepad->isConnected()) {
    if (myGamepad->a()) {
      static int colorIdx = 0;
      // Change color LED. Cycle it on each press
      switch (colorIdx % 3) {
        case 0:
          myGamepad->setColorLED(255, 0, 0);
          break;
        case 1:
          myGamepad->setColorLED(0, 255, 0);
          break;
        case 2:
          myGamepad->setColorLED(0, 0, 255);
          break;
      }
      colorIdx++;
    }

    if (myGamepad->b()) {
      // Turn on the 4 LED. Each bit represents one LED.
      static int led = 0;
      led++;
      myGamepad->setPlayerLEDs(led & 0x0f);
    }

    if (myGamepad->x()) {
      // Duration: 255 is ~2 seconds
      // value: intensity
      myGamepad->setRumble(0x80 /* force */, 0xc0 /* duration */);
    }
  }
  // After calling updateGamepadsData, you can get start using the Gamepad
  // API.

  delay(200);
}
