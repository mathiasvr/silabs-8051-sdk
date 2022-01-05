//-----------------------------------------------------------------------------
// F91x_External_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0) or External Interrupt 1 (/INT1) as an interrupt source.  The code
// executes the initialization routines and then spins in an infinite while()
// loop.  If the button on P0.2 (on the target board) is pressed, then the
// edge-triggered /INT0 input on P0.2 will cause an interrupt and toggle the
// yellow LED. If the button on P0.3 is pressed, then the edge-triggered
// /INT1 input on P0.3 will cause an interrupt and toggle the Red LED.
//
// Pinout:
//
// P0.2 - /INT0 (Switch 2)
// P0.3 - /INT1 (Swtich 3)
//
// P1.5 - Red LED
// P1.6 - Yellow LED
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
// 6) Press the switches.  Every time a switch is pressed, one of
//    the LEDs should toggle.
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
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,    SFR_P1, 5);          // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED, SFR_P1, 6);          // '0' means ON, '1' means OFF
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch depressed
SI_SBIT(SW3,        SFR_P0, 3);          // SW3 == 0 means switch depressed


//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             20000000    // Clock speed in Hz

#define LED_ON  0
#define LED_OFF 1


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void Ext_Interrupt_Init (void);        // Configure External Interrupts (/INT0
                                       // and /INT1)


SI_INTERRUPT_PROTO(INT0_ISR, INT0_IRQn);
SI_INTERRUPT_PROTO(INT1_ISR, INT1_IRQn);

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
   PCA0MD &= ~0x40;                    // Disable Watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   Oscillator_Init();                  // Initialize the system clock
   Port_Init ();                       // Initialize crossbar and GPIO
   Ext_Interrupt_Init();               // Initialize External Interrupts

   IE_EA = 1;

   while(1);                           // Infinite while loop waiting for
                                       // an interrupt from /INT0 or /INT1
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal low power
// oscillator at its maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   CLKSEL = 0x04;                     // Select low power oscillator with a 
                                      // clock divider value of 1 (20MHz)
}


//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// Pinout:
//
// P0.2   digital   open-drain    Switch 2
// P0.3   digital   open-drain    Switch 3
// P1.5   digital   push-pull     Red LED
// P1.6   digital   push-pull     Orange LED
//-----------------------------------------------------------------------------
void Port_Init (void)
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
// Ext_Interrupt_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures and enables /INT0 and /INT1 (External Interrupts)
// as negative edge-triggered.
//
//-----------------------------------------------------------------------------
void Ext_Interrupt_Init (void)
{
   TCON = 0x05;                        // /INT 0 and /INT 1 are edge triggered

   IT01CF = 0x32;                      // /INT0 active low; /INT0 on P0.2;
                                       // /INT1 active low; /INT1 on P0.3

   IE_EX0 = 1;                            // Enable /INT0 interrupts
   IE_EX1 = 1;                            // Enable /INT1 interrupts
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// /INT0 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P0.2, YELLOW_LED is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT0_ISR, INT0_IRQn)
{
   YELLOW_LED = !YELLOW_LED;
}

//-----------------------------------------------------------------------------
// /INT1 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P0.3, RED_LED is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT1_ISR, INT1_IRQn)
{
   RED_LED = !RED_LED;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
