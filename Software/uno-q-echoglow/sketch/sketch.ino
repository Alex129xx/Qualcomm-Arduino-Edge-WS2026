// SPDX-FileCopyrightText: Copyright (C) Electronic Cats
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>

Arduino_LED_Matrix matrix;

// The onboard LED matrix is physically staggered/diagonal, but the
// Arduino_LED_Matrix API addresses it as a logical 8-row x 12-column bitmap.
// Keep the icons sparse and centered in that native logical layout so they
// remain readable on the physical matrix.

const uint8_t ROWS = 8;
const uint8_t COLS = 12;
const uint16_t COMMAND_HOLD_MS = 1800;

byte RenderFrame[ROWS][COLS];

const byte HeartBitmap[ROWS][COLS] = {
  {0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// Warmer-light / nuan_yidian: simple sun.
const byte SunBitmap[ROWS][COLS] = {
  {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
  {0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0},
  {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
};

// Cooler-light / leng_yidian: sparse snowflake/star.
const byte SnowflakeBitmap[ROWS][COLS] = {
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
};

// Brighter / liang_yidian: plus.
const byte PlusBitmap[ROWS][COLS] = {
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
};

// Dimmer / an_yidian: minus.
const byte MinusBitmap[ROWS][COLS] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

void showBitmap(const byte bitmap[ROWS][COLS]) {
  for (uint8_t row = 0; row < ROWS; row++) {
    for (uint8_t col = 0; col < COLS; col++) {
      RenderFrame[row][col] = bitmap[row][col];
    }
  }

  matrix.renderBitmap(RenderFrame, ROWS, COLS);
}

void showCommandBitmap(const byte bitmap[ROWS][COLS]) {
  showBitmap(bitmap);
  delay(COMMAND_HOLD_MS);
  showBitmap(HeartBitmap);
}

void setup() {
  matrix.begin();
  matrix.clear();
  showBitmap(HeartBitmap);

  Bridge.begin();
  Bridge.provide("warmer_light", warmer_light);
  Bridge.provide("cooler_light", cooler_light);
  Bridge.provide("dimmer", dimmer);
  Bridge.provide("brighter", brighter);
}

void loop() {}

void warmer_light() {
  showCommandBitmap(SunBitmap);
}

void cooler_light() {
  showCommandBitmap(SnowflakeBitmap);
}

void dimmer() {
  showCommandBitmap(MinusBitmap);
}

void brighter() {
  showCommandBitmap(PlusBitmap);
}
