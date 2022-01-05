/**************************************************************************//**
 * Copyright (c) 2021 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// logic.c
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "tick.h"
#include "disp.h"
#include "render.h"
#include "joystick.h"
#include "logic.h"
#include <SI_EFM8BB52_Register_Enums.h>
#include "thinfont.h"
#include "and_gate.h"
#include "nand_gate.h"
#include "or_gate.h"
#include "nor_gate.h"
#include "xor_gate.h"
#include "xnor_gate.h"

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////

// Generic line buffer
SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

// State labels for gate inputs and output
char outputState[2] = "1";
char input1State[2] = "1";
char input2State[2] = "1";

#define ROWS 4
#define COLS 12

typedef SI_SEGMENT_VARIABLE(, char, const SI_SEG_CODE) codeStr_t;
typedef SI_SEGMENT_VARIABLE(truthTable_t[ROWS][COLS], char, const SI_SEG_CODE);

// PB1 PB0  LED",
truthTable_t truthTableAND = {
  " 0   0    0",
  " 0   1    0",
  " 1   0    0",
  " 1   1    1"};

truthTable_t truthTableNAND = {
  " 0   0    1",
  " 0   1    1",
  " 1   0    1",
  " 1   1    0"};

truthTable_t truthTableOR = {
  " 0   0    0",
  " 0   1    1",
  " 1   0    1",
  " 1   1    1"};

truthTable_t truthTableNOR = {
  " 0   0    1",
  " 0   1    0",
  " 1   0    0",
  " 1   1    0"};

truthTable_t truthTableXOR = {
  " 0   0    0",
  " 0   1    1",
  " 1   0    1",
  " 1   1    0"};

truthTable_t truthTableXNOR = {
  " 0   0    1",
  " 0   1    0",
  " 1   0    0",
  " 1   1    1"};

// Demo state variables
uint8_t currentLut = LUT_AND;  // current look up table
SI_VARIABLE_SEGMENT_POINTER(currentSymbol, uint8_t, const SI_SEG_CODE) = and_gate_bits; // current gate picture
codeStr_t (*currentTruthTable)[COLS] = truthTableAND; // current truth table

///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// transitionDemoState
//-----------------------------------------------------------------------------
//
// Transitions demo to new state (new logic gate). State transition order:
//   AND -> NAND -> OR -> NOR -> XOR -> XNOR -> AND ...
//
// Updates pointer to gate symbol picture. Update CLU register to implement
// new logic look up table.
//
void transitionDemoState(void)
{
  switch(currentLut)
  {
	case LUT_AND:
	  currentLut = LUT_NAND;
	  currentSymbol = nand_gate_bits;
	  currentTruthTable = truthTableNAND;
	  break;
	case LUT_NAND:
	  currentLut = LUT_OR;
	  currentSymbol = or_gate_bits;
	  currentTruthTable = truthTableOR;
	  break;
	case LUT_OR:
	  currentLut = LUT_NOR;
	  currentSymbol = nor_gate_bits;
	  currentTruthTable = truthTableNOR;
	  break;
	case LUT_NOR:
	  currentLut = LUT_XOR;
	  currentSymbol = xor_gate_bits;
	  currentTruthTable = truthTableXOR;
	  break;
	case LUT_XOR:
	  currentLut = LUT_XNOR;
	  currentSymbol = xnor_gate_bits;
	  currentTruthTable = truthTableXNOR;
	  break;
	case LUT_XNOR:
	  currentLut = LUT_AND;
	  currentSymbol = and_gate_bits;
	  currentTruthTable = truthTableAND;
	  break;
	default:
	  while(1);
  }

  SFRPAGE = 0x20;
  CLU0FN = currentLut;
  SFRPAGE = 0x00;
}

//-----------------------------------------------------------------------------
// readJoystick
//-----------------------------------------------------------------------------
//
// Obtain new ADC sample and returns joystick direction. Valid return values:
//  JOYSTICK_NONE   JOYSTICK_N   JOYSTICK_S
//  JOYSTICK_C      JOYSTICK_E   JOYSTICK_W
//
uint8_t readJoystick(void)
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
// inputJoystick
//-----------------------------------------------------------------------------
//
// Get joystick input. If joystick was moved in any direction and released,
// transition demo to new state (show new logic gate).
//
void inputJoystick(void)
{
  uint8_t dir;

  dir = readJoystick();

  // if joystick is pressed any direction
  if (dir != JOYSTICK_NONE)
  {
    // wait for release then transition
    while(dir != JOYSTICK_NONE)
    {
      dir = readJoystick();
    }
    transitionDemoState();
  }
}

//-----------------------------------------------------------------------------
// updateInputStateLabels
//-----------------------------------------------------------------------------
//
// Update the input state labels. Called by main().
// Output state label is updated in the CLU interrupt.
//
void updateInputStateLabels (void)
{
  logic_writeState(INPUT2, (uint8_t) BSP_PB0);
  logic_writeState(INPUT1, (uint8_t) BSP_PB1);
}

//-----------------------------------------------------------------------------
// addLabel
//-----------------------------------------------------------------------------
//
// Adds one row of an 8-bit tall text label to the line buffer.
// Does not clear the line buffer. Does not draw to the screen.
// To draw to screen, call DISP_WriteLine() after addLabel().
//
// str - pointer to string text (0 - 21 characters) to display
// x   - starting column position
// y   - row of the 8-bit tall text label
//
void addLabel(SI_VARIABLE_SEGMENT_POINTER(str, char, RENDER_STR_SEG), uint8_t x, uint8_t y)
{
  RENDER_StrLine(Line, x, y, str);
}

//-----------------------------------------------------------------------------
// drawScreen
//-----------------------------------------------------------------------------
//
// Displays gate symbol, input/output name labels, and input/output state
// labels on the screen. Picture is drawn one row at a time.
//
// gate_bits - pointer to gate picture
// rowNum - beginning row number on the screen (0 - 127) for the gate picture
//
void drawScreen (SI_VARIABLE_SEGMENT_POINTER(gate_bits, uint8_t, RENDER_SPRITE_SEG), uint8_t rowNum)
{
  uint8_t i;       // row index for screen
  uint8_t x_name;  // row index for name labels
  uint8_t y_name;  // col index for name labels
  uint8_t x_state; // row index for state labels
  uint8_t y_state; // col index for state labels
  uint8_t picWidth = and_gate_width;
  uint8_t picHieght = and_gate_height;

  for (i = 0; i < picHieght; i++)
  {
    // Draw gate symbol
    RENDER_ClrLine(Line);
    RENDER_SpriteLine(Line, X_CENTER - picWidth/2, i, gate_bits, picWidth);

    // Draw input and output name and state labels
    if (i >= INPUT1_LABEL_ROW && i < (INPUT1_LABEL_ROW + FONT_HEIGHT))
    {
        if (i == INPUT1_LABEL_ROW)
        {
          x_name = INPUT_NAME_LABEL_COL;
          y_name = 0;

          x_state = INPUT_STATE_LABEL_COL;
          y_state = 0;
        }

        // Input 1 name label
        addLabel("PB1", x_name, y_name);
        y_name++;

        // Input 1 state label
        addLabel((SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))logic_readState(INPUT1), x_state, y_state);
        y_state++;
    }
    else if (i >= OUTPUT_LABEL_ROW && i < (OUTPUT_LABEL_ROW + FONT_HEIGHT))
    {
      if (i == OUTPUT_LABEL_ROW)
      {
        x_name = OUTPUT_NAME_LABEL_COL;
        y_name = 0;

        x_state = OUTPUT_STATE_LABEL_COL;
        y_state = 0;
      }

      // Output name label
      addLabel("OUT", x_name, y_name);
      y_name++;

      // Output state label
      addLabel((SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))logic_readState(OUTPUT), x_state, y_state);
      y_state++;
    }
    else if (i >= INPUT2_LABEL_ROW && i < (INPUT2_LABEL_ROW + FONT_HEIGHT))
    {
      if (i == INPUT2_LABEL_ROW)
      {
        x_name = INPUT_NAME_LABEL_COL;
        y_name = 0;

        x_state = INPUT_STATE_LABEL_COL;
        y_state = 0;
      }

      // Input 2 name label
      addLabel("PB0", x_name, y_name);
      y_name++;

      // Input 2 state label
      addLabel((SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))logic_readState(INPUT2), x_state, y_state);
      y_state++;
    }

    DISP_WriteLine(rowNum + i, Line);
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
    RENDER_Large_StrLine(Line, 28, i, str, fontScale);

    for (j = 1; j < fontScale + 1; j++)
    {
      DISP_WriteLine(rowNum + (i * fontScale) + j, Line);
    }
  }
}

//-----------------------------------------------------------------------------
// drawTruthTable
//-----------------------------------------------------------------------------
//
// Show truth table on the screen
//
void drawTruthTable(void)
{
  uint8_t truthTableindex = 0;
  uint8_t y_index;

  drawScreenText("PB1 PB0  OUT", 80, FONT_SCALE_DEFAULT);

  for (y_index = 90; y_index <= 120; y_index+=10)
  {
    drawScreenText((SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))currentTruthTable[truthTableindex], y_index, FONT_SCALE_DEFAULT);
	  truthTableindex++;
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
  static uint16_t last_tick = 0;
  uint16_t tick;

  // Render at 50 Hz
  while (((tick = GetTickCount()) - last_tick) < HZ_TO_MS(DEMO_FRAME_RATE));
  last_tick = tick;
}


///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// logic_readState
//-----------------------------------------------------------------------------
//
// Returns pointer to state label. Used by addLabel()
//
// io - specifies input/output to retrieve. Valid arguments are:
//      OUTPUT, INPUT1, INPUT2
//
char * logic_readState(uint8_t io)
{
  char * retval;

  switch (io)
  {
    case OUTPUT:
      retval = outputState;
      break;

    case INPUT1:
      retval = input1State;
      break;

    case INPUT2:
      retval = input2State;
      break;

    default:
      while(1);
  }
  return retval;
}

//-----------------------------------------------------------------------------
// logic_writeState
//-----------------------------------------------------------------------------
//
// Update the state labels
//
// io    - specifies input/output to update. Valid arguments are:
//         OUTPUT, INPUT1, INPUT2
// state - hex value 0x00 or 0x01 to indicate the input's or ouput's state
//
void logic_writeState(uint8_t io, uint8_t state)
{
  // convert hex to string
  if (state == 0)
    state = '0';
  else if (state == 1)
    state = '1';

  // update state label variables
  switch (io)
  {
    case OUTPUT:
      outputState[0] = state;
      break;

    case INPUT1:
      input1State[0] = state;
      break;

    case INPUT2:
      input2State[0] = state;
      break;
  }
}

//-----------------------------------------------------------------------------
// logic_main
//-----------------------------------------------------------------------------
//
// Main driver function containing demo loop
//
void logic_main(void)
{
  while(1)
  {
    inputJoystick();
    updateInputStateLabels();
    drawScreen(currentSymbol, 12);
    drawTruthTable();
    synchFrame();
  }
}
