//-----------------------------------------------------------------------------
// F70x_Blinky.c
//-----------------------------------------------------------------------------
// Copyright (C) 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program flashes the yellow LED on the C8051F700 target board about
// five times a second using the interrupt handler for Timer2.
//
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J8: P1.0 - DS3
//	  - J15: VDD - power source
// 2) Connect the USB Debug Adapter to P3.
// 3) Connect the USB cable to P4.
// 4) Compile and download code to the 'F70x target board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) The green LED on P1.0 will blink at a rate of about 5 Hz.
//
// Target:         C8051F700
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (PD)
//    -7 JUL 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"             // compiler declarations
#include <SI_C8051F700_Defs.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(GREEN_LED,    SFR_P1, 0);        // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000/8        // SYSCLK frequency in Hz

#define LED_ON           0
#define LED_OFF          1

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void PORT_Init (void);
void Timer2_Init (int counts);
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
   WDTCN = 0xDE;                       // WDTE = 0 (clear watchdog timer
   WDTCN = 0xAD;                       // enable)
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   PORT_Init ();

   Timer2_Init (SYSCLK / 12 / 10);     // Init Timer2 to generate interrupts
                                       // at a 10 Hz rate.

   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Spin forever
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P1.0   digital   push-pull     Green LED
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;
   
   SFRPAGE = CONFIG_PAGE;
   P1MDOUT |= 0x01;                    // P1.0 is push-pull

   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/48 as its time base.
//
void Timer2_Init (int counts)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;
   
   SFRPAGE = CONFIG_PAGE;

   TMR2CN  = 0x00;                      // Stop Timer2; Clear TF2;
                                        // use SYSCLK/12 as timebase
   CKCON  &= ~0x60;                     // Timer2 clocked based on T2XCLK;

   TMR2RL  = -counts;                   // Init reload values
   TMR2    = 0xffff;                    // set to reload immediately
   IE_ET2     = 1;                      // enable Timer2 interrupts
   TMR2CN_TR2 = 1;                      // start Timer2

   SFRPAGE = SFRPAGE_save;
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
	TMR2CN_TF2H = 0;                    // clear Timer2 interrupt flag
   GREEN_LED = !GREEN_LED;              // change state of LEDs

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
