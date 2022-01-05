/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// draw.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "efm8_usb.h"
#include "hidmouse.h"
#include "descriptors.h"
#include "idle.h"
#include "disp.h"
#include "render.h"
#include "dpad_n.h"
#include "dpad_ne.h"
#include "dpad_e.h"
#include "dpad_se.h"
#include "dpad_s.h"
#include "dpad_sw.h"
#include "dpad_w.h"
#include "dpad_nw.h"
#include "splash.h"
#include "draw.h"
#include "button.h"

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
// Generic line buffer
SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// DrawScreenText
// ----------------------------------------------------------------------------
//
// Description - Show one line of text on the screen
//
// str - pointer to string text (0 - 21 characters) to display
// rowNum - row number of the screen (0 - 127)
// fontScale - font scaler (1 - 4)
//
//-----------------------------------------------------------------------------
void DrawScreenText(SI_VARIABLE_SEGMENT_POINTER(str, char, SI_SEG_GENERIC), uint8_t rowNum, uint8_t fontScale)
{
  uint8_t i, j;

  /* Copy a line from text area into line buffer. */
  for (i = 0; i < DISP_WIDTH / 8; i++)
  {
    Line[i] = splash_bits[16 * 100 + i];
  }

  for (i = 0; i < FONT_HEIGHT; i++) {
    RENDER_Large_StrLine(Line, TEXT_AREA_X1, i, str, fontScale);

    for (j = 1; j < fontScale + 1; j++) {
      DISP_WriteLine(rowNum + (i * fontScale) + j, Line);
    }
  }
}

//-----------------------------------------------------------------------------
// DrawSplashScreen() Routine
// ----------------------------------------------------------------------------
//
// Description - Draw Silabs splash screen.
//
//-----------------------------------------------------------------------------
void DrawSplashScreen(void)
{
  uint8_t i;
  for (i = 0; i < DISP_HEIGHT; i++)
  {
    RENDER_ClrLine(Line);
    RENDER_SpriteLine(Line, 0, i, splash_bits, DISP_WIDTH);
    DISP_WriteLine(i, Line);
  }
}

//-----------------------------------------------------------------------------
// DrawClrStatusArea() Routine
// ----------------------------------------------------------------------------
//
// Description - Clear status area on screen
//
// ignoreTextLines - If true, then don't erase text lines that will get overridden
//                   by DrawScreenText()
//
//-----------------------------------------------------------------------------
void DrawClrStatusArea(bool ignoreTextLines)
{
  uint8_t i;

  // Copy a line from text area into line buffer
  for (i = 0; i < DISP_WIDTH / 8; i++)
  {
    Line[i] = splash_bits[DISP_WIDTH / 8 * 100 + i];
  }

  for (i = TEXT_AREA_Y1; i < TEXT_AREA_Y2; i++)
  {
    if (!ignoreTextLines ||
        (i <= TEXT_LINE1_Y1)
        ||
        (i > TEXT_LINE1_Y2 && i <= TEXT_LINE2_Y1)
        ||
        (i > TEXT_LINE2_Y2))
    {
      DISP_WriteLine(i, Line);
    }
  }
}

//-----------------------------------------------------------------------------
// DrawJoystickDirection() Routine
// ----------------------------------------------------------------------------
//
// Description - Show Joystick direction on the screen.
//
// row - row number of center Joystick arrow on the  screen. (0-127)
// dir - Joystick direction
//-----------------------------------------------------------------------------
void DrawJoystickDirection(uint8_t row, uint8_t dir)
{
  uint8_t i;
  uint8_t j;
  uint8_t x;
  uint8_t y;
  uint8_t height = dpad_n_width;
  uint8_t width = dpad_n_height;
  SI_VARIABLE_SEGMENT_POINTER(sprite, uint8_t, RENDER_SPRITE_SEG);

  // This a position adjustment to fit in text area in splash screen
  uint8_t adj = 6;

  x = (DISP_WIDTH / 2) - (width / 2);
  y = row - (adj / 2);

  if (dir & JOY_UP)
  {
    y = row - height;
  }
  else if (dir & JOY_DOWN)
  {
    y = row + height - adj;
  }

  if (dir & JOY_LEFT)
  {
    x = (DISP_WIDTH / 2) - (width / 2) - width;
  }
  else if (dir & JOY_RIGHT)
  {
    x = (DISP_WIDTH / 2) - (width / 2) + width;
  }

  switch (dir)
  {
    case JOY_UP:
      //x = (DISP_WIDTH / 2) - (width / 2);
      //y = row - height;
      sprite = dpad_n_bits;
      break;

    case JOY_UP | JOY_RIGHT:
      //x = (DISP_WIDTH / 2) - (width / 2) + width;
      //y = row - height;
      sprite = dpad_ne_bits;
      break;

    case JOY_RIGHT:
      //x = (DISP_WIDTH / 2) - (width / 2) + width;
      //y = row - (adj / 2);
      sprite = dpad_e_bits;
      break;

    case JOY_DOWN | JOY_RIGHT:
      //x = (DISP_WIDTH / 2) - (width / 2) + width;
      //y = row + height - adj;
      sprite = dpad_se_bits;
      break;

    case JOY_DOWN:
      //x = (DISP_WIDTH / 2) - (width / 2);
      //y = row + height - adj;
      sprite = dpad_s_bits;
      break;

    case JOY_DOWN | JOY_LEFT:
      //x = (DISP_WIDTH / 2) - (width / 2) - width;
      //y = row + height - adj;
      sprite = dpad_sw_bits;
      break;

    case JOY_LEFT:
      //x = (DISP_WIDTH / 2) - (width / 2) - width;
      //y = row - (adj / 2);
      sprite = dpad_w_bits;
      break;

    case JOY_UP | JOY_LEFT:
      //x = (DISP_WIDTH / 2) - (width / 2) - width;
      //y = row - height;
      sprite = dpad_nw_bits;
      break;

    default:
      return;
  }

  // Draw sprite
  for (i = 0; i < height; i++)
  {
    // Copy a line from text area into line buffer
    for (j = 0; j < DISP_WIDTH / 8; j++)
    {
      Line[j] = splash_bits[DISP_WIDTH / 8 * 100 + j];
    }
    RENDER_SpriteLine(Line, x, i, sprite, width);
    DISP_WriteLine(y + i, Line);
  }
}

//-----------------------------------------------------------------------------
// DrawMouseStatus() Routine
// ----------------------------------------------------------------------------
//
// Description - Show joystick move direction and buttons status on the screen.
//
//-----------------------------------------------------------------------------
void DrawMouseStatus(void)
{
  static uint8_t prevJoystickStatus = 0;
  uint8_t i;
  uint8_t joystickStatus = Joystick_GetStatus();

  if (joystickStatus != prevJoystickStatus)
  {
    if ((joystickStatus & BUTTON_MASK)!= 0)
    {
      uint8_t str[sizeof(MULTI_BUTTON_STR_FORMAT)];
      
      for (i = 0; i < sizeof(MULTI_BUTTON_STR_FORMAT); i++)
      {
        str[i] = MULTI_BUTTON_STR_FORMAT[i];
      }

      if (joystickStatus & LEFT_BUTTON)
      {
        str[MULTI_BUTTON_STR_LEFT_OFFSET] = 'L';
      }
      if (joystickStatus & RIGHT_BUTTON)
      {
        str[MULTI_BUTTON_STR_RIGHT_OFFSET] = 'R';
      }
      if (joystickStatus & CENTER_BUTTON)
      {
        str[MULTI_BUTTON_STR_CENTER_OFFSET] = 'C';
      }

      DrawClrStatusArea(true);
      DrawScreenText((SI_VARIABLE_SEGMENT_POINTER(, char, SI_SEG_GENERIC))str, 80, 1);
      DrawScreenText("    Pressed!", 96, 1);
    }
    else
    {
      DrawClrStatusArea(false);
      DrawJoystickDirection(ARROW_POS_Y1, (joystickStatus & JOY_MASK));
    }

    prevJoystickStatus = joystickStatus;
  }
}
