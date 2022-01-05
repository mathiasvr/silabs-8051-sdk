//-----------------------------------------------------------------------------
// F35x_Oscillator_RC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the oscillator for use with an
// external RC Oscillator.
//
//
// How To Test:
// ------------
// 1) Connect a 33pF capacitor (C14) between P0.3 (XTAL2) and GND, and a
//    330kOhm resistor (R7) between P0.3 (XTAL2) and Vcc (3.3V). And
//    specify the desired frequency by changing the constant <RC_FREQUENCY>
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
//       Note: Add 2pF stray capacitance to C.
//
// For a resistor value of 330k, and a capacitor value of 33pF, the
// expected RC_FREQUENCY is 112.9 kHz. The actual measured frequency
// on P0.0 of the target board was 92 kHz.
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
// External Oscillator RC Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the target frequency
//
#define RC_FREQUENCY   112948          //  Target RC Frequency in Hz

// XFCN Setting Macro
#if  (RC_FREQUENCY <= 25000)
   #define XFCN 0
#elif(RC_FREQUENCY <= 50000)
   #define XFCN 1
#elif(RC_FREQUENCY <= 100000)
   #define XFCN 2
#elif(RC_FREQUENCY <= 200000)
   #define XFCN 3
#elif(RC_FREQUENCY <= 400000)
   #define XFCN 4
#elif(RC_FREQUENCY <= 800000)
   #define XFCN 5
#elif(RC_FREQUENCY <= 1600000)
   #define XFCN 6
#elif(RC_FREQUENCY <= 3200000)
   #define XFCN 7
#else
   #error "RC Frequency must be less than or equal to 3.2 MHz"
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
// in RC mode.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   int16_t i;                              // Delay counter

   // Use RC ocillator mode and set XFCN based on RC frequency
   OSCXCN = (OSCXCN_XOSCMD__RC | XFCN);

   for (i=0; i < 300; i++);            // Wait for RC osc. to start

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