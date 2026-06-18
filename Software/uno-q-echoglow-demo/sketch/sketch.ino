// SPDX-FileCopyrightText: Copyright (C) Electronic Cats
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>

#include "heart_frames.h"

Arduino_LED_Matrix matrix;

const uint32_t WarmFrame[4] = {
  0x07009209,
  0xc851428a,
  0x13904900,
  0xe0000000,
};

const uint32_t CoolFrame[4] = {
  0x02011104,
  0x901f00f8,
  0x09208880,
  0x40000000,
};

const uint32_t BrightFrame[4] = {
  0x8f88fe0f,
  0xf8ffe7ff,
  0x1ff07f11,
  0xf1000000,
};

const uint32_t DimFrame[4] = {
  0x00000000,
  0x800e0070,
  0x01000000,
  0x00000000,
};

void setBuiltinLed(bool on) {
  // The built-in LED is only a tiny confirmation indicator in this demo.
  // The main feedback is the onboard LED Matrix.
  digitalWrite(LED_BUILTIN, on ? LOW : HIGH);
}

void pulseBuiltinLed(int onMs = 80) {
  setBuiltinLed(true);
  delay(onMs);
  setBuiltinLed(false);
}

void showFrame(const uint32_t frame[4]) {
  matrix.clear();
  delay(30);
  matrix.loadFrame(frame);
}

void showHeart() {
  showFrame(HeartStatic);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  setBuiltinLed(false);

  matrix.begin();
  matrix.clear();
  showHeart();

  Bridge.begin();
  Bridge.provide("warmer_light", warmer_light);
  Bridge.provide("cooler_light", cooler_light);
  Bridge.provide("dimmer", dimmer);
  Bridge.provide("brighter", brighter);
  Bridge.provide("reset_demo", reset_demo);
  Bridge.provide("clear_matrix", clear_matrix);
}

void loop() {
}

void warmer_light() {
  showFrame(WarmFrame);
  pulseBuiltinLed(80);
}

void cooler_light() {
  showFrame(CoolFrame);
  pulseBuiltinLed(80);
}

void dimmer() {
  showFrame(DimFrame);
  pulseBuiltinLed(80);
}

void brighter() {
  showFrame(BrightFrame);
  pulseBuiltinLed(80);
}

void reset_demo() {
  setBuiltinLed(false);
  showHeart();
}

void clear_matrix() {
  setBuiltinLed(false);
  matrix.clear();
}
