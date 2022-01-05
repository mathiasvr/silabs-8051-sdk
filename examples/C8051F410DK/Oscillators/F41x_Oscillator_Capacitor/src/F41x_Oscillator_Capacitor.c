//-----------------------------------------------------------------------------
// F41x_Oscillator_Capacitor.c
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
// 1) Ensure shorting blocks are place on the following:
//    J9:   VUNREG (9V adapter) or 5VEC3 (Debugger) 
//    J10:  +3VD/+3VD
//    J12:  +3VD/VREGIN
//          VREGIN_EN
// 2) Ensure that a capacitor on XTAL2 (C43) is installed on the C8051F410
//    target board.
// 3) Modify the constant <XFCN> to match the desired frequency. If using a
//    33pF capacitor and 3.3V, see the chart below.
// 4) Connect the USB Debug Adapter to J4. If VUNREG is selected on J9,
//    connect the 9V power adapter to P1.
// 5) Compile and download code to the C8051F410-TB development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in the
//    quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Measure the frequency output on P0.0.
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
//          varies   |    0
//          16 kHz   |    1
//          47 kHz   |    2
//         126 kHz   |    3
//         397 kHz   |    4
//         1.1 MHz   |    5
//         4.4 MHz   |    6
//        10.7 MHz   |    7
//
//
// Target:         C8051F41x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 specific code with compiler agnostic code from 
//      compiler_defs.h.
//    - 21 JAN 2014
//
// Release 1.1 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0
//    -Initial Revision (FB)
//    -15 AUG 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F410_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator C Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constants:
//
#define C_FREQUENCY    47000           // Target C Frequency in Hz

// XFCN Setting Macro
#define XFCN             2             // XFCN/KF setting.

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
// in C mode.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   int16_t i;                              // Delay counter

   OSCXCN = (0x50 | XFCN);             // Start external oscillator with
                                       // the appropriate XFCN setting
                                       // in C Mode

   for (i=0; i < 256; i++);            // Wait for C osc. to start

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
// P1.1   analog     don't care     XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
    // Oscillator Pins
   P1MDIN &= ~0x02;                    // P0.1 is analog
   P1SKIP |= 0x02;                     // P0.1 skipped in the Crossbar

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = 0x08;                     // Route /SYSCLK to first available pin
   XBR1    = 0x40;                     // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------