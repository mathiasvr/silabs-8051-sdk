//-----------------------------------------------------------------------------
// F91x_Voltage_Monitor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// Program Description:
// --------------------
// This program demonstrates the Voltage Supply Monitor power consumption 
// listed in Table 4.4 of the datasheet. This program also demonstrates the 
// VDD Supply Monitor Threshold listed in Table 4.4 of the datasheet. The 
// supply monitor is enabled at the start of the program and switch SW0_2 is
// used to toggle the enable bit. The power consumption can be tested by 
// measuring the current of both modes and taking the difference. The supply
// monitor is selected as a reset source when the supply monitor is enabled. 
// When the supply monitor is disabled, it is deselected as a reset source.
//
//
// How To Test:
// ------------
//  1) Ensure a shorting block is placed across VBAT & WALL_PWR (J11). 
//  2) Place VDD Select SW4 in the 2 Cell (3.3v) position and power the board
//     using the P2 power connector.
//  3) Download and run the code on a C8051F912-TB target board.
//  4) Remove all shorting blocks on the board except for the block across 
//     the following pins:
//     a) LED1.5 and P1.5 on J8
//     b) P0.2H and SW0.2 on J8
//  5) Connect an ammeter across Imeasure (J17).
//  6) Record the current reading. This reading is the current with the voltage
//     monitor enabled.
//  7) Use switch at P0.2 (SW0.2) to disable the voltage monitor.
//  8) Record the current reading and subtract the value from the previously
//     recorded value. Verify the difference is within the range of the values
//     listed on the datasheet for Voltage Monitor supply current under Table 4.4.
//  9) Use SW0.2 to enable the voltage monitor.
// 10) Using the IDE, stop the program and disconnect the board.
// 11) Remove any and all power sources to the board including the debugger.
// 12) Remove the ammeter from Imeasure and replace the shorting block across
//     Imeasure.
// 13) Connect a power supply to VDD/DC+ and set to 3.3V (LED1.5 ON).
// 14) Use an oscilloscope to monitor /RST-SW (J9 Pin5). 
// 15) Reduce the supply voltage until RESET is pulled low (LED1.5 OFF). 
//     Verify the voltage at which RESET is pulled low is within the values 
//     given for VDD Supply Monitor Threshold in Table 4.4. 
//
//
// Target:         C8051F91x/90x
// Tool Chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (MT)
//    - 22 OCT 2013
//
//
// Refer to F91x_Voltage_Monitor_Readme.txt for more information. 
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SW_PRESSED      0
#define SW_NOT_PRESSED  1

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(SW0_2, SFR_P0,2);

SI_SBIT(DS3, SFR_P1,5);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
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
    PCA0MD &= ~0x40;                     
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   
    // Disable the watchdog timer

    SYSCLK_Init ();                     // Initialize system clock to 24.5 MHz
    Port_Init ();                       // Initialize crossbar and GPIO
	
    IE_EA = 1;                             // Enable global interrupts

    // Loop forever
    while (1) {}
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator divided by 1 as its clock source.  Also enables missing clock
// detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
    VDM0CN |= 0x80;                     // Enable VDD Monitor 

    // Select Precision Internal Oscillator 24.5MHz as system
    // clock source
    REG0CN |= 0x10;
    OSCICN |= 0x80;	
    CLKSEL = 0x00; 
    
    // Waiting for above the VDD monitor warning threshold
    while (!(VDM0CN & 0x20));
    RSTSRC = 0x02;                      // Enable missing clock detector and
                                        // set VDD Monitor as reset source
    
    //At clock frequencies above 10 MHz, the system clock cycle becomes 
    //short enough that the one-shot timer no longer provides a power benefit.
    //Disabling the one-shot timer at higher frequencies 
    //reduces power consumption. 
    FLSCL = 0x40;	
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the Crossbar and GPIO ports.
//
// P0.2     digital     open-drain     SW0_2 
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{

   // Port 0 initialization
   P0MDIN |= 0x04;                      // Set P0.2 as digital
   P0MDOUT &= ~0x04;                    // Set P0.2 as open-drain
   P0 |= 0x04;                          // Set the P0.2 latch to '1'

   P0MAT  |= 0x04;                      // Set up the port-match value on P0.2
   P0MASK |= 0x04;                      // An event will occur if
                                        // (P0 & P0MASK) != (P0MAT & P0MASK)
                                        // The switch pins are 1 if the switch
                                        // isn't pressed.

   EIE2 |= 0x02;                        // Enable Port Match interrupts
   DS3 = 0;                             // DS3 ON
   XBR2 = 0x40;                         // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port Match ISR
//-----------------------------------------------------------------------------
//
// Whenever SW0_2 (P0.2) is pressed, the VDD Monitor enable is toggled. When the
// VDD Monitor is enabled, the VDD Monitor is selected as a reset source. When
// the VDD Monitor is disabled, the VDD Monitor is deselected as a reset 
// source.
//
// The Port Match interrupt occurs whenever (Pn & PnMASK) != (PnMAT & PnMASK).
// We must disable the interrupt and wait until the switches are released in
// main, or else the interrupt will trigger multiple times.  It is generally
// not good practice to sit in an ISR for long periods of time.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PM_ISR, PMATCH_IRQn)
{
   if (SW0_2 == SW_PRESSED)
   {
      // If the Voltage Supply Monitor is enabled
      if((VDM0CN & 0x80) == 0x80)
      {
         VDM0CN &= ~0x80;              // Disable VDD Monitor
         RSTSRC &= ~0x02;              // Clear VDD Monitor as reset source
      } 
      else
      {
         VDM0CN |= 0x80;               // Enable VDD Monitor
         RSTSRC |= 0x02;               // Set VDD Monitor as reset source
      }

      P0MAT &= ~0x04;                  // Set port match to trigger at switch
                                       // rising edge to prevent multiple 
                                       // interrupts from occurring while the
                                       // switch is pressed   
   }
   else
   {
      P0MAT |= 0x04;                   // Reset port match to trigger at switch
                                       // falling edge
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
