//-----------------------------------------------------------------------------
// F80x_SuspendMode_ComparatorWake.c
//-----------------------------------------------------------------------------
// Copyright (C) 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program blinks N LED using Timer 2 whenever the configured
// comparator output is positive.  When the comparator toggles to a
// negative value, the comparator ISR sets the device to suspend mode.
// Whenever the comparator's output toggles back to a positive value,
// the comparator wakes the device and blinking resumes.
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) Ensure that jumpers are placed on the following:
//    - J3: P1.0_LED - P1.0
//    - J2
// 2) Connect the USB Debug Adapter to J4.
// 3) Connect P1.1 on J1 to the on-board potentiometer (J6) or to an external
//    voltage source set to a mid-rail voltage. Connect P1.2 on J1 to P1.4_SW on J3.
// 4) Compile and download code to a 'F80x device by selecting Run -> Debug
//    from the menus, clicking the Debug button in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Press P1.4 to place the device in sleep mode.
//    Press P1.4 again to wake it back up. The LED will turn on when
//    the device is awake.
//
//
// Target:         C8051F80x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (PD)
//    -10 JUL 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"             // compiler declarations
#include <SI_C8051F800_Defs.h>         // SFR declarations


//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED,    SFR_P1, 0);              // '0' means ON, '1' means OFF

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK             24500000L


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
	PCA0MD &= ~0x40;                   // WDTE = 0 (disable watchdog timer)
}
 
//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void);
void Comparator0_Init(void);
void PORT_Init (void);
void Timer2_Init (void);
SI_INTERRUPT_PROTO(TIMER2_ISR, INTERRUPT_TIMER2);
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{


   PORT_Init ();                       // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator
   Comparator0_Init();                 // Initialize Comparator0
   Timer2_Init();                      // Initialize Timer 2

   IE_EA = 1;                          // Enable global interrupts

   //----------------------------------
   // Main Application Loop
   //----------------------------------
   while (1)
   {
      OSCICN |= 0x20;                  // Place MCU in suspend mode
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal high power
// oscillator.  The function also configures the external clock FR to
// use an external CMOS oscillator, which will be used to clock Timer 3.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   OSCXCN    = 0x20;
   CLKSEL = 0x00;                      // Select internal osc.
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
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P1MDIN &= ~0x06;                    // P1.1, P1.2 are analog

   P1MDOUT |= 0x01;                    // P1.0 is push-pull

   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}


//-----------------------------------------------------------------------------
// Comparator0_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the comparator to pins P1.1 and P1.2
//-----------------------------------------------------------------------------

void Comparator0_Init (void)
{
   uint8_t i;
   CPT0CN = 0x80;                      // Comparator enabled

   for (i = 0; i < 35; i++);           // Wait 10us for initialization

   CPT0CN    &= ~0x30;
   CPT0MX = 0x45;                      // P1.1 = Negative Input
                                       // P1.2 = Positive Input

   for (i = 0; i < 35; i++);           // Wait 10us for initialization
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer2 as a 16-bit reload timer, interrupt enabled.
// Using the SYSCLK at 24.5MHz/8 with a 1:12 prescaler.
//
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer2_Init(void)
{
   CKCON &= ~0x60;                     // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RL = -(SYSCLK/12/1000);         // Reload value to be used in Timer2
   TMR2 = TMR2RL;                      // Init the Timer2 register

   TMR2CN = 0x04;                      // Enable Timer2 in auto-reload mode
   TMR2RLH = 0x02;
   TMR2RLL = 0x00;
   IE_ET2 = 1;                         // Timer2 interrupt enabled
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// Here we process the Timer2 interrupt and toggle the LED
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
   LED = !LED;                         // Toggle the LED
   TMR2CN_TF2H = 0;                    // Reset Interrupt
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
