//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2016 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates the available power modes on the EFM8UB2 STK using
// the LCD and push buttons.
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
// 5) The power modes example should start.
// 6) Press PB1 to cycle through the 5 different power modes shown on
//    the LCD.
// 7) Press PB0 to enter the selected power mode. The LED turns off.
//    For all modes except Shutdown, the display will show "*Active*" while
//    in the mode. For Shutdown mode the display is turned off.
// 8) Press PB0 (modes Normal and Idle) to return to the menu,
//    or press Reset (all modes) to restart the example. The LED turns on.
//
// Target:         EFM8UB2
// Tool chain:     Generic
//
// Release 0.1 (MD;AT)
//    - Initial Revision
//    - 1 APR 2016
//
//-----------------------------------------------------------------------------
// Resources:
//-----------------------------------------------------------------------------
// SYSCLK - 48 MHz HFOSC0
// SPI0   - 1 MHz
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.1 - Display CS (Active High)
// P0.6 - SCK
// P1.0 - MOSI
// P1.6 - LED G
// P0.2 - Push Button 0
// P0.3 - Push Button 1
// P2.5 - Joystick (not used)
// P1.4 - Display enable
//
//-----------------------------------------------------------------------------

#include "bsp.h"
#include "pwr.h"

#include "draw.h"
#include "tick.h"
#include "InitDevice.h"


//-----------------------------------------------------------------------------
// Local Definitions
//-----------------------------------------------------------------------------

// Power mode enumeration
#define NORMAL_MODE 0
#define IDLE_MODE 1
#define STOP_MODE 2
#define SUSPEND_MODE 3
#define SHUTDOWN_MODE 4
#define LAST_POWER_MODE SHUTDOWN_MODE

SI_VARIABLE_SEGMENT_POINTER(powerModeText[], char, SI_SEG_CODE) =
{
  "0. Normal Mode",
  "1. Idle Mode",
  "2. Stop Mode",
  "3. Suspend Mode",
  "4. Shutdown Mode",
};

// Power mode status
#define STATUS_MODE_ACTIVE 0
#define STATUS_MODE_WAKEUP 1

SI_VARIABLE_SEGMENT_POINTER(powerStatusText[], char, SI_SEG_CODE) =
{
  "*Active*",
  "*Wakeup*",
};

// Function that demonstrates the available power modes
void APP_enterPowerMode(uint8_t mode);


//-----------------------------------------------------------------------------
// User Interface Functions
//-----------------------------------------------------------------------------

// Push button assignments
#define UI_BTN_MENU   (1 << 0)
#define UI_BTN_SELECT (1 << 1)

// Read and return push button status
uint8_t UI_getButtons(void)
{
  uint8_t status = 0;
  if (BSP_PB1 == BSP_PB_PRESSED) {
    status = UI_BTN_MENU;
  } else if (BSP_PB0 == BSP_PB_PRESSED) {
    status = UI_BTN_SELECT;
  }
  return status;
}

// Manage the UI menu and return the user selection
uint8_t UI_getPowerMode(uint8_t mode)
{
  // Initialize the display and draw the UI
  DISP_Init();
  DrawSplashScreen();
  DrawScreenText(powerModeText[mode], TEXT_ROW0_Y);

  // Menu selection loop
  while (true)
  {
    // Wait for button press leading edge
    while (UI_getButtons() != 0);
    while (UI_getButtons() == 0);

    // Cycle menu through available modes
    if (UI_getButtons() & UI_BTN_MENU)
    {
      mode++;
      if (mode > LAST_POWER_MODE)
      {
        mode = NORMAL_MODE;
      }
      DrawScreenText(powerModeText[mode], TEXT_ROW0_Y);
    }
    // Selection made, break out of menu loop
    else if (UI_getButtons() & UI_BTN_SELECT)
    {
      break;
    }
  }

  // Wait for all buttons to be released
  while (UI_getButtons() != 0);

  // Display mode status, turn off the LED and return the selection
  DrawScreenText(powerStatusText[STATUS_MODE_ACTIVE], TEXT_ROW1_Y);
  BSP_LED_G = BSP_LED_OFF;
  return mode;
}

// Disables the LCD display
void UI_disableDisplay(void)
{
  // Clear the display screen
  DISP_ClearAll();
  // Wait for BSP to complete all display updates
  while (BSP_DISP_CS == SPI_CS_ASSERT_LVL);
  // Disable the display
  BSP_DISP_EN = DISP_EN_BC;
}

// INT0 ISR - Triggered on leading edge of UI_BTN_SELECT.
SI_INTERRUPT(INT0_ISR, INT0_IRQn)
{
  // Turn on LED whenever 5V is applied or released
  BSP_LED_G = BSP_LED_ON;
}


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
// Example main()
//-----------------------------------------------------------------------------
void main(void)
{
  uint8_t modeSelected = NORMAL_MODE;

  // Initialize the device
  enter_DefaultMode_from_RESET();

  // Safety trap. Press either push button while resetting the device to
  // stall the application here.
  while (UI_getButtons());

  // Start with the LED on
  BSP_LED_G = BSP_LED_ON;

  // Enable all interrupts
  IE_EA = 1;

  // Mainloop
  while (true)
  {
    // Get the user's selection
    modeSelected = UI_getPowerMode(modeSelected);

    // Turn off the display for Shutdown Mode
    if (modeSelected == SHUTDOWN_MODE)
    {
      UI_disableDisplay();
    }

    // Halt the display tick and wait for display updates to finish
    TMR3CN0 &= ~TMR3CN0_TR3__BMASK;
    while (BSP_DISP_CS == SPI_CS_ASSERT_LVL);

    // Change all input pins to analog to disable internal pullups
    P0MDIN = 0x46;
    P1MDIN = 0x51;
    P2MDIN = 0x00;
    P3MDIN = 0x00;
    P4MDIN = 0x00;

    // Enter the selected power mode
    APP_enterPowerMode(modeSelected);

    // Restore all pins to digital mode for normal operation
    P4MDIN = 0xFF;
    P3MDIN = 0xFF;
    P2MDIN = 0xFF;
    P1MDIN = 0xFF;
    P0MDIN = 0xFF;

    // Restart the display update tick
    TMR3CN0 |= TMR3CN0_TR3__BMASK;
  }
}


//-----------------------------------------------------------------------------
// Demonstrate the available power modes
//-----------------------------------------------------------------------------
void APP_enterPowerMode(uint8_t mode)
{
  // Start the selected power mode
  switch (mode)
  {
    case NORMAL_MODE:
      // Wait for button press on PB0
      while (BSP_PB0 != BSP_PB_PRESSED);
      break;

    case IDLE_MODE:
      // Interrupts must be enabled to return from Idle mode
      PWR_enterIdle();
      break;

    case STOP_MODE:
      // Disable Supply Monitor and set Regulators to low power mode
      // to see datasheet current (Mon adds ~15 uA, Reg adds ~100 uA)
      PWR_enterStop();
      break;

    case SUSPEND_MODE:
      // Disable Supply Monitor and set both Regulators to low power mode
      // to see datasheet current (Mon adds ~15 uA, Reg adds ~100 uA)
      PWR_enterSuspend();
      break;

    case SHUTDOWN_MODE:
      // Disable Supply Monitor and 5V Regulator and set internal
      // 1.8V Regulator to low power mode to see datasheet current
      // (Mon adds ~15 uA, Reg adds ~150 uA)
      PWR_enterShutdown();
      break;
  }
}
