
//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>             // compiler declarations
#include "SI_C8051F970_Defs.h"
#include "InitDevice.h"
#include "cslib_config.h"
#include "cslib.h"
#include "profiler_interface.h"
#include "comm_routines.h"
// Modules

#include "SliderLibrary.h"
#include "SliderLEDControl.h"

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   uint8_t count, index;

   enter_DefaultMode_from_RESET();
   SCON0_TI = 1; // enable printf

  // Initializes the UART interface
  CSLIB_commInit();

   CSLIB_initHardware();               // Configures all peripherals
                                       // controlled by capsense, including
                                       // the sensing block and port pins

   CSLIB_initLibrary();                // Initializes capsense variables
                                       // and performs some scans to initialize
                                       // the baselines

   InitSliders();

   InitSliderLEDControl();

   while(1)
   {
      CSLIB_update();                  // Performs all scanning and data
                                       // structure updates
      UpdateSliders();


      CSLIB_commUpdate();
 		// -----------------------------------------------------------------------------
		// If low power features are enabled, this will either put the device into a low
		// power state until it is time to take another scan, or put the device into a
		// low-power sleep mode if no touches are active
		// -----------------------------------------------------------------------------
      CSLIB_lowPowerUpdate();

      SliderLEDUpdate(Slider[0].centroid, Slider[0].active);
      count = 0;
      for(index = 4; index < CSLIB_numSensors; index++)
      {
         if(CSLIB_isSensorDebounceActive(index))
            count++;
      }
      if(count == 0)
      {
         P0 |= 0x20;
         P1 |= 0x20;
      }
      else if(count==1)
      {
         P0 &= ~0x20;
         P1 |= 0x20;
      }
      else
      {
         P0 &= ~0x20;
         P1 &= ~0x20;
      }
   }



}
