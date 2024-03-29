//-----------------------------------------------------------------------------
// power.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Driver for the 'F91x power management function.
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
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
#include <SI_C8051F912_Register_Enums.h>
#include "C8051F912_lib.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Variables used for the RTC interface
uint8_t PMU0CF_Local;                       // Holds the desired Wake-up sources
 
// Define Wake-Up Flags
uint8_t RTC_Alarm;
uint8_t RTC_Failure;
uint8_t Comparator_Wakeup;
uint8_t Port_Match_Wakeup;

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void LPM(uint8_t mode);
void LPM_Init(void);
void LPM_Enable_Wakeup (uint8_t wakeup);
void LPM_Disable_Wakeup (uint8_t wakeup);


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
  PMU0CF = CLEAR;
  PMU0CF_Local = 0;
  RTC_Alarm = 0;
  RTC_Failure = 0;
  Comparator_Wakeup = 0;
  Port_Match_Wakeup = 0;
  P2MDOUT |= 0x01;
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
void LPM(uint8_t mode)
{
   uint8_t b;
   uint8_t CLKSEL_save;
   uint8_t EA_save;
   uint8_t PMU0CF_snapshot;
   uint8_t RTC0CN_snapshot;
   
   // Save current interrupt state and disable interrupts 
   EA_save = IE_EA;
   IE_EA = 0;
   
   // Save current system clock selection and select the low power oscillator
   // divided by 2 as the system clock
   CLKSEL_save = CLKSEL;
   CLKSEL = 0x14;
   
   // Enable the Flash read one-shot timer   
   FLSCL &= ~BYPASS;                   // Clear the one-shot bypass bit
   FLWR  =   NON_ZERO;                 // Write a "dummy" value to FLWR
   
   // Verify that the system clock setting has been applied
   while(!(CLKSEL & 0x80));            // Wait until CLKRDY -> 1
   

   //----------------------------------
   // ~~~ Device in Low Power Mode ~~~
   //
       PMU0CF = (mode | PMU0CF_Local);
   //
   // ~~~   Device is now Awake    ~~~
   //----------------------------------
   

   // Restore the System Clock
   CLKSEL = CLKSEL_save;
   
   // Disable (Bypass) the Flash Read one-shot timer if the system clock
   // frequency is higher than 10 MHz
   if(SYSCLK > 10000000)
   {
      FLSCL |= BYPASS;                 // Set the one-shot bypass bit                     
   }
  
   // Capture the wake-up source and clear all wake-up source flags   
   PMU0CF_snapshot = PMU0CF;
   PMU0CF = CLEAR;

   // Capture RTC events that occured while PMU0CF was being cleared
   RTC0CN_snapshot = RTC_Read(RTC0CN); 

   // If an RTC Alarm occured while PMU0CF was being cleared, clear 
   // the PCU0CF flag again   
   if(RTC0CN_snapshot & (ALRM) && ((PMU0CF_snapshot & RTCAWK) == 0))
   {
      PMU0CF_snapshot |= PMU0CF;
      PMU0CF = CLEAR;
   }

   //----------------------------------
   // Decode Wake-Up Sources
   //----------------------------------

   // Check for an RTC Alarm
   if((PMU0CF_snapshot & RTCAWK) || (RTC0CN_snapshot & ALRM))
   {
      RTC_Alarm = 1;
   }
   // Check for an RTC Clock Failure
   if((PMU0CF_snapshot & RTCFWK) || (RTC0CN_snapshot & OSCFAIL))
   {
      RTC_Failure = 1;  
   }
   // Check for a Port Match Wakeup
   if(PMU0CF_snapshot & PMATWK)
   {
      Port_Match_Wakeup = 1;  
   }
   // Check for a Comparator Wakeup
   if(PMU0CF_snapshot & CPT0WK)
   {
      Comparator_Wakeup = 1;  
   }
   
   // Restore Interrupt State
   IE_EA = EA_save;
   
   // Check for a reset pin Wakeup
   if(PMU0CF_snapshot & RSTWK)
   {
      // Delay 15uS per datasheet recommendation
      for(b = 255; b > 0; b--);
   }
    
}               

 
