/*
 * FreeModbus Libary: ESP8266 Port
 * Copyright (C) 2017 Toni Naukkarinen <toni.naukkarinen@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: port.h,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#ifndef _PORT_H
#define _PORT_H

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include "c_types.h"
#include "gpio.h"
#include "esp8266/eagle_soc.h"
#include "esp8266/ets_sys.h"
#include "esp8266/uart_register.h"
#include "freertos/FreeRTOS.h"
#include "uart.h"

#define	INLINE                      inline
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION( )   portENTER_CRITICAL()
#define EXIT_CRITICAL_SECTION( )    portEXIT_CRITICAL()

typedef unsigned char UCHAR;
typedef char CHAR;

typedef uint16_t USHORT;
typedef int16_t SHORT;

typedef uint32_t ULONG;
typedef int32_t LONG;

// Required stub functions for ESP8266/newlib
int _getpid_r();
int _kill_r();

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

#endif // _PORT_H
