//-----------------------------------------------------------------------------
// F39x_Oscillator_Crystal.c
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
// 1) Ensure that a quartz crystal, 10Meg resistor, and loading capacitors
//    are installed.
// 2) Ensure shorting blocks are not placed on J6 and J7.
// 3) Specify the crystal frequency in the constant <CRYSTAL_FREQUENCY>.
// 4) Download code to an 'F39x UDP MCU board.
// 5) Measure the frequency output on P0.0.
//
//
// Target:         C8051F39x/37x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (ST)
//    - 30 JUL 2012
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F390_Register_Enums.h>

//-----------------------------------------------------------------------------
// External Oscillator Crystal Frequency Selection
//-----------------------------------------------------------------------------
//
// Please initialize the following constant to the crystal frequency
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
   #error "Crystal Frequency must be less than 30MHz"
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
   int i;                              // delay counter

   OSCXCN = (0x60 | XFCN);             // start external oscillator with
                                       // the appropriate XFCN setting
                                       // based on crystal frequency

   for (i=0; i < 256; i++);            // Wait for crystal osc. to start

   while (!(OSCXCN & 0x80));           // Wait for crystal osc. to settle

   RSTSRC = 0x02;                      // Enable missing clock detector and
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
// P0.0   digital    push-pull      /SYSCLK
// P0.2   analog     don't care     XTAL1
// P0.3   analog     don't care     XTAL2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Oscillator Pins
   P0MDIN &= ~0x0C;                    // P0.2, P0.3 are analog
   P0SKIP |= 0x0C;                     // P0.2, P0.3 skipped in the Crossbar

   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull

   // Crossbar Initialization
   XBR0    = 0x08;                     // Route /SYSCLK to first available pin
   XBR1    = 0x40;                     // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
