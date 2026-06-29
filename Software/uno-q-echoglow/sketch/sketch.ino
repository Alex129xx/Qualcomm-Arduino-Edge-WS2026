// SPDX-FileCopyrightText: Copyright (C) Electronic Cats
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>

Arduino_LED_Matrix matrix;

// The UNO Q LED matrix is 8 rows x 12 columns.
// Using renderBitmap() keeps the icons readable and avoids confusing
// packed hexadecimal frame orientation.

// Default idle display: heart.
byte HeartBitmap[8][12] = {
  {0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0},
  {0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
};

// Warmer-light: sun icon.
byte SunBitmap[8][12] = {
  {0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
  {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
  {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0},
};

// Cooler-light: snowflake icon.
byte SnowflakeBitmap[8][12] = {
  {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1},
  {0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0},
  {0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0},
  {0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0},
  {0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0},
  {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1},
  {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
};

// Brighter: plus icon.
byte PlusBitmap[8][12] = {
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
};

// Dimmer: minus icon.
byte MinusBitmap[8][12] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

void showBitmap(byte bitmap[8][12]) {
  matrix.renderBitmap(bitmap, 8, 12);
}

void showCommandBitmap(byte bitmap[8][12], uint16_t holdMs = 1200) {
  showBitmap(bitmap);
  delay(holdMs);
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
