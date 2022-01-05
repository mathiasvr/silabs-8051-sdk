//-----------------------------------------------------------------------------
// F35x_Timer0_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Use Timer0 as two 8-bit timers
//
// This program presents an example of use of the Timer0 of the C8051F35x's in
// two 8-bit reload counter/timer mode. It uses the 'F350DK as HW platform.
//
// It sets the two timers to interrupt every millisecond.
//
// When the timer low interrupts a counter (low_counter) increments and upon
// reaching the value defined by LED1_TOGGLE_RATE toggles P0.6 LED
// When the timer high interrupts a counter (high_counter) increments and upon
// reaching the value defined by LED2_TOGGLE_RATE toggles P0.7 LED
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J3: P0.6 - P0.6_LED (toggled by TL0)
//    - J3: P0.7 - P0.7_LED (toggled by TH0)
//    - J8: Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 3) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) Verify that the LEDs blink.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 26 JUN 2014
//
// Release 1.0
//    -Initial Revision (CG)
//    -14 DEC 2005
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>

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

#define LED1_TOGGLE_RATE          100  // LED1 toggle rate in milliseconds
                                       // if LED1_TOGGLE_RATE = 1, LED1 will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define LED2_TOGGLE_RATE           30  // LED2 toggle rate in milliseconds
                                       // if LED2_TOGGLE_RATE = 1, LED2 will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define TIMER0_RELOAD_HIGH       AUX2  // Reload value for Timer0 high byte
#define TIMER0_RELOAD_LOW        AUX2  // Reload value for Timer0 low byte

SI_SBIT(LED1, SFR_P0, 6);                // LED='1' means ON
SI_SBIT(LED2, SFR_P0, 7);                // LED='1' means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   Timer0_Init ();                     // Initialize the Timer0
   Port_Init ();                       // Init Ports
   IE_EA = 1;                          // Enable global interrupts

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
//    P0.6 -> LED1 (toggled by TL0)
//    P0.7 -> LED2 (toggled by TH0)
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;

   P0MDOUT = 0xC0;                     // Set LED1 and LED2 to
                                       // be used as push-pull
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as two 8-bit timers, interrupt enabled.
// Using the internal osc. at 24.5MHz with a prescaler of 1:8 and reloading the
// TH0 register with TIMER0_RELOAD_HIGH and TL0 with TIMER0_RELOAD_HIGH.
//
// Note: The Timer0 uses a 1:48 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
   TH0 = TIMER0_RELOAD_HIGH;           // Init Timer0 High register
   TL0 = TIMER0_RELOAD_LOW;            // Init Timer0 Low register

   // Timer0 in two 8-bit mode
   TMOD = TMOD_CT0__TIMER|
          TMOD_T0M__MODE3;

   // Timer0 uses a 1:48 prescaler
   CKCON = CKCON_T0M__PRESCALE |
           CKCON_SCA__SYSCLK_DIV_48;

   IE_ET0 = 1;                         // Timer0 interrupt enabled
   IE_ET1 = 1;                         // Timer1 interrupt enabled

   // Enable Timer0/
   TCON = TCON_TR0__RUN |
          TCON_TR1__RUN;
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer0 interrupt and toggle LED1.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer0_ISR, TIMER0_IRQn)
{
   static uint16_t low_counter=0;           // Define counter variable

   TL0 = TIMER0_RELOAD_LOW;            // Reinit Timer0 Low register
   if((low_counter++) == LED1_TOGGLE_RATE)
   {
      low_counter = 0;                 // Reset interrupt counter
      LED1 = ~LED1;                    // Toggle LED1
   }
}

//-----------------------------------------------------------------------------
// Timer1_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer1 interrupt and toggle LED2.
// Note: In this mode the Timer0 is effectively using the Timer1 interrupt
// vector. For more info check the Timers chapter of the datasheet.
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer1_ISR, TIMER1_IRQn)
{
   static uint16_t high_counter=0;

   TH0 = TIMER0_RELOAD_HIGH;           // Reinit Timer0 High register
   if((high_counter++) == LED2_TOGGLE_RATE)
   {
      high_counter = 0;                // Reset interrupt counter
      LED2 = ~LED2;                    // Toggle LED2
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------