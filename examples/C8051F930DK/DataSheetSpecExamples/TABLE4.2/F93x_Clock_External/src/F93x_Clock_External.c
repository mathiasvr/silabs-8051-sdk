//-----------------------------------------------------------------------------
// F93x_Clock_External.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the current consumption of an external clock in
// normal and idle modes.
//
// Refer to F93x_Clock_External_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.2 - External Mode Current):
// -----------------------------------------------
// 1) Ensure the shorting block is attached to the J11(VBAT & WALL_PWR)
// 2) Remove the J17 shorting block and attach an ammeter between
//    the Imeasure headers.
// 3) Remove all other shorting blocks and select 2 Cell (3.3v) on SW4.
// 4) Ensure that a 1 MHz quartz crystal (Y1), 10Meg resistor (R7), and 33pF 
//    loading capacitors (C21, C20) are installed.
// 5) The default clock mode is Normal mode, to measure Idle mode, set the
//    constant <EXTERNAL_IDLE> to 1.
// 5) Compile and download code to a 'F91x device on a C8051F930-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 6) Disconnect the debugger. The code should now be running.
// 7) To change the measurement modes, return to step 4).
//
// Target:         C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 21 AUG 2013
//
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F930_Register_Enums.h"

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

// Please initialize the following constant to the crystal frequency
//
#define CRYSTAL_FREQUENCY    1000000    //  Crystal Frequency in Hz

// XFCN Setting Macro
#if  (CRYSTAL_FREQUENCY <= 20000)
    #define XFCN 0
#elif(CRYSTAL_FREQUENCY <= 58000)
    #define XFCN 1
#elif(CRYSTAL_FREQUENCY <= 155000)
    #define XFCN 2
#elif(CRYSTAL_FREQUENCY <= 415000)
    #define XFCN 3
#elif(CRYSTAL_FREQUENCY <= 1100000)
    #define XFCN 4
#elif(CRYSTAL_FREQUENCY <= 3100000)
    #define XFCN 5
#elif(CRYSTAL_FREQUENCY <= 8200000)
    #define XFCN 6
#elif(CRYSTAL_FREQUENCY <= 40000000)
    #define XFCN 7
#else
    #error "Crystal Frequency must be less than 40MHz"
#endif

#define CLK_DIV         0x00

#define EXTERNAL_IDLE   0

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
    uint8_t i;
                                        // enable)
                                        
    // Initialize Port 
    Port_Init();   
    // Disable precision oscillator regulator
    REG0CN &=~0x10;
    
    // Starting Mode: External oscillator 1 MHz Normal Mode
    Oscillator_Init();
	while (1)
	{ 
        //  Fetching instructions from Flash in Normal Mode
        for (i=150;i!=0;i--); 
        
#if (EXTERNAL_IDLE == 1)
        // Go into Idle Mode
        PCON |= 0x01;                   // Idle Mode
        PCON = PCON;                    // Follow the instruction to enter idle
                                        // with a 3 cycle dummy instruction
#endif
    }
}

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the external oscillator
// in crystal mode.the system clock is 1 Mhz.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
    OSCXCN = (0x70 | XFCN);             // start external oscillator with
                                        // the appropriate XFCN setting
                                        // based on crystal frequency
    
    while (!(OSCXCN & 0x80));           // Wait for crystal osc. to settle
    
    RSTSRC = 0x06;                      // Enable missing clock detector and
                                        // leave VDD Monitor reset enabled
    
    CLKSEL = 0x01 | CLK_DIV;            // Select external oscillator as system
                                        // clock source, 
	
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins. The switch used
// is configured to both wake up from sleep mode and change the oscillator
// operating frequency.
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
 
    // P0.2, P0.3 Oscillator Pins
    P0MDIN  &= ~0x0C;                  // P0.2, P0.3 are analog
    
    P0SKIP  |=  0x0C;                  // P0.2, P0.3 skipped in the Crossbar
    P0MDOUT &= ~0x0C;                  // PP0.2, P0.3 output mode is open-drain
    P0      |=  0x0C;                  // P0.2, P0.3 output drivers are OFF
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
