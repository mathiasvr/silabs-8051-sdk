//-----------------------------------------------------------------------------
// F91x_Oscillator_Crystal.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the oscillator for use with an
// external quartz crystal.
//
//
// How To Test:
//
// 1) Ensure that a quartz crystal (Y1), 10Meg resistor (R7), and 33pF loading 
//    capacitors (C21, C20) are installed.
// 2) Ensure that J8[P0.2H-SW0.2] and J8[P0.3H-SW0.3] do not contain shorting
//    blocks. 
// 3) Ensure that jumpers are placed on the following:
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 4) Connect the USB Debug Adapter to J4.
// 5) Turn the POWER switch (SW5) to the "ON" position.
// 6) Specify the crystal frequency in the constant <CRYSTAL_FREQUENCY>.
// 7) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) Measure the frequency output on P0.0.
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
//    - 07 JULY 2009
//
// Release 1.0
//    - Initial Revision (FB)
//    - 15 AUG 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator Crystal Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the crystal frequency
//
#define CRYSTAL_FREQUENCY    32768     //  Crystal Frequency in Hz

// XFCN Setting Macro
#if  (CRYSTAL_FREQUENCY <= 20000)
   #define XFCN 0
#elif(CRYSTAL_FREQUENCY <= 58000)
   #define XFCN 1
#elif(CRYSTAL_FREQUENCY <= 155000)
   #define XFCN 2
#elif(CRYSTAL_FREQUENCY <= 415000)
   #define XFCN 3
#elif(CRYSTAL_FREQUENCY <= 1100000)
   #define XFCN 4
#elif(CRYSTAL_FREQUENCY <= 3100000)
   #define XFCN 5
#elif(CRYSTAL_FREQUENCY <= 8200000)
   #define XFCN 6
#elif(CRYSTAL_FREQUENCY <= 40000000)
   #define XFCN 7
#else
   #error "Crystal Frequency must be less than 40MHz"
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
// in crystal mode.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   
   OSCXCN = (0x60 | XFCN);             // start external oscillator with
                                       // the appropriate XFCN setting
                                       // based on crystal frequency

   while (!(OSCXCN & 0x80));           // Wait for crystal osc. to settle

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor reset enabled

   CLKSEL = 0x01;                      // Select external oscillator as system
                                       // clock source

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
// P0.0   digital    push-pull             /SYSCLK
// P0.2   analog     open-drain/High-Z      XTAL1
// P0.3   analog     open-drain/High-Z      XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Oscillator Pins
   P0MDIN  &= ~0x0C;                   // P0.2, P0.3 are analog
   P0SKIP  |=  0x0C;                   // P0.2, P0.3 skipped in the Crossbar
   P0MDOUT &= ~0x0C;                   // P0.2, P0.3 output mode is open-drain
   P0      |=  0x0C;                   // P0.2, P0.3 output drivers are OFF

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = 0x08;                     // Route /SYSCLK to first available pin
   XBR2    = 0x40;                     // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
