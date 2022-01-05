//-----------------------------------------------------------------------------
// F91x_SmaRTClock.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the consistency of the SmaRTClock Oscillator
// freqency through the full voltage supply range (1.8 V to 3.6 V).
// This feature only available on F912 and F902 devices. The clock frequency
// can be measured through P0.0.
//
// Refer to F91x_SmaRTClock_Readme.txt for more information regarding this
// code example's functionality.
//
// How to Measure (Table 4.9 - SmaRTClock Frequency):
// -------------------------------------------------
// 1) Ensure the shorting block is attached to the J11 (VBAT & WALL_PWR) 
//    header.
// 2) Remove all other shorting blocks and select 2 Cell (3.3v) on SW4
// 3) Connect a DC Power Supply to VDD (J2) and GND.
// 4) Connect an oscilloscope probe to P0.0 and GND
// 5) Compile and download the code to target board.
// 6) Run the code.
// 7) Measure the frequency output on P0.0 while changing the output from
//    1.8 V to 3.6 V.
//
// Target:         C8051F91x/90x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.1 (JL)
//    - Updated description.
//    - 18 Feb 2014
//
// Release 1.0
//    - Initial Revision (MT)
//    - 23 OCT 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

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

    // Selsct  SmaRTClock as system clock source.
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
// Parameters   : 1) unsigned char reg - address of RTC register to write
//                2) unsigned char value - the value to write to <reg>
//
// This function will Write one byte to the specified RTC register.
// Using a register number greater that 0x0F is not permitted.
//-----------------------------------------------------------------------------
void RTC_Write (unsigned char reg, unsigned char value)
{
   reg &= 0x0F;                         // mask low nibble
   RTC0ADR  = reg;                      // pick register
   RTC0DAT = value;                     // write data
   while ((RTC0ADR & 0x80) == 0x80);    // poll on the BUSY bit
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

    unsigned char CLKSEL_SAVE = CLKSEL;   


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
