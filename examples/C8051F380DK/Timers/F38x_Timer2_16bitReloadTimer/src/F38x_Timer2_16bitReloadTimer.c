//-----------------------------------------------------------------------------
// F38x_Timer2_16bitReloadTimer.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program presents an example of use of the Timer2 of the C8051F38x's in
// 16-bit reload counter/timer mode. It uses the 'F38xDK as HW platform.
//
// The timer reload registers are initialized with a certain value so that
// the timer counts from that value up to FFFFh and when it overflows an
// interrupt is generated and the timer is automatically reloaded.
// In this interrupt the ISR toggles the LED.
//
// Pinout:
//
//    P2.2 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J19: Power - P1_PWR or SER_PWR
//    - J7: +3VD - VDD
//    - J12: P2.2 - P2.2_LED
// 2) Connect the USB Debug Adapter to J9. If P1_PWR power is selected on J19,
//    connect the 9V power adapter to P1.
// 3) If a different LED blink rate is required change the following constant:
//       -> LED_TOGGLE_RATE in milliseconds
// 4) Compile and download code to a 'F380 device on a C8051F380-TB development
//    board selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) The LED should blink at <LED_TOGGLE_RATE>.
//
//
// Target:         C8051F380
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 09 JAN 2014
//
// Release 1.0
//    -Initial Revision (CG)
//    -08 Mar 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F380_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             12000000/8  // SYSCLK in Hz (12 MHz internal
                                       // oscillator / 8)
                                       // the internal oscillator has a
                                       // tolerance of +/- 2%

#define TIMER_PRESCALER            12  // Based on Timer2 CKCON and TMR2CN
                                       // settings

#define LED_TOGGLE_RATE            50  // LED toggle rate in milliseconds
                                       // if LED_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: LED_TOGGLE_RATE*TIMER_TICKS_PER_MS should not exceed 65535 (0xFFFF)
// for the 16-bit timer

#define AUX1     TIMER_TICKS_PER_MS*LED_TOGGLE_RATE
#define AUX2     -AUX1

#define TIMER2_RELOAD            AUX2  // Reload value for Timer2

SI_SBIT(LED, SFR_P2, 2);                 // LED='1' means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(Timer2_ISR, TIMER2_IRQn);

void Port_Init (void);                 // Port initialization routine
void Timer2_Init (void);               // Timer2 initialization routine

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

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
   PCA0MD &= ~0x40;                    // Clear watchdog timer enable
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   Timer2_Init ();                     // Initialize the Timer2
   Port_Init ();                       // Init Ports
   IE_EA = 1;                             // Enable global interrupts

   while (1);                          // Loop forever
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// Pinout:
//
//    P2.2 -> LED
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   XBR1 = 0x40;                        // Enable crossbar
   P2MDOUT = 0x04;                     // Set P2.2(LED) to push-pull
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures Timer2 as a 16-bit reload timer, interrupt enabled.
// Using the SYSCLK at 12MHz/8 with a 1:12 prescaler.
//
// Note: The Timer2 uses a 1:12 prescaler.  If this setting changes, the
// TIMER_PRESCALER constant must also be changed.
//-----------------------------------------------------------------------------
void Timer2_Init(void)
{
   CKCON &= ~0x60;                     // Timer2 uses SYSCLK/12
   TMR2CN &= ~0x01;

   TMR2RL = TIMER2_RELOAD;             // Reload value to be used in Timer2
   TMR2 = TMR2RL;                      // Init the Timer2 register

   TMR2CN = 0x04;                      // Enable Timer2 in auto-reload mode
   IE_ET2 = 1;                            // Timer2 interrupt enabled
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
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   LED = ~LED;                         // Toggle the LED
   TMR2CN_TF2H = 0;                           // Reset Interrupt
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
