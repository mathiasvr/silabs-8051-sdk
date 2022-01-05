//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example implements a USB HID joystick using the on-board joystick and push
// buttons for direction and left and right button presses.
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB1 STK
//-----------------------------------------------------------------------------
// 1) Place the SW104 switch in "AEM" mode.
// 2) Connect the EFM8UB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The HID joystick demo should start.
// 6) Connect a micro USB cable from the PC to the STK.
// 7) The HID joystick should enumerate on the PC as a game pad.
// 8) Use the joystick to move the directional pad and press the left and right
//    push buttons to activate button 1 and button 2 respectively.
// 9) To disable low energy USB mode, short P0.0 to ground (short EXP1 to EXP3).
//
// Target:         EFM8UB1
// Tool chain:     Generic
//
// Release 0.1 (CM)
//    - Initial Revision
//    - 10 OCT 2014
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 24.5 MHz HFOSC0 / 1
// ADC0   - 10-bit
// USB0   - Full speed
// P0.0 - Low energy mode enable pin (active low)
// P0.2 - push button
// P0.3 - push button
// P1.7 - Joystick (analog voltage divider)
// P2.3 - Display enable
//

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "InitDevice.h"
#include "efm8_usb.h"
#include "descriptors.h"
#include "idle.h"
#include "button.h"
#include "usbconfig.h"

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

uint8_t tmpBuffer;

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
//
// Note: the software watchdog timer is not disabled by default in this
// example, so a long-running program will reset periodically unless
// the timer is disabled or your program periodically writes to it.
//
// Review the "Watchdog Timer" section under the part family's datasheet
// for details.
//
//-----------------------------------------------------------------------------
int16_t main(void)
{
  // Disable the WDT
  WDT_0_enter_DefaultMode_from_RESET();

  // Debug trap to prevent LP mode lockup
  while (BSP_PB0 == BSP_PB_PRESSED);

  enter_DefaultMode_from_RESET();

  BSP_DISP_EN = 0;

  while (1)
  {
    // Enter idle mode to save power
    // Will resume due to USB SOF interrupt
    PCON0 |= PCON0_IDLE__IDLE;
    NOP();
    NOP();
    NOP();
  }
}
