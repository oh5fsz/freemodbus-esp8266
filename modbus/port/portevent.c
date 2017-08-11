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
 * File: $Id: portevent.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "freertos/queue.h"

/* ----------------------- Variables ----------------------------------------*/
static xQueueHandle xModbusEventQueue;

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortEventInit( void )
{
    BOOL qStatus = FALSE;
    if (xModbusEventQueue = xQueueCreate(1, sizeof(eMBEventType))) {
      qStatus = TRUE;
    }
    return qStatus;
}

void vMBPortEventClose(void) {
  if (xModbusEventQueue != 0) {
    vQueueDelete(xModbusEventQueue);
    xModbusEventQueue = 0;
  }
}

BOOL
xMBPortEventPost( eMBEventType eEvent )
{
    xQueueSendFromISR(xModbusEventQueue, (const void*)&eEvent, pdFALSE);
    return TRUE;
}

BOOL
xMBPortEventGet( eMBEventType * eEvent )
{
    BOOL xEventHappened = FALSE;

    if (xQueueReceive(xModbusEventQueue, eEvent, portTICK_RATE_MS*50)) {
      xEventHappened = TRUE;
    }
    return xEventHappened;
}
