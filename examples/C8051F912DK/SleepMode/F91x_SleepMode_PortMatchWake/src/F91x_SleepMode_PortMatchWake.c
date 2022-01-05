//-----------------------------------------------------------------------------
// F91x_SleepMode_PortMatchWake.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes the red LED on the C8051F912 target board using the 
// interrupt handler for Timer2 when the MCU is awake.
//
// Pressing the switch on P0.3 will place the device in a low power mode.
// Pressing the switch on P0.2 will wake up the device using Port Match.
//
// How To Test:
//
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
// 6) Press P0.3 to place the device in sleep mode. 
//    Press P0.2 to wake it back up. The Red LED will blink when 
//    the device is awake.
//
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - 14 JAN 2014
//
// Release 1.1 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.0
//    - Initial Revision (FB)
//    - 08 OCT 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F912_Register_Enums.h>
#include "C8051F912_lib.h"             // library declarations
                       
//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,    SFR_P1, 5);          // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED, SFR_P1, 6);          // '0' means ON, '1' means OFF
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed
SI_SBIT(SW3,        SFR_P0, 3);          // SW3 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
                                               
#define LED_ON           0
#define LED_OFF          1

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

   while(!SW2);                        // Debug Trap -- Prevents the system
                                       // from entering sleep mode after
                                       // a reset if Switch 2 is pressed.

   PORT_Init ();                       // Initialize Port I/O

   RED_LED = LED_OFF;                  // Initilize the state of the signals
   YELLOW_LED = LED_OFF;                      

   OSCILLATOR_Init ();                 // Initialize Oscillator
   
   Timer2_Init (SYSCLK / 12 / 32);     // Init Timer2 to generate interrupts
                                       // at a 32 Hz rate.

   LPM_Init();                         // Initialize Power Management
   LPM_Enable_Wakeup(PORT_MATCH);      // Enable Port Match wake-up source

   IE_EA = 1;                             // Enable global interrupts
                                       
   
   //----------------------------------
   // Main Application Loop
   //----------------------------------
   while (1)
   {


      //-----------------------------------------------------------------------
      // Task #1 - Handle Port Match Event
      //-----------------------------------------------------------------------
      if(Port_Match_Wakeup)
      {

         Port_Match_Wakeup = 0;        // Reset Port Match Flag to indicate 
                                       // that we have detected an event

         // Adjust LED state for active mode
         YELLOW_LED = LED_OFF;
         RED_LED = LED_ON;

      }     
      

      //-----------------------------------------------------------------------
      // Task #2 - Enter sleep mode if P0.3 switch is pressed
      //-----------------------------------------------------------------------

      if(!SW3)                         // If the P0.3 switch is pressed
      {
         

         // Configure the Port I/O for Low Power Mode 
         RED_LED = LED_OFF;            // Turn off the RED_LED

         YELLOW_LED = LED_ON;          // Turn on Yellow LED to indicate that
                                       // the device is entering the low power
                                       // state
         LPM (SLEEP);

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
   RED_LED = !RED_LED;                 // change state of LED

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
// P0.2   digital   open-drain    Switch 2
// P0.3   digital   open-drain    Switch 3
//
// P1.5   digital   push-pull     TRIGGER indicator 
// P1.6   digital   push-pull     Debug Pin

//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDIN |= 0x0C;                     // P0.2, P0.3 are digital
   P1MDIN |= 0x60;                     // P1.5, P1.6 are digital

   P0MDOUT &= ~0x0C;                   // P0.2, P0.3 are open-drain
   P1MDOUT |= 0x60;                    // P1.5, P1.6 are push-pull

   P0     |= 0x0C;                     // Set P0.2, P0.3 latches to '1'

   // Setup the Port Match Wake-up Source to wake up on the falling
   // edge of P0.2 (when the switch is pressed)
   P0MASK = 0x04;                      // Mask out all pins except for P0.2
   P0MAT = 0x04;                       // P0.2 should be HIGH while the device
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



              
