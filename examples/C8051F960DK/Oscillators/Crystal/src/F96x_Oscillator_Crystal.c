//-----------------------------------------------------------------------------
// F96x_Oscillator_Crystal.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the oscillator for use with an
// external quartz crystal.
//
//
// How To Test (UDP F960 MCU EMIF board only!):
//
// 1) Ensure that a quartz crystal oscillator circuit is connected to 
//    XTAL1 (P0.2) and XTAL2 (P0.3).
// 2) Specify the crystal frequency in the constant <CRYSTAL_FREQUENCY>.
// 3) Connect the USB Debug Adapter to J13.
// 4) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 5) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 6) Connect a 9V power adapter to P1 of the development board.
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU EMIF board
//    by selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) Measure the frequency output on P0.0.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 20 JAN 2014
//
// Release 1.0
//    - Initial Revision (FB)
//    - 19 MAY 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F960_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator Crystal Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the crystal frequency
//
#define CRYSTAL_FREQUENCY    22100000     //  Crystal Frequency in Hz

// XFCN Setting Macro
#if  (CRYSTAL_FREQUENCY <= 20000)
   #define XFCN 0
#elif(CRYSTAL_FREQUENCY <= 58000)
   #define XFCN 1
#elif(CRYSTAL_FREQUENCY <= 155000)
   #define XFCN 2
#elif(CRYSTAL_FREQUENCY <= 415000)
   #define XFCN 3
#elif(CRYSTAL_FREQUENCY <= 1100000)
   #define XFCN 4
#elif(CRYSTAL_FREQUENCY <= 3100000)
   #define XFCN 5
#elif(CRYSTAL_FREQUENCY <= 8200000)
   #define XFCN 6
#elif(CRYSTAL_FREQUENCY <= 40000000)
   #define XFCN 7
#else
   #error "Crystal Frequency must be less than 40MHz"
#endif

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

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

   while (1);                          // Infinite Loop

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

   SFRPAGE = LEGACY_PAGE;
   OSCXCN = (0x60 | XFCN);             // start external oscillator with
                                       // the appropriate XFCN setting
                                       // based on crystal frequency

   while (!(OSCXCN & 0x80));           // Wait for crystal osc. to settle

   RSTSRC = 0x06;                      // Enable missing clock detector and
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
// P0.2   analog     open-drain/High-Z      XTAL1
// P0.3   analog     open-drain/High-Z      XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   SFRPAGE = LEGACY_PAGE;
   // Oscillator Pins
   P0MDIN  &= ~0x0C;                   // P0.2, P0.3 are analog
   P0SKIP  |=  0x0C;                   // P0.2, P0.3 skipped in the Crossbar
   P0MDOUT &= ~0x0C;                   // P0.2, P0.3 output mode is open-drain
   P0      |=  0x0C;                   // P0.2, P0.3 output drivers are OFF

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = 0x08;                     // Route /SYSCLK to first available pin
   XBR2    = 0x40;                     // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
