//-----------------------------------------------------------------------------
// F53xA_Oscillator_Capacitor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the oscillator for using with an
// external capacitor.
//
//
// How To Test:
// ------------
// 1) Connect a 33pF capacitor (C29 or C34) between XTAL2 and GND.
//    And specify the desired XFCN setting by modifying the constant <XFCN>.
// 2) Connect the USB Debug Adapter to HDR2 for "A" SIDE or HDR1 for "B" SIDE.
//    Connect the 9V power adapter to P5.
// 3) Compile and download code to C8051F530A target board
//    (either "A" SIDE or "B" SIDE) with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) Measure the frequency output on P0.0.
//
// The oscillation frequency and the required External Oscillator Frequency
// Control value (XFCN) can be determined by the following equation:
//
//       Clock frequency (MHz) = KF(XFCN) / (C * VDD)
//
//       where
//       KF(XFCN) is a factor based on XFCN.
//       C = capacitor value on the XTAL2 pin in pF
//       Vdd = power supply voltage in Volts
//
//        XFCN   |   K Factor
//       --------------------
//         0     |    0.87
//         1     |    2.6
//         2     |    7.7
//         3     |   22
//         4     |   65
//         5     |  180
//         6     |  664
//         7     | 1590
//
// The actual frequency is best determined by measurement. In this example,
// a 33pF capacitor was used with VDD = 2.6V. Component values are best
// chosen by trial and error. It is a good idea to include at least 1pF
// of stray capacitances in frequency calculations. The expected frequencies
// is 248.9kHz. The actual measured SYSCLK frequency on P0.0 was 220 kHz 
// when XFCN is set to 3.
//
//
// Target:         C8051F52xA/F53xA (C8051F530A TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 09 SEP 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1
//    -Updated for 'F53xA TB (TP)
//    -30 JAN 2008
//
// Release 1.0
//    -Initial Revision (DS)
//    -18 JUNE 2007


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F530_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator RC Frequency Selection
//-----------------------------------------------------------------------------
//
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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;   // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   PORT_Init();                        // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   while (1);                          // Loop forever

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
   uint16_t i;                              // Delay counter

   OSCXCN = (OSCXCN_XOSCMD__C | XFCN); // Start external oscillator with
                                       // the appropriate XFCN setting
                                       // in C Mode

   for (i = 0; i < 256; i++);          // Wait for C osc. to start

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // VDD Monitor reset

   CLKSEL = CLKSEL_CLKSL__EXTOSC;      // Select external oscillator as system
                                       // clock source

   OSCICN = OSCICN_IOSCEN__DISABLED;   // Disable the internal oscillator.
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
// P1.0   analog     don't care     XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Oscillator Pins
   P1MDIN &= ~0x01;                    // P1.0 is analog
   P1SKIP |= 0x01;                     // P1.0 skipped in the Crossbar

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0 = XBR0_SYSCKE__ENABLED;        // Route /SYSCLK to first available pin
   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------