//=============================================================================
// lcdPrintf.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// C File Description:
//
//
// Target:
//    C8051F96x
//
// IDE:
//    Silicon Laboratories IDE
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
//    F96x LCD
//
// Release 0.0
//    - TBD
//
//
// This software must be used in accordance with the End User License Agreement.
//
//=============================================================================
//-----------------------------------------------------------------------------
// Includes
//
// This file is written using 8051 compiler independent code.
//-----------------------------------------------------------------------------
#include <stdio.h>                     // This is needed for vsprintf()
#include <stdarg.h>                    // This is needed for the va_list
#include "lcdPrintf.h"
#include "lcdPutString.h"
#include "si_toolchain.h"
//-----------------------------------------------------------------------------
// Function Name
//          lcdPrintf (const char *fmt, ...)
// Parameters   :const char *fmt, variable argument list
// Return Value : none
//
// Description:
//
// This implementation of lcdPrintf may be used to format and send a character
// string to the LCD. This function uses the standard ANSI vsprintf function
// to print to the string. This vsprintf function is ideal for this function
// because it allows a variable argument list to be passed as a parameter.
// The sprintf() function would not work here with a variable argument list.
//
// The vsprintf() function is normally quite large. So inclusion of this
// function is optional.
//
//-----------------------------------------------------------------------------
void lcdPrintf (const char *fmt, ...)
{
   va_list ap;                         // ap is type va_list from stdarg.h

   static SI_SEGMENT_VARIABLE(string[MAX_LCD_STRING], char, SI_SEG_XDATA);

   va_start (ap, fmt);                 //start variable arg list

   vsprintf(string, fmt, ap);

   lcdPutString(string);

   va_end(ap);
}
//=============================================================================
// End of File
//=============================================================================
