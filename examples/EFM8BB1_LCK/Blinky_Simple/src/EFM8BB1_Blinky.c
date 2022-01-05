//-----------------------------------------------------------------------------
// EFM8BB1_Blinky.c
//-----------------------------------------------------------------------------
// Copyright (C) 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes the LED on the EFM8BB1 MCU board about
// five times a second using the interrupt handler for Timer2.
//
//
// How To Test:
//
// 2) Connect the EFM8BB1 LCK board to a PC using a micro USB cable.
// 3) Compile and download code to the EFM8BB1 LCK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The LED should blink at approximately 5 Hz.
//
//
// Target:         EFM8BB1
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2
//    - Updated to use with Simplicity Studio (DL)
//    - 8 JUNE 2015
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 14 JAN 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB1_Defs.h>
#include <SI_EFM8BB1_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define SYSCLK       24500000/8        // SYSCLK frequency in Hz
SI_SBIT(LED, SFR_P1, 4);               // P1.4 LED

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void Port_Init (void);
void Timer2_Init (int counts);

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
   WDTCN = 0xDE; //First key
   WDTCN = 0xAD; //Second key
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   // Disable watchdog with key sequence
   Port_Init ();
   Timer2_Init (SYSCLK / 12 / 10);     // Init Timer2 to generate interrupts
                                       // at a 10 Hz rate.

   IE_EA = 1;                          // Enable global interrupts
   LED = 1;
   while (1) {}                        // Spin forever
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports.
//
// P1.6 - LED (push-pull output)
//
void Port_Init (void)
{
   P1MDOUT |= 0x40;                    // Enable LED as a push-pull output

   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
void Timer2_Init (int counts)
{
   TMR2CN0 = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON0 &= ~0x60;                    // Timer2 clocked based on T2XCLK;

   TMR2RL = -counts;                   // Init reload values
   TMR2 = 0xffff;                      // Set to reload immediately
   IE_ET2 = 1;                         // Enable Timer2 interrupts
   TMR2CN0_TR2 = 1;                    // Start Timer2
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
// This routine changes the state of the LED whenever Timer2 overflows.
//
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
   TMR2CN0_TF2H = 0;                   // Clear Timer2 interrupt flag
   LED = !LED;                         // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
