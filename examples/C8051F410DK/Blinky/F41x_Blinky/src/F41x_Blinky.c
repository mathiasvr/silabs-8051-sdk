//-----------------------------------------------------------------------------
// F41x_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes the green LED on the C8051F41x target board about
// five times a second using the interrupt handler for Timer2.
//
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
// 2) Connect the USB Debug Adapter to J4. If VUNREG is selected on J9,
//    connect the 9V power adapter to P1.
// 3) Compile and download code to the C8051F410-TB development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in the
//    quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) The P2.1 LED (D3) should be flashing at approximately 5 Hz.
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
//    -Initial Revision (GP)
//    -22 DEC 2005
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F410_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000 / 64     // SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P2, 1);                  // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(Timer2_ISR, TIMER2_IRQn);

void OSCILLATOR_Init (void);
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
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void) {

   // disable watchdog timer
                                       // enable)

   OSCILLATOR_Init ();                 // Initialize system clock to
   PORT_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init (SYSCLK / 10);          // Init Timer2 to generate
                                       // interrupts at a 10Hz rate.

   IE_EA = 1;                          // Enable global interrupts

   while (1) {                         // Spin forever
   }
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5MHz / 64
// oscillator as its clock source.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   OSCICN = 0x81;                      // Configure internal oscillator
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P2.1   digital   push-pull     LED
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   XBR0     = 0x00;                    // No digital peripherals selected
   XBR1     = 0x40;                    // Enable crossbar and weak pull-ups
   P2MDOUT |= 0x02;                    // Enable LED as a push-pull output
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1)  int16_t counts - calculated Timer overflow rate
//                    range is postive range of integer: 0 to 32767
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK as its time base.
//
//-----------------------------------------------------------------------------
void Timer2_Init (int16_t counts)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
   CKCON   = 0x30;                     // Timer2 clocked based on SYSCLK

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xffff;                   // Set to reload immediately
   IE_ET2  = 1;                        // Enable Timer2 interrupts
   TMR2CN_TR2 = 1;                     // Start Timer2
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
//This routine changes the state of the LED whenever Timer2 overflows.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag
   LED = ~LED;                         // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------