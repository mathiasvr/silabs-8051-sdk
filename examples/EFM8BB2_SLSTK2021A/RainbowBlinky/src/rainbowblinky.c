/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// rainbowblinky.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "InitDevice.h"
#include "rgb_led.h"
#include "joystick.h"
#include "disp.h"
#include "render.h"
#include "tick.h"
#include "utils.h"
#include "rainbowblinky.h"
#include "adc.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
// Typedefs
/////////////////////////////////////////////////////////////////////////////

typedef SI_VARIABLE_SEGMENT_POINTER(, char, const SI_SEG_CODE) codeStr_t;

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////

uint8_t colorIndex = 0;
Color ledColor = {0, 0, 0};
uint8_t ledIntensityCounter = 0;

const SI_SEGMENT_VARIABLE(colors[8], Color, SI_SEG_CODE) =
{
 {0xff, 0x00, 0x00}, // Red
 {0xff, 0xff, 0x00}, // Yellow
 {0x00, 0x00, 0xff}, // Blue
 {0x80, 0x00, 0x80}, // Violet
 {0xff, 0xa5, 0x00}, // Orange
 {0xff, 0xff, 0xff}, // White
 {0x00, 0x80, 0x00}, // Green
 {0x4b, 0x00, 0x82}, // Indigo
};

SI_SEGMENT_VARIABLE(colorNames[], codeStr_t, const SI_SEG_CODE) =
{
 " Red  ",
 "Yellow",
 " Blue ",
 "Violet",
 "Orange",
 "White ",
 "Green ",
 "Indigo",
 "Loader"
};

/////////////////////////////////////////////////////////////////////////////
// Global Variables - Menu String Constants
/////////////////////////////////////////////////////////////////////////////

const SI_SEGMENT_VARIABLE(LED_INTENSITY_BREATHE[256], uint8_t, SI_SEG_CODE) =
{
  221,221,220,218,216,213,210,206,202,197,192,187,181,175,169,162,156,149,142,
  136,129,122,116,109,103,97,91,85,79,74,69,64,59,55,50,46,42,39,35,32,29,26,
  24,21,19,17,15,13,11,10,8,7,6,5,4,3,2,2,1,1,0,0,0,0,0,0,0,0,0,1,1,2,2,3,4,5
  ,6,7,8,10,11,13,15,17,19,21,24,26,29,32,35,39,42,46,50,55,59,64,69,74,79,85,
  91,97,103,109,116,122,129,136,142,149,156,162,169,175,181,187,192,197,202,
  206,210,213,216,218,220,221,221,221,220,218,216,213,210,206,202,197,192,187,
  181,175,169,162,156,149,142,136,129,122,116,109,103,97,91,85,79,74,69,64,59,
  55,50,46,42,39,35,32,29,26,24,21,19,17,15,13,11,10,8,7,6,5,4,3,2,2,1,1,0,0,
  0,0,0,0,0,0,0,1,1,2,2,3,4,5,6,7,8,10,11,13,15,17,19,21,24,26,29,32,35,39,42,
  46,50,55,59,64,69,74,79,85,91,97,103,109,116,122,129,136,142,149,156,162,
  169,175,181,187,192,197,202,206,210,213,216,218,220,221
};

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

uint8_t DetectJoystickChange();

void DrawColorName();

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

// Must see two or more consecutive joystick presses in the
// same direction to qualify as a press.
//
// This helps reduce problems with secondary joystick
// position detection as the joystick bounces to a cardinal
// direction before returning to center.
uint8_t GetJoystickDirection()
{
  static uint8_t prevDirection = JOYSTICK_NONE;
  uint8_t curDirection = joystickDirection;
  uint8_t direction = JOYSTICK_NONE;

  if (curDirection == prevDirection)
  {
    direction = curDirection;
  }

  prevDirection = curDirection;

  return direction;
}

// Determines when a Joystick toggle has occurred
uint8_t DetectJoystickChange()
{
  static uint8_t prevDirection = JOYSTICK_NONE;
  uint8_t curDirection = GetJoystickDirection();
  uint8_t direction = JOYSTICK_NONE;

  if (curDirection != prevDirection)
  {
    direction = curDirection;
  }

  prevDirection = curDirection;

  return direction;
}

void DrawColorName()
{
  static SI_SEGMENT_VARIABLE(line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);
  uint8_t i;

  // Draw the color name string centered in the middle of the screen
  for (i = 0; i < FONT_HEIGHT; i++)
  {
    RENDER_ClrLine(line);
    RENDER_StrLine(line, (DISP_WIDTH - RENDER_GetStrSize((SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))colorNames[0]))/2, i, (SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))colorNames[colorIndex]);
    DISP_WriteLine((DISP_HEIGHT - FONT_HEIGHT)/2 + i, line);
  }
}

// Apply default settings
void RainbowBlinky_Init()
{
  ledColor = colors[colorIndex];

  // Redraw Default Demo on start
  DrawColorName();
}

// Call every time ticks is updated
void RainbowBlinky_Update()
{
  uint8_t direction = DetectJoystickChange();

  // Use color lookup table to map joystick direction
  // to color and color name
  if (direction >= JOYSTICK_N && direction <= JOYSTICK_SW)
  {
    colorIndex = direction - JOYSTICK_N;
    ledColor = colors[colorIndex];
  }

  DrawColorName();

  // Start the bootloader if PB0 is pressed
  if (BSP_PB0 == BSP_PB_PRESSED)
  {
    // Draw "loader" on the LCD screen
    colorIndex = 8;
    DrawColorName();

    // Start the bootloader by setting data[0] = bootloader signature and
    // soft resetting the device
    *((uint8_t SI_SEG_DATA *)0x00) = 0xA5;
    RSTSRC = RSTSRC_SWRSF__SET | RSTSRC_PORSF__SET;
  }
}

void RainbowBlinky_UpdateLED()
{
  uint8_t intensity = LED_INTENSITY_BREATHE[ledIntensityCounter] >> 3;

  RGB_SetColor(ledColor, intensity);
  ledIntensityCounter++;
}
