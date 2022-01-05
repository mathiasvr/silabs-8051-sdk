//-----------------------------------------------------------------------------
// CapTouchSense_WakeOnRTC_Init.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This file contains the peripheral initialization functions related to the 
// Capacitive TouchSense Switched with Wake-on-RTC implementation.
//
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Corrected pin initialization for F930-TB
//    - 15 JAN 2014
//
// Release 1.0
//    -Initial Revision (PKC)
//    -20 MAY 2008
//    -Based on CapTouchSenseDC_Memory_Game.c (FB)
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F930_Register_Enums.h>
#include "CapTouchSense_WakeOnRTC.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t CS_Switch_Mux_Setting[] =
{
	0xC8, // Red switch     - P2.0
	0x8C  // Yellow switch  - P2.1
};

uint8_t LED_P1_Mask[] =
{
	0x20, // Red LED     - P1.5
	0x40  // Yellow LED  - P1.6
};

//-----------------------------------------------------------------------------
// Delay_Microseconds
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : uint16_t count - Timer3 count value.
//
// Delays the specified number of microseconds.
//-----------------------------------------------------------------------------
void Delay_Microseconds (uint16_t count)
{
   CKCON |= 0x80;    // Timer3 clocked by SYSCLK
   TMR3CN = 0x00;    // Timer3 clocked by SYSCLK
                     // Clear TF3H
   TMR3 = -count;
   TMR3CN |= 0x04;   // Start Timer3
   while ((TMR3CN & 0x80) == 0); // Wait till TF3H overflow is set
}

// NOTE:
// The RTC_Init function is defined in the file "CapTouchSense_WakeOnRTC_RTC.c"

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
