// badgebit.ino
// Copyright 2013 The Unwashed Friars.
// License: GPLv2.  See LICENSE.txt
// Homepage: http://github.com/mik3y/badgebit

// Overview

// The main loop of this program continuously services the Wiegand bus.
// Two pin change interrupts, one for each Wiegand pin, accumulate data
// bits.
//
// A Wiegand message is considered complete after WIEGAND_TIMEOUT_MILLIS
// have elapsed with no pulses.

// Program Configuration

// Pins.
#define PIN_DATA_0 4
#define PIN_DATA_1 5
#define PIN_OUTPUT 3
#define PIN_BUTTON 2
#define PIN_LED_RED 1
#define PIN_LED_GREEN 0

// Timeout after which a Wiegand message is considered complete.
#define WIEGAND_TIMEOUT_MILLIS 1000

// Delay during which the output pin will be held high.
#define OUTPUT_DELAY_MILLIS 1000

// Main program -- no changes necessary after this line.

#include <TinyWireM.h>
#include <USI_TWI_Master.h>
#include <PinChangeInterrupt.h>
#include "Wiegand.h"

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
  digitalWrite(PIN_OUTPUT, HIGH);
  delay(OUTPUT_DELAY_MILLIS);
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
  attachPcInterrupt(PIN_DATA_0, onWiegandData0RisingEdge, RISING);

  pinMode(PIN_DATA_1, INPUT);
  digitalWrite(PIN_DATA_1, HIGH);
  attachPcInterrupt(PIN_DATA_1, onWiegandData1RisingEdge, RISING);

  gWiegand.reset();
  gLastWiegandInterruptMillis = 0;

  pinMode(PIN_OUTPUT, OUTPUT);
  digitalWrite(PIN_BUTTON, LOW);

  pinMode(PIN_BUTTON, INPUT);
  digitalWrite(PIN_BUTTON, HIGH);
  attachPcInterrupt(PIN_DATA_1, onButtonRisingEdge, RISING);

  pinMode(PIN_LED_RED, OUTPUT);
  digitalWrite(PIN_LED_RED, HIGH);

  pinMode(PIN_LED_GREEN, OUTPUT);
  digitalWrite(PIN_LED_GREEN, HIGH);
}

void loop() {
  processWiegand();
}

// vim: syntax=c
