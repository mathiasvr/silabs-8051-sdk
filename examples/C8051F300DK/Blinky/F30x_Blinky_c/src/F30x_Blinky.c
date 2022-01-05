//-----------------------------------------------------------------------------
// F30x_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// This program flashes the green LED on the C8051F30x target board about 
// five times a second using the interrupt handler for Timer2.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J3: P0.2 - P0.2_LED
// 2) Connect the USB Debug Adapter to J4.
// 3) Connect the 9V power adapter to P1.
// 4) Compile and download code to a 'F300 device on a C8051F300-TB development
//    board selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) The LED should blink at 5 Hz.
//
// Target:         C8051F30x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 09 JAN 2014
//
// Release 1.0
//    -Initial Revision BW
//    -11-Oct-01
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F300_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000 / 8          // SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P0, 2);                     // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(Timer2_ISR, TIMER2_IRQn);
void SYSCLK_Init (void);
void PORT_Init (void);
void Timer2_Init (int16_t counts);

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
   PCA0MD &= ~0x40;                       // WDTE = 0 (clear watchdog timer 
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {

   // disable watchdog timer
                                          // enable)

   SYSCLK_Init ();                        // Initialize system clock to 
                                          // 24.5MHz
   PORT_Init ();                          // Initialize crossbar and GPIO
   Timer2_Init (SYSCLK / 12 / 10);        // Init Timer2 to generate 
                                          // interrupts at a 10Hz rate.

   IE_EA = 1;                             // enable global interrupts

   while (1) {                            // spin forever
   }
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock to use the internal 24.5MHz / 8 
// oscillator as its clock source.  Also enables missing clock detector reset.
//
void SYSCLK_Init (void)
{
   OSCICN = 0x04;                         // configure internal oscillator for
                                          // its lowest frequency
   RSTSRC = 0x04;                         // enable missing clock detector
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports.
// P0.2 - LED (push-pull)

void PORT_Init (void)
{
   XBR0     = 0x04;                       // skip P0.2 (LED) in crossbar pin
                                          // assignments
   XBR1     = 0x00;                       // no digital peripherals selected
   XBR2     = 0x40;                       // Enable crossbar and weak pull-ups
   P0MDOUT |= 0x04;                       // enable LED as a push-pull output
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 
// interval specified by <counts> using SYSCLK/12 as its time base.
//
void Timer2_Init (int16_t counts)
{
   TMR2CN  = 0x00;                        // Stop Timer2; Clear TF2;
                                          // use SYSCLK/12 as timebase
   CKCON  &= ~(CKCON_T2MH__BMASK |
               CKCON_T2ML__BMASK );       // Timer2 clocked based on TMR2CN_T2XCLK

   TMR2RL  = -counts;                     // Init reload values
   TMR2    = 0xffff;                      // set to reload immediately
   IE_ET2  = 1;                           // enable Timer2 interrupts
   TMR2CN_TR2 = 1;                        // start Timer2
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
// This routine changes the state of the LED whenever Timer2 overflows.
//
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                       // clear Timer2 interrupt flag
   LED = ~LED;                            // change state of LED
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------