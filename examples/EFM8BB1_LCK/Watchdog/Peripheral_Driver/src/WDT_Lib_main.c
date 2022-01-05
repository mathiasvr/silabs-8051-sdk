//-----------------------------------------------------------------------------
// WDT_Lib_main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program configures the Watchdog timer for a time-out interval of
// approximately one second. In the main loop, the WDT is constantly being
// reset. Pressing and holding the button on the LCK
// will keep the program from resetting the WDT, which will cause it to
// time-out and reset the device.
//
// In this example, Timer 2 is used to generate an interrupt five times a
// second which, in turn, blinks either the LED if the last reset
// was not from the WDT, otherwise the LED is on.
//
// Resources:
//   SYSCLK  - 24.5 MHz HFOSC / 8
//   Timer 2 - 5 Hz Interrupt
//   P1.4    - LED
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB1 LCK
//-----------------------------------------------------------------------------
// 1) Connect the EFM8BB1 LCK board to a PC using a micro USB cable.
// 2) Compile and download code to the EFM8BB1 LCK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 3) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 4) The LED should blink at approximately 5 Hz.
// 5) Hold BTN0 to pause updating the WDT.
// 7) Once the WDT times-out, the system will reset and the LED
//    will be turned on constantly.
//
// Target:         EFM8BB1
// Tool chain:     Generic
//
// Release 0.1 (BL)
//    - Initial Revision
//    - 9 FEB 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB1_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "bsp.h"
#include "wdt_0.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define LFOCLK_HZ 10000UL         // LFOSC default frequency, in Hz
#define WDT_TICKS (LFOCLK_HZ*1)   // ~1 second worth of WDT ticks

// Default system clock for UB3 is 24.5 MHz HFOSC divided by 8
#define SYSTEM_CLOCK_FREQUENCY  (24500000 / 8)

// WDT delay cycles as defined by Erratum WDT_E102 is 2 clock cycles
#define WDT_DELAY_CYCLES        2

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bool WdtReset = false;      // Keeps track of whether the last reset was WDT

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
    // Call hardware initialization routine
    enter_DefaultMode_from_RESET();

    // Check to see if the WDT was the last reset cause
    if (RSTSRC & RSTSRC_WDTRSF__SET)
    {
      WdtReset = true;
      while(1);
    }

    // Feed WDT timer before disabling (Erratum WDT_E102)
    WDT0_feed();
    // Add 2 LFO cycle delay before disabling WDT (Erratum WDT_E102)
    WDT0_delay(SYSTEM_CLOCK_FREQUENCY, LFOCLK_HZ, WDT_DELAY_CYCLES);
    // Disable WDT before changing interval
    WDT0_stop();
    // Delay 2 LFO cycles to ensure WDT is completely disabled
    WDT0_delay(SYSTEM_CLOCK_FREQUENCY, LFOCLK_HZ, WDT_DELAY_CYCLES);
    // Change WDT interval
    WDT0_init(WDT0_ticksToInterval(WDT_TICKS), WDT0_LFOSC, WDT0_IDLE_SUSPEND);
    // Start WDT
    WDT0_start();

    while (1) 
   {
      // Reset the Watchdog Timer
      WDT0_feed();
      // Delay between WDT feeds to work around Erratum WDT_E101
      WDT0_delay(SYSTEM_CLOCK_FREQUENCY, LFOCLK_HZ, WDT_DELAY_CYCLES);
      
      // Pause here while any buttons are pressed. This will time-out the WDT.
      while(BSP_BTN0 == BSP_BTN_PRESSED);
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
    // Leave the LED on if the last reset was WDT
    BSP_LED0 = BSP_LED_ON;
  }
  else
  {
    // Otherwise, toggle the LED.
    BSP_LED0 ^= 1;
  }

}

