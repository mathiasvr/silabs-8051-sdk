//-----------------------------------------------------------------------------
// F41x_Timer2_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer2 of the C8051F41x's in
// two 8-bit reload counter/timer mode. It uses the 'F410DK as HW platform.
//
// It sets Timer2 to be two 8-bit timers that interrupt every millisecond.
//
// When the Timer2 low interrupts a counter (low_counter) increments and upon
// reaching the value defined by LED1_TOGGLE_RATE toggles LED1
// When the Timer2 high interrupts a counter (high_counter) increments and upon
// reaching the value defined by LED2_TOGGLE_RATE toggles LED2
//
// Pinout:
//
//    P2.1 -> LED1 (toggled by TMR2L)
//
//    P2.3 -> LED2 (toggled by TMR2H)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    J9:   VUNREG (9V adapter) or 5VEC3 (Debugger) 
//    J10:  +3VD/+3VD
//    J12:  +3VD/VREGIN
//          VREGIN_EN
//    J17:  5VEC3/VIO or +3VD/VIO
//          VIO_EN
//    J5:   P2.1/D3
//          P2.3/D5
// 2) Connect the USB Debug Adapter to J4. If VUNREG is selected on J9,
//    connect the 9V power adapter to P1.
// 3) If a different LED blink rate is required change the following constants:
//       -> LED1_TOGGLE_RATE in milliseconds
//       -> LED2_TOGGLE_RATE in milliseconds
// 4) Compile and download code to the C8051F410-TB development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in the
//    quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8. 
// 6) LED D3 should blink at approximately 5 Hz.
//    LED D5 should blink at approximately 15 Hz.
//
//
// Target:         C8051F41x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 specific code with compiler agnostic code from 
//      compiler_defs.h.
//    - 21 JAN 2014
//
// Release 1.1 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0
//    -Initial Revision (CG)
//    -08 Mar 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F410_Register_Enums.h>

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

#define LED1_TOGGLE_RATE          100  // LED1 toggle rate in milliseconds
                                       // if LED1_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define LED2_TOGGLE_RATE           30  // LED2 toggle rate in milliseconds
                                       // if LED2_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define TIMER2_RELOAD_HIGH       AUX2  // Reload value for Timer2 high byte
#define TIMER2_RELOAD_LOW        AUX2  // Reload value for Timer2 low byte

SI_SBIT(LED1, SFR_P2, 1);                 // LED='1' means ON
SI_SBIT(LED2, SFR_P2, 3);                 // LED2 to be used also by Timer2

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

   OSCICN |= 0x04;                     // Force Internal Osc. 1:8 prescaler
   while ((OSCICN & 0x40 ) == 0);      // Wait until stable

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
//    P2.1 -> LED (toggled by TMR2L)
//
//    P2.3 -> SIGNAL (toggled by TMR2H)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   XBR1 = 0x40;                        // Enable crossbar
   P2MDOUT = 0x0A;                     // Set LED and SIGNAL to
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
// matches and, upon a match, toggles LED1 and LED2.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, 5)
{
   static uint16_t low_counter=0;  // Define counter variables
   static uint16_t high_counter=0;

   if(TMR2CN_TF2L == 1)
   {
      if((low_counter++) == LED1_TOGGLE_RATE)
      {
         low_counter = 0;              // Reset interrupt counter
         LED1 = ~LED1;                 // Toggle LED1
      }
      TMR2CN_TF2L = 0;
   }
   if(TMR2CN_TF2H == 1)
   {
      if((high_counter++) == LED2_TOGGLE_RATE)
      {
         high_counter = 0;             // Reset interrupt counter
         LED2 = ~LED2;                 // Toggle LED2
      }
      TMR2CN_TF2H = 0;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------