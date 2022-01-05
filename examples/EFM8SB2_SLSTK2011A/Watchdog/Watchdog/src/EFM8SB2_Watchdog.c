//-----------------------------------------------------------------------------
// SB1_Watchdog.c
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
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8SB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
//        - The test will blink the LED0 at a rate of 10Hz until the switch PB0
//          (P1.2) is pressed.
//        - Once the the switch is pressed and held for a long enough time,
//          it will prevent the WDT from being touched and the WDT will
//          cause a reset.
//        - Upon reset the code checks for a WDT reset and blinks the LED five
//          times faster than before to indicate the same.
//
// Target:         EFM8SB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 16 JAN 2015
//
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>
#include <SI_EFM8SB2_Register_Enums.h>                  // SI_SFR declarations
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 6);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (SW2, SFR_P0, 2);                 // SW2==0 means switch depressed

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
  DISP_EN = DISP_BC_DRIVEN;

	if ((RSTSRC & 0x02) == 0x00)        // First check the PORSF bit. if PORSF
	{                                   // is set, all other RSTSRC flags are
									   // invalid
	  // Check if the last reset was due to the Watch Dog Timer
	  if (RSTSRC == 0x08)
	  {
		 enter_FastBlink_from_RESET();

		 while(1);                       // wait forever
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
	   while(!SW2);                    // Force the MCU to stay in this task as
									   // int32_t as SW2 is pressed. This task must
									   // finish before the watchdog timer
									   // timeout expires.

	   //----------------------------------------------------------------------
	   // Task #2 - Reset Watchdog Timer
	   //----------------------------------------------------------------------
	   PCA0CPH5 = 0x00;                // Write a 'dummy' value to the PCA0CPH5
									   // register to reset the watchdog timer
									   // timeout. If a delay longer than the
									   // watchdog timer delay occurs between
									   // successive writes to this register,
									   // the device will be reset by the watch
									   // dog timer.
	}
}
