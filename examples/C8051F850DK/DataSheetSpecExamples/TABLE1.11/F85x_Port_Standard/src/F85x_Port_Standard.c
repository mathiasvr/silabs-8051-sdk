//-----------------------------------------------------------------------------
// F85x_Port_Standard.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// Refer to F85x_Port_Standard_Readme.txt for more information regarding the
// functionality of this code example.
//
//
// How To Measure (Table 1.11 - Port I/O):
// ---------------------------------------
//  1) Ensure a shorting block is placed across Imeasure (JP2).
//  2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//     using the J6 power connector.
//  3) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//     development board by selecting Run -> Debug from the menus, clicking the
//     Debug button in the quick menu, or pressing F11.
//  4) Run the code by selecting Run -> Resume from the menus, clicking the
//     Resume button in the quick menu, or pressing F8.
//  5) Remove all of the shorting blocks on the board except for the block(s)
//     across the following pins:
//     a) Imeasure on JP2
//     b) DS5 and P1.0 on J27
//     c) DS6 and P1.1 on J27
//  6) Measure the Output High Voltage (Low Drive) - Table 1.11
//     a) Connect a 3.3k ohm resistor between P0.0 (J24) and GND.
//     b) Measure the voltage across P0.0 and GND. Ensure the voltage is
//        greater than the minimum value given for Output High Voltage (Low
//        Drive).
//  7) Measure the Output Low Voltage (Low Drive) - Table 1.11
//     a) Connect a 2.4k ohm resistor between P0.1 (J24) and VDD.
//     b) Measure the voltage across P0.1 and GND. Ensure the voltage is less
//        than the maximum value given for Output Low Voltage (Low Drive).
//  8) Measure the Output High Voltage (High Drive) - Table 1.11
//     a) Connect a 1.1k ohm resistor between P1.2 (J25) and GND.
//     b) Measure the voltage across P1.2 and GND. Ensure the voltage is
//        greater than the minimum value given for Output High Voltage (High
//        Drive).
//  9) Measure the Output Low Voltage (High Drive) - Table 1.11
//     a) Connect a 390 ohm resistor between P1.3 (J25) and VDD.
//     b) Measure the voltage across P1.3 and GND. Ensure the voltage is less
//      than the maximum value given for Output Low Voltage (High Drive).
//  10) Test the Input High Voltage - Table 1.11
//     a) Use a voltage supply to sweep the voltage at P1.4 from VDD to VDD-0.6.
//     b) Ensure DS5 remains ON and DS6 remains OFF for the duration of the
//        sweep.
// 11) Test the Input Low Voltage - Table 1.11
//     a) Use a voltage supply to sweep the voltage at P1.4 from GND to 0.6V.
//     b) Ensure DS5 remains OFF and DS6 remains ON for the duration of the
//        sweep.
//
//
// Target:         C8051F85x
// Target Board:   UPMU-F850 MCU-100
// Tool Chain:     Generic
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 5 FEB 2014
//
// Release 1.0
//    - Initial Revision (BGD)
//    - 16 JUL 2013
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK    24500000             // SYSCLK frequency in Hz

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SI_SBIT(DS5, SFR_P1, 0);
SI_SBIT(DS6, SFR_P1, 1);

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
   WDTCN = 0xDE;
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   // Disable the watchdog timer

   SYSCLK_Init ();                     // Initialize system clock to 24.5 MHz
   Port_Init ();                       // Initialize crossbar and GPIO

   // Loop forever
   while (1)
   {
      // If P1.4 is reading high
      if((P1 & 0x10) == 0x10)
      {
         DS5 = 0;                      // DS5 ON
         DS6 = 1;                      // DS6 OFF
      }
      // Otherwise, P1.4 is reading low
      else
      {
         DS5 = 1;                      // DS5 OFF
         DS6 = 0;                      // DS6 ON
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
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled

   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz
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
// P0.0  digital  push-pull   Output High Voltage (Low Drive)
// P0.1  digital  push-pull   Output Low Voltage  (Low Drive)
// P1.0  digital  push-pull   DS5
// P1.1  digital  push-pull   DS6
// P1.2  digital  push-pull   Output High Voltage (High Drive)
// P1.3  digital  push-pull   Output Low Voltage  (High Drive)
// P1.4  digital  open-drain  Input High/Low Voltage
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   // Port 0 initialization
   P0MDIN  |=  0x03;                   // Set P0.0 and P0.1 as digital
   P0MDOUT |=  0x03;                   // Set P0.0 and P0.1 as push-pull output
   P0      |=  0x01;                   // Set P0.0 latch to high
   P0      &= ~0x02;                   // Set P0.1 latch to low
   PRTDRV  &= ~0x01;                   // Set Port 0 to use low drive strength

   // Port 1 initialization
   P1MDIN  |=  0x1F;                   // Set P1.0 to P1.4 as digital
   P1MDOUT |=  0x0F;                   // Set P1.0 to P1.3 as push-pull output
   P1MDOUT &= ~0x10;                   // Set P1.4 to open-drain input
   P1      |=  0x14;                   // Set P1.2 and P1.4 latch to high
   P1      &= ~0x08;                   // Set P1.3 latch to low

   XBR2     = 0x40;                    // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
