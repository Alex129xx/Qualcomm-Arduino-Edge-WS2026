// SPDX-FileCopyrightText: Copyright (C) Electronic Cats
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>

Arduino_LED_Matrix matrix;

// The Arduino LED matrix API is addressed as 8 rows x 12 columns.
// On the physical UNO Q board, the LEDs are staggered, so the display looks
// more like an 8 x 13 visual grid. The icons below are therefore drawn on an
// 8 x 13 visual canvas and then mapped back to the 8 x 12 matrix expected by
// renderBitmap().

const uint8_t ROWS = 8;
const uint8_t LOGICAL_COLS = 12;
const uint8_t VISUAL_COLS = 13;

byte RenderFrame[ROWS][LOGICAL_COLS];

// Default idle display: heart.
const byte HeartBitmap[ROWS][VISUAL_COLS] = {
  {0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
};

// Warmer-light / nuan_yidian: sun icon.
const byte SunBitmap[ROWS][VISUAL_COLS] = {
  {0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0},
  {0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0},
  {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0},
};

// Cooler-light / leng_yidian: snowflake icon.
const byte SnowflakeBitmap[ROWS][VISUAL_COLS] = {
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
};

// Brighter / liang_yidian: plus icon.
const byte PlusBitmap[ROWS][VISUAL_COLS] = {
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
};

// Dimmer / an_yidian: minus icon.
const byte MinusBitmap[ROWS][VISUAL_COLS] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

void showVisualBitmap(const byte visualBitmap[ROWS][VISUAL_COLS]) {
  for (uint8_t row = 0; row < ROWS; row++) {
    // Odd physical rows are visually shifted by one column. Selecting a
    // different 12-column window for odd rows keeps vertical features aligned
    // on the staggered physical matrix.
    const uint8_t visualStart = row % 2;

    for (uint8_t col = 0; col < LOGICAL_COLS; col++) {
      RenderFrame[row][col] = visualBitmap[row][col + visualStart];
    }
  }

  matrix.renderBitmap(RenderFrame, ROWS, LOGICAL_COLS);
}

void showCommandBitmap(const byte bitmap[ROWS][VISUAL_COLS], uint16_t holdMs = 1200) {
  showVisualBitmap(bitmap);
  delay(holdMs);
  showVisualBitmap(HeartBitmap);
}

void setup() {
  matrix.begin();
  matrix.clear();
  showVisualBitmap(HeartBitmap);

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
