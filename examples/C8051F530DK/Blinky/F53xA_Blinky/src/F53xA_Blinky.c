//-----------------------------------------------------------------------------
// F53xA_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program blinks an LED every second.
//
// Pinout:
//
// P1.3 - LED
//
//
// How to Test:
// ------------
// 1) Ensure shorting blocks on C8051F530A TB are placed on the following:
//    "A" SIDE
//    HDR4: P1.3_A - P1.3_LED_A
//    "B" SIDE
//    HDR3: P1.3_B - P1.3_LED_B
// 2) Connect the USB Debug Adapter to HDR2 for "A" SIDE or HDR1 for "B" SIDE.
//    Connect the 9V power adapter to P5.
// 3) Compile and download code to C8051F530A target board
//    (either "A" SIDE or "B" SIDE) with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) The code is running when the LED blinks.
//
//
// Target:         C8051F52xA/F53xA (C8051F530A TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 09 SEP 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1
//    -29 JAN 2008 (TP)
//    -Updated for C8051F52xA/F53xA
//
// Release 1.0
//    -Initial Revision (CG)
//    -07 SEP 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F530_Register_Enums.h>

//-----------------------------------------------------------------------------
// SBIT definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P1, 3);                 // LED definition = P1.3

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t Counter;                            // Counter used to measure time between
                                       // toggles of the LED

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define SYSCLK 24500000                // System Clock in MHz

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void Timer2_Init (void);

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------
void main ( void )
{

   Counter = 0;                        // Initialize timer internal counter

   OSCILLATOR_Init ();                 // Initialize the oscillator
   PORT_Init ();                       // Initialize the crossbar and ports
   Timer2_Init ();                     // Initialize Timer2

   IE_EA = 1;                          // Enable global interrupts
   while (1)                           // Loop forever
   {
      if ( Counter == 0 )              // If the counter is zero reload it
      {                                // and toggle the LED
         Counter = 100;                // Initialize the counter to be
                                       // decremented by timer2; time
                                       // in miliseconds
         LED = ~LED;                   // Toggle LED
      }
   }
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
// This function initializes the system clock to (24.5 Mhz)
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   // Set internal oscillator to run at its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P1.3   digital   push-pull     LED1
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P1MDOUT = 0x08;                     // Force LED pin to be push-pull
   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED;
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes Timer2 to interrupt every 1 ms.
//
// Note: This function uses the value of SYSCLK to determine the 1 ms wait
// time.
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   CKCON |= 0x10;                      // Timer2 uses the SYSCLK
   TMR2RL = -((SYSCLK+500L)/1000);     // Time for 1msec @SYSCLK in both reload
   TMR2 = TMR2RL;                      // and timer register
   TMR2CN = 0x04;                      // Timer2 in 16-bit reload timer mode
   IE_ET2 = 1;                         // Enable interrupt in timer2
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// Timer2 Interrupt. Asserted every 1msec and decrements the the counter used
// to trigger an LED toggle.
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Reset timer2 interrupt flag
   if ( Counter )                      // Decrement counter if necessary
   {
      Counter--;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------