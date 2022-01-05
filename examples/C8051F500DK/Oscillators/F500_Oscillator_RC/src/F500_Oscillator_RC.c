//-----------------------------------------------------------------------------
// F500_Oscillator_RC.c
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
// 1) Connect a 33pF capacitor (C6) between P0.3 (XTAL2) and GND, and a
//    330kOhm resistor (R3) between P0.3 (XTAL2) and VDD. And
//    specify the desired frequency by changing the constant <RC_FREQUENCY>
// 2) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 3) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 4) Verify that the J22 jumper is not populated.
// 5) Compile and download code to C8051F500-TB
//    with Simplicity IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 7) Measure the frequency output on P0.0.
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
// on P0.0 of the target board was 82.7 kHz.
//
//
// Target:         C8051F50x/1x (Side A of a C8051F500-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 18 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.1 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 03 MAR 2008 (GP)
//    -Initial Revision


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = LEGACY_PAGE;              // Set SFR Page for PCA0MD


   PORT_Init ();                       // Initialize Port I/O
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

   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // VDD Monitor reset

   SFRPAGE = CONFIG_PAGE;

   OSCXCN = (OSCXCN_XOSCMD__RC | XFCN);// Start external oscillator with
                                       // the appropriate XFCN setting
                                       // based on RC frequency

   for (i=0; i < 256; i++);            // Wait for RC osc. to start

   CLKSEL = CLKSEL_CLKSL__EXTOSC;      // Select external oscillator as system
                                       // clock source

   OSCICN = OSCICN_IOSCEN__DISABLED;   // Disable the internal oscillator.

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
// P0.0   digital    push-pull     /SYSCLK
// P0.3   analog     don't care     XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE; 

   // Oscillator Pins
   P0MDIN &= ~0x08;                    // P0.3 is analog
   P0SKIP |= 0x08;                     // P0.3 skipped in the Crossbar

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   XBR1 = XBR1_SYSCKE__ENABLED;        // Route /SYSCLK to P0.0
   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------