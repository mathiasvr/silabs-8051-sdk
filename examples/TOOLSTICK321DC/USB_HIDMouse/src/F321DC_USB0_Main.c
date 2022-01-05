//-----------------------------------------------------------------------------
// F321DC_USB_Main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This application will communicate with a PC across the USB interface.
// The device will appear to be a mouse, and will manipulate the cursor
// on screen.
//
// How To Test:    See Readme.txt
//
//
// Target:         ToolStick321DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   See Readme.txt
// Project Name:   F321DC_MouseExample
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 21 MAR 2014
//
// Release 1.0
//    -Initial Revision (PD)
//    -27 MAR 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F320_Register_Enums.h"
#include "F321DC_USB0_InterruptServiceRoutine.h"
#include "F321DC_USB0_Mouse.h"

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
   USB0_Init ();

   IE_EA = 1;
   while (1)
   {
      SendPacket (0);
   }
}
