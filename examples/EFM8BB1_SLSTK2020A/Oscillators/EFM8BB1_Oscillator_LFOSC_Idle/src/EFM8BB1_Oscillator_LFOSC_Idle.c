//-----------------------------------------------------------------------------
// EFM8BB1_Oscillator_LFOSC_Idle.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// The program demonstrates the Low Frequency Oscillator (LFOSC) and power
// savings mode operation on the EFM8BB1. The MCU will sit in low power
// idle mode with only the low frequency oscillator and timer T0 running.
// When the MCU wakes up, it will switch to a 24.5 MHz clock, turn on an
// LED, and wait 100 ms.
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) Place the power switch in the AEM position and power the board
//    using the DBG USB connector.
// 2) Compile and download code to the EFM8BB1 board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 3) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 4) The Green LED should toggle approx. once a second.
//
// Target:         EFM8BB1
// Command Line:   None
//
// Release 1.2 (BL)
//		- Updated How To Test
//		- 2 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 19 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK  24500000               // SYSCLK in Hz

#define SLEEP_TIME 10000               // Sleep 1 second

#define WAIT_TIME    100               // Wait 100 ms

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void Enter_Idle(uint16_t);
void wait_ms(uint16_t);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
SI_SBIT (LED1, SFR_P1, 4);              // Green LED

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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   enter_DefaultMode_from_RESET();
   IE_EA = 1;                           // Set global interrupt enable

   while (1)
   {
      Enter_Idle(SLEEP_TIME);

      LED1 = !LED1;                     // Turn off the LED

      wait_ms(WAIT_TIME);
   }
}

//-----------------------------------------------------------------------------
// Enter_Idle ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : time - sleep time (10000 = 1 second)
//
// This function will put the MCU into idle mode using the T0 interrupt to wake
// up the MCU. In idle mode, the MCU will be running off the internal low
// frequency oscillator (LFOSC) running at 10 kHz (80 kHz / 8).
//
//-----------------------------------------------------------------------------
void Enter_Idle(uint16_t time)
{
   IE_EA = 0;                             // Clear global interrupt enable
   TCON &= ~TCON_TF0__BMASK;              // clear T0 flag
   enter_Sleep_Mode_from_DefaultMode();

   // Set T0 using time
   time = -time;
   TL0 = (uint8_t)(time & 0x00FF);
   TH0 = (uint8_t)(time >> 8);

   // Comment out this line to measure steady state idle current
   //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
   IE_EA = 1;                          // Enable global interrupts
   //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

   CLKSEL = 0x02;                      // Switch to LFOSC
   TCON_TR0 = 1;                       // Start Timer 0

   // Go into idle mode
   PCON0 |= 0x01;                      // Idle mode
   PCON0 = PCON0;                      // Follow the instruction to enter idle
                                       // with a 3 cycle dummy instruction

   //--wake--//
   IE_EA = 0;
   enter_DefaultMode_from_Sleep_Mode();   //Apply setup change
   IE_EA = 1;                             // Set global interrupt enable
}

//-----------------------------------------------------------------------------
// wait_ms ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : time - delay time in ms
//
// This function will implement a delay using T0.  This function assumes
// SYSCLK is running from the HFOSC at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void wait_ms (uint16_t ms)
{
   while (ms) {
      TCON_TR0 = 0;                         // Stop Timer0
      TH0 = (uint8_t)-(SYSCLK/1000 >> 8);            // Overflow in 1ms
      TL0 = (uint8_t)-(SYSCLK/1000);
      TCON_TF0 = 0;                         // Clear overflow indicator
      TCON_TR0 = 1;                         // Start Timer0
      while (!TCON_TF0);                    // Wait for overflow
      ms--;                                 // Update ms counter
   }

   TCON_TR0 = 0;                            // Stop Timer0
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
