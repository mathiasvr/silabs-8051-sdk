//-----------------------------------------------------------------------------
// EFM8BB1_Comparators_ResetSource.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Silicon Laboratories MCUs can use different mechanisms to reset the device.
//
// This software shows the necessary configuration to use Comparator0 as
// a reset source.  The code executes the initialization routines and then
// spins in an infinite while() loop.  If the right button (on the board) is
// pressed, then the comparators positive input (CP0+) input will drop below
// the comparator's negative input (CP0-).  When this happens, Comparator0
// will cause the device to be reset.
//
// When the code is waiting for the button to be pressed, the LED is on.
//
// Upon every device reset, the code checks the RSTSRC register to determine
// if the last reset was due to a comparator reset.  If it was a
// comparator reset, the code will blink the LED constantly.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   Comparator0
//   Timer0 - for delay in microseconds
//   P0.2   - BTN0
//   P0.6   - Comparator0 negative input (CP0-)
//   P0.7   - Comparator0 positive input (CP0+)
//   P1.4   - LED
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 LCK
//-----------------------------------------------------------------------------
// 1) Connect 1.8V from bench power supply to P0.6 (comparator negative input)
// 2) Connect P0.2 (push button) to P0.7 (comparator positive input) with an
//    external wire.
// 4) Connect the EFM8BB1 LCK board to a PC using a micro USB cable.
// 5) Compile and download code to the EFM8BB1 LCK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 6) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 7) Green LED should be on (comparator reset not detected).
// 8) Press the right switch (this causes a comparator reset).
// 9) Verify that the LED is blinking.
//
// Target:         EFM8BB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------
#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (LED, SFR_P1, 4);              // LED

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK       24500000          // Clock speed in Hz
#define COMPARATOR_SETTLE  10          // Settle time in us

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void Timer0_Delay (uint16_t us);       // Used as a delay

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
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{
   // When checking the RSTSRC register for the source of the last reset,
   // you must first also check that the PORSF bit is also '0'.  If the PORSF
   // bit is set ('1'), then all other bits in the RSTSRC register are invalid.

   // If last reset was due to comparator reset
   if (((RSTSRC & 0x02) == 0x00) && ((RSTSRC & 0x20) == 0x20))
   {
      enter_BlinkMode_from_RESET();
      while (1)
      {
         Timer0_Delay (30000);         // Delay
         LED = !LED;                   // Flip LED state
      }
   }
   // Last reset was due to a power-on reset or something else
   else
   {
	  enter_CompareMode_from_RESET();

      LED = 0;                         // Turn LED on

      while(1){}                       // Infinite while loop waiting for
                                       // comparator reset
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_Delay
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint16_t us - number of microseconds of delay
//                        range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <us> microseconds before returning.
//-----------------------------------------------------------------------------

void Timer0_Delay (uint16_t us)
{
   unsigned i;                         // Millisecond counter

   for (i = 0; i < us; i++)            // Count microseconds
   {
      TCON &= ~0x30;                   // STOP Timer0 and clear overflow
      TH0 = (-SYSCLK/1000000) >> 8;    // Set Timer0 to overflow in 1us
      TL0 = -SYSCLK/1000000;
      TCON |= 0x10;                    // START Timer0
      while (TCON_TF0 == 0);           // Wait for overflow
   }
}
