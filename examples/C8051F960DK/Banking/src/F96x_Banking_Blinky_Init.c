//-----------------------------------------------------------------------------
// F96x_Banking_Blinky_Init.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// This file contains the Blinky initialization functions. These functions are
// set to be placed in Bank 3 by the linker. To view these settings, navigate
// to project properties -> C/C++ Build -> Settings, navigate to the Tool
// Settings Tab, and view "Banking" under Keil 8051 Linker.
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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SI_C8051F960_Register_Enums.h"     // SFR declarations


//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   SFRPAGE = LEGACY_PAGE;

   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/48 as its time base.
//
void Timer2_Init (int counts)
{
   SFRPAGE = LEGACY_PAGE;

   TMR2CN  = 0x00;                        // Stop Timer2; Clear TF2;
                                          // use SYSCLK/12 as timebase
   CKCON  &= ~0x60;                       // Timer2 clocked based on T2XCLK;

   TMR2RL  = -counts;                     // Init reload values
   TMR2    = 0xffff;                      // set to reload immediately
   IE_ET2      = 1;                       // enable Timer2 interrupts
   TMR2CN_TR2  = 1;                       // start Timer2
}
