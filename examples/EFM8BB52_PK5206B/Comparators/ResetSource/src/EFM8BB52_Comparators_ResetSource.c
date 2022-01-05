//-----------------------------------------------------------------------------
// EFM8BB52_Comparators_ResetSource.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Silicon Laboratories MCUs can use different mechanisms to reset the device.
//
// This software shows the necessary configuration to use Comparator0 as
// a reset source.  The code executes the initialization routines and then
// spins in an infinite while() loop.  If the right button (on the board) is
// pressed, then the comparator's positive input (CP0+) input will drop below
// the comparator's negative input (CP0-).  When this happens, Comparator0
// will cause the device to be reset. Note that the comparator needs to
// be stabilized before the comparator resource source can be enabled. Different
// comparator mode will require different delay. For this example,
// the comparator is running in mode 2 and a 20us delay is inserted.
//
// When the code is waiting for the button to be pressed, the LED is on.
//
// Upon every device reset, the code checks the RSTSRC register to determine
// if the last reset was due to a comparator reset.  If it was a
// comparator reset, the code will blink the LED constantly.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 1
//   Comparator0
//   Timer0 - for delay in microseconds
//   P0.2   - Right push button / Comparator0 positive input (CP0+)
//   P0.6   - Comparator0 negative input (CP0-)
//   P1.4   - LED green
//   P3.4   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB52 Pro Kit
//-----------------------------------------------------------------------------
// 1) Connect 1.8V from bench power supply to P0.6 (comparator negative input)
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8BB52 Pro Kit board to a PC using a USB cable.
// 4) Compile and download code to the EFM8BB52 Pro Kit board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Green LED should be on (comparator reset not detected).
// 7) Press the BTN0 (this causes a comparator reset).
// 8) Verify that the green LED is blinking.
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 1.0 (RN)
//    - Initial Revision
//    - March 2021
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P3, 4);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (LED, SFR_P1, 4);              //  LED green

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
   DISP_EN = DISP_BC_DRIVEN;           // EFM8 does not drive display

   // When checking the RSTSRC register for the source of the last reset,
   // you must first also check that the PORSF bit is also '0'.  If the PORSF
   // bit is set ('1'), then all other bits in the RSTSRC register are invalid.

   // If last reset was due to comparator reset
   if (((RSTSRC & RSTSRC_PORSF__BMASK) == RSTSRC_PORSF__NOT_SET) &&
	   ((RSTSRC & RSTSRC_C0RSEF__BMASK) == RSTSRC_C0RSEF__SET)) {
      enter_BlinkMode_from_RESET();

      while (1) {
         Timer0_Delay (30000);         // Delay
         LED = !LED;                   // Flip LED state
      }
   }
   // Last reset was due to a power-on reset or something else
   else {
     enter_CompareMode_from_RESET();
     Timer0_Delay(20);                 // 20 us delay inserted between Comparator
                                       // initialization and reset source
                                       // configuration
     RSTSRC = RSTSRC_PORSF__NOT_SET | RSTSRC_MCDRSF__NOT_SET
	            | RSTSRC_C0RSEF__SET;

     LED = 0;                          // Turn LED on

     while(1){}                        // Infinite while loop waiting for
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
//                    range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <us> microseconds before returning.
//-----------------------------------------------------------------------------

void Timer0_Delay (uint16_t us)
{
   unsigned i;                         // Millisecond counter

   for (i = 0; i < us; i++) {          // Count microseconds
      TCON &= ~0x30;                   // STOP Timer0 and clear overflow
      TH0 = (-SYSCLK/1000000) >> 8;    // Set Timer0 to overflow in 1us
      TL0 = -SYSCLK/1000000;
      TCON |= 0x10;                    // START Timer0
      while (TCON_TF0 == 0);           // Wait for overflow
   }
}
