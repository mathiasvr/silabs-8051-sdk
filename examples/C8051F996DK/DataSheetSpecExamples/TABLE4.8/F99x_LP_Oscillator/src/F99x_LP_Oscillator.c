//-----------------------------------------------------------------------------
// F99x_LP_Oscillator.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// This example demonstrates the frequency output of the Low Power Oscillator
// from 1.8V to 3.6V, as specified on table 4.8 of the datasheet.
//
// Refer to F99x_LP_Oscillator_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.8 - Internal Low Power Oscillator):
// -------------------------------------------------
// 1) Ensure the shorting block is attached to the J10 (VBAT & WALL_PWR)
//    header.
// 2) Remove all other shorting blocks.
// 3) Connect a DC Power Supply to VDD (J2) and GND.
// 4) Connect an oscilloscope probe to P0.0 and GND
// 5) Compile and download code to a 'F99x device on a C8051F996-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Measure the frequency output on P0.0 while changing the output from
//    1.8 V to 3.6 V.
//
// Target:         C8051F99x-C8051F98x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (MT)
//    - 29 OCT 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F990_Register_Enums.h>

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
    PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
    
                                        // enable)

    Port_Init();                        // Initialize Port I/O

    // Selsct  Low power oscillator as system clock source.
    Oscillator_Init ();                 
    
    while (1);
  
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal low power
// oscillator at the maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
     // Select Internal Low Power Oscillator as system clock source
    CLKSEL = 0x04; 
 
    //At clock frequencies above 10 MHz, the system clock cycle becomes 
    //short enough that the one-shot timer no longer provides a power benefit.
    //Disabling the one-shot timer at higher frequencies 
    //reduces power consumption. 
    FLSCL = 0x40;			   
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P0.0   digital    push-pull             output system clock 
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    P0MDIN |= 0x01;                     // P0.0 is digital
    P0MDOUT |= 0x01 ;                   // P0.0 is output
  
    // Crossbar Initialization
    XBR0 = 0x08;                        // Route /SYSCLK to first available pin
    XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
