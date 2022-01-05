/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// draw.c
/////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "bsp.h"
#include "draw.h"
#include "splash.h"

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
// Generic line buffer
SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);


//-----------------------------------------------------------------------------
// RENDER_ClrLineSegment
//-----------------------------------------------------------------------------
//
// Clear the contents of a line segment buffer, setting its contents
// to the background color.
//
// line - a line buffer to clear (length: DISP_BUF_SIZE)
// x1 - the pixel x position to start writing, must be 8 bit alignment
// x2 - the pixel x position to end writing, must be divisible by 8
//-----------------------------------------------------------------------------
static void RENDER_ClrLineSegment(
		SI_VARIABLE_SEGMENT_POINTER(line, uint8_t, RENDER_LINE_SEG), uint8_t x1, uint8_t x2)
{
	uint8_t i;
    for (i = (x1 / 8); i < (x2 / 8); i++) {
        line[i] = 0;
    }
}


//-----------------------------------------------------------------------------
// DrawSplashScreen() Routine
//-----------------------------------------------------------------------------
//
// Draw SiLabs splash screen.
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


//-----------------------------------------------------------------------------
// RenderText
//-----------------------------------------------------------------------------
//
// Draw a string on screen centralist.
//
//-----------------------------------------------------------------------------
void DrawScreenText(SI_VARIABLE_SEGMENT_POINTER(str, char, RENDER_STR_SEG), uint8_t y)
{
    uint8_t i;
    uint8_t len = RENDER_GetStrSize(str);

    /* Copy a line from text area into line buffer. */
    for (i = 0; i < DISP_WIDTH / 8; i++)
    {
      Line[i] = splash_bits[16 * 100 + i];
    }

    for (i = 0; i < FONT_HEIGHT; i++) {
        RENDER_ClrLineSegment(Line, TEXT_AREA_X1, TEXT_AREA_X2);
        RENDER_StrLine(Line, (DISP_WIDTH - len) / 2, i, str);
        DISP_WriteLine(i + y, Line);
    }
}

