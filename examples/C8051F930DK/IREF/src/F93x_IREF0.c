//-----------------------------------------------------------------------------
// F93x_IREF0.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows how to enable and configure the current reference
// to source current in Low Power Mode, where each step is 1uA and the
// maximum current is 63uA. When R14 is populated with a 10K Ohm resistor,
// the voltage on P0.7/IREF0 should vary between 0V and 0.63V. To keep the
// output voltage/current constant, press the P0.2 switch.
//
// To change from low power mode to high current mode, simply go to modify the current
// reference module in Configurator. In high current mode, each step is 8uA and the maximum
// current is 504uA. In this mode, the voltage should vary between 0V and 0.95V
// when a 10K Ohm resistor is used at the IREF0 output.
//
// Pinout:
//
// P0.2 - Switch
// P0.7 - IREF0 output
// P1.5 - Red LED
// P1.6 - Yellow LED
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J8:   P0.2/SW0.2
//       J7:   P0.7/IREF
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Ensure that C19 and R14 are populated.
// 2) Connect the USB Debug Adapter to J9.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 4) Place voltmeter or oscilloscope on P0.7/IREF0.
// 5) To hold the voltage/current at its current value, press the
//    P0.2 Switch.
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.2
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.1
//    - Compiled and tested for C8051F930-TB (JH)
//    - 06 JULY 2009
//
// Release 1.0
//    - Initial Revision (FB)
//    - 15 AUG 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(YELLOW_LED, SFR_P1, 6);          // '0' means ON, '1' means OFF
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define LED_ON                0
#define LED_OFF               1

     	 	 	 	 	 	 	 	 	       // Set the maximum output code depending
#define MAX_VALUE  0x0C             // on the currently selected current

#define MIN_VALUE     0x00             // Set the minimum output code

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Wait_MS(uint16_t ms);         // Inserts programmable delay

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
	bit rising = 1;

	//Enter default mode
	enter_DefaultMode_from_RESET();

	while(1)
	{
	  // If P0.2 Switch is not pressed
	  if(SW2)
	  {
		 //----------------------------------------
		 // Set LED State
		 //----------------------------------------

		 // Turn off the Yellow LED
		 YELLOW_LED = LED_OFF;

		 //----------------------------------------
		 // Control output voltage/current
		 //----------------------------------------

		 // If output voltage should be increasing
		 if(rising)
		 {
			// Increment the output current
			IREF0CN++;

			// If we have reached the maximum value
			if((IREF0CN & 0x3F) == MAX_VALUE)
			{
			   // Change Direction
			   rising = 0;
			}
		 } else

		 // Output voltage should be decreasing
		 {
			// Decrement the output current
			IREF0CN--;

			// If we have reached the minimum value
			if((IREF0CN & 0x3F) == MIN_VALUE)
			{
			   // Change Direction
			   rising = 1;
			}
		 }

		 //----------------------------------------
		 // Insert Delay
		 //----------------------------------------

		 Wait_MS(50);                  // Wait 50 milliseconds

	  } else

	  // P0.2 switch is pressed
	  {

		 //----------------------------------------
		 // Set LED State
		 //----------------------------------------

		 // Turn on the Yellow LED
		 YELLOW_LED = LED_ON;

		 //----------------------------------------
		 // Control output voltage/current
		 //----------------------------------------

		 // Keep the current value as-is.

		 //----------------------------------------
		 // Insert Delay
		 //----------------------------------------

		 // There is no need to insert a delay.

	  }


	} // end while(1)

	// NOTREACHED
	return 0;
}
//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Wait_MS
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters:
//   1) uint16_t ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 65335
//
// This routine inserts a delay of <ms> milliseconds.
//-----------------------------------------------------------------------------
void Wait_MS(uint16_t ms)
{
   char i;

   TCON_TR0 = 0;                            // Stop Timer0

   TL0 = TH0;                          // Init Timer0 low byte to the
                                       // reload value

   TCON_TF0 = 0;                            // Clear Timer0 Interrupt Flag
   IE_ET0 = 0;                            // Timer0 interrupt disabled
   TCON_TR0 = 1;                            // Timer0 on

   while(ms--)
   {
      for(i = 0; i < 10; i++)
      {
         TCON_TF0 = 0;
         while(!TCON_TF0);
      }
   }

   TCON_TF0 = 0;

}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
