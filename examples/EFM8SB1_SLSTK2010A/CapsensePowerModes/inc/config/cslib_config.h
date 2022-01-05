//-----------------------------------------------------------------------------
// cslib_config.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://www.silabs.com

#ifndef _PROJECT_CONFIG_H
#define _PROJECT_CONFIG_H
#include <si_toolchain.h>

// Sets the number of consecutive samples that must fall above
// the ACTIVE_PERCENTAGE threshold before a channel is characterized
// as active
// -----------------------------------------------------------------------------
// $[Button Debounce]
#define DEF_BUTTON_DEBOUNCE                       2
// [Button Debounce]$

// -----------------------------------------------------------------------------
// These frequency settings assume proper configuration of the timer source.  This
// configuration should be included in HardwareRoutines.c/h
// -----------------------------------------------------------------------------
// $[Mode Periods]
#define DEF_ACTIVE_MODE_PERIOD                    20
#define DEF_SLEEP_MODE_PERIOD                     20
// [Mode Periods]$

// -----------------------------------------------------------------------------
// Timer overflow events, based on ACTIVE_MODE_FREQUENCY
// set this to 1 for free run active mode sensing, 0 for active mode with low power
// -----------------------------------------------------------------------------
// $[Sleep Counts]
#define DEF_COUNTS_BEFORE_SLEEP                   50
// [Sleep Counts]$

// -----------------------------------------------------------------------------
// This setting determines if an active mode algorithm waits in a low power state
// to stall out remaining time before 1/ACTIVE_MODE_FREQUENCY is reached since
// the last scan.  If this is set to 0, the system will continuously scan and
// update variables instead of stalling.  The ACTIVE_MODE_FREQUENCY setting is
// still used to determine the time that must pass with all sensors inactive
// before sleep mode is entered.
// -----------------------------------------------------------------------------
// $[Free Run Setting]
#define DEF_FREE_RUN_SETTING                      1
// [Free Run Setting]$

// -----------------------------------------------------------------------------
// Set this to 1 to enable sleep mode sensing, set this to 0 to always remain in
// active mode
// -----------------------------------------------------------------------------
// $[Sleep Mode]
#define DEF_SLEEP_MODE_ENABLE                     1
// [Sleep Mode]$


void AlgorithmTick(void);

#endif // __SILICON_LABS_CSLIB_CONFIG_H

