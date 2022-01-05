//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates two different capsense power modes: responsive vs
// low power.
//
// In responsive mode, the MCU wakes up every 20 ms to sample the capacitive
// buttons and updates the status of the three sensors on the LCD.
//
// In low power mode, the MCU wakes up every 500 ms to sample the capacitive
// buttons. After a touch is detected, the MCU wakes up every 20 ms to scan
// sensors until no touch is detected for 100 counts.
//
//-----------------------------------------------------------------------------
// How To Test: EFM8SB1 STK
//-----------------------------------------------------------------------------
// 1) Place the SW104 switch in "AEM" mode.
// 2) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The LCD shows the current mode: responsive or low power
// 6) Touch any of the three capacitive sensors and note that the
//    LCD displays "on" for any of the sensors that are activated.
//    Since the three sensors are designed to overlap each other to form
//    a circle slider, most touches will activate 2 or 3 of the sensors
//    at a time.
// 7) To switch modes, press and hold the corresponding push button while
//    activating one of the capacitive sensors to wake the MCU.
//    PB1 - Responsive Mode
//    PB0 - Low-Power Mode
//
// Target:         EFM8SB1
// Tool chain:     Generic
//
// Release 0.1 (CM)
//    - Initial Revision
//    - 10 OCT 2014
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 20 MHz LPOSC / 1
// SPI0   - 1 MHz
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.1 - CS (Active High)
// P0.2 - CS0
// P0.3 - CS1
// P0.6 - SCK
// P1.0 - MOSI
// P1.1 - LED
// P1.5 - CS2
// P2.7 - Display enable
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "capsense_power_modes.h"
#include "InitDevice.h"
// $[Generated Includes]
#include "cslib.h"
#include "cslib_config.h"
// [Generated Includes]$

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main(void)
{
  // Call hardware initialization routine
  enter_DefaultMode_from_RESET();

  // LED is active high, turn it off by default
  BSP_LED0 = BSP_LED_OFF;

  capsense_power_modes_init();

  while (1)
  {
// $[Generated Run-time code]
    // -----------------------------------------------------------------------------
    // If low power features are enabled, this will either put the device into a low
    // power state until it is time to take another scan, or put the device into a
    // low-power sleep mode if no touches are active
    // -----------------------------------------------------------------------------
    CSLIB_lowPowerUpdate();

    // -----------------------------------------------------------------------------
    // Performs all scanning and data structure updates
    // -----------------------------------------------------------------------------
    CSLIB_update();

// [Generated Run-time code]$
    capsense_power_modes_main();
  }
}
