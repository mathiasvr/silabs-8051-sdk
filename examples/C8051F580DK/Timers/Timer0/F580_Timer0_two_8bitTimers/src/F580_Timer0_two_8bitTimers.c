//-----------------------------------------------------------------------------
// F580_Timer0_two_8bitTimers.c.c
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program presents an example of use of the Timer0 of the C8051F580's in
// two 8-bit counter/timer mode.
//
// It sets the two timers to interrupt every millisecond.
//
// When the Timer0 low interrupts a counter (low_counter) increments and upon
// reaching the value defined by LED_TOGGLE_RATE toggles the LED
// When the Timer0 high interrupts a counter (high_counter) increments and upon
// reaching the value defined by SIGNAL_TOGGLE_RATE toggles the SIGNAL pin
//
// Pinout:
//
//    P1.3 -> LED (toggled by TL0)
//
//    P1.5 -> SIGNAL (toggled by TH0)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Open the F580_Timer0_two_8bitTimers.c file in the Silicon Labs IDE.
// 2) If desired, change the number of interrupts (ms) to toggle an output:
//    LED_TOGGLE_RATE
//    SIGNAL_TOGGLE_RATE
// 3) Compile and download the code.
// 4) Verify J19 is populated on the 'F580 TB.
// 5) Connect an oscilloscope to SIGNAL (P1.5)
// 6) Run the code.
// 7) Verify that the LED is blinking and SIGNAL is toggling.
//
//
// Target:         C8051F580 (Side A of C8051F580TB)
// Tool chain:     KEIL C51 8.01 / KEIL EVAL C51
// Command Line:   None
//
// Release 1.1 / 10 JUL 2015 (SG)
//    - Revised for Simplicity Studio
//
// Release 1.0
//    -Initial Revision (ADT)
//    -19 JUN 2008
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F580_Register_Enums.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24000000/8  // SYSCLK in Hz (24 MHz internal
                                       // oscillator / 8)

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

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
// An interrupt occurs whenever TL0 or TH0 overflows from 0xFF to 0x0000
// so we want the reload value to be 1 ms in timer ticks before 0xFF.
#define TIMER_PRESCALER            48  // Based on Timer2 CKCON and TMR2CN
                                       // settings

// Reload value for TL0 or TH0
#define TIMER_RELOAD  (uint16_t)(0xFF - (SYSCLK/TIMER_PRESCALER/1000))

SI_SBIT(LED, P1, 3);                     // LED='1' means ON
SI_SBIT(SIGNAL, P1, 5);                  // SIGNAL to be used also by Timer0

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void Port_Init (void);                 // Port initialization routine
void Timer0_Init (void);               // Timer0 initialization routine

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
   SFRPAGE = ACTIVE1_PAGE;

   OSCILLATOR_Init ();                 // Initialize Oscillator
   Timer0_Init ();                     // Initialize the Timer0
   Port_Init ();                       // Initialize Ports

   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Loop forever
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the internal oscillator to maximum internal frequency / 8, which
// is 3 MHz
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x84;

   SFRPAGE = SFRPAGE_save;
}

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
//    P1.3 -> LED (toggled by TL0)
//
//    P1.5 -> SIGNAL (toggled by TH0)
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_SAVE = SFRPAGE;          // Save Current SFR page

   SFRPAGE = CONFIG_PAGE;              // Set SFR page

   XBR2 = 0x40;                        // Enable crossbar
   P1MDOUT = 0x28;                     // Set P1.3(LED) and P1.5(SIGNAL) to
                                       // be used as push-pull
   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as two 8-bit timers, interrupt enabled.
// Using the internal osc. at 24MHz with a prescaler of 1:48 and reloading the
// TH0 register with TIMER0_RELOAD_HIGH and TL0 with TIMER0_RELOAD_HIGH.
//
// Note: The Timer0 uses a 1:48 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{

   TH0 = TIMER_RELOAD;                 // Init Timer0 High register
   TL0 = TIMER_RELOAD;                 // Init Timer0 Low register

   TMOD = 0x03;                        // Timer0 in two 8-bit mode
   CKCON = 0x02;                       // Timer0 uses a 1:48 prescaler
   IE_ET0 = 1;                         // Timer0 interrupt enabled
   IE_ET1 = 1;                         // Timer1 interrupt enabled
   TCON = 0x50;                        // Timer0 ON

   // Timer 0 and 1 can be accessed on all SFR pages.
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
   static uint16_t low_counter = 0;         // Define counter variable

   TL0 = TIMER_RELOAD;                 // Reinit Timer0 Low register
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
   static uint16_t high_counter = 0;

   TH0 = TIMER_RELOAD;                 // Reinit Timer0 High register
   if((high_counter++) == SIGNAL_TOGGLE_RATE)
   {
      high_counter = 0;                // Reset interrupt counter
      SIGNAL = ~SIGNAL;                // Toggle Signal
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
