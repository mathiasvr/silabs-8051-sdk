//-----------------------------------------------------------------------------
// EFM8UB2_Comparator0_ResetSource.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Silicon Laboratories MCUs can use different mechanisms to reset the device.
//
// This software shows the necessary configuration to use Comparator0 as
// a reset source.  The code executes the initialization routines and then
// spins in an infinite while() loop.  If the button on P0.2 (on the target
// board) is pressed, then the comparators postive input (CP0+) input will drop
// below the comparator's negative input (CP0-).  When this happens,
// Comparator0 will cause the device to be reset.
//
// When the code is waiting for the button to be pressed, the LED is on.
//
// Upon every device reset, the code checks the RSTSRC register to determine
// if the last reset was due to a comparator reset.  If it was a
// comparator reset, the code will blink the LED constantly.
//
// Resources:
//   P0.2   - Right push button
//   P2.6   - Comparator0 negative input (CP0-)
//   P3.4   - Comparator0 positive input (CP0+)
//   P1.7   - LED green
//
// How To Test:
//
// 1) Connect P2.6 (Comparator negative input) to an external voltage source
//    set at approx. 1.8 V. Use an external wire to connect P0.2 (switch) to
//    P3.4 (Comparator positice input).
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 4) Compile and download code to a EFM8UB2 development
//    board selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Verify that the blue LED is on.
// 7) Press the PB0 switch (this causes a comparator reset)
// 8) Verify that the blue LED is blinking.
//
//
// Target:         EFM8UB2
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 09 JAN 2014
//
// Release 1.0
//    -Initial Revision (GP)
//    -17 MAY 2006
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000    // Clock speed in Hz
#define COMPARATOR_SETTLE  10          // Settle time in us

SI_SBIT(LED, SFR_P1, 7);               // LED='0' means ON'
SI_SBIT(SWITCH, SFR_P0, 2);            // Push-button switch on board

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Timer0_Delay (uint16_t us);   // Used as a delay

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
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{

   // When checking the RSTSRC register for the source of the last reset,
   // you must first also check that the PORSF bit is also '0'.  If the PORSF
   // bit is set ('1'), then all other bits in the RSTSRC register are invalid.

	 enter_DefaultMode_from_RESET();
   
   // If last reset was due to comparator reset
   if (((RSTSRC & RSTSRC_PORSF__BMASK) == RSTSRC_PORSF__NOT_SET) && ((RSTSRC & RSTSRC_C0RSEF__BMASK)==RSTSRC_C0RSEF__SET))
   {
      while (1)
      {
         Timer0_Delay (30000);         // Delay
         LED = !LED;                   // Flip LED state
      }
   }
   // Last reset was due to a power-on reset or something else
   else
   {
      LED = 0;                         // Turn LED on

      while(1){}                       // Infinite while loop waiting for
	                                   // comparator reset
   }
}

//-----------------------------------------------------------------------------
// Timer0_Delay
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint16_t us - number of microseconds of delay
//                    range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <us> microseconds before returning.
//-----------------------------------------------------------------------------

void Timer0_Delay (uint16_t us)
{
   unsigned i;                         // Millisecond counter

   for (i = 0; i < us; i++)            // Count microseconds
   {
      TCON &= ~0x10;                   // STOP Timer0
      TH0 = (-SYSCLK/1000000) >> 8;    // Set Timer0 to overflow in 1us
      TL0 = (uint8_t)-SYSCLK/1000000;
      TCON |= 0x10;                    // START Timer0
      while (TCON_TF0 == 0);                // Wait for overflow
      TCON &= ~0x20;                   // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
