//-----------------------------------------------------------------------------
// F93x_Port_Standard.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// This example demonstrates the Port I/O DC Electrical Characteristics
// found on table 4.3 of the datasheet.
//
// Refer to F93x_Port_Standard_Readme.txt for more information regarding the
// functionality of this code example.
//
//
// How To Measure (Table 4.3 - Port I/O):
// ---------------------------------------
//  1) Ensure a shorting block is placed across to J11(VBAT & WALL_PWR) header.
//  2) Place VDD Select (SW4) in the 2 CELL position and power the board
//     using the P2 power connector.
//  3) Download and run the code on the target board.
//  4) Remove all of the shorting blocks on the board except for the block(s)
//    across the following pins:
//     a) Imeasure on J17
//     b) LED1.5 and P1.5 on J8
//     c) LED1.6 and P1.6 on J8
//  5) Measure the Output High Voltage (Low Drive) - Table 4.3
//     a) Connect a 330k, 3.3k or 1k ohm resistor between P0.0 (J2) and GND.
//     b) Measure the voltage across P0.0 and GND. Ensure the voltage is
//        greater than the minimum value given for Output High Voltage (Low
//        Drive).
//  6) Measure the Output Low Voltage (Low Drive) - Table 4.3
//     a) Connect a 2.4k ,330k or 820 ohm resistor between P0.1 (J2) and VDD.
//     b) Measure the voltage across P0.1 and GND. Ensure the voltage is less
//        than the maximum value given for Output Low Voltage (Low Drive).
//  7) Measure the Output High Voltage (High Drive) - Table 4.3
//     a) Connect a 330k, 1.2k or 330 ohm resistor between P1.0 (J3) and GND.
//     b) Measure the voltage across P1.0 and GND. Ensure the voltage is
//        greater than the minimum value given for Output High Voltage (High
//        Drive).
//  8) Measure the Output Low Voltage (High Drive) - Table 4.3
//     a) Connect a 330k, 390 or 15 ohm resistor between P1.1 (J3) and VDD.
//     b) Measure the voltage across P1.1 and GND. Ensure the voltage is less
//        than the maximum value given for Output Low Voltage (High Drive).
//  9) Test the Input High Voltage - Table 4.3
//     a) Use a voltage supply to sweep the voltage at P1.4 from VDD to VDD-0.6.
//     b) Ensure LED1_5 remains ON and LED1_6 remains OFF for the duration of
//        the sweep.
// 10) Test the Input Low Voltage - Table 4.3
//     a) Use a voltage supply to sweep the voltage at P1.4 from GND to 0.6V.
//     b) Ensure LED1_5 remains OFF and LED1_6 remains ON for the duration of
//        the sweep.
//
//
// Target:         C8051F93x/92x
// Tool Chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (MT)
//    - 2 SEP 2013
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F930_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(LED1_5, SFR_P1, 5);
SI_SBIT(LED1_6, SFR_P1, 6);

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
    PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
                                        // enable)

   SYSCLK_Init ();                      // Initialize system clock to 24.5 MHz
   Port_Init ();                        // Initialize crossbar and GPIO

   // Loop forever
   while (1)
   {
      // If P1.4 is reading high
      if((P1 & 0x10) == 0x10)
      {
         LED1_5 = 0;                    // LED1.5 ON
         LED1_6 = 1;                    // LED1.6 OFF
      }
      // Otherwise, P1.4 is reading low
      else
      {
         LED1_5 = 1;                    // LED1.5 OFF
         LED1_6 = 0;                    // LED1.6 ON
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
    // Select Precision Internal Oscillator 24.5MHz as system
    // clock source
    REG0CN |= 0x10;
    OSCICN |= 0x80;	
    CLKSEL = 0x00;
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
// P0.0  digital  push-pull   output   Output High Voltage (Low Drive)
// P0.1  digital  push-pull   output   Output Low Voltage  (Low Drive)

// P1.0  digital  push-pull   output   Output High Voltage (High Drive)
// P1.1  digital  push-pull   output   Output Low Voltage  (High Drive)

// P1.4  digital  open-drain  input    Input High/Low Voltage

// P1.5  digital  push-pull   output   LED1.5
// P1.6  digital  push-pull   output   LED1.6
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
    // Port 0 initialization
    P0MDIN  |=  0x03;                   // Set P0.0 and P0.1 as digital
    P0MDOUT |=  0x03;                   // Set P0.0 and P0.1 as push-pull output
    P0      |=  0x01;                   // Set P0.0 latch to high
    P0      &= ~0x02;                   // Set P0.1 latch to low

    // Port 1 initialization
    P1MDIN  |=  0x73;                   // Set P1.0 P1.1 P1.4
                                        // P1.5 P1.6 as digital
    P1MDOUT |=  0x63;                   // Set P1.0 P1.1
                                        // P1.5 P1.6 as push-pull output
    P1MDOUT &= ~0x10;                   // Set P1.4 to open-drain input
    P1      |=  0x71;                   // Set P1.0 P1.4 P1.5 P1.6 latch to high
    P1      &= ~0x02;                   // Set P1.1 latch to low
    P1DRV   = 0x03;                     // P1.0 P1.1 high output drive strength
    XBR2    = 0x40;                     // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
