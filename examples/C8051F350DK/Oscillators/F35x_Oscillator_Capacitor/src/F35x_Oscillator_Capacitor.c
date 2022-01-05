//-----------------------------------------------------------------------------
// F35x_Oscillator_Capacitor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the oscillator for use with an
// external capacitor.
//
//
// How To Test:
// ------------
// 1) Connect a 33pF capacitor (C14) between P0.3 (XTAL2) and GND.
//    And specify the desired XFCN setting by modifying the constant <XFCN>.
// 2) Ensure shorting blocks are installed on the following:
//    - J8: Power selection, P1 9V or J4 Debugger
// 3) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 4) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 5) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 6) Measure the frequency output on P0.0.
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
//       XFCN | K Factor
//       ---------------
//         0  |    0.87
//         1  |    2.6
//         2  |    7.7
//         3  |   22
//         4  |   65
//         5  |  180
//         6  |  664
//         7  | 1590
//
// The actual frequency is best determined by measurement. In this example,
// a 33pF capacitor was used with VDD = 3.3V. The calculated frequency is
// 202 kHz. The actual measured SYSCLK frequency on P0.0 was 175 kHz for this
// example when XFCN is set to 3.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 03 JUL 2014
//
// Release 1.0
//    -Initial Revision (PD)
//    -14 AUG 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator C Frequency Selection
//-----------------------------------------------------------------------------

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   // Disable watchdog timer

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
// This function initializes the system clock to use the external oscillator
// in C mode.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   int16_t i;                              // delay counter

   // Use C mode and set XFCN
   OSCXCN = (OSCXCN_XOSCMD__C | XFCN);

   for (i=0; i < 300; i++);            // Wait for C osc. to start

   // Enable missing clock detector and VDD Monitor reset
   RSTSRC = 0x06;

   // Select external oscillator as system clock source
   CLKSEL = CLKSEL_CLKSL__EXTOSC;

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
   XBR0 = XBR0_SYSCKE__ENABLED;        // Route /SYSCLK to first available pin
   // Enable Crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------