//-----------------------------------------------------------------------------
// F38x_Oscillator_Capacitor.c
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
// 1) Ensure shorting blocks are place on the following:
//    - J19: Power - P1_PWR or SER_PWR
//    - J7: +3VD - VDD
//    - J11: P0.7 - XTAL2
// 2) Connect the USB Debug Adapter to J9. If P1_PWR power is selected on J19,
//    connect the 9V power adapter to P1.
// 3) Ensure that a capacitor on XTAL2 (C6) is installed on a C8051F380
//    target board.
// 4) Specify the target frequency in the constant <C_FREQUENCY>.
// 5) Compile and download code to a 'F380 device on a C8051F380-TB development
//    board selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
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
//          23 kHz   |    1
//          64 kHz   |    2
//         180 kHz   |    3
//         518 kHz   |    4
//         1.5 MHz   |    5
//         5.3 MHz   |    6
//        12.5 MHz   |    7
//
//
//
// Target:         C8051F38x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 09 JAN 2014
//
// Release 1.0
//    -Initial Revision (FB)
//    -15 AUG 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F380_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator C Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the target frequency
//
#define C_FREQUENCY   180000           // Target C Frequency in Hz

// XFCN Setting Macro
#define XFCN             3             // XFCN/KF setting.

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
// P0.7   analog     don't care     XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{

   // Oscillator Pins
   P0MDIN &= ~0x80;                    // P0.7 is analog
   P0SKIP |= 0x80;                     // P0.7 skipped in the Crossbar

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = 0x08;                     // Route /SYSCLK to first available pin
   XBR1    = 0x40;                     // Enable Crossbar and weak pull-ups

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------