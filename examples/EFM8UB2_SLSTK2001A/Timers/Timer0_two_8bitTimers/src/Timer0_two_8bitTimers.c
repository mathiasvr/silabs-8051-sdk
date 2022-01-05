//-----------------------------------------------------------------------------
// Timer0_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer0 of the EFM8UB2's in
// two 8-bit reload counter/timer mode. It uses the EFM8UB2DK as HW platform.
//
// It sets the two timers to interrupt every millisecond.
//
// When the timer low interrupts a counter (low_counter) increments and upon
// reaching the value defined by LED1_TOGGLE_RATE toggles LED1
// When the timer high interrupts a counter (high_counter) increments and upon
// reaching the value defined by LED2_TOGGLE_RATE toggles LED2
//
// Pinout:
//
//    P1.6 -> LED1 (toggled by TL0)
//    P1.7 -> LED2 (toggled by TH0)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) LED1 should blink at <LED1_TOGGLE_RATE>.
//    LED2 should blink at <LED2_TOGGLE_RATE>.
//
//
// Target:         EFM8UB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"

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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   enter_DefaultMode_from_RESET();

   while (1);                          // Loop forever
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
