//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates a Data Logger demo on the EFM8SB1 STK using
// the Humidity & Temperature Sensor (Si7021), LCD, joystick, push buttons, and RGB LED.
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
// 5) PB1 - toggle between graph and log display output
//    PB0 - toggle between temperature in C, temperature in F, and relative
//          humidity
//
// Target:         EFM8SB1
// Tool chain:     Generic
//
// Release 0.1 (JL)
//    - Initial Revision
//    - 06 NOV 2014
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK  - 24.5 MHz HFOSC0 / 1
// ADC0    - 10-bit
// SPI0    - 1 MHz
// Timer 2 - 2 MHz (SPI CS delay)
// Timer 3 - 1 kHz (1 ms tick)
// P0.1    - CS (Active High)
// P0.6    - SCK
// P1.0    - MOSI
// P1.1    - LED
// P1.2    - SDA/PB0
// P1.3    - SCL/PB1
// P1.4    - Joystick (analog voltage divider)
// P2.7    - Display enable

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "InitDevice.h"
#include "main.h"
#include "disp.h"
#include "joystick.h"
#include "log.h"
#include "tick.h"
#include "deque.h"
#include <intrins.h>

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// Current item to display, whether it is displaying F, C, or H
uint8_t displayItem;

// Display mode determines between DISPLAY_MODE_GRAPH and DISPLAY_MODE_LOGGER mode
bool prevDisplayMode;
bool displayMode;

bool alarmWakeUp = true;
bool updateLCD;
bool skipPrint = false;

bool loggerMode = REAL_TIME_MODE;

// Joystick direction
uint8_t joystick;
uint8_t prev_joystick = JOYSTICK_NONE;

SI_SEGMENT_VARIABLE(hours, uint8_t, SI_SEG_IDATA) = 0;
SI_SEGMENT_VARIABLE(minutes, uint8_t, SI_SEG_IDATA) = 0;
SI_SEGMENT_VARIABLE(seconds, uint8_t, SI_SEG_IDATA) = 0;

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
void checkPBStatus(void);
void checkJoystickStatus(void);

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
  uint8_t i;

  // If slave is holding SDA low because of an improper SMBus reset or error
  while (!SDA)
  {
    // Provide clock pulses to allow the slave to advance out
    // of its current state. This will allow it to release SDA.
    XBR2 = 0x40;                     // Enable Crossbar
    SCL = 0;                         // Drive the clock low
    for (i = 0; i < 255; i++);       // Hold the clock low
    SCL = 1;                         // Release the clock
    while (!SCL);                    // Wait for open-drain clock output to rise
    for (i = 0; i < 10; i++);        // Hold the clock high
    XBR2 = 0x00;                     // Disable Crossbar
  }

  // Default mode is Celsius
  displayItem = DISPLAY_ITEM_CELSIUS;

  // Default display is graph mode
  displayMode = DISPLAY_MODE_GRAPH;
  prevDisplayMode = DISPLAY_MODE_GRAPH;

  // Enter default mode
  enter_DefaultMode_from_RESET();

  IE_EA = 1;

  // Initialize LCD
  DISP_Init();

  while (1)
  {
    if (_testbit_(alarmWakeUp))
    {
      // Take measurement data
      UpdateCurrentMeasurement();

      // Set flag to update LCD later
      updateLCD = true;
    }

    if (_testbit_(updateLCD))
    {
      if (displayMode == DISPLAY_MODE_GRAPH)
      {
        // Scale the temperature or humidity data to LCD y values.
        ScaleLocation();

        // Display graph
        displayGraph();

        // Displays the data and side axis to the LCD
        DisplayWaveform();
      }
      else // DISPLAY_MODE_LOGGER Mode
      {
        if (prevDisplayMode == DISPLAY_MODE_GRAPH)
        {
          fullDisplay = true;
          // Set as real time mode.
          loggerMode = REAL_TIME_MODE;
        }

        if (currentLogSize <= LCD_PAGE_SIZE)
        {
          // Set as real time mode.
          loggerMode = REAL_TIME_MODE;
        }
        else
        {
          // Check the current status of joystick
          checkJoystickStatus();

          if (joystick == JOYSTICK_N)
          {
            // History mode, back button pressed
            if (loggerMode == REAL_TIME_MODE)
            {
              // First time to change to history mode
              loggerMode = HISTORY_MODE;

              // Display the previous data
              endEntryNum = currentLogSize - (currentLogSize % LCD_PAGE_SIZE);
            }
            else if (endEntryNum > LCD_PAGE_SIZE)
            {
              endEntryNum -= LCD_PAGE_SIZE;
            }
            else
            {
              // It is back to the first page and no need to update LCD.
              skipPrint = true;
            }
          }
          else if ((joystick == JOYSTICK_S) && (loggerMode == HISTORY_MODE))
          {
            // History mode, forward button pressed
            if ((currentLogSize - endEntryNum) >= LCD_PAGE_SIZE)
            {
              endEntryNum += LCD_PAGE_SIZE;
            }
            else
            {
              loggerMode = REAL_TIME_MODE;
            }
          }
          else if (joystick == JOYSTICK_C)
          {
            // Cancel history mode
            loggerMode = REAL_TIME_MODE;
          }
        }

        // Check if need to update LCD display
        if (!(_testbit_(skipPrint)))
        {
          if (loggerMode == REAL_TIME_MODE)
          {
            endEntryNum = currentLogSize;
          }

          DisplayLogger(endEntryNum);
        }

        prevDisplayMode = DISPLAY_MODE_LOGGER;
      }
    }

    // Check buttons status
    checkPBStatus();

    if ((prevDisplayMode == DISPLAY_MODE_LOGGER) && (displayMode == DISPLAY_MODE_LOGGER))
    {
      // Check the current status of joystick
      checkJoystickStatus();
    }
  }
}

//---------------------------------------------------------------------------
// CheckPBStatus
//---------------------------------------------------------------------------
//
// Description - check for buttons status.
//
//
void checkPBStatus(void)
{
  if (BSP_PB0 == BSP_PB_PRESSED)
  {
    displayItem++;
    if (displayItem >= 3)
    {
      displayItem = 0;
    }

    // Set flag to update LCD later
    updateLCD = true;
  }

  if (BSP_PB1 == BSP_PB_PRESSED)
  {
    // Save and update current display mode.
    prevDisplayMode = displayMode;
    displayMode = !displayMode;

    // Set flag to update LCD later
    updateLCD = true;
  }

  //Wait for both switches to be released.
  while ((BSP_PB0 == BSP_PB_PRESSED) || (BSP_PB1 == BSP_PB_PRESSED));
}

//---------------------------------------------------------------------------
// checkJoystickStatus
//---------------------------------------------------------------------------
//
// Description - Check for the current status of joystick
//
void checkJoystickStatus(void)
{
  uint32_t mv;
  uint8_t dir;

  // Start polling joystick.
  ADC0CN0_ADBUSY = 1;

  while (!ADC0CN0_ADINT);
  ADC0CN0_ADINT = 0;

  mv = ((uint32_t) ADC0) * 3300 / 1024;

  dir = JOYSTICK_convert_mv_to_direction(mv);

  if (dir != prev_joystick)
  {
    prev_joystick = joystick;

    joystick = dir;

    // Set flag to update LCD later
    updateLCD = true;
  }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
