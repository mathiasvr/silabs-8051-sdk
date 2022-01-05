/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// function_generator.c
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Call Graph
//
// FunctionGenerator_main()
//  |
//  +- drawSplash()
//  |   +- getWaitJoystick()
//  |   |   +- getJoystick()
//  |
//  +- drawStaticSprites()
//  |   +- drawScreenSprite()
//  |
//  +- processInput()
//  |   +- transitionDemoWaveform()
//  |   +- transitionDemoFrequency()
//  |
//  +- drawScreen()
//  |   +- drawScreenWaveform()
//  |   +- drawScreenFrequency()
//  |       +- drawScreenText()
//  |
//  +- synchFrame()
//
// Timer4_ISR()
//
// PORTMATCH_ISR()
//

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "tick.h"
#include "disp.h"
#include "render.h"
#include "joystick.h"
#include "thinfont.h"
#include "function_generator.h"
#include "sine.h"
#include "square.h"
#include "triangle.h"
#include "sawtooth.h"
#include "windowed_sine.h"
#include "nav_up.h"
#include "nav_down.h"
#include "nav_left.h"
#include "nav_right.h"
#include "waveform_tables.h"
#include "retargetserial.h"

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////

// Generic line buffer
SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

// Demo state variables
static DemoState currentDemoState = DEMO_SINE;
static SI_VARIABLE_SEGMENT_POINTER(currentTable, uint16_t, const SI_SEG_CODE) = sineTable; // current waveform table for DAC output
static SI_VARIABLE_SEGMENT_POINTER(currentWaveform, uint8_t, const SI_SEG_CODE) = sine_bits; // current waveform picture

// Frequency selection
#define SUPPORTED_NUM_FREQ 8
static SI_SEGMENT_VARIABLE(frequency[SUPPORTED_NUM_FREQ], uint16_t, SI_SEG_XDATA) = {
		10L,
		20L,
		50L,
		100L,
		200L,
		500L,
		1000L,
		2000L
};

// Current Frequency Selection
static uint8_t currentFreqIndex = 3;

// Phase offset (updated when frequency is changed)
static uint16_t phaseOffset = 100 * PHASE_PRECISION / SAMPLE_RATE_DAC;

// Kill splash
KillSpash killSplashFlag = SHOW_SPLASH;

///////////////////////////////////////////////////////////////////////////////
// Supporting Functions
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// transitionDemoWaveform
//-----------------------------------------------------------------------------
//
// Change function/waveform.
// Left  - change function order: sine < square < triangle < sawtooth < windowed sine
// Right - change function order: sine > square > triangle > sawtooth > windowed sine
//
// dir - valid arguments are: JOYSTICK_E, JOYSTICK_W
//
static void transitionDemoWaveform(uint8_t dir)
{
  if (dir == JOYSTICK_E)
  {
	  switch (currentDemoState)
	  {
		case DEMO_SINE:
		  currentDemoState = DEMO_SQUARE;
		  currentWaveform = square_bits;
		  currentTable = squareTable;
		  break;

		case DEMO_SQUARE:
		  currentDemoState = DEMO_TRIANGLE;
		  currentWaveform = triangle_bits;
		  currentTable = triangleTable;
		  break;

		case DEMO_TRIANGLE:
          currentDemoState = DEMO_SAWTOOTH;
		  currentWaveform = sawtooth_bits;
		  currentTable = sawtoothTable;
		  break;

		case DEMO_SAWTOOTH:
		  currentDemoState = DEMO_WINDOWED_SINE;
		  currentWaveform = windowed_sine_bits;
		  currentTable = windowedSineTable;
		  break;

		case DEMO_WINDOWED_SINE:
		  currentDemoState = DEMO_SINE;
		  currentWaveform = sine_bits;
		  currentTable = sineTable;
		  break;
	  }
  }
  else if (dir == JOYSTICK_W)
  {
	  switch (currentDemoState)
	  {
		case DEMO_SINE:
		  currentDemoState = DEMO_WINDOWED_SINE;
		  currentWaveform = windowed_sine_bits;
		  currentTable = windowedSineTable;
		  break;

		case DEMO_SQUARE:
		  currentDemoState = DEMO_SINE;
		  currentWaveform = sine_bits;
		  currentTable = sineTable;
		  break;

		case DEMO_TRIANGLE:
		  currentDemoState = DEMO_SQUARE;
		  currentWaveform = square_bits;
		  currentTable = squareTable;
		  break;

		case DEMO_SAWTOOTH:
		  currentDemoState = DEMO_TRIANGLE;
		  currentWaveform = triangle_bits;
		  currentTable = triangleTable;
		  break;

		case DEMO_WINDOWED_SINE:
		  currentDemoState = DEMO_SAWTOOTH;
		  currentWaveform = sawtooth_bits;
		  currentTable = sawtoothTable;
		  break;
	  }
  }
}

//-----------------------------------------------------------------------------
// transitionDemoFrequency
//-----------------------------------------------------------------------------
//
// Change frequency of the function.
// Up   - increase frequency
// Down - decrease frequency
//
// dir - valid arguments are: JOYSTICK_N, JOYSTICK_S
//
static void transitionDemoFrequency(uint8_t dir)
{
  if (dir == JOYSTICK_N)
  {
	  // increase freq
	  if(currentFreqIndex == SUPPORTED_NUM_FREQ-1)
	  {
		  currentFreqIndex = 0;
	  }
	  else
	  {
		  currentFreqIndex++;
	  }
  }
  else if (dir == JOYSTICK_S)
  {
	  // decrease freq
	  if (currentFreqIndex == 0)
	  {
		  currentFreqIndex = SUPPORTED_NUM_FREQ-1;
	  }
	  else
	  {
		  currentFreqIndex--;
	  }
  }

  phaseOffset = frequency[currentFreqIndex] * PHASE_PRECISION / SAMPLE_RATE_DAC;
}

//-----------------------------------------------------------------------------
// getJoystick
//-----------------------------------------------------------------------------
//
// Get new ADC sample and return joystick direction. Valid return values:
//  JOYSTICK_NONE   JOYSTICK_N   JOYSTICK_S
//  JOYSTICK_C      JOYSTICK_E   JOYSTICK_W
//
static uint8_t getJoystick(void)
{
  uint32_t mv;
  uint8_t dir;

  ADC0CN0_ADBUSY = 1;
  while (!ADC0CN0_ADINT);
  ADC0CN0_ADINT = 0;

  mv = ((uint32_t)ADC0) * 3300 / 1024;

  dir = JOYSTICK_convert_mv_to_direction(mv);

  return dir;
}

//-----------------------------------------------------------------------------
// getWaitJoystick
//-----------------------------------------------------------------------------
//
// Get joystick input. If joystick was moved, wait for release. Return joystick
// direction. Valid return values:
//  JOYSTICK_NONE   JOYSTICK_N   JOYSTICK_S
//  JOYSTICK_C      JOYSTICK_E   JOYSTICK_W
//
static uint8_t getWaitJoystick(void)
{
  uint8_t dir, dirSave;

  dir = getJoystick();
  dirSave = dir;

  // wait for release then transition
  while (dir != JOYSTICK_NONE)
  {
    dir = getJoystick();
  }

  return dirSave;
}

//-----------------------------------------------------------------------------
// getJoystickDemo
//-----------------------------------------------------------------------------
//
// Get and process joystick input.
// Left/Right = change function/waveform
// Up/Down    = change frequency
//
static void processInput(uint8_t dir)
{
  // process input
  if ((dir == JOYSTICK_E) || (dir == JOYSTICK_W))
  {
    transitionDemoWaveform(dir);
  }
  else if ((dir == JOYSTICK_N) || (dir == JOYSTICK_S))
  {
    transitionDemoFrequency(dir);
  }
}

//-----------------------------------------------------------------------------
// drawScreenText
//-----------------------------------------------------------------------------
//
// Show one line of text on the screen
//
// str - pointer to string text (0 - 21 characters) to display
// rowNum - row number of the screen (0 - 127)
// fontScale - font scaler (1 - 4)
//
static void drawScreenText(SI_VARIABLE_SEGMENT_POINTER(str, char, RENDER_STR_SEG), uint8_t rowNum)
{
  uint8_t i;

  for (i = 0; i < FONT_HEIGHT; i++)
  {
    RENDER_ClrLine(Line);
    RENDER_StrLine(Line, 4, i, str);
    DISP_WriteLine(rowNum + i, Line);
  }
}

//-----------------------------------------------------------------------------
// drawScreenSprite
//-----------------------------------------------------------------------------
//
// Displays a sprite on the screen
//
// sprite - pointer to sprite/picture
// width  - width of sprite
// height - height of sprite
// xPos - beginning row number on the screen (0 - 127)
// yPos - beginning col number on the screen (0 - 127)
//
static void drawScreenSprite(SI_VARIABLE_SEGMENT_POINTER(sprite, uint8_t, RENDER_SPRITE_SEG),
		                     uint8_t width, uint8_t height,
		                     uint8_t xPos, uint8_t yPos)
{
  uint8_t i;

  for (i = 0; i < height; i++)
  {
    RENDER_ClrLine(Line);
    RENDER_SpriteLine(Line, xPos, i, sprite, width);
    DISP_WriteLine(yPos + i, Line);
  }
}

//-----------------------------------------------------------------------------
// drawScreenWaveform
//-----------------------------------------------------------------------------
//
// Draw screen waveform and left/right arrows
//
static void drawScreenWaveform(void)
{
  uint8_t i; // row index for waveform sprite
  uint8_t j = 0; // row index for nav sprites

  for (i = 0; i < sine_height; i++)
  {
	RENDER_ClrLine(Line);
	RENDER_SpriteLine(Line, X_POS_WAVEFORM, i, currentWaveform, sine_width);

	if ((i >= Y_POS_NAV_ARROW_LEFT_RIGHT) && (i < Y_POS_NAV_ARROW_LEFT_RIGHT + nav_left_height))
	{
	  RENDER_SpriteLine(Line, X_POS_NAV_ARROW_LEFT, j, nav_left_bits, nav_left_width);
	  RENDER_SpriteLine(Line, X_POS_NAV_ARROW_RIGHT, j, nav_right_bits, nav_right_width);
	  j++;
	}
	DISP_WriteLine(Y_POS_WAVEFORM + i, Line);
  }

}

//-----------------------------------------------------------------------------
// drawScreenFrequency
//-----------------------------------------------------------------------------
//
// Update the function frequency on the screen. Format:
//   f = 1000 Hz
//
static void drawScreenFrequency(void)
{
  char freqStr[22];

  // display frequency on screen
  RETARGET_SPRINTF(freqStr, "     f = %d Hz", frequency[currentFreqIndex]);

  drawScreenText(freqStr, Y_POS_FREQ);
}

//-----------------------------------------------------------------------------
// drawSplash
//-----------------------------------------------------------------------------
//
// Display splash screen with instructions.
//
static void drawSplash(void)
{
	uint16_t ticks = GetTickCount();

	drawScreenText(" __________________", 7);
	drawScreenText(" FUNCTION GENERATOR", 4);

	drawScreenText("USE SCOPE TO OBSERVE", 48);
	drawScreenText("    DAC OUTPUTS:", 58);
	drawScreenText("    P3.0 & P3.1", 74);

	// kill splash if timeout elapses, any button is pressed, or joystick
	// is moved in any direction
	while (((GetTickCount() - ticks) < SPASH_TIMEOUT) &&
			(killSplashFlag == SHOW_SPLASH) &&
			(getWaitJoystick() == JOYSTICK_NONE));
}

//-----------------------------------------------------------------------------
// drawScreenStaticSprites
//-----------------------------------------------------------------------------
//
// Draw static/non-changing sprites on screen. Static sprites are up and down
// arrows. Left/right arrows are drawn with waveform.
//
static void drawScreenStaticSprites(void)
{
  drawScreenSprite(nav_up_bits, nav_up_width, nav_up_height,
		           X_POS_NAV_ARROW_UP_DOWN, Y_POS_NAV_ARROW_UP);
  drawScreenSprite(nav_down_bits, nav_down_width, nav_down_height,
		           X_POS_NAV_ARROW_UP_DOWN, Y_POS_NAV_ARROW_DOWN);
}

//-----------------------------------------------------------------------------
// drawScreen
//-----------------------------------------------------------------------------
//
// Draw dynamic/changeable sprites and info (waveform and frequency) on screen.
//
static void drawScreen(void)
{
  drawScreenWaveform();
  drawScreenFrequency();
}

//-----------------------------------------------------------------------------
// synchFrame
//-----------------------------------------------------------------------------
//
// Delay until start of next frame
//
static void synchFrame(void)
{
  static uint16_t lastTick = 0;
  uint16_t tick;

  // Render at 50 Hz
  while (((tick = GetTickCount()) - lastTick) < HZ_TO_MS(DEMO_FRAME_RATE));
  lastTick = tick;
}

///////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
///////////////////////////////////////////////////////////////////////////////

SI_INTERRUPT_USING(TIMER4_ISR, TIMER4_IRQn, 1)
{
  static uint16_t phaseAcc = 0;       // Holds phase accumulator

  SI_UU16_t temp;   // The temporary value that holds
									  // value before being written
									  // to the DAC
  
  TMR4CN0 &= ~TMR3CN0_TF3H__BMASK;    // Clear Timer4 overflow flag

  phaseAcc += phaseOffset;            // Increment phase accumulator

  // Read the table value
  temp.u16 = currentTable[phaseAcc >> 8];

  // Set the value of <temp> to the next output of DAC at full-scale
  // amplitude. The rails are 0x000 and 0xFFF. DAC low byte must be
  // written first.

  SFRPAGE = PG4_PAGE;

  DAC1L = DAC0L = temp.u8[LSB];
  DAC1H = DAC0H = temp.u8[MSB];
}

//-----------------------------------------------------------------------------
// PMATCH_ISR
//-----------------------------------------------------------------------------
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the button is released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for int32_t periods of time.
//
SI_INTERRUPT(PMATCH_ISR, PMATCH_IRQn)
{
  uint8_t SFRPAGE_save = SFRPAGE;

  // Kill splash and run demo if user presses any button
  if((BSP_PB0 == BSP_PB_PRESSED) || (BSP_PB1 == BSP_PB_PRESSED))
  {
	  killSplashFlag = KILL_SPLASH;
  }

  SFRPAGE = PG2_PAGE;

  EIE1 &= ~0x02;                     // Disable Port Match interrupts to
                                     // prevent multiple interrupts from
                                     // occurring while the button is
                                     // pressed
  SFRPAGE = SFRPAGE_save;
}

///////////////////////////////////////////////////////////////////////////////
// Driver Function
///////////////////////////////////////////////////////////////////////////////

void FunctionGenerator_main(void)
{
  drawSplash();

  DISP_ClearAll();
  drawScreenStaticSprites();

  while(1)
  {
    processInput(getWaitJoystick());
    drawScreen();
    synchFrame();
  }
}
