//-----------------------------------------------------------------------------
// F3xx_USB_Main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to communicates with a PC application to
// control the LED blinking.
//
//
// How To Test:    See Readme.txt
// ------------
// 1) Ensure shorting blocks on C8051F326-TB are place on the following:
//    J2: VDD  - +3VD
//    J3: P2.2 - P2.2_LED
//    J3: P2.3 - P2.3_LED
//    J7: VDD  - VIO
//    J8: Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1.
// 3) Include the appropriate "F3xx_Blink_Control_F326.c" file to build and
//    exclude other device source files from build by right-clicking on these
//    files in the project window and selecting
//    Resource Configurations -> Exclude from Build... to remove objects from
//    building.
// 4) Compile and download code to C8051F326 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 6) Connect USB cable to J9 and PC.
// 7) Run the GUI application HID_Blinky that comes with the firmware
//    and test the features.
//
//
// Target:         C8051F326/7
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
// Project Name:   F3xx_BlinkyExample
//
//
// Release 1.3 (JL)
//    - Updated description.
//    - 13 MAY 2014
//
// Release 1.2 (ES)
//    -Added support for Raisonance
//    -No change to this file
//    -02 APR 2010
// Release 1.1
//    -Added feature reports for dimming controls
//    -Added PCA dimmer functionality
//    -16 NOV 2006
// Release 1.0
//    -Initial Revision (PD)
//    -07 DEC 2005
//
//-----------------------------------------------------------------------------
// Header Files
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_USB0_Register.h"
#include "F3xx_Blink_Control.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_Descriptor.h"

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------


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
void main(void)
{

   System_Init ();
   Usb_Init ();

   IE_EA = 1;

   while (1)
   {
      if (BLINK_SELECTORUPDATE)
      {
         BLINK_SELECTORUPDATE = 0;
         SendPacket (IN_BLINK_SELECTORID);
      }
   }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
