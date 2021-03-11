# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased

## Changed

- The internal library `ArduinoRS485ClassMod` was renamed to `RS485Class`.

- In `RS485Class`:
    All baudrate variables and parameters are now `long` instead of `unsigned long`.
    This is because `HardwareSerial.begin` only takes a `long` for baudrate.

## Fixed

- In `RS485Class`:
    `_baudrate` was not initialized in the constructor. The `begin` functions set it, but `sendBreak` and `sendBreakMicroseconds` did not. Calling them without calling `begin` before would cause the `HardwareSerial` passed in to the constructor to have its `begin` function called with a garbage baudrate. `_baudrate` is now initialized to `-1`, and `begin`, `sendBreak`, and `sendBreakMicroseconds` do nothing if the baudrate is less than `0`.

- Changed `#include` in `libmodbus/modbus-rtu-private.h` to the new path for `RS485Class`.

- Added parenthesis to clarify operator precedence:
    This:
    `dest[i] = tab_byte[(i - idx) / 8] & (1 << shift) ? 1 : 0;`
    became this:
    `dest[i] = (tab_byte[(i - idx) / 8] & (1 << shift)) ? 1 : 0;`

- Fixed the same expression appearing on both side of an `||` in an `if` in `libmodbus/modbus.c`.

## 0.0.0 - 2020-10-16

### Added

- Initial commit of everything to the new repository.
    The code is based on my other library, `DynamicModbusRTUServerClass`, forked from its version 4.0.0.
