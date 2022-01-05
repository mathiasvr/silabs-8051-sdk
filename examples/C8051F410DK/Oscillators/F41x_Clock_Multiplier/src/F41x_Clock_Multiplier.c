//-----------------------------------------------------------------------------
// F41x_Clock_Multiplier.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates how to initialize and switch to the C8051F41x
// Clock Multiplier.
// - Pinout:
//    P1.4 -> SW
//
//    P2.1 -> LED
//
//    P3.0 -> C2D (debug interface)
//
//    all other port pins unused
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
//          P1.4/SW2
// 2) Connect the USB Debug Adapter to J4. If VUNREG is selected on J9,
//    connect the 9V power adapter to P1.
// 3) Compile and download code to the C8051F410-TB development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in the
//    quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//      - the test will blink an LED at a frequency based on the SYSCLK
//      - press the switch on P1.4 to switch the SYSCLK between the internal
//        oscillator/8 (3.0625 MHz) and the Clock Multiplier*(2/6) (16.33 MHz)
//        using the IntOsc/2 as the input source.
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
//    -Initial Revision (TP)
//    -25 MAY 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F410_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define init_SYSCLK 24500000 / 8       // Initial SYSCLK setting = IntOsc

SI_SBIT(LED, SFR_P2, 1);                  // LED='1' means ON
SI_SBIT(SW, SFR_P1, 4);                   // SW='0' means switch pressed

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(Timer2_ISR, 5);

void SYSCLK_Init (void);
void CLKMUL_Init (void);
void Port_Init (void);
void Timer2_Init (void);

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
                                       // enable)

   SYSCLK_Init ();                     // Initialize system clock to 12MHz

   CLKMUL_Init ();                     // Initialize the Clock Multiplier

   Port_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init ();                     // Init Timer2 to generate
                                       // interrupts for the LED.
                                       // When the SYSCLK is switched to
                                       // the Clock Multiplier output, the
                                       // interrupts will be at 2x the rate.

   IE_EA = 1;                             // Enable global interrupts

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
// This routine initializes the system clock to use the internal 24.5 MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   OSCICN = 0x84;                      // Configure internal oscillator for
                                       // (24.5 / 8 MHz) = 3.062

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
                                       // '00'b = Internal Oscillator/2

   CLKMUL |= 0x18;                     // Select the Multiplier output scaling
                                       // factor via the MULDIV bits
                                       // '110'b = scale by 2/6
                                       // Clock Multiplier output =
                                       // (IntOsc/2) * 2/6 = 16.33 MHz

   CLKMUL |= 0x80;                     // Enable the Multiplier with the MULEN
                                       // bit (CLKMUL | = 0x80).

   for (i = 12; i > 0; i--);           // Delay for >5 µs.
                                       // At 3.0625 MHz SYSCLK ~ 326 ns, so
                                       // 5 us is 16 SYSCLK counts.
                                       // DJNZ = 2 clocks.

   CLKMUL |= 0xC0;                     // Initialize the Multiplier with the
                                       // MULINIT bit (CLKMUL | = 0xC0).

   while ((CLKMUL & 0x20) != 0x20);    // Poll for MULRDY => ‘1’.
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
// P1.4   digital   open-drain    SW (switch)
// P2.1   digital   push-pull     LED
//
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   P2MDOUT |= 0x02;                    // Enable LED as a push-pull output

   XBR0     = 0x00;                    // No digital peripherals selected
   XBR1     = 0x40;                    // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Timer2_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt after the
// maximum possible time (TMR2RL = 0x0000).
//
//-----------------------------------------------------------------------------

void Timer2_Init (void)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase

   CKCON  &= ~0x30;                    // Timer2 clocked based on TMR2CN_T2XCLK;

   TMR2RL  = 0x0000;                   // Init reload value
   TMR2    = 0xffff;                   // set to reload immediately

   IE_ET2     = 1;                        // Enable Timer2 interrupts

   TMR2CN_TR2     = 1;                        // Start Timer2
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
SI_INTERRUPT(Timer2_ISR, 5)
{
   TMR2CN_TF2H = 0;                           // Clear Timer2 interrupt flag

   LED = ~LED;                         // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------