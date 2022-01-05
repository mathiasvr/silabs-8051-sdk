//-----------------------------------------------------------------------------
// F96x_Banking_Blinky_Init.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// How To Test:
//
// See F96x_Banking_Blinky.c
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.0 / 12 SEP 2008 (GP)
//    -Initial Revision

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"     // SFR declarations

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void PORT_Init (void);
void Timer2_Init (int16_t counts);

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
