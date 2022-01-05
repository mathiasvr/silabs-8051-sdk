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

#include <bsp.h>
#include <efm8_usb.h>
#include <descriptors.h>
#include <idle.h>
#include <disp.h>
#include <render.h>
#include <splash.h>
#include <draw.h>
#include <button.h>

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
	for (i = 0; i < DISP_HEIGHT; i++) {
		RENDER_ClrLine(Line);
		RENDER_SpriteLine(Line, 0,  i, splash_bits, DISP_WIDTH);
		DISP_WriteLine(i, Line);
	}
}
