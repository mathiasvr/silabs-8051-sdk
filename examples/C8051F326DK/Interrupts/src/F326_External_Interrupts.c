//-----------------------------------------------------------------------------
// F326_External_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0) as an interrupt source.  It uses the external interrupt 0 to detect
// rising edge on P0.0 and then toggles a LED.
//
//
// How To Test:
//
// 1) Ensure shorting blocks on C8051F326-TB are placed on the following:
//    J2: VDD  - +3VD
//    J3: P2.2 - P2.2_LED
//    J7: VDD  - VIO
//    J8: Power selection, P1 9V or J4 Debugger
// 2) Connect P2.0_SW pin on J3 to P0.0 on J1.
// 3) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1.
// 4) Compile and download code to the 'F326 target board with Simplicity IDE
//    by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11
// 5) Run the code by selecting Run -> Resume from the menus
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 6) Press the button S2 to toggle the LED D4.
//
//
// Target:         C8051F326/7
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 02 APR 2014
//
// Release 1.0
//    -Initial Revision (GP)
//    -10 JULY 2007
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F326_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             12000000    // Clock speed in Hz

SI_SBIT(LED1, SFR_P2, 2);
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void Ext_Interrupt_Init (void);        // Configure /INT0

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
  // Disable the watchdog here
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
                                       // an interrupt from /INT0
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
// This routine initializes the system clock to use the precision internal
// oscillator as its clock source.
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   OSCICN = 0x83;                      // Set internal oscillator to run
                                       // at its maximum frequency
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the pins.
//
// Pinout:
//
// P0.0 - digital       open-drain      /INT0
// P2.2 - digital       push-pull       LED1
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   GPIOCN   = 0x40;                    // Enable input path and weak pullups

   P2MDOUT  = 0x04;                    // LED1 is a push-pull outputs
}

//-----------------------------------------------------------------------------
// Ext_Interrupt_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures and enables the /INT0 interrupt
//
// The configuration of /INT0 depends on the setting of the TMOD.3
//
// -----------------------------------------------------
// |                    |  TMOD.3 = 0  |  TMOD.3 = 1   |
// -----------------------------------------------------
// |  /INT0 Pinout      |  P0.0        |  P0.2         |
// |  Edge Sensitivity  |  Rising Edge |  Falling Edge |
// |  Level Senstivity  |  Active High |  Active Low   |
// -----------------------------------------------------
//
// See Section 6.3.2 of the 'F326 Datasheet for more details
//
// In this example TMOD.3 = 0, so /INT0 is assigned to P0.0, and is
// Rising Edge Sensitive.
//
//-----------------------------------------------------------------------------
void Ext_Interrupt_Init (void)
{
   IE_EX0 = 1;                         // Enable /INT0 interrupt

   TMOD &= ~0x08;                      // Clear TMOD.3

   TCON |= 0x01;                       // /INT0 is edge sensitive

   // The /INT0 pending flag is set when the /INT0 configuration is changed,
   // so clear it now to prevent vectoring to the ISR immediately when
   // global interrupts are enabled
   TCON_IE0 = 0;

}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// /INT0 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P0.0, LED1 is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
void INT0_ISR (void) interrupt 0
{
   LED1 = !LED1;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
