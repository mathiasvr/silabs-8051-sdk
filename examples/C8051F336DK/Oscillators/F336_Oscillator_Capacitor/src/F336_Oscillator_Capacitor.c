//-----------------------------------------------------------------------------
// F336_Oscillator_Capacitor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the oscillator for use with an
// external capacitor.
//
//
// How To Test:
//
// 1) Ensure that a capacitor (C14) is installed.
// 2) Specify the target frequency in the constant <C_FREQUENCY>.
// 3) Download code to an 'F338 target board.
// 4) Measure the frequency output on P0.0.
//
// Choosing Component Values:
//
//       frequency (Hz) is proportional to [ KF(XFCN) / (C * VDD) ]
//
//       where KF(XFCN) is a factor based on XFCN.
//
// The actual frequency is best determined by measurement. In this example,
// a 33pF capacitor was used with VDD = 3.3V. Component values are best
// chosen by trial and error.
//
// The resulting frequencies achievable using 33pF and 3.3V VDD are:
//
//       frequency   |   XFCN
//      ----------------------
//           6 kHz   |    0
//          17 kHz   |    1
//          51 kHz   |    2
//         144 kHz   |    3
//         420 kHz   |    4
//         1.2 MHz   |    5
//         4.4 MHz   |    6
//        11.6 MHz   |    7
//
//
// Target:         C8051F336/7/8/9
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.1 (JL)
//    - Updated description
//    - 25 JAN 2014
//
// Release 1.0
//    -Initial Revision (SM)
//    -01 AUG 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F336_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator RC Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the target frequency
//
#define C_FREQUENCY   11600000           // Target RC Frequency in Hz

// XFCN Setting Macro
#define XFCN             0             // XFCN/KF setting.

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);

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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
                                       // enable)

   PORT_Init();                        // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   while (1) {};                       // loop forever

}                                      // end of main()

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
// This function initializes the system clock to use the external oscillator
// in C mode.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{

   int i;                              // delay counter

   OSCXCN = (0x50 | XFCN);             // start external oscillator with
                                       // the appropriate XFCN setting
                                       // in C Mode

   for (i = 0; i < 256; i++);          // Wait for C osc. to start

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // VDD Monitor reset

   CLKSEL = 0x01;                      // Select external oscillator as system
                                       // clock source

   OSCICN = 0x00;                      // Disable the internal oscillator.
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// The oscillator pins should be configured as analog inputs when using the
// external oscillator in crystal mode.
//
// P0.0   digital    push-pull     /SYSCLK
// P0.3   analog     don't care     XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Oscillator Pins
   P0MDIN &= ~0x08;                    // P0.3 is analog
   P0SKIP |= 0x08;                     // P0.3 skipped in the Crossbar

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = 0x08;                     // Route /SYSCLK to first available pin
   XBR1    = 0x40;                     // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
