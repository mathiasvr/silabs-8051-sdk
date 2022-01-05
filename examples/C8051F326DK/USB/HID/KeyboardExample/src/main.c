//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program enumerates as a USB keyboard. Each time a button is pressed
// a character is sent to the host. A series of presses will spell out
// "HID Keyboard ". The status of the Caps Lock and Num Lock on the host will
// be indicated by the LED's labelled Pw.3 and P2.2 on the C8051F326 Target Board.
//
// Resources:
// SYSCLK - 12 MHz INTOSC * 2
// USB0   - Full speed
// P2.0 - push button
// P2.1 - push button
// P2.2 - LED
// P2.3 - LED
//
//-----------------------------------------------------------------------------
// How To Test: C8051F326-TB
//-----------------------------------------------------------------------------
// 1) Plug the USB Debug Adapter into the port labelled DEBUG.
// 2) Power the C8051F326-TB at P1.
// 3) Attach a USB cable between the target PC and the USB port of the
//    C8051F326-TB.
// 4) Compile and download code to the C8051F326 Target Board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) The HID keyboard example should start.
// 7) The device should enumerate on the PC as a HID keyboard.
// 8) Press either push-button (P2.0 or P2.1) to send one character in the
//    string "HID Keyboard ".
// 9) Pressing Num Lock or Caps Lock on the host keyboard will toggle LED's
//    P2.3 and P2.2, respectively.
//
// Target:         C8051F326
// Tool chain:     Generic
//
// Release 0.1 (JM)
//    - Initial Revision
//    - 13 APR 2015
//
#include "SI_C8051F326_Register_Enums.h"
#include "efm8_usb.h"
#include "descriptors.h"
#include "idle.h"

// ----------------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------------
void Init_Device(void);

// ----------------------------------------------------------------------------
// Variables
// ----------------------------------------------------------------------------
uint8_t keySeqNo = 0;        // Current position in report table.
bool keyPushed = 0;          // Current pushbutton status.

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
 
// ----------------------------------------------------------------------------
// main() Routine
// ---------------------------------------------------------------------------
//
// Note: The software watchdog timer is not disabled by default in this
// example, so a long-running program will reset periodically unless
// the timer is disabled or your program periodically writes to it.
//
// Review the "Watchdog Timer" section under the part family's datasheet
// for details.
//
// ----------------------------------------------------------------------------
int16_t main(void)
{
  Init_Device();            // Initialize peripherals, clocks, and interrupts
  USBD_Init( &initstruct);  // Initialize USB
  IE_EA = 1;                // Enable interrupts

  while (1)
  {
    // Poll the switches on P2.0 and P2.1
    if ((P2 & 0x01) == 0)
    {
      while ((P2 & 0x01) == 0)
      {
      }

      keyPushed = 1;
    }
    if ((P2 & 0x02) == 0)
    {
      while ((P2 & 0x02) == 0)
      {
      }

      keyPushed = 1;
    }
  }
}
