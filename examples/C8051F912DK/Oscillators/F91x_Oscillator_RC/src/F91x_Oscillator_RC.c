//-----------------------------------------------------------------------------
// F91x_Oscillator_RC.c
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
// 1) Ensure that jumpers are placed on the following:
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Ensure that a capacitor (C21) and resistor (R16) are installed.
// 3) Ensure that J8[P0.3H-SW0.3] does not contain a shorting block.
// 4) Connect the USB Debug Adapter to J4.
// 5) Turn the POWER switch (SW5) to the "ON" position.
// 6) Specify the target frequency in the constant <RC_FREQUENCY>. 
// 7) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) Measure the frequency output on P0.0.
//
// Choosing Component Values:
//
//       frequency (Hz) = 1.23 x 10^9 / ( R * C )
//
//       where R is in KOhms
//             C is in pF
//
//       Note: Add 2pF stray capacitance to C.
//       
//       Note: Resistor should be 10K Ohm or larger.
//       Note: Capacitor should be 100 pF or smaller. 
//
// For a resistor value of 300K, and a capacitor value of 33pF, the
// expected RC_FREQUENCY is 112.9 kHz. The actual measured frequency
// on the target board was 38 kHz.
//
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 14 JAN 2014
//
// Release 1.2 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.1
//    - Port to C8051F912 from C8051F930 (JH)
//    - Compile and test C8051F912_defs.h on 912 board (JH)
//    - 10 JULY 2009
//
//
// Release 1.0
//    - Initial Revision (FB)
//    - 15 AUG 2007

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator RC Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the target frequency
//
#define RC_FREQUENCY   36000         //  Target RC Frequency in Hz

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

   while (1);                          // Infinite Loop

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
// This function initializes the system clock to use the external oscillator
// in RC mode.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{

   OSCXCN = (0x40 | XFCN);             // start external oscillator with
                                       // the appropriate XFCN setting
                                       // based on RC frequency

   while (!(OSCXCN & 0x80));           // Wait for RC osc. to settle

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // VDD Monitor reset

   CLKSEL = 0x01;                      // Select external oscillator as system
                                       // clock source

   OSCICN = 0x00;                      // Disable the internal oscillator.
   //OSCICN = 0x0F;
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
// P0.0   digital    push-pull            /SYSCLK
// P0.3   analog     open-drain/High-Z     XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Oscillator Pin 
   P0MDIN  &= ~0x08;                   // P0.3 is analog
   P0SKIP  |=  0x08;                   // P0.3 skipped in the Crossbar
   P0MDOUT &= ~0x08;                   // P0.3 output mode is open-drain
   P0      |=  0x08;                   // P0.3 output drivers are OFF

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = 0x08;                     // Route /SYSCLK to first available pin
   XBR2    = 0x40;                     // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
