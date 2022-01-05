//-----------------------------------------------------------------------------
// F96x_SmaRTClock.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the output frequency of the internal Low Frequency
// Oscillator, as specified on table 4.11 of the datasheet.
//
// Refer to F96x_SmaRTClock_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.11 - Oscillator Frequency (LFO)):
// -------------------------------------------------
// 1) Remove the VBAT CURRENT shorting block (J15) and connect a DC Power
//    supply to VBAT and GND.
// 2) Remove all other shorting blocks and select VBAT on SW5.
// 3) Connect an oscilloscope probe to P0.0 and GND.
// 4) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Measure the frequency output on P0.0 while changing the output from
//    1.8 V to 3.8 V.
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 6 FEB 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 04 NOV 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Register_Enums.h"

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

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
    PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
void Oscillator_Init (void);
void Port_Init (void);
void RTC_Init (void);
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
    
                                        // enable)

    Port_Init();                        // Initialize Port I/O

    // Select  SmaRTClock as system clock source.
    RTC_Init ();
    while (1);
  
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P0.0   digital    push-pull             output system clock 
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    P0MDIN |= 0x01;                     // P0.0 is digital
    P0MDOUT |= 0x01 ;                   // P0.0 is output
   
    // Crossbar Initialization
    XBR0 = 0x08;                        // Route /SYSCLK to first available pin
    XBR2 = 0x40;                        // Enable Crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// RTC_Write ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint8_t reg - address of RTC register to write
//                2) uint8_t value - the value to write to <reg>
//
// This function will Write one byte to the specified RTC register.
// Using a register number greater that 0x0F is not permitted.
//-----------------------------------------------------------------------------
void RTC_Write (uint8_t reg, uint8_t value)
{
   uint8_t  restoreSFRPAGE;
   restoreSFRPAGE = SFRPAGE;
   
   SFRPAGE = LEGACY_PAGE;
   reg &= 0x0F;                        // mask low nibble
   RTC0ADR  = reg;                     // pick register
   RTC0DAT = value;                    // write data
   
   SFRPAGE= restoreSFRPAGE;
}

//-----------------------------------------------------------------------------
// RTC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the RTC. First it unlocks the RTC interface,
// enables the RTC, clears ALRMn and CAPTUREn bits. Then it sets up the RTC
// to operate using a 16.4khZ Low Frequency Oscillator. 
//
//-----------------------------------------------------------------------------
void RTC_Init (void)
{

    uint8_t CLKSEL_SAVE = CLKSEL;   

    RTC0KEY = 0xA5;                     // Unlock the smaRTClock interface
    RTC0KEY = 0xF1;

    RTC_Write (RTC0XCN, 0x08);          // Using the Low Frequency Oscillator
    RTC_Write (RTC0CN, 0x80);           // Enable smaRTClock oscillator
      
    //----------------------------------
    // Wait for smaRTClock to start
    //----------------------------------
    CLKSEL    =  0x74;                  // Switch to 156 kHz low power
                                        // internal oscillator
   
    PMU0CF = 0x20;                      // Clear PMU wake-up source flags

    CLKSEL = CLKSEL_SAVE;               // Restore system clock
    while(!(CLKSEL & 0x80));            // Poll CLKRDY
  
    RSTSRC = 0x06;                      // Enable missing clock detector and
                                        // leave VDD Monitor enabled.
   
    CLKSEL &= ~0x70;                    // Specify a clock divide value of 1
   
    while(!(CLKSEL & 0x80));            // Wait for CLKRDY to ensure the 
                                        // divide by 1 has been applied
      
    CLKSEL = 0x03;                      // Select smaRTClock oscillator 
                                        // as the system clock
    OSCICN &= ~0x80;                    // Disable internal oscillator
    
    FLSCL = 0x00;
    RTC0KEY = 0x55;                     // lock the smaRTClock interface;
    
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
