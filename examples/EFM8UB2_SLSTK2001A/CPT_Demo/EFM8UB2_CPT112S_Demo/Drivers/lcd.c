/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include "bsp.h"
#include "disp.h"
#include "render.h"
#include "smbus.h"

// Standard library
#include <string.h>
#include "retargetserial.h"
#include <stdlib.h>

// project library
#include "project_config.h"
#include "lcd.h"
// Sprites
#include "out_on.h"
#include "out_off.h"
#include "out_disable.h"
#include "circle.h"

/////////////////////////////////////////////////////////////////////////////
// Button action definitions
/////////////////////////////////////////////////////////////////////////////

#define		button_released		0
#define		button_pressed		1
#define		button_disabled		2
#define		slider_pressed		0xFFFF

/////////////////////////////////////////////////////////////////////////////
// Typedefs
/////////////////////////////////////////////////////////////////////////////

typedef SI_VARIABLE_SEGMENT_POINTER(, char, const SI_SEG_CODE) codeStr_t;

/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////

// Generic line buffer
SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

// Text for the Slider
SI_SEGMENT_VARIABLE(SliderInfo[2], codeStr_t, const SI_SEG_CODE) =
{
 " Place a finger  ",
 " on the slider  "
};

/////////////////////////////////////////////////////////////////////////////
// Globals - Graphics
/////////////////////////////////////////////////////////////////////////////

// Sprite lookup table
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(outSprites[3], uint8_t, RENDER_SPRITE_SEG, SI_SEG_IDATA) =
{
  out_off_bits,
  out_on_bits,
  out_disable_bits
};

// Keep track of which cap-sense buttons are being pressed
SI_SEGMENT_VARIABLE(buttonStatus[OUT_ROWS][OUT_COLS], uint8_t, SI_SEG_IDATA);

uint8_t Out_x;	// starting x point
uint8_t Out_y = 2*OUT_WIDTH;	// starting y point

/******************************************************************************
 * initialize and clear the 128x128 lcd display
 *****************************************************************************/
void initLCD(void)
{
  DISP_Init();
  DISP_ClearAll();
}

/******************************************************************************
 * update lcd display frame
 *****************************************************************************/
void updateLCD(void)
{
  uint8_t row, col;
  uint8_t line;
  uint8_t dispLine = Out_y;

  // update OUT[OUT_ROWS][OUT_COLS] data structure
  for (row = 0; row < OUT_ROWS; row++)
  {
    for (col = 0; col < OUT_COLS; col++)
    {
      // get current Cap-sense Button number
      uint8_t curButton = col + row * OUT_COLS;
      if (curButton >= TOTAL_CAPSENSE_PIN-1)
      {
        // check for unused sprites, make them to disabled state
        buttonStatus[row][col] = button_disabled;
      }
      else if (capsenseCurrent & (0x01 << curButton))
      {
        // check if it's being pressed, if so set to ON state
        buttonStatus[row][col] = button_pressed;
      }
      else
      {
        // if not being pressed, set to OFF state
        buttonStatus[row][col] = button_released;
      }
    }
  }

  //Draw the circle for the slider
  if (sliderCurrent != slider_pressed) // on slider press
  {
    PrintBall();
  }
  else if (sliderCurrent == slider_pressed) // on slider release
  {
    SliderName();
  }

  // Draw Circle Sprites for buttons
  for (row = 1; row < OUT_ROWS; row++)
  {
    for (line = 0; line < OUT_HEIGHT; line++)
    {
      RENDER_ClrLine(Line);

      for (col = 0; col < OUT_COLS; col++)
      {
        // get current Cap-sense Button number
        uint8_t curButton = col + row * OUT_COLS;

        // if statement to not displaying disabled circles
        if (curButton < TOTAL_CAPSENSE_PIN)
        {
          // Draw OUT circle
          RENDER_SpriteLine(Line,
							col * OUT_WIDTH, line,
                            outSprites[buttonStatus[row][col]],
                            OUT_WIDTH);
        }
      }
      // Draw current display line to screen
      DISP_WriteLine(dispLine, Line);
      dispLine++;
    }
  }
}

/******************************************************************************
 * Display text on the slider
 *****************************************************************************/

void SliderName()
{
  uint8_t i;

  // Draw the slider text string centered in the middle of the screen
  for (i = 0; i < FONT_HEIGHT; i++)
  {
    RENDER_ClrLine(Line);
    RENDER_StrLine(Line,
    			   (DISP_WIDTH-RENDER_GetStrSize((SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))SliderInfo[0]))/2,
    				i,
    				(SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))SliderInfo[0]);
    DISP_WriteLine(i+FONT_HEIGHT, Line);
    RENDER_StrLine(Line,
    			   (DISP_WIDTH-RENDER_GetStrSize((SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))SliderInfo[1]))/2,
    				i,
    				(SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))SliderInfo[1]);
    DISP_WriteLine(i+FONT_HEIGHT*2, Line);
  }
}

/******************************************************************************
 * Draw the Ball on the LCD screen when the slider is used
 *****************************************************************************/

void PrintBall()
{
    uint8_t i;
    uint8_t lineNum;

    // Erase dirty lines from last circle position
    for (i = 0; i < circle_height; i++)
    {
        lineNum = i +  (circle_height / 2);

        // Only erase the lines that won't be drawn for the new circle position
        if (lineNum + 1 > -8 && lineNum < 8)
            DISP_ClearLine(lineNum, COLOR_BLACK);

    }

    // Draw the new circle position
    for (i = 0; i < circle_height; i++)
    {
        RENDER_ClrLine(Line);
        RENDER_SpriteLine(Line,
						  TOTAL_OUT_WIDTH-(circle_height+sliderValue),
						  i,
						  circle_bits,
						  circle_height);
        DISP_WriteLine(i + (circle_height / 2), Line);
    }
}
