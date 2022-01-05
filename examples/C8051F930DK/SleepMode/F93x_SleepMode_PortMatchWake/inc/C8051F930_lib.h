//-----------------------------------------------------------------------------
// C8051F930_lib.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Driver for the 'F93x power management function.
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.2 
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.0
//    -23 JUL 2009
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "smartclock.h"                // RTC Functionality
#include "power.h"                     // Power Management Functionality

//-----------------------------------------------------------------------------
// Global Configuration Options
//-----------------------------------------------------------------------------

#define SYSCLK  20000000

#define SMARTCLOCK_ENABLED   1
