//-----------------------------------------------------------------------------
// F97x_SmaRTClock_Single_Alarm_Suspend.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to use F97x RTC timer without auto reset
// function. CPU will enter suspend mode and wake up when RTC Alarm is matched
// and LED15 will be turned on finally.
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
// 5) LED15 will be turn on when RTC Alarm (10 secondes) is matched.
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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

#include "F97x_SmaRTClock.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
SI_SBIT(LED15, P1, 5);                       // LED=1 means ON

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
void main (void)
{
   enter_DefaultMode_from_RESET();

   while(1);
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
   RTC_Write(RTC0CN, RTC0CN_RTC0EN__DISABLED |  // Disable RTC
                     RTC0CN_RTC0TR__STOP |      // Stop RTC Timer
                     RTC0CN_RTC0AEN__DISABLED); // Clear flag and disable Alarm

   // Turn on Alarm LED
   LED15 = 0;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
