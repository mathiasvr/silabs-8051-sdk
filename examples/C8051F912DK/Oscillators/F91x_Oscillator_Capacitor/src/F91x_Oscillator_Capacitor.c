//-----------------------------------------------------------------------------
// F91x_Oscillator_Capacitor.c
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
// 1) Ensure that jumpers are placed on the following:
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Ensure that a capacitor (C21) is installed.
// 3) Ensure that J8[P0.3H-SW0.3] does not contain a shorting block.
// 4) Connect the USB Debug Adapter to J4.
// 5) Turn the POWER switch (SW5) to the "ON" position.
// 6) Specify the target frequency in the constant <C_FREQUENCY>. 
// 7) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) Measure the frequency output on P0.0.
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
//           4 kHz   |    0  // Note: Disable missing clock detector for
//           8 kHz   |    1  // very low frequencies caused by XFCN values
//          22 kHz   |    2  // less than 3.
//          64 kHz   |    3
//         187 kHz   |    4
//         553 kHz   |    5
//         2.3 MHz   |    6
//         7.7 MHz   |    7
//
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
//    - 06 JULY 2009
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
// External Oscillator RC Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the target frequency
//
#define C_FREQUENCY   21000          // Target C Frequency in Hz

// XFCN Setting Macro if using a 33pF capacitor.
#if  (C_FREQUENCY <= 4000)
   #define XFCN 0
#elif(C_FREQUENCY <= 8000)
   #define XFCN 1
#elif(C_FREQUENCY <= 22000)
   #define XFCN 2
#elif(C_FREQUENCY <= 64000)
   #define XFCN 3
#elif(C_FREQUENCY <= 187000)
   #define XFCN 4
#elif(C_FREQUENCY <= 553000)
   #define XFCN 5
#elif(C_FREQUENCY <= 2300000)
   #define XFCN 6
#elif(C_FREQUENCY <= 7700000)
   #define XFCN 7
#else
   #error "C Frequency must be less than or equal to 7.7 MHz for a C value of 33pF"
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

   while (1);;                         // Infinite Loop

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
// in C mode.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{

   OSCXCN = (0x50 | XFCN);             // start external oscillator with
                                       // the appropriate XFCN setting
                                       // in C Mode

   while (!(OSCXCN & 0x80));           // Wait until clock is detected 

   //RSTSRC = 0x06;                      // Enable missing clock detector and
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
// P0.3   digital    open-drain/High-Z      XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Oscillator Pins
   P0MDIN  |=  0x08;                   // P0.3 is digital
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
