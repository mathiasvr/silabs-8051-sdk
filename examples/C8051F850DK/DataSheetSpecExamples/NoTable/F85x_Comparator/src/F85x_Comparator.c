//-----------------------------------------------------------------------------
// F85x_Comparator.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Refer to F85x_Comparator_Readme.txt for more information 
// regarding this code example's functionality. 
//
//
// Important User Warning:
//   If an 'F85x MCU is programmed to enter stop mode immediately post-reset,
//   the debugger will no longer be able to reprogram or interact with the
//   board, rendering it inoperable. This example code features a switch
//   which acts as a safeguard to prevent the MCU from entering stop mode
//   until after the switch is pressed. If the user chooses to remove this 
//   safeguard, they do so at their own risk.
//
//
// How To Test (Comparator - Functionality):
// -----------------------------------------
// 1) Compile and download the code to your C8051F85x_86x board.
// 2) Connect probes from an oscilloscope monitoring P0.0 and/or P0.1.
//    P0.0 corresponds to CP0 and P0.1 corresponds to CP0A, as configured
//    in the crossbar.
// 3) Connect the POT header pin to P0.2 on your board.
// 4) Ensure that the JP2 (Imeasure) and J27-S1 (P1.7) shorting blocks are 
//    connected, and remove all other shorting blocks.    
// 5) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8. Use the variable 
//    potentiometer to change the comparator status.
// 7) Use the oscilloscope to monitor the status of both the asynchronous
//    and synchronous comparator outputs. P0.0 corresponds to the synchronous
//    output, while P0.1 corresponds to the asynchronous output.
// 8) Push the switch, S1, to enable Stop Mode. While in this mode, the
//    asynchronous output will still function, while the synchronous
//    output will not.
//
//
// Target:         C8051F85x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 5 FEB 2014
//
// Release 1.0
//    - Initial Revision (PMD)
//    - 8 JUL 2013
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

SI_SBIT(S1, SFR_P1, 7);

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void CMP0_Init (void);
void Port_Init (void);

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
   WDTCN = 0xDE;                       // Disable watchdog timer
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{


   CMP0_Init ();                       // Initialize Comparator 0
   Port_Init ();                       // Initialize switch and route crossbar
                                       // pins CP0 and CP0A

   while (S1 == SW_NOT_PRESSED){}      // Wait until S1 is pushed
   
   PCON |= 0x02;                       // Enter Stop Mode
   
   while (1){}                 
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMP0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initializes comparator to serve as killswitch for CPA0.
//
//-----------------------------------------------------------------------------
void CMP0_Init(void)
{
   CPT0CN = 0x80;                      // Comparator enabled
   CPT0MX = 0x28;                      // P0.2 = Negative Input
                                       // LDO = Positive Input
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
// P1.7   digital   open-drain   S1 (Switch 1)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // S1 (P1.7) Input - Oscillator Clock Cycle Switch
   P1MDIN |= 0x80;                     // P1.7 is digital
   P1MDOUT &= ~0x80;                   // P1.7 is open-drain
   P1 |= 0x80;                         // Set the P1.7 latch to '1'
   
   // Configure Crossbar
   XBR0 = 0x18;                        // Enable CP0 and CP0A Outputs
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
