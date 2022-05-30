/*
 * Copyright © 2001-2011 Stéphane Raimbault <stephane.raimbault@gmail.com>
 * Copyright © 2018 Arduino SA. All rights reserved.
 * Copyright © 2022 Darryl Noakes <darryl.noakes@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef MODBUS_RTU_PRIVATE_H
#define MODBUS_RTU_PRIVATE_H

#ifndef _MSC_VER
#include <stdint.h>
#else
#include "stdint.h"
#endif

#include "RS485Class/RS485Class.h"

#define _MODBUS_RTU_HEADER_LENGTH      1
#define _MODBUS_RTU_PRESET_REQ_LENGTH  6
#define _MODBUS_RTU_PRESET_RSP_LENGTH  2

#define _MODBUS_RTU_CHECKSUM_LENGTH    2

typedef struct _modbus_rtu {

    RS485Class *rs485;

    unsigned long baud;
    uint16_t config;

    /* To handle many slaves on the same link */
    int confirmation_to_ignore;
    
} modbus_rtu_t;

#endif /* MODBUS_RTU_PRIVATE_H */
