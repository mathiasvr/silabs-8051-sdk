#ifndef  LCD_PUT_STRING_H
#define  LCD_PUT_STRING_H
//=============================================================================
// lcdPutString.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header File Description:
//
// Target:
//    C8051F96x.
//
// IDE:
//    Silicon Laboratories IDE
//
// Tool Chains:
//    Keil
//    SDCC
//    Raisonance
//
// Project Name:
//    C8051F96x LCD Example
//
// Release 0.1
//    - TBD
//
// This software must be used in accordance with the End User License Agreement.
//
//=============================================================================
#define  AUTO_CLEAR_DISPLAY
//-----------------------------------------------------------------------------
// Public function prototypes (API)
//-----------------------------------------------------------------------------
void lcdPutString (char * string);
//-----------------------------------------------------------------------------
// lcdPutChar() prototype
//
// The lcdPutChar() function is specific to the particular LCD display and
// the pin connections. A prototype is included here to keep this file
// hardware generic. The function lcdPutChar() must be defined elsewhere
// or a linker error will occur.
//-----------------------------------------------------------------------------
void lcdPutChar (char c);
//=============================================================================
// end LCD_PUTS_H
//=============================================================================
#endif //LCD_PUT_STRING_H