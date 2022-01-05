//-----------------------------------------------------------------------------
// F970_Ports_PortMatch.c
//-----------------------------------------------------------------------------
// Copyright 2010 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Wake up MCU from IDLE when Switches are pressed 
//
// This program demonstrates how to use the Port Match feature.  The firmware
// enables the port match interrupt and puts the device in idle mode.  Pressing
// a switch wakes the device up from idle mode and turns off an LED in the Port
// Match ISR. The port match is inverted to then trigger on the next release of
// the switch. The firmware then enters idle mode until the switch release
// triggers another port match and turns the LED back on.
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
// 5) Push the button BTN03 and see that the corresponding
//    LED05 turns off.
// 6) Release BTN03 to turn LED05 on again.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 02 MAY 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
SI_SBIT(LED05,   SFR_P0, 5);         // '0' means ON, '1' means OFF
SI_SBIT(BTN03,   SFR_P0, 3);         // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(PORT_Match_ISR, PMATCH_IRQn);

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
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
  enter_DefaultMode_from_RESET();

   while (1)
   {

      PCON |= PCON_IDLE__IDLE;         // Enter Idle mode
      NOP();                           // Safety dummy instruction

      // Pressing the switch pulls P0.3 low which triggers the Port Match
      // interrupt.  When the Port Match interrupt occurs, the device wakes up
      // from Idle mode and vectors to the Port_Match_ISR

      // Port Match is a level sensitive interrupt, so the firmware inverts the
      // P0.3 match value to cause it to be triggered on the next edge.

      // Once the ISR is finished, code execution resumes after the instruction
      // that put the device in idle mode
   }
}

//-----------------------------------------------------------------------------
// Interrupt Service Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Match_ISR
//-----------------------------------------------------------------------------
//
// Whenever the Port_Match ISR is active, toggle the LED. Reverse the polarity
// of the match value. This will cause the match to occur on both positive and
// negative edges.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(PORT_Match_ISR, PMATCH_IRQn)
{
   LED05 = !LED05;                          // Toggle LED

   // Flip P0.3 match polarity
   P0MAT = (P0MAT & ~P0MAT_B3__BMASK) | (P0 & P0_B3__BMASK);

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
