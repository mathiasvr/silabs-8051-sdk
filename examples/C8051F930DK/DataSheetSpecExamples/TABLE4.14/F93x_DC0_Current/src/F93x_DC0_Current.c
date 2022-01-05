//-----------------------------------------------------------------------------
// F93x_DC0_Current.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// Refer to F93x_DC0_Current_Readme.txt for more information regarding
// the functionality of this code example.
//
// How To Test (Table 4.14 - Bias Current):
// ---------------------------------------------------------
// 1) Place VDD Select 1 CELL (1.8v) in the SW4 position and attach an Power
//    supply between VBAT and GND
// 2) Place an ammeter in VBAT series connection.
// 3) Set voltage supply output 2.4v voltage.
// 4) Remove all of the shorting blocks on the board
// 5) Default is Normal mode, if user want to measure Low Power mode, set
//    #define LOW_POWER to 1.
// 6) Download and run the code on a C8051F930-TB target board.
// 7) Record the current.then increase power supply output voltage to 3.0v,
//    record the new current.
// 8) The difference should be read between the minimum and maximum values that
//    provided on the datasheet for the DC-DC Converter in Table 4.14
//
//
//
// Target:         C8051F93x/92x
// Tool Chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (MT)
//    - 03 NOV 2013
//
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F930_Register_Enums.h>

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK              20000000    // Clock speed in Hz

#define SUSPEND_MODE        0x40

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);                // Configure the system clock
void PORT_Init (void);                  // Configure the Crossbar and GPIO
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Debounce latches for SW0.2

// Mode type enumerations
enum DC0_LEVEL {LEVEL0 = 0,
                LEVEL1 = 1,
                LEVEL2 = 2,
                LEVEL3 = 3,
                LEVEL4 = 4,
                LEVEL5 = 5,
                LEVEL6 = 6,
                LEVEL7 = 7};
						
enum DC0_LEVEL dc0Level;

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
    PCA0MD &= ~0x40;                    // Disable Watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{


    PORT_Init ();                       // Initialize crossbar
    SYSCLK_Init();                      // Initialize the system clock


    // Default DC0 enabled as level4 output 2.4v
    // Default internal oscillator frequency 2.4MHz
    DC0CN = LEVEL4;

    PMU0CF = 0x20;                      // Clean all Wakeup flag
    // Go into suspend mode
    PMU0CF = SUSPEND_MODE;
    while(1);


}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// Pinout:
//
// P0.2   digital   open-drain          Switch 1
// P0.3   digital   open-drain          Switch 2
// P1.5   digital   push-pull           DS3
// P1.6   digital   push-pull           DS4
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
    XBR2    = 0x40;                     // Enable crossbar and enable
                                        // weak pull-ups
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal low power
// oscillator at its maximum frequency.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{

    // Select Low-Power Oscillator 20 MHz as system clock source

    CLKSEL = 0x04;
    OSCICN &= ~0x80;
    REG0CN &=~0x10;

}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
