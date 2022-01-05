//-----------------------------------------------------------------------------
// F36x_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Typical blinking LED
//
// This program flashes the green LED on the C8051F36x target board about
// five times a second using the interrupt handler for Timer2.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.2 - P3.2_LED
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) The code is working if the P3.2 LED D4 blinks.
//
//
// Target:         C8051F36x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 29 JUL 2014
//
// Release 1.0
//    -Initial Revision (TP)
//    -12 DEC 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK         24500000 / 8    // Approximate SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P3, 2);                  // Green LED: '1' = ON; '0' = OFF

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void PORT_Init (void);
void Timer2_Init (uint16_t counts);
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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable Watchdog timer
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

   SFRPAGE = LEGACY_PAGE;              // Page to sit in for now

   while (1) {}                        // Spin forever
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports
//
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;            // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;                // Switch to the necessary SFRPAGE

   XBR1    = XBR1_XBARE__ENABLED;        // Enable crossbar and weak pull-ups
   P3MDOUT |= 0x04;                      // enable P3.2 (LED) as push-pull output

   SFRPAGE = SFRPAGE_save;               // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1)  int counts - calculated Timer overflow rate
//                    range is postive range of integer: 0 to 32767
//
// Configure Timer2 to auto-reload and generate an interrupt at interval
// specified by <counts> using SYSCLK/12 as its time base.
//
//
void Timer2_Init (uint16_t counts)
{
   uint8_t SFRPAGE_save = SFRPAGE;            // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;                // Switch to the necessary SFRPAGE

   TMR2CN = 0x00;                        // Stop Timer2; Clear TF2;
                                         // Use SYSCLK/12 as timebase
   TMR2RL = -counts;                     // Init reload values
   TMR2 = 0xFFFF;                        // Set to reload immediately

   IE_ET2 = 1;                           // Enable Timer2 interrupts

   TMR2CN_TR2 = 1;                       // Start Timer2

   SFRPAGE = SFRPAGE_save;               // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
// This routine changes the state of the LED whenever Timer2 overflows.
//
// NOTE: The SFRPAGE register will automatically be switched to the Timer 2
// Page when an interrupt occurs.  SFRPAGE will return to its previous setting
// on exit from this routine.
//
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag
   LED = ~LED;                         // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------