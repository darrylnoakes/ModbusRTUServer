/*
  This file is part of the ArduinoRS485 library.

  Copyright (c) 2018 Arduino SA. All rights reserved.
  Copyright (c) 2022 Darryl Noakes <darryl.noakes@gmail.com>

  This program is free software: you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "RS485.h"

RS485Class::RS485Class(HardwareSerial& hwSerial, int txPin, int dePin, int rePin) :
  _serial(&hwSerial),
  _txPin(txPin),
  _dePin(dePin),
  _rePin(rePin),
  _haveInit(false),
  _transmisionBegun(false),
  _baudrate(0)
{
}

void RS485Class::begin(unsigned long baudrate)
{
  begin(baudrate, SERIAL_8N1);
}

void RS485Class::begin(unsigned long baudrate, uint16_t config)
{
  _baudrate = baudrate;
  _config = config;
  _haveInit = true;

  if (_dePin > -1) {
    pinMode(_dePin, OUTPUT);
    digitalWrite(_dePin, LOW);
  }

  if (_rePin > -1) {
    pinMode(_rePin, OUTPUT);
    digitalWrite(_rePin, HIGH);
  }

  _transmisionBegun = false;

  _serial->begin(baudrate, config);
}

void RS485Class::end()
{
  _serial->end();

  if (_rePin > -1) {
    digitalWrite(_rePin, LOW);
    pinMode(_rePin, INPUT);
  }
  
  if (_dePin > -1) {
    digitalWrite(_dePin, LOW);
    pinMode(_dePin, INPUT);
  }
}

int RS485Class::available()
{
  return _serial->available();
}

int RS485Class::peek()
{
  return _serial->peek();
}

int RS485Class::read(void)
{
  return _serial->read();
}

void RS485Class::flush()
{
  return _serial->flush();
}

size_t RS485Class::write(uint8_t b)
{
  if (!_transmisionBegun) {
    setWriteError();
    return 0;
  }

  return _serial->write(b);
}

RS485Class::operator bool()
{
  return true;
}

void RS485Class::beginTransmission()
{
  if (_dePin > -1) {
    digitalWrite(_dePin, HIGH);
    delayMicroseconds(50);
  }

  _transmisionBegun = true;
}

void RS485Class::endTransmission()
{
  _serial->flush();

  if (_dePin > -1) {
    digitalWrite(_dePin, LOW);
    delayMicroseconds(50);
  }

  _transmisionBegun = false;
}

void RS485Class::receive()
{
  if (_rePin > -1) {
    digitalWrite(_rePin, LOW);
  }
}

void RS485Class::noReceive()
{
  if (_rePin > -1) {
    digitalWrite(_rePin, HIGH);
  }
}

void RS485Class::sendBreak(unsigned int duration)
{
  if (_haveInit) {
    _serial->flush();
    _serial->end();
    pinMode(_txPin, OUTPUT);
    digitalWrite(_txPin, LOW);
    delay(duration);
    _serial->begin(_baudrate, _config);
  }
}

void RS485Class::sendBreakMicroseconds(unsigned int duration)
{
  if (_haveInit) {
    _serial->flush();
    _serial->end();
    pinMode(_txPin, OUTPUT);
    digitalWrite(_txPin, LOW);
    delayMicroseconds(duration);
    _serial->begin(_baudrate, _config);
  }
}

void RS485Class::setPins(int txPin, int dePin, int rePin)
{
  endTransmission();

/* THIS CAN INTERFERE WITH OTHER CALLS TO `pinMode` by "resetting" pins.
  if (_rePin > -1) {
    digitalWrite(_rePin, LOW);
    pinMode(_rePin, INPUT);
  }
  
  if (_dePin > -1) {
    digitalWrite(_dePin, LOW);
    pinMode(_dePin, INPUT);
  }
*/

  _txPin = txPin;
  _dePin = dePin;
  _rePin = rePin;

  if (_dePin > -1) {
    pinMode(_dePin, OUTPUT);
    digitalWrite(_dePin, LOW);
  }

  if (_rePin > -1) {
    pinMode(_rePin, OUTPUT);
    digitalWrite(_rePin, LOW);
  }
}
