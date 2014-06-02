/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Matthew Stump
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************/

#ifndef __SOFT_UART_H__
#define __SOFT_UART_H__

//------------------------------------------------------------------------------
// Hardware-related definitions
//------------------------------------------------------------------------------
#define UART_TXD 0x02                   // TXD on P1.1 (Timer0_A.OUT0)
#define UART_RXD 0x04                   // RXD on P1.2 (Timer0_A.CCI1A)

typedef void (*soft_uart_rx_callback_t)(unsigned char c);

/**
 * Function configures Timer_A for full-duplex UART operation
 *
 * @param optional receive callback, pass NULL if not used.
 */
void
soft_uart_init(
    soft_uart_rx_callback_t rxCallback);

/**
 * Outputs one byte using the Timer_A UART
 *
 * @param byte to be transmitted
 */
void
soft_uart_tx(
    unsigned char byte);

/**
 * Prints a string over using the Timer_A UART
 *
 * @param null terminated string to be transmitted
 */
void
soft_uart_print(
    char *string);

/**
 * Synchronous fetch of byte from uart.
 *
 * @return received byte
 */
unsigned char
soft_uart_get();

#endif
