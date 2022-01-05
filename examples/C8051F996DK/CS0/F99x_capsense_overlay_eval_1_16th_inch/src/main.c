//-----------------------------------------------------------------------------
// CSLibraryExample_NoOverlay
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
//Capacitive Sensing library example tuned for a 1/8 inch acrylic overlay
//
// This example uses the capacitive sensing firmware library to detect
// touches on the 24 sensors found on the F970 target board.  The library
// configuration files are set to use touch detection thresholds appropriate
// for sensors with a 1/8 inch overlay.
//
// When firmware detects a touch on C8 or C9, the DS4 LED lights up.  If a second
// touch is detected, DS3 also lights up.
//
// For more information on the capacitive sense library and
// Capacitive Sense Profiler, see AN0828: “Capacitive Sensing Library Overview”
// and AN0829: “Capacitive Sensing Library Configuration Guide” by clicking the
// Application Notes tile from the main Simplicity Studio launcher.
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>             // compiler declarations
#include "SI_C8051F990_Defs.h"
#include "cslib_config.h"
#include "cslib.h"
#include "profiler_interface.h"
#include "comm_routines.h"
#include "device_init.h"
// Modules
//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 7);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   uint8_t count, index;



   systemInit();                       // Configures all peripherals not
                                       // controlled by the capsense library

   DISP_EN = DISP_BC_DRIVEN;


   CSLIB_commInit();                   // Initializes the UART interface

   CSLIB_initHardware();               // Configures all peripherals
                                       // controlled by capsense, including
                                       // the sensing block and port pins

   CSLIB_initLibrary();                // Initializes capsense variables
                                       // and performs some scans to initialize
                                       // the baselines





   while(1)
   {

      CSLIB_update();                  // Performs all scanning and data
                                       // structure updates

      CSLIB_commUpdate();
 		// -----------------------------------------------------------------------------
		// If low power features are enabled, this will either put the device into a low
		// power state until it is time to take another scan, or put the device into a
		// low-power sleep mode if no touches are active
		// -----------------------------------------------------------------------------
      CSLIB_lowPowerUpdate();
      count = 0;
      for(index = 0; index < CSLIB_numSensors; index++)
      {
         if(CSLIB_isSensorDebounceActive(index))
            count++;
      }
      if(count == 0)
      {
         P1 |= 0x08;
         P1 |= 0x20;
      }
      else if(count==1)
      {
         P1 &= ~0x08;
         P1 |= 0x20;
      }
      else
      {
         P1 &= ~0x08;
         P1 &= ~0x20;
      }
   }



}
