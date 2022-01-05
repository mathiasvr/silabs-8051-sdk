//-----------------------------------------------------------------------------
// F336_Timer0_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer0 of the C8051F336 in
// two 8-bit reload counter/timer mode. It uses the 'F336DK as HW platform.
//
// It sets the two timers to interrupt every millisecond.
//
// When the timer low interrupts a counter (low_counter) increments and upon
// reaching the value defined by LED_TOGGLE_RATE toggles the LED
// When the timer high interrupts a counter (high_counter) increments and upon
// reaching the value defined by SIGNAL_TOGGLE_RATE toggles the SIGNAL pin
//
// Pinout:
//
//    P1.3 -> LED (toggled by TL0)
//    P1.7 -> SIGNAL (toggled by TH0)
//
//    all other port pins unused
//
// How To Test:
//
// 1) If desired, change the number of interrupts (ms) to toggle an output:
//    LED_TOGGLE_RATE
//    SIGNAL_TOGGLE_RATE
// 2) Compile and download the code to C8051F338-TB.
// 3) Verify the LED pin of J3 are populated.
// 4) Run the code the code.
// 5) Verify that the LED is blinking and SIGNAL is toggling.
//
//
// Target:         C8051F336/7/8/9
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.1 (JL)
//    - Updated description
//    - 25 JAN 2014
//
// Release 1.0
//    -Initial Revision (SM)
//    -30 JULY 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F336_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000/8  // SYSCLK in Hz (24.5 MHz internal
                                       // oscillator / 8)
                                       // the internal oscillator has a
                                       // tolerance of +/- 2%

#define TIMER_PRESCALER            48  // Based on Timer CKCON settings

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

#define TIMER0_RELOAD_HIGH       AUX2  // Reload value for Timer0 high byte
#define TIMER0_RELOAD_LOW        AUX2  // Reload value for Timer0 low byte

SI_SBIT(LED, SFR_P1, 3);                 // LED==1 means ON
SI_SBIT(SIGNAL, SFR_P1, 7);              // SW1==0 means switch depressed

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
// P1.3   digital   push-pull   LED (toggled by TL0)
// P1.7   digital   push-pull   SIGNAL (toggled by TH0)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   XBR1 = 0xC0;                        // Enable crossbar
   P1MDOUT = 0x88;                     // Set LED and SIGNAL to push-pull
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
// Here we process the Timer0 interrupt and toggle the LED
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
   static unsigned int low_counter=0;  // Define counter variable

   TL0 = TIMER0_RELOAD_LOW;            // Reinit Timer0 Low register
   if((low_counter++) == LED_TOGGLE_RATE)
   {
      low_counter = 0;                 // Reset interrupt counter
      LED = ~LED;                      // Toggle the LED
   }
}

//-----------------------------------------------------------------------------
// Timer1_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer1 interrupt and toggle the SIGNAL I/O
// Note: In this mode the Timer0 is effectively using the Timer1 interrupt
// vector. For more info check the Timers chapter of the datasheet.
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER1_ISR, TIMER1_IRQn)
{
   static unsigned int high_counter=0;

   TH0 = TIMER0_RELOAD_HIGH;           // Reinit Timer0 High register
   if((high_counter++) == SIGNAL_TOGGLE_RATE)
   {
      high_counter = 0;                // Reset interrupt counter
      SIGNAL = ~SIGNAL;                // Toggle Signal
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
