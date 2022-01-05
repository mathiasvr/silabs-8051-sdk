//-----------------------------------------------------------------------------
// F96x_Ports_SwitchLED.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates how to configure port pins as digital inputs
// and outputs.  The C8051F96x target board has two push-button switches
// and two LEDs connected to port pins.  The program constantly checks the
// status of the switches and if one or both are closed, it turns off
// one or both LEDs, respectively.
//
//
// How To Test:
//
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Ensure that jumpers are placed on the following:
//       J16 (LCD only):
//             P0.3/LED4/SW4
// 3) Connect the USB Debug Adapter to J13.
// 4) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 5) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 6) Connect a 9V power adapter to P1 of the development board.
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) Press the switches SW1 or SW2, LED3 and LED4 should light up, 
//    respectively.
//
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Added support for the EMIF board.
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
// Define Hardware
//-----------------------------------------------------------------------------
//#define UDP_F960_MCU_MUX_LCD
#define UDP_F960_MCU_EMIF
//-----------------------------------------------------------------------------
// Hardware Dependent definitions
//-----------------------------------------------------------------------------
#ifdef UDP_F960_MCU_MUX_LCD
SI_SBIT(SW1, SFR_P0, 0);
SI_SBIT(SW2, SFR_P0, 1);
SI_SBIT(LED3, SFR_P0, 2);
SI_SBIT(LED4, SFR_P0, 3);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(SW1, SFR_P3, 0);
SI_SBIT(SW2, SFR_P3, 1);
SI_SBIT(LED3, SFR_P3, 2);
SI_SBIT(LED4, SFR_P3, 3);
#endif
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             20000000    // Clock speed in Hz

#define LED_ON  0
#define LED_OFF 1

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

   while (1)
   {


      // Set the state of the LED3
      if (SW1 == 0)                    // If switch pressed
      {
         LED3 = LED_ON;                // Turn on LED3
      }
      else
      {
         LED3 = LED_OFF;               // Else, turn it off
      }

      // Set the state of the LED4
      if (SW2 == 0)                    // If switch pressed
      {
         LED4 = LED_ON;                // Turn on LED4
      }
      else
      {
         LED4 = LED_OFF;               // Else, turn it off
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
   SFRPAGE = LEGACY_PAGE;
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
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   SFRPAGE = LEGACY_PAGE;

   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
