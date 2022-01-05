//-----------------------------------------------------------------------------
// F580_Blinky.c
//-----------------------------------------------------------------------------
// Copyright (C) 2009 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program flashes the green LED on the C8051F580 target board about
// five times a second using the interrupt handler for Timer2.
//
//
// How To Test:
//
// 1) Download code to the 'F580 target board
// 2) Ensure that the P1.3 pins are shorted together on the J19 header
// 3) Run the program.  If the LED flashes, the program is working
//
//
// Target:         C8051F500 (Side A of a C8051F580-TB)
// Tool chain:     Keil C51 8.0 / Keil EVAL C51
// Command Line:   None
//
// Release 1.1 / 10 JUL 2015 (SG)
//    - Revised for Simplicity Studio
//
// Release 1.0 / 19 FEB 2009 (GP)
//    -Initial Revision

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F580_Register_Enums.h>         // SFR declarations
#include "F580_Blinky_Init.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24000000/8        // SYSCLK frequency in Hz

//-----------------------------------------------------------------------------
// Pin Defintions
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P1, 3);                  // LED = 1 means ON

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
   PCA0MD &= ~0x40;                    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = ACTIVE1_PAGE;


   OSCILLATOR_Init ();                 // Configure system clock

   PORT_Init ();                       // Initialize crossbar

   TIMER2_Init (SYSCLK / 12 / 10);     // Init Timer2 to generate interrupts
                                       // at a 10 Hz rate.

   IE_EA = 1;                          // Enable global interrupts

   while (1) {}                        // Spin forever
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer2 overflows.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag
   LED = !LED;                         // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
