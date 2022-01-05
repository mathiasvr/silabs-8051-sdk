//-----------------------------------------------------------------------------
// F91x_CapTouchSense_WakeOnRTC_Main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program implements Low-Power Capacitive TouchSense with Wake on RTC.
//
// How To Test:
//
// Setup:
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.5/LED1.5
//             P1.6/LED1.6
//             P0.2/SW0.2
//             P0.3/SW0.3
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) If uncalibrated, the RED LED will light up. In this case, ensure that 
//    no object or a finger is on any of the CapTouchSense switches and press 
//    the SW2 push-button switch to begin calibration.
// 7) All LEDs will flash once to indicate that calibration is complete.
// 8) Place a finger on each CapTouchSense switch to see the corresponding LED
//    light up.
//
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 13 JAN 2014
//
// Release 1.2 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.1 
//    - Compiled and tested for C8051F930-TB, C8051F912-TB,
//    - and TOOLSTICK TOUCHSENSE DC (JH)
//    - Port to C8051F912-TB from C8051F930-TB (JH)
//    - 06 JULY 2009
//
// Release 1.0
//    - Initial Revision (PKC)
//    - 20 MAY 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F912_Register_Enums.h>
#include "F91x_CapTouchSense_WakeOnRTC.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t System_State;

uint8_t CS_Switch_State[NUM_SWITCHES];

//SI_LOCATED_VARIABLE(CS_Switch_Cal_Threshold[NUM_SWITCHES], SI_UU16_t, code, 0x7800, 0);
#ifdef SDCC
	SI_LOCATED_VARIABLE_NO_INIT(CS_Switch_Cal_Threshold[NUM_SWITCHES], SI_UU16_t, SI_SEG_CODE, CAL_CONSTANTS_PAGE_ADDR);
#else
	SI_UU16_t code CS_Switch_Cal_Threshold[NUM_SWITCHES] _at_ CAL_CONSTANTS_PAGE_ADDR;
#endif

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
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint8_t i;

                                       // enable)
   SYSCLK_Init ();
   PORT_IO_Init ();
   RTC_Init ();
   TouchSense_Init ();

   // Set RTC alarm wakeup interval
   RTC_SetAlarmInterval (WAKEUP_INTERVAL);
   IE_EA = 1;                             // Enable global interrupts

   // Erased Flash locations are set to 0xFF. Check if calibration is required:
   if (CS_Switch_Cal_Threshold[0].u16 == 0xFFFF)
   {
      Calibrate ();
   }

   while (1)                           // Spin forever
   {
      // Task #1: Update the status of the switches

      CLKSEL = 0x24;                   // Select internal low-power osc.
                                       // divided by 4 as the system clock @ 5 MHz
      TouchSense_Update ();            // Update the status of the switches

      //-------------------

      // Task #2: Take action based on switch states
      // In this example, light up LEDs while switch is depressed

      // Also update the System_State variable:
      // In this example, when a switch is depressed, the device should not enter sleep mode
      // because the "action" is to have an LED lit. In other applications, this may not be necessary.

      CLKSEL = 0x04;                   // Select internal low-power osc.
                                       // divided by 1 as the system clock @ 20 MHz
      System_State = INACTIVE;

      for (i = 0; i < NUM_SWITCHES; i++)
      {
         if (CS_Switch_State[i] == SWITCH_PRESSED)
         {
            P1 &= ~LED_P1_Mask[i];
            System_State = ACTIVE;
         }
         else
         {
            P1 |= LED_P1_Mask[i];
         }
      }

      //-------------------

      // Task #3: If the system is inactive, enter sleep mode
      if (System_State == INACTIVE)
      {
         CPT0CN &= ~0x80;              // Disable the comparator

         PMU0CF = 0x20;                // Clear all wake-up flags
         PMU0CF = 0x8C;                // Enter SLEEP mode
                                       // Wake-up source: RTC
         // Power-up the comparator
         CPT0CN |= 0x80;               // Enable the comparator
         // Wait for comparator to settle
         Delay_Microseconds (COMPARATOR_POWER_UP_TIME_CNT);
         CPT0CN &= ~0x30;              // Clear the comparator edge flags

         // Task #4: On wake-up, check wake-up sources and take action
         if (PMU0CF & 0x08)
         {
            // RTC Oscillator Fail Event
            // Re-initialize oscillator
            RTC_Init ();
         }

         if (PMU0CF & 0x04)
         {
            // RTC Alarm Event
            // Proceed to CapTouchSense switch update
         }
      }
   }
}

//-----------------------------------------------------------------------------
// TouchSense_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Updates the state of the Touch Sense switches.
//-----------------------------------------------------------------------------
void TouchSense_Update (void)
{
   SI_UU16_t timer_count_diff;
   uint16_t timer_count_a, timer_count_b;
   uint8_t i;

   for (i = 0; i < NUM_SWITCHES; i++)
   {
      CPT0MX = CS_Switch_Mux_Setting[i];

      // Prepare Timer2 for the first TouchSense reading
      TMR2CN_TF2H = 0;                     // Clear Timer2 overflow flag
      IE_ET2 = 1;                      // Enable Timer2 interrupts
      PCON |= IDLE;                 // Enter IDLE mode
      // Timer2 interrupt will occur when a comparator rising edge is captured
      // MCU will exit IDLE mode when the Timer2 interrupt occurs
      // Code in the Timer2 ISR will clear the TMR2CN_TF2H overflow flag
      timer_count_a = TMR2RL;       // Capture count
      
      // Prepare Timer2 for the second TouchSense reading
      PCON |= IDLE;                 // Enter IDLE mode
      // Timer2 interrupt will occur when a comparator rising edge is captured
      // MCU will exit IDLE mode when the Timer2 interrupt occurs
      // Code in the Timer2 ISR will clear the TMR2CN_TF2H overflow flag
      timer_count_b = TMR2RL;        // Capture count

      IE_ET2 = 0;                      // Disable Timer2 interrupts

      // Calculate the oscillation period
      timer_count_diff.u16 = timer_count_b - timer_count_a;

      if (timer_count_diff.u16 > CS_Switch_Cal_Threshold[i].u16)
      {
         CS_Switch_State[i] = 0;
      }
      else
      {
         CS_Switch_State[i] = 1;
      }
   }
}

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// This ISR clears the Timer2 overflow flag.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (TIMER2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                           // Clear overflow flag
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
