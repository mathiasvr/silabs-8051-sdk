//-----------------------------------------------------------------------------
// EFM8BB1_Timer0_13bitExtTimer.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer0 in 13-bit
// counter/timer mode from an external pin. It uses two I/O pins; one to create
// the input pulses (SIGNAL) to be counted and another one to enable the
// counting process (GATE).
//
// Pinout:
//
//    P0.0 -> T0 (Timer0 External Input)
//    P0.1 -> /INT0
//
//    P1.4 -> LED (digital, push-pull)
//
//    P0.6 -> SIGNAL (digital, push-pull)
//    P0.7 -> GATE (digital, push-pull)
//
//    P0.2 -> PB0 (switch)
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) Externally connect the following pins:
//                      P0.0 (T0) <--> P0.6 (SIGNAL)
//                   P0.1 (/INT0) <--> P0.7 (GATE)
// 2) Power the board using the DBG USB connector.
// 3) Modify PULSES_PER_TOGGLE to change the number of input pulses/interrupt.
// 4) Modify SOFTWARE_DELAY to change the speed of the SIGNAL waveform.
// 5) Compile and download code to the EFM8BB1 board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 7) To enable the counting, press and hold PB0 (switch). Firmware will set or
//    clear the state of the GATE signal based on the switch value.
// 8) The LED will blink and SIGNAL can be observed on an
//    oscilloscope.
//
// Target:         EFM8BB1
// Command Line:   None
//
//
// Release 1.2 (BL)
//		- Updated How To Test with Studio specific instructions
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

#include <SI_EFM8BB1_Register_Enums.h>
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
// Configurator set for HFOSC/8 =
#define SYSCLK     3062000
#define SOFTWARE_DELAY (SYSCLK/100000L)// Software timer to generate the
                                       // SIGNAL output
                                       // Generate a signal in the kHz range

#define SW_PRESSED                   0
#define SW_NOT_PRESSED               1

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT(SIGNAL, SFR_P0, 6);            // SIGNAL is used to input pulses into
                                       // T0 pin
SI_SBIT(GATE, SFR_P0, 7);              // Gate control signal for Timer0
SI_SBIT(S1, SFR_P0, 2);                // Switch that enables counting


//-----------------------------------------------------------------------------
// Function Prototypes
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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   volatile uint16_t counter = 0;

   enter_DefaultMode_from_RESET();

   IE_EA = 1;                          // Enable global interrupts

   while (1)
   {
      if (S1 == SW_PRESSED)
      {
         GATE = 1;                     // Enable counting
      }
      else
      {
         GATE = 0;                     // Disable counting
      }

      // Wait a certain time before toggling signal
      for(counter = 0; counter < SOFTWARE_DELAY; counter++);

      SIGNAL = !SIGNAL;                // Toggle the SIGNAL pin
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
