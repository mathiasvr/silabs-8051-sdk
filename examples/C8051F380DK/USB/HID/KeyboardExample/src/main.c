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
// be indicated by the LED's labelled Pw.3 and P2.2 on the C8051F380 Target Board.
//
// Resources:
// SYSCLK - 48 MHz INTOSC / 1
// USB0   - Full speed
// P2.0 - push button
// P2.1 - push button
// P2.2 - LED
// P2.3 - LED
//
//-----------------------------------------------------------------------------
// How To Test: C8051F380-TB
//-----------------------------------------------------------------------------
// 1) Plug the USB Debug Adapter into the port labelled DEBUG.
// 2) Power the C8051F380-TB at P1.
// 3) Attach a USB cable between the target PC and P3 of the C8051F380-TB.
// 4) Compile and download code to the C8051F380 Target Board.
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
// Target:         C8051F38x
// Tool chain:     Generic
//
// Release 0.1 (JM)
//    - Initial Revision
//    - 13 APR 2015
//
#include "SI_C8051F380_Register_Enums.h"
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
  }
}

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// PCA0 ISR Content goes here. Remember to clear flag bits:
// PCA0CN0::CCF0 (PCA Module 0 Capture/Compare Flag)
// PCA0CN0::CCF1 (PCA Module 1 Capture/Compare Flag)
// PCA0CN0::CCF2 (PCA Module 2 Capture/Compare Flag)
// PCA0CN0::CCF3 (PCA Module 3 Capture/Compare Flag)
// PCA0CN0::CCF4 (PCA Module 4 Capture/Compare Flag)
// PCA0CN0::CF (PCA Counter/Timer Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
  if (PCA0CN_CCF0)
  {
    PCA0CN_CCF0 = 0;
    keyPushed = 1;
  }
  if (PCA0CN_CCF1)
  {
    PCA0CN_CCF1 = 0;
    keyPushed = 1;
  }
}
