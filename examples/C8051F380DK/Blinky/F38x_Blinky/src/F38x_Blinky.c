//-----------------------------------------------------------------------------
// F38x_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes the P2.2 green LED on the C8051F38x target board
// five times a second using the interrupt handler for Timer2.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J19: Power - P1_PWR or SER_PWR
//    - J7: +3VD - VDD
//    - J12: P2.2 - P2.2_LED
// 2) Connect the USB Debug Adapter to J9. If P1_PWR power is selected on J19,
//    connect the 9V power adapter to P1.
// 3) Compile and download code to a 'F380 device on a C8051F380-TB development
//    board selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Observe that LED1 (P2.2) blinks rapidly.
//
//
//
// Target:         C8051F380
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 09 JAN 2014
//
//
// Release 1.0 - 10-FEB-2011 (PKC)
//    -Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F380_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       12000000 / 8      // SYSCLK frequency in Hz

#define BLINK_RATE   10                // Timer2 Interrupts per second

SI_SBIT(LED1, SFR_P2, 2);                // LED='1' means ON

//-----------------------------------------------------------------------------
// Function Prototypes
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
   PCA0MD &= ~0x40;                         // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
                                            // enable)

   OSCILLATOR_Init ();                      // Initialize system clock
   PORT_Init ();                            // Initialize crossbar and GPIO

   Timer2_Init (SYSCLK / 12 / BLINK_RATE);  // Init Timer2 to generate
                                            // interrupts at a 10Hz rate.

   IE_EA = 1;                               // Enable global interrupts

   while (1) {}                             // Spin forever
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
// This function initializes the system clock to (12 Mhz / 8)
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   OSCICN = 0x80;                      // Configure internal oscillator for
                                       // its lowest frequency
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
// P2.2   digital   push-pull     LED1
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   XBR0     = 0x00;                    // No digital peripherals selected
   XBR1     = 0x40;                    // Enable crossbar and weak pull-ups
   P2MDOUT |= 0x04;                    // Enable LED as a push-pull output
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
// interval specified by <counts> using SYSCLK/12 as its time base.
//
//-----------------------------------------------------------------------------
void Timer2_Init (int16_t counts)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // Use SYSCLK/12 as timebase
   CKCON  &= ~(CKCON_T2MH__BMASK |
               CKCON_T2ML__BMASK );       // Timer2 clocked based on TMR2CN_T2XCLK;

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
// This routine changes the state of the LED whenever Timer2 overflows.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag
   LED1 = !LED1;                       // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------