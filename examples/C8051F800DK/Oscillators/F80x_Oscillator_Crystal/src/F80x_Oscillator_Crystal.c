//-----------------------------------------------------------------------------
// F80x_Oscillator_Crystal.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure the oscillator for use with an
// external quartz crystal.
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J2
//    - J9
//    - J10
// 2) Ensure that a quartz crystal, 10Meg resistor, and 33pF loading capacitors
//    are installed.
// 3) Connect the USB Debug Adapter to J4.
// 4) Specify the crystal frequency in the constant <CRYSTAL_FREQUENCY>.
// 5) Compile and download code to the 'F80x UDP MCU board by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 7) Measure the frequency output on P0.0.
//
// Target:         C8051F80x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (PD)
//    -07 JUL 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"
#include <SI_C8051F800_Defs.h>         // SFR declarations

//-----------------------------------------------------------------------------
// External Oscillator Crystal Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the crystal frequency
//
#define CRYSTAL_FREQUENCY    32000     //  Crystal Frequency in Hz

// XFCN Setting Macro
#if  (CRYSTAL_FREQUENCY <= 32000)
   #define XFCN 0
#elif(CRYSTAL_FREQUENCY <= 84000)
   #define XFCN 1
#elif(CRYSTAL_FREQUENCY <= 225000)
   #define XFCN 2
#elif(CRYSTAL_FREQUENCY <= 590000)
   #define XFCN 3
#elif(CRYSTAL_FREQUENCY <= 1500000)
   #define XFCN 4
#elif(CRYSTAL_FREQUENCY <= 4000000)
   #define XFCN 5
#elif(CRYSTAL_FREQUENCY <= 10000000)
   #define XFCN 6
#elif(CRYSTAL_FREQUENCY <= 24500000)
   #define XFCN 7
#else
   #error "Crystal Frequency must be less than 24.5MHz"
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
   PCA0MD &= ~0x40;                    // WDTE = 0 (disable watchdog timer)
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

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
   OSCXCN = (0x60 | XFCN);             // Start external oscillator with
                                       // the appropriate XFCN setting
                                       // based on crystal frequency
   REF0CN = 0x02;                      // Turn on bias generator
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
   XBR1    = 0x40;                     // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
