//-----------------------------------------------------------------------------
// F70x_LowPower_IdleMode.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This code puts c8051f700 in idle mode for a low power consuming state
//  to verify the current consumption numbers given for idle mode at
//  a internal oscillator speed of 1.25 MHz.
//
// How to test:
//
// 1) To get your board into its lowest power configuration, first read the
//       steps in the c8051f700 User's Guide under the Frequently Asked 
//       Questions section. 
// 2) Ensure shorting blocks are place on the following:
//	  - J15: VDD - power source
// 3) Connect the USB Debug Adapter to P3.
// 4) Connect the USB cable to P4.
// 5) Compile and download code to the 'F70x target board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Disconnect the USB cable and USB Debug Adaptor and connect an
//    external power supply.
// 8) Verify that the current consumption from the external power supply
//     is about 330 uA.
//
// Target:        C8051F70x
// Tool chain:    Simplicity Studio / Keil C51 9.53
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
#include <SI_c8051f700_defs.h>                // SFR declarations

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
   WDTCN = 0xDE;                       // Disable watchdog timer
   WDTCN = 0xAD;
}
 
//----------------------------------------------------------------------------
// main() Routine
//----------------------------------------------------------------------------

void main(void)
{

   SFRPAGE = CONFIG_PAGE;
   OSCICN |= 0x80;                     // Enable the precision internal osc.
      
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x70;                      // Select precision internal osc. 
                                       // divided by 128 as the system clock
      
   PCON |= 0x01;                       // Idle mode on
}
