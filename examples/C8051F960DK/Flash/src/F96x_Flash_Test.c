//-----------------------------------------------------------------------------
// F96x_Flash_Test.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program tests all the functions available in F96x_FlashPrimitives.c
// and F96x_FlashUtils.c.  It uses a test data array to mimic the changes
// made to the Flash and test for correctness.  While the test is running, the
// LED is on.  If the test fails, the LED will turn off.  If the test passes,
// the LED will blink.
//
// Pinout:
//
//    P0.0 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// Option 1:
// 1) Load the F96x_Flash_test.c, F96x_FlashPrimitives.c, and F96x_FlashUtils.c
//    files in the Silicon Labs IDE.
// 2) Compile and download code to a 'F96x device.
// 3) Verify the LED pin of J13 are populated on the 'F96x TB.
// 4) Run the code
// 5) Check that the LED is blinking, indicating a passing test.
//
// Option 2:
// 1) Load the F96x_Flash_Test.c, F96x_FlashPrimitives.c, and F96x_FlashUtils.c
//    files in Simplicity Studio IDE.
// 2) Compile and download code to a UDP F96x MCU MUX LCD board.
// 3) Open the Code window in the IDE and verify the following data at
//    address <start_address>:
//
//        48 49 4A 44 45 46
//        47 00 48 49 4A 44
//        45 46 47 00 5A 5A
//        5A 5A 5A 5A 5A 5A
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil
// Command Line:   None
//
// Release 1.1 (ST)
//    - Removed references to scratchpad memory/SFLE bit
//    - Corrected FLASH_TEMP and FLASH_LAST addresses
//    - Added support for 64, 32 and 16 Kb flash sizes
//    - 26 AUG 2014
//
// Release 1.0 (ST)
//    - Initial Revision
//    - 25 AUG 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F960_Register_Enums.h>
#include "F96x_FlashPrimitives.h"
#include "F96x_FlashUtils.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P0, 0);                 // LED == 0 means ON

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define LED_ON      0
#define LED_OFF     1

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);           // Oscillator initialization routine
void VDDMon_Init (void);               // VDD Monitor initialization routine
void PORT_Init (void);                 // Port initialization routine

//-----------------------------------------------------------------------------
// File global variables
//-----------------------------------------------------------------------------

// This is the test address.  It is a 17-bit address that treats the 128K
// or 96K of code space as a linear space.
FLADDR start_address = 0x18FFE;        // Address located in Bank 3

SI_SEGMENT_VARIABLE(test_write_buff[8], uint8_t, SI_SEG_XDATA) = "ABCDEFG";
SI_SEGMENT_VARIABLE(test_write_buff2[3], uint8_t, SI_SEG_XDATA) = "HIJ";
SI_SEGMENT_VARIABLE(test_read_buff[8], uint8_t, SI_SEG_XDATA) = {0};
SI_SEGMENT_VARIABLE(test_compare_buff[8], uint8_t, SI_SEG_XDATA) = "ABCDEFG";

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
   PCA0MD &= ~0x40;                    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint8_t temp_byte = 0x00;
   uint8_t i, j;

   bit error_flag = 0;

   SFRPAGE = LEGACY_PAGE;              // Set for PCA0MD and RSTSRC


   if ((RSTSRC & 0x02) != 0x02)
   {
      if ((RSTSRC & 0x40) == 0x40)
      {
         LED = LED_OFF;
         while (1);                    // Last reset was caused by a Flash
                                       // Error Device Reset
                                       // LED is off and loop forever to
                                       // indicate error
      }
   }

   OSCILLATOR_Init ();                 // Initialize the internal oscillator
   VDDMon_Init ();                     // Initialize the VDD monitor
   PORT_Init ();                       // Initialize the crossbar

   LED = LED_ON;

   // Initially erase the test pages of Flash
   FLASH_PageErase (start_address);
   FLASH_PageErase (start_address + 2);

   //BEGIN TEST================================================================

   // Check if able to Write and Read the Flash--------------------------------
   FLASH_ByteWrite (start_address, 0xA5);

   temp_byte = FLASH_ByteRead (start_address);

   if (temp_byte != 0xA5)
   {
      error_flag = 1;
   }
   //--------------------------------------------------------------------------


   // Check if able to Erase a page of the Flash-------------------------------
   FLASH_PageErase (start_address);

   temp_byte = FLASH_ByteRead (start_address);

   if (temp_byte != 0xFF)
   {
      error_flag = 1;
   }
   //--------------------------------------------------------------------------

   // Check if able to write and read a series of bytes------------------------
   FLASH_Write (start_address, test_write_buff, sizeof(test_write_buff));

   FLASH_Read (test_read_buff, start_address, sizeof(test_write_buff));

   for (i = 0; i < sizeof(test_write_buff); i++)
   {
      if (test_read_buff[i] != test_write_buff[i])
      {
         error_flag = 1;
      }
   }
   //--------------------------------------------------------------------------

   // Check if able to Erase a few bytes---------------------------------------
   FLASH_Clear (start_address, 2);

   FLASH_Read (test_read_buff, start_address, sizeof(test_write_buff));

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
   FLASH_Update (start_address, test_write_buff2, 3);

   FLASH_Read (test_read_buff, start_address, sizeof(test_write_buff));

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
   FLASH_Copy (start_address + sizeof(test_write_buff), start_address,
               sizeof(test_write_buff));

   FLASH_Read (test_read_buff, start_address + sizeof(test_write_buff),
              sizeof(test_read_buff));

   for (i = 0; i < sizeof(test_write_buff); i++)
   {
      if (test_read_buff[i] != test_compare_buff[i])
      {
         error_flag = 1;
      }
   }
   //--------------------------------------------------------------------------

   // FLASH test routines------------------------------------------------------
   FLASH_Fill (start_address + sizeof(test_write_buff) * 2, 
               sizeof(test_write_buff), 0x5A);

   FLASH_Read(test_read_buff, start_address + sizeof(test_write_buff) * 2,
              sizeof(test_write_buff));

   for (i = 0; i < sizeof(test_write_buff); i++)
   {
      if (test_read_buff[i] != 0x5A)
      {
         error_flag = 1;
      }
   }
   //--------------------------------------------------------------------------

   //END OF TEST===============================================================

   while (1)                           // Loop forever
   {
      // Blink LED to indicate success
      if (error_flag == 0)
      {
         LED = !LED;

         // Simple delay loop
         for (i = 65000; i > 0; i--) 
         {
            for (j = 65000; j > 0; j--) 
            {
            } 
         }
      }
      else
      {
         LED = LED_OFF;
      }
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at its maximum frequency / 8.
//
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x84;                      // Configure internal oscillator for
                                       // 24 MHz / 8

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// VDDMon_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the VDD Monitor and enables it as a reset source.
// The VDD Monitor is enabled following a power-on reset, but this routine
// explicitly enables the VDD Monitor.
//
// Note: If the VDD Monitor is not enabled or not enabled as a reset source,
// any attempt to write or erase Flash will cause a Flash Error Device Reset.
//-----------------------------------------------------------------------------
void VDDMon_Init(void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   // Step 1. Enable the VDD Monitor and set it to the high threshold
   VDM0CN = 0xA0;

   // Step 2. Wait for the VDD Monitor to stabilize (approximately 5 µs).
   // Note: This delay should be omitted if software contains routines which
   // erase or write Flash memory.
   // Omitted in this code

   // Step 3. Select the VDD Monitor as a reset source (PORSF bit in
   // RSTSRC = ‘1’).

   RSTSRC = 0x02;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P0.0   digital   push-pull     LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P0MDIN |= 0x01;                     // P1.3 is digital
   P0MDOUT |= 0x01;                    // P1.3 is push-pull

   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
