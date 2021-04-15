# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased

## Changed

- The internal library `ArduinoRS485ClassMod` was renamed to `RS485Class`.

- Changed `#include`s in `libmodbus` to the new path for `RS485Class`.

## Fixed

- In `RS485Class`:
    `_baudrate` was not initialized in the constructor. The `begin` functions sets it,
    but `sendBreak` and `sendBreakMicroseconds` did not detect whether it was set or not.
    Calling them without calling `begin` beforehand would cause the `HardwareSerial` passed
    in to the constructor to have its `begin` function called with a garbage baudrate.
    A variable called `_haveInit` has been added, which is initialized to `false`, and set
    to `true` when `begin` is called. `_baudrate` is now initialized to `0`. `sendBreak`
    and `sendBreakMicroseconds` now do nothing if `_haveInit` is `false`.

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
