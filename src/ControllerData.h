// Copyright 2023 - 2023, Ricardo Quesada, http://retro.moe
// SPDX-License-Identifier: Apache-2.0 or LGPL-2.1-or-later

#ifndef BP32_CONTROLLER_DATA_H
#define BP32_CONTROLLER_DATA_H

#include <inttypes.h>

// Must match nina_controller_t defined here:
// https://gitlab.com/ricardoquesada/bluepad32/-/blob/main/src/components/bluepad32/uni_platform_nina.c

enum {
  CONTROLLER_CLASS_NONE,
  CONTROLLER_CLASS_GAMEPAD,
  CONTROLLER_CLASS_MOUSE,
  CONTROLLER_CLASS_KEYBOARD,
  CONTROLLER_CLASS_BALANCE_BOARD,
};

typedef struct __attribute__((packed)) {
  // Usage Page: 0x01 (Generic Desktop Controls)
  uint8_t dpad;
  int32_t axis_x;
  int32_t axis_y;
  int32_t axis_rx;
  int32_t axis_ry;

  // Usage Page: 0x02 (Sim controls)
  int32_t brake;
  int32_t throttle;

  // Usage Page: 0x09 (Button)
  uint16_t buttons;

  // Misc buttons (from 0x0c (Consumer) and others)
  uint8_t misc_buttons;

  int32_t gyro[3];
  int32_t accel[3];
} nina_gamepad_t;

typedef struct __attribute__((packed)) {
  int32_t delta_x;
  int32_t delta_y;
  uint8_t buttons;
  uint8_t misc_buttons;
  int8_t scroll_wheel;
} nina_mouse_t;

typedef struct __attribute__((packed)) {
  uint16_t tr;      // Top right
  uint16_t br;      // Bottom right
  uint16_t tl;      // Top left
  uint16_t bl;      // Bottom left
  int temperature;  // Temperature
} nina_balance_board_t;

struct ControllerData {
  int8_t idx;
  // Class of controller: gamepad, mouse, balance, etc.
  uint8_t klass;
  union {
    nina_gamepad_t gamepad;
    nina_mouse_t mouse;
    nina_balance_board_t balance_board;
  };
  uint8_t battery;
} __attribute__((packed));

typedef ControllerData* ControllerDataPtr;

#endif  // BP32_CONTROLLER_DATA_H
