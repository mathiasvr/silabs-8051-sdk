//-----------------------------------------------------------------------------
// F93x_Flash.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program tests all the functions available in F93x_FlashPrimitives.c
// and F93x_FlashUtils.c.  It uses a test data array to mimic the changes
// made to the Flash and test for correctness.  While the test is running, the
// LED is on.  If the test fails, the LED will turn off.  If the test passes,
// the LED will blink.
//
// Pinout:
//
//    P1.6 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// Option 1:
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.6/LED1.6
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J9.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Check that the LED is blinking, indicating a passing test.
//
// Option 2:
// 1) Ensure that jumpers are placed on the following:
//       J8:   P1.6/LED1.6
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J9.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button
//    in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) View the CODE space by navigating to
//    "Window" -> "Show View" -> "Memory"
//    Add a memory monitor at the following address:
//    CODE <START_ADDRESS> (default 0x1000)
//    verify the following data:
//
//        48 49 4A 44 45 46
//        47 00 48 49 4A 44
//        45 46 47 00 5A 5A
//        5A 5A 5A 5A 5A 5A
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.0
//    -Initial Revision (TP)
//    -14 JAN 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include "F93x_FlashPrimitives.h"
#include "F93x_FlashUtils.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             20000000/8  // SYSCLK in Hz
#define START_ADDRESS          0x1000

SI_SBIT(LED, SFR_P1, 6);                 // LED

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Timer0_Delay_ms (uint16_t ms);

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
// ----------------------------------------------------------------------------
int main (void)
{
	uint8_t temp_byte = 0x00;
	FLADDR start_address = START_ADDRESS;

	char test_write_buff[8] = "ABCDEFG";
	char test_write_buff2[3] = "HIJ";
	char test_read_buff[8] = {0};
	char test_compare_buff[8] = "ABCDEFG";

	uint8_t i;

	bit error_flag = 0;

	//Enter default mode
	enter_DefaultMode_from_RESET();

	LED = 1;

	// Initially erase the test page of Flash
	FLASH_PageErase(start_address, 0);

	//BEGIN TEST================================================================

	// Check if able to Write and Read the Flash--------------------------------
	FLASH_ByteWrite(start_address, 0xA5, 0);

	temp_byte = FLASH_ByteRead(start_address, 0);

	if (temp_byte != 0xA5)
	{
	  error_flag = 1;
	}
	//--------------------------------------------------------------------------


	// Check if able to Erase a page of the Flash-------------------------------
	FLASH_PageErase(start_address, 0);

	temp_byte = FLASH_ByteRead(start_address, 0);

	if (temp_byte != 0xFF)
	{
	  error_flag = 1;
	}
	//--------------------------------------------------------------------------

	// Check if able to write and read a series of bytes------------------------
	FLASH_Write(start_address, test_write_buff, sizeof(test_write_buff), 0);

	FLASH_Read(test_read_buff, start_address, sizeof(test_write_buff), 0);

	for (i = 0; i < sizeof(test_write_buff); i++)
	{
	  if (test_read_buff[i] != test_write_buff[i])
	  {
		 error_flag = 1;
	  }
	}
	//--------------------------------------------------------------------------

	// Check if able to Erase a few bytes---------------------------------------
	FLASH_Clear(start_address, 2, 0);

	FLASH_Read(test_read_buff, start_address, sizeof(test_write_buff), 0);

	// Simulate the same changes to a data array for comparison
	test_compare_buff[0] = 0xFF;
	test_compare_buff[1] = 0xFF;

	for (i = 0; i < sizeof(test_compare_buff); i++)
	{
	  if (test_read_buff[i] != test_compare_buff[i])
	  {
		 error_flag = 1;
	  }
	}
	//--------------------------------------------------------------------------

	// Check if able to "update" (erase then re-write) a few bytes--------------
	FLASH_Update (start_address, test_write_buff2, 3, 0);

	FLASH_Read(test_read_buff, start_address, sizeof(test_write_buff), 0);

	// Simulate the same changes to a data array for comparison
	test_compare_buff[0] = test_write_buff2[0];
	test_compare_buff[1] = test_write_buff2[1];
	test_compare_buff[2] = test_write_buff2[2];

	for (i = 0; i < sizeof(test_compare_buff); i++)
	{
	  if (test_read_buff[i] != test_compare_buff[i])
	  {
		 error_flag = 1;
	  }
	}
	//--------------------------------------------------------------------------

	// Check if able to copy data in the Flash----------------------------------
	FLASH_Copy (start_address+sizeof(test_write_buff), 0, start_address, 0,
			   sizeof(test_write_buff));

	FLASH_Read(test_read_buff, start_address+sizeof(test_write_buff),
			  sizeof(test_read_buff), 0);

	for (i = 0; i < sizeof(test_write_buff); i++)
	{
	  if (test_read_buff[i] != test_compare_buff[i])
	  {
		 error_flag = 1;
	  }
	}
	//--------------------------------------------------------------------------

	// FLASH test routines------------------------------------------------------
	FLASH_Fill (start_address+sizeof(test_write_buff)*2, sizeof(test_write_buff),
			   0x5A, 0);

	FLASH_Read(test_read_buff, start_address+sizeof(test_write_buff)*2,
			  sizeof(test_write_buff), 0);

	for (i = 0; i < sizeof(test_write_buff); i++)
	{
	  if (test_read_buff[i] != 0x5A)
	  {
		 error_flag = 1;
	  }
	}
	//--------------------------------------------------------------------------

	// FLASH test scratchpad----------------------------------------------------
	FLASH_ByteWrite(0x0000, 0xA5, 1);

	temp_byte = FLASH_ByteRead(0x0000, 1);

	if (temp_byte != 0xA5)
	{
	  error_flag = 1;
	}
	//--------------------------------------------------------------------------

	//END OF TEST===============================================================

	while (1)                           // Loop forever
	{
	  // Blink LED to indicate success
	  if (error_flag == 0)
	  {
		 LED = !LED;

		 Timer0_Delay_ms (100);
	  }
	  else
	  {
		 LED = 0;
	  }
	}

	// NOTREACHED
	return 0;
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_Delay_ms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) uint16_t ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <ms> milliseconds before returning.
//
// Note: This routine modifies the SCA0-1 bits in CKCON, which will affect
// the timing of Timer1.
//
//-----------------------------------------------------------------------------
void Timer0_Delay_ms (uint16_t ms)
{
   uint16_t i;                         // Millisecond counter

   TCON  &= ~0x30;                     // STOP Timer0 and clear overflow flag

   for (i = 0; i < ms; i++)            // Count milliseconds
   {
      TCON &= ~0x10;                   // STOP Timer0
      TH0 = (-SYSCLK/48/1000) >> 8;    // Set Timer0 to overflow in 1ms
      TL0 = -SYSCLK/48/1000;
      TCON |= 0x10;                    // START Timer0
      while (TCON_TF0 == 0);                // Wait for overflow
      TCON &= ~0x20;                   // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
