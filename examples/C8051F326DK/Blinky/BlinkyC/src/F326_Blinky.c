//-----------------------------------------------------------------------------
// F326_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes the P2.2 green LED on the C8051F326 target board
// five times a second using the interrupt handler for Timer1.
//
//
// How To Test:
//
// 1) Ensure shorting blocks on C8051F326-TB are placed on the following:
//    J2: VDD  - +3VD
//    J3: P2.2 - P2.2_LED
//    J7: VDD  - VIO
//    J8: Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1.
// 3) Compile and download code to C8051F326 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) The code is working if the P2.2 LED D4 blinks.
//
//
// Target:         C8051F326/7
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 02 APR 2014
//
// Release 1.0
//    -Initial Revision (GP)
//    -01 DEC 2005
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F326_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK         12000000 / 8    // SYSCLK frequency in Hz
#define REFRESH_RATE   SYSCLK / 4 / 10 // Generate interrupts at 10 Hz
#define TIMER1_COUNT   -REFRESH_RATE
#define TIMER1_RELOAD_HIGH       ((TIMER1_COUNT & 0xFF00 ) >> 8)
#define TIMER1_RELOAD_LOW         (TIMER1_COUNT & 0x00FF)

SI_SBIT(LED, SFR_P2, 2);                  // LED='1' means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void Timer1_Init (void);
SI_INTERRUPT_PROTO(Timer1_ISR, TIMER1_IRQn);

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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   OSCILLATOR_Init ();                 // Initialize system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   Timer1_Init ();                     // Init Timer1 to generate
                                       // interrupts at a 10Hz rate.

   IE_EA = 1;                          // Enable global interrupts

   while (1) {}                        // Spin forever
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
// This routine initializes the system clock to use the internal 12MHz / 8
// oscillator as its clock source.
//
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void)
{
   OSCICN = 0x80;                      // Configure internal oscillator
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the GPIO ports.
//
// P2.2   digital   push-pull     LED1
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   P2MDOUT |= 0x04;                    // Enable LED as a push-pull output
}

//-----------------------------------------------------------------------------
// Timer1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer1 to 16-bit mode and generate an interrupt at
// interval specified by <counts> using SYSCLK/4 as its time base.
//
//-----------------------------------------------------------------------------
void Timer1_Init (void)
{
   TCON     = 0x00;                     // Stop Timer1; Clear TCON_TF1;
   TH1      = TIMER1_RELOAD_HIGH;       // Initialize the timer start value.
   TL1      = TIMER1_RELOAD_LOW;

   TMOD     = 0x10;                     // Timer1 is a Mode 1: 16-bit timer;

   CKCON    = 0x01;                     // Timer 0/1 clocked by SYSCLK/4

   IE_ET1   = 1;                        // Enable Timer1 interrupts
   TCON_TR1 = 1;                        // Start Timer1
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer1_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer1 overflows.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(Timer1_ISR, TIMER1_IRQn)
{
   TCON_TR1 = 0;                        // Stop Timer1
   LED      = ~LED;                     // Change state of LED
   TH1      = TIMER1_RELOAD_HIGH;       // Initialize the timer start value.
   TL1      = TIMER1_RELOAD_LOW;
   TCON_TR1 = 1;                        // Start Timer1
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------