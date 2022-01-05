//-----------------------------------------------------------------------------
// EFM8BB1_LED_Pulse.c
//-----------------------------------------------------------------------------
// Copyright 2018 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program pulses the LED on the EFM8BB1 LCK board using the PCA PWM
// output. Pressing BTN0 changes the PWM frequency, changing the rate at which
// the LED pulses.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC
//   Timer0 - PCA clock (overflows)
//   PCA  - PWM Output to LED
//   P0.2 - BTN0
//   P1.4 - LED
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 LCK
//-----------------------------------------------------------------------------
// 1) Connect the EFM8BB1 LCK board to a PC using a micro USB cable.
// 2) Compile and download code to the EFM8BB1 LCK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 3) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 4) The LED should pulse at approximately 0.5 Hz.
// 5) Pressing BTN0 will increase the pulse frequency, up to 6 Hz.
// 6) Pressing BTN0 when at 6 Hz pulse will reset the pulse rate to 0.5 Hz.
//
// Target:         EFM8BB1
// Tool chain:     Generic
//
// Release 0.1 (BML)
//    - Initial Revision
//    - 17 OCT 2018
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB1_Register_Enums.h>
#include <InitDevice.h>

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT(BTN0, SFR_P0, 2);
#define BTN0_PRESSED (0)
#define BTN0_RELEASED (1)

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
void main (void)
{
   enter_DefaultMode_from_RESET();
   while (1) {
     // Idle until next interrupt (PCA cycle overflow)
     PCON0 = PCON0_IDLE__IDLE;

     // Re-enable portmatch interrupts once BTN0 is released
     if (BTN0 == BTN0_RELEASED) {
       EIE1 |= EIE1_EMAT__ENABLED;
     }
   }                        // Spin forever
}
