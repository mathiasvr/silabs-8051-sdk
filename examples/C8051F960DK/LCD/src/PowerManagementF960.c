//-----------------------------------------------------------------------------
// power.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Driver for the power management function.
//
// Target:         C8051F96x
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
#include <SI_C8051F960_Register_Enums.h>
#include "SmaRTClockF960.h"
#include "PowerManagementF960.h"
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Variables used for the RTC interface
uint8_t PMU0CF_Local;                       // Holds the desired Wake-up sources

// Define Wake-Up Flags
bit RTC_ClockFailWakeUp;
bit ComparatorWakeUp;
bit PortMatchWakeUp;
bit RTC_Alarm0_WakeUp;
bit RTC_Alarm1_WakeUp;
bit RTC_Alarm2_WakeUp;
bit PulseCounterWakeUp;

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// LPM_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the low power functionality
//
//-----------------------------------------------------------------------------
void LPM_Init (void)
{
  SFRPAGE = LEGACY_PAGE;
  PMU0CF = CLEAR;
  PMU0FL = 0;
  PMU0CF_Local = 0;
  RTC_Alarm0_WakeUp = 0;
  RTC_Alarm1_WakeUp = 0;
  RTC_Alarm2_WakeUp = 0;
  RTC_ClockFailWakeUp = 0;
  ComparatorWakeUp = 0;
  PortMatchWakeUp = 0;
}

//-----------------------------------------------------------------------------
// LPM_Enable_Wakeup ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will enable wakeup sources
//
//-----------------------------------------------------------------------------
void LPM_Enable_Wakeup (uint8_t wakeup)
{
  PMU0CF_Local |= (wakeup & (RTC | PORT_MATCH | COMPARATOR));
}

//-----------------------------------------------------------------------------
// LPM_Disable_Wakeup ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will disable wakeup sources
//
//-----------------------------------------------------------------------------
void LPM_Disable_Wakeup (uint8_t wakeup)
{
  PMU0CF_Local &= ~(wakeup & (RTC | PORT_MATCH | COMPARATOR));
}


//-----------------------------------------------------------------------------
// LPM
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint8_t mode -- can be SLEEP or SUSPEND
//
// This function places the device in a low power mode
//
//-----------------------------------------------------------------------------
uint8_t LPM(uint8_t mode)
{
   uint8_t d;
   uint8_t EA_save;
   uint8_t PMU0CF_snapshot;
   uint8_t RTC0CN1_snapshot;

   // Save current interrupt state and disable interrupts
   EA_save = IE_EA;
   IE_EA = 0;

   SFRPAGE = LEGACY_PAGE;

   //return an error if CLKSEL is external clock or RTC clock.
   if(((CLKSEL&0x07)==0x01)||((CLKSEL&0x07)==0x03))
      return 0xFF;


   //----------------------------------
   // ~~~ Device in Low Power Mode ~~~
   //
       PMU0CF = (mode | PMU0CF_Local);
   //
   // ~~~   Device is now Awake    ~~~
   //----------------------------------

   // Capture the wake-up source and clear all wake-up source flags
   PMU0CF_snapshot = PMU0CF;
   PMU0CF = CLEAR;

   // RTC missing event code not needed on F960
	// Capture RTC events that occured while PMU0CF was being cleared
   // RTC0CN1_snapshot = RTC_Read(RTC0CN1);

   // If an RTC Alarm occured while PMU0CF was being cleared, clear
   // the PCU0CF flag again
   // if(((RTC0CN1_snapshot & 0x70)!=0) && ((PMU0CF_snapshot & RTCAWK) == 0))
   // {
   //   PMU0CF_snapshot |= PMU0CF;
   //   PMU0CF = CLEAR;
   // }

   //----------------------------------
   // Decode Wake-Up Sources
   //----------------------------------

   // Check for an RTC Alarm
   if(PMU0CF_snapshot & RTCAWK)
   {
      RTC0CN1_snapshot = RTC_Read(RTC0CF);
		
		if((RTC0CN1_snapshot&0x10)==0x10)
      {
         RTC_Alarm0_WakeUp = 1;
      }
      else if((RTC0CN1_snapshot&0x20)==0x20)
      {
         RTC_Alarm1_WakeUp = 1;
      }
      else if((RTC0CN1_snapshot&0x40)==0x40)
      {
         RTC_Alarm2_WakeUp = 1;
      }
   }
   // Check for an RTC Clock Failure
   if((PMU0CF_snapshot & RTCFWK) || (RTC0CN & OSCFAIL))
   {
      RTC_ClockFailWakeUp = 1;
   }
   // Check for a Port Match Wakeup
   if(PMU0CF_snapshot & PMATWK)
   {
      PortMatchWakeUp = 1;
   }
   // Check for a Comparator Wakeup
   if(PMU0CF_snapshot & CPT0WK)
   {
      ComparatorWakeUp = 1;
   }

   // Check for a Comparator Wakeup
   if((PMU0FL & 0x01)==0x01)
   {
      PulseCounterWakeUp = 1;
   }

   // Restore Interrupt State
   IE_EA = EA_save;

   // Check for a reset pin Wakeup
   if(PMU0CF_snapshot & RSTWK)
   {
      // Delay 15uS per datasheet recommendation
      for(d = 255; d > 0; d--);
   }

   return 0;
}

