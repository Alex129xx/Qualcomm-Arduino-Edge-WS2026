// SPDX-FileCopyrightText: Copyright (C) Electronic Cats
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>

#include "heart_frames.h"

Arduino_LED_Matrix matrix;

void setBuiltinLed(bool on) {
  // The onboard LED is active-low on UNO Q examples.
  digitalWrite(LED_BUILTIN, on ? LOW : HIGH);
}

void flashBuiltinLed(int count, int onMs = 80, int offMs = 80) {
  for (int i = 0; i < count; i++) {
    setBuiltinLed(true);
    delay(onMs);
    setBuiltinLed(false);
    delay(offMs);
  }
}

void showHeart() {
  matrix.loadFrame(HeartStatic);
}

void animateHeart() {
  matrix.loadSequence(HeartAnim);
  matrix.playSequence();
  delay(650);
  matrix.loadFrame(HeartStatic);
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
  showHeart();
  flashBuiltinLed(1, 180, 120);
}

void cooler_light() {
  matrix.clear();
  delay(150);
  showHeart();
  flashBuiltinLed(2, 90, 90);
}

void dimmer() {
  matrix.clear();
  flashBuiltinLed(4, 50, 60);
  delay(200);
  showHeart();
}

void brighter() {
  animateHeart();
  flashBuiltinLed(3, 70, 70);
}

void reset_demo() {
  setBuiltinLed(false);
  showHeart();
}

void clear_matrix() {
  setBuiltinLed(false);
  matrix.clear();
}
