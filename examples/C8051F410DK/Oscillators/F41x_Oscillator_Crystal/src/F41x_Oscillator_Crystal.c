//-----------------------------------------------------------------------------
// F41x_Oscillator_Crystal.c
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
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    J9:   VUNREG (9V adapter) or 5VEC3 (Debugger) 
//    J10:  +3VD/+3VD
//    J12:  +3VD/VREGIN
//          VREGIN_EN
//    J17:  5VEC3/VIO or +3VD/VIO
//          VIO_EN
// 2) Ensure that a quartz crystal (Y2), 10Meg resistor (R4), and loading
//    capacitors (C44, C43) are installed.
// 3) Specify the crystal frequency in the constant <CRYSTAL_FREQUENCY>.
// 4) Connect the USB Debug Adapter to J4. If VUNREG is selected on J9,
//    connect the 9V power adapter to P1.
// 5) Compile and download code to the C8051F410-TB development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in the
//    quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) Measure the frequency output on P0.0.
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
//    -17 JUL 2006

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F410_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator Crystal Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the crystal frequency
//
#define CRYSTAL_FREQUENCY    32768     //  Crystal Frequency in Hz

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
#elif(CRYSTAL_FREQUENCY <= 25000000)
   #define XFCN 7
#else
   #error "Crystal Frequency must be less than 25MHz"
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

   while (1) {};                       // loop forever

}                                      // end of main()

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

   int16_t i;                              // delay counter

   OSCXCN = (0x60 | XFCN);             // start external oscillator with
                                       // the appropriate XFCN setting
                                       // based on crystal frequency

   for (i=0; i < 256; i++);            // Wait for crystal osc. to start

   while (!(OSCXCN & 0x80));           // Wait for crystal osc. to settle

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // VDD Monitor reset

   CLKSEL = 0x01;                      // Select external oscillator as system
                                       // clock source. /SYSCLK on the port
                                       // pin is equal to the system clock.

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
// P0.0   digital    push-pull      /SYSCLK
// P1.0   analog     don't care     XTAL1
// P1.1   analog     don't care     XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
    // Oscillator Pins
   P1MDIN &= ~0x03;                    // P1.0, P1.1 are analog
   P1SKIP |= 0x03;                     // P1.0, P1.1 skipped in the Crossbar
   P1     |= 0x03;                     // P1.0, P1.1 port latches set to '1'

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = 0x08;                     // Route /SYSCLK to first available pin
   XBR1    = 0x40;                     // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------