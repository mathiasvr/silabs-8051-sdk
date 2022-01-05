//-----------------------------------------------------------------------------
// F91x_Sleep_Wakeup.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the wake-up time from Idle, Suspend, and Sleep
// modes, as specified on table 4.5 of the datasheet.
//
// Refer to F91x_Sleep_Wakeup_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.5 - Power Management Wakeup Time):
// -----------------------------------------------
// 1) Ensure a shorting block is placed across J17 header and select 2 cell
//    ( 3.3V )on SW4
// 2) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) LED1.5 and P1.5 on J8
//    b) SW0_2 and P0.2 on J8
// 3) set #define ENTER_MODE to select mode.
// 4) Compile and download the code to a C8051F912-TB target board.
// 5) Run the code.
// 6) Use an oscilloscope to monitor the SW0_2 (P0.2) and DS3(P1.5).
// 7) Push SW0_2, the time between DS3 and SW0_2 from high to low is sleep
//    wakeup time on datasheet Table 4.5.

// Target:         C8051F91x/90x
// Tool chain:     Generic
// Command Line:   None
//
//
// Revision History:
// ------------------
// Release 1.0
//    - Initial Revision (MT)
//    - 23 OCT 2013
//
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW0_2, SFR_P0, 2);
SI_SBIT(DS3,   SFR_P1, 5);
//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define PORT_MATCH_WAKEUP   0x02
#define SUSPEND_MODE        0x40
#define SLEEP_MODE          0x80

#define ENTER_SLEEP         0
#define ENTER_SUSPEND       1
#define ENTER_IDLE          2

// Select enter mode
// 0 Sleep mode
// 1 Suspend mode
// 2 Idle mode
#define ENTER_MODE          0

//-----------------------------------------------------------------------------
// Mode Variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

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
 
void Oscillator_Init (void);
void Oscillator_Cycle (void);
void Port_Init (void);
void Enter_Sleep (void);
void Enter_Suspend (void);
void Enter_Idle (void);
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
                                        // enable)
    // Prevent processor into sleep, when SW2 pressed after reset.
    while(SW0_2 == 0);

    // Initialize Port I/O (SW2)
    Port_Init();

    // Initialize Oscillator (Default is 20.0 MHz LPOSC)
    Oscillator_Init();
    PMU0CF = 0x20;                      // Clean all Wakeup flag
    DS3 = 0;                            // Turn on DS3.
    while (1)
    {
		
        // Wait until user press SW2
        while( SW0_2 == SW_PRESSED );
        DS3 = 1;                        // Turn off DS3

        switch (ENTER_MODE)
        {
            case ENTER_SLEEP:
                Enter_Sleep();
                break;

            case ENTER_SUSPEND:
                Enter_Suspend();
                break;

            case ENTER_IDLE:
                Enter_Idle();
                break;

            default:
                break;
        }
    }
}

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to Low Power frequency
// oscillator at the maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
    CLKSEL = 0x04;          // Switch to Low-Power Oscillator 20Mhz
    OSCICN &= ~0x80;
    REG0CN &=~0x10;
    PMU0CF = 0x20;          // Clean all wakeup flag
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins. The switch used
// is configured to wake up from sleep mode and change the CPU Mode
//
// P1.5   digital   push-pull    output  DS3
// P0.2   digital   open-drain   input   SW0_2
//
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    // SW2 (P0.2) Input - Oscillator Clock Cycle Switch
    P0MDIN |= 0x04;                     // P0.2 is digital
    P0MDOUT &= ~0x04;                   // P0.2 is open-drain
    P0 |= 0x04;                         // Set P0.2 latch to '1'

    // LED1(P1.5) Output - Measure Sleep wake up time
    P1MDOUT |= 0x20;                    // P1.5 is push-pull
    P1 |= 0x20;                         // Set P0.2 latch to '1'

    // Set Port Match settings for S1 (P0.2)
    P0MASK |= 0x04;                     // Enable Port Match check for P0.2
    P0MAT  |= 0x04;                     // Set Port Match value to 1 for P0.2

    // Crossbar Initialization
    XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Enter_Sleep
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into sleep mode, in order to allow the user
// to measure Suspend  mode current draw.
// System wake up when SW2 (P0.1) is pressed.
//
//-----------------------------------------------------------------------------
void Enter_Sleep (void)
{
    // Go into sleep mode
    PMU0CF = SLEEP_MODE | PORT_MATCH_WAKEUP;

    //--Sleep mode--//
    //= = = = = = = = = = = = = = =
    //= = = = = = = = = = = = = = =
    //--wake on port match event--//
    DS3 = 0;                           // Turn on DS3.
    PMU0CF = 0x20;
}

//-----------------------------------------------------------------------------
// Enter_Suspend
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into suspend mode, in order to allow the user
// to measure Suspend mode current draw.
// System wake up when SW2 (P0.1) is pressed.
//
//-----------------------------------------------------------------------------
void Enter_Suspend (void)
{
    // Go into suspend mode
    PMU0CF = SUSPEND_MODE | PORT_MATCH_WAKEUP;

    //--Suspend mode--//
    //= = = = = = = = = = = = = = =
    //= = = = = = = = = = = = = = =
    //--wake on port match event--//
    DS3 = 0;                           // Turn on DS3.
    PMU0CF = 0x20;
}

//-----------------------------------------------------------------------------
// Enter_Idle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into Idle mode, in order to allow the user
// to measure Idle mode current draw.
// System wake up when SW0_2 (P0.2) is pressed.
//
//-----------------------------------------------------------------------------
void Enter_Idle (void)
{
    uint8_t IE_save;
    uint8_t EIE2_save;

    IE_EA = 0;                          // Clear global interrupt enable

    // Save interrupt state
    IE_save = IE;                       // Preserve IE register
    EIE2_save = EIE2;                   // Preserve EIE1 register
    IE = 0x00;                          // Disable interrupts

    EIE2 = 0x02;                        // Enable port match interrupts
    IE_EA = 1;                             // Enable global interrupts

    // Go into idle mode
    PCON |= 0x01;                       // Idle mode
    PCON = PCON;                        // Follow the instruction to enter idle
    // with a 3 cycle dummy instruction

    //--Idle mode--//
    //--wake on port match event--//
    EIE2 = EIE2_save;                   // Restore ext interrupts
    IE = IE_save;                       // Restore interrupts
    IE_EA = 1;                             // Set global interrupt enable
}

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// This interrupt wakes the processor from idle mode, and proceeds to continue
// to the Oscillator_Cycle() function.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for long periods of time.
//
// P0.2   digital   open-drain   input   SW0.2 - Port Match Trigger
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PM_ISR,PMATCH_IRQn)
{

    DS3 = 0;                            // Turn on DS3.
    EIE2 &= ~0x02;                      // Disable Port Match interrupts to
                                        // prevent multiple interrupts from
                                        // occurring while the switches are
                                        // pressed
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
