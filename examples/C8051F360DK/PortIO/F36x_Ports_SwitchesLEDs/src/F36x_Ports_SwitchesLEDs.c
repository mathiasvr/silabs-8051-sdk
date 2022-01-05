//-----------------------------------------------------------------------------
// F36x_Ports_SwitchesLEDs.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Configure port pins as different types of inputs and outputs
//
// This program demonstrates how to configure the port pins as digital inputs
// and outputs. The C8051F360 target board has two push-button switches
// connected to port pins and and two LEDs. The program constantly checks the
// status of the switches and if they are pushed, it turns on the
// respective LED.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.0 - SW3.0
//    J12: P3.0 - SW3.1
//    J12: P3.2 - P3.2_LED
//    J12: P3.3 - P3.3_LED
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) Press the switches (P3.0 and P3.1) and see that the LEDs turn on and off.
//
//
// Target:         C8051F36x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 29 JUL 2014
//
// Release 1.0
//    -Initial Revision (GP)
//    -29 AUG 2007


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK    24500000             // System clock in Hz

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SBIT(SW1,  SFR_P3, 0);                 // Push-button switch on board
SI_SBIT(SW2,  SFR_P3, 1);                 // Push-button switch on board
SI_SBIT(LED1, SFR_P3, 2);                 // Green LED
SI_SBIT(LED2, SFR_P3, 3);                 // Green LED

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
   PCA0MD  &= ~PCA0MD_WDTE__ENABLED;   // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// main () Routine
//-----------------------------------------------------------------------------
void main (void)
{

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
   }
}

//-----------------------------------------------------------------------------
// PORT_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// To configure a pin as a digital input, the pin is configured as digital
// and open-drain and the port latch should be set to a '1'. The weak-pullups
// are used to pull the pins high. Pressing the switch pulls the pins low.
//
// To configure a pin as a digital output, the pin is configured as digital
// and push-pull.
//
// An output pin can also be configured to be an open-drain output if system
// requires it. For example, if the pin is an output on a multi-device bus,
// it will probably be configured as an open-drain output instead of a
// push-pull output. For the purposes of this example, the pin is configured
// as push-pull output because the pin in only connected to an LED.
//
// P3.0 - digital       open-drain      SW1 (Switch 1)
// P3.1 - digital       open-drain      SW2 (Switch 2)
// P3.2 - digital       push-pull       LED1
// P3.3 - digital       push-pull       LED2
//
//-----------------------------------------------------------------------------
void PORT_Init ()
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P3MDOUT |= 0x0C;                    // Configure P3.2,P3.3 to push-pull

   XBR1 |= XBR1_XBARE__ENABLED;        // Enable crossbar

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1; // Initialize internal oscillator to
                                       // highest frequency

   RSTSRC  = 0x04;                     // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------