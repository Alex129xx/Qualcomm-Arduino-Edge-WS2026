// SPDX-FileCopyrightText: Copyright (C) Electronic Cats
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>

Arduino_LED_Matrix matrix;

const uint32_t IdleFrame[4] = {
  0x00030c79,
  0xe7fe3fc1,
  0xf80f0060,
  0x00000000,
};

const uint32_t WarmFrame[4] = {
  0x0602641f,
  0x83fc3fc1,
  0xf8264060,
  0x00000000,
};

const uint32_t CoolFrame[4] = {
  0x06026416,
  0x8ffffff1,
  0x68264060,
  0x00000000,
};

const uint32_t BrightFrame[4] = {
  0xffffffff,
  0xffffffff,
  0xffffffff,
  0x00000000,
};

const uint32_t DimFrame[4] = {
  0x00000000,
  0x00600600,
  0x00000000,
  0x00000000,
};

void showFrame(const uint32_t frame[4], uint16_t holdMs = 1000) {
  matrix.loadFrame(frame);
  delay(holdMs);
}

void blinkFrame(const uint32_t frame[4], uint8_t times, uint16_t onMs, uint16_t offMs) {
  for (uint8_t i = 0; i < times; i++) {
    matrix.loadFrame(frame);
    delay(onMs);
    matrix.clear();
    delay(offMs);
  }
  matrix.loadFrame(IdleFrame);
}

void setup() {
  matrix.begin();
  matrix.clear();
  matrix.loadFrame(IdleFrame);

  Bridge.begin();
  Bridge.provide("warmer_light", warmer_light);
  Bridge.provide("cooler_light", cooler_light);
  Bridge.provide("dimmer", dimmer);
  Bridge.provide("brighter", brighter);
}

void loop() {}

void warmer_light() {
  blinkFrame(WarmFrame, 2, 350, 120);
}

void cooler_light() {
  blinkFrame(CoolFrame, 2, 350, 120);
}

void dimmer() {
  showFrame(DimFrame, 900);
  matrix.loadFrame(IdleFrame);
}

void brighter() {
  blinkFrame(BrightFrame, 2, 180, 120);
}
