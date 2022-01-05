//-----------------------------------------------------------------------------
// F85x_Clock_Idle.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// Refer to F85x_Clock_Idle_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 1.2 - Idle Mode Current):
// -----------------------------------------------
// 1) Ensure that the shorting blocks are attached to the P1.7-S1 (J27) header.
// 2) Remove the JP2 (Imeasure) shorting block and attach an ammeter between
//    the JP2 (Imeasure) headers.
// 3) Remove all other shorting blocks.
// 4) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) To cycle between oscillator frequencies, push S1 (P1.7). The new idle 
//    mode current cannot be read until the user lets go of S1.
//     
//
// Target:        C8051F85x
// Target Board:   UPMU-F850 MCU-100
// Tool chain:    Simplicity Studio / Keil C51 9.51
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

//-----------------------------------------------------------------------------
// Global Declarations
//-----------------------------------------------------------------------------

#define SW_PRESSED           0
#define SW_NOT_PRESSED       1

//-----------------------------------------------------------------------------
// Mode Variables
//-----------------------------------------------------------------------------

// Mode type enumerations
enum CLOCK_IDLE_MODE {HFOSC_FAST = 1,
                      HFOSC_SLOW = 2,
	                   LFOSC_NORM = 3};
						
enum CLOCK_IDLE_MODE nextMode;

// Debounce latches for S1
bit switch1Latch = 0; 

// Debounce latch for oscillator mode
bit modeLatch = 0;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(PM_ISR,PMATCH_IRQn);

void Oscillator_Init (void);
void Oscillator_Cycle (void);
void Port_Init (void);
void Enter_Sleep (void);

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

   // Starting Mode: 24.5 MHz High-Frequency Oscillator
   nextMode = HFOSC_SLOW;

   // Initialize Port I/O (S1)
   Port_Init();             
      
   // Initialize Oscillator (Default is 24.5 MHz HFOSC)
   Oscillator_Init();  
     
   while (1)
   {
   
      // Wait until user lets go of S1 before sleeping
      while( S1 == SW_PRESSED ){}
         
      // Sleep mode, ends upon S1 Port Match event
      Enter_Sleep();

      // Cycle to next oscillator frequency after wake up
	  Oscillator_Cycle();

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
// This function initializes the system clock to use the internal precision
// oscillator at the maximum frequency.
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
// This function configures the crossbar and ports pins. The switch used
// is configured to both wake up from sleep mode and change the oscillator
// operating frequency.
//
// P1.7   digital   open-drain   input   S1 (Switch 1)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // S1 (P1.7) Input - Oscillator Clock Cycle Switch
   P1MDIN |= 0x80;                     // P1.7 is digital
   P1MDOUT &= ~0x80;                   // P1.7 is open-drain
   P1 |= 0x80;                         // Set the P1.7 latch to '1'
   
   // Set Port Match settings for S1 (P1.7)
   P1MASK |= 0x80;                     // Enable Port Match check for P1.7
   P1MAT  |= 0x80;                     // Set Port Match value to 1 for P1.7

   // Crossbar Initialization
   XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Runtime Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Enter_Sleep
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Puts the processor into sleep mode, in order to allow the user
// to measure idle mode current draw. Sleep mode ends when S1 (P1.7) is
// pushed.
//
// P1.7   digital   open-drain   input   S1 (Switch 1)
//
//-----------------------------------------------------------------------------
void Enter_Sleep (void)
{
   uint8_t IE_save;
   uint8_t EIE1_save;

   IE_EA = 0;                             // Clear global interrupt enable

   // Save interrupt state
   IE_save = IE;                       // Preserve IE register
   EIE1_save = EIE1;                   // Preserve EIE1 register
   IE = 0x00;                          // Disable interrupts
   
   EIE1 = 0x02;                        // Enable port match interrupts
   IE_EA = 1;                             // Enable global interrupts

   // Go into idle mode
   PCON |= 0x01;                       // Idle mode
   PCON = PCON;                        // Follow the instruction to enter idle
                                       // with a 3 cycle dummy instruction
								
   //--sleep mode--//
   //--wake on port match event--//

   EIE1 = EIE1_save;                   // Restore ext interrupts
   IE = IE_save;                       // Restore interrupts
   IE_EA = 1;                             // Set global interrupt enable

}

//-----------------------------------------------------------------------------
// Oscillator_Cycle
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function sets the internal oscillator to the next frequency setting.
// The process occurs once each time the processor is started up from sleep
// mode after a port match interrupt.
//
// Mode 1: High-Frequency Oscillator, 24.5 MHz
// Mode 2: High-Frequency Oscillator, 1.53 MHz
// Mode 3: Low-Frequency Oscillator,    80 kHz
//
//-----------------------------------------------------------------------------
void Oscillator_Cycle(void)
{

      // Cycle through oscillator settings
      switch(nextMode)
	  {

        case HFOSC_FAST :
		     CLKSEL = 0x00;           // Enable HFOSC (CLKDIV = 1)
		     nextMode = HFOSC_SLOW;
			  break;

        case HFOSC_SLOW :
   	     CLKSEL = 0x40;           // Enable HFOSC (CLKDIV = 16)
		     nextMode = LFOSC_NORM;
			  break; 

        case LFOSC_NORM :
           OSCLCN |= 0x83;          // Enable LFOSC (CLKDIV = 1)
			  
			  // Poll until the LFOSC is ready
           while ((OSCLCN & 0x40) != 0x40);
			  
           CLKSEL = 0x02;           // Switch to LFOSC
		     nextMode = HFOSC_FAST;
		     break;

	  }

}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// This interrupt wakes the processor from idle mode, and proceeds to continue
// to the Oscillator_Cycle() function.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for int32_t periods of time.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PM_ISR,PMATCH_IRQn)
{

   EIE1 &= ~0x02;                      // Disable Port Match interrupts to
                                       // prevent multiple interrupts from
                                       // occurring while the switches are
                                       // pressed
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
