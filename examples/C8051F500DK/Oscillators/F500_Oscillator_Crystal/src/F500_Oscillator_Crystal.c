//-----------------------------------------------------------------------------
// F500_Oscillator_Crystal.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the oscillator for use with an
// external crystal.
//
//
// How To Test:
// ------------
// 1) Populate Y1 with a crystal (32.768 kHz) and update <CRYSTAL_FREQUENCY>
//    with the frequency in Hz.
// 2) Populate R2 (10 MOhm) and C6/C7 according to the crystal datasheet
//    (22 pF), remove R7 resistors and do not install J9 and J10.
// 3) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 4) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 5) Verify that the J22 jumper is not populated.
// 6) Compile and download code to C8051F500-TB
//    with Simplicity IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 8) Measure the frequency output on P0.0.
//
//
// Target:         C8051F50x/1x (Side A of a C8051F500-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.2 / 18 AUG 2014 (JL)
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
// External Oscillator Crystal Frequency Selection
//-----------------------------------------------------------------------------
//
// Initialize the following constant to the crystal frequency
//
#define CRYSTAL_FREQUENCY    32768     //  Crystal Frequency in Hz

// XFCN Setting Macro
#if  (CRYSTAL_FREQUENCY <= 32000)
   #define XFCN 0
#elif(CRYSTAL_FREQUENCY <= 84000)
   #define XFCN 1
#elif(CRYSTAL_FREQUENCY <= 225000)
   #define XFCN 2
#elif(CRYSTAL_FREQUENCY <= 590000)
   #define XFCN 3
#elif(CRYSTAL_FREQUENCY <= 1500000)
   #define XFCN 4
#elif(CRYSTAL_FREQUENCY <= 4000000)
   #define XFCN 5
#elif(CRYSTAL_FREQUENCY <= 10000000)
   #define XFCN 6
#elif(CRYSTAL_FREQUENCY <= 30000000)
   #define XFCN 7
#else
   #error "Crystal Frequency must be no more than 30 MHz"
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

   // Start external oscillator with the appropriate XFCN setting based on
   // crystal frequency
   OSCXCN = (OSCXCN_XOSCMD__CRYSTAL | XFCN);

   for (i = 0; i < 256; i++);          // Wait for crystal osc. to start

   // Wait for crystal osc. to settle
   while (!(OSCXCN & OSCXCN_XTLVLD__BMASK));

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

   XBR1 = XBR1_SYSCKE__ENABLED;        // Route /SYSCLK to P0.0
   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------