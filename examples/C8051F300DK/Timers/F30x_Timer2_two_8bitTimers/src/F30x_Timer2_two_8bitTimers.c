//-----------------------------------------------------------------------------
// F30x_Timer2_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer2 of the C8051F30x's in
// two 8-bit reload counter/timer mode. It uses the 'F300DK as HW platform.
//
// It sets Timer2 to be two 8-bit timers that interrupt every millisecond.
//
// When the Timer2 low interrupts a counter (low_counter) increments and upon
// reaching the value defined by LED_TOGGLE_RATE toggles the LED
// When the Timer2 high interrupts a counter (high_counter) increments and upon
// reaching the value defined by SIGNAL_TOGGLE_RATE toggles the SIGNAL pin
//
// Pinout:
//
//    P0.2 -> LED (toggled by TMR2L)
//
//    P0.7 -> SIGNAL (toggled by TMR2H)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J3: P0.2_LED
// 2) Connect the USB Debug Adapter to J4.
// 3) Connect the 9V power adapter to P1.
// 4) If a different toggle rates are required, change the following constants:
//       -> LED_TOGGLE_RATE in milliseconds
//       -> SIGNAL_TOGGLE_RATE in milliseconds
// 5) Compile and download code to a 'F300 device on a C8051F300-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Verify that the LED (P0.2) is blinking and SIGNAL (P0.7) is toggling.
//
//
// Target:         C8051F300
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 10 JAN 2014
//
// Release 1.0
//
//    -Initial Revision (CG)
//    -08 NOV 2005
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F300_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000/8  // SYSCLK in Hz (24.5 MHz internal
                                       // oscillator / 8)
                                       // the internal oscillator has a
                                       // tolerance of +/- 2%

#define TIMER_PRESCALER            12  // Based on Timer2 CKCON and TMR2CN
                                       // settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: TIMER_TICKS_PER_MS should not exceed 255 (0xFF) for the 8-bit timers

#define AUX1     TIMER_TICKS_PER_MS
#define AUX2     -AUX1

#define LED_TOGGLE_RATE           100  // LED toggle rate in milliseconds
                                       // if LED_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define SIGNAL_TOGGLE_RATE         30  // SIGNAL pin toggle rate in
                                       // milliseconds
                                       // if SIGNAL_TOGGLE_RATE = 1, the
                                       // SIGNAL output will be on for 1
                                       // millisecond and off for 1
                                       // millisecond

#define TIMER2_RELOAD_HIGH       AUX2  // Reload value for Timer2 high byte
#define TIMER2_RELOAD_LOW        AUX2  // Reload value for Timer2 low byte

SI_SBIT(LED, SFR_P0, 2);                  //
SI_SBIT(SIGNAL, SFR_P0, 7);               //

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(Timer2_ISR, 5);

void Port_Init (void);                 // Port initialization routine
void Timer2_Init (void);               // Timer2 initialization routine

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
   PCA0MD &= ~0x40;                    // Clear watchdog timer enable
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   Timer2_Init ();                     // Initialize the Timer2
   Port_Init ();                       // Init Ports
   IE_EA = 1;                             // Enable global interrupts

   while (1);                          // Loop forever
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// Pinout:
//
//    P0.2 -> LED (toggled by TMR2L)
//
//    P0.7 -> SIGNAL (toggled by TMR2H)
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   XBR2 = 0x40;                        // Enable crossbar
   P0MDOUT = 0x84;                     // Set LED and SIGNAL to
                                       // be used as push-pull
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer2 as two 8-bit reload timers with interrupt.
// Using the internal osc. at 24.5MHz/8 with a timer prescaler of 1:12
// reloading the TMR2H register with TIMER2_RELOAD_HIGH and TMR2L with
// TIMER2_RELOAD_HIGH.
//
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer2_Init(void)
{
   CKCON &= ~0x60;                     // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RLH = TIMER2_RELOAD_HIGH;       // Init Timer2 Reload High register
   TMR2RLL = TIMER2_RELOAD_LOW;        // Init Timer2 Reload Low register

   TMR2H = TIMER2_RELOAD_HIGH;         // Init Timer2 High register
   TMR2L = TIMER2_RELOAD_LOW;          // Init Timer2 Low register

   TMR2CN = 0x2C;                      // Low byte interrupt enabled
                                       // Timer2 enabled
                                       // and Split-Mode enabled
   IE_ET2 = 1;                            // Timer2 interrupt enabled
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This function processes the Timer2 interrupt. It checks both counters for
// matches and, upon a match, toggles the LED and SIGNAL.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, 5)
{
   static uint16_t low_counter=0;  // Define counter variables
   static uint16_t high_counter=0;

   if(TMR2CN_TF2L == 1)
   {
      if((low_counter++) == LED_TOGGLE_RATE)
      {
         low_counter = 0;              // Reset interrupt counter
         LED = ~LED;                   // Toggle the LED
      }
      TMR2CN_TF2L = 0;
   }
   if(TMR2CN_TF2H == 1)
   {
      if((high_counter++) == SIGNAL_TOGGLE_RATE)
      {
         high_counter = 0;             // Reset interrupt counter
         SIGNAL = ~SIGNAL;             // Toggle the SIGNAL pin
      }
      TMR2CN_TF2H = 0;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------