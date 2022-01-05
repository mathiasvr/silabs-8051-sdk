//-----------------------------------------------------------------------------
// SB1_Timer0_13bitExtTimer.c
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
//    P0.1 -> T0 (Timer0 External Input)
//    P0.6 -> /INT0
//    P1.2 -> BUTTON1 (switch)
//
//    P1.0 -> SIGNAL (digital, push-pull)
//    P0.7 -> GTE (digital, push-pull)
//    P2.0 -> LED (digital, push-pull)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Place the switch in "AEM" mode.
// 2) Connect the following pins:
//                   P0.1 <--> P1.0 (T0 with SIGNAL)
//                   P0.6 <--> P0.7 (/INT0 with GTE)
// 3) To change the number of input pulses/interrupt, modify
//    PULSES_PER_TOGGLE in Interrupts.c
// 4) To change the speed of the SIGNAL waveform, modify
//    SOFTWARE_DELAY
// 5) Connect the EFM8SB2 STK board to a PC using a mini USB cable.
// 6) Compile and download code to the EFM8SB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 7) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 8) To enable the counting, press and hold BUTTON1 (switch), which will be
//    polled to enable the timer.
// 9) The LED will blink and SIGNAL can be observed on an oscilloscope.
//
//
// Target:         EFM8SB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 16 JAN 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>
#include <SI_EFM8SB2_Register_Enums.h>                  // SI_SFR declarations
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

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 6);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (GTE, SFR_P0, 7);                 // Gate control signal for Timer0
SI_SBIT (LED, SFR_P1, 1);                 // LED==0 means ON
SI_SBIT (SIGNAL, SFR_P1, 0);              // SIGNAL is used to input pulses into
                                       // T0 pin
SI_SBIT (BUTTON1, SFR_P0, 2);             // Button that enables counting

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	volatile uint8_t counter = 0;

	//Enter default mode
	enter_DefaultMode_from_RESET();

	DISP_EN = DISP_BC_DRIVEN;

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
}
