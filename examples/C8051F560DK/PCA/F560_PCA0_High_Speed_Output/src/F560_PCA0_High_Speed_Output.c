//-----------------------------------------------------------------------------
// F560_PCA0_High_Speed_Output.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program sends a square wave out on an I/O pin, using the PCA's
// High-Speed Output Mode.
//
// In this example, PCA Module 0 is used to generate the waveform, and the
// crossbar is configured to send the CEX0 pin out on P0.0.
//
// How To Test:
//
// 1) Download code to a 'F560 device which has an oscilloscope monitoring P0.0
// 2) Make sure there is no jumper on J22.
// 3) Run the program - the waveform should be visible on the oscilloscope.
//    The frequency measured should be the same as <CEX0_FREQUENCY>.
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
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F550_Defs.h>
#include <SI_C8051F550_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24000000          // Internal oscillator frequency in Hz

#define CEX0_FREQUENCY  10000          // Frequency to output in Hz

#define DIVIDE_RATIO (SYSCLK/CEX0_FREQUENCY/2) // SYSCLK cycles per interrupt

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PCA0_Init (void);

SI_INTERRUPT_PROTO(PCA0_ISR, INTERRUPT_PCA0);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint16_t Next_Compare_Value;                // Next edge to be sent out in HSO mode

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
   PCA0MD = 0x00;                      // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = LEGACY_PAGE;              // Set SFR Page for PCA0 registers


   PORT_Init ();                       // Initialize crossbar and GPIO
   OSCILLATOR_Init ();                 // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   IE_EA = 1;                          // Globally enable interrupts

   while (1);                          // Spin here to wait for ISR
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
// This function initializes the system clock to use the internal oscillator
// at 24 MHz.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0xC7;                      // Set internal oscillator to run
                                       // at its maximum frequency

   CLKSEL = 0x00;                      // Select the internal osc. as
                                       // the SYSCLK source

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P0.0   digital   push-pull     PCA0 CEX0
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x01;                    // Set CEX0 (P0.0) to push-pull

   XBR1    = 0x04;                     // Enable CEX0 on crossbar
   XBR2    = 0x40;                     // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up High-Speed output
// mode for Module 0 (CEX0 pin).
//
// The frequency of the square wave generated at the CEX0 pin is defined by
// the parameter CEX0_FREQUENCY.
//
// The maximum square wave frequency output for this example is about 230 kHz.
// The minimum square wave frequency output for this example is about 190 Hz
//
// The PCA time base in this example is configured to use SYSCLK, and SYSCLK
// is set up to use the internal oscillator running at 24 MHz.
// Using different PCA clock sources or a different processor clock will
// result in a different frequency for the square wave, and different
// maximum and minimum options.
//
//    -------------------------------------------------------------------------
//    How "High Speed Output Mode" Works:
//
//       The PCA's High Speed Output Mode works by toggling the output pin
//    associated with the module every time the PCA0 register increments and
//    the new 16-bit PCA0 counter value matches the module's capture/compare
//    register (PCA0CPn). When initially enabled in high-speed output mode, the
//    CEXn pin associated with the module will be at a logic high state.  The
//    first match will cause a falling edge on the pin.  The next match will
//    cause a rising edge on the pin, and so on.
//
//    By loading the PCA0CPn register with the next match value every time a
//    match happens, arbitrary waveforms can be generated on the pin with high
//    levels of precision.
//    -------------------------------------------------------------------------
//
// When setting the capture/compare register for the next edge value, the low
//  byte of the PCA0CPn register (PCA0CPLn) should be written first, followed
//  by the high byte (PCA0CPHn).  Writing the low byte clears the ECOMn bit,
//  and writing the high byte will restore it.  This ensures that a match does
//  not occur until the full 16-bit value has been written to the compare
//  register.  Writing the high byte first will result in the ECOMn bit being
//  set to '0' after the 16-bit write, and the next match will not occur at
// the correct time.
//
// It is best to update the capture/compare register as soon after a match
//  occurs as possible so that the PCA counter will not have incremented past
//  the next desired edge value. This code implements the compare register
//  update in the PCA ISR upon a match interrupt.
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x08;                      // Use SYSCLK as time base

   PCA0CPM0 = 0x4D;                    // Module 0 = High Speed Output mode,
                                       // Enable Module 0 Interrupt flag,
                                       // Enable ECOM bit

   PCA0L = 0x00;                       // Reset PCA Counter Value to 0x0000
   PCA0H = 0x00;

   PCA0CPL0 = DIVIDE_RATIO & 0x00FF;   // Set up first edge
   PCA0CPH0 = (DIVIDE_RATIO & 0xFF00) >> 8;

   // Set up the variable for the following edge
   Next_Compare_Value = PCA0CP0 + DIVIDE_RATIO;

   EIE1 |= 0x08;                       // Enable PCA interrupts

   PCA0CN_CR = 1;                      // Start PCA

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This is the ISR for the PCA.  It handles the case when a match occurs on
// channel 0, and updates the PCA0CPn compare register with the value held in
// the global variable "Next_Compare_Value".
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
   if (PCA0CN_CCF0)                     // If Module 0 caused the interrupt
   {
	  PCA0CN_CCF0 = 0;                  // Clear module 0 interrupt flag.

      PCA0CPL0 = (Next_Compare_Value & 0x00FF);
      PCA0CPH0 = (Next_Compare_Value & 0xFF00) >> 8;

      // Set up the variable for the following edge
      Next_Compare_Value = PCA0CP0 + DIVIDE_RATIO;
   }
   else                                // Interrupt was caused by other bits.
   {
      PCA0CN &= ~0xBE;                 // Clear other interrupt flags for PCA
   }
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
