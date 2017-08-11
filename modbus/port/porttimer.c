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
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "esp_timer.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ---------------------- variables -----------------------------------------*/
uint16_t timeout;
os_timer_t modbustimer;
/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
    timeout = usTim1Timerout50us;
    // Initialize the software timer
    os_timer_setfn(&modbustimer, (os_timer_func_t*)prvvTIMERExpiredISR, NULL);
    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
  /*
     Documentation shows os_timer_arm_us only in NonOS SDK API reference, but it seems
     to work fine under RTOS SDK as well. I checked the pulse width with logic analyzer.
  */
  os_timer_arm_us(&modbustimer, timeout*50, false);
}

inline void
vMBPortTimersDisable(  )
{
    os_timer_disarm(&modbustimer);
}

static void prvvTIMERExpiredISR( void )
{
  ( void )pxMBPortCBTimerExpired(  );
}
