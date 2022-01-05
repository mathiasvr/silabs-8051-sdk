//-----------------------------------------------------------------------------
// F85x_SPI0_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program configures a SPI Slave. The MCU is configured in 4-wire Slave
// Mode.
//
// This example is intended to be used with the SPI0_Master example with a 
// maximum SPI_CLOCK frequency of 1 MHz.
//
// Pinout:
//
// P0.0 - SPI SCK    (digital input, open-drain)
// P0.1 - SPI MISO   (digital output, push-pull)
// P0.2 - SPI MOSI   (digital input, open-drain)
// P0.3 - SPI NSS    (digital input, open-drain)
//
// P1.0 - LED1       (digital output, push-pull)
//
// all other port pins unused.
//
//-----------------------------------------------------------------------------
// How To Test: UPMU-F850
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) Ensure that J32 is not populated.
// 3) Connect the device to a SPI master running the corresponding
//    SPI_Master code with SPI_CLOCK set to 1 MHz or lower.
// 4) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 5) Compile and download code to the 'F85x UDP MCU board by selecting 
//		Run -> Debug from the menus, clicking the Debug button in the quick menu,
//		or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the 
//		Resume button in the quick menu, or pressing F8.
//    If the communication passes, the LEDs on both the Master
//    and Slave boards will blink slowly. If it fails, the LEDs will be off.
//
//
//-----------------------------------------------------------------------------
// How To Test: ToolStick F85x
//-----------------------------------------------------------------------------
// 1) Ensure shorting blocks are place on the following:
//    - JP1
// 2) Connect the device to a SPI master running the corresponding
//    SPI_Master code with SPI_CLOCK set to 1 MHz or lower.
// 3) Connect the ToolStick F85x board to a PC using a Silicon Labs ToolStick
//    Base Adapter.
// 4) Compile and download code to the ToolStick F85x board by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//    If the communication passes, the LEDs on both the Master
//    and Slave boards will blink slowly. If it fails, the LEDs will be off.
//
// Target:         C8051F85x/86x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.3 (ST)
//	  - Updated to use Simplicity Configurator initializations
//    - 30 JAN 2014
//
// Release 1.2 (BL)
//	  - Updated How To Test with Studio specific instructions
//	  - 3 JAN 2014
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    - Initial Revision (TP)
//    - 26 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>
#include "InitDevice.h"

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
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   enter_DefaultMode_from_RESET();

   IE_EA = 1;                          // Enable global interrupts

   while (1);                          // Loop forever
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
