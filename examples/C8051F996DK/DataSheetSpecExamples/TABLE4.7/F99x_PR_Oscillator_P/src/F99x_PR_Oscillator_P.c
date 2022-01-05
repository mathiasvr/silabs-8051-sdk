//-----------------------------------------------------------------------------
// F99x_PR_Oscillator_P.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the current consumption of the Internal Precision
// Oscillator, as specified on table 4.7 of the datasheet.
//
// Refer to F99x_PR_Oscillator_P_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.7 - Internal Precision Oscillator):
// -------------------------------------------------
// 1) Ensure that shorting blocks are attached to the J10 (VBAT & WALL_PWR)
//    and J8 (P0.2H & SW0_2) headers.
// 2) Remove the J17 shorting block and attach an ammeter between
//    the Imeasure headers.
// 3) Remove all other shorting blocks.
// 4) Compile and download code to a 'F99x device on a C8051F996-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) The Internal Precision Oscillator is enabled by default, record this
//    initial current.
// 7) Use SW0.2 (P0.2) to disable the Internal Precision Oscillator.
//    Record this new current measurement.
// 8) The difference between the two measurements is the Internal Precision
//    Oscillator current.
//
// Target:         C8051F99x-C8051F98x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
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
SI_SBIT(SW0_2, SFR_P0, 2);
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1
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
    uint8_t switch1Latch = 0; 
    
                                        // enable)

    Port_Init();                        // Initialize Port I/O
    
    // Default enable 24.5 MHz Internal precision Oscillator
    // Selsct  Low power oscillator as system clock source.
    Oscillator_Init ();                 
    
    while (1)
    {
        // waiting for user press SW0_2 KEY
        if(SW0_2 == SW_PRESSED)
        {
            switch1Latch = 1;
        }
        
        if ((SW0_2 == SW_NOT_PRESSED) && (switch1Latch == 1))
        {
            if (OSCICN & 0x80)
            {
                // Disable Precision Internal Oscillator 
                REG0CN &= ~0x10;
                OSCICN &= ~0x80;
            }
            else
            {
                // Enable Precision Internal Oscillator 
                REG0CN |= 0x10;
                OSCICN |= 0x80;
            }
            switch1Latch = 0;
        }
    }
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
    // Internal Precision Oscillator 24.5MHz enabled
    REG0CN |= 0x10;
    OSCICN |= 0x80;	
    
    // Select Low-Power Oscillator as system clock source
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
// P0.2   digital    push-pull             Precision Oscillator Clock ON/OFF
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    // SW0_2 (P0.2) Input - Oscillator Clock Cycle Switch
    P0MDIN |= 0x04;                      // P0.2 is digital
    P0MDOUT &= ~0x04;                    // P0.2 is open-drain
    P0 |= 0x40;                          // Set the P0.2 latch to '1'
 
    // Crossbar Initialization
    XBR2 = 0x40;                         // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
