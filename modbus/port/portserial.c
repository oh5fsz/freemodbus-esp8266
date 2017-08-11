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
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include <string.h>
#include "freertos/queue.h"
/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );
static void uart_intr_handler(void);

/* ----------------------- FreeRTOS queue for uart --------------------------*/
xQueueHandle xUartQ;
/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    UART_IntrConfTypeDef uart_intr;
    if (xRxEnable && xTxEnable) {
      uart_intr.UART_IntrEnMask = UART_RXFIFO_TOUT_INT_ENA | UART_RXFIFO_FULL_INT_ENA | UART_TXFIFO_EMPTY_INT_ENA;
      uart_intr.UART_RX_FifoFullIntrThresh = 255;
      uart_intr.UART_RX_TimeOutIntrThresh = 2;
      uart_intr.UART_TX_FifoEmptyIntrThresh = 10;
    }
    else if (xRxEnable && !xTxEnable) {
      uart_intr.UART_IntrEnMask = UART_RXFIFO_TOUT_INT_ENA | UART_RXFIFO_FULL_INT_ENA;
      uart_intr.UART_RX_FifoFullIntrThresh = 255;
      uart_intr.UART_RX_TimeOutIntrThresh = 2;
    }
    else if (!xRxEnable && xTxEnable) {
      uart_intr.UART_IntrEnMask = UART_TXFIFO_EMPTY_INT_ENA;
      uart_intr.UART_TX_FifoEmptyIntrThresh = 10;
    }
    else {
    }
    UART_IntrConfig(UART0, &uart_intr);
    UART_intr_handler_register(uart_intr_handler, NULL);
    ETS_UART_INTR_ENABLE();
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    UART_WaitTxFifoEmpty(UART0);
    UART_ConfigTypeDef uart_config;

    switch (ulBaudRate) {
      case 300:
        uart_config.baud_rate = BIT_RATE_300;
        break;
      case 600:
        uart_config.baud_rate = BIT_RATE_600;
        break;
      case 1200:
        uart_config.baud_rate = BIT_RATE_1200;
        break;
      case 2400:
        uart_config.baud_rate = BIT_RATE_2400;
        break;
      case 4800:
        uart_config.baud_rate = BIT_RATE_4800;
        break;
      case 9600:
        uart_config.baud_rate = BIT_RATE_9600;
        break;
      case 19200:
        uart_config.baud_rate = BIT_RATE_19200;
        break;
      case 38400:
        uart_config.baud_rate = BIT_RATE_38400;
        break;
      case 57600:
        uart_config.baud_rate = BIT_RATE_57600;
        break;
      case 74880:
        uart_config.baud_rate = BIT_RATE_74880;
        break;
      case 115200:
        uart_config.baud_rate = BIT_RATE_115200;
        break;
      case 230400:
        uart_config.baud_rate = BIT_RATE_230400;
        break;
      case 460800:
        uart_config.baud_rate = BIT_RATE_460800;
        break;
      case 921600:
        uart_config.baud_rate = BIT_RATE_921600;
        break;
      case 1843200:
        uart_config.baud_rate = BIT_RATE_1843200;
        break;
      case 3686400:
        uart_config.baud_rate = BIT_RATE_3686400;
        break;
      default:
        // Invalid baud rate
        return FALSE;
      }

    switch (ucDataBits) {
      case 5:
        uart_config.data_bits = UART_WordLength_5b;
        break;
      case 6:
        uart_config.data_bits = UART_WordLength_6b;
        break;
      case 7:
        uart_config.data_bits = UART_WordLength_7b;
        break;
      case 8:
        uart_config.data_bits = UART_WordLength_8b;
        break;
      default:
        // Invalid data bits
        return FALSE;
    }

    switch (eParity) {
      case MB_PAR_EVEN:
        uart_config.parity = USART_Parity_Even;
        uart_config.stop_bits = USART_StopBits_1;
        break;
      case MB_PAR_ODD:
        uart_config.parity = USART_Parity_Odd;
        uart_config.stop_bits = USART_StopBits_1;
        break;
      case MB_PAR_NONE:
        uart_config.parity = USART_Parity_None;
        // According to Modbus standard, two stop bits are used if no parity is used.
        uart_config.stop_bits = USART_StopBits_2;
        break;
      default:
        return FALSE;
    }

    uart_config.flow_ctrl         = USART_HardwareFlowControl_None;
    uart_config.UART_RxFlowThresh = 120;

    // If for some reason you need inverted voltage levels, see include/driver/uart.h and set this.
    uart_config.UART_InverseMask =  UART_None_Inverse;

    // ESP8266 can only use UART0 for RX, so we ignore ucPORT.
    UART_ParamConfig(UART0, &uart_config);
    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    WRITE_PERI_REG(UART_FIFO(UART0), ucByte);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    CHAR rxChar = READ_PERI_REG(UART_FIFO(0)) & 0xFF;
    memcpy(pucByte, &rxChar, sizeof(CHAR));
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}

static void uart_intr_handler(void) {
  uint32 uart_intr_status = READ_PERI_REG(UART_INT_ST(UART0)) ;
  if ((uart_intr_status & UART_RXFIFO_FULL_INT_ST) == UART_RXFIFO_FULL_INT_ST) {
      // UART RX FIFO is full
      prvvUARTRxISR();
      WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR);
  } else if ((uart_intr_status & UART_RXFIFO_TOUT_INT_ST) == UART_RXFIFO_TOUT_INT_ST) {
      prvvUARTRxISR();
      WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_TOUT_INT_CLR);
  } else if ((uart_intr_status & UART_TXFIFO_EMPTY_INT_ST) == UART_TXFIFO_EMPTY_INT_ST) {
      prvvUARTTxReadyISR();
      WRITE_PERI_REG(UART_INT_CLR(UART0), UART_TXFIFO_EMPTY_INT_CLR);
  } else {
          //skip
  }
}
