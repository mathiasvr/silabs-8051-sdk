//-----------------------------------------------------------------------------
// F91x_Ports_SwitchLED.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs.  The C8051F912 target board has two push-button switches
// and two LEDs connected to port pins.  The program constantly checks the
// status of the switches and if one or both are closed, it turns off 
// one or both LEDs, respectively.
//
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.5/LED1.5
//             P1.6/LED1.6
//             P0.2/SW0.2
//             P0.3/SW0.3
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Push the button (P0.2 or P0.3) and see that the corresponding 
//    LED (Red or Yellow) turns off.
//
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 14 JAN 2014
//
// Release 1.2 
//    - Compiled and tested for Raisonance Toolchain (FB)
//    - 17 MAY 10
//
// Release 1.1
//    - Compile and test C8051F912_defs.h on 912 board (JH)
//    - Port to C8051F912-TB from C8051F930-TB (JH)
//    - 06 JULY 2009
//
// Release 1.0
//    -Initial Revision (FB)
//    -12 AUG 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,    SFR_P1, 5);          // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED, SFR_P1, 6);          // '0' means ON, '1' means OFF
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed
SI_SBIT(SW3,        SFR_P0, 3);          // SW3 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             20000000    // Clock speed in Hz

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

   while (1)
   {

      
      // Set the state of the red LED
      if (SW2 == 0)                    // If switch pressed
      {
         RED_LED = LED_OFF;            // Turn off Red LED
      }
      else
      { 
         RED_LED = LED_ON;            // Else, turn it on
      }
      
      // Set the state of the yellow LED
      if (SW3 == 0)                    // If switch pressed
      {
         YELLOW_LED = LED_OFF;         // Turn off Yellow LED
      }
      else
      {
         YELLOW_LED = LED_ON;          // Else, turn it on
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
// This function initializes the system clock to use the internal low power
// oscillator at its maximum frequency.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   CLKSEL = 0x04;                     // Select low power oscillator with a 
                                      // clock divider value of 1 (20MHz)
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
// push-pull output.  For the purposes of this example, the output pins are 
// configured as push-pull outputs because the pins are only connected to LEDs.
//
// P0.2   digital   open-drain    Switch 2
// P0.3   digital   open-drain    Switch 3
// P1.5   digital   push-pull     Red LED
// P1.6   digital   push-pull     Yellow LED
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDIN |= 0x0C;                     // P0.2, P0.3 are digital
   P1MDIN |= 0x60;                     // P1.5, P1.6 are digital

   P0MDOUT &= ~0x0C;                   // P0.2, P0.3 are open-drain
   P1MDOUT |= 0x60;                    // P1.5, P1.6 are push-pull

   P0     |= 0x0C;                     // Set P0.2, P0.3 latches to '1'

   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
