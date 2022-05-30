/*
  This file is part of the ModbusRTUServer library.

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

#include <errno.h>

#include "libmodbus/modbus.h"
#include "libmodbus/modbus-rtu.h"

#include "ModbusServerClass.hpp"

/////////////////////////////
// CONSTRUCTORS/DESTRUCTOR //
/////////////////////////////

ModbusRTUServerClass::ModbusRTUServerClass(
    HardwareSerial &hwSerial,
    uint8_t tx_pin,
    uint8_t driver_enable_pin,
    uint8_t receiver_enable_pin) :

                                   RS485_{RS485Class(hwSerial, tx_pin, driver_enable_pin, receiver_enable_pin)},
                                   mb_(NULL)
{
  memset(&mbMapping_, 0x00, sizeof(mbMapping_));
}

ModbusRTUServerClass::~ModbusRTUServerClass()
{
  if (mbMapping_.tab_bits != NULL)
  {
    free(mbMapping_.tab_bits);
  }

  if (mbMapping_.tab_input_bits != NULL)
  {
    free(mbMapping_.tab_input_bits);
  }

  if (mbMapping_.tab_input_registers != NULL)
  {
    free(mbMapping_.tab_input_registers);
  }

  if (mbMapping_.tab_registers != NULL)
  {
    free(mbMapping_.tab_registers);
  }

  if (mb_ != NULL)
  {
    modbus_free(mb_);
  }
}

////////////
// PUBLIC //
////////////

// BEGIN/END //

int ModbusRTUServerClass::begin(int id, unsigned long baudrate, uint16_t config)
{
  // This test is kind of redundant, since `modbusBegin` won't return non-true, but that might change, so I'll leave here.
  if (!modbusBegin(id, baudrate, config))
  {
    return 0;
  }

  return 1;
}

void ModbusRTUServerClass::end()
{
  modbusEnd();
}

// MODBUS //

int ModbusRTUServerClass::poll()
{
  uint8_t request[MODBUS_RTU_MAX_ADU_LENGTH];

  int requestLength = modbus_receive(mb_, request);

  if (requestLength > 0)
  {
    modbus_reply(mb_, request, requestLength, &mbMapping_);
    return 1;
  }

  return 0;
}

void ModbusRTUServerClass::setRS485Pins(int tx_pin, int de_pin, int re_pin)
{
  modbus_set_rs485_pins(mb_, tx_pin, de_pin, re_pin);
}

int ModbusRTUServerClass::configureCoils(int start_address, int nb)
{
  if (start_address < 0 || nb < 1)
  {
    errno = EINVAL;

    return -1;
  }

  size_t s = sizeof(mbMapping_.tab_bits[0]) * nb;

  mbMapping_.tab_bits = (uint8_t *)realloc(mbMapping_.tab_bits, s);

  if (mbMapping_.tab_bits == NULL)
  {
    mbMapping_.start_bits = 0;
    mbMapping_.nb_bits = 0;

    return 0;
  }

  memset(mbMapping_.tab_bits, 0x00, s);
  mbMapping_.start_bits = start_address;
  mbMapping_.nb_bits = nb;

  return 1;
}

int ModbusRTUServerClass::configureDiscreteInputs(int start_address, int nb)
{
  if (start_address < 0 || nb < 1)
  {
    errno = EINVAL;

    return -1;
  }

  size_t s = sizeof(mbMapping_.tab_input_bits[0]) * nb;

  mbMapping_.tab_input_bits = (uint8_t *)realloc(mbMapping_.tab_input_bits, s);

  if (mbMapping_.tab_input_bits == NULL)
  {
    mbMapping_.start_input_bits = 0;
    mbMapping_.nb_input_bits = 0;

    return 0;
  }

  memset(mbMapping_.tab_input_bits, 0x00, s);
  mbMapping_.start_input_bits = start_address;
  mbMapping_.nb_input_bits = nb;

  return 1;
}

int ModbusRTUServerClass::configureHoldingRegisters(int start_address, int nb)
{
  if (start_address < 0 || nb < 1)
  {
    errno = EINVAL;

    return -1;
  }

  size_t s = sizeof(mbMapping_.tab_registers[0]) * nb;

  mbMapping_.tab_registers = (uint16_t *)realloc(mbMapping_.tab_registers, s);

  if (mbMapping_.tab_registers == NULL)
  {
    mbMapping_.start_registers = 0;
    mbMapping_.nb_registers = 0;

    return 0;
  }

  memset(mbMapping_.tab_registers, 0x00, s);
  mbMapping_.start_registers = start_address;
  mbMapping_.nb_registers = nb;

  return 1;
}

int ModbusRTUServerClass::configureInputRegisters(int start_address, int nb)
{
  if (start_address < 0 || nb < 1)
  {
    errno = EINVAL;

    return -1;
  }

  size_t s = sizeof(mbMapping_.tab_input_registers[0]) * nb;

  mbMapping_.tab_input_registers = (uint16_t *)realloc(mbMapping_.tab_input_registers, s);

  if (mbMapping_.tab_input_registers == NULL)
  {
    mbMapping_.start_input_registers = 0;
    mbMapping_.nb_input_registers = 0;

    return 0;
  }

  memset(mbMapping_.tab_input_registers, 0x00, s);
  mbMapping_.start_input_registers = start_address;
  mbMapping_.nb_input_registers = nb;

  return 1;
}

int ModbusRTUServerClass::coilRead(int address)
{
  if (mbMapping_.start_bits > address ||
      (mbMapping_.start_bits + mbMapping_.nb_bits) < (address + 1))
  {
    errno = EMBXILADD;

    return -1;
  }

  return mbMapping_.tab_bits[address - mbMapping_.start_bits];
}

int ModbusRTUServerClass::discreteInputRead(int address)
{
  if (mbMapping_.start_input_bits > address ||
      (mbMapping_.start_input_bits + mbMapping_.nb_input_bits) < (address + 1))
  {
    errno = EMBXILADD;

    return -1;
  }

  return mbMapping_.tab_input_bits[address - mbMapping_.start_input_bits];
}

long ModbusRTUServerClass::holdingRegisterRead(int address)
{
  if (mbMapping_.start_registers > address ||
      (mbMapping_.start_registers + mbMapping_.nb_registers) < (address + 1))
  {
    errno = EMBXILADD;

    return -1;
  }

  return mbMapping_.tab_registers[address - mbMapping_.start_registers];
}

long ModbusRTUServerClass::inputRegisterRead(int address)
{
  if (mbMapping_.start_input_registers > address ||
      (mbMapping_.start_input_registers + mbMapping_.nb_input_registers) < (address + 1))
  {
    errno = EMBXILADD;

    return -1;
  }

  return mbMapping_.tab_input_registers[address - mbMapping_.start_input_registers];
}

int ModbusRTUServerClass::coilWrite(int address, uint8_t value)
{
  if (mbMapping_.start_bits > address ||
      (mbMapping_.start_bits + mbMapping_.nb_bits) < (address + 1))
  {
    errno = EMBXILADD;

    return 0;
  }

  mbMapping_.tab_bits[address - mbMapping_.start_bits] = value;

  return 1;
}

int ModbusRTUServerClass::holdingRegisterWrite(int address, uint16_t value)
{
  if (mbMapping_.start_registers > address ||
      (mbMapping_.start_registers + mbMapping_.nb_registers) < (address + 1))
  {
    errno = EMBXILADD;

    return 0;
  }

  mbMapping_.tab_registers[address - mbMapping_.start_registers] = value;

  return 1;
}

int ModbusRTUServerClass::registerMaskWrite(int address, uint16_t and_mask, uint16_t or_mask)
{
  long value = holdingRegisterRead(address);

  if (value < 0)
  {
    return 0;
  }

  value &= and_mask;
  value |= or_mask;

  if (!holdingRegisterWrite(address, value))
  {
    return 0;
  }

  return 1;
}

int ModbusRTUServerClass::discreteInputWrite(int address, uint8_t value)
{
  if (mbMapping_.start_input_bits > address ||
      (mbMapping_.start_input_bits + mbMapping_.nb_input_bits) < (address + 1))
  {
    errno = EMBXILADD;

    return 0;
  }

  mbMapping_.tab_input_bits[address - mbMapping_.start_input_bits] = value;

  return 1;
}

int ModbusRTUServerClass::inputRegisterWrite(int address, uint16_t value)
{
  if (mbMapping_.start_input_registers > address ||
      (mbMapping_.start_input_registers + mbMapping_.nb_input_registers) < (address + 1))
  {
    errno = EMBXILADD;

    return 0;
  }

  mbMapping_.tab_input_registers[address - mbMapping_.start_input_registers] = value;

  return 1;
}

/////////////
// PRIVATE //
/////////////

// MODBUS //

int ModbusRTUServerClass::modbusBegin(int id, unsigned long baudrate, uint16_t config)
{
  modbusEnd();

  mb_ = modbus_new_rtu(RS485_, baudrate, config);

  modbus_set_slave(mb_, id);

  modbus_connect(mb_);

  return 1;
}

void ModbusRTUServerClass::modbusEnd()
{
  if (mbMapping_.tab_bits != NULL)
  {
    free(mbMapping_.tab_bits);
  }

  if (mbMapping_.tab_input_bits != NULL)
  {
    free(mbMapping_.tab_input_bits);
  }

  if (mbMapping_.tab_input_registers != NULL)
  {
    free(mbMapping_.tab_input_registers);
  }

  if (mbMapping_.tab_registers != NULL)
  {
    free(mbMapping_.tab_registers);
  }

  memset(&mbMapping_, 0x00, sizeof(mbMapping_));

  if (mb_ != NULL)
  {
    modbus_close(mb_);
    modbus_free(mb_);

    mb_ = NULL;
  }
}
