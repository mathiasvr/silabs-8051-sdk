//-----------------------------------------------------------------------------
// F31x_Oscillator_CMOS.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates how to initialize for and switch to an external
// CMOS Oscillator.
// - Pinout:
//    P0.0 -> /SYSCLK
//
//    P0.3 -> XTAL2 - External CMOS Oscillator input
//
//    P3.3 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J8: Power - 9V or Debugger
//    - J3: P3.3_LED
//    - J10: XTAL2
// 2) Define the input CMOS clock frequency using <CMOS_clock>
// 3) Connect the CMOS input clock to XTAL2 / P0.3 (or J10 on the TB)
// 4) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1.
// 5) Compile and download code to the C8051F310-TB development board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8:
//      - the test will blink an LED at a frequency based on the CMOS
//         Oscillator
//      - the 'F31x will also output the SYSCLK to a port pin (P0.0) for
//         observation
//
//
// Target:         C8051F31x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 specific code with compiler agnostic code from 
//      compiler_defs.h.
//    - 16 JAN 2014
//
// Release 1.0
//    -Initial Revision (TP)
//    -30 MAY 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F310_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define CMOS_clock 3062500             // CMOS oscillator frequency

// Timer2 using SYSCLK/12 as its time base
// Timer2 counts 65536 SYSCLKs per Timer2 interrupt
// LED target flash rate = 10 Hz (may be slower, depending on CMOS_clock)
//
// If CMOS_clock is too slow to divide into a number of counts,
// <count> will always remain 0. In this case, the LED Flash rate will be
// slower than 10 Hz.
#define LED_interrupt_count CMOS_clock/12/65536/10

SI_SBIT(LED, SFR_P3, 3);                  // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(Timer2_ISR, 5);

void SYSCLK_Init (void);
void ExtCMOSOsc_Init (void);
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
// Main routine performs all configuration tasks, switches to the external CMOS
// oscillator, and loops forever, blinking the LED.
//
void main (void) {

                                       // enable)

   SYSCLK_Init ();                     // Initialize system clock to 24.5MHz

   ExtCMOSOsc_Init ();                 // Initialize for and switch to the
                                       // external CMOS oscillator

   Port_Init ();                       // Initialize crossbar and GPIO

   Timer2_Init ();                     // Init Timer2 to generate
                                       // interrupts for the LED.

   IE_EA = 1;                             // Enable global interrupts

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
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
void SYSCLK_Init (void)
{
   OSCICN = 0x83;                      // Configure internal oscillator for
                                       // its highest frequency (24.5 MHz)

   RSTSRC = 0x04;                      // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// ExtCMOSOsc_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes for and switches to the External CMOS Oscillator.
//
// Note: In RC, capacitor, or CMOS clock configuration, the clock source
// should be wired to the XTAL2 pin as shown in Option 2, 3, or 4 in the
// Oscillators section of the datasheet.
//
void ExtCMOSOsc_Init (void)
{
   P0MDIN |= 0x08;                     // In CMOS clock mode, the associated
                                       // pin should be configured as a
                                       // digital input.

   OSCXCN = 0x20;                      // External Oscillator is a CMOS clock
                                       // (no divide by 2 stage)
                                       // XFCN bit settings do not apply to a
                                       // CMOS clock

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
// P0.1                           unused
// P0.2                           unused
// P0.3   digital   open-drain    XTAL2 - External CMOS Oscillator input
// P0.4                           unused
// P0.5                           unused
// P0.6                           unused
// P0.7                           unused
//
// P1                             unused
//
// P2                             unused
//
// P3.0                           unused
// P3.1                           unused
// P3.2                           unused
// P3.3   digital   push-pull     LED
// P3.4                           unused
//
void PORT_Init (void)
{
   P0MDOUT |= 0x01;                    // Enable /SYSCLK as a push-pull output

   P3MDOUT |= 0x08;                    // Enable LED as a push-pull output

   P0SKIP |= 0x08;                     // Skip the XTAL2 pin

   XBR0 = 0x08;                        // Route /SYSCLK to a port pin
   XBR1 = 0x40;                        // Enable crossbar and weak pull-ups
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
void Timer2_Init (void)
{
   TMR2CN = 0x00;                      // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase

   CKCON &= ~0x30;                     // Timer2 clocked based on TMR2CN_T2XCLK;

   TMR2RL = 0x0000;                    // Init reload value
   TMR2 = 0xffff;                      // Set to reload immediately

   IE_ET2 = 1;                            // Enable Timer2 interrupts

   TMR2CN_TR2 = 1;                            // Start Timer2
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
   static uint16_t count = 0;

   TMR2CN_TF2H = 0;                           // Clear Timer2 interrupt flag

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