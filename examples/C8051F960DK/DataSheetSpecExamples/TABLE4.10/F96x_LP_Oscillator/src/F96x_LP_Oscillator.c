//-----------------------------------------------------------------------------
// F96x_PR_Oscillator.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the consistency of the Internal Low Power 
// Oscillator with the VDD ranging from 1.8 V to 3.8 V (the full supply range).
//
// How to Measure (Table 4.10 - Internal Low Power Oscillator):
// -------------------------------------------------
// 1) Remove the VBAT CURRENT shorting block (J15) and connect DC Power supply,
//    to VBAT and GND.
// 2) Remove all other shorting blocks and select VBAT on SW5.
// 3) Connect an oscilloscope probe to P0.0 and GND.
// 4) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Measure the frequency output on P0.0 while changing the output from
//    1.8 V to 3.8 V.
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 19 OCT 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

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

    // Select  Low power oscillator as system clock source.
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
    SFRPAGE = CONFIG_PAGE;    
    // At clock frequencies above 10 MHz, the system clock cycle becomes 
    // short enough that the one-shot timer no longer provides a power benefit.
    // Disabling the one-shot timer at higher frequencies reduces power 
    // consumption. 
    FLSCL     = 0x40;        
    SFRPAGE = LEGACY_PAGE;
    
    // Select Internal Low Power Oscillator as system clock source
    CLKSEL = 0x04; 
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
