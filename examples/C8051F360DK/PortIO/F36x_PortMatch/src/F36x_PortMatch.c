//-----------------------------------------------------------------------------
// F36x_PortMatch.c
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
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: SW3.0 - J3: P1.4
//    J12: SW3.1 - J4: P2.4
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
// 5) Press the switches. Every time a switch is pressed, the P3.2 or P3.3
//    LED should toggle.
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
//    -Initial Revision (SM)
//    -13 JULY 2007


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

void Oscillator_Init (void);
void Port_Init (void);
void Interrupt_Init (void);

SI_INTERRUPT_PROTO(PORTMATCH_ISR, PMATCH_IRQn);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

SI_SBIT(SW1,  SFR_P1, 4);                 // Push-button switch on board
SI_SBIT(SW2,  SFR_P2, 4);                 // Push-button switch on board
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

   Oscillator_Init ();                 // Initialize 24.5 MHz oscillator
   Port_Init ();                       // Initialize IO ports
   Interrupt_Init ();                  // Initialize Port Match Interrupts

   IE_EA = 1;                          // Set Global interrupt enable

   SFRPAGE = LEGACY_PAGE;

   P1MAT = 0x10;                       // Set up the port-match value on P2
   P1MASK = 0x10;                      // An event will occur if
                                       // (P1 & P1MASK) != (P1MAT & P1MASK)
                                       // The SW1 pin = 1 if the switch isn't
                                       // pressed
   P2MAT = 0x10;                       // Set up the port-match value on P2
   P2MASK = 0x10;

   while (1) {}                        // Wait for interrupt
}

//-----------------------------------------------------------------------------
// Port_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P1.4 - digital       open-drain      SW1 (Switch 1)
// P2.4 - digital       open-drain      SW2 (Switch 2)
// P3.2 - digital       push-pull       LED1
// P3.3 - digital       push-pull       LED2
//
//-----------------------------------------------------------------------------
void Port_Init ()
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P1MDOUT &= ~0x10;                   // P1.4 is open drain
   P2MDOUT &= ~0x10;                   // P2.4 is open drain
   P3MDOUT |= 0x0C;                    // configure P3.2,P3.3 to push-pull

   XBR1 |= XBR1_XBARE__ENABLED;        // Enable crossbar

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1; // Initialize internal oscillator to
                                       // highest frequency

   RSTSRC  = 0x04;                     // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
   EIE2 |= EIE2_EMAT__ENABLED;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORTMATCH_ISR
//-----------------------------------------------------------------------------
//
// Whenever an edge appears on P1.4, LED1 is toggled.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PORTMATCH_ISR, PMATCH_IRQn)
{
   // If the Port Match event occured on Port 1 than LED1 is toggled
   if((P1 & P1MASK) != (P1MAT & P1MASK))  {
      LED1 = !LED1; }

   // Waiting for switch 1 to be released
   while((P1 & P1MASK) != (P1MAT & P1MASK));

   // If the Port Match event occured on Port 0 than LED2 is toggled
   if((P2 & P2MASK) != (P2MAT & P2MASK))  {
      LED2 = !LED2;  }

   // Waiting for switch 2 to be released
   while((P2 & P2MASK) != (P2MAT & P2MASK));
}

//-----------------------------------------------------------------------------
// End of File
//-----------------------------------------------------------------------------