//-----------------------------------------------------------------------------
// F97x_External_Interupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Interrupt driven IO using Buttons and LEDs
//
// This software shows the necessary configuration to use External Interrupts 0
// and 1 as interrupt sources. If the BTN03 is pressed, then the edge-triggered
// /INT0 will cause an interrupt and toggle LED05. If BTN04 is pressed, then the
// /level-triggered /INT1 will continuously toggle LED15 until it is released.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) Press a button. When BTN03 is pressed, LED05 will toggle once. When BTN04
//    is pressed, LEd15 will continuously toggle until released.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 30 APR 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
SI_SBIT(LED05,   SFR_P0, 5);               // '0' means ON, '1' means OFF
SI_SBIT(LED15,   SFR_P1, 5);               // '0' means ON, '1' means OFF

SI_SBIT(BTN03,   SFR_P0, 3);               // '0' means ON, '1' means OFF
SI_SBIT(BTN04,   SFR_P0, 4);               // '0' means ON, '1' means OFF


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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
SI_INTERRUPT_PROTO(ExtInt0_ISR, INT0_IRQn);
SI_INTERRUPT_PROTO(ExtInt1_ISR, INT1_IRQn);
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   enter_DefaultMode_from_RESET();

   while(1);                           // Infinite while loop waiting for
                                       // an interrupt from /INT0 or /INT1
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// /Ext_Interrupt0_ISR
//-----------------------------------------------------------------------------
//
// Whenever a falling edge appears on P0.3, LED is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ExtInt0_ISR, INT0_IRQn)
{
   LED05 = !LED05; // Toggle LED
}

//-----------------------------------------------------------------------------
// /Ext_Interrupt1_ISR
//-----------------------------------------------------------------------------
//
// Whenever an active low level appears on P0.4, LED is toggled.
// The interrupt pending flag is not cleared. If the interrupt request is
// not deactivated, another interrupt request will occur as long as the level
// matches the INT1 configuration
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ExtInt1_ISR, INT1_IRQn)
{
   LED15 = !LED15; // Toggle LED
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
