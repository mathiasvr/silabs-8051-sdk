//=============================================================================
// F96x_LCD_LowPowerSleep_VIM878.c
//=============================================================================
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// This program demonstrates the use of the VIM876 LCD drivers to display a
// message on the UDP F960 MCU MUX LCD board.
//
// The LCD_Init_VIM878 code in this module is specific to the VIM878.
// This is a 4-mux 128-segment display.
//
// The corresponding lcdPutChar_VIM878 module must be included in the project.
// The lcdPutchar function is specific to the particular display.
//
// How To Test:
//
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Short LCD_NSS/P0.3 on J16.
// 2) Connect the USB Debug Adapter to J13.
// 3) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 4) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 5) Connect a 9V power adapter to P1 of the development board.
// 6) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) "'F960 LCD" should be displayed on the LCD.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Updated Description / How to Test
//    - 27 JAN 2014
//
//=============================================================================
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F960_Register_Enums.h>
#include "lcdPutString.h"
#include "lcdPrintf.h"
#include "lcdConfig.h"
#include "SmaRTClockF960.h"
#include "PowerManagementF960.h"
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void LCD_Init_VIM878 (void);
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
// Main Prototypes
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = LEGACY_PAGE;

   RTC_Init();
   LCD_Init_VIM878();


   //lcdPutString("'@.'@.'@.'@.'@.'@.'@.'@.");
   //lcdPrintf("%u", 12345);
   lcdPutString("'F960 LCD");

   SFRPAGE = LEGACY_PAGE;
	VDM0CN = 0x00;                        // Disable VBAT monitor

   LPM_Init();                         // Initialize Power Management
   LPM(SLEEP);                         // Enter Sleep Until Next Alarm

   while(1);
}
//-----------------------------------------------------------------------------
// LCD_Init_VIM878
//
// Description:
// This function will configure the LCD for a 128-segment 4-mux display.
//
// The refresh rate assumes a 16 kHz RTC clock using the LFO.
//
// This function uses the lcdConfig module to configure the LCD regsiters.
//
//-----------------------------------------------------------------------------
void LCD_Init_VIM878 (void)
{
   //          +- 4-mux
   //          |  +- 32 segments
   //          |  |   +- Mode 1 - Bypass Contrast mode
   //          |  |   |  +- Contrast 2.9 volts
   //          |  |   |  |     +- Refresh 60 Hz
   //          |  |   |  |     |   +- Toggle 1 Hz
   LCD0_Config(4, 32, 1, 2900, 60, 1);
}
//=============================================================================
