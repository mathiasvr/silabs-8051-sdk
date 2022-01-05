//-----------------------------------------------------------------------------
// EFM8BB51_Flash.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program tests all the functions available in EFM8BB51_FlashPrimitives.c
// and EFM8BB51_FlashUtils.c.  It uses a test data array to mimic the changes
// made to the flash and test for correctness.  While the test is running, the
// LED is on.  If the test fails, the LED will turn off.  If the test passes,
// the LED will blink.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC / 8
//   Timer0 - LED blink
//   P1.5   - LED0
//   P2.0   - Display enable, shared by C2D pin
//            See AN127 for C2D pin sharing information
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB51 PK
//-----------------------------------------------------------------------------
// Option 1:
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB51 PK board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB51 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Check that the LED is blinking, indicating a passing test.
//
// Option 2:
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB51 PK board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB51 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) View the CODE space by navigating to
//		"Window" -> "Show View" -> "Memory Browser"
//		and verify the following data at START_ADDRESS (default 0x1000):
//
//        48 49 4A 44 45 46
//        47 00 48 49 4A 44
//        45 46 47 00 5A 5A
//        5A 5A 5A 5A 5A 5A
//
// Note: This example is written for EFM8BB51 devices with 16KB flash. To port
//       this example to devices with smaller flash sizes, modify the
//       START_ADDRESS definition in this file, and the FLASH_TEMP and
//       FLASH_LAST definitions in EFM8BB51_FlashPrimitives.h
//
// Target:         EFM8BB51
// Tool chain:     Generic
//
// Release 0.1 (DS)
//    - Initial Revision
//    - 02 FEB 2021
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB51_Register_Enums.h>
#include "InitDevice.h"
#include "EFM8BB51_FlashPrimitives.h"
#include "EFM8BB51_FlashUtils.h"

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable, shared by C2D pin
                                       // See AN127 for C2D pin sharing info
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (LED, SFR_P1, 5);              // LED0
#define LED_ON   1
#define LED_OFF  0

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK             24500000/8  // SYSCLK in Hz
#define START_ADDRESS          0x800

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(flash_write_array[512], uint8_t, const SI_SEG_CODE, START_ADDRESS);

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
//-----------------------------------------------------------------------------
void main (void)
{
  uint8_t temp_byte = 0x00;
  uint8_t test_write_buff[8] = "ABCDEFG";
  uint8_t test_write_buff2[3] = "HIJ";
  uint8_t test_read_buff[8] = {0};
  uint8_t test_compare_buff[8] = "ABCDEFG";
  uint8_t i;
  bool error_flag = 0;

  enter_DefaultMode_from_RESET();

  DISP_EN = DISP_BC_DRIVEN;

  LED = LED_ON;

  SFRPAGE = 0x01;

  SFRPAGE = 0x00;

  // Initially erase the test page of flash
  FLASH_PageErase(START_ADDRESS);

  // BEGIN TEST===============================================================

  // Check if able to Write and Read the flash--------------------------------
  FLASH_ByteWrite(START_ADDRESS, 0xA5);

  temp_byte = FLASH_ByteRead(START_ADDRESS);

  if (temp_byte != 0xA5)
  {
    error_flag = 1;
  }

  // Check if able to Erase a page of the flash-------------------------------
  FLASH_PageErase(START_ADDRESS);

  temp_byte = FLASH_ByteRead(START_ADDRESS);

  if (temp_byte != 0xFF)
  {
    error_flag = 1;
  }

  // Check if able to write and read a series of bytes------------------------
  FLASH_Write(START_ADDRESS, test_write_buff, sizeof(test_write_buff));

  FLASH_Read(test_read_buff, START_ADDRESS, sizeof(test_write_buff));

  for (i = 0; i < sizeof(test_write_buff); i++)
  {
    if (test_read_buff[i] != test_write_buff[i])
    {
      error_flag = 1;
    }
  }

  // Check if able to Erase a few bytes---------------------------------------
  FLASH_Clear(START_ADDRESS, 2);

  FLASH_Read(test_read_buff, START_ADDRESS, sizeof(test_write_buff));

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

  // Check if able to "update" (erase then re-write) a few bytes--------------
  FLASH_Update (START_ADDRESS, test_write_buff2, 3);

  FLASH_Read(test_read_buff, START_ADDRESS, sizeof(test_write_buff));

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

  // Check if able to copy data in the flash----------------------------------
  FLASH_Copy (START_ADDRESS+sizeof(test_write_buff), START_ADDRESS,
             sizeof(test_write_buff));

  FLASH_Read(test_read_buff, START_ADDRESS+sizeof(test_write_buff),
            sizeof(test_read_buff));

  for (i = 0; i < sizeof(test_write_buff); i++)
  {
    if (test_read_buff[i] != test_compare_buff[i])
    {
      error_flag = 1;
    }
  }

  // Flash test routines------------------------------------------------------
  FLASH_Fill (START_ADDRESS+sizeof(test_write_buff)*2, sizeof(test_write_buff),
             0x5A);

  FLASH_Read(test_read_buff, START_ADDRESS+sizeof(test_write_buff)*2,
            sizeof(test_write_buff));

  for (i = 0; i < sizeof(test_write_buff); i++)
  {
    if (test_read_buff[i] != 0x5A)
    {
      error_flag = 1;
    }
  }

  // END OF TEST==============================================================

  while (1) // Loop forever
  {
    // Blink LED to indicate success
    if (error_flag == 0)
    {
      LED = !LED;
      Timer0_Delay_ms (100);
    }
    else
    {
      LED = LED_OFF;
    }
  }
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

   TCON  &= ~TCON_TF0__BMASK;          // Clear Timer0 overflow flag
   for (i = 0; i < ms; i++)            // Count milliseconds
   {
      TCON &= ~0x10;                   // STOP Timer0
      TH0 = (-SYSCLK/48/1000) >> 8;    // Set Timer0 to overflow in 1ms
      TL0 = (uint8_t)-SYSCLK/48/1000;
      TCON |= TCON_TR0__BMASK;         // START Timer0
      while (TCON_TF0 == 0);           // Wait for overflow
      TCON &= ~TCON_TF0__BMASK;        // Clear overflow indicator
   }
}
