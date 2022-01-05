//-----------------------------------------------------------------------------
// F97x_SmaRTClock_Periodic_Alarm.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to use F97x RTC timer with auto reset
// function. LED15 will be toggled once RTC Alarm is matched.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) LED15 will be toggled periodically at <RTC_ALARM_DELAY> period.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 09 MAY 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"
#include "F97x_SmaRTClock.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
SI_SBIT(LED15, P1, 5);                     // LED=1 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(RTC_Alarm_ISR, RTC0ALARM_IRQn);

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main(void)
{

  enter_DefaultMode_from_RESET();

  // Alarm LED turned off
  LED15 = 1;

  while (1)
    ;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// RTC_Alarm_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED when the RTC Alarm is matched.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(RTC_Alarm_ISR, RTC0ALARM_IRQn)
{
  // Clear flag so that ISR is not called multiple times
  RTC_Write(RTC0CN, RTC0CN_RTC0EN__ENABLED |   // Keep RTC enabled
      RTC0CN_RTC0TR__RUN |       // Start RTC Timer
      RTC0CN_RTC0AEN__DISABLED); // Clear flag

  // Set flag to auto-reload ALARM value
  RTC_Write(RTC0CN, RTC0CN_RTC0EN__ENABLED |   // Keep RTC enabled
      RTC0CN_RTC0TR__RUN |       // Start RTC Timer
      RTC0CN_RTC0AEN__ENABLED |  // Alarm enabled
      RTC0CN_ALRM__SET);         // Auto-reload ALARM value

  // Toggle Alarm LED
  LED15 = !LED15;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
