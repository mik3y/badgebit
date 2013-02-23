badgebit
========

## What

Badgebit turns a [Digispark](http://www.digistump.com) (tiny AVR device)
into aa Wiegand/HID-compatible badge access control device.

Features:
* Wiegand-compatible 2-wire reader interface.  Supports many devices (ie OmniProx OP40, ~$30 on ebay).
* General-purpose output triggered on badge recognition.
* Can be used without a host computer (standalone mode).
* Single-button "learn mode" for storing or clearing known badge IDs.

## Hardware

Board schematic and layout are [on upverter](http://upverter.com/mik3y/66e4df5b5d819d7e/BadgeBit).

## Software

Arduino/Digispark-compatible program is in this tree.

## Who

* anupam pathak
* mike wakerly

## Why

[Upverter](http://www.upverter.com/) is a Bay Area tech startup that makes an
online EDA tool.

We were interested in trying out their tool, so we entered their hackathon and
came up with this contrived project as something possibly feasible in an
afternoon.
