//-----------------------------------------------------------------------------
// F320_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Typical blinking LED
//
// This program flashes the green LED on the C8051F32x target board about
// five times a second using the interrupt handler for Timer2.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    J3: P2.2 - P2.2_LED
// 2) If power the C8051F320-TB from the power adapter, please connect the 9V
//    power adapter to P1, ensure J8 Pin2 is connected to Pin3, and J2 is
//    installed.
//    If power the target board from USB, please connect a USB cable to PC, and
//    ensure J11 is installed.
//    After power the board, the red PWR LED D3 is on.
// 3) Connect the USB Debug Adapter to J4.
// 4) Compile and download code to the C8051F320-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 5) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 6) If the LED flashes, the program is working.
//
//
// Target:         C8051F320/1
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.2 (JL)
//    - Updated descriptions
//    - 28 SEP 2014
//
// Release 1.1 / 10 MAR 2002 (TP)
//    -Updated Format
//
// Release 1.0 / 06 NOV 2002 (HF)
//    -Initial Revision


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F320_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       12000000 / 8      // SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P2, 2);                  // LED='1' means ON

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
   PCA0MD &= ~PCA0MD_WDTE__BMASK;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{
   // Disable Watchdog timer

   SYSCLK_Init ();                     // Initialize system clock
   PORT_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init (SYSCLK / 12 / 10);     // Init Timer2 to generate
                                       // interrupts at a 10Hz rate.

   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Spin forever
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value: none
// Parameters:   none
//
// This routine initializes the system clock to use the internal 12MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   // configure internal oscillator for its lowest frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_8;
   RSTSRC = 0x04;                      // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value: none
// Parameters:   none
//
// Configure the Crossbar and GPIO ports.
//
// P2.2 - LED (push-pull)
//
// All other port pins unused
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P2MDOUT |= 0x04;                    // Enable LED as a push-pull output

   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value: none
// Parameters:   none
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
//-----------------------------------------------------------------------------
void Timer2_Init (int counts)
{
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON  &= ~(CKCON_T2MH__BMASK |
               CKCON_T2ML__BMASK );       // Timer2 clocked based on TMR2CN_T2XCLK;

   TMR2RL = -counts;                   // Init reload values
   TMR2 = 0xffff;                      // Set to reload immediately
   IE_ET2 = 1;                         // Enable Timer2 interrupts
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
   LED = !LED;                         // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------