/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "iec60730.h"
#include "iec60730_oem_plausibility.h"
#include "bsp.h"
#include "InitDevice.h"
#include "iec60730_oem_comm.h"
// $[Generated Includes]
// [Generated Includes]$


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
 
/**************************************************************************//**
 * public main() routine
 *
 * @returns int Not used.
 * 
 * Main function for the IEC60730 demo. Includes initialization, POST,
 * BIST, Analog Plausibility and GPIO Plausibility. 
 * Infinite loop, does not return.
 *****************************************************************************/
int main(void) {
#ifdef DEBUG // TEST FUNCTIONS
  // Set P1 as digital 
  P1MDIN = 0xFF;
  // Skip P1 ports on the crossbar
  P1SKIP = 0xFF;
  // Enable XBAR
  XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;
#endif

  // Note that disabling the BC is needed for the loopback UART interface
  // that demonstrates the serial plausiblity test

  BSP_BC_EN = BSP_BC_DISCONNECTED;     // Board controller disconnected from EFM8
                                       // UART pins

  IE_EA = 1;

  // Call hardware initialization routine
  enter_DefaultMode_from_RESET();

  iec60730_GpioInit();

  iec60730_Post();

  // This variable is only used for automated testing of 
  // iec60730_updateCRCWithDataBuffer(). Do not include it in any OEM code.
  iec60730_crcCheckTestData = 0x5555;

  while (1) {
    // OEM defined GPIO check
    iec60730_GpioCheck();
    // OEM defined Analog check
    iec60730_AnalogCheck();
    // OEM code will set IEC60730_OEMx_COMPLETE in their code and interrupts
    // Set unused IEC60730_OEMx_COMPLETE bits to 1
    iec60730_programmeCounterCheck |= IEC60730_OEM0_COMPLETE
                                    | IEC60730_OEM1_COMPLETE
                                    | IEC60730_OEM2_COMPLETE
                                    | IEC60730_OEM3_COMPLETE
                                    | IEC60730_OEM4_COMPLETE
                                    | IEC60730_OEM5_COMPLETE
                                    | IEC60730_OEM6_COMPLETE
                                    | IEC60730_OEM7_COMPLETE;
    iec60730_Bist();
  }
}
