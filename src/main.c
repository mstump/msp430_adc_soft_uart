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

/**
 * Convert an ADC output (steps) into millivolts. The MSP430G2231
 * sports a 10 bit ADC which means range is 2^10 or 1024.
 *
 * @param the steps as outputed by the ADC
 * @param reference voltage in millivolts
 * @param the number of steps supported by the ADC
 *
 * @return millivolts
 */
inline int
adc_step_to_volt(
    int steps,
    int vref,
    int range) {
  return (vref / range) * steps;
}

uint16_t
ADC_single_meas(
    uint16_t chan) {
  ADC10CTL0 &= ~ENC;
  ADC10CTL0  = ADC10SHT_3
             | ADC10ON
             | SREF_0;
  ADC10CTL1  = ADC10SSEL_0 + chan;
  ADC10CTL0 |= ENC + ADC10SC;
  while (ADC10CTL1 & BUSY);
  return ADC10MEM;
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
  if (CALBC1_1MHZ==0xFF) {              // If calibration constants erased
    while(1);                           // do not load, trap CPU!!
  }

  // Frequency
  DCOCTL  = 0;                          // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;                // 1mhz is required for the UART
  DCOCTL  = CALDCO_1MHZ;

  // UART
  P1SEL  = UART_TXD + UART_RXD;         // Timer function for TXD/RXD pins
  P1DIR |= UART_TXD;

  // ADC
  /* P1SEL     |= BIT3;                    // ADC input pin P1.3 */
  /* // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on */
  /* // Don't use interrupts just poll */
  /* ADC10CTL0  = SREF_0 + ADC10SHT_3 + ADC10ON; */
  /* ADC10CTL1  = INCH_3 + ADC10DIV_3 ;    // Channel 3, ADC10CLK/3 */
  /* ADC10AE0  |= BIT3; */

  __enable_interrupt();
  soft_uart_init(NULL);               // Start Timer_A UART

  for (;;) {
    // Update board outputs according to received byte
    unsigned char rxBuffer = soft_uart_get();
    char value_string[5]   = { 0 };

    switch (rxBuffer) {
      case '1':
        /* __delay_cycles(1000); */
        /* ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start */
        /* while (ADC10CTL1 & BUSY); */

        /* // itoa(adc_step_to_volt(ADC10MEM, 3000, 1024), value_string, 10); */
        /* itoa(ADC10MEM, value_string, 10); */

        itoa(ADC_single_meas(INCH_4), value_string, 10);
        soft_uart_print("A3: ");
        soft_uart_print(value_string);
        soft_uart_print("mV\r\n");
        break;
      default:
        break;
    }
  }
}
