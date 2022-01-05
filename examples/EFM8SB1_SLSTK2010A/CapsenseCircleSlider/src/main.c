//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the use of the circular cap-sense slider on the
// EFM8SB1 STK using the LCD and cap sense buttons.
//
//-----------------------------------------------------------------------------
// How To Test: EFM8SB1 STK
//-----------------------------------------------------------------------------
// 1) Place the SW104 switch in "AEM" mode.
// 2) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Make sure that the cap-sense pads on the circular slider are not touched
//    during startup to allow the device to baseline the untouched pads.
// 6) After a second, touch the circular slider and move your finger
//    around the interior of the circular slider outlined in silk screen.
// 7) The LCD will display a ball on the screen at the same angle as the
//    touch location on the circular slider.
// 8) The LED brightness will change to indicate the angle:
//    12 o'clock - LED off
//    3  o'clock - LED 25% on
//    6  o'clock - LED 50% on
//    9  o'clock - LED 75% on
//
// Target:         EFM8SB1
// Tool chain:     Generic
//
// Release 0.1 (CM)
//    - Initial Revision
//    - 10 OCT 2014
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 20 MHz LPOSC / 1
// SPI0   - 1 MHz
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.1 - CS (Active High)
// P0.2 - CS0
// P0.3 - CS1
// P0.6 - SCK
// P1.0 - MOSI
// P1.1 - LED
// P1.5 - CS2
// P2.7 - Display enable
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "circle_slider.h"
#include "InitDevice.h"
// $[Generated Includes]
#include "cslib.h"
#include "cslib_config.h"
// [Generated Includes]$

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	// Call hardware initialization routine
	enter_DefaultMode_from_RESET();

  // LED is active high, turn it off by default
  BSP_LED0 = BSP_LED_OFF;

  circle_slider_init();

	while (1) 
   {
// $[Generated Run-time code]
		// -----------------------------------------------------------------------------
		// If low power features are enabled, this will either put the device into a low
		// power state until it is time to take another scan, or put the device into a
		// low-power sleep mode if no touches are active
		// -----------------------------------------------------------------------------
		CSLIB_lowPowerUpdate();

		// -----------------------------------------------------------------------------
		// Performs all scanning and data structure updates
		// -----------------------------------------------------------------------------
		CSLIB_update();

// [Generated Run-time code]$

		circle_slider_main();
   }                             
}
