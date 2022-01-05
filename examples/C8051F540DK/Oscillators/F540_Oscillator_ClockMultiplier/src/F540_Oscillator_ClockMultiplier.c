//-----------------------------------------------------------------------------
// F540_Oscillator_ClockMultiplier.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the clock multiplier to multiply
// the internal system clock from 24 MHz to 48 MHz.  The system clock is output
// to P0.0.
//
//
// How To Test:
//
// 1) Remove the shorting block on J13 on the C8051F540-TB
// 2) Download and run the code
// 3) Measure the frequency output on P0.0.
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
// External Oscillator RC Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the target frequency
//
#define INTOSC     24000000            // Maximum internal oscillator frequency
#define SYSCLK     48000000            // Multiplied Frequency

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

   while (1) {};                       // Loop forever
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
// This function initializes the system clock to use the clock multiplier
// output as the system clock
//
// If the system clock speed is greater than 25 MHz, the Flash timing should
// be adjusted using FLSCL SFR.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint16_t i;                              // Delay counter

   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0xC7;                      // Set internal osc. divider to 1

   CLKMUL = 0x00;                      // Reset the Clock Multiplier
                                       // Set input to Internal Oscillator

   CLKMUL |= 0x80;                     // Enable the Clock Multiplier

   for (i = 0; i < 120; i++);          // Need to delay (5 us) 120 clock cycles

   CLKMUL |= 0xC0;                     // Initialize the Clock Multiplier

   while (!(CLKMUL & 0x20));

   FLSCL = 0x10;                       // Set new Flash read timing for higher
                                       // clock speed

   CLKSEL = 0x02;                      // Switch system clock to multiplier

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
// P0.0   digital    push-pull     /SYSCLK
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   XBR1    = 0x02;                     // Route /SYSCLK to P0.0
   XBR2    = 0x40;                     // Enable Crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------