/*
 * Copyright © 2001-2011 Stéphane Raimbault <stephane.raimbault@gmail.com>
 * Copyright © 2018 Arduino SA. All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include "modbus.h"

#include "ArduinoRS485ClassMod/ArduinoRS485ClassMod.h"

MODBUS_BEGIN_DECLS

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * RS232 / RS485 ADU = 253 bytes + slave (1 byte) + CRC (2 bytes) = 256 bytes
 */
#define MODBUS_RTU_MAX_ADU_LENGTH  256

MODBUS_API modbus_t* modbus_new_rtu(RS485Class &rs485, unsigned long baud, uint16_t config);

MODBUS_API void modbus_set_rs485_pins(modbus_t *ctx, int tx_pin, int de_pin, int re_pin);

MODBUS_END_DECLS

#endif /* MODBUS_RTU_H */
