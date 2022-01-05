//-----------------------------------------------------------------------------
// F800_Ports_PortMatch.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to use the Port Match feature.  The firmware
// enables the port match interrupt and puts the device in idle mode.  Pressing
// a switch wakes the device up from idle mode and turns on an LED in the Port
// Match ISR.  Releasing the switch turns off the LED and the re-enters idle
// mode.
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J3: P1.0_LED - P1.0
//    - J3: P1.4_SW - P1.4
//    - J2
// 2) Connect the USB Debug Adapter to J4.
// 3) Compile and download code to the C8051F800-TB by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 5) Push and hold the switch (P1.4) and see that the LED is on
// 6) Release the switch and see that the LED turns off
//
//
//
// Target:         C8051F800
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 / 08 JUL 2008 (PD)
//    -Initial Revision

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"
#include <SI_C8051F800_Defs.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P1, 0);                 // LED == 1 means ON
SI_SBIT(SW1, SFR_P1, 4);                 // SW1 == 0 means switch depressed

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PORT_Match_Init (void);
void SYSCLK_Init (void);

SI_INTERRUPT_PROTO(PORT_Match_ISR, INTERRUPT_PORT_MATCH);

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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   PORT_Init ();                       // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   SYSCLK_Init ();					   // Initialize System Clock

   PORT_Match_Init ();                 // Enable the Port Match interrupt

   IE_EA = 1;                          // Enable global interrupts

   while (1)
   {
	   // Put the device in idle mode
      LED = 0;                         // Turn off LED
      PCON |= 0x01;                    // Enter Idle mode
      PCON = PCON;                     // Safety dummy instruction

      // Pressing the switch pulls P1.1 low which triggers the Port Match
      // interrupt.  When the Port Match interrupt occurs, the device wakes up
      // from Idle mode and vectors to the Port_Match_ISR

      // Port Match is a level sensitive interrupt, so the firmware waits for
      // the switch to be released in the ISR before proceeding.

      // Once the ISR is finished, code execution resumes after the instruction
      // that put the device in idle mode
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal precision
// oscillator at its maximum frequency and enables the missing clock
// detector.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   OSCICN |= 0x80;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
}
//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal
// oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   OSCICN |= 0x83;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
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
// P1.0   digital   push-pull     LED
// P1.1   digital   open-drain    SW1
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   P1MDIN |= 0x11;                     // P1.0 is digital
                                       // P1.4 is digital

   P1MDOUT |= 0x01;                    // P1.0 is push-pull
   P1MDOUT &= ~0x10;                   // P1.4 is open-drain

   P1     |= 0x10;                     // Set P1.4 latch to '1'

   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}

//-----------------------------------------------------------------------------
// PORT_Match_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Enable the Port Match interrupt for P1.4.  Whenever P1.4 is logic low, the
// Port Match interrupt is active.
//
//-----------------------------------------------------------------------------

void PORT_Match_Init (void)
{
   P1MASK = 0x10;                      // Set P1.4 as the pin to monitor
   P1MAT  = 0x10;                      // Interrupt when P1.4 is low

   EIE1 |= 0x02;                       // Enable Port Match Interrupts
}

//-----------------------------------------------------------------------------
// Interrupt Service Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Match_ISR
//-----------------------------------------------------------------------------
//
// Whenever the Port_Match ISR is active, turn on the LED.  Since the Port
// Match interrupt is level sensitive and not edge sensitive, the ISR
// waits for the switch to be released  and the interrupt is cleared before
// exiting Otherwise, the Port_Match ISR would constantly be triggered until
// the switch was released.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(PORT_Match_ISR, 8)
{
   LED = 1;                            // Turn on LED

   while (SW1 == 0);                   // Wait for switch release
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
