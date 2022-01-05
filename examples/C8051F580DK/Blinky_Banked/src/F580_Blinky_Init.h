//-----------------------------------------------------------------------------
// F580_Blinky_Init.h
//-----------------------------------------------------------------------------
// Copyright (C) 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// How To Test:
//
// See F580_Blinky_Main.c
//
// Target:         C8051F580 (Side A of a C8051F580-TB)
// Tool chain:     Keil C51 8.0 / Keil EVAL C51
// Command Line:   None
//
// Release 1.1 / 10 JUL 2015 (SG)
//    - Revised for Simplicity Studio
//
// Release 1.0 / 12 SEP 2008 (GP)
//    -Initial Revision

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F580_Register_Enums.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void TIMER2_Init (uint16_t counts);

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
