//-----------------------------------------------------------------------------
// F560_Timer2_two_8bitTimers.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer2 of the C8051F560's in
// two 8-bit reload counter/timer mode.
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
//    P1.3 -> LED (toggled by TMR2L)
//    P1.5 -> SIGNAL (toggled by TMR2H)
//
//    all other port pins unused
//
// How To Test:
//
// 1) Open the F560_Timer2_two_8bitTimers.c file in the Silicon Labs IDE.
// 2) If necessary change the number of interrupts to toggle an output:
//    LED_TOGGLE_RATE
//    SIGNAL_TOGGLE_RATE
// 3) Compile and download the code.
// 4) Verify the LED pins of J19 are populated on the 'F560 TB.
// 5) Connect an oscilloscope up to SIGNAL (P1.5) to verify the correct rate.
// 5) Run the code.
// 6) Verify that the LED is blinking and SIGNAL is toggling.
//
//
// Target:         C8051F56x (Side A of C8051F560TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 / 8  MAY 2015 (DL)
//    -Updated to use with Simplicity Studio
//
// Release 1.1 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 15 JAN 2009 (GP)
//    -Initial Revision
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F550_Defs.h>
#include <SI_C8051F550_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24000000/8  // SYSCLK in Hz

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
// An interrupt occurs whenever TMR2L or TMR2H overflows from 0xFF to 0x00
// so we want the reload value to be 1 ms in timer ticks before 0xFF.
#define TIMER_PRESCALER            12  // Based on Timer2 CKCON and TMR2CN
                                       // settings

// Reload value for TMR2L or TMR2H
#define TIMER2_RELOAD  (uint8_t)(0xFF - (SYSCLK/TIMER_PRESCALER/1000))



SI_SBIT(LED, SFR_P1, 3);                 // LED==0 means ON
SI_SBIT(SIGNAL, SFR_P1, 5);              // SIGNAL to be used also by Timer2

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);           // Oscillator initialization
void Port_Init (void);                 // Port initialization routine
void Timer2_Init (void);               // Timer2 initialization routine
SI_INTERRUPT_PROTO(TIMER2_ISR, INTERRUPT_TIMER2);

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
   SFRPAGE = LEGACY_PAGE;


   OSCILLATOR_Init ();                 // Initialize Oscillator
   Timer2_Init ();                     // Initialize the Timer2
   Port_Init ();                       // Init Ports

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

   OSCICN = 0xC4;                      // Internal Oscillator / 8

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// P1.3   digital   push-pull   LED
// P1.5   digital   push-pull   SIGNAL
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   XBR2 = 0x40;                        // Enable crossbar
   P1MDOUT = 0x28;                     // Set LED and SIGNAL to
                                       // be used as push-pull

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer2 as two 8-bit reload timers with interrupt.
// Using the internal osc. at 24MHz/8 with a timer prescaler of 1:12
// reloading the TMR2H register with TIMER2_RELOAD and TMR2L with
// TIMER2_RELOAD.
//
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer2_Init(void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   CKCON &= ~0x30;                     // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RLH = (uint8_t) TIMER2_RELOAD;       // Init Timer2 Reload High register
   TMR2RLL = (uint8_t) TIMER2_RELOAD;       // Init Timer2 Reload Low register

   TMR2H = (uint8_t) TIMER2_RELOAD;         // Init Timer2 High register
   TMR2L = (uint8_t) TIMER2_RELOAD;         // Init Timer2 Low register

   TMR2CN = 0x2C;                      // Low byte interrupt enabled
                                       // Timer2 enabled
                                       // and Split-Mode enabled
   IE_ET2 = 1;                         // Timer2 interrupt enabled

   SFRPAGE = SFRPAGE_save;
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
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   static uint16_t low_counter = 0;         // Define counter variables
   static uint16_t high_counter = 0;

   if(TMR2CN_TF2L == 1)
   {
      if((low_counter++) == LED_TOGGLE_RATE)
      {
         low_counter = 0;              // Reset interrupt counter
         LED = !LED;                   // Toggle the LED
      }
      TMR2CN_TF2L = 0;
   }
   if(TMR2CN_TF2H == 1)
   {
      if((high_counter++) == SIGNAL_TOGGLE_RATE)
      {
         high_counter = 0;             // Reset interrupt counter
         SIGNAL = !SIGNAL;             // Toggle the SIGNAL pin
      }
      TMR2CN_TF2H = 0;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
