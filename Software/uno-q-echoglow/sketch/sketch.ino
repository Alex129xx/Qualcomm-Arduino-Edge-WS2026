// SPDX-FileCopyrightText: Copyright (C) Electronic Cats
//
// SPDX-License-Identifier: MPL-2.0

#include <Arduino_LED_Matrix.h>
#include <Arduino_RouterBridge.h>

Arduino_LED_Matrix matrix;

// Default idle display: heart.
const uint32_t HeartFrame[4] = {
  0x00030c79,
  0xe7fe3fc1,
  0xf80f0060,
  0x00000000,
};

// Warmer-light: sun icon.
const uint32_t SunFrame[4] = {
  0x0602641f,
  0x83fc3fc1,
  0xf8264060,
  0x00000000,
};

// Cooler-light: snowflake icon.
const uint32_t SnowflakeFrame[4] = {
  0x06026416,
  0x80f00f01,
  0x68264060,
  0x00000000,
};

// Brighter: plus icon.
const uint32_t PlusFrame[4] = {
  0x00006006,
  0x01f81f80,
  0x60060000,
  0x00000000,
};

// Dimmer: minus icon.
const uint32_t MinusFrame[4] = {
  0x00000000,
  0x01f81f80,
  0x00000000,
  0x00000000,
};

void showCommandFrame(const uint32_t frame[4], uint16_t holdMs = 1200) {
  matrix.loadFrame(frame);
  delay(holdMs);
  matrix.loadFrame(HeartFrame);
}

void setup() {
  matrix.begin();
  matrix.clear();
  matrix.loadFrame(HeartFrame);

  Bridge.begin();
  Bridge.provide("warmer_light", warmer_light);
  Bridge.provide("cooler_light", cooler_light);
  Bridge.provide("dimmer", dimmer);
  Bridge.provide("brighter", brighter);
}

void loop() {}

void warmer_light() {
  showCommandFrame(SunFrame);
}

void cooler_light() {
  showCommandFrame(SnowflakeFrame);
}

void dimmer() {
  showCommandFrame(MinusFrame);
}

void brighter() {
  showCommandFrame(PlusFrame);
}
