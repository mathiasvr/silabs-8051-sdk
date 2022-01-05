//-----------------------------------------------------------------------------
// EFM8BB3_PCA_HighSpeedOutput.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for main file:
//
#ifndef __PCA_HIGH_SPEED_OUTPUT_H__
#define __PCA_HIGH_SPEED_OUTPUT_H__


//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK       24500000          // Internal oscillator frequency in Hz
#define CEX0_FREQUENCY  10000          // Frequency to output in Hz
#define DIVIDE_RATIO (SYSCLK/CEX0_FREQUENCY/2) // SYSCLK cycles per interrupt

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern uint16_t Next_Compare_Value;       // Next edge to be sent out in HSO mode

#endif//__PCA_HIGH_SPEED_OUTPUT_H__
