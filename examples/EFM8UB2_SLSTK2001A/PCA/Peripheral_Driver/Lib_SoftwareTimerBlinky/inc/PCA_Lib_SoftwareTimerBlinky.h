//-----------------------------------------------------------------------------
// PCA_Lib_SoftwareTimerBlinky.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:

#ifndef __PCA_LIB_SOFTWARE_TIMER_BLINKY_H__
#define __PCA_LIB_SOFTWARE_TIMER_BLINKY_H__

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK       24000000          // Internal oscillator frequency in Hz
#define LED_FREQUENCY       5          // Frequency to blink LED at in Hz
#define T0_CLOCKS         240          // Use 240 clocks per T0 Overflow
// SYSCLK cycles per interrupt
#define PCA_TIMEOUT ((SYSCLK/T0_CLOCKS)/LED_FREQUENCY/2)


//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern uint16_t Next_Compare_Value;       // Next edge to be sent out in HSO mode

#endif // __PCA_LIB_SOFTWARE_TIMER_BLINKY_H__
