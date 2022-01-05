//-----------------------------------------------------------------------------
// F30x_Ports_SwitchLED.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs.  The C8051F300 target board has one push-button switch
// connected to a port pin and one LED.  The program constantly checks the 
// status of the switch and if it is pushed, it turns on the LED.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J3: P0.2_LED
//          P0.3_SW
// 2) Connect the USB Debug Adapter to J4.
// 3) Connect the 9V power adapter to P1.
// 4) Compile and download code to a 'F300 device on a C8051F300-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Push the button (P0.3) and see that the LED turns on 
//
//
// Target:         C8051F300
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 09 JAN 2014
//
// Release 1.0
//    -Initial Revision (GP)
//    -15 NOV 2005
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F300_Register_Enums.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED1, SFR_P0, 2);                     // LED1 ='1' means ON
SI_SBIT(SW1, SFR_P0, 3);                      // SW1 ='0' means switch pressed

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
      if (SW1 == 0)                    // If switch depressed
      { 
         LED1 = 1;                     // Turn on LED
      }
	  else   
      {  
         LED1 = 0;                     // Else, turn it off
      }                   
   }                                   // End of while(1)
}                                      // End of main()

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
// This function initializes the system clock to use the internal oscillator
// at its maximum frequency.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency (24.5 Mhz)
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and port pins.
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
// push-pull output.  For the purposes of this example, the pin is configured
// as push-pull output because the pin in only connected to an LED.
//
// P0.2   digital   push-pull     LED1
// P0.3   digital   open-drain    Switch 1
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDIN |= 0x0C;                     // P0.2 and P03. are digital

   P0MDOUT = 0x04;                     // P0.3 is open-drain; P0.2 is push-pull

   P0     |= 0x08;                     // Set P0.3 latch to '1'

   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
