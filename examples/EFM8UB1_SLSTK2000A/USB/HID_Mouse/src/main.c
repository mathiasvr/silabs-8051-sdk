//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example implements a USB HID mouse using the on-board joystick and push
// buttons for direction and left and right mouse button presses.
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
// 5) The HID Mouse demo should start.
// 6) Connect a micro USB cable from the PC to the STK.
// 7) The HID mouse should enumerate on the PC.
// 8) Use the joystick to move the mouse and press the left and right
//    push buttons to activate the left and right mouse buttons
//    respectively.
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
// SPI0   - 1 MHz
// USB0   - Full speed
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick and ADC start of conversion)
// P0.1 - CS (Active High)
// P0.2 - push button
// P0.3 - push button
// P0.6 - SCK
// P1.0 - MOSI
// P1.4 - LED Green
// P1.5 - LED Blue
// P1.6 - LED Red
// P1.7 - Joystick (analog voltage divider)
// P2.3 - Display enable
//

/////////////////////////////////////////////////////////////////////////////
// main.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "bsp.h"
#include "InitDevice.h"
#include "efm8_usb.h"
#include "descriptors.h"
#include "idle.h"
#include "disp.h"
#include "render.h"
#include "tick.h"
#include "draw.h"
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
  uint16_t lastTick = 0;
  uint16_t tick;

  enter_DefaultMode_from_RESET();

  DISP_Init();

  DrawSplashScreen();

  while (1)
  {
    // Update the LCD mouse status every 50 ms
    if (((tick = GetTickCount()) - lastTick) >= 50)
    {
      DrawMouseStatus();
      lastTick = tick;
    }
  }
}
