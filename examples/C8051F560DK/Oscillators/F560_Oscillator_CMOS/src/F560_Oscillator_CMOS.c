//-----------------------------------------------------------------------------
// F560_Oscillator_CMOS.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates how to initialize for and switch to an external
// CMOS Oscillator.
//
// P0.0 -> /SYSCLK
// P0.3 -> XTAL2 - External CMOS Oscillator input
// P1.3 -> LED
//
// How To Test:
//
// 1) Define the input CMOS clock frequency using <CMOS_clock>
// 2) Compile and download code to a 'F560 device.
// 3) Connect the CMOS input clock to XTAL2 / P0.3 (or J1 on the TB)
// 4) Run the code:
//      - the test will blink an LED at a frequency based on the CMOS
//         Oscillator
//      - the 'F560 will also output the SYSCLK to a port pin (P0.0) for
//         observation
//
//
// Target:         C8051F56x (Side A of a C8051F560-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 / 8  MAY 2015 (DL)
//    -Updated to use with Simplicity Studio
//
// Release 1.1 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 15 JAN 2009 (GP)
//    -Initial Revision

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F550_Defs.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define CMOS_clock 19000000             // CMOS oscillator frequency

// Timer2 using SYSCLK as its time base
// Timer2 counts 65536 SYSCLKs per Timer2 interrupt
// LED target flash rate = 10 Hz (may be slower, depending on CMOS_clock)

#define LED_interrupt_count CMOS_clock / 65536 / 10

SI_SBIT(LED, SFR_P1, 3);                 // LED == 1 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void TIMER2_Init (void);

SI_INTERRUPT_PROTO(TIMER2_ISR, TIMER2_IRQn);

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
   PCA0MD &= ~0x40;                    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, switches to the external CMOS
// oscillator, and loops forever, blinking the LED.
//
//-----------------------------------------------------------------------------
void main (void)
{
   SFRPAGE = LEGACY_PAGE;              // Set SFR Page for PCA0MD


   OSCILLATOR_Init ();                 // Initialize External CMOS Oscillator

   PORT_Init ();                       // Initialize crossbar and GPIO

   TIMER2_Init ();                     // Init Timer2 to generate
                                       // interrupts for the LED.

   IE_EA = 1;                          // Enable global interrupts
   while (1);                          // Spin forever
}

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes for and switches to the External CMOS Oscillator
// and also enables the missing clock detector reset.
//
// Note: In RC, capacitor, or CMOS clock configuration, the clock source
// should be wired to the XTAL2 pin as shown in Option 2, 3, or 4 in the
// Oscillators section of the datasheet.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   RSTSRC = 0x04;                      // Enable missing clock detector

   SFRPAGE = CONFIG_PAGE;

   P0MDOUT &= ~0x08;                   // Configure P0.3 as open-drain

   P0MDIN |= 0x08;                     // In CMOS clock mode, the associated
                                       // pin should be configured as a
                                       // digital input.

   OSCXCN = 0x20;                      // External Oscillator is a CMOS clock
                                       // (no divide by 2 stage)
                                       // XFCN bit settings do not apply to a
                                       // CMOS clock

   CLKSEL = 0x01;                      // Switch to the external CMOS clock

   SFRPAGE = SFRPAGE_save;
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
// P1.3   digital   push-pull     LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x01;                    // Enable /SYSCLK as a push-pull output

   P1MDOUT |= 0x08;                    // Enable LED as a push-pull output

   P0SKIP |= 0x08;                     // Skip the XTAL2 pin
   P1SKIP |= 0x08;                     // Skip the LED pin

   XBR1 = 0x02;                        // Route /SYSCLK to a port pin
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER2_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt after the
// maximum possible time (TMR2RL = 0x0000).
//
//-----------------------------------------------------------------------------
void TIMER2_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   CKCON |= 0x30;                      // Timer2 clock is system clock

   TMR2RL = 0x0000;                    // Init reload value
   TMR2 = 0xFFFF;                      // Set to reload immediately

   IE_ET2 = 1;                         // Enable Timer2 interrupts

   TMR2CN_TR2 = 1;                     // Start Timer2

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer2 overflows a
// certain number of times
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   static uint16_t count = 0;

   TMR2CN_TF2H = 0;                    // Clear Timer2 interrupt flag

   if (count == LED_interrupt_count)   // If counter reaches target overflows
   {
      LED = ~LED;                      // Change state of LED
      count = 0;                       // Reset counter
   }
   else
   {
      count++;                         // Count overflow
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
