//-----------------------------------------------------------------------------
// Timer0_13bitExtTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer0 of the EFM8UB2's in
// 13-bit counter/timer in counter mode from an external pin. It uses two I/O
// pins; one to create the input pulses (SIGNAL) to be counted and another one
// to enable the counting process (GTE).
//
// This code uses the EFM8UB2DK as HW platform.
//
// Pinout:
//
//    P1.0 -> T0 (Timer0 External Input)
//    P0.6 -> /INT0
//
//    P0.7 -> GTE (digital, push-pull)
//
//    P3.0 -> SIGNAL (digital, push-pull)
//
//    P0.2 -> BUTTON1 (switch)
//
//    P1.6 -> LED1 (digital, push-pull)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connnect the following pins:
//                   P1.0 <--> P3.0 (T0 with SIGNAL)
//                   P0.6 <--> P0.7 (/INT0 with GTE)
// 3) To change the number of input pulses/interrupt, modify
//    PULSES_PER_TOGGLE.
// 4) To change the speed of the SIGNAL waveform, modify
//    SOFTWARE_DELAY
// 5) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 6) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 7) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 8) To enable the counting, press and hold PB0 (switch), which will be
//    polled to enable the timer.
// 9) The LED will blink and SIGNAL (P3.0) can be observed on an oscilloscope.
//
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
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             12000000/8  // SYSCLK in Hz (12 MHz internal
                                       // oscillator / 8)
                                       // the internal oscillator has a
                                       // tolerance of +/- 2%

#define SOFTWARE_DELAY  SYSCLK/200000  // Software timer to generate the
                                       // SIGNAL output
                                       // Generate a signal in the kHz range

SI_SBIT (GTE, SFR_P0, 7);                 // Gate control signal for Timer0
SI_SBIT (SIGNAL, SFR_P3, 0);              // SIGNAL is used to input pulses into
                                          // T0 pin
SI_SBIT (BUTTON1, SFR_P0, 2);             // Button that enables counting

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
   uint16_t counter;

   enter_DefaultMode_from_RESET();

   while (1)
   {
      if (BUTTON1 == 0)                 // If button pressed, enable counting
      {
         GTE = 1;
      }
      else
      {
         GTE = 0;
      }

      // Wait a certain time before toggling signal
      for (counter=0; counter < SOFTWARE_DELAY; counter++);

      SIGNAL = ~SIGNAL;                // Toggle the SIGNAL pin
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
