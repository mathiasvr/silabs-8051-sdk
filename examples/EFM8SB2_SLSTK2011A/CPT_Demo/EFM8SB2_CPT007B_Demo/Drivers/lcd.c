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

/////////////////////////////////////////////////////////////////////////////
// Button action definitions
/////////////////////////////////////////////////////////////////////////////

#define		button_released		0
#define		button_pressed		1
#define		button_disabled		2
#define		slider_pressed		0xFFFF

/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////

// Generic line buffer
SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

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
uint8_t Out_y = 96;	// starting y point

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
	  #ifdef SI_EFM8SB1_DEFS_H // SB1 demo supports CS00-03
		    if (curButton >= 4)
	  #else
		    if (curButton >= TOTAL_CAPSENSE_PIN)
	  #endif
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

  // Draw Circle Sprites
  for (col = 0; col < OUT_COLS; col++)
  {
    for (line = 0; line < OUT_HEIGHT; line++)
    {
    	//to display the mirror image of buttons in buttonStatus
    	uint8_t row_inv = OUT_ROWS-1;
    	RENDER_ClrLine(Line);

		for (row = 0; row < OUT_ROWS; row++)
		{
		// get current Cap-sense Button number
		uint8_t curButton = row_inv + col * OUT_ROWS;

		// if statement to not displaying disabled circles
		if (curButton < TOTAL_CAPSENSE_PIN)
		{
			if (col == 1)
			{	//stagger for buttons in the first row
			   RENDER_SpriteLine(Line, (row * OUT_WIDTH)-(OUT_WIDTH/2), line,
											outSprites[buttonStatus[row_inv][col]],
												OUT_WIDTH);
			}
			else
			{
				// Draw OUT circle
				RENDER_SpriteLine(Line, row * OUT_WIDTH, line,
								outSprites[buttonStatus[row_inv][col]],
								OUT_WIDTH);
			}
		}

        row_inv--;
      }
      // Draw current display line to screen
      DISP_WriteLine(dispLine, Line);
      dispLine--;
    }
  }
}
