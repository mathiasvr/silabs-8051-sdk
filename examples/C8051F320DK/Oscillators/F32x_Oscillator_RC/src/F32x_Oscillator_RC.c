//-----------------------------------------------------------------------------
// F32x_Oscillator_RC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the oscillator for use with an
// external RC Oscillator.
//
//
// How To Test:
//
// 1) Ensure that a capacitor (C14) and resistor (R7) are installed on the 
//    C8051F320 target board.
// 2) Specify the target frequency in the constant <RC_FREQUENCY>.
// 3) Download code to an 'F320 target board.
// 4) Measure the frequency output on P0.0.
//
// Choosing Component Values:
//
//       frequency (Hz) = 1 / (1.05 * R * C )
//
//       where R is in Ohms
//             C is in Farads
//
//       Note: Add 2pF stray capacitance to C.
//
// For a resistor value of 330K, and a capacitor value of 33pF, the
// expected RC_FREQUENCY is 82.5 kHz. The actual measured frequency
// on the target board was 83 kHz.
//
//
// Target:         C8051F32x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (FB)
//    -15 AUG 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F320_Register_Enums.h>


//-----------------------------------------------------------------------------
// External Oscillator RC Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the target frequency
//
#define RC_FREQUENCY   83000           //  Target RC Frequency in Hz

// XFCN Setting Macro
#if  (RC_FREQUENCY <= 25000)
   #define XFCN 0
#elif(RC_FREQUENCY <= 50000)
   #define XFCN 1
#elif(RC_FREQUENCY <= 100000)
   #define XFCN 2
#elif(RC_FREQUENCY <= 200000)
   #define XFCN 3
#elif(RC_FREQUENCY <= 400000)
   #define XFCN 4
#elif(RC_FREQUENCY <= 800000)
   #define XFCN 5
#elif(RC_FREQUENCY <= 1600000)
   #define XFCN 6
#elif(RC_FREQUENCY <= 3200000)
   #define XFCN 7
#else
   #error "RC Frequency must be less than or equal to 3.2 MHz"
#endif

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

   while (1) {};                       // Loop forever
}                                      // End of main()

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
// in RC mode.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   int i;                              // Delay counter

   OSCXCN = (0x40 | XFCN);             // Start external oscillator with
                                       // the appropriate XFCN setting
                                       // based on RC frequency

   for (i=0; i < 256; i++);            // Wait for RC osc. to start

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
