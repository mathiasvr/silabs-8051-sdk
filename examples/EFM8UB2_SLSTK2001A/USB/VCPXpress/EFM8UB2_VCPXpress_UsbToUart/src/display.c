/******************************************************************************
 * Copyright (c) 2014 by Silicon Laboratories Inc. All rights reserved.
 *
 * @section License
 * (C) Copyright 2014 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "SI_EFM8UB2_Register_Enums.h"
#include "disp.h"
#include "render.h"
#include "display.h"

/**************************************************************************//**
 *
 * This file contains the application specific LCD drawing routines. It is
 * built on top of the efm8_memory_lcd board support library
 *
 *****************************************************************************/

#define TX_START 3
#define RX_START 10

//One row of pixels
SI_SEGMENT_VARIABLE(lineBuf[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

//Text line variables
SI_SEGMENT_VARIABLE(txText[21], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(rxText[21], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(txPosition, uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(rxPosition, uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(txRow, uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(rxRow, uint8_t, SI_SEG_XDATA);

//Write line of text
void printStr(uint8_t row, uint8_t col, uint8_t* txt)
{
	uint8_t y;
	uint8_t cRow;
	uint8_t cCol;

	cRow = 8*row;
	cCol = 6*col;

  EIE1 &= ~EIE1_EUSB0__BMASK;
	RENDER_ClrLine(lineBuf);

  for (y = 0; y < 8; y++)
	{
  	RENDER_StrLine(lineBuf, cCol, y, txt);
		DISP_WriteLine(cRow+y, lineBuf);
	}
  EIE1 |= EIE1_EUSB0__BMASK;
}

//Clear a line of text
void clearText(SI_VARIABLE_SEGMENT_POINTER(buffer, uint8_t, SI_SEG_XDATA))
{
	uint8_t i;
	for(i=0; i<21; i++){
		*buffer = 0x00;
		buffer++;
	}
}

//Initialize the display
void initDisplay()
{
  DISP_Init();
  RENDER_ClrLine(lineBuf);
  txPosition = 0;
  rxPosition = 0;
  txRow      = TX_START;
  rxRow      = RX_START;
	clearText(txText);
	clearText(rxText);


  printStr(0, 2, "Usb <--> Uart Demo");
  printStr(1, 0, "---------------------");
  printStr(2, 0, "TX Data:");
  printStr(8, 0, "---------------------");
  printStr(9, 0, "RX Data:");
}

//Clear the log window for tx or rx log.
void clearLog(uint8_t start)
{
	uint8_t y;

  for (y = start*8; y < (start+5)*8; y++)
	{
		DISP_ClearLine(y, 0x00);
	}
}

//Increment Tx to next line of display
void newLineTx()
{
	//Increment to next line and clear line buffer
  txRow++;
	txPosition = 0;
	clearText(txText);

  //If we rollover clear line and
	if(txRow > TX_START + 4)
	{
		clearLog(TX_START);
		txRow = TX_START;
	}
}

//Increment Tx to next line of display
void newLineRx()
{
	//Increment to next line and clear line buffer
  rxRow++;
	rxPosition = 0;
	clearText(rxText);

  //If we rollover clear line and
	if(rxRow > RX_START + 4)
	{
		clearLog(RX_START);
		rxRow = RX_START;
	}
}

//Print a new character to the Tx Display
void printTx(uint8_t val)
{
	//if CR then go to next line
	if(val == 0x0D)
	{
		newLineTx();
		return;
	}

	//print character and
	txText[txPosition] = val;
	txPosition++;
  printStr(txRow, 0, txText);

  //If we are out of space on this line increment to the next line.
	if(txPosition > 19)
	{
		newLineTx();
	}
}

//Print a new character to the Tx Display
void printRx(uint8_t val)
{
	//if CR then go to next line
	if(val == 0x0D)
	{
		newLineRx();
		return;
	}

	//print character and
	rxText[rxPosition] = val;
	rxPosition++;
  printStr(rxRow, 0, rxText);

  //If we are out of space on this line increment to the next line.
	if(rxPosition > 19)
	{
		newLineRx();
	}
}
