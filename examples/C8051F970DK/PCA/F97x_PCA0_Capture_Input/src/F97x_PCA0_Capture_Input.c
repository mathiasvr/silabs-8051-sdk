//-----------------------------------------------------------------------------
// F97x_PCA0_Capture_Input.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Use the PCA to time external IO events
//
// This program uses the PCA's capture mode to time events on an input pin.
// The difference between subsequent captures is calculated and stored in
// memory.
//
// In this example, PCA Module 0 is used to capture rising edges of an
// incoming digital waveform, and the crossbar is configured to receive the
// CEX0 pin on on P0.3.  The PCA is configured to clock once every 10 us.
//
// Timer2 overflows are used to toggle pin P0.1 at an overflow rate specified
// by the definition T2_RELOAD_CLOCKS. This signal at P0.1 is provided simply
// as a test method for the code.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
// 2) SELF TEST:
//     - Add a jumper from P0.1 to P0.3 (T2 Output <> PCA0 CEX0)
//    EXTERNAL SOURCE:
//     - Connect an external signal to P0.3 (PCA0 CEX0)
// 3) Place the VDD SELECT switch in the VREG position and power the board.
// 4) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 5) Add "capture_period" to the Watch List
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) Wait a few seconds and then pause the execution of the program.
// 8) SELF TEST:
//     - Verify that "capture_period" == 50 (500us)
//    EXTERNAL SOURCE:
//     - Verify that "capture_period" is equal to the source's period
//       in units of 10us (IE: 50 = 500us)
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 30 APR 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
SI_SBIT(TEST_OUT, SFR_P0, 1);            // Output for testing purposes

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

SI_INTERRUPT_PROTO(PCA0_ISR, PCA0_IRQn);

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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main(void)
{
  enter_DefaultMode_from_RESET();

  while (1) {
    // check to see if Timer2 overflowed
    if ((TMR2CN & TMR2CN_TF2H__BMASK) == TMR2CN_TF2H__SET) {
      TMR2CN &= ~TMR2CN_TF2H__BMASK;                   // Clear T2 overflow
      TEST_OUT = !TEST_OUT;         // Toggle test pin
    }
  }
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This is the ISR for the PCA.  It handles the case when a capture occurs on
// channel 0, and updates the variables to reflect the new capture information.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
  static unsigned int current_capture_value, previous_capture_value;
  static unsigned int capture_period;

  if (PCA0CN_CCF0 != 0) // If Module 0 caused the interrupt
      {
    PCA0CN_CCF0 = 0;   // Clear module 0 interrupt flag.

    // Store most recent capture value
    current_capture_value = PCA0CP0;

    // Calculate capture period from last two values.
    capture_period = current_capture_value - previous_capture_value;

    // Update previous capture value with most recent info.
    previous_capture_value = current_capture_value;
  } else // Interrupt was caused by other bits.
  {
    PCA0CN &= ~(PCA0CN_CF__BMASK |   // Clear other interrupt flags for PCA
        PCA0CN_CCF2__BMASK |
        PCA0CN_CCF1__BMASK);
  }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
