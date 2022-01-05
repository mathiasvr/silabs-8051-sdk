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
#include "joystick.h"
#include "disp.h"
#include "render.h"
#include "tick.h"
#include "utils.h"
#include "pwm_blinky.h"
#include "adc.h"
#include <string.h>
#include "retargetserial.h"

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////

uint8_t ledIntensity = 64;

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

void SetLedIntensity(uint8_t intensity);
void DrawIntensityValue();

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

void DrawIntensityValue()
{
  static SI_SEGMENT_VARIABLE(line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);
  char str[20];
  uint8_t i;

  RETARGET_SPRINTF(str, "Brightness: %d", (int16_t)ledIntensity);

  // Draw the color name string centered in the middle of the screen
  for (i = 0; i < FONT_HEIGHT; i++)
  {
    RENDER_ClrLine(line);
    RENDER_StrLine(line, (DISP_WIDTH - RENDER_GetStrSize(str))/2, i, str);
    DISP_WriteLine((DISP_HEIGHT - FONT_HEIGHT)/2 + i, line);
  }
}

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

// Apply default settings
void PWM_Blinky_Init()
{
  ledIntensity = 64;

  // Redraw Default Demo on start
  DrawIntensityValue();
}

// Call every time ticks is updated
void PWM_Blinky_Update()
{
  uint8_t direction = joystickDirection;

  if (direction == JOYSTICK_N)
  {
    if (ledIntensity < 255)
    {
      ledIntensity++;
    }
  }
  else if (direction == JOYSTICK_S)
  {
    if (ledIntensity > 0)
    {
      ledIntensity--;
    }
  }

  DrawIntensityValue();

  // Start the bootloader if PB0 is pressed
  if (BSP_PB0 == BSP_PB_PRESSED)
  {
    // Turn the LED off and update the screen
    ledIntensity = 0;
    DrawIntensityValue();
    PWM_Blinky_UpdateLED();

    // Start the bootloader by setting data[0] = bootloader signature and
    // soft resetting the device
    *((uint8_t SI_SEG_DATA *)0x00) = 0xA5;
    RSTSRC = RSTSRC_SWRSF__SET | RSTSRC_PORSF__SET;
  }
}

void PWM_Blinky_UpdateLED()
{
  SetLedIntensity(ledIntensity);
}
