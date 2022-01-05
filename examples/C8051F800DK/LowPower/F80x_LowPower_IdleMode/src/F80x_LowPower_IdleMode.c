//-----------------------------------------------------------------------------
// F80x_LowPower_IdleMode.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This code puts C8051F800 in idle mode for a low power consuming state
//  to verify the current consumption numbers given for idle mode at
//  an internal oscillator speed of 1.25 MHz.
//
// How to test:
//
// 1) To get your board into its lowest power configuration, first read the
//       steps in the C8051F800 User's Guide under the Frequently Asked
//       Questions section. 
// 2) Ensure shorting blocks are place on the following:
//    - J2
// 3) Connect the USB Debug Adapter to J4.
// 4) Compile and download code to the 'F85x UDP MCU board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 6) Disconnect the USB Debug Adaptor from the board
// 7) Verify that the current consumption from the external power supply
//     is about 330 uA.
//
// Target:        C8051F80x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:  None
//
// Revision History:
//
// Release 1.0 / 10 JUL 2008 (PD)
//    -Initial Revision
//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------

#include "si_toolchain.h"
#include <SI_C8051F800_Defs.h>         // SFR declarations

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
   PCA0MD &= ~0x40;                    // WDTE = 0 (disable watchdog timer)
}
 
//----------------------------------------------------------------------------
// main() Routine
//----------------------------------------------------------------------------

void main(void)
{

   OSCICN |= 0x80;                     // Enable the precision internal osc.
      
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x70;                      // Select precision internal osc. 
                                       // divided by 128 as the system clock
      
   PCON |= 0x01;                       // Idle mode on
   PCON = PCON;                        // ... followed by a 3-cycle dummy instruction
}
