//-----------------------------------------------------------------------------
// F53xA_PortMatch.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This software shows the necessary configuration to use Port Match as an
// interrupt source. The code executes the initialization routines and then
// spins in an infinite while() loop. If the button on P1.4 (on the target
// board) is pressed, then the Port Match Interrupt will cause the LED to
// toggle.
//
// Pinout:
//
// Port Match: P1.4
//
// P1.3 - LED
// P1.4 - SW1 (Switch 1)
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F530A TB are placed on the following:
//    "A" SIDE
//    HDR4: P1.3_A - P1.3_LED_A
//    HDR4: P1.4_A - P1.4_SW_A
//    "B" SIDE
//    HDR3: P1.3_B - P1.3_LED_B
//    HDR3: P1.4_B - P1.4_SW_B
// 2) Connect the USB Debug Adapter to HDR2 for "A" SIDE or HDR1 for "B" SIDE.
//    Connect the 9V power adapter to P5.
// 3) Compile and download code to C8051F530A target board
//    (either "A" SIDE or "B" SIDE) with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) Press the switch P1.4. Every time the switch is pressed, the P1.3
//    LED should toggle.
//
//
// Target:         C8051F52xA/F53xA (C8051F530A TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 09 SEP 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1
//    -Updated for 'F530A TB (TP)
//    -31 JAN 2008
//
// Release 1.0
//    -Initial Revision (SM)
//    -13 JULY 2007


//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F530_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             245000000   // Clock speed in Hz

SI_SBIT(SW1,  SFR_P1, 4);                // Push-button switch on board
SI_SBIT(LED1, SFR_P1, 3);                // Green LED

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void Interrupt_Init (void);            // Configure Interrupts

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void)
{

   Oscillator_Init();                  // Initialize the system clock
   Port_Init ();                       // Initialize crossbar and GPIO
   Interrupt_Init();                   // Initialize External Interrupts

   IE_EA = 1;

   P1MAT = 0x10;                       // Set up the port-match value on P1
   P1MASK = 0x10;                      // An event will occur if
                                       // (P1 & P1MASK) != (P1MAT & P1MASK)
                                       // The SW1 pin = 1 if the switch isn't
                                       // pressed

   while(1);                           // Infinite while loop waiting for
                                       // an interrupt
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
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   // Set internal oscillator to run at its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// Pinout:
//
// P1.3 - digital   push-pull   LED
// P1.4 - digital   open-drain  Switch1 (SW1)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT  = 0x08;                    // LED is a push-pull output

   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
          XBR1_XBARE__ENABLED;
}

//-----------------------------------------------------------------------------
// Interrupt_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function enables Port Match Interrupts.
//
//-----------------------------------------------------------------------------
void Interrupt_Init (void)
{
   EIE1 |= EIE1_EMAT__ENABLED;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PortMatch_ISR
//-----------------------------------------------------------------------------
//
// Whenever an edge appears on P1.4, the LED is toggled.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PORT_Match_ISR, PMATCH_IRQn)
{
   // If the Port Match event occured on Port 1, then the LED is toggled
   if((P1 & P1MASK) != (P1MAT & P1MASK))
   {
      LED1 = !LED1;
   }

   // Waiting for switch 1 to be released
   while((P1 & P1MASK) != (P1MAT & P1MASK));
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------