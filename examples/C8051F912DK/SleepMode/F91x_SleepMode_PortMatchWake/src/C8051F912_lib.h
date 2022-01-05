//-----------------------------------------------------------------------------
// C8051F912_lib.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Driver for the 'F91x power management function.
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.0
//    -23 JUL 2009
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "smaRTClock.h"                // RTC Functionality
#include "power.h"                     // Power Management Functionality


//-----------------------------------------------------------------------------
// Global Configuration Options
//-----------------------------------------------------------------------------

#define SYSCLK  20000000

#define SMARTCLOCK_ENABLED   1
