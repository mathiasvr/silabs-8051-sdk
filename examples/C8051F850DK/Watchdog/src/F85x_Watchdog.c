//-----------------------------------------------------------------------------
// F85x_Watchdog.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program uses the Watchdog Timer to generate resets if the times between
// writes to the WDT update register (WDTCN) exceed a specified limit. The WDT
// can be disabled and enabled in the software as needed. This program resets
// the MCU when the S1 switch is held down. Upon a Watchdog reset, the LED
// blinks four times faster compared to before. The reset is caused by an
// overflow of the WDT and can be confirmed by checking the value of the
// RSTSRC register.
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to the 'F85x UDP MCU board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8:
//         a) The test will blink the DS5 P1.0 LED at a rate of 10 Hz until the
//            switch S1 (P1.7) is pressed.
//         b) Once the the switch is pressed and held for a long enough time,
//            (default 1.6 seconds) the WDT will cause a reset.
//         c) The initialization code checks for a WDT reset and blinks the LED
//            four times faster than before.
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 3) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8:
//         a) The test will blink the DS5 P1.0 LED at a rate of 10 Hz until the
//            switch S1 (P1.7) is pressed.
//         b) Once the the switch is pressed and held for a long enough time,
//            (default 1.6 seconds) the WDT will cause a reset.
//         c) The initialization code checks for a WDT reset and blinks the LED
//            four times faster than before.
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
//
//
// Release 1.2 (BL)
//		- Updated How To Test with Studio specific instructions
//		- Fixed an incorrect WDT interval causing WDT to have an extremely long
//		  timeout.
//		- 3 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 20 FEB 2013
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK            24500000 / 8 // SYSCLK frequency in Hz

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT(S1, SFR_P1, 7);

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{

   if ((RSTSRC & 0x02) == 0x00)        // First check the PORSF bit. If PORSF
   {                                   // is set, all other RSTSRC flags are
                                       // indeterminate.
      // Check if the last reset was due to the Watchdog Timer
      if (RSTSRC == 0x08)
      {
    	 enter_PassedMode_from_RESET();
    	 IE_EA = 1;                         // Enable global interrupts

         while(1);                       // wait forever
      }
      else
      {
         // Init Timer2 to generate interrupts at a 5 Hz rate.
         enter_WatchdogMode_from_RESET();
      }

   }

   IE_EA = 1;

   while (1)
   {
       while(!S1);                     // Force the MCU to not kick the WDT as
                                       // long as S1 is pressed.  If S1 is
                                       // held for long enough, a WDT reset
                                       // will occur.

       WDTCN = 0xA5;                   // Reset the WDT
    }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
