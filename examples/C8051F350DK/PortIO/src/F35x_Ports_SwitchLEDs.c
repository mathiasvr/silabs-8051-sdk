//-----------------------------------------------------------------------------
// F35x_Ports_SwitchLEDs.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// Configure port pins as different types of inputs and outputs
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs. The C8051F350 target board has one push-button switch
// connected to a port pin and two LEDs. The program constantly checks the
// status of the switch and if it is pushed, it turns on the P0.6 LED and
// turns off the P0.7 LED. If the switch is not pushed, the P0.7 LED in on
// and the P0.6 LED is off.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J3 : P0.6 - P0.6_LED
//    - J3 : P0.7 - P0.7_LED
//    - J3 : P1.0 - P1.0_SW
//    - J8 : Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 3) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 5) Press the switche S2. The green LED D2 turns on and the D1 turns off.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 24 JUN 2014
//
// Release 1.0
//    -Initial Revision (GP)
//    -15 NOV 2005


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED1, SFR_P0, 6);                // LED='1' means ON
SI_SBIT(LED2, SFR_P0, 7);                // LED='1' means ON
SI_SBIT(SW1,  SFR_P1, 0);                // Push-button switch on board

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   // Disable Watchdog timer

   PORT_Init();                        // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   while (1)
   {
      if (SW1 == 0)                    // If switch depressed
      {
         LED1 = 1;                     // Turn on LED1
         LED2 = 0;                     // Turn off LED2
      }
      else
      {
         LED1 = 0;                     // Turn off LED1
         LED2 = 1;                     // Turn on LED2
      }
   }
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
// This function initializes the system clock to use the internal oscillator
// at its maximum frequency.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1; // Configure internal oscillator for
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
// P0.6   digital   push-pull     LED1
// P0.7   digital   push-pull     LED2
// P1.0   digital   open-drain    Switch 1
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDIN |= 0xC0;                     // P0.6 and P0.7 are digital
   P1MDIN |= 0x01;                     // P1.0 is digital

   P0MDOUT = 0xC0;                     // P0.6 and P0.7 are push-pull
   P1MDOUT = 0x00;                     // P1.0 is open-drain

   P1     |= 0x01;                     // Set P1.0 latch to '1'

   // Enable crossbar and weak pull-ups
   XBR1    = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------