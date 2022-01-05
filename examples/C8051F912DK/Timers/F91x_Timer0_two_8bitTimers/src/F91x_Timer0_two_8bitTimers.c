//-----------------------------------------------------------------------------
// F91x_Timer0_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer0 of the C8051F91x in
// two 8-bit reload counter/timer mode. It uses the 'F912DK as HW platform.
//
// It sets the two timers to interrupt every millisecond.
//
// When the timer low interrupts a counter (low_counter) increments and upon
// reaching the value defined by YELLOW_TOGGLE_RATE toggles the Yellow LED
// When the timer high interrupts a counter (high_counter) increments and upon
// reaching the value defined by RED_TOGGLE_RATE toggles the RED LED
//
// Pinout:
//
//    P1.6 -> Yellow LED (toggled by TL0)
//    P1.5 -> Red LED    (toggled by TH0)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.6/LED1.6
//             P1.5/LED1.5
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) If a different LED blink rates are required, change the following:
//       -> YELLOW_TOGGLE_RATE
//       -> RED_TOGGLE_RATE
// 5) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) The yellow LED will blink at YELLOW_TOGGLE_RATE, the red LED will blink
//    at RED_TOGGLE_RATE.
//
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 14 JAN 2014
//
// Release 1.2 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.1
//    - Port to C8051F912 from C8051F930 (JH)
//    - Compile and test C8051F912_defs.h on 912 board (JH)
//    - 06 JULY 2009
//
// Release 1.0
//    -Initial Revision (FB)
//    -20 NOV 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             20000000/8  // SYSCLK in Hz (20 MHz internal
                                       // low power oscillator / 8)
                                       // the low power oscillator has a
                                       // tolerance of +/- 10%
                                       // the precision oscillator has a 
                                       // tolerance of +/- 2%


#define TIMER_PRESCALER            48  // Based on Timer CKCON settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: TIMER_TICKS_PER_MS should not exceed 255 (0xFF) for the 8-bit timers

#define AUX1     TIMER_TICKS_PER_MS
#define AUX2     -AUX1

#define YELLOW_TOGGLE_RATE        100  // Yellow LED toggle rate in milliseconds
                                       // if YELLOW_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define RED_TOGGLE_RATE           30   // Red LED toggle rate in milliseconds
                                       // if RED_TOGGLE_RATE = 1, the
                                       // SIGNAL output will be on for 1
                                       // millisecond and off for 1
                                       // millisecond

#define TIMER0_RELOAD_HIGH       AUX2  // Reload value for Timer0 high byte
#define TIMER0_RELOAD_LOW        AUX2  // Reload value for Timer0 low byte

SI_SBIT(YELLOW_LED, SFR_P1, 6);          // YELLOW_LED == 0 means ON
SI_SBIT(RED_LED, SFR_P1, 5);             // RED_LED    == 0 means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Port_Init (void);                 // Port initialization routine
void Timer0_Init (void);               // Timer0 initialization routine
SI_INTERRUPT_PROTO(TIMER0_ISR, TIMER0_IRQn);
SI_INTERRUPT_PROTO(TIMER1_ISR, TIMER1_IRQn);

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

   Timer0_Init ();                     // Initialize the Timer0
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
// P1.6   digital   push-pull   Yellow LED (toggled by TL0)
// P1.5   digital   push-pull   Red LED    (toggled by TH0)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   XBR2 = 0x40;                        // Enable crossbar
   P1MDOUT = 0xC0;                     // Set LED's to push-pull
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as two 8-bit timers, interrupt enabled.
// Using the internal osc. at 12MHz with a prescaler of 1:8 and reloading the
// TH0 register with TIMER0_RELOAD_HIGH and TL0 with TIMER0_RELOAD_HIGH.
//
// Note: The Timer0 uses a 1:48 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
   TH0 = TIMER0_RELOAD_HIGH;           // Init Timer0 High register
   TL0 = TIMER0_RELOAD_LOW;            // Init Timer0 Low register

   TMOD = 0x03;                        // Timer0 in two 8-bit mode
   CKCON = 0x02;                       // Timer0 uses a 1:48 prescaler
   IE_ET0 = 1;                            // Timer0 interrupt enabled
   IE_ET1 = 1;                            // Timer1 interrupt enabled
   TCON = 0x50;                        // Timer0 and timer1 ON
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer0 interrupt and toggle the Yellow LED
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
   static uint16_t low_counter=0;  // Define counter variable

   TL0 = TIMER0_RELOAD_LOW;            // Reinit Timer0 Low register
   if((low_counter++) == YELLOW_TOGGLE_RATE)
   {
      low_counter = 0;                 // Reset interrupt counter
      YELLOW_LED = !YELLOW_LED;        // Toggle the LED
   }
}

//-----------------------------------------------------------------------------
// Timer1_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer1 interrupt and toggle the Red LED
// Note: In this mode the Timer0 is effectively using the Timer1 interrupt
// vector. For more info check the Timers chapter of the datasheet.
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER1_ISR, TIMER1_IRQn)
{
   static uint16_t high_counter=0;

   TH0 = TIMER0_RELOAD_HIGH;           // Reinit Timer0 High register
   if((high_counter++) == RED_TOGGLE_RATE)
   {
      high_counter = 0;                // Reset interrupt counter
      RED_LED = !RED_LED;              // Toggle LED
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
