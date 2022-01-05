//-----------------------------------------------------------------------------
// F39x_External_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This software shows the necessary configuration to use External Interrupt 0
// (/INT0) or External Interrupt 1 (/INT1) as an interrupt source.  The code
// executes the initialization routines and then spins in an infinite while()
// loop.  If the button on P1.0 (on the target board) is pressed, then the
// edge-triggered /INT0 input on P0.0 will cause an interrupt and toggle the
// LED.
//
// Pinout:
//
// P0.0 - /INT0
// P0.1 - /INT1
//
// P1.0 - SW1 (Switch)
// P1.1 - LED
//
// How To Test:
//
// 1) Compile and download code to a 'F390/F370 UDP MCU board.
// 2) On the board, connect P1.0 to P0.0 for /INT0 or P0.1 for /INT1.
// 3) Place two shorting blocks on:
//       J8: SW  - P1.0
//       J8: LED - P1.1
// 4) Ensure shorting block is not placed on J10
// 5) Press the switches.  Every time a switch is pressed, the P1.1
//    LED should toggle.
//
// Target:         C8051F39x/37x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (ST)
//    - 16 JAN 2012
//

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F390_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             245000000   // Clock speed in Hz

SI_SBIT(LED, SFR_P1, 1);                 // LED==1 means ON
SI_SBIT(SW1, SFR_P1, 0);                 // SW==0 means switch depressed

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);               // Configure the system clock
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

   SYSCLK_Init();                      // Initialize the system clock
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
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 49MHz
// oscillator divided by 2 as its clock source.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   OSCICN = 0x83;                      // Set system clock to 24.5 MHz
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
// P0.0 - digital   open-drain  /INT0
// P0.1 - digital   open-drain  /INT1
//
// P1.0 - digital   open-drain  SW1 (Switch)
// P1.1 - digital   push-pull   LED
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   XBR1     = 0x40;                    // Enable crossbar and weak pullups

   P0SKIP   = 0x03;
   P1MDOUT  = 0x02;                    // LED is a push-pull output
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

   IT01CF = 0x10;                      // /INT0 active low; /INT0 on P0.0;
                                       // /INT1 active low; /INT1 on P0.1

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
// Whenever a negative edge appears on P0.0, LED1 is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT0_ISR, INT0_IRQn)
{
   LED = !LED;
}

//-----------------------------------------------------------------------------
// /INT1 ISR
//-----------------------------------------------------------------------------
//
// Whenever a negative edge appears on P0.1, LED2 is toggled.
// The interrupt pending flag is automatically cleared by vectoring to the ISR
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(INT1_ISR, INT1_IRQn)
{
   LED = !LED;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
