//-----------------------------------------------------------------------------
// F91x_IREF0_Time.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the dynamic performance of the IREF0 internal
// current reference, as specified in table 4.13 of the datasheet.
//
// Refer to F91x_IREF0_Time_Readme.txt for more information regarding
// the functionality of this code example.
//
// How To Test (Table 4.13 - Dynamic Performance):
// ---------------------------------------------------------
// 1) Ensure a shorting block is placed the J11 (VBAT & WALL_PWR).
// 2) Place VDD Select (2 CELL) in the SW4 position and power the board
//    using the P2 power connector.
// 3) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//    a) IREF0 on J7
//    b) SW0.2 and P0.2H on J8
// 4) Download and run the code on a C8051F912-TB target board.
// 5) Attach oscilloscope probes to P0.7 and P1.5.
// 6) To cycle between Reference on/off, push SW0.2 (P0.2).
// 7) The toggle time between P0.7 and P1.5 is the output time on datasheet
//    for IREF0 in Table 4.13
//
// Target:          C8051F91x/90x
// Tool Chain:      Generic
// Command Line:    None
//
//
// Release 1.0
//    - Initial Revision (MT)
//    - 26 OCT 2013
//
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(DS3,   SFR_P1, 5);                // '0' means ON, '1' means OFF
SI_SBIT(SW0_2, SFR_P0, 2);                // SW0_2 == 0 means switch pressed


//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK              20000000    // Clock speed in Hz
#define SW_PRESSED          0
#define SW_NOT_PRESSED      1

#define LED_ON              0
#define LED_OFF             1
#define MAX_VALUE           0x3F
#define MIN_VALUE           0x00        // Set the minimum output code

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);                // Configure the system clock
void PORT_Init (void);                  // Configure the Crossbar and GPIO
void Wait_MS(unsigned int ms);          // Inserts programmable delay
void Mode_Update(void);                 // Cycle Reference mode,push SW0_2
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Debounce latches for SW0.2
uint8_t switch1Latch = 0;

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


    PORT_Init ();                       // Initialize crossbar and GPIO
    SYSCLK_Init();                      // Initialize the system clock

    IREF0CN = MAX_VALUE;  		        // Configure IREF0 as a current source
    DS3 = LED_OFF;
    while(1)
    {
        Mode_Update();
    }

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
// P0.2   digital   open-drain/High-Z    Switch
// P0.7   analog    open-drain/High-Z    IREF0
// P1.5   digital   push-pull            DS3
//-----------------------------------------------------------------------------
void PORT_Init (void)
{

    // IREF0 Analog Output
    P0MDIN  &= ~0x80;                   // P0.7 is analog
    P0MDOUT &= ~0x80;                   // P0.7 is open-drain
    P0      |=  0x80;                   // Set P0.7 latch to '1'
    P0SKIP  |=  0x80;                   // P0.7 skipped in Crossbar

    // Switch
    P0MDIN  |=  0x04;                   // P0.2 is digital
    P0MDOUT &= ~0x04;                   // P0.2 is open-drain
    P0      |=  0x04;                   // Set P0.2 latches to '1'
    P0SKIP  |=  0x04;                   // P0.2 skipped in Crossbar

    // LEDs
    P1MDIN  |= 0x20;                    // P1.5 is digital
    P1MDOUT |= 0x20;                    // P1.5 is push-pull
    P1      |= 0x20;                    // Set P1.5 latches to '1'
    P1SKIP  |= 0x20;                    // P1.5 skipped in Crossbar


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
    // Since using the precision internal oscillator as the system clock source,
    // the precision voltage reference should not be enabled from a disabled
    // state.
    CLKSEL = 0x04;
    OSCICN &= ~0x80;
    REG0CN &=~0x10;

    // At clock frequencies above 10 MHz, the system clock cycle becomes
    // short enough that the one-shot timer no longer provides a power benefit.
    // Disabling the one-shot timer at higher frequencies reduces power
    // consumption.
    FLSCL = 0x40;
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Mode_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function checks switch input and select IREF0 mode ,
// once each time the switch is pressed.
// The cycling order is as follows:
//
// Switch SW0_2:
// To toggle Reference and DS3 on/off
// P0.2   digital   open-drain   input   SW0_2
//
//-----------------------------------------------------------------------------
void Mode_Update(void)
{
    // Cycle Reference mode if P0.2 is pushed
    if( (SW0_2 == SW_PRESSED) && (!switch1Latch) )
    {
        if (IREF0CN > 0)
        {
            IREF0CN = 0;

        }
        else
        {
            IREF0CN = MAX_VALUE;
        }

        DS3 = ~DS3;
        // Arm latches to prevent auto-cycling with switch held
        switch1Latch = 1;
        Wait_MS(50);                    // Wait 50 milliseconds
    }

    // Reset latch once switch is released
    else if(SW0_2 == SW_NOT_PRESSED)
    {
        switch1Latch = 0;
    }
}
//-----------------------------------------------------------------------------
// Wait_MS
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters:
//   1) unsigned int ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 256
//
// This routine inserts a delay of <ms> milliseconds.
//-----------------------------------------------------------------------------
void Wait_MS(unsigned int ms)
{
    uint8_t i;

    TCON_TR0 = 0;                            // Stop Timer0

    TMOD &= ~0x0F;                      // Timer0 in 8-bit mode
    TMOD |= 0x02;

    CKCON &= ~0x03;                     // Timer0 uses a 1:48 prescaler
    CKCON |= 0x02;


    TH0 = -SYSCLK/48/10000;             // Set Timer0 Reload Value to
                                        // overflow at a rate of 10kHz

    TL0 = TH0;                          // Init Timer0 low byte to the
                                        // reload value

    TCON_TF0 = 0;                            // Clear Timer0 Interrupt Flag
    IE_ET0 = 0;                            // Timer0 interrupt disabled
    TCON_TR0 = 1;                            // Timer0 on

    while(ms--)
    {
        for(i = 0; i < 10; i++)
        {
            TCON_TF0 = 0;
            while(!TCON_TF0);
        }
    }
    TCON_TF0 = 0;
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
