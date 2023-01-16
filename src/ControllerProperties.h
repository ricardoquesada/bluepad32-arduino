// Copyright 2021 - 2023, Ricardo Quesada, http://retro.moe
// SPDX-License-Identifier: Apache-2.0 or LGPL-2.1-or-later

#ifndef BP32_CONTROLLER_PROPERTIES_H
#define BP32_CONTROLLER_PROPERTIES_H

// Must match nina_controller_properties_t defined here:
// https://gitlab.com/ricardoquesada/bluepad32/-/blob/main/src/components/bluepad32/uni_platform_nina.c

enum {
  CONTROLLER_PROPERTY_FLAG_RUMBLE = 1UL << 0,
  CONTROLLER_PROPERTY_FLAG_PLAYER_LEDS = 1UL << 1,
  CONTROLLER_PROPERTY_FLAG_PLAYER_LIGHTBAR = 1UL << 2,

  CONTROLLER_PROPERTY_FLAG_GAMEPAD = 1UL << 13,
  CONTROLLER_PROPERTY_FLAG_MOUSE = 1UL << 14,
  CONTROLLER_PROPERTY_FLAG_KEYBOARD = 1UL << 15,
};

struct ControllerProperties {
  uint8_t idx;          // Device index
  uint8_t btaddr[6];    // BT Addr
  uint8_t type;         // model: copy from nina_gamepad_t
  uint8_t subtype;      // subtype. E.g: Wii Remote 2nd version
  uint16_t vendor_id;   // VID
  uint16_t product_id;  // PID
  uint16_t flags;       // Features like Rumble, LEDs, etc.
} __attribute__((packed));

typedef ControllerProperties* ControllerPropertiesPtr;

#endif  // BP32_CONTROLLER_PROPERTIES_H
