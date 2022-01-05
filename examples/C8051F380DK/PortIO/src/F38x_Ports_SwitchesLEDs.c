//-----------------------------------------------------------------------------
// F38x_Ports_SwitchesLEDs.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure the port pins as digital inputs
// and outputs.  The C8051F380 target board has two push-button switches
// connected to port pins and and two LEDs.  The program constantly checks the 
// status of the switches and if they are pushed, it turns on the 
// respective LED.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J19: Power - P1_PWR or SER_PWR
//    - J7: +3VD - VDD
//    - J12: P2.0 - SW2.0
//           P2.1 - SW2.1
//           P2.2 - P2.2_LED
//           P2.3 - P2.3_LED
// 2) Connect the USB Debug Adapter to J9. If P1_PWR power is selected on J19,
//    connect the 9V power adapter to P1.
// 3) Compile and download code to a 'F380 device on a C8051F380-TB development
//    board selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 3) Push the buttons (P2.0 and P2.1) and see that the LEDs turn on 
//
//
//
// Target:         C8051F38x
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

#include <SI_C8051F380_Register_Enums.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------
SI_SBIT(SW1, SFR_P2, 0);                  // SW1 ='0' means switch pressed
SI_SBIT(SW2, SFR_P2, 1);                  // SW2 ='0' means switch pressed
SI_SBIT(LED1, SFR_P2, 2);                      // LED='1' means ON
SI_SBIT(LED2, SFR_P2, 3);                      // LED='1' means ON

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
	  
      if (SW2 == 0)                    // If switch depressed
      { 
         LED2 = 1;                     // Turn on LED
      }
	  else   
      {  
         LED2 = 0;                     // Else, turn it off
      }                   
   }                                   // end of while(1)
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
// push-pull output.  For the purposes of this example, the pin is configured
// as push-pull output because the pin in only connected to an LED.
//
// P2.0   digital   open-drain    Switch 1
// P2.1   digital   open-drain    Switch 2
// P2.2   digital   push-pull     LED1
// P2.3   digital   push-pull     LED2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P2MDIN |= 0x0F;                     // Lower four pins on P2 are digital

   P2MDOUT = 0x0C;                     // enable LEDs as push-pull outputs
                                       // enable Switches as open-drain

   P2     |= 0x03;                     // Set port latches for P2.0 
                                       // and P2.1 to '1'

   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------