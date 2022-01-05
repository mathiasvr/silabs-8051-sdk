/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef INC_STK_DISPLAY_H_
#define INC_STK_DISPLAY_H_

/**************************************************************************//**
 * Writes a line of text to the LCD.
 *****************************************************************************/
void STK_DISPLAY_printStr(uint8_t row, uint8_t col, uint8_t* txt);

/**************************************************************************//**
 * Clears a line of text on the LCD.
 *****************************************************************************/
void STK_DISPLAY_clearText(SI_VARIABLE_SEGMENT_POINTER(buffer, uint8_t, SI_SEG_XDATA));

/**************************************************************************//**
 * Initializes the LCD display.
 *  .......................
 *  .      BGX DEMO       .
 *  .---------------------.
 *  .TX Data:             .
 *  .                     .
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
void STK_DISPLAY_init();

void STK_DISPLAY_header();

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
void STK_DISPLAY_initChooseMode();

/**************************************************************************//**
 * Initializes the LCD display for the central demo.
 *  .......................
 *  .  CONNECT TO A BGX:  .
 *  .---------------------.
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .      Scanning       .
 *  .      _              .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .  Press any button.  .
 *  .                     .
 *  .......................
 *****************************************************************************/
void STK_DISPLAY_initCentral();

/**************************************************************************//**
 * Clear the log window for TX or RX log.
 *****************************************************************************/
void STK_DISPLAY_clearLog(uint8_t start);

/**************************************************************************//**
 * Increment the TX Data portion to the next line of display.
 *****************************************************************************/
void STK_DISPLAY_newLineTx();

/**************************************************************************//**
 * Increment the RX Data portion to the next line of display.
 *****************************************************************************/
void STK_DISPLAY_newLineRx();

/**************************************************************************//**
 * Print a new character to the TX Data portion of the LCD.
 *
 * @param val is the character to be printed
 *****************************************************************************/
void STK_DISPLAY_printTx(uint8_t val);

/**************************************************************************//**
 * Print a new character to the RX Data portion of the LCD.
 *
 * @param val is the character to be printed
 *
 * The data printed is what the UB2 STK receives. For demo purposes, the
 * BGX Commander app is used to send data to the UB2 STK. This mobile app
 * has a default CRLF stream line ending. To avoid creating 2 new lines
 * for each stream received, LF creates a new line, and CR is ignored.
 *****************************************************************************/
void STK_DISPLAY_printRx(uint8_t val);

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
void STK_DISPLAY_scanning();

/**************************************************************************//**
 * Initializes the select device screen.
 *  .......................
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
 *  .   Use joystick to   .
 *  .   select device #   .
 *  .         1           .
 *  .......................
 *****************************************************************************/
void STK_DISPLAY_selectDevice();

#endif /* INC_STK_DISPLAY_H_ */
