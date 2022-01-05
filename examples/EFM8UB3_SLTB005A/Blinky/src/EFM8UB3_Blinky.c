//-----------------------------------------------------------------------------
// EFM8UB3_Blinky.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program flashes the red/green/blue LED on the EFM8UB3 STK board about
// five times a second using the interrupt handler for Timer2.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 8
//   Timer2 - 5 Hz interrupt
//   P0.2   - BTN0 switch ('SW1' on BRD5002A schematic)
//   P1.4   - LED green
//   P1.5   - LED blue
//   P1.6   - LED red
//   P2.3   - Display enable
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB3 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8UB3 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB3 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The RGB LED should alternate colors at approximately 5 Hz.
// 6) When the BTN0 switch is pressed the device will enter bootloader mode.
//
// Target:         EFM8UB3
// Tool chain:     Generic
//
// Release 0.2
//    - Go to bootloader mode when BTN0 switch is pressed (BG)
//    - 13 MAR 2018
//
// Release 0.1 (MW, ST)
//    - Initial Revision
//    - 10 OCT 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB3_Register_Enums.h>
#include <InitDevice.h>

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT(BTN0, SFR_P0, 2);	// BTN0 switch ('SW1' on BRD5002A schematic)

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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();

   IE_EA = 1;                          // Enable global interrupts

   while (1)	// Spin forever
   {
       // Start the bootloader if BTN0 switch ('SW1' on schematic) is pressed
       if (BTN0 == 0)
       {
         // Start the bootloader by setting data[0] = bootloader signature and
         // soft resetting the device
         *((uint8_t SI_SEG_DATA *)0x00) = 0xA5;
         RSTSRC = RSTSRC_SWRSF__SET | RSTSRC_PORSF__SET;
       }
   }
}
