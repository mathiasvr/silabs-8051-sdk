//-----------------------------------------------------------------------------
// F540_Oscillator_Crystal.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
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
// 1) Ensure that a quartz crystal (Y1), 10Mohm resistor (R2), and 33pF loading 
//    capacitors (C7 and C8) are installed on the C8051F540-TB.
// 2) Specify the crystal frequency in the constant <CRYSTAL_FREQUENCY>.
// 3) Remove shorting block on J13
// 4) Download code to an 'F540 target board.
// 5) Measure the frequency output on P0.0.
//
//
// Target:         C8051F540 (Side A of a C8051F540-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
// Release 1.2 / 14 MAY 2015 (SY)
//    -Update SFR/bit name for Simplicity Studio
//
// Release 1.1 / 10 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 06 NOV 2008 (GP)
//    -Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F540_Defs.h>

//-----------------------------------------------------------------------------
// External Oscillator Crystal Frequency Selection
//-----------------------------------------------------------------------------
//
// Initialize the following constant to the crystal frequency
//
#define CRYSTAL_FREQUENCY    22118400  //  Crystal Frequency in Hz

// XFCN Setting Macro
#if  (CRYSTAL_FREQUENCY <= 32000)
   #define XFCN 0
#elif(CRYSTAL_FREQUENCY <= 84000)
   #define XFCN 1
#elif(CRYSTAL_FREQUENCY <= 225400)
   #define XFCN 2
#elif(CRYSTAL_FREQUENCY <= 590000)
   #define XFCN 3
#elif(CRYSTAL_FREQUENCY <= 1540000)
   #define XFCN 4
#elif(CRYSTAL_FREQUENCY <= 4000000)
   #define XFCN 5
#elif(CRYSTAL_FREQUENCY <= 10000000)
   #define XFCN 6
#elif(CRYSTAL_FREQUENCY <= 30000000)
   #define XFCN 7
#else
   #error "Crystal Frequency must be less than 30MHz"
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
   PCA0MD &= ~0x40;                    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = LEGACY_PAGE;              // Set SFR Page for PCA0MD


   PORT_Init();                        // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   while (1);                          // loop forever
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
   uint16_t i;                              // Delay counter

   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // VDD Monitor reset

   SFRPAGE = CONFIG_PAGE;

   OSCXCN = (0x60 | XFCN);             // Start external oscillator with
                                       // the appropriate XFCN setting
                                       // based on crystal frequency

   for (i = 0; i < 256; i++);          // Wait for crystal osc. to start

   while (!(OSCXCN & 0x80));           // Wait for crystal osc. to settle

   CLKSEL = 0x01;                      // Select external oscillator as system
                                       // clock source

   OSCICN = 0x00;                      // Disable the internal oscillator.

   SFRPAGE = SFRPAGE_save;
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
// P0.0   digital    push-pull      /SYSCLK
// P0.2   analog     don't care     XTAL1
// P0.3   analog     don't care     XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE; 
 
   // Oscillator Pins
   P0MDIN &= ~0x0C;                    // P0.2, P0.3 are analog
   P0SKIP |= 0x0C;                     // P0.2, P0.3 skipped in the Crossbar

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR1    = 0x02;                     // Route /SYSCLK to crossbar
   XBR2    = 0x40;                     // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
