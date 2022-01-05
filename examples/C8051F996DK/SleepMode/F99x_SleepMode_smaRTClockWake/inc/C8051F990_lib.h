//-----------------------------------------------------------------------------
// C8051F990_lib.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Driver for the power management function.
//
// Target:         C8051F99x-C8051F98x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
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