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
// reset. Pressing and holding either of the buttons on the STK (PB0 or PB1)
// will keep the program from resetting the WDT, which will cause it to
// time-out and reset the device.
//
// In this example, whenever the last reset source was from the WDT,
// Timer 2 is enabled, which generates an interrupt five times a second to
// toggle LED0.
//
// Resources:
//   SYSCLK  - 20 MHz LFOSC / 64
//   Timer 2 - 5 Hz Interrupt
//   P2.7    - Display enable
//   P1.1    - LED0
//
//-----------------------------------------------------------------------------
// How To Test: EFM8SB1 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The Green LED should blink at approximately 5 Hz.
// 6) Hold PB0 or PB1 to pause updating the WDT.
// 7) Once the WDT times-out, the system will reset and the LED0 will begin
//    blinking.
//
// Target:         EFM8SB1
// Tool chain:     Generic
//
// Release 0.1 (BL)
//    - Initial Revision
//    - 9 FEB 2015
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8SB1_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "bsp.h"
#include "wdt_0.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define PCACLK_HZ (20000000UL/8/8/12)  // PCA counter input frequency, in Hz
#define WDT_TICKS (PCACLK_HZ*1)        // ~1 second worth of WDT ticks

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	// Call hardware initialization routine
	enter_DefaultMode_from_RESET();

	BSP_DISP_EN = 0;                          // Display not driven by EFM8

	// Turn on the LED
	BSP_LED0 = BSP_LED_ON;

	// Check to see if the WDT was the last reset cause
	if (RSTSRC & RSTSRC_WDTRSF__SET)
	{
	  // Turn on Timer 2, which will toggle the LED.
	  TMR2CN0_TR2 = 1;
	  while(1);
	}

	WDT0_init(WDT0_ticksToInterval(WDT_TICKS), WDT0_SYSCLK_DIV12, WDT0_IDLE_SUSPEND);

	WDT0_start();

	while (1) 
   {
	  // Reset the Watchdog Timer
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

  // Toggle the LED
  BSP_LED0 ^= 1;
}

