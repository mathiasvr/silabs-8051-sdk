//-----------------------------------------------------------------------------
// F50x_Timer0_two_8bitTimers.c.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Use Timer0 as two 8-bit timers
//
// This program presents an example of use of the Timer0 of the C8051F50x's in
// two 8-bit counter/timer mode.
//
// It sets Timer0 to be two timers to interrupt every millisecond.
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
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    J19: P1.3 LED
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 2) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 3) Compile and download code to C8051F500 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) Connect an oscilloscope to SIGNAL (P1.5).
//    Verify that the LED is blinking and SIGNAL is toggling.
//
//
// Target:         C8051F50x/1x (Side A of C8051F500TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.2 / 11 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.1 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0
//    -Initial Revision (ADT)
//    -19 JUN 2008


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>

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
// An interrupt occurs whenever TL0 or TH0 overflows from 0xFF to 0x00
// so we want the reload value to be 1 ms in timer ticks before 0xFF.
#define TIMER_PRESCALER            12  // Based on Timer0 CKCON and TMR2CN
                                       // settings

// Reload value for TL0 or TH0
#define TIMER_RELOAD  (uint8_t)(0xFF - (SYSCLK/TIMER_PRESCALER/1000))

SI_SBIT(LED,    SFR_P1, 3);              // LED='1' means ON
SI_SBIT(SIGNAL, SFR_P1, 5);              // SIGNAL to be used also by Timer0

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);                 // Port initialization routine
void TIMER0_Init (void);               // Timer0 initialization routine

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
   SFRPAGE = LEGACY_PAGE;

   OSCILLATOR_Init ();                 // Initialize Oscillator
   TIMER0_Init ();                     // Initialize the Timer0
   PORT_Init ();                       // Initialize Ports

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

   // Configure internal oscillator for 24 MHz / 8
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_8;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// P1.3   digital   push-pull   LED
// P1.5   digital   push-pull   SIGNAL
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_SAVE = SFRPAGE;          // Save Current SFR page

   SFRPAGE = CONFIG_PAGE;              // Set SFR page

   // Set P1.3(LED) and P1.5(SIGNAL) to be used as push-pull
   P1MDOUT = 0x28;

   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}

//-----------------------------------------------------------------------------
// TIMER0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Timer0 as two 8-bit timers, interrupt enabled.
// Using the internal osc. at 24MHz/8 with a prescaler of 1:12 and reloading the
// TH0 register with TIMER0_RELOAD_HIGH and TL0 with TIMER0_RELOAD_HIGH.
//
// Note: The Timer0 uses a 1:12 prescaler. If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void TIMER0_Init (void)
{

   TH0 = TIMER_RELOAD;                 // Init Timer0 High register
   TL0 = TIMER_RELOAD;                 // Init Timer0 Low register

   TMOD = TMOD_T0M__MODE3;             // Timer0 in two 8-bit mode
   CKCON = CKCON_SCA__SYSCLK_DIV_12;   // Timer0 uses a 1:12 prescaler
   IE_ET0 = 1;                         // Timer0 interrupt enabled
   IE_ET1 = 1;                         // Timer1 interrupt enabled
   TCON = TCON_TR1__RUN |
          TCON_TR0__RUN;               // Timer0 ON

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
   static uint16_t low_counter=0;           // Define counter variable

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
   static uint16_t high_counter=0;

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