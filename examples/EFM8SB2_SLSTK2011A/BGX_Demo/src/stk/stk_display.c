/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <stdlib.h>
#include <SI_EFM8SB2_Register_Enums.h>
#include <bsp_config.h>
#include <disp.h>
#include <render.h>
#include <bsp.h>
#include <string.h>
#include "stk_display.h"
#include "tick.h"
#include "efm8_config.h"

/**************************************************************************//**
 * This file contains the application specific LCD drawing routines. It is
 * built on top of the efm8_memory_lcd board support library.
 *****************************************************************************/
// Row # where TX Data begins
#define TX_START 4
// Row # where RX Data begins
#define RX_START 9
// Number of rows given for the TX Data log on the LCD
#define NUM_TX_ROWS 3
// Number of rows given for the RX Data log on the LCD
#define NUM_RX_ROWS 7
// Maximum horizontal columns on the LCD
#define MAX_SCREEN_WIDTH 21
// One row of pixels
SI_SEGMENT_VARIABLE(lineBuf[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);
// Text line variables
SI_SEGMENT_VARIABLE(txText[MAX_SCREEN_WIDTH], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(rxText[MAX_SCREEN_WIDTH], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(txPosition, uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(rxPosition, uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(txRow, uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(rxRow, uint8_t, SI_SEG_XDATA);
// Peripheral/Central and Connected/Disconnected states
enum states{P, PtoC, CtoP, none};
enum states previousState = none;
// Device name
extern SI_SEGMENT_VARIABLE(deviceName[15], uint8_t, SI_SEG_XDATA);
/**************************************************************************//**
 * Writes a line of text to the LCD.
 *****************************************************************************/
void STK_DISPLAY_printStr(uint8_t row, uint8_t col, uint8_t* txt)
{
  uint8_t y;
  uint8_t cRow;
  uint8_t cCol;

  cRow = 8*row;
  cCol = 6*col;
  RENDER_ClrLine(lineBuf);

  for (y = 0; y < 8; y++)
  {
    RENDER_StrLine(lineBuf, cCol, y, txt);
    DISP_WriteLine(cRow+y, lineBuf);
  }
}

/**************************************************************************//**
 * Clears a line of text on the LCD.
 *****************************************************************************/
void STK_DISPLAY_clearText(SI_VARIABLE_SEGMENT_POINTER(buffer, uint8_t, SI_SEG_XDATA))
{
  uint8_t i;
  for (i = 0; i < MAX_SCREEN_WIDTH; i++)
  {
    *buffer = 0x00;
    buffer++;
  }
}

/**************************************************************************//**
 * Initializes the LCD display.
 *  .......................
 *  .      BGX-XXXX       .
 *  .     Peripheral      .
 *  .---------------------.
 *  .TX Data:             .
 *  .                     .
 *  .                     .
 *  .---------------------.
 *  .RX Data:             .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .......................
 *****************************************************************************/
void STK_DISPLAY_init()
{
  DISP_Init();
  RENDER_ClrLine(lineBuf);
  txPosition = 0;
  rxPosition = 0;
  txRow      = TX_START;
  rxRow      = RX_START;
  STK_DISPLAY_clearText(txText);
  STK_DISPLAY_clearText(rxText);

  STK_DISPLAY_printStr(0, 6, deviceName);
  STK_DISPLAY_printStr(2, 0, "---------------------");
  STK_DISPLAY_printStr(3, 0, "TX Data:");
  STK_DISPLAY_printStr(7, 0, "---------------------");
  STK_DISPLAY_printStr(8, 0, "RX Data:");
  STK_DISPLAY_clearLog(RX_START);
  STK_DISPLAY_header();
  STK_DISPLAY_printRx((uint8_t)"\n");
}

/**************************************************************************//**
 * Prints the header.
 *  .......................
 *  .      BGX-XXXX       .
 *  .     Peripheral      .
 *  .---------------------.
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .......................
 *****************************************************************************/
void STK_DISPLAY_header()
{
  extern bool mode;

  // if connection pin = disconnected
  if (CONNECTION_PIN == DISCONNECTED)
  {
    if(previousState != P)
    {
      STK_DISPLAY_printStr(1, 5, "Peripheral");
      previousState = P;
    }
  }
  // if connection pin = connected & central was chosen in beginning
  else if(mode == CENTRAL_DEMO)
  {
    if(previousState != CtoP)
    {
      STK_DISPLAY_printStr(1, 1, "Central->Peripheral");
      previousState = CtoP;
      mode = PERIPHERAL_DEMO;
    }
  }
  // if connection pin = connected
  else if (mode == PERIPHERAL_DEMO)
  {
    if(previousState != PtoC && previousState != CtoP)
    {
      STK_DISPLAY_printStr(1, 1, "Peripheral->Central");
      previousState = PtoC;
    }
  }
}

/**************************************************************************//**
 * Initializes the LCD display to let the user choose the BGX mode.
 *  .......................
 *  .       BGX-XXXX      .
 *  .---------------------.
 *  .    CHOOSE A MODE    .
 *  .---------------------.
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .  CENTRAL PERIPHERAL .
 *  .    PB1      PB0     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .......................
 *****************************************************************************/
void STK_DISPLAY_initChooseMode()
{
  DISP_Init();
  RENDER_ClrLine(lineBuf);
  txPosition = 0;
  rxPosition = 0;
  rxRow      = 3;
  STK_DISPLAY_clearText(txText);
  STK_DISPLAY_clearText(rxText);

  STK_DISPLAY_printStr(0, 6, deviceName);
  STK_DISPLAY_printStr(1, 0, "---------------------");
  STK_DISPLAY_printStr(2, 4, "CHOOSE A MODE");
  STK_DISPLAY_printStr(3, 0, "---------------------");
  STK_DISPLAY_printStr(9, 2, "CENTRAL PERIPHERAL");
  STK_DISPLAY_printStr(10, 4, "PB1      PB0");
}

/**************************************************************************//**
 * Initializes the LCD display for the central demo.
 *  .......................
 *  .      BGX-XXXX       .
 *  .---------------------.
 *  .  CONNECT TO A BGX:  .
 *  .---------------------.
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .......................
 *****************************************************************************/
void STK_DISPLAY_initCentral()
{
  DISP_Init();
  RENDER_ClrLine(lineBuf);
  txPosition = 0;
  rxPosition = 0;
  rxRow      = 3;
  STK_DISPLAY_clearText(txText);
  STK_DISPLAY_clearText(rxText);

  STK_DISPLAY_printStr(0, 6, deviceName);
  STK_DISPLAY_printStr(1, 0, "---------------------");
  STK_DISPLAY_printStr(2, 2, "CONNECT TO A BGX:");
  STK_DISPLAY_printStr(3, 0, "---------------------");
}

/**************************************************************************//**
 * Clear the log window for TX or RX log.
 *****************************************************************************/
void STK_DISPLAY_clearLog(uint8_t start)
{
  uint8_t y;
  // Clears RX log
  if (start == RX_START)
  {
    for (y = start*8; y < (start + NUM_RX_ROWS)*8; y++)
    {
      DISP_ClearLine(y, 0x00);
    }
  }
  // Clears TX log
  else
  {
    for (y = start*8; y < (start + NUM_TX_ROWS)*8; y++)
    {
      DISP_ClearLine(y, 0x00);
    }
  }
}

/**************************************************************************//**
 * Increment the TX Data portion to the next line of display.
 *****************************************************************************/
void STK_DISPLAY_newLineTx()
{
  // Increment to next line and clear line buffer
  txRow++;
  txPosition = 0;
  STK_DISPLAY_clearText(txText);

  // If we rollover clear line and
  if (txRow > TX_START + NUM_TX_ROWS)
  {
    STK_DISPLAY_clearLog(TX_START);
    txRow = TX_START;
  }
}

/**************************************************************************//**
 * Increment the RX Data portion to the next line of display.
 *****************************************************************************/
void STK_DISPLAY_newLineRx()
{
  // Increment to next line and clear line buffer
  rxRow++;
  rxPosition = 0;
  STK_DISPLAY_clearText(rxText);

  // If we rollover clear line and
  if (rxRow > RX_START + NUM_RX_ROWS)
  {
    STK_DISPLAY_clearLog(RX_START);
    rxRow = RX_START;
  }
}

/**************************************************************************//**
 * Print a new character to the TX Data portion of the LCD.
 *
 * @param val is the character to be printed
 *****************************************************************************/
void STK_DISPLAY_printTx(uint8_t val)
{
  if (txRow == TX_START + NUM_TX_ROWS)
  {
    STK_DISPLAY_newLineTx();
  }
  // If CR/LF then go to next line
  if (val == 0x0D || val == 0x0A)
  {
    STK_DISPLAY_newLineTx();
    return;
  }

  // Print character
  txText[txPosition] = val;
  txPosition++;
  STK_DISPLAY_printStr(txRow, 0, txText);

  // If we are out of space on this line increment to the next line.
  if (txPosition >= MAX_SCREEN_WIDTH)
  {
    STK_DISPLAY_newLineTx();
  }
}

/**************************************************************************//**
 * Print a new character to the RX Data portion of the LCD.
 *
 * @param val is the character to be printed
 *
 * The data printed is what the STK receives. For demo purposes, the
 * BGX Commander app is used to send data to the STK. This mobile app
 * has a default CRLF stream line ending. To avoid creating 2 new lines
 * for each stream received, LF creates a new line, and CR is ignored.
 *****************************************************************************/
void STK_DISPLAY_printRx(uint8_t val)
{
  // If LF then go to next line
  if (val == 0x0A)
  {
    STK_DISPLAY_newLineRx();
    return;
  }
  // If CR then ignore
  else if (val == 0x0D)
  {
    return;
  }

  if (rxRow == RX_START + NUM_RX_ROWS)
  {
    STK_DISPLAY_newLineRx();
  }

  // Print character
  rxText[rxPosition] = val;
  rxPosition++;
  STK_DISPLAY_printStr(rxRow, 0, rxText);

  // If we are out of space on this line increment to the next line.
  if (rxPosition >= MAX_SCREEN_WIDTH)
  {
    STK_DISPLAY_newLineRx();
  }
}

/**************************************************************************//**
 *  Displays the scan loading screen on the LCD.
 *  .......................
 *  .      BGX-XXXX       .
 *  .---------------------.
 *  .  CONNECT TO A BGX:  .
 *  .---------------------.
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .      Scanning       .
 *  .      _              .
 *  .                     .
 *  .                     .
 *  .                     .
 *  . Press PB0 or PB1 to .
 *  .   see BGX devices   .
 *  .......................
 *****************************************************************************/
void STK_DISPLAY_scanning()
{
  uint8_t i = 0;

  STK_DISPLAY_clearLog(6);
  STK_DISPLAY_clearLog(8);
  STK_DISPLAY_clearLog(13);
  STK_DISPLAY_clearLog(14);
  STK_DISPLAY_printStr(8, 6, "Scanning");
  STK_DISPLAY_printStr(13, 1, "Press PB0 or PB1 to");
  STK_DISPLAY_printStr(14, 3, "see BGX devices");

  // Display loading screen until a button is pressed
  while (BSP_PB1 != BSP_PB_PRESSED && BSP_PB0 != BSP_PB_PRESSED)
  {
    if (i < 8)
    {
      STK_DISPLAY_printStr(9, i + 6, "-");
      Wait(100);
      i++;
    }
    else
    {
      i = 0;
    }
  }

  STK_DISPLAY_clearLog(6);
  STK_DISPLAY_clearLog(8);
  STK_DISPLAY_clearLog(13);
  STK_DISPLAY_clearLog(14);
}

/**************************************************************************//**
 * Initializes the select device screen.
 *  .......................
 *  .       BGX-XXXX      .
 *  .---------------------.
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .   Use joystick to   .
 *  .   select device #   .
 *  .         1           .
 *  .......................
 *****************************************************************************/
void STK_DISPLAY_selectDevice()
{
  STK_DISPLAY_printStr(11, 3, "Use joystick to");
  STK_DISPLAY_printStr(12, 3, "select device #");
  STK_DISPLAY_printStr(14, 10, "1");
}
