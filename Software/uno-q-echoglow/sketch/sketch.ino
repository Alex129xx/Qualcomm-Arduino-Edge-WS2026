// SPDX-FileCopyrightText: Copyright (C) Electronic Cats
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>

Arduino_LED_Matrix matrix;

// The official UNO Q LED Matrix Painter example treats the onboard matrix as
// an 8-row x 13-column grayscale buffer and renders it with matrix.draw().
const uint8_t ROWS = 8;
const uint8_t COLS = 13;
const uint8_t PIXELS = ROWS * COLS;
const uint8_t LED_ON = 7;
const uint16_t COMMAND_HOLD_MS = 1800;

uint8_t frame[PIXELS];

void clearFrame() {
  for (uint8_t i = 0; i < PIXELS; i++) {
    frame[i] = 0;
  }
}

void setPixel(uint8_t row, uint8_t col) {
  if (row >= ROWS || col >= COLS) return;
  frame[row * COLS + col] = LED_ON;
}

void hLine(uint8_t row, uint8_t colStart, uint8_t colEnd) {
  for (uint8_t col = colStart; col <= colEnd && col < COLS; col++) {
    setPixel(row, col);
  }
}

void vLine(uint8_t col, uint8_t rowStart, uint8_t rowEnd) {
  for (uint8_t row = rowStart; row <= rowEnd && row < ROWS; row++) {
    setPixel(row, col);
  }
}

void showFrame() {
  matrix.draw(frame);
}

void drawHeart() {
  clearFrame();
  hLine(0, 3, 4);
  hLine(0, 8, 9);
  hLine(1, 2, 5);
  hLine(1, 7, 10);
  hLine(2, 1, 11);
  hLine(3, 1, 11);
  hLine(4, 2, 10);
  hLine(5, 3, 9);
  hLine(6, 4, 8);
  hLine(7, 5, 7);
}

void drawSun() {
  clearFrame();
  setPixel(0, 3);
  setPixel(0, 6);
  setPixel(0, 9);
  setPixel(1, 4);
  setPixel(1, 6);
  setPixel(1, 8);
  setPixel(2, 1);
  hLine(2, 4, 8);
  setPixel(2, 11);
  hLine(3, 2, 10);
  hLine(4, 2, 10);
  setPixel(5, 1);
  hLine(5, 4, 8);
  setPixel(5, 11);
  setPixel(6, 4);
  setPixel(6, 6);
  setPixel(6, 8);
  setPixel(7, 3);
  setPixel(7, 6);
  setPixel(7, 9);
}

void drawSnowflake() {
  clearFrame();
  vLine(6, 0, 7);
  hLine(3, 3, 9);
  hLine(4, 3, 9);
  setPixel(1, 1);
  setPixel(2, 2);
  setPixel(5, 10);
  setPixel(6, 11);
  setPixel(1, 11);
  setPixel(2, 10);
  setPixel(5, 2);
  setPixel(6, 1);
  setPixel(2, 5);
  setPixel(2, 7);
  setPixel(5, 5);
  setPixel(5, 7);
}

void drawPlus() {
  clearFrame();
  vLine(6, 0, 7);
  hLine(3, 2, 10);
  hLine(4, 2, 10);
}

void drawMinus() {
  clearFrame();
  hLine(3, 2, 10);
  hLine(4, 2, 10);
}

void showCommand(void (*drawIcon)()) {
  drawIcon();
  showFrame();
  delay(COMMAND_HOLD_MS);
  drawHeart();
  showFrame();
}

void setup() {
  matrix.begin();
  matrix.setGrayscaleBits(3);
  matrix.clear();
  drawHeart();
  showFrame();

  Bridge.begin();
  Bridge.provide("warmer_light", warmer_light);
  Bridge.provide("cooler_light", cooler_light);
  Bridge.provide("dimmer", dimmer);
  Bridge.provide("brighter", brighter);
}

void loop() {}

void warmer_light() {
  showCommand(drawSun);
}

void cooler_light() {
  showCommand(drawSnowflake);
}

void dimmer() {
  showCommand(drawMinus);
}

void brighter() {
  showCommand(drawPlus);
}
