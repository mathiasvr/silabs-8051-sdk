//-----------------------------------------------------------------------------
// WDT_Lib_main.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program configures the Watchdog timer for a time-out interval of
// approximately two seconds. In the main loop, the WDT is constantly being
// reset. Pressing and holding either of the buttons on the STK (PB0 or PB1)
// will keep the program from resetting the WDT, which will cause it to
// time-out and reset the device.
//
// In this example, Timer 2 is used to generate an interrupt five times a
// second which, in turn, blinks either LED0 (if the last reset was not from
// the WDT), or LED1 (if the WDT caused the last reset).
//
// Resources:
//   SYSCLK  - 24.5 MHz HFOSC / 8
//   Timer 2 - 5 Hz Interrupt
//   P0.2    - push button 0
//   P0.3    - push button 1
//   P1.5    - LED0
//   P1.6    - LED1
//   P2.0    - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB51 PK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB51 PK board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB51 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) LED0 should blink at approximately 5 Hz.
// 6) Hold PB0 or PB1 to pause updating the WDT.
// 7) Once the WDT times-out, the system will reset and the LED1
//    will begin blinking.
//
// Target:         EFM8BB51
// Tool chain:     Generic
//
// Release 0.1 (JB)
//    - Initial Revision
//    - 30 MAR 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB51_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "bsp.h"
#include "wdt_0.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define LFOCLK_HZ 10000UL         // LFOSC default frequency, in Hz
#define WDT_TICKS (LFOCLK_HZ*1)   // ~1 second worth of WDT ticks

// Default system clock for BB51 is 24.5 MHz HFOSC divided by 8
#define SYSTEM_CLOCK_FREQUENCY  (24500000 / 8)

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bool WdtReset = false;      // Keeps track of whether the last reset was WDT

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
  // Disable WDT before doing anything else
  WDT0_stop();

  // Check to see if the WDT was the last reset cause
  if (RSTSRC & RSTSRC_WDTRSF__SET)
    WdtReset = true;

  // Call hardware initialization routine
  enter_DefaultMode_from_RESET();

  // Do not drive display
  DISP_EN = DISP_BC_DRIVEN;

  // Pause here if WDT caused the last reset
  while(WdtReset);

  // WDT is stopped so can change the interval
  WDT0_init(WDT0_ticksToInterval(WDT_TICKS), WDT0_LFOSC, WDT0_IDLE_SUSPEND);

  // Start WDT
  WDT0_start();

  while (1)
  {
    // Reset the WDT
    WDT0_feed();

    // Pause here while any buttons are pressed. This will time-out the WDT.
    while(BSP_PB0 == BSP_PB_PRESSED || BSP_PB1 == BSP_PB_PRESSED);
  }
}

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN0::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN0::TF2L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
  TMR2CN0_TF2H = 0;

  if (WdtReset)
  {
    // Toggle LED1 if the last reset was WDT
    BSP_LED1 ^= 1;
  }
  else
  {
    // Otherwise, toggle LED0.
    BSP_LED0 ^= 1;
  }

}

