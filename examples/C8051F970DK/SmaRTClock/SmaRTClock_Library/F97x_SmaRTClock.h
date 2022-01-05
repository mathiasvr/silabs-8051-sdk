#ifndef  F97X_SMARTCLOCK_H
#define  F97X_SMARTCLOCK_H
//----------------------------------------------------------------------------
// F97x_SmaRTClock.h
//----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header File Description:
//
// This file is SmaRTClock header file for the C8051F97x.
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2/JL)
//    - 19 MAY 2014
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define RTC_USE_CRYSTAL    1
#define RTC_USE_LFO        0
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void  RTC_Init (uint8_t useCrystal);
uint8_t    RTC_Read (uint8_t reg);
void  RTC_Write (uint8_t reg, uint8_t value);
uint8_t    RTC_SetTimer(uint32_t time);
uint32_t   RTC_CaptureTimer (void);
void  RTC_WriteAlarm(uint32_t time);
uint32_t   RTC_ReadAlarm ();
//-----------------------------------------------------------------------------
#endif  // F97X_SMARTCLOCK_H
