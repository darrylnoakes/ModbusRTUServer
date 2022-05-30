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

#ifndef _MODBUS_RTU_SERVER_SRC_MODBUS_SERVER_CLASS_HPP
#define _MODBUS_RTU_SERVER_SRC_MODBUS_SERVER_CLASS_HPP

#include <Arduino.h>

#include "libmodbus/modbus.h"
#include "libmodbus/modbus-rtu.h"

class ModbusRTUServerClass
{
public:
  ModbusRTUServerClass(
      HardwareSerial &hwSerial,
      uint8_t tx_pin,
      uint8_t driver_enable_pin,
      uint8_t receiver_enable_pin);
  ~ModbusRTUServerClass();

  int begin(int id, unsigned long baudrate = 19200, uint16_t config = SERIAL_8N1);
  void end();

  void setRS485Pins(int tx_pin, int de_pin, int re_pin);

  /**
   * Poll interface for requests
   * 
   * Return 1 if a message was received, 0 otherwise
   */
  int poll();

  /**
   * Configure the servers coils.
   *
   * @param start_address start address of coils
   * @param nb number of coils to configure
   *
   * @return 0 on success, 1 on failure
   */
  int configureCoils(int start_address, int nb);

  /**
   * Configure the servers discrete inputs.
   *
   * @param start_address start address of discrete inputs
   * @param nb number of discrete inputs to configure
   *
   * @return 0 on success, 1 on failure
   */
  int configureDiscreteInputs(int start_address, int nb);

  /**
   * Configure the servers holding registers.
   *
   * @param start_address start address of holding registers
   * @param nb number of holding registers to configure
   *
   * @return 1 on success, 0 or -1 on failure (-1 is for incorrect parameters, the most likely case)
   */
  int configureHoldingRegisters(int start_address, int nb);

  /**
   * Configure the servers input registers.
   *
   * @param start_address start address of input registers
   * @param nb number of input registers to configure
   *
   * @return 0 on success, 1 on failure
   */
  int configureInputRegisters(int start_address, int nb);

  // same as ModbusClientClass.h
  int coilRead(int address);
  int discreteInputRead(int address);
  long holdingRegisterRead(int address);
  long inputRegisterRead(int address);
  int coilWrite(int address, uint8_t value);
  int holdingRegisterWrite(int address, uint16_t value);
  int registerMaskWrite(int address, uint16_t and_mask, uint16_t or_mask);

  /**
   * Write the value of the server's Discrete Input for the specified address
   * and value.
   *
   * @param address address to use for operation
   * @param value discrete input value to write
   *
   * @return 1 on success, 0 on failure.
   */
  int discreteInputWrite(int address, uint8_t value);

  /**
   * Write the value of the server's Input Register for the specified address
   * and value.
   *
   * @param address address to use for operation
   * @param value input register value to write
   *
   * @return 1 on success, 0 on failure.
   */
  int inputRegisterWrite(int address, uint16_t value);

private:
  RS485Class RS485_;

  modbus_t *mb_;
  modbus_mapping_t mbMapping_;

  /**
   * Start the Modbus RTU server with the specified parameters
   *
   * @param id (slave) id of the server
   * @param baudrate Baud rate to use
   * @param config serial config. to use defaults to SERIAL_8N1
   *
   * Return 1 on success, 0 on failure
   */
  int modbusBegin(int id, unsigned long baudrate, uint16_t config);

  /**
   * Stop the server
   */
  void modbusEnd();
};

#endif
