//-----------------------------------------------------------------------------
// F80x_Blinky.c
//-----------------------------------------------------------------------------
// Copyright (C) 2009, Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program flashes the yellow LED on the C8051F800 target board about
// five times a second using the interrupt handler for Timer2.
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J3: P1.0_LED - P1.0
//    - J2
// 2) Connect the USB Debug Adapter to J4.
// 3) Compile and download code to the 'F80x UDP MCU board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 5) The P1.0 LED should be blinking at approximately 5 Hz.
//
//
// Target:         C8051F800
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 - 15 JUL 2009 (PKC)
//    -Initial Revision.
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"             // compiler declarations
#include <SI_C8051F800_Defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(GREEN_LED,    SFR_P1, 0);        // '1' means ON, '0' means OFF

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000/8        // SYSCLK frequency in Hz

#define LED_ON           1
#define LED_OFF          0

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void Oscillator_Init (void);
void PORT_Init (void);
void Timer2_Init (int counts);
SI_INTERRUPT_PROTO(Timer2_ISR, INTERRUPT_TIMER2);

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
   PCA0MD &= ~0x40;                    // WDTE = 0 (disable watchdog timer)
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   
   Oscillator_Init ();
   PORT_Init ();

   Timer2_Init (SYSCLK / 12 / 10);     // Init Timer2 to generate interrupts
                                       // at a 10 Hz rate.

   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Spin forever
}

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz / 8.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   OSCICN |= 0x80;                     // Enable the precision internal osc.
   OSCICN &= ~0x03;                    // IFCN=00b ( = div by 8)
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
   P1MDOUT |= 0x01;                    // P1.0 is push-pull

   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/48 as its time base.
//
//-----------------------------------------------------------------------------
void Timer2_Init (int counts)
{
   TMR2CN  = 0x00;                        // Stop Timer2; Clear TF2;
                                          // use SYSCLK/12 as timebase
   CKCON  &= ~0x60;                       // Timer2 clocked based on T2XCLK;

   TMR2RL  = -counts;                     // Init reload values
   TMR2    = 0xffff;                      // set to reload immediately
   IE_ET2     = 1;                        // enable Timer2 interrupts
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
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
	TMR2CN_TF2H = 0;                      // clear Timer2 interrupt flag
   GREEN_LED = !GREEN_LED;                // change state of LEDs
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
