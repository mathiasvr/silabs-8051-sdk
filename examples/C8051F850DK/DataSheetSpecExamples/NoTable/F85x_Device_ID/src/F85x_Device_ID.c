//-----------------------------------------------------------------------------
// F85x_Device_ID.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
//
// Refer to F85x_Device_ID_Readme.txt for more information regarding the
// functionality of this code example.
//
//
// How To Test:
// ------------
// 1) Ensure a shorting block is placed across Imeasure (JP2).
// 2) Place VDD Select (SW1) in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 3) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Stop the program and locate the debug current instruction pointer. The
//    pointer will be pointing to a while(1) loop. Verify the device ID and
//    derivative ID for the device under test match the switch case the while(1)
//    loop is within.
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
//    - 17 JUL 2013
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

// Device ID for C8051F85x/86x devices
#define C8051F85x86x    0x30

// Derivative ID for C8051F85x/86x product family by device
#define C8051F850_GU    0xD0
#define C8051F851_GU    0xD1
#define C8051F852_GU    0xD2
#define C8051F853_GU    0xD3
#define C8051F854_GU    0xD4
#define C8051F855_GU    0xD5
#define C8051F850_GM    0xF0
#define C8051F851_GM    0xF1
#define C8051F852_GM    0xF2
#define C8051F853_GM    0xF3
#define C8051F854_GM    0xF4
#define C8051F855_GM    0xF5
#define C8051F860_GS    0xE0
#define C8051F861_GS    0xE1
#define C8051F862_GS    0xE2
#define C8051F863_GS    0xE3
#define C8051F864_GS    0xE4
#define C8051F865_GS    0xE5

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
   WDTCN = 0xDE;
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   // Disable the watchdog timer

   // The device is in the C8051F85x/86x family
   if (DEVICEID == C8051F85x86x)
   {
      // The user can place code relating to the C8051F85x/86x product family
      // here. All C8051F85x/86x family members have the following features:
      // --------------------------------------------------------------------
      // - CIP51 Core running up to 25 MHz
      // - Two Internal Oscillators (24.5 MHz and 80 kHz)
      // - I2C/SMBus
      // - SPI
      // - UART
      // - 3-Channel Programmable Counter Array (PWM, Clock Generation,
      //   Capture/Compare)
      // - 4 16-bit Timers
      // - 2 Comparators
      // - 16-bit CRC Unit

      // Select device by derivative ID
      switch (DERIVID)
      {
         // The user can place code relating to a specific family member within
         // the C8051F85x/86x product family here in the following case
         // statements contain comments detailing product specific features.

         case C8051F850_GU:

            // - 8 kB Flash Memory
            // - 512 B RAM
            // - 18 Digital Port I/Os
            // - 16 ADC0 Channels
            // - 16 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F851_GU:

            // - 4 kB Flash Memory
            // - 512 B RAM
            // - 18 Digital Port I/Os
            // - 16 ADC0 Channels
            // - 16 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F852_GU:

            // - 2 kB Flash Memory
            // - 256 B RAM
            // - 18 Digital Port I/Os
            // - 16 ADC0 Channels
            // - 16 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F853_GU:

            // - 8 kB Flash Memory
            // - 512 B RAM
            // - 18 Digital Port I/Os
            // - 16 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F854_GU:

            // - 4 kB Flash Memory
            // - 512 B RAM
            // - 18 Digital Port I/Os
            // - 16 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F855_GU:

            // - 2 kB Flash Memory
            // - 256 B RAM
            // - 18 Digital Port I/Os
            // - 16 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F850_GM:

            // - 8 kB Flash Memory
            // - 512 B RAM
            // - 16 Digital Port I/Os
            // - 15 ADC0 Channels
            // - 15 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F851_GM:

            // - 4 kB Flash Memory
            // - 512 B RAM
            // - 16 Digital Port I/Os
            // - 15 ADC0 Channels
            // - 15 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F852_GM:

            // - 2 kB Flash Memory
            // - 256 B RAM
            // - 16 Digital Port I/Os
            // - 15 ADC0 Channels
            // - 15 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F853_GM:

            // - 8 kB Flash Memory
            // - 512 B RAM
            // - 16 Digital Port I/Os
            // - 15 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F854_GM:

            // - 4 kB Flash Memory
            // - 512 B RAM
            // - 16 Digital Port I/Os
            // - 15 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F855_GM:

            // - 2 kB Flash Memory
            // - 256 B RAM
            // - 16 Digital Port I/Os
            // - 15 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F860_GS:

            // - 8 kB Flash Memory
            // - 512 B RAM
            // - 13 Digital Port I/Os
            // - 12 ADC0 Channels
            // - 12 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F861_GS:

            // - 4 kB Flash Memory
            // - 512 B RAM
            // - 13 Digital Port I/Os
            // - 12 ADC0 Channels
            // - 12 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F862_GS:

            // - 2 kB Flash Memory
            // - 256 B RAM
            // - 13 Digital Port I/Os
            // - 12 ADC0 Channels
            // - 12 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F863_GS:

            // - 8 kB Flash Memory
            // - 512 B RAM
            // - 13 Digital Port I/Os
            // - 12 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F864_GS:

            // - 4 kB Flash Memory
            // - 512 B RAM
            // - 13 Digital Port I/Os
            // - 12 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         case C8051F865_GS:

            // - 2 kB Flash Memory
            // - 256 B RAM
            // - 13 Digital Port I/Os
            // - 12 I/O with Comparator 0/1 Inputs

            while (1);

            // NOT REACHED
            break;

         default:
            // The derivative ID is not recognized.
            while(1);
      }
   }
   // The device is NOT in the C8051F85x/86x family
   else
      while (1);
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
