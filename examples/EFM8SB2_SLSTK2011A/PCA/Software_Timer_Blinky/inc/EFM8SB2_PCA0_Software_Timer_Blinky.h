/*
 * SB1_PCA0_Software_Timer_Blinky.h
 *
 *  Created on: Jun 3, 2014
 *      Author: jiguo1
 */

#ifndef EFM8SB2_PCA0_SOFTWARE_TIMER_BLINKY_MAIN_H_
#define EFM8SB2_PCA0_SOFTWARE_TIMER_BLINKY_MAIN_H_

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000L         // Internal oscillator frequency in Hz

#define LED_FREQUENCY 5L               // Frequency to blink LED at in Hz
#define T0_CLOCKS 245L                 // Use 245 clocks per T0 Overflow
                                       // SYSCLK cycles per interrupt

#define PCA_TIMEOUT ((SYSCLK/T0_CLOCKS)/LED_FREQUENCY/2)

SI_SBIT (LED0, SFR_P2, 0);                 // LED==1 means ON

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

extern uint16_t Next_Compare_Value;       // Next edge to be sent out in HSO mode


#endif /* EFM8SB2_PCA0_SOFTWARE_TIMER_BLINKY_MAIN_H_ */
