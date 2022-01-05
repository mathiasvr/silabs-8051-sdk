//-----------------------------------------------------------------------------
// F99x_IREF0.c
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
// The compile option <CURRENT_MODE> may be set to "HIGH_CURRENT" to place
// IREF0 in High Current mode, where each step is 8uA and the maximum
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
//       J7:   P0.7/IREF0
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F99x device on a C8051F99x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Place voltmeter or oscilloscope on P0.7/IREF0.
// 7) To hold the voltage/current at its current value, press the
//    P0.2 Switch.
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
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(YELLOW_LED, SFR_P1, 3);          // '0' means ON, '1' means OFF
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define LED_ON                0
#define LED_OFF               1

#define LOW_POWER             0
#define HIGH_CURRENT          1

#define CURRENT_MODE       HIGH_CURRENT   // "LOW_POWER" or "HIGH_CURRENT"

#if(CURRENT_MODE == HIGH_CURRENT)      // Set the maximum output code depending
   #define MAX_VALUE  0x0C             // on the currently selected current
#else                                  // mode
   #define MAX_VALUE  0x3F
#endif

#define MIN_VALUE     0x00             // Set the minimum output code

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

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
 
//-
//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	bit rising = 1;

	#if(CURRENT_MODE == HIGH_CURRENT)

		enter_HighCurrentMode_from_RESET();		// Configure IREF0 as a current source
									   	   	    // in High Current mode

	#else

		enter_LowPowerMode_from_RESET();	// Configure IREF0 as a current source
									   	    // in Low Power mode
	#endif

	while (1)
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

	}                             // Spin forever

	// NOTREACHED
	return 0;
}

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
