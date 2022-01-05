//-----------------------------------------------------------------------------
// F70x_Ports_SwitchLED.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs.  The C8051F700 target board has a push-button switch
// and an LED connected to port pins.  The program constantly checks the
// status of the switch and if it is closed, it turns off the LED.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//	  - J15: VDD - power source
//    - J8: P1.0 - DS3
//    - J8: P1.1 - SW1
// 2) Connect the USB Debug Adapter to P3.
// 3) Connect the USB cable to P4.
// 4) Compile and download code to the C8051F70x development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Press and hold switch SW1 to turn the LED on, release to turn
//    the LED off.
//
// Target:         C8051F700
// Tool chain:     Simplicity Studio / Keil C51 9.53
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
#include <SI_C8051F700_defs.h>         // SFR declarations

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(GREEN_LED, SFR_P1, 0);           // '0' means ON, '1' means OFF
SI_SBIT(SW,        SFR_P1, 1);           // SW == 0 means switch pressed


//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000    // Clock speed in Hz

#define LED_ON  0
#define LED_OFF 1

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
   WDTCN = 0xDE;                       // Disable watchdog timer
   WDTCN = 0xAD;
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

      // Set the state of the red LED
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
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x80;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

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
// P1.1   digital   open-drain    Switch
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   P1MDOUT = 0x01;

   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
