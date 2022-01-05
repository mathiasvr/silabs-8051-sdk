//-----------------------------------------------------------------------------
// F326_Clock_Multiplier.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates how to configure the Clock Multiplier. A LED will
// blink at a frequency based on the system clock.
//
//
// How To Test:
//
// 1) Ensure shorting blocks on C8051F326-TB are placed on the following:
//    J2: VDD  - +3VD
//    J3: P2.0 - P2.0_SW
//    J3: P2.2 - P2.2_LED
//    J7: VDD  - VIO
//    J8: Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1.
// 3) Compile and download code to the 'F326 target board with Simplicity IDE
//    by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11
// 4) Run the code by selecting Run -> Resume from the menus
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) Press the switch S2 to switch the SYSCLK between the internal oscillator
//    (12 MHz) and the Clock Multiplier/2 (24 MHz).
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
//    -24 MAY 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F326_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define init_SYSCLK 12000000           // Initial SYSCLK setting = IntOsc

SI_SBIT(SW,  SFR_P2, 0);
SI_SBIT(LED, SFR_P2, 2);

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void CLKMUL_Init (void);
void Port_Init (void);
void Timer0_Init (void);

void Timer0_ISR (void);

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
// Main routine performs all configuration tasks, then loops forever polling on
// the SW switch and switching the CLKSEL register between the Internal
// Oscillator and the Clock Multiplier/2.
//
//-----------------------------------------------------------------------------

void main (void)
{
   SYSCLK_Init ();                     // Initialize system clock to 12MHz

   CLKMUL_Init ();                     // Initialize the Clock Multiplier

   Port_Init ();                       // Initialize crossbar and GPIO
   Timer0_Init ();                     // Init Timer0 to generate
                                       // interrupts for the LED.
                                       // When the SYSCLK is switched to
                                       // the Clock Multiplier output, the
                                       // interrupts will be at 2x the rate.

   IE_EA = 1;                          // Enable global interrupts

   while (1) {                         // Spin forever

      while (SW != 0);                 // Wait until the switch is pressed

      CLKSEL = 0x02;                   // Switch SYSCLK to the CLKMUL output
                                       // LED now blinking at 2x the rate

      while (SW == 0);                 // Wait until the switch is released



      while (SW != 0);                 // Wait until the switch is pressed

      CLKSEL = 0x00;                   // Switch SYSCLK to the IntOsc output
                                       // LED now blinking at the original
                                       // rate

      while (SW == 0);                 // Wait until the switch is released
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
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   OSCICN = 0x83;                      // Configure internal oscillator for
                                       // its highest frequency (12 MHz)

   RSTSRC = 0x04;                      // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// CLKMUL_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the Clock Multiplier using the Internal Oscillator
// as the source.
//
//-----------------------------------------------------------------------------

void CLKMUL_Init (void)
{
   char i;

   CLKMUL = 0x00;                      // Reset the Multiplier by writing 0x00
                                       // to register CLKMUL.

   CLKMUL |= 0x00;                     // Select the Multiplier input source
                                       // via the MULSEL bits.
                                       // '00'b = Internal Oscillator

   CLKMUL |= 0x80;                     // Enable the Multiplier with the MULEN
                                       // bit (CLKMUL | = 0x80).

   for (i = 35; i > 0; i--);           // Delay for >5 �s.
                                       // At 12 MHz SYSCLK ~ 80 ns, so 5 us is
                                       // 61 SYSCLK counts. DJNZ = 2 clocks.

   CLKMUL |= 0xC0;                     // Initialize the Multiplier with the
                                       // MULINIT bit (CLKMUL | = 0xC0).

   while ((CLKMUL & 0x20) != 0x20);    // Poll for MULRDY => �1�.
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
// P2.0   digital   open-drain    SW (switch)
// P2.2   digital   push-pull     LED
//
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   P2MDOUT |= 0x04;                    // Enable LED as a push-pull output

   GPIOCN = 0x40;                      // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer0_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer0 to 16-bit mode and generate an interrupt after the
// maximum possible time (TH0/TL0 = 0x0000).
//
void Timer0_Init (void)
{
   TCON_TR0 = 0;                       // Make sure Timer0 is off

   TMOD |= 0x01;                       // 16-bit mode (Mode 1)
   CKCON = 0x00;                       // Use SYSCLK/12 as the timebase

   TL0 = 0x00;                         // Init timer value
   TH0 = 0x00;

   IE_ET0  = 1;                        // Enable Timer0 interrupts

   TCON_TR0 = 1;                       // Start Timer0
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer0 overflows.
//
//-----------------------------------------------------------------------------

void Timer0_ISR (void) interrupt 1
{
   TCON_TF0 = 0;                       // Clear Timer0 interrupt flag

                                       // No need to reinitialize timer, as
                                       // it will just roll over and
                                       // continue counting

   LED = ~LED;                         // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
