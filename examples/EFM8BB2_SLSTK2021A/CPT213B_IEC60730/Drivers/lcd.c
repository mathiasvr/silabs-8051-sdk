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
#include "iec60730.h"
#include "cpt213b_state_machine.h"
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
#include "heart_empty.h"
#include "heart_full.h"
#include "heart_broken.h"
#include "skull.h"

/////////////////////////////////////////////////////////////////////////////
// Button action definitions
/////////////////////////////////////////////////////////////////////////////

#define		button_released		0
#define		button_pressed		1
#define		button_disabled		2

/////////////////////////////////////////////////////////////////////////////
// Typedefs
/////////////////////////////////////////////////////////////////////////////

typedef SI_VARIABLE_SEGMENT_POINTER(, char, const SI_SEG_CODE) codeStr_t;

/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////

// Generic line buffer
SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

/////////////////////////////////////////////////////////////////////////////
// Globals - Graphics
/////////////////////////////////////////////////////////////////////////////

// Sprite lookup table
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(outSprites[2], uint8_t, RENDER_SPRITE_SEG, SI_SEG_IDATA) =
{
  out_off_bits,
  out_on_bits,
};

// Heart/Skull lookup table
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(ncmState[4], uint8_t, RENDER_SPRITE_SEG, SI_SEG_IDATA) =
{
  heart_empty_bits,
  heart_full_bits,
  heart_broken_bits,
  skull_bits
};

// Keep track of which cap-sense buttons are being pressed
SI_SEGMENT_VARIABLE(buttonStatus[OUT_ROWS][OUT_COLS], uint8_t, SI_SEG_IDATA);

uint8_t Out_x;	// starting x point
uint8_t Out_y = OUT_WIDTH+(OUT_WIDTH/2);	// starting y point

// Function to print the heart or the skull depending on the state
void printNCM(uint8_t factorNCM);

/******************************************************************************
 * initialize and clear the 128x128 lcd display
 *****************************************************************************/
void initLCD(void)
{
  DISP_Init();
}

/******************************************************************************
 * update lcd display frame
 *****************************************************************************/
void updateLCD(void)
{
  uint8_t row, col;
  uint8_t line;
  uint8_t dispLine = Out_y;
  uint8_t total_rows = 4;
  uint8_t curButton;
  // update OUT[OUT_ROWS][OUT_COLS] data structure
  for (col = 0; col < OUT_COLS; col++)
  {
    for (row = 0; row < OUT_ROWS; row++)
    {
#if CPT_DEMO_MODE
	  if(col == 1)
	  {
		total_rows = OUT_ROWS;
	    // get current Cap-sense Button number
		curButton = (row + col * total_rows) - 1;
	  }
		else if (col > 1)
		{
			total_rows = OUT_ROWS-1;
			// get current Cap-sense Button number
			curButton = (row + col * total_rows) + 1;
		}
		else
		{
			total_rows = OUT_ROWS-1;
			// get current Cap-sense Button number
			curButton = (row + col * total_rows);
		}
#else
		curButton = (row + col * total_rows);
#endif

      if(curButton < TOTAL_CAPSENSE_PIN)
      {
        if (capsenseCurrent & (0x01 << curButton))
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
  }

  // Draw Circle Sprites
  for (col = 0; col < OUT_COLS; col++)
  {
    for (line = 0; line < OUT_HEIGHT; line++)
    {
      RENDER_ClrLine(Line);
      for (row = 0; row < total_rows; row++)
      {
#if CPT_DEMO_MODE
		if(col == 1)
		{
			total_rows = 5;
			// get current Cap-sense Button number
			curButton = (row + col * total_rows) - 1;
		}
		else if (col > 1)
		{
			total_rows = 4;
			// get current Cap-sense Button number
			curButton = (row + col * total_rows) + 1;
		}
		else
		{
			total_rows = 4;
			// get current Cap-sense Button number
			curButton = (row + col * total_rows);
		}
#else
		curButton = (row + col * total_rows);
#endif
        // if statement to not displaying disabled circles
        if (curButton < TOTAL_CAPSENSE_PIN)
        {
#if CPT_DEMO_MODE
          if (col % 2 == 0)
          {	//stagger for buttons in the first and third rows
#endif
        	RENDER_SpriteLine(Line, (row * OUT_WIDTH)+(OUT_WIDTH/2), line,
            outSprites[buttonStatus[row][col]],
            OUT_WIDTH);

#if CPT_DEMO_MODE
          }
          else
          {
        	// Draw OUT circle
            RENDER_SpriteLine(Line, row * OUT_WIDTH, line,
            outSprites[buttonStatus[row][col]],
            OUT_WIDTH);

          }
#endif
        }
      }
      if(dispLine >= Out_y)
      {
    	// Draw current display line to screen
		DISP_WriteLine(dispLine, Line);
		if(dispLine == Out_y*2)
		{
		  iec60730_RestartWatchdog();
		}
		dispLine++;
      }
    }
  }
}
/******************************************************************************
 * Print NCM
 *
 * This function is used to print the state of the CPT device. If status
 * packets are received on time, a heart will keep beating at the top of the
 * LCD screen.
 * If the device is in No Confidence Mode, the code prints a broken heart
 * until the state is restored back to normal.
 * If the device is in a safe state, a skull is printed on the top of the
 * screen and the device.
 *****************************************************************************/
void printNCM(uint8_t factorNCM)
{
  uint8_t i;

  // heart_empty_height == heart_full_height == skull_height
  for (i = 0; i < heart_empty_height; i++)
  {
    RENDER_ClrLine(Line);
    RENDER_SpriteLine(Line, (TOTAL_OUT_WIDTH-heart_empty_width),
    		i, ncmState[factorNCM], heart_empty_width);
    DISP_WriteLine(i, Line);
  }
}
