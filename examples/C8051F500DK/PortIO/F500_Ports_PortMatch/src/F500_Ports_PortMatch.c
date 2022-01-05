//-----------------------------------------------------------------------------
// F500_Ports_PortMatch.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to use the Port Match feature. The firmware
// enables the port match interrupt and puts the device in idle mode. Pressing
// a switch wakes the device up from idle mode and turns on an LED in the Port
// Match ISR. Releasing the switch turns off the LED and the re-enters idle
// mode.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    J19: P1.3 LED
//    J19: P1.4 - P1.4_SW
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 2) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 3) Compile and download code to C8051F500 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) Push and hold the switch (P1.4) and see that the LED is on. And then
//    release the switch and see that the LED turns off.
//
//
// Target:         C8051F500 (Side A of a C8051F500-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 11 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1 / 11 JUN 2008 (ADT)
//    -Edited formatting
//
// Release 1.0 / 08 APR 2008 (GP)
//    -Initial Revision


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P1, 3);                 // LED == 1 means ON
SI_SBIT(SW1, SFR_P1, 4);                 // SW1 == 0 means switch depressed

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PORT_Match_Init (void);

SI_INTERRUPT_PROTO(PORT_Match_ISR, PMATCH_IRQn);

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = LEGACY_PAGE;              // Set SFR Page for PCA0MD


   PORT_Init ();                       // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   PORT_Match_Init ();                 // Enable the Port Match interrupt

   IE_EA = 1;                          // Enable global interrupts

   while (1)
   { 	  
	   // Put the device in idle mode
      LED = 0;                         // Turn off LED
      PCON |= PCON_IDLE__IDLE;         // Enter Idle mode
      PCON = PCON;                     // Safety dummy instruction

      // Pressing the switch pulls P1.4 low which triggers the Port Match 
      // interrupt.  When the Port Match interrupt occurs, the device wakes up
      // from Idle mode and vectors to the Port_Match_ISR

      // Port Match is a level sensitive interrupt, so the firmware waits for
      // the switch to be released in the ISR before proceeding.

      // Once the ISR is finished, code execution resumes after the instruction
      // that put the device in idle mode
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
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   // Configure internal oscillator for 24 MHz / 8
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_8;

   SFRPAGE = LEGACY_PAGE;
   RSTSRC = 0x04;                      // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;
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
// P1.3   digital   push-pull     LED
// P1.4   digital   open-drain    SW1
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P1MDIN |= 0x08;                     // P1.3 is digital
   P1MDIN |= 0x10;                     // P1.4 is digital

   P1MDOUT |= 0x08;                    // P1.3 is push-pull
   P1MDOUT &= ~0x10;                   // P1.4 is open-drain

   P1     |= 0x10;                     // Set P1.4 latch to '1'

   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Match_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Enable the Port Match interrupt for P1.4.  Whenever P1.4 is logic low, the
// Port Match interrupt is active.
//
//-----------------------------------------------------------------------------

void PORT_Match_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   P1MASK = 0x10;                      // Set P1.4 as the pin to monitor
   P1MAT  = 0x10;                      // Interrupt when P1.4 is low

   EIE2 |= EIE2_EMAT__ENABLED;         // Enable Port Match Interrupts

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Match_ISR
//-----------------------------------------------------------------------------
//
// Whenever the Port_Match ISR is active, turn on the LED.  Since the Port
// Match interrupt is level sensitive and not edge sensitive, the ISR
// waits for the switch to be released  and the interrupt is cleared before
// exiting Otherwise, the Port_Match ISR would constantly be triggered until
// the switch was released.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(PORT_Match_ISR, PMATCH_IRQn)
{
   LED = 1;                            // Turn on LED

   while (SW1 == 0);                   // Wait for switch release
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------