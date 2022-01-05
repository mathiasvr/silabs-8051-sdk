//=============================================================================
// F96x_LCD_LowPowerSleep_VIM878.c
//=============================================================================
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// This example demonstrates the LCD contrast voltage error, as specified on
// table 4.18 of the datasheet. The expected contrast voltage will be displayed
// on the LCD to be compared against the measured voltage. Pressing SW1 or SW2
// will increase or decrease the LCD contrast voltage.
//
//
// How To Test:
//
// 1) Attach a voltmeter to one of the LCD pins (such as P4.1).
// 2) Connect the USB Debug Adapter to J13.
// 3) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 4) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 5) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Press SW1 (P0.0) to decrease the LCD contrast voltage, SW2 (P0.1) to
//    increase it. The minimum contrast voltage is 1.9V, the maximum is 3.72V.
//    The LCD contrast voltage will be displayed, in millivolts, on the LCD.
//    By default, the LCD voltage will be 2.5V, the LCD will display "2500 MV".
// 8) Measure the maximum LCD voltage on the desired pin. The LCD will be in
//    constant-contrast mode, meaning that it will not vary with the supply
//    voltage. The measured voltage should not vary by more than the maximum
//    as specified on table 4.18 of the datasheet.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 17 MAR 2014
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
#include "stdbool.h"

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------
SI_SBIT(SW1, SFR_P0, 0);               // SW1 / SW2  == 0 means switch pressed
SI_SBIT(SW2, SFR_P0, 1);

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define MINIMUM_CONTRAST_VOLTAGE_mv 1900
#define MAXIMUM_CONTRAST_VOLTAGE_mv 3720
#define DEFAULT_CONTRAST_VOLTAGE_mv 2500

#define LCD_REFRESH_RATE_Hz   60
#define LCD_TOGGLE_RATE_Hz    1

#define VIM878_LCD_MUX        4
#define VIM878_LCD_SEGMENTS   32

#define SW_PRESSED          0
#define SW_NOT_PRESSED      1
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void LCD_Init_VIM878 (void);
void Port_Init(void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bit SW1_PRESS = false;
bit SW2_PRESS = false;

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
   uint16_t contrastVoltage_mv = DEFAULT_CONTRAST_VOLTAGE_mv;


   Port_Init();
   RTC_Init();
   LCD_Init_VIM878();

	VDM0CN = 0x00;                        // Disable VBAT monitor

   LPM_Init();                         // Initialize Power Management

   //Allow port-match events to wake the device from sleep
   LPM_Enable_Wakeup(PORT_MATCH);

   //Globally enable interrupts
   IE_EA = 1;

   while(1)
   {
      if (SW2_PRESS) {
         contrastVoltage_mv += 60;
      } else if (SW1_PRESS) {
         contrastVoltage_mv -= 60;
      }

      //Clear the button events after they have been used.
      SW1_PRESS = false;
      SW2_PRESS = false;

      //Bound the min and max contrast voltages
      if (contrastVoltage_mv >= MAXIMUM_CONTRAST_VOLTAGE_mv) {
         contrastVoltage_mv = MAXIMUM_CONTRAST_VOLTAGE_mv;
      } else if (contrastVoltage_mv <= MINIMUM_CONTRAST_VOLTAGE_mv) {
         contrastVoltage_mv = MINIMUM_CONTRAST_VOLTAGE_mv;
      }

      LCD0_SetContrast(contrastVoltage_mv);

      lcdPrintf("%u mV", contrastVoltage_mv);

      LPM(SLEEP);
   }
}

//-----------------------------------------------------------------------------
// LCD_Init_VIM878
//
// Description:
// This function will configure the LCD for a 128-segment 4-mux display.
//
// The refresh rate assumes a 16 kHz RTC clock using the LFO.
//
// This function uses the lcdConfig module to configure the LCD registers.
//
//-----------------------------------------------------------------------------
void LCD_Init_VIM878 (void)
{
   LCD0_Config(
         VIM878_LCD_MUX,
         VIM878_LCD_SEGMENTS,
         LCD_CONSTANT_CONTRAST_MODE,
         DEFAULT_CONTRAST_VOLTAGE_mv,
         LCD_REFRESH_RATE_Hz,
         LCD_TOGGLE_RATE_Hz);
}

//-----------------------------------------------------------------------------
// Port_Init
//
// Description:
// This function initializes the two push-buttons as inputs.
//
// Both SW1 (P0.0) and SW2 (P0.1) are used as wake-up sources by triggering
// port-match interrupts.
//
//-----------------------------------------------------------------------------
void Port_Init(void)
{
   // Set SW1 (P0.0) as a port match source. This will allow SW1 to wake the
   // device from sleep mode.
   P0MAT |= P0MAT_B0__HIGH;
   P0MASK |= P0MASK_B0__COMPARED;

   // Set SW2 (P0.1) as a port match source. This will allow SW2 to wake the
   // device from sleep mode.
   P0MAT |= P0MAT_B1__HIGH;
   P0MASK |= P0MASK_B1__COMPARED;

   //Enable portmatch interrupts
   EIE2 |= EIE2_EMAT__ENABLED;
}

//-----------------------------------------------------------------------------
// PMATCH_ISR
//
// Description:
// This function is triggered whenever SW1 or SW2 is pressed. It saves the sate
// of the buttons (for use as inputs later), then waits for both buttons to
// be released and debounced.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PMATCH_ISR, PMATCH_IRQn)
{
   uint8_t debounceTimer;

   if (SW1 == SW_PRESSED) {
      SW1_PRESS = true;
   }

   if (SW2 == SW_PRESSED) {
      SW2_PRESS = true;
   }

   //Wait for both switches to be released.
   while ((SW1 == SW_PRESSED) || (SW2 == SW_PRESSED));

   //Delay to de-bounce the switches. If either switch reads that it is once
   //again pressed, reset the delay timer.
   for (debounceTimer = 0; debounceTimer < 0xFF; debounceTimer++) {
      if ((SW1 == SW_PRESSED) || (SW2 == SW_PRESSED)) {
         debounceTimer = 0;
      }
   }
}
