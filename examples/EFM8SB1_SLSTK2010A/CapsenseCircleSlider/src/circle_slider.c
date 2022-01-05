/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// circle_slider.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "circle_slider.h"
#include "cslib_hwconfig.h"
#include "cslib.h"
#include "disp.h"
#include "render.h"
#include "circle.h"
#include "tick.h"

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

static void SetLedIntensity(uint8_t intensity);
static void NormalizeAngle(uint16_t* angle);
static void PrintBall(uint16_t angle);
static bool IsTouchQualified();
static void UpdateLed(uint16_t angle);
static uint16_t CalculatePosition();
static void SynchFrame();

/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////

// Line buffer used to render and display to LCD
SI_SEGMENT_VARIABLE(line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

// x-coordinate lookup table to map angle to x position (between 14 and 114).
// Only store coordinates for every 6 degrees to save space.
//
// Formula: 50 x cos(360 - angle + 90) + 64,
// where angle is in degrees
const SI_SEGMENT_VARIABLE(X_COORDS[], uint8_t, SI_SEG_CODE) = {
    64,
    69,
    74,
    79,
    84,
    89,
    93,
    97,
    101,
    104,
    107,
    110,
    112,
    113,
    114,
    114,
    114,
    113,
    112,
    110,
    107,
    104,
    101,
    97,
    93,
    89,
    84,
    79,
    74,
    69,
    64,
    59,
    54,
    49,
    44,
    39,
    35,
    31,
    27,
    24,
    21,
    18,
    16,
    15,
    14,
    14,
    14,
    15,
    16,
    18,
    21,
    24,
    27,
    31,
    35,
    39,
    44,
    49,
    54,
    59
};

/////////////////////////////////////////////////////////////////////////////
// Static Functions
/////////////////////////////////////////////////////////////////////////////

void SetLedIntensity(uint8_t intensity)
{
    if (intensity == 0)
    {
        PCA0CPM0 &= ~PCA0CPM0_ECOM__ENABLED;
    }
    else
    {
        PCA0CPH0 = 255-intensity;
    }
}

// Given an angle, normalize the angle
// to between 0 and 360 degrees [0, 360)
void NormalizeAngle(uint16_t* angle)
{
    while (*angle >= 360)
    {
        *angle -= 360;
    }
}

// Given an angle in degrees between 0 and 360,
// display a ball at the angle on the LCD.
// (0 degrees is 12 o'clock, 90 degrees is 3 o'clock)
void PrintBall(uint16_t angle)
{
    static uint8_t lastY = 0;
    uint8_t i;
    uint8_t x, y;
    uint8_t lineNum;

    // Use lookup table to map angle to x position
    x = X_COORDS[angle / 6];

    // Use lookup table for x coords to y coords as well.
    // sin is the same as cos, shifted 90 degrees
    angle += 90;
    NormalizeAngle(&angle);

    angle = 360 - angle;
    NormalizeAngle(&angle);

    y = X_COORDS[angle / 6];

    // Erase dirty lines from last circle position
    for (i = 0; i < circle_height; i++)
    {
        lineNum = i + lastY - (circle_height / 2);

        // Only erase the lines that won't be drawn for the new circle position
        if (!LINE_OVERLAPS_OBJ(lineNum, y - (circle_height / 2), circle_height))
        {
            DISP_ClearLine(lineNum, COLOR_BLACK);
        }
    }

    // Draw the new circle position
    for (i = 0; i < circle_height; i++)
    {
        RENDER_ClrLine(line);
        RENDER_SpriteLine(line, x - (circle_width / 2), i, circle_bits, 16);
        DISP_WriteLine(i + y - (circle_height / 2), line);
    }

    lastY = y;
}

// Update the LED brightness based on angle
// angle [0, 359] => intensity [0, 255]
void UpdateLed(uint16_t angle)
{
  static lastIntensity = 0;

  // 256 / 360 reduces to 32 / 45
  uint8_t intensity = angle * 32 / 45;

  if (intensity != lastIntensity)
  {
    SetLedIntensity(intensity);
    lastIntensity = intensity;
  }
}

bool IsTouchQualified()
{
  uint8_t sensor_index;
  uint16_t touch_sum = 0;

  // Only update the slider if at least one of the CS0 channels is active
  if (CSLIB_isSensorDebounceActive(0) ||
      CSLIB_isSensorDebounceActive(1) ||
      CSLIB_isSensorDebounceActive(2))
  {
    // For each sensor, calculate the delta between the current
    // capacitance value and baseline capacitance value, or delta
    for (sensor_index = 0; sensor_index < DEF_NUM_SENSORS; sensor_index++)
    {
        if (CSLIB_getNoiseAdjustedSensorData(sensor_index) >= CSLIB_node[sensor_index].currentBaseline)
        {
            touch_sum += ((CSLIB_getNoiseAdjustedSensorData(sensor_index) - CSLIB_node[sensor_index].currentBaseline) >> 8);
        }
    }

    // Sum of touch deltas must reach a minimum threshold
    // before updating circle slider position.
    //
    // This helps reduce jumpiness when the touch
    // is being released due to non-linear capacitance
    // response
    if (touch_sum > MIN_SUM_TOUCH)
    {
      return true;
    }
  }

  return false;
}

// Perform the centroid algorithm to determine the wheel angle position
// (0 degrees is 12 o'clock, 90 degrees is 3 o'clock)
//
// Sensor orientation:
//
// CS1 CS2
//   CS0
//
uint16_t CalculatePosition()
{
    uint8_t sensor_index;
    uint16_t angle = 0;
    uint16_t deltas[3];
    uint16_t weights[3];
    uint16_t offset;

    // For each sensor, calculate the delta between the current
    // capacitance value and baseline capacitance value, or delta
    for (sensor_index = 0; sensor_index < DEF_NUM_SENSORS; sensor_index++)
    {
        if (CSLIB_getNoiseAdjustedSensorData(sensor_index) < CSLIB_node[sensor_index].currentBaseline)
        {
            deltas[sensor_index] = 0;
        }
        else
        {
            deltas[sensor_index] = (CSLIB_getNoiseAdjustedSensorData(sensor_index) - CSLIB_node[sensor_index].currentBaseline) >> 8;
        }
    }

    // CS0 touch delta is highest, perform weighted linear slider
    // binning algorithm to assign a value between 20 - 60
    if (deltas[0] >= deltas[2] &&
        deltas[0] >= deltas[1])
    {
        // Binned weights by sensor position:
        // CS1:60 CS2:20
        //    CS0:40
        weights[0] = 40;
        weights[1] = 60;
        weights[2] = 20;

        // An average weighted value of 20 equals 60 degrees
        offset = 60;
    }
    // CS2 touch delta is highest, perform weighted linear slider
    // binning algorithm to assign a value between 20 - 60
    else if (deltas[2] >= deltas[0] &&
             deltas[2] >= deltas[1])
    {
        // Binned weights by sensor position:
        // CS1:20 CS2:40
        //    CS0:60
        weights[0] = 60;
        weights[1] = 20;
        weights[2] = 40;

        // An average weighted value of 20 equals -60 degrees, or 300 degrees
        offset = 300;
    }
    // CS1 touch delta is highest, perform weighted linear slider
    // binning algorithm to assign a value between 20 - 60
    else
    {
        // Binned weights by sensor position:
        // CS1:40 CS2:60
        //   CS0:20
        weights[0] = 20;
        weights[1] = 40;
        weights[2] = 60;

        // An average weighted value of 20 equals 180 degrees
        offset = 180;
    }

    // Calculate the angle using binned weights and offset from above

    // Apply calculated binned weights to get a value between [20, 60]
    angle = (deltas[0] * weights[0] + deltas[1] * weights[1] + deltas[2] * weights[2]) / (deltas[0] + deltas[1] + deltas[2]);

    // Convert binned value from [20, 60] to [0, 240]
    angle = (6 * angle) - 120;

    // Apply offset
    angle += offset;

    // Normalize angle between [0, 360)
    NormalizeAngle(&angle);

    return angle;
}

// Delay until start of next frame
void SynchFrame()
{
    static uint16_t lastTick = 0;
    uint16_t tick;

    // Render at 50 Hz
    while (((tick = GetTickCount()) - lastTick) < HZ_TO_MS(MAIN_FRAME_RATE));
    lastTick = tick;
}

/////////////////////////////////////////////////////////////////////////////
// Global Functions
/////////////////////////////////////////////////////////////////////////////

void circle_slider_init()
{
  // Initialize LCD
  DISP_Init();
}

void circle_slider_main()
{
  uint16_t angle;

  SynchFrame();                   // Synch to start of frame for 50 Hz refresh rate

  // Check if sum of sensors is above a minimum threshold
  // for better touch release behavior
  if (IsTouchQualified())
  {
    // Perform the centroid algorithm to determine the wheel angle position
    // (0 degrees is 12 o'clock, 90 degrees is 3 o'clock)
    angle = CalculatePosition();

    // Display the ball at the angle specified
    PrintBall(angle);

    // Update the LED brightness based on angle
    UpdateLed(angle);
  }
  // Otherwise, clear the display
  else
  {
    DISP_ClearAll();
  }
}
