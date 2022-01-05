//-----------------------------------------------------------------------------
// Watchdog.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt  
//
// Program Description:
//
// This program helps the user to learn about operating the Watch Dog Timer.
// The WDT is used to generate resets if the times between writes to the WDT 
// update register (PCA0CPH4) exceed a specified limit. The WDT can be disabled 
// and enabled in the software as needed. When enabled the PCA Module 4 acts as
// the WDT. This program resets the MCU when P0.2 switch is pressed. Also upon
// reset the LED blinks approximately ten times faster when compared to before. 
// The reset is caused due to a WDT overflow and can be confirmed by checking 
// the value of the RSTSRC register where bit 3 is set to indicate a reset
// caused by WDT.
// 
// How to Test:
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB2 STK board to a PC using a mini USB cable..
// 3) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
//        - The test will blink the LED at a rate of 10Hz until the switch PB0
//          (P0.2) is pressed.
//        - Once the the switch is pressed and held for a long enough time,
//          it will prevent the WDT from being touched and the WDT will
// 		    cause a reset.
//        - Upon reset the code checks for a WDT reset and blinks the LED ten 
// 		  times faster than before to indicate the same.      
//
// Target:         EFM8UB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// 16-bit SI_SFR Definitions for EFM8UB2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

SI_SBIT (LED, SFR_P1, 6);                 // LED='0' means ON
SI_SBIT (PB0, SFR_P0, 2);                 // SW2='0' means switch pressed

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
//
// The MAIN routine performs all the intialization, and then loops until the 
// switch is pressed. When PB0 (P0.2) is pressed the code checks the RSTSRC
// register to make sure if the last reset is because of WDT.
//-----------------------------------------------------------------------------
void main(void)
{

  if ((RSTSRC & 0x02) == 0x00)        // First check the PORSF bit. if PORSF
  {                                   // is set, all other RSTSRC flags are
                                      // invalid

    // Check if the last reset was due to the Watch Dog Timer
    if (RSTSRC == 0x08)
    {
      enter_FastBlink_from_RESET();
      while (1);                           // Wait forever
    }

  }

  enter_SlowBlink_WD_from_RESET();

  //--------------------------------------------------------------------------
  // Main Application Loop/Task Scheduler
  //--------------------------------------------------------------------------

  while (1)
  {
    //----------------------------------------------------------------------
    // Task #1 - Check Port I/O
    //----------------------------------------------------------------------
    while (!PB0);                   // Force the MCU to stay in this task as
                                    // long as SW2 is pressed. This task must
                                    // finish before the watchdog timer
                                    // timeout expires.

    //----------------------------------------------------------------------
    // Task #2 - Reset Watchdog Timer
    //----------------------------------------------------------------------
    PCA0CPH4 = 0x00;              // Write a 'dummy' value to the PCA0CPH4
                                  // register to reset the watchdog timer
                                  // timeout. If a delay longer than the
                                  // watchdog timer delay occurs between
                                  // successive writes to this register,
                                  // the device will be reset by the watch
    // dog timer.
  }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
