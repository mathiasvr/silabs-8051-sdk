//-----------------------------------------------------------------------------
// F53xA_Oscillator_RC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the oscillator for using with an
// external RC Oscillator.
//
//
// How To Test:
// ------------
// 1) Connect a 33pF capacitor (C29 or C34) between XTAL2 and GND, and a
//    330kOhm resistor (R16 or R21) between XTAL2 and Vcc (3.3V). And
//    specify the desired frequency by changing the constant <RC_FREQUENCY>
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
//
// The capacitor should be no greater than 100 pF.
// The oscillation frequency can be determined by the following equation:
//
//       Clock frequency (MHz) = 1.23 x 10^3 / ( R * C )
//
//       where
//       R = pull-up resistor value in kOhm
//       C = capacitor value on the XTAL2 pin in pF
//
// Note: Add 2pF stray capacitance to C.
//
// For a resistor value of 330K, and a capacitor value of 33pF, the
// expected RC_FREQUENCY is 112.9 kHz. The actual measured SYSCLK frequency
// on P0.0 was 113.9 kHz.
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
//    -Updated for 'F530A TB (TP)
//    -30 JAN 2008
//
// Release 1.0
//    -Initial Revision (DS)
//    -18 June 2007


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F530_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator RC Frequency Selection
//-----------------------------------------------------------------------------
// Note: The Cap_Value and Resistor_Value numbers are formatted specifically
// to keep all numbers whole (not decimals) and within a representable range
// (32 bits).
//

#define Cap_Value 33                   // Value of the external capacitor, in
                                       // pF
                                       // The capacitor should be no greater
                                       // than 100 pF.

#define Resistor_Value 330000L         // Value of the external pull-up
                                       // resistor, in ohms

                                       // Note: Debugging cannot occur when the
                                       // SYSCLK is less than 32 kHz.
// Error checks on the input values
// The capacitor should be no greater than 100 pF.
#if (Cap_Value > 100)
   #error "Defined Capacitor Value outside allowable range!"
   #define XFCN 0
#endif

// XFCN bits setting based on the oscillator frequency
#define RC_FREQUENCY 1230000000L/(Resistor_Value * Cap_Value)

// XFCN Setting Macro
#if  (RC_FREQUENCY <= 20)
   #define XFCN 0
#elif(RC_FREQUENCY <= 58)
   #define XFCN 1
#elif(RC_FREQUENCY <= 155)
   #define XFCN 2
#elif(RC_FREQUENCY <= 415)
   #define XFCN 3
#elif(RC_FREQUENCY <= 1100)
   #define XFCN 4
#elif(RC_FREQUENCY <= 3100)
   #define XFCN 5
#elif(RC_FREQUENCY <= 8200)
   #define XFCN 6
#elif(RC_FREQUENCY <= 25000)
   #define XFCN 7
#else
   #error "RC Frequency must be less than or equal to 25 MHz"
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
// in RC mode.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint16_t i;                              // Delay counter

   OSCXCN = (OSCXCN_XOSCMD__RC | XFCN);// Start external oscillator with
                                       // the appropriate XFCN setting
                                       // based on RC frequency

   for (i = 0; i < 256; i++);          // Wait for RC osc. to start

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
// The oscillator pin should be configured as an analog input when using the
// external oscillator in RC mode.
//
// P0.0   digital    push-pull     /SYSCLK
//
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