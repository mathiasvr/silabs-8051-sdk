//-----------------------------------------------------------------------------
// F96x_PR_Oscillator_F.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the consistency of the Internal Precision 
// Oscillator with the VDD ranging from 1.8 V to 3.8 V (the full supply range).
//
// How to Measure (Table 4.9 - Internal Precision Oscillator):
// -------------------------------------------------
// 1) Remove the J15 shorting block and connect DC Power supply to VBAT and
//    GND.
// 2) Remove all other shorting blocks and select VBAT on SW5
// 3) Connect an oscilloscope probe to P0.3 and GND
// 4) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Measure the frequency output on P0.3 while changing the output from
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
//    - 18 OCT 2013
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
    
    // Default enable 24.5 MHz Internal precision Oscillator
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
// This function initializes the system clock to use the internal precision
// oscillator at the maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
    //At clock frequencies above 10 MHz, the system clock cycle becomes 
    //short enough that the one-shot timer no longer provides a power benefit.
    //Disabling the one-shot timer at higher frequencies 
    //reduces power consumption. 
    SFRPAGE = CONFIG_PAGE;
    FLSCL     = 0x40;        
    SFRPAGE = LEGACY_PAGE;	
    
    // Internal Precision Oscillator 24.5MHz enabled
    REG0CN |= 0x10;
    OSCICN |= 0x80;	
    
    // Select Internal Precision Oscillator as system clock source
    CLKSEL = 0x00;     	   
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
// P0.3   digital    push-pull             output system clock 
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    P0MDIN |= 0x08;                     // P0.3 is digital
    P0MDOUT |= 0x08 ;                   // P0.3 is output
    P0SKIP  |=  0x07;                   // P0.0 P0.1 P0.2 skipped in the 
                                        // Crossbar
    // Crossbar Initialization
    XBR0 = 0x08;                        // Route /SYSCLK to first available pin
    XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
