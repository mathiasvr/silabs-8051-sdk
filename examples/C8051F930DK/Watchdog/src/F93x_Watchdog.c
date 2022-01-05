//-----------------------------------------------------------------------------
// F93x_Watchdog.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program helps the user to learn about operating the Watch Dog Timer.
// The WDT is used to generate resets if the times between writes to the WDT
// update register (PCA0CPH5) exceed a specified limit. The WDT can be disabled
// and enabled in the software as needed. When enabled the PCA Module 2 acts as
// the WDT. This program resets the MCU when P0.2 switch is pressed. Also upon
// reset the LED blinks approximately five times faster when compared to before.
// The reset is caused due to a WDT overflow and can be confirmed by checking
// the value of the RSTSRC register where bit 3 is set to indicate a reset
// caused by WDT.
//
// How to Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J12:  P0.2/SW0.2
//             P1.6/LED1.6
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J9.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8:
//        - The test will blink the LED at a rate of 10Hz until the switch SW2
//          (P0.2) is pressed.
//        - Once the the switch is pressed and held for a int32_t enough time,
//          it will prevent the WDT from being touched and the WDT will
//          cause a reset.
//        - Upon reset the code checks for a WDT reset and blinks the LED five
//          times faster than before to indicate the same.
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.2
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.1
//    - Compiled and tested for C8051F930-TB (JH)
//    - 07 JULY 2009
//
// Release 1.0
//    - Initial Revision FB
//    - 19 NOV 2007

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000 / 8      // SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P1, 6);                 // LED==1 means ON
SI_SBIT(SW2, SFR_P0, 2);                 // SW2==0 means switch depressed

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	if ((RSTSRC & 0x02) == 0x00)        // First check the PORSF bit. if PORSF
	{                                   // is set, all other RSTSRC flags are
									   // invalid
	  // Check if the last reset was due to the Watch Dog Timer
	  if (RSTSRC == 0x08)
	  {
		  enter_FastBlink_from_RESET();

		 while(1);                          // wait forever
	  }
	  else
	  {
		  enter_SlowBlink_from_RESET();
	  }

	}
	// Calculate Watchdog Timer Timeout
	// Offset calculated in PCA clocks
	// Offset = ( 256 x PCA0CPL5 ) + 256 - PCA0L
	//        = ( 256 x 255(0xFF)) + 256 - 0
	// Time   = Offset * (12/SYSCLK)
	//        = 255 ms ( PCA uses SYSCLK/12 as its clock source)

	enter_SlowBlink_WD_from_SlowBlink();

	//--------------------------------------------------------------------------
	// Main Application Loop/Task Scheduler
	//--------------------------------------------------------------------------

	while (1)
	{
	   //----------------------------------------------------------------------
	   // Task #1 - Check Port I/O
	   //----------------------------------------------------------------------
	   while(!SW2);                 // Force the MCU to stay in this task as
									// int32_t as SW2 is pressed. This task must
									// finish before the watchdog timer
									// timeout expires.

	   //----------------------------------------------------------------------
	   // Task #2 - Reset Watchdog Timer
	   //----------------------------------------------------------------------
	   PCA0CPH5 = 0x00;             // Write a 'dummy' value to the PCA0CPH5
									// register to reset the watchdog timer
									// timeout. If a delay longer than the
									// watchdog timer delay occurs between
									// successive writes to this register,
									// the device will be reset by the watch
									// dog timer.
	}

	// NOTREACHED
	return 0;
}
