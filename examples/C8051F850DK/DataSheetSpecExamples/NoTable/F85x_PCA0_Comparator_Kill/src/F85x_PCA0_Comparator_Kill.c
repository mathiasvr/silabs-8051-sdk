//-----------------------------------------------------------------------------
// F85x_PCA0_Comparator_Kill.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Refer to F85x_PCA0_Comparator_Kill_Readme.txt for more information 
// regarding this code example's functionality. 
//
//
// How To Test (CPA0 - Comparator Kill Feature):
// ---------------------------------------------
// 1) Connect probes from an oscilloscope monitoring P0.0. P0.0 corresponds to
//    the PCA0 output, CEX0.
// 2) Connect the POT header pin to P0.1 on your board.
// 3) Ensure that the P1.7-S1 (J27) header pins have a shorting block attached
//    to them. 
// 4) Remove all other shorting blocks, excluding the JP2 (Imeasure) shorting
//    block.
// 5) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Based on the current state of the potentiometer, the oscilloscope will
//    either display a square wave PCA0 output on P0.0 or nothing.
// 8) Modify the preprocessor #define, CMP_POLARITY, to toggle the program's 
//    comparator polarity mode. This mode determines which comparator state 
//    (high or low) will disable the PCA0 output.
//
//
// Target:         C8051F85x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 5 FEB 2014
//
// Release 1.0
//    - Initial Revision (PMD)
//    - 8 JUL 2013
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Internal oscillator frequency in Hz
#define CEX0_FREQUENCY  50000          // Frequency to output on CEX0 (Hz)

// ** USER MUST ENTER THEIR CHOICE FOR THIS DEFINE FIELD **

#define CMP_POLARITY        0          // Comparator clear polarity (0 / 1) 

// ********************************************************

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void Port_Init (void);
void PCA0_Init (void);
void CMP0_Init (void);

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
   WDTCN = 0xDE;                       // Disable watchdog timer
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   CMP0_Init ();                       // Initialize Comparator 0
   Port_Init ();                       // Initialize crossbar and GPIO
   SYSCLK_Init ();                     // Initialize oscillator
   PCA0_Init ();                       // Initialize PCA0

   while (1){}                         
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMP0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initializes comparator to serve as killswitch for CPA0.
//
//-----------------------------------------------------------------------------
void CMP0_Init(void)
{
   CPT0CN = 0x80;                      // Comparator enabled
   CPT0MX = 0x18;                      // P0.1 = Negative Input
                                       // LDO = Positive Input
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator divided by 1 as its clock source.  Also enables missing clock
// detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz
   RSTSRC = 0x06;                      // Enable missing clock detector
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
// P0.0   digital   push-pull   output   PCA0 (CEX0)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   XBR0 = 0x00;
   XBR1 = 0x01;                        // Route CEX0 to P0.0
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   P0MDOUT |= 0x01;                    // Set CEX0 (P0.0) to push-pull
}

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the PCA time base, and sets up frequency output
// mode for Module 0 (CEX0 pin).
//
// The frequency generated at the CEX0 pin is equal to CEX0_FREQUENCY Hz,
// which is defined in the "Global Constants" section at the beginning of the
// file.
//
// The PCA time base in this example is configured to use SYSCLK / 12.
// The frequency range that can be generated using this example is ~4 kHz to
// ~1 MHz when the processor clock is 24.5 MHz.  Using different PCA clock
// sources or a different processor clock will generate different frequency
// ranges.
//
//    -------------------------------------------------------------------------
//    How "Frequency Output Mode" Works:
//
//    The PCA's Frequency Output Mode works by toggling an output pin every
//    "N" PCA clock cycles.  The value of "N" should be loaded into the PCA0CPH
//    register for the module being used (in this case, module 0).  Whenever
//    the register PCA0L (PCA counter low byte) and the module's PCA0CPL value
//    are equivalent, two things happen:
//
//    1) The port pin associated with the PCA module toggles logic state
//    2) The value stored in PCA0CPH is added to PCA0CPL.
//
//    Using this mode, a square wave is produced at the output port pin of the
//    PCA module. The speed of the waveform can be changed by changing the
//    value of the PCA0CPH register.
//    -------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x00;                      // Use SYSCLK/12 as time base

   PCA0CPM0 = 0x46;                    // Module 0 = Frequency Output mode

   // Configure frequency for CEX0
   // PCA0CPH0 = (SYSCLK/12)/(2*CEX0_FREQUENCY), where:
   // SYSCLK/12 = PCA time base
   // CEX0_FREQUENCY = desired frequency
   PCA0CPH0 = (SYSCLK/12)/(2*CEX0_FREQUENCY);

   PCA0CLR |= 0x01;                    // Set Comparator Clear ON
 
   if( CMP_POLARITY == 1 )
   {
      PCA0CLR |= 0x80;                 // Set CMP Polarity = 1
   }
   else
   {
      PCA0CLR &= ~(0x80);              // Set CMP Polarity = 0 
   }

   // Start PCA counter
   PCA0CN_CR = 1;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
