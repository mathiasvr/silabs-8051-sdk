//-----------------------------------------------------------------------------
// F326_Oscillator_CMOS.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates how to initialize for and switch to an external
// CMOS Oscillator. A LED will blink at a frequency based on the CMOS clock.
//
//
// How To Test:
//
// 1) Ensure shorting blocks on C8051F326-TB are placed on the following:
//    J2: VDD  - +3VD
//    J3: P2.2 - P2.2_LED
//    J7: VDD  - VIO
//    J8: Power selection, P1 9V or J4 Debugger
// 2) Connect the CMOS input clock to XTAL2 pin of J10.
// 3) Define the input CMOS clock frequency using <CMOS_clock>
// 4) Compile and download code to the 'F326 target board with Simplicity IDE
//    by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11
// 5) Run the code by selecting Run -> Resume from the menus
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 6) LED D4 will blink and the system clock output on P0.0
//
//
// Target:         C8051F326/7
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 03 APR 2014
//
// Release 1.0
//    -Initial Revision (TP)
//    -1 JUNE 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F326_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define CMOS_clock 3062500             // CMOS oscillator frequency

// Timer1 using SYSCLK/12 as its time base
// Timer1 counts 65536 SYSCLKs per Timer1 interrupt
// LED target flash rate = 10 Hz (may be slower, depending on CMOS_clock)
//
// If CMOS_clock is too slow to divide into a number of counts,
// <count> will always remain 0. In this case, the LED Flash rate will be
// slower than 10 Hz.
#define LED_interrupt_count CMOS_clock/12/65536/10

SI_SBIT(LED, SFR_P2, 2);

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void ExtCMOSOsc_Init (void);
void Port_Init (void);
void Timer1_Init (void);

void Timer1_ISR (void);

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
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, switches to the external CMOS
// oscillator, and loops forever, blinking the LED.
//
void main (void) {

   SYSCLK_Init ();                     // Initialize system clock to 12MHz

   ExtCMOSOsc_Init ();                 // Initialize for and switch to the
                                       // external CMOS oscillator

   Port_Init ();                       // Initialize crossbar and GPIO

   Timer1_Init ();                     // Init Timer1 to generate
                                       // interrupts for the LED.

   IE_EA = 1;                          // Enable global interrupts

   while (1) {                         // Spin forever
   }
}

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 12 MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
void SYSCLK_Init (void)
{
   OSCICN = 0x83;                      // Configure internal oscillator for
                                       // its highest frequency (12 MHz)

   RSTSRC = 0x04;                      // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// ExtCMOSOsc_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine switches to the External CMOS Oscillator.
//
// Note: In CMOS clock configuration, the clock source should be wired to the
// XTAL2 pin as shown in the Oscillators section of  the datasheet.
//
void ExtCMOSOsc_Init (void)
{
   CLKSEL = 0x01;                      // Switch to the external CMOS clock
}

//-----------------------------------------------------------------------------
// Port_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P0.0   digital   push-pull     /SYSCLK
// P0.3   digital   open-drain    XTAL2 - External CMOS Oscillator input
// P2.2   digital   push-pull     LED
//
void PORT_Init (void)
{
   P0MDOUT |= 0x01;                    // Enable /SYSCLK as a push-pull output

   P2MDOUT |= 0x04;                    // Enable LED as a push-pull output

   GPIOCN |= 0x01;                     // Route /SYSCLK to P0.0
   GPIOCN |= 0x80;                     // Enable weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer1_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer1 to 16-bit mode and generate an interrupt after the
// maximum possible time.
//
void Timer1_Init (void)
{
   TCON = 0x00;                        // Stop Timer1; Clear TCON_TF1

   TMOD = 0x10;                        // Timer1 in 16-bit mode

   CKCON = 0x00;                       // Timer1 uses SYSCLK/12

   TH1 = 0x00;                         // Interrupt after the maximum time
   TL1 = 0x00;

   IE_ET1 = 1;                         // Enable Timer1 interrupts

   TCON_TR1 = 1;                       // Start Timer1
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
void Timer1_ISR (void) interrupt 3
{
   static unsigned int count = 0;

   TCON_TF1 = 0;                       // Clear Timer1 interrupt flag

   TH1 = 0x00;                         // Reload Timer1 to interrupt after the
   TL1 = 0x00;                         // maximum time

   if (count == LED_interrupt_count)
   {
      LED = ~LED;                      // Change state of LED

      count = 0;
   }
   else
   {
      count++;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
