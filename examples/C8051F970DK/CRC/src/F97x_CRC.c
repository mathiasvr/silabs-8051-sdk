//-----------------------------------------------------------------------------
// F97x_CRC.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This firmware calculates a CRC on data with a software-implemented CRC and
// the C8051F970's hardware-implemented CRC.  It measures the cycles spent
// performing both tasks and outputs this information through the UART.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP1: VCP RX <> P0.1/UART0.TX
//    - JP2: VCP TX <> P0.2/UART0.RX
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Connect a USB cable from J9 of the development board to a PC,
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the virtual COM port at <BAUDRATE> (115200) and 8-N-1.
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) When LED05 is solid, the program has completed.
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 30 APR 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
SI_SBIT(LED, SFR_P0, 5);                 // LED=0 means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
uint16_t UpdateCRC(uint16_t CRC_acc, uint8_t CRC_input);

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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint16_t CRC_Val;                         // Stores software-implemented CRC
   uint16_t SoftwareCRC_NumCycles;
   uint16_t HardwareCRC_NumCycles;

   SI_UU16_t CRCHW;                          // Saves hardware-generated CRC

   enter_DefaultMode_from_RESET();

   SCON0_TI = 1; // enable printf

   LED = 0;                             // Turn off LED


   SFRPAGE = TIMER2_PAGE;
   TMR2CN_TR2 = 1;                      // Enable timer 2

   // First, generate a 16-bit CRC in software, using a sequence
   // of characters as inputs, and save the number of cycles used
   // to perform the calculations
   CRC_Val = 0xFFFF;

   CRC_Val = UpdateCRC(CRC_Val, '1');
   CRC_Val = UpdateCRC(CRC_Val, '2');
   CRC_Val = UpdateCRC(CRC_Val, '3');
   CRC_Val = UpdateCRC(CRC_Val, '4');
   CRC_Val = UpdateCRC(CRC_Val, '5');
   CRC_Val = UpdateCRC(CRC_Val, '6');
   CRC_Val = UpdateCRC(CRC_Val, '7');
   CRC_Val = UpdateCRC(CRC_Val, '8');
   CRC_Val = UpdateCRC(CRC_Val, '9');

   SFRPAGE = TIMER2_PAGE;
   TMR2CN_TR2 = 0;                      // Disable timer 2

   SoftwareCRC_NumCycles = TMR2;        // Save number of cycles

   TMR2 = 0;                            // Clear timer

   TMR2CN_TR2 = 1;                      // Enable timer 2


   // Now use the C8051F750's CRC to generate the same CRC and save
   // the number of cycles used to perform the calculations
   SFRPAGE = CRC0_PAGE;

   //CRC0CN |= 0x00;                     // 32-bit CRC
   CRC0CN |= CRC0CN_CRCVAL__SET_ONES;    // Initializes CRC to 0xFFFF
   CRC0CN |= CRC0CN_CRCINIT__INIT;       // Initializes CRC result

   CRC0IN = '1';                         // Write values into the CRC
   CRC0IN = '2';
   CRC0IN = '3';
   CRC0IN = '4';
   CRC0IN = '5';
   CRC0IN = '6';
   CRC0IN = '7';
   CRC0IN = '8';
   CRC0IN = '9';

   CRC0CN = CRC0CN_CRCPNT__ACCESS_UPPER; // Return bits 15-8 of the 16-bit CRC result
   CRCHW.u8[0] = CRC0DAT;                // Read CRC value from hardware

   CRC0CN = CRC0CN_CRCPNT__ACCESS_LOWER; // Return bits 7-0 of the 16-bit CRC result
   CRCHW.u8[1] = CRC0DAT;                // Read CRC value from hardware


   SFRPAGE = TIMER2_PAGE;
   TMR2CN_TR2 = 0;                       // Disable timer 2


   HardwareCRC_NumCycles = TMR2;         // Save number of cycles

   // Output results
   printf("\nCycles used for software-generated CRC: %d",SoftwareCRC_NumCycles);
   printf("\nSoftware-generated CRC: %x", CRC_Val);
   printf("\nCycles used for hardware-generated CRC: %d",HardwareCRC_NumCycles);
   printf("\nHardware-generated CRC: %x\n", CRCHW.u16);

   LED = 0;                              // Turn on LED

   while (1);                            // Spin forever
}


uint16_t UpdateCRC (uint16_t CRC_acc, uint8_t CRC_input)
{
   uint8_t i;

   #define POLY 0x1021      // bit-reversed version of the poly 0x1021

   // Create the CRC "dividend" for polynomial arithmetic (binary arithmetic
   // with no carries)
   CRC_acc = CRC_acc ^ (CRC_input << 8);

   // "Divide" the poly into the dividend using CRC XOR subtraction
   // CRC_acc holds the "remainder" of each divide
   //
   // Only complete this division for 8 bits since input is 1 byte
   for (i = 0; i < 8; i++)
   {
      // Check if the MSB is set (if MSB is 1, then the POLY can "divide"
      // into the "dividend")
      if ((CRC_acc & 0x8000) == 0x8000)
      {
         // if so, shift the CRC value, and XOR "subtract" the poly
         CRC_acc = CRC_acc << 1;
         CRC_acc ^= POLY;
      }
      else
      {
         // if not, just shift the CRC value
         CRC_acc = CRC_acc << 1;
      }
   }

   // Return the final remainder (CRC value)
   return CRC_acc;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
