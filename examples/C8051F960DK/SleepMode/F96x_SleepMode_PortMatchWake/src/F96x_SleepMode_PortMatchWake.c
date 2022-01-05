//-----------------------------------------------------------------------------
// F96x_SleepMode_PortMatchWake.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes  LED2 on the C8051F96x target board using the
// interrupt handler for Timer2 when the MCU is awake.
//
// Pressing the switch on P0.0 (SW1) will wake up the device using Port Match.
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
// 8) The device will initially be asleep. Press SW1 to wake it back up. 
// 9) LED2 will blink when the device is awake.
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
//#define UDP_F960_MCU_MUX_LCD
#define UDP_F960_MCU_EMIF
//-----------------------------------------------------------------------------
// Hardware Dependent definitions
//-----------------------------------------------------------------------------
#ifdef UDP_F960_MCU_MUX_LCD
SI_SBIT(SW1, SFR_P0, 0);
SI_SBIT(LED2, SFR_P0, 1);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(SW1, SFR_P3, 0);
SI_SBIT(LED2, SFR_P3, 1);
#endif

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK  20000000

#define LED_ON           0
#define LED_OFF          1

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void);
void PORT_Init (void);
void Timer2_Init (int16_t counts);

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

   while(!SW1);                        // Debug Trap -- Prevents the system
                                       // from entering sleep mode after
                                       // a reset if Switch 2 is pressed.

   PORT_Init ();                       // Initialize Port I/O

   LED2 = LED_OFF;                     // Initilize the state of the signals

   OSCILLATOR_Init ();                 // Initialize Oscillator

   Timer2_Init (SYSCLK / 12 / 32);     // Init Timer2 to generate interrupts
                                       // at a 32 Hz rate.

   LPM_Init();                         // Initialize Power Management
   LPM_Enable_Wakeup(PORT_MATCH);      // Enable Port Match wake-up source

   IE_EA = 1;                             // Enable global interrupts


   LED2 = LED_OFF;                    // Turn off LED2

                                       // the device is entering the low power
                                       // state
   LPM (SLEEP);
 //----------------------------------
   // Main Application Loop
   //----------------------------------
   while (1)
   {

      //-----------------------------------------------------------------------
      // Task #1 - Handle Port Match Event
      //-----------------------------------------------------------------------
      if(PortMatchWakeUp)
      {

         PortMatchWakeUp = 0;           // Reset Port Match Flag to indicate
                                       // that we have detected an event

         // Adjust LED state for active mode
         LED2 = LED_ON;

      }

   }

}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
// This routine changes the state of the LED whenever Timer2 overflows.
//
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                           // clear Timer2 interrupt flag
   LED2 = !LED2;                 // change state of LED

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
// P0.6   digital   open-drain    Switch 2
// P0.7   digital   push-pull     Red LED
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDOUT |= 0x80;                    // P0.7 push pull,

   // Setup the Port Match Wake-up Source to wake up on the falling
   // edge of P0.0 (when the switch is pressed)
   P0MASK = 0x01;                      // Mask out all pins except for P0.2
   P0MAT = 0x01;                       // P0.2 should be HIGH while the device
                                       // is in the low power mode

   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
void Timer2_Init (int16_t counts)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON  &= ~0x60;                    // Timer2 clocked based on TMR2CN_T2XCLK;

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = TMR2RL;                   // initalize timer to reload value
   IE_ET2     = 1;                        // enable Timer2 interrupts
   TMR2CN_TR2     = 1;                        // start Timer2
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------



