
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8SB1_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
// $[Generated Includes]
#include "cslib_config.h"
#include "cslib.h"
#include "profiler_interface.h"
#include "comm_routines.h"
// [Generated Includes]$

SI_SBIT (LED0, SFR_P1, 1);          // '1' means ON, '0' means OFF


//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	// Call hardware initialization routine
	enter_DefaultMode_from_RESET();
	P0 |= 0x01;
	P2MDOUT |= 0x80;
	P2 &= ~0x80;

  // Initializes the UART interface
  CSLIB_commInit();

  // Configures all peripherals controlled by capsense, including
  // the sensing block and port pins
  CSLIB_initHardware();

  // Initializes the capsense variables and performs some scans to
  // initialize the baselines
  CSLIB_initLibrary();

	while (1) 
   {
		// -----------------------------------------------------------------------------
		// Performs all scanning and data structure updates
		// -----------------------------------------------------------------------------
		CSLIB_update();

		CSLIB_commUpdate();

 		// -----------------------------------------------------------------------------
		// If low power features are enabled, this will either put the device into a low
		// power state until it is time to take another scan, or put the device into a
		// low-power sleep mode if no touches are active
		// -----------------------------------------------------------------------------

		CSLIB_lowPowerUpdate();

		if(CSLIB_anySensorDebounceActive())
		{
			LED0 = 1;
		}
		else
		{
			LED0 = 0;
		}
   }                             


}
