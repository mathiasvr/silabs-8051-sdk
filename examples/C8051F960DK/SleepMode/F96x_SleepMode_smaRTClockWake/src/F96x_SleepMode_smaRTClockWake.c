//-----------------------------------------------------------------------------
// F96x_SleepMode_smaRTClockWake.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes LED2 on the C8051F96x target board about
// five times a second using the interrupt handler for Timer2 when the
// MCU is awake.
//
// The smaRTClock is configured to generate a wake-up event every 2 seconds.
// The device will toggle between Sleep/Suspend mode and Normal mode. In
// the low power state, LED1 will be turned on.
//
// If a smaRTClock oscillator failure occurs, both LEDs will remain on
// and the device will be in a high power state.
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
// 8) The device will alternate between sleep mode and awake.
// 9) LED2 will blink when the device is awake, LED1 will be on when the
//    device is sleeping.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Added support for the EMIF board.
//    - 20 JAN 2014
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F960_Register_Enums.h>
#include "PowerManagementF960.h"
#include "SmaRTClockF960.h"

// Define Hardware
//-----------------------------------------------------------------------------
#define UDP_F960_MCU_MUX_LCD
//#define UDP_F960_MCU_EMIF
//-----------------------------------------------------------------------------
// Hardware Dependent definitions
//-----------------------------------------------------------------------------
#ifdef UDP_F960_MCU_MUX_LCD
SI_SBIT(LED1, SFR_P0, 0);
SI_SBIT(LED2, SFR_P0, 1);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(LED1, SFR_P3, 0);
SI_SBIT(LED2, SFR_P3, 1);
#endif
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
bit Blink_LED = 0;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
//#define SYSCLK  10000000
#define RTCCLK  16000L

#define LED_ON           0
#define LED_OFF          1

// Constants for determining the RTC wake-up interval
#define WAKE_INTERVAL   10L         // Wakeup-interval in seconds
#define WAKE_INTERVAL_TICKS  ((RTCCLK * WAKE_INTERVAL))
//#define ONE_MS_TICKS         ((RTCCLK * 1L ) / 1000L)

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void);
void PORT_Init (void);
void Timer3_Init (int16_t counts);

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

                                       // enable)

   PORT_Init ();                       // Initialize Port I/O

   LED2 = LED_OFF;

   OSCILLATOR_Init ();                 // Initialize Oscillator

   Timer3_Init (16000 / 10);           // Init Timer3 to generate interrupts
                                       // at a 10 Hz rate.

   RTC_Init ();                        // Initialize SmaRTClock

   RTC_WriteAlarm(0, WAKE_INTERVAL_TICKS);// Set the Alarm Value

   // Enable Alarm0 and Auto-Reload
   RTC_Write(RTC0CF, 0x09);

   // Run Timer
   RTC_Write(RTC0CN, (RTC_Read(RTC0CN)|0x10));

   LPM_Init();                         // Initialize Power Management
   LPM_Enable_Wakeup(RTC);

   IE_EA = 1;

   RTC_Alarm0_WakeUp = 1;                      // Set the RTC Alarm Flag on startup

   // Place the device in Sleep Mode
   LED1 = LED_ON;
   LPM(SLEEP);                      // Enter Sleep Until Next Alarm
   //----------------------------------
   // Main Application Loop
   //----------------------------------
   while (1)
   {

      //-----------------------------------------------------------------------
      // Task #1 - Handle RTC Failure
      //-----------------------------------------------------------------------
      if(RTC_ClockFailWakeUp)
      {
         RTC_ClockFailWakeUp = 0;              // Reset RTC Failure Flag to indicate
                                       // that we have detected an RTC failure
                                       // and are handling the event

         // Do something...RTC Has Stopped Oscillating
         while(1);                     // <Insert Handler Code Here>
      }


      //-----------------------------------------------------------------------
      // Task #2 - Handle RTC Alarm
      //-----------------------------------------------------------------------
      if(RTC_Alarm0_WakeUp)
      {

         RTC_Alarm0_WakeUp = 0;                // Reset RTC Alarm Flag to indicate
                                       // that we have detected an alarm
                                       // and are handling the alarm event


         // clear flag
         RTC_Write(RTC0CF, 0x09);

         Blink_LED = !Blink_LED;       // Change blinking state

         if(Blink_LED)
         {
            // Adjust LED state for active mode
            LED1 = LED_OFF;
            LED2 = LED_ON;

         }
         else
         {
            // Adjust LED state for sleep mode
            LED1 = LED_ON;
            LED2 = LED_OFF;
         }

      }

      //-----------------------------------------------------------------------
      // Task #3 - Sleep for the remainder of the interval
      //-----------------------------------------------------------------------
      // If blinking, stay awake, otherwise sleep

      if(Blink_LED)
      {
         // Wait for next alarm or clock failure, then clear flags
         while((PMU0CF & RTCAWK) == 0);
         if(PMU0CF & RTCAWK) RTC_Alarm0_WakeUp = 1;
         if(PMU0CF & RTCFWK) RTC_ClockFailWakeUp = 1;
         PMU0CF = 0x20;

      }
      else
      {
         // Place the device in Sleep Mode

         LPM(SLEEP);                      // Enter Sleep Until Next Alarm

      }

   }
}
//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
// This routine changes the state of the LED whenever Timer3 overflows.
//
SI_INTERRUPT(Timer3_ISR, TIMER3_IRQn)
{
   TMR3CN &= ~0x80;                       // clear Timer3 interrupt flag
   LED2 = !LED2;                    // change state of the LED2

}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal precision
// oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{


   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x04;                      // Select low power internal osc.
                                       // divided by 1 as the system clock

   FLSCL |= 0x40;                      // Set the BYPASS bit for operation > 10MHz

}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// This function configures the crossbar and ports pins.
//
// P0.6   digital   open-drain    Switch 2
// P0.7   digital   push-pull     Red LED
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDOUT |= 0x80;                    // P0.7 push pull,

   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}
//-----------------------------------------------------------------------------
// Timer3_Init
//-----------------------------------------------------------------------------
//
//
void Timer3_Init (int16_t counts)
{
   TMR3CN  = 0x03;                        // Stop Timer3; Clear TF3;
                                          // use SmaRTClock as timebase

   CKCON  &= ~0xC0;                       // Timer3 clocked based on TMR2CN_T2XCLK;

   TMR3RL  = -counts;                     // Init reload values
   TMR3    = TMR3RL;                      // initalize timer to reload value
   EIE1   |= 0x80;                        // enable Timer3 interrupts
   TMR3CN |= 0x04;                        // start Timer3
}




