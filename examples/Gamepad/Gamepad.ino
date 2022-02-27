// Copyright 2021 - 2022, Ricardo Quesada
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
#include <Bluepad32.h>

GamepadPtr myGamepad;

void setup() {
  // Initialize serial
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect.
    // You don't have to do this in your game. This is only for debugging
    // purposes, so that you can see the output in the serial console.
    ;
  }

  String fv = BP32.firmwareVersion();
  Serial.print("Firmware version installed: ");
  Serial.println(fv);

  // BP32.pinMode(27, OUTPUT);
  // BP32.digitalWrite(27, 0);

  // This call is mandatory. It setups Bluepad32 and creates the callbacks.
  BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();
}

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedGamepad(GamepadPtr gp) {
  // In this example we only use one gamepad at the same time.
  myGamepad = gp;
  Serial.println("CALLBACK: Gamepad is connected!");
}

void onDisconnectedGamepad(GamepadPtr gp) {
  Serial.println("CALLBACK: Gamepad is disconnected!");
  myGamepad = nullptr;
}

void loop() {
  // This call fetches all the gamepad info from the NINA (ESP32) module.
  // Just call this function in your main loop.
  // The gamepads pointer (the ones received in the callbacks) gets updated
  // automatically.
  BP32.update();

  // It is safe to always do this before using the gamepad API.
  // This guarantees that the gamepad is valid and connected.
  if (myGamepad && myGamepad->isConnected()) {
    // There are different ways to query whether a button is pressed.
    // By query each button individually:
    //  a(), b(), x(), y(), l1(), etc...
    if (myGamepad->a()) {
      static int colorIdx = 0;
      // Some gamepads like DS4 and DualSense support changing the color LED.
      // It is possible to change it by calling:
      switch (colorIdx % 3) {
        case 0:
          // Red
          myGamepad->setColorLED(255, 0, 0);
          break;
        case 1:
          // Green
          myGamepad->setColorLED(0, 255, 0);
          break;
        case 2:
          // Blue
          myGamepad->setColorLED(0, 0, 255);
          break;
      }
      colorIdx++;
    }

    if (myGamepad->b()) {
      // Turn on the 4 LED. Each bit represents one LED.
      static int led = 0;
      led++;
      // Some gamepads like the DS3, DualSense, Nintendo Wii, Nintendo Switch
      // support changing the "Player LEDs": those 4 LEDs that usually indicate
      // the "gamepad seat".
      // It is possible to change them by calling:
      myGamepad->setPlayerLEDs(led & 0x0f);
    }

    if (myGamepad->x()) {
      // Duration: 255 is ~2 seconds
      // force: intensity
      // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S support
      // force feedback.
      // It is possible to set it by calling:
      myGamepad->setRumble(0xc0 /* force */, 0xc0 /* duration */);
    }

    // Another way to query the buttons, is by calling buttons(), or
    // miscButtons() which return a bitmask.
    // Some gamepads also have DPAD, axis and more.
    char buffer[120];
    snprintf(buffer, sizeof(buffer) - 1,
             "dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, "
             "%4d, brake: %4d, throttle: %4d, misc: 0x%02x",
             myGamepad->dpad(),        // DPAD
             myGamepad->buttons(),     // bitmask of pressed buttons
             myGamepad->axisX(),       // (-511 - 512) left X Axis
             myGamepad->axisY(),       // (-511 - 512) left Y axis
             myGamepad->axisRX(),      // (-511 - 512) right X axis
             myGamepad->axisRY(),      // (-511 - 512) right Y axis
             myGamepad->brake(),       // (0 - 1023): brake button
             myGamepad->throttle(),    // (0 - 1023): throttle (AKA gas) button
             myGamepad->miscButtons()  // bitmak of pressed "misc" buttons
    );
    Serial.println(buffer);

    // You can query the axis and other properties as well. See Gamepad.h
    // For all the available functions.
  }

  delay(150);
}
