/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// voltmeter.c
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "tick.h"
#include "disp.h"
#include "render.h"
#include "voltmeter.h"
#include "joystick.h"
#include "adc.h"
#include "thinfont.h"
#include "dpad_n.h"
#include "dpad_e.h"
#include "dpad_s.h"
#include "dpad_w.h"
#include "dpad_c.h"
#include "dpad_nw.h"
#include "dpad_ne.h"
#include "dpad_sw.h"
#include "dpad_se.h"
#include "dpad_bold_n.h"
#include "dpad_bold_e.h"
#include "dpad_bold_s.h"
#include "dpad_bold_w.h"
#include "dpad_bold_c.h"
#include "dpad_bold_nw.h"
#include "dpad_bold_ne.h"
#include "dpad_bold_sw.h"
#include "dpad_bold_se.h"

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////

SI_SEGMENT_VARIABLE_SEGMENT_POINTER(west_bits,      uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(east_bits,      uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(north_bits,     uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(south_bits,     uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(center_bits,    uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(northwest_bits, uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(northeast_bits, uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(southwest_bits, uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(southeast_bits, uint8_t, const SI_SEG_CODE, SI_SEG_XDATA);

// Generic line buffer
SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

// ADC input: joystick or external
bool adcInput;
#define ADCINPUT_JOYSTICK 0
#define ADCINPUT_EXTERNAL 1

// Flag to update the ADC input text on the screen.
uint8_t updateAdcInput;
#define UPDATEADCINPUT_NO  0
#define UPDATEADCINPUT_YES 1

///////////////////////////////////////////////////////////////////////////////
// Supporting Functions
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// convertMillivoltToVolt
//-----------------------------------------------------------------------------
//
// Converts mV string to V string with units.
// e.g. '33000' to '3.3000 V'
//
// mVolt - 0 to 33000 e-4 V
// voltStr  - string with V info, length is 9 (added decimal point, space, and unit)
//
void convertMillivoltToVolt(uint16_t mVolt, char * voltStr)
{
  int8_t pos = 7;
  
  // Work backwards generating the string
  // Start with null-terminator
  // followed by V symbol  
  voltStr[pos--] = '\0';
  voltStr[pos--] = 'V';
  
  // followed by thousandths digit
  // followed by hundredths digit
  // followed by tenths digit
  // followed by the decimal point at pos=1
  // followed by the ones digit
  for (; pos >= 0; pos--)
  {
    // Pos = 1 is the location of the decimal point
    if (pos == 1)
    {
      voltStr[pos] = '.';
    }
    else
    {
      // Get the right-most digit from the number
      // and convert to ascii
      voltStr[pos] = (mVolt % 10) + '0';
      
      // Shift number by 1 decimal place to the right
      mVolt /= 10; 
    }
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
void drawScreenText(SI_VARIABLE_SEGMENT_POINTER(str, char, RENDER_STR_SEG), uint8_t rowNum, uint8_t fontScale)
{
  uint8_t i, j;

  for (i = 0; i < FONT_HEIGHT; i++)
  {
    RENDER_ClrLine(Line);
    RENDER_Large_StrLine(Line, 4, i, (SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))str, fontScale);

    for (j = 1; j < fontScale + 1; j++)
    {
      DISP_WriteLine(rowNum + (i * fontScale) + j, Line);
    }
  }
}

//-----------------------------------------------------------------------------
// drawScreenDpadRow
//-----------------------------------------------------------------------------
//
// Displays one row of dpad graphics (ie left arrow, center, right arrow)
// centered on the screen.
//
// pic1_bits - pointer to leftmost picture
// pic2_bits - pointer to center picture
// pic3_bits - pointer to rightmost picture
// rowNum - row number on the screen (0 - 127)
//
void drawScreenDpadRow(
  SI_VARIABLE_SEGMENT_POINTER(pic1_bits, uint8_t, RENDER_SPRITE_SEG),
  SI_VARIABLE_SEGMENT_POINTER(pic2_bits, uint8_t, RENDER_SPRITE_SEG),
  SI_VARIABLE_SEGMENT_POINTER(pic3_bits, uint8_t, RENDER_SPRITE_SEG),
  uint8_t rowNum)
{
  uint8_t i;
  uint8_t picWidth = dpad_c_width;
  uint8_t picHieght = dpad_c_height;

  for (i = 0; i < picHieght; i++)
  {
    RENDER_ClrLine(Line);
    RENDER_SpriteLine(Line,
                      X_CENTER - picWidth / 2 - picWidth, i,
                      pic1_bits, picWidth);
    RENDER_SpriteLine(Line, X_CENTER - picWidth / 2, i, pic2_bits,
                      picWidth);
    RENDER_SpriteLine(Line,
                      X_CENTER - picWidth / 2 + picWidth, i,
                      pic3_bits, picWidth);
    DISP_WriteLine(rowNum + i, Line);
  }
}

//-----------------------------------------------------------------------------
// drawScreenAdcInput
//-----------------------------------------------------------------------------
//
// Update the ADC input on the screen if updateAdcInput flag is set.
//
// updateAdcInput flag is set at the beginning of the demo and
// when the user presses the button.
//
void drawScreenAdcInput(void)
{
  uint8_t i;

  if (updateAdcInput == UPDATEADCINPUT_YES)
  {
    // Draw adcInput
    if (adcInput == ADCINPUT_JOYSTICK)
    {
      drawScreenText("ADC INPUT: JOYSTICK", 0, FONT_SCALE_DEFAULT);
    }
    else if (adcInput == ADCINPUT_EXTERNAL)
    {
      drawScreenText("ADC INPUT: EXTERNAL", 0, FONT_SCALE_DEFAULT);

      // Clear dpad from screen
      for (i = 127-48; i <= 127; i++)
      {
        DISP_ClearLine(i, COLOR_BLACK);
      }

      drawScreenText("APPLY VOLTAGE", 127-48, FONT_SCALE_DEFAULT);
      drawScreenText("(0-3.3V) TO P1.1", 127-36, FONT_SCALE_DEFAULT);
    }

    updateAdcInput = UPDATEADCINPUT_NO;
  }
}

//-----------------------------------------------------------------------------
// drawScreen
//-----------------------------------------------------------------------------
//
// Update ADC reading on the screen in the following format
//    y.yyyy V
//
// And displays the directional pad. When dpad is pressed, the respective
// arrow is bolded on the display.
//
// mVolt - ADC input in 10^-4 V as unsigned long
//
void drawScreen(uint32_t mVolt)
{
  char voltStr[9];

  static uint32_t mVoltLast = 0;

  // Draw ADC input
  drawScreenAdcInput();

  if((mVolt < mVoltLast - HYSTERESIS_MV) || (mVolt > mVoltLast + HYSTERESIS_MV))
  {
	  convertMillivoltToVolt(mVolt, voltStr);
	  drawScreenText(voltStr, Y_VOLTAGE_POS, FONT_SCALE_VOLTAGE);
	  mVoltLast = mVolt;
  }

  if(adcInput == ADCINPUT_JOYSTICK)
  {
    // Draw directional pad
    drawScreenDpadRow(northwest_bits, north_bits,  northeast_bits, 127-48);
    drawScreenDpadRow(west_bits,      center_bits, east_bits,      127-32);
    drawScreenDpadRow(southwest_bits, south_bits,  southeast_bits, 127-16);
  }
}

//-----------------------------------------------------------------------------
// synchFrame
//-----------------------------------------------------------------------------
//
// Delay until start of next frame
//
void synchFrame(void)
{
  static uint16_t lastTick = 0;
  uint16_t tick;

  // Render at 50 Hz
  while (((tick = GetTickCount()) - lastTick) < HZ_TO_MS(DEMO_FRAME_RATE));
  lastTick = tick;
}

//-----------------------------------------------------------------------------
// dpadReset
//-----------------------------------------------------------------------------
//
// Delay until start of next frame
//
void dpadReset(void)
{
  center_bits = dpad_c_bits;
  north_bits  = dpad_n_bits;
  east_bits   = dpad_e_bits;
  south_bits  = dpad_s_bits;
  west_bits   = dpad_w_bits;
  northwest_bits = dpad_nw_bits;
  northeast_bits = dpad_ne_bits;
  southwest_bits = dpad_sw_bits;
  southeast_bits = dpad_se_bits;
}

//-----------------------------------------------------------------------------
// updateDpad
//-----------------------------------------------------------------------------
//
// Updates dpad image pointers
//
// direction - return value from JOYSTICK_convert_mv_to_direction()
//
void updateDpad(uint8_t direction)
{
  dpadReset();
  switch(direction)
  {
    case JOYSTICK_C:
      center_bits = dpad_bold_c_bits;
      break;
    case JOYSTICK_N:
      north_bits  = dpad_bold_n_bits;
      break;
    case JOYSTICK_E:
      east_bits   = dpad_bold_e_bits;
      break;
    case JOYSTICK_S:
      south_bits  = dpad_bold_s_bits;
      break;
    case JOYSTICK_W:
      west_bits   = dpad_bold_w_bits;
      break;
    case JOYSTICK_NE:
      northeast_bits = dpad_bold_ne_bits;
      break;
    case JOYSTICK_NW:
      northwest_bits = dpad_bold_nw_bits;
      break;
    case JOYSTICK_SE:
      southeast_bits = dpad_bold_se_bits;
      break;
    case JOYSTICK_SW:
      southwest_bits = dpad_bold_sw_bits;
      break;
    case JOYSTICK_NONE:
      break;
    default:
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////
// Interrupt Service Routines
///////////////////////////////////////////////////////////////////////////////

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

  // If the port match event occurred on button press, change ADC input
  // and set updateAdcInput flag so input text will be updated later in main()
  if(BSP_PB0 == BSP_PB_PRESSED)
  {
    adcInput = !adcInput;
    updateAdcInput = UPDATEADCINPUT_YES;
  }

  SFRPAGE = LEGACY_PAGE;
  if (adcInput == ADCINPUT_JOYSTICK)
  {
    ADC0MX = ADC0MX_ADC0MX__ADC0P13; // P1.7
  }
  else if (adcInput == ADCINPUT_EXTERNAL)
  {
    ADC0MX = ADC0MX_ADC0MX__ADC0P7; // P1.1
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

void Voltmeter_main(void)
{
  uint32_t adcVoltage = 0; // in units of 10^-4 V

  // Initial ADC input is joystick
  adcInput = ADCINPUT_JOYSTICK;
  updateAdcInput = UPDATEADCINPUT_YES;

  // Display instructions
  drawScreenText("MOVE JOYSTICK.",       12, FONT_SCALE_DEFAULT);
  drawScreenText("THE JOYSTICK VOLTAGE", 36, FONT_SCALE_DEFAULT);
  drawScreenText("AND DIRECTION WILL",   48, FONT_SCALE_DEFAULT);
  drawScreenText("BE DISPLAYED.",        60, FONT_SCALE_DEFAULT);
  drawScreenText("PRESS BUTTON PB0 TO",  84, FONT_SCALE_DEFAULT);
  drawScreenText("SWITCH ADC INPUT.",    96, FONT_SCALE_DEFAULT);

  // Wait for joystick movement
  while(JOYSTICK_convert_mv_to_direction(ADC_GetVoltage()/10) == JOYSTICK_NONE);
  DISP_ClearAll();

  // Begin demo
  while(1)
  {
    adcVoltage = ADC_GetVoltage();
    updateDpad(JOYSTICK_convert_mv_to_direction(adcVoltage/10));
    drawScreen(adcVoltage);
    synchFrame();

    // Wait for button to be released before re-enabling port match
    // interrupts
    if (BSP_PB0 == BSP_PB_UNPRESSED)
    {
      EIE1 |= 0x02;
    }
  }
}
