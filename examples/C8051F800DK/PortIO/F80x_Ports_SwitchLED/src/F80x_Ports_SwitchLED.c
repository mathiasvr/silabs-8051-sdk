//-----------------------------------------------------------------------------
// F80x_Ports_SwitchLED.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs.  The C8051F800 target board has a push-button switch
// and an LED connected to port pins.  The program constantly checks the
// status of the switch and if it is closed, it turns off the LED.
//
//
// How To Test:
//
// 1) Download code to a 'F800 target board
// 2) Ensure shorting block are installed on the first 8 pins of the
//    J8 header.
// 3) Push the button (P1.4) and see that the LED turns on.
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J3: P1.0_LED - P1.0
//    - J3: P1.4_SW - P1.4
//    - J2
// 2) Connect the USB Debug Adapter to J4.
// 3) Compile and download code to the C8051F800-TB by selecting
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//		Resume button in the quick menu, or pressing F8.
// 5) Push the button and see that the LED turns off.
//
//
// Target:         C8051F800
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (PD)
//    -08 JUL 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"
#include <SI_C8051F800_Defs.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(GREEN_LED, SFR_P1, 0);           // '0' means ON, '1' means OFF
SI_SBIT(SW,        SFR_P1, 4);           // SW == 0 means switch pressed


//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000    // Clock speed in Hz

#define LED_ON  1
#define LED_OFF 0

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
   PCA0MD &= ~0x40;                    // WDTE = 0 (disable watchdog timer)
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   PORT_Init();                        // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   while (1)
   {

      // Set the state of the green LED
      if (SW == 0)                    // If switch pressed
      {
         GREEN_LED = LED_OFF;         // Turn off LED
      }
      else
      {
         GREEN_LED = LED_ON;          // Else, turn it on
      }

   }                                  // end of while(1)
}                                     // end of main()

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
// This function initializes the system clock to use the internal
// oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   OSCICN |= 0x83;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
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
// To configure a pin as a digital input, the pin is configured as digital
// and open-drain and the port latch should be set to a '1'.  The weak-pullups
// are used to pull the pins high.  Pressing the switch pulls the pins low.
//
// To configure a pin as a digital output, the pin is configured as digital
// and push-pull.
//
// An output pin can also be configured to be an open-drain output if system
// requires it.  For example, if the pin is an output on a multi-device bus,
// it will probably be configured as an open-drain output instead of a
// push-pull output.  For the purposes of this example, the output pin is
// configured as push-pull output because the pin is only connected to an LED.
//
// P1.0   digital   push-pull     Green LED
// P1.4   digital   open-drain    Switch
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P1MDOUT = 0x01;

   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
