//-----------------------------------------------------------------------------
// PCA_Lib_FrequencyOutput.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends a square wave out of an I/O pin, using the PCA's
// Frequency Output Mode.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P1.4.
//
//    How "Frequency Output Mode" Works:
//
//    The PCA's Frequency Output Mode works by toggling an output pin every
//    "N" PCA clock cycles.  The value of "N" should be loaded into the PCA0CPH
//    register for the module being used (in this case, module 0).  Whenever
//    the register PCA0L (PCA counter low byte) and the module's PCA0CPL value
//    are equivalent, two things happen:
//
//    1) The port pin associated with the PCA module toggles logic state
//    2) The value stored in PCA0CPH is added to PCA0CPL.
//
//    Using this mode, a square wave is produced at the output port pin of the
//    PCA module. The speed of the waveform can be changed by changing the
//    value of the PCA0CPH register.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 1
//   PCA    - Frequency Output
//   P2.3   - Display enable
//   P1.4   - PCA CEX0 / LED green
//
//-----------------------------------------------------------------------------
// How To Test: EFM8LB1 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8LB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8LB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Place an oscilloscope probe on P1.4. The waveform should be visible
//    on the oscilloscope.
//
// Target:         EFM8LB1
// Tool chain:     Generic
//
// Release 0.1 (ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "InitDevice.h"
#include "pca_0.h"

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
// Main Routine
//-----------------------------------------------------------------------------
void main(void)
{
  enter_DefaultMode_from_RESET();

  BSP_DISP_EN = 0;           // Display not driven by EFM8

  while (1);
}
