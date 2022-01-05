//-----------------------------------------------------------------------------
// F38x_Flash_Test.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program tests all the functions available in F380_FlashPrimitives.c
// and F380_FlashUtils.c.  It uses a test data array to mimic the changes
// made to the Flash and test for correctness.  While the test is running, the
// LED is on.  If the test fails, the LED will turn off.  If the test passes,
// the LED will blink.
//
// Pinout:
//
//    P2.2 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// Option 1:
// 1) Load the F38x_Flash.c, F380_FlashPrimitives.c, and F380_FlashUtils.c
//    files in the Silicon Labs IDE.
// 2) Compile and download code to a 'F38x device.
// 3) Verify the LED pins of J12 are populated on the 'F38x TB.
// 4) Run the code
// 5) Check that the LED is blinking, indicating a passing test.
//
// Option 2:
// 1) Load the F38x_Flash.c, F380_FlashPrimitives.c, and F380_FlashUtils.c
//    files in the Silicon Labs IDE.
// 2) Compile and download code to a 'F38x device.
// 3) Open the Code window in the IDE and verify the following data at
//    address <start_address>:
//
//        48 49 4A 44 45 46
//        47 00 48 49 4A 44
//        45 46 47 00 5A 5A
//        5A 5A 5A 5A 5A 5A
//
// Target:         C8051F38x
// Tool chain:     KEIL C51
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (ST)
//    -06 AUG 2014
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F380_Register_Enums.h>
#include "F380_FlashPrimitives.h"
#include "F380_FlashUtils.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK               12000000  // SYSCLK in Hz (48 MHz internal
                                       // oscillator / 4)
                                       // the internal oscillator has a
                                       // tolerance of +/- 2%

sbit LED = P2^2;                       // LED

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Oscillator initialization routine
void VDDMon_Init (void);               // VDD Monitor initialization routine
void Port_Init (void);                 // Port initialization routine

void Timer0_Delay_ms (unsigned int ms);

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
   unsigned char temp_byte = 0x00;
   FLADDR start_address = 0x5FFE;

   char test_write_buff[8] = "ABCDEFG";
   char test_write_buff2[3] = "HIJ";
   char test_read_buff[8] = {0};
   char test_compare_buff[8] = "ABCDEFG";

   unsigned char i;

   bit error_flag = 0;

                                       // enable)

   if ((RSTSRC & 0x02) != 0x02)
   {
      if ((RSTSRC & 0x40) == 0x40)
      {
         LED = 0;
         while(1);                     // Last reset was caused by a Flash
                                       // Error Device Reset
                                       // LED is off and loop forever to
                                       // indicate error
      }
   }

   Oscillator_Init();                  // Initialize the internal oscillator
                                       // to 12 MHz

   VDDMon_Init();

   Port_Init();

   LED = 1;

   SFRPAGE = LEGACY_PAGE;

   // Initially erase the test page of Flash
   FLASH_PageErase(start_address);

   //BEGIN TEST================================================================

   // Check if able to Write and Read the Flash--------------------------------
   FLASH_ByteWrite(start_address, 0xA5);

   temp_byte = FLASH_ByteRead(start_address);

   if (temp_byte != 0xA5)
   {
      error_flag = 1;
   }
   //--------------------------------------------------------------------------


   // Check if able to Erase a page of the Flash-------------------------------
   FLASH_PageErase(start_address);

   temp_byte = FLASH_ByteRead(start_address);

   if (temp_byte != 0xFF)
   {
      error_flag = 1;
   }
   //--------------------------------------------------------------------------

   // Check if able to write and read a series of bytes------------------------
   FLASH_Write(start_address, test_write_buff, sizeof(test_write_buff));

   FLASH_Read(test_read_buff, start_address, sizeof(test_write_buff));

   for (i = 0; i < sizeof(test_write_buff); i++)
   {
      if (test_read_buff[i] != test_write_buff[i])
      {
         error_flag = 1;
      }
   }
   //--------------------------------------------------------------------------

   // Check if able to Erase a few bytes---------------------------------------
   FLASH_Clear(start_address, 2);

   FLASH_Read(test_read_buff, start_address, sizeof(test_write_buff));

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

   FLASH_Read(test_read_buff, start_address, sizeof(test_write_buff));

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
   FLASH_Copy (start_address+sizeof(test_write_buff), start_address,
               sizeof(test_write_buff));

   FLASH_Read(test_read_buff, start_address+sizeof(test_write_buff),
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
   FLASH_Fill (start_address+sizeof(test_write_buff)*2, sizeof(test_write_buff),
               0x5A);

   FLASH_Read(test_read_buff, start_address+sizeof(test_write_buff)*2,
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
         LED = ~LED;

         Timer0_Delay_ms (100);
      }
      else
      {
         LED = 0;
      }
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the internal oscillator to 12 MHz.
//-----------------------------------------------------------------------------
void Oscillator_Init(void)
{
   unsigned char SFRPAGE_save = SFRPAGE; // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN |= 0x03;                     // Set the Internal Oscillator to run
                                       // at 12 MHz

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
   // VDM0CN and RSTSRC are accessible from all SFR pages

   // Step 1. Enable the VDD Monitor (VDMEN bit in VDM0CN = ‘1’).
   VDM0CN = 0x80;                      // Enable the VDD Monitor

   // Step 2. Wait for the VDD Monitor to stabilize (approximately 5 µs).
   // Note: This delay should be omitted if software contains routines which
   // erase or write Flash memory.
   // Omitted in this code

   // Step 3. Select the VDD Monitor as a reset source (PORSF bit in
   // RSTSRC = ‘1’).

   RSTSRC = 0x02;
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// Pinout:
//
//    P3.2 -> LED
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   unsigned char SFRPAGE_save = SFRPAGE; // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   XBR1 = 0x40;                        // Enable crossbar
   P2MDOUT = 0x04;                     // Set LED as push-pull

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_Delay_ms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) unsigned int ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 65335
//
// Configure Timer0 to delay <ms> milliseconds before returning.
//
// Note: This routine modifies the SCA0-1 bits in CKCON, which will affect
// the timing of Timer1.
//-----------------------------------------------------------------------------
void Timer0_Delay_ms (unsigned int ms)
{
   unsigned i;                         // Millisecond counter

   TCON  &= ~0x30;                     // STOP Timer0 and clear overflow flag
   TMOD  &= ~0x0F;                     // Configure Timer0 to 16-bit mode
   TMOD  |=  0x01;

   CKCON &=  ~0x04;                    // Timer0 counts SYSCLK / 48
   CKCON |= 0x02;

   for (i = 0; i < ms; i++)            // Count milliseconds
   {
      TCON &= ~0x10;                   // STOP Timer0
      TH0 = (-SYSCLK/48/1000) >> 8;    // Set Timer0 to overflow in 1ms
      TL0 = -SYSCLK/48/1000;
      TCON |= 0x10;                    // START Timer0
      while (TCON_TF0 == 0);           // Wait for overflow
      TCON &= ~0x20;                   // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
