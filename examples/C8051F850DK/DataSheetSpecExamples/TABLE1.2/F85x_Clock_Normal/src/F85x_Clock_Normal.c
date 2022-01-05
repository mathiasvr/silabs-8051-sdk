//-----------------------------------------------------------------------------
// F85x_Clock_Normal.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// Refer to F85x_Clock_Normal_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 1.2 - Normal Mode Current):
// -------------------------------------------------
// 1) Ensure that shorting blocks are attached to the P1.7-S1 (J27) and P2.1-S2
//    (J27) headers.
// 2) Remove the JP2 (Imeasure) shorting block and attach an ammeter between
//    the JP2 (Imeasure) headers.
// 3) Remove all other shorting blocks.
// 4) If oscilloscope measurements are desired, attach an oscilloscope probe
//    to P0.0.
// 5) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 7) To cycle between oscillator frequencies, push S1 (P1.7).
// 8) To cycle between measurement modes, push S2 (P2.1).
//
//
// Target:         C8051F85x
// Target Board:   UPMU-F850 MCU-100
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 5 FEB 2014
//
// Release 1.0
//    - Initial Revision (PD)
//    - 24 JUN 2013
//
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
SI_SBIT(S2, SFR_P2, 1);

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

//-----------------------------------------------------------------------------
// Mode Variables
//-----------------------------------------------------------------------------

// Mode type enumerations
enum CLOCK_NORMAL_MODE {HFOSC_FAST = 1,
                        HFOSC_SLOW = 2,
                        LFOSC_NORM = 3};
						
enum CLOCK_NORMAL_MODE currentMode;

// Debounce latch for oscillator mode
uint8_t modeLatch = 0;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Oscillator_Update (void);
void Port_Init (void);
void Port_Update (void);

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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{

   // Disable the watchdog timer

   // Default Mode: 24.5 MHz High-Frequency Oscillator
   currentMode = HFOSC_FAST;

   // Initialize Port I/O (S1, S2, and SYSCLK output)
   Port_Init();             
      
   // Initialize Oscillator (Default is 24.5 MHz HFOSC)
   Oscillator_Init ();         

   while (1)
   {

     // Detect switch inputs and adjust modes
     Port_Update();

     // Update oscillator with new modes
     Oscillator_Update();

   } 
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
// This function initializes the system clock to use the internal high 
// frequency oscillator at the maximum frequency.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled
   
   CLKSEL = 0x00;                      // Enable HFOSC (CLKDIV = 1)				   
	
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
// P0.0   digital   push-pull    output   SYSCLK (System Clock)
// P1.7   digital   open-drain   input    S1 (Switch 1)
// P2.1   digital   open-drain   input    S2 (Switch 2)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // S1 (P1.7) Input - Oscillator Clock Cycle Switch
   P1MDIN |= 0x80;                     // P1.7 is digital
   P1MDOUT &= ~0x80;                   // P1.7 is open-drain
   P1 |= 0x80;                         // Set the P1.7 latch to '1'

   // S2 (P2.1) Input - Measurement Mode Switch
   P2MDOUT &= ~0x02;                   // P2.1 is open-drain
   P2 |= 0x02;                         // Set the P2.1 latch to '1'
   
   // Buffered System Clock Output
   P0MDOUT |= 0x01;                    // P0.0 is push-pull output

   // Crossbar Initialization
   XBR0 |= 0x80;                       // Route /SYSCLK to first available pin
   XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function checks switch input and cycles through oscillator
// modes, once each time the switch is pressed. The cycling order 
// is as follows:
//
// Mode 1: High-Frequency Oscillator, 24.5 MHz
// Mode 2: High-Frequency Oscillator, 1.53 MHz
// Mode 3: Low-Frequency Oscillator,    80 kHz
//
// P1.7   digital   open-drain   input   S1 (Switch 1)
// P2.1   digital   open-drain   input   S2 (Switch 2)
//
//-----------------------------------------------------------------------------
void Port_Update(void)
{

   // Debounce latches for S1 and S2
   static uint8_t switch1Latch = 0; 
   static uint8_t switch2Latch = 0;

   // Cycle frequency mode if P1.7 is pushed
   if( (S1 == SW_PRESSED) && (!switch1Latch) )
   {
      switch (currentMode)
      {

         case HFOSC_FAST:
              currentMode = HFOSC_SLOW;
              break;
         case HFOSC_SLOW:
              currentMode = LFOSC_NORM;
              break;
         case LFOSC_NORM:
              currentMode = HFOSC_FAST;
              break;
      }
      // Arm latches to prevent auto-cycling with switch held
      switch1Latch = 1;
	   modeLatch = 1;
   }

   // Reset latch once switch is released
   else if(S1 == SW_NOT_PRESSED)
   {
      switch1Latch = 0;
   }

   // Cycle measurement mode if P2.1 is pushed
   if( (S2 == SW_PRESSED) && (!switch2Latch) )
   {
      // Toggle P0.0 between push-pull on/off
      P0MDOUT ^= 0x01; 	 
      XBR0 ^= 0x80;

      // Arm latches to prevent auto-cycling with switch held	  
      switch2Latch = 1;
   }

   // Reset latch once switch is released
   else if(S2 == SW_NOT_PRESSED)
   {
      switch2Latch = 0;
   }

}

//-----------------------------------------------------------------------------
// Oscillator_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes new oscillator settings based on the operating
// mode chosen by the user. This function only changes oscillator settings
// once each time the switch is pressed.
//
//-----------------------------------------------------------------------------
void Oscillator_Update(void)
{

   if( modeLatch )
   {
      // Cycle through oscillator settings
      switch(currentMode)
	   {

         case HFOSC_FAST :
		      CLKSEL = 0x00;           // Enable HFOSC (CLKDIV = 1)
			   break;

         case HFOSC_SLOW :
		      CLKSEL = 0x40;           // Enable HFOSC (CLKDIV = 16)
			   break;

         case LFOSC_NORM :
            OSCLCN |= 0x83;          // Enable LFOSC (CLKDIV = 1)
			  
			   // Poll until the LFOSC is ready
            while ((OSCLCN & 0x40) != 0x40);
              
			   CLKSEL = 0x02;           // Switch to LFOSC
			   break;

	  }
	  // Reset latch once mode is changed
	  modeLatch = 0;
   }

   if( currentMode != LFOSC_NORM )
   {
      OSCLCN &= ~0x80;                 // Disable LFOSC if using HFOSC modes
   }

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
