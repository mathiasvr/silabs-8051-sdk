/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// capsense_power_modes.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "capsense_power_modes.h"
#include "cslib_hwconfig.h"
#include "cslib_config.h"
#include "cslib.h"
#include "disp.h"
#include "render.h"
#include "tick.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

static void DrawText(
    uint8_t row,
    SI_VARIABLE_SEGMENT_POINTER(str, char, RENDER_STR_SEG),
    bool center);
static void UpdateModeText();
static void SwitchToResponsiveMode();
static void SwitchToLowPowerMode();
static void UpdateSensorStatus();

/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////

// Line buffer used to render and display to LCD
SI_SEGMENT_VARIABLE(line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

SI_SEGMENT_VARIABLE(RESPONSIVE_MODE_TEXT[], char, RENDER_STR_SEG) = "Responsive Mode";
SI_SEGMENT_VARIABLE(LOW_POWER_MODE_TEXT[], char, RENDER_STR_SEG) = "Low-Power Mode";

uint8_t PowerMode = PM_RESPONSIVE;

/////////////////////////////////////////////////////////////////////////////
// Static Functions
/////////////////////////////////////////////////////////////////////////////

static void DrawText(
    uint8_t row,
    SI_VARIABLE_SEGMENT_POINTER(str, char, RENDER_STR_SEG),
    bool center)
{
  uint8_t i;
  uint8_t posx = 0;

  if (center)
  {
    posx = (DISP_WIDTH - RENDER_GetStrSize(str)) / 2;
  }

  for (i = 0; i < FONT_HEIGHT; i++)
  {
    RENDER_StrLine(line, posx, i, str);
    DISP_WriteLine(i + (row * FONT_HEIGHT), line);
  }
}

// Display "Responsive Mode" or "Low-Power Mode" on the first
// line of the LCD
static void UpdateModeText()
{
  if (PowerMode == PM_RESPONSIVE)
  {
    DrawText(0, RESPONSIVE_MODE_TEXT, true);
  }
  else
  {
    DrawText(0, LOW_POWER_MODE_TEXT, true);
  }
}

// Switch to responsive mode capsense parameters
static void SwitchToResponsiveMode()
{
  PowerMode = PM_RESPONSIVE;

  UpdateModeText();

  CSLIB_sleepModePeriod = RESPONSIVE_SLEEP_PERIOD;
  CSLIB_activeModePeriod = RESPONSIVE_ACTIVE_PERIOD;
  CSLIB_countsBeforeSleep = RESPONSIVE_SLEEP_COUNTS;
}

// Switch to low-power mode capsense parameters
static void SwitchToLowPowerMode()
{
  PowerMode = PM_LOW_POWER;

  UpdateModeText();

  CSLIB_sleepModePeriod = LOW_POWER_SLEEP_PERIOD;
  CSLIB_activeModePeriod = LOW_POWER_ACTIVE_PERIOD;
  CSLIB_countsBeforeSleep = LOW_POWER_SLEEP_COUNTS;
}

// Display the status of the three sensors
// on the second line of the LCD
//
// Sensor orientation:
// CS1 CS2
//   CS0
static void UpdateSensorStatus()
{
  static uint8_t lastActiveSensors = 0xFF;
  uint8_t activeSensors = 0;
  uint8_t i;

  // Get the sensor status and convert to
  // a bitmask
  for (i = 0; i < DEF_NUM_SENSORS; i++)
  {
    if (CSLIB_isSensorDebounceActive(i))
    {
      activeSensors |= (1 << i);
    }
  }

  // Only update the LCD if the sensor
  // status has changed
  if (activeSensors != lastActiveSensors)
  {
    SI_SEGMENT_VARIABLE(sensor_text[8], char, RENDER_STR_SEG);

    for (i = 0; i < DEF_NUM_SENSORS; i++)
    {
      sensor_text[0] = i + '0';
      sensor_text[1] = ':';
      sensor_text[2] = ' ';

      if (activeSensors & (1 << i))
      {
        strcpy(sensor_text + 3, "On ");
      }
      else
      {
        strcpy(sensor_text + 3, "Off");
      }

      sensor_text[6] = 0;
      sensor_text[7] = 0;

      DrawText(i + 2, sensor_text, true);
    }

    lastActiveSensors = activeSensors;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Global Functions
/////////////////////////////////////////////////////////////////////////////

void capsense_power_modes_init()
{
  // Initialize LCD
  DISP_Init();

  SwitchToResponsiveMode();
  UpdateSensorStatus();
}

void capsense_power_modes_main()
{
  // Drive PB debounce capacitors high
  // (capsense low-power library code clears the port latch,
  // which grounds the debounce caps)
  P1MDOUT |= P1MDOUT_B2__BMASK | P1MDOUT_B3__BMASK;
  BSP_PB1 = 1;
  BSP_PB0 = 1;

  // Set PB pins back to open-drain
  P1MDOUT &= ~(P1MDOUT_B2__BMASK | P1MDOUT_B3__BMASK);

  // Switch to responsive mode
  if (PowerMode != PM_RESPONSIVE && BSP_PB1 == BSP_PB_PRESSED)
  {
    SwitchToResponsiveMode();
  }
  // Switch to low power mode
  else if (PowerMode != PM_LOW_POWER && BSP_PB0 == BSP_PB_PRESSED)
  {
    SwitchToLowPowerMode();
  }

  UpdateSensorStatus();
}
