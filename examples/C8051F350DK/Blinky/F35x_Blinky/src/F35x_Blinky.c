//-----------------------------------------------------------------------------
// F35x_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Typical blinking LED
//
// This program flashes the green LED on the C8051F35x target board about
// five times a second using the interrupt handler for Timer2.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J3: P0.7 - P0.7_LED
//    - J8: Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 3) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) The code is working if the P0.7 LED D1 blinks.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 23 JUN 2014
//
// Release 1.0
//    - Initial Revision (BD)
//    - 06 FEB 2004


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F350_Register_Enums.h>  // SFR declarations

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000 / 8         // SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P0, 7);                    // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void SYSCLK_Init (void);
void PORT_Init (void);
void Timer2_Init (int counts);
SI_INTERRUPT_PROTO(Timer2_ISR, TIMER2_IRQn);

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;       // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) {


   SYSCLK_Init ();                        // Initialize system clock
   PORT_Init ();                          // Initialize crossbar and GPIO
   Timer2_Init (SYSCLK / 12 / 10);        // Init Timer2 to generate
                                          // interrupts at a 10Hz rate.

   IE_EA = 1;                             // enable global interrupts

   while (1);                             // spin forever
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
   // Internal oscillator divided by 8
   OSCICN = OSCICN_IOSCEN__ENABLED | OSCICN_IFCN__HFOSC_DIV_8;
   // enable missing clock detector
   RSTSRC = RSTSRC_MCDRSF__SET;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports.
// P0.7 - LED (push-pull)
//
void PORT_Init (void)
{
   // Enable crossbar and weak pull-ups
   XBR1     = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;
   // enable LED as a push-pull output
   P0MDOUT |= P0MDOUT_B7__PUSH_PULL;
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
void Timer2_Init (int counts)
{
   TMR2CN  = 0x00;                        // Stop Timer2; Clear TF2;
                                          // use SYSCLK/12 as timebase
   CKCON   = CKCON_T2MH__EXTERNAL_CLOCK |
             CKCON_T2ML__EXTERNAL_CLOCK;  // Timer2 clocked based on T2XCLK;

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