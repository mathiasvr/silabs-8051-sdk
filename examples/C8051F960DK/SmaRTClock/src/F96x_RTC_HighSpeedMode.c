//=============================================================================
// F96x_RTC_HighSpeedMode.c
//=============================================================================
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the use of the F960's RTC High Speed Mode to
// write/read the RTC timer and RTC alarm value. This mode allows the registers
// to be accessed immediately, without having to wait one RTC clock cycle for 
// the registers to be updated.
//
// This simple test code will hang just after the respective test
// if any test fails. If all tests pass, it will run to the end of the
// program and turn on LED1.
//
// How To Test:
//
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Connect the USB Debug Adapter to J13.
// 3) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 4) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 5) Connect a 9V power adapter to P1 of the development board.
// 6) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) LED1 should be on if the RTC fast access writes and reads were
//    successful.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Updated Description / How to Test
//    - Added an LED indicator for success/failure.
//    - 27 JAN 2014
//
//=============================================================================
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F960_Register_Enums.h>
#include "SmaRTClockF960.h"

//-----------------------------------------------------------------------------
// Define Hardware
//-----------------------------------------------------------------------------
#define UDP_F960_MCU_MUX_LCD
//#define UDP_F960_MCU_EMIF
//-----------------------------------------------------------------------------
// Hardware Dependent definitions
//-----------------------------------------------------------------------------
#ifdef UDP_F960_MCU_MUX_LCD
SI_SBIT(LED1, SFR_P0, 0);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(LED1, SFR_P3, 0);
#endif

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define LED_ON           0
#define LED_OFF          1

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
   PCA0MD  &= ~0x40;                   // disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Main Prototypes
//-----------------------------------------------------------------------------
void main (void)
{
	uint32_t time;

   SFRPAGE = LEGACY_PAGE;

   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
   LED1 = LED_OFF;

   RTC_Init();

   RTC_SetTimer(0x00001000);

   RTC_Write(RTC0CN, 0x90);           // run timer

   time = RTC_CaptureTimer();

   //No RTC ticks should have elapsed since we set the timer if high speed mode
   //is working correctly.
   while (time != 0x00001000);      //Code will hang here on error

   RTC_WriteAlarm(0,0x00002000);
   RTC_WriteAlarm(1,0x00003000);
   RTC_WriteAlarm(2,0x00004000);

   time = RTC_ReadAlarm(0);
   while (time != 0x00002000);      //Code will hang here on error

   time = RTC_ReadAlarm(1);
   while (time != 0x00003000);      //Code will hang here on error

   time = RTC_ReadAlarm(2);
   while (time != 0x00004000);      //Code will hang here on error

   LED1 = LED_ON;
   while(1);
}
//=============================================================================
