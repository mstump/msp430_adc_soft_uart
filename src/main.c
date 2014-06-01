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

#include <msp430.h>
#include <stddef.h>
#include "soft_uart.h"

//------------------------------------------------------------------------------
// main()
//------------------------------------------------------------------------------
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer
    if (CALBC1_1MHZ==0xFF) {			// If calibration constants erased
      while(1);                         // do not load, trap CPU!!
    }
    DCOCTL  = 0;                        // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;

    P1OUT = 0x00;                       // Initialize all GPIO
    P1SEL = UART_TXD + UART_RXD;        // Timer function for TXD/RXD pins
    P1DIR = 0xFF & ~UART_RXD;           // Set all pins but RXD to output
    P2OUT = 0x00;
    P2SEL = 0x00;
    P2DIR = 0xFF;

    __enable_interrupt();
    soft_uart_init(NULL);               // Start Timer_A UART

    for (;;) {
        // Update board outputs according to received byte
        unsigned char rxBuffer = soft_uart_get();
        switch (rxBuffer) {
          case '1':
            soft_uart_print("received 1\r\n");
            break;
          case '2':
            soft_uart_print("received 2\r\n");
            break;
          default:
            break;
        }
        // Echo received character
        soft_uart_tx(rxBuffer);
    }
}
