//-----------------------------------------------------------------------------
// F540_Timer0_8bitReloadTimer.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes the green LED on the C8051F540 target board about
// once a second using the interrupt handler for Timer0.
//
//
// How To Test:
//
// 1) Download code to the 'F540 target board
// 2) Ensure that the P1.3 pins are shorted together on the J16 header
// 3) Run the program.  If the LED flashes, the program is working
//
//
// Target:         C8051F540 (Side A of a C8051F540-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
// Release 1.2 / 14 MAY 2015 (SY)
//    -Update SFR/bit name for Simplicity Studio
//
// Release 1.1 / 10 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 06 NOV 2008 (GP)
//    -Initial Revision

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F540_Defs.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24000000/8        // SYSCLK frequency in Hz

#define BLINK_RATE   244               // Number of 4.096 ms intervals between
                                       // toggling the LED pin
                                       // 244 intervals is about 1 per second

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P1, 3);                 // LED ='1' means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void TIMER0_Init (void);

SI_INTERRUPT_PROTO(Timer0_ISR, TIMER0_IRQn);

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
   PCA0MD &= ~0x40;                    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = LEGACY_PAGE;              // Set SFRPAGE for PCA0MD


   OSCILLATOR_Init ();                 // Configure system clock
   PORT_Init ();                       // Initialize crossbar
   TIMER0_Init ();                     // Initialize Timer0

   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Spin forever
}


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

   OSCICN = 0xC4;

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P1.3 - digital  push-pull   LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P1MDOUT |= 0x08;                    // Enable LED as a push-pull output
   P1SKIP  |= 0x08;                    // Skip the LED pin on the crossbar

   XBR2     = 0x40;                    // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure Timer0 to 8-bit auto-reload (mode 3) and generate an interrupt
// every 256 Timer0 cycles using SYSCLK/48 as the Timer0 time base.
//
// With the system clock at 3 MHz, the Timer0 time base is 62.5 kHz.  Timer0
// will overflow every 4.096 ms
//
//-----------------------------------------------------------------------------

void TIMER0_Init (void)
{
   // No need to set SFRPAGE as all registers accessed in this function
   // are available on all pages

   TH0 = 0x00;                         // Init Timer0 reload register
   TL0 = TH0;                          // Set the intial Timer0 value

   TMOD  = 0x02;                       // Timer0 in 8-bit reload mode
   CKCON = 0x02;                       // Timer0 uses a 1:48 prescaler
   IE_ET0   = 1;                       // Timer0 interrupt enabled
   TCON  = 0x10;                       // Timer0 ON
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer0 overflows.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(Timer0_ISR, TIMER0_IRQn)
{
   // The Timer0 interrupt flag is automatically cleared by vectoring to
   // the Timer0 ISR

   static uint8_t count = 0;
   count++;                            // Increment when an interrupt occurs

   if (count == BLINK_RATE)
   {
      LED = !LED;                      // Change state of LED
      count = 0;                       // Clear the counter
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
