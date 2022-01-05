//-----------------------------------------------------------------------------
// F99x_Timer0_13bitExtTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer0 in
// 13-bit counter/timer in counter mode from an external pin. It uses two I/O
// pins; one to create the input pulses (SIGNAL) to be counted and another one
// to enable the counting process (GTE).
//
//
// Pinout:
//
//    P0.0 -> T0 (Timer0 External Input)
//    P0.1 -> /INT0
//    P0.2 -> BUTTON1 (switch)
//
//    P1.4 -> SIGNAL (digital, push-pull)
//    P1.5 -> GTE (digital, push-pull)
//    P1.3 -> LED (digital, push-pull)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J12:  P1.3/LED1.3
//             P0.2/SW0.2
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connnect the following pins:
//                   P0.0 <--> P1.4 (T0 with SIGNAL)
//                   P0.1 <--> P1.5 (/INT0 with GTE)
// 3) Connect the USB Debug Adapter to J4.
// 4) Turn the POWER switch (SW5) to the "ON" position.
// 5) To change the number of input pulses/interrupt, modify
//    PULSES_PER_TOGGLE in Interrupts.c
// 6) To change the speed of the SIGNAL waveform, modify
//    SOFTWARE_DELAY
// 7) Compile and download code to a 'F99x device on a C8051F99x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) To enable the counting, press and hold BUTTON1 (switch), which will be
//    polled to enable the timer.
// 10) The LED will blink and SIGNAL can be observed on an oscilloscope.
//
//
// Target:         C8051F99x-C8051F98x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 13 JAN 2014
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F990_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK           (20000000L/8L) // SYSCLK in Hz (20 MHz internal
                                       // low power oscillator / 8)
                                       // the low power oscillator has a
                                       // tolerance of +/- 10%
                                       // the precision oscillator has a
                                       // tolerance of +/- 2%

#define SOFTWARE_DELAY  (SYSCLK/100000L) // Software timer to generate the
                                       // SIGNAL output
                                       // Generate a signal in the kHz range

SI_SBIT(GTE, SFR_P1, 5);                 // Gate control signal for Timer0
SI_SBIT(LED, SFR_P1, 3);                 // LED==0 means ON
SI_SBIT(SIGNAL, SFR_P1, 4);              // SIGNAL is used to input pulses into
                                       // T0 pin
SI_SBIT(BUTTON1, SFR_P0, 2);             // Button that enables counting

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

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
// ----------------------------------------------------------------------------
int main (void)
{
	volatile uint8_t counter = 0;

	//Enter default mode
	enter_DefaultMode_from_RESET();

	LED = 1;                            // Turn off LED

	while (1)
	{
		if (BUTTON1 == 0)                // If button pressed, enable counting
		{
		 GTE = 1;
		}
		else
		{
		 GTE = 0;
		}

		// Wait a certain time before toggling signal
		for(counter = 0; counter < SOFTWARE_DELAY; counter++);

		SIGNAL = !SIGNAL;                // Toggle the SIGNAL pin
	}                             // Spin forever

	// NOTREACHED
	return 0;
}
