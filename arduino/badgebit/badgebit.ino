/**
 * badgebit.ino
 * Copyright 2013 Mike Wakerly <opensource@hoho.com>
 *
 * This file is part of the Badgebit project.  For more information, see
 * http://github.com/mik3y/badgebit/
 *
 * Badgebit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Badgebit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Badgebit.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <TinyWireM.h>
#include <USI_TWI_Master.h>
#include <PinChangeInterrupt.h>
#include "Wiegand.h"

#define PIN_DATA_0 4
#define PIN_DATA_1 5
#define PIN_OUTPUT 3
#define PIN_BUTTON 2
#define PIN_LED_RED 1
#define PIN_LED_GREEN 0

#define WIEGAND_TIMEOUT_MILLIS 1000

typedef unsigned char uint8_t;

static Wiegand gWiegand;
static unsigned long gLastWiegandInterruptMillis = 0;

static void onWiegandData0RisingEdge() {
  gWiegand.handleData0Pulse();
  gLastWiegandInterruptMillis = millis();
}

static void onWiegandData1RisingEdge() {
  gWiegand.handleData1Pulse();
  gLastWiegandInterruptMillis = millis();
}

static void onButtonRisingEdge() {
  gWiegand.handleData1Pulse();
  gLastWiegandInterruptMillis = millis();
}

static void handleBadge(uint8_t *buf) {
  // TODO: Send badge ID over serial.
  digitalWrite(PIN_OUTPUT, HIGH);
  delay(500);
  digitalWrite(PIN_OUTPUT, LOW);
}

static void processWiegand() {
  if (gLastWiegandInterruptMillis == 0) {
    return;
  }
  unsigned long now = millis();

  if ((now - gLastWiegandInterruptMillis) > WIEGAND_TIMEOUT_MILLIS) {
    uint8_t buf[WIEGAND_BUFSIZ];
    int num_bits = gWiegand.getData(buf);
    if (num_bits > 0) {
      handleBadge(buf);
    }
    gWiegand.reset();
    gLastWiegandInterruptMillis = 0;
  }
}

// Arduino entry points.

void setup() {
  pinMode(PIN_DATA_0, INPUT);
  digitalWrite(PIN_DATA_0, HIGH);
  //attachPcInterrupt(PIN_DATA_0, onWiegandData0RisingEdge, RISING);

  pinMode(PIN_DATA_1, INPUT);
  digitalWrite(PIN_DATA_1, HIGH);
  //attachPcInterrupt(PIN_DATA_1, onWiegandData1RisingEdge, RISING);

  gWiegand.reset();
  gLastWiegandInterruptMillis = 0;

  pinMode(PIN_OUTPUT, OUTPUT);
  digitalWrite(PIN_BUTTON, LOW);

  pinMode(PIN_BUTTON, INPUT);
  digitalWrite(PIN_BUTTON, HIGH);
  //attachPcInterrupt(PIN_DATA_1, onButtonRisingEdge, RISING);

  pinMode(PIN_LED_RED, OUTPUT);
  digitalWrite(PIN_LED_RED, HIGH);

  pinMode(PIN_LED_GREEN, OUTPUT);
  digitalWrite(PIN_LED_GREEN, HIGH);
}

void loop() {
  processWiegand();
}

// vim: syntax=c
