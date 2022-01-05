//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates the RGB LED using the joystick to control the color
// output.
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB2 STK
//-----------------------------------------------------------------------------
// 1) Place the SW104 switch in "AEM" mode.
// 2) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The Rainbow Blinky demo should start.
// 6) Use the Joystick to change the color of the RGB LED:
//    Up         - Red
//    Up/Right   - Orange
//    Right      - Yellow
//    Down/Right - Green
//    Down       - Blue
//    Down/Left  - Indigo
//    Left       - Violet
//    Up/Left    - White
// 7) The selected color will be displayed on the LCD.
//
// Target:         EFM8UB2
// Tool chain:     Generic
//
// Release 0.1 (CM)
//    - Initial Revision
//    - 10 OCT 2014
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 24 MHz HFOSC / 2
// ADC0   - 10-bit
// SPI0   - 1 MHz
// Timer0 - 2 kHz (ADC start of conversion)
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.1 - CS (Active High)
// P0.2 - push button
// P0.3 - push button
// P0.6 - SCK
// P1.0 - MOSI
// P1.4 - Display enable
// P1.6 - LED Green
// P1.7 - LED Blue
// P2.0 - LED Red
// P2.5 - Joystick (analog voltage divider)
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "bsp.h"
#include "InitDevice.h"
#include "disp.h"
#include "tick.h"
#include "rainbowblinky.h"

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
int main(void)
{
  uint16_t lastTick;

  // Enter default mode
  enter_DefaultMode_from_RESET();

  // Enable all interrupts
  IE_EA = 1;

  DISP_Init();
  RainbowBlinky_Init();

  // Update when system tick is updated
  // Otherwise, idle to save power
  lastTick = GetTickCount();
  while (1)
  {
    // Idle until an interrupt wakes us up
    PCON0 = PCON0_IDLE__IDLE | PCON0_STOP__NORMAL;

    // Check to see if a millisecond has elapsed
    // Wakeup source might not be timer
    if ((GetTickCount() - lastTick) >= 10)
    {
      // Update ticks counter
      lastTick = GetTickCount();

      // Update display and parse input
      RainbowBlinky_Update();

      // Update LED Waveform
      RainbowBlinky_UpdateLED();
    }
  }
}
