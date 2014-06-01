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
#include <stdlib.h>
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

  // Frequency
  DCOCTL  = 0;                        // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;              // 1mhz is required for the UART
  DCOCTL  = CALDCO_1MHZ;

  P1OUT = 0x00;                       // Initialize all GPIO

  // UART
  P1SEL = UART_TXD + UART_RXD;        // Timer function for TXD/RXD pins
  P1DIR = 0xFF & ~UART_RXD;           // Set all pins but RXD to output

  // ADC
  ADC10CTL0  = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1  = INCH_3;                // input A3
  ADC10AE0  |= 0x08;                  // PA.3 ADC option select

  // Disable P2
  P2OUT = 0x00;
  P2SEL = 0x00;
  P2DIR = 0xFF;

  __enable_interrupt();
  soft_uart_init(NULL);               // Start Timer_A UART

  for (;;) {
    // Update board outputs according to received byte
    unsigned char rxBuffer = soft_uart_get();
    char value_string[5] = { 0 };

    switch (rxBuffer) {
      case '1':
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
        __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit

        itoa(ADC10MEM, value_string, 10);

        soft_uart_print("A3: ");
        soft_uart_print(value_string);
        soft_uart_print("\r\n");
        break;
      default:
        break;
    }
  }
}

// ADC10 interrupt service routine
#pragma vector = ADC10_VECTOR
__interrupt void
ADC10_ISR() {
  __bic_SR_register_on_exit(CPUOFF);    // Clear CPUOFF bit from 0(SR)
}
