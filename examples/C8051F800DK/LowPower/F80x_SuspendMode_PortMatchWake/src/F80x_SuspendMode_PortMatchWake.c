//-----------------------------------------------------------------------------
// F80x_SuspendMode_PortMatchWake.c
//-----------------------------------------------------------------------------
// Copyright (C) 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program turns on or off the LED before going into suspend mode.  The
// device wakes from suspend when a port match event occurs on the switch.
//
//-----------------------------------------------------------------------------
// How To Test:
//-----------------------------------------------------------------------------
// 1) Ensure that jumpers are placed on the following:
//    - J3: P1.0_LED - P1.0
//    - J3: P1.4_SW - P1.4
//    - J2
// 2) Connect the USB Debug Adapter to J4.
// 3) Compile and download code to a 'F80x device by selecting Run -> Debug
//    from the menus, clicking the Debug button in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Press P1.4 to place the device in sleep mode.
//    Press P1.4 again to wake it back up. The LED will turn on when
//    the device is awake.
//
//
// Target:         C8051F80x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (PD)
//    -10 JUL 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"             // compiler declarations
#include <SI_C8051F800_Defs.h>         // SFR declarations


//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(GREEN_LED,    SFR_P1, 0);        // '0' means ON, '1' means OFF
SI_SBIT(SW,           SFR_P1, 4);        // SW == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK          24500000       // SYSCLK frequency in Hz


#define LED_ON           0
#define LED_OFF          1

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
volatile uint16_t delay;

//-----------------------------------------------------------------------------
// Function PROTOTYPES
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
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   PORT_Init ();                       // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   // Setup the Port Match Wake-up Source to wake up on the falling
   // edge of P1.1 (when the switch is pressed)
   P1MASK = 0x10;                      // Mask out all pins except for P1.1
   P1MAT = 0x10;                       // P1.1 should be HIGH while the device
                                       // is in the low power mode
   IE_EA = 1;                          // Enable global interrupts

   //----------------------------------
   // Main Application Loop
   //----------------------------------
   while (1)
   {
      if(!SW)                          // If the P1.1 switch is pressed
      {
         // Simple de-bounce delay
         for (delay = 0; delay < 250; delay++);
         while (!SW);                  // Wait for switch to be released
         for (delay = 0; delay < 250; delay++);

         // Configure the Port I/O for Low Power Mode
         GREEN_LED = LED_OFF;          // Turn off the LED or other
                                       // high-current devices

         OSCICN |= 0x20;               // Place MCU in suspend mode

         // At this point the device is in suspend mode
         // A port match event on P1.1

         // Simple de-bounce delay
         for (delay = 0; delay < 250; delay++);
         while (!SW);                  // Wait for switch to be released
         for (delay = 0; delay < 250; delay++);

         GREEN_LED = LED_ON;

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
// This function initializes the system clock to use the internal high power
// oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   OSCICN |= 0x83;                     // Enable the precision internal osc.

   CLKSEL = 0x00;                      // Select internal internal osc.
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
// P1.0   digital   push-pull     LED
// P1.4   digital   open-drain    Switch
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P1MDIN |= 0x11;                     // P1.0, P1.1 are digital

   P1MDOUT &= ~0x10;                   // P1.1 is open-drain
   P1MDOUT |= 0x01;                    // P1.0 is push-pull

   P1     |= 010;                      // Set P1.1 latch to '1'

   XBR1    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
