//-----------------------------------------------------------------------------
// power.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Driver for the  power management function.
//
// Target:         C8051F96x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//
//
//-----------------------------------------------------------------------------
#ifndef COMPILER_DEFS_H
#endif
//-----------------------------------------------------------------------------
// Register and Bit Definitions
//-----------------------------------------------------------------------------

// Power Management Bit Definitions
#define SLEEP        0x80              // Sleep Mode Select
#define SUSPEND      0x40              // Suspend Mode Select
#define CLEAR        0x20              // Wake-Up Flag Clear
#define RSTWK        0x10              // Reset Pin Falling Edge Wake-Up
#define RTCFWK       0x08              // SmaRTClock Failure Wake-Up
#define RTCAWK       0x04              // SmaRTClock Alarm Wake-Up
#define PMATWK       0x02              // Port Match Wake-Up
#define CPT0WK       0x01              // Comparator0 Wake-Up

// Friendly names for the LPM function arguments
#define PORT_MATCH   PMATWK
#define RTC          RTCFWK + RTCAWK
#define COMPARATOR   CPT0WK

// FLSCL Bit Definition
#define BYPASS    0x40
#define NON_ZERO  0x01



//-----------------------------------------------------------------------------
// Extern Variable Definitions
//-----------------------------------------------------------------------------
extern uint8_t PMU0CF_Local; 

// Define Wake-Up Flags
extern bit RTC_ClockFailWakeUp;
extern bit ComparatorWakeUp;
extern bit PortMatchWakeUp;
extern bit RTC_Alarm0_WakeUp;
extern bit RTC_Alarm1_WakeUp;
extern bit RTC_Alarm2_WakeUp;
extern bit PulseCounterWakeUp;
//-----------------------------------------------------------------------------
// API Function PROTOTYPES 
//-----------------------------------------------------------------------------

void LPM_Init (void);
uint8_t LPM(uint8_t mode);
void LPM_Enable_Wakeup (uint8_t wakeup);
void LPM_Disable_Wakeup (uint8_t wakeup);
