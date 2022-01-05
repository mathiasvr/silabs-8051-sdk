//-----------------------------------------------------------------------------
// F85x_Comparator_Performance.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// How to Use:
//
// 1) Ensure that the shorting blocks are attached to the J27-S1 (P1.7)
//    and the J27-S2 (P2.1) headers.
// 2) Remove the JP2 (Imeasure) shorting block and attach an ammeter between
//    the JP2 (Imeasure) headers.
// 3) Remove all other shorting blocks.
// 4) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) To cycle between comparator enable and disable, push S1 (P1.7). The new
//    current cannot be read until the user lets go of S1.
// 7) To cycle between the different comparator modes, push S2 (P2.1). The new
//    mode will not be set until the user lets go of S2.
//
// Target:         C8051F85x
// Target Board:   UPMU-F850 MCU-100
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - 5 FEB 2014
//
// Release 1.0
//    - Initial Revision 
//    - 11 JULY 2013
//
//
// Refer to F85x_Comparator_Performance_Readme.txt for more information.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK          119406         // Clock speed in Hz
#define SW_PRESSED           0
#define SW_NOT_PRESSED       1
SI_SBIT(S1, SFR_P1, 7);
SI_SBIT(S2, SFR_P2, 1);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);               // Configure the system clock
void Comparator0_Init (void);          // Configure Comparator0
void Port_Init(void);

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
   WDTCN = 0xDE;
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// MAIN Routine

void main (void)
{
   uint8_t inputlatch = 0;
   uint8_t inputlatch2 = 0;
   uint8_t compmode = 0;
   // Disable the watchdog timer

   SYSCLK_Init();                      // Initialize the system clock
   Comparator0_Init();                 // Initialize Comparator0
   Port_Init();
   
   while(1) 
   {
      if (S1 == SW_PRESSED && (inputlatch == 0))   // When switch is press, CPT is turned
      {                                            // on/off
	      CPT0CN ^= 0x80;
		   inputlatch = 1;
      }
 
      if (S1 == SW_NOT_PRESSED)
      {
         inputlatch = 0;
      }

      if (S2 == SW_PRESSED)
      {
         CPT0MD &= 0xFC;
         CPT0MD |= compmode;
         inputlatch2 = 1;
      }

      if ((S2 == SW_NOT_PRESSED) && (inputlatch2 == 1))
      {
         switch (compmode)
         {
            case 0: compmode = 1;
            break;

            case 1: compmode = 2;
            break;

            case 2: compmode = 3;
            break;

            case 3: compmode = 0;
            break;

            default: while(1) {};         // Error with Comparator Mode
            break;
         }

         inputlatch2 = 0;
      }
   }
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
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator divided by 1 as its clock source.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   CLKSEL = 0x70;                      // Set system clock to 119 kHz
}

//-----------------------------------------------------------------------------
// Comparator0_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the comparator to pins P0.0 and P0.1
//-----------------------------------------------------------------------------

void Comparator0_Init (void)
{
   int16_t j  = 0;

   CPT0CN = 0x80;                      // Comparator enabled
   CPT0MD = 0x00;		                  // Comp Mode 0
   CPT0MX = 0x10;                      // P0.1 = Negative Input
                                       // P0.0 = Positive Input
   for(j = 0; j < 10000; j++);         // Account for Settling Time of CPT
}

//-----------------------------------------------------------------------------
// Port_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the crossbar and GPIO ports.
//
// P1.7  digital  push-pull      Comparator Enable/Disable Switch
// P2.1  digital  push-pull      Comparator Mode Switch
//-----------------------------------------------------------------------------

void Port_Init (void)
{
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   P1MDOUT &= ~0x80;
   P1 |= 0x80;
   P2MDOUT &= ~0x02;
   P2 |= 0x80;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
