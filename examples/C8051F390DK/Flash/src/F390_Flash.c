//-----------------------------------------------------------------------------
// F390_Flash.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program tests all the functions available in F390_FlashPrimitives.c
// and F390_FlashUtils.c.  It uses a test data array to mimic the changes
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
//       J1:   IMEASURE
//       J8:   LED/P1.1
// 2) Connect the USB Debug Adapter to J2.
// 3) Turn the POWER switch (SW1) to the "VREG" position.
// 4) Compile and download code to a 'F390 device on a UDP F390/F370 MCU
//    Daughter Card board by selecting Run -> Debug from the menus, clicking
//    the Debug button in the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Check that the LED is blinking, indicating a passing test.
//
// Option 2:
// 1) Ensure that jumpers are placed on the following:
//       J1:   IMEASURE
//       J8:   LED/P1.1
// 2) Connect the USB Debug Adapter to J2.
// 3) Turn the POWER switch (SW1) to the "VREG" position.
// 4) Compile and download code to a 'F390 device on a UDP F390/F370 MCU
//    Daughter Card board by selecting Run -> Debug from the menus, clicking
//    the Debug button in the quick menu, or pressing F11.
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
// Target:         C8051F37x/C8051F39x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 15 JAN 2014

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"
#include "SI_C8051F390_Register_Enums.h"                 // SFR declarations
#include "F390_FlashPrimitives.h"
#include "F390_FlashUtils.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             49000000/16  // SYSCLK in Hz
#define START_ADDRESS          0x1000

SI_SBIT(LED, SFR_P1, 1);                 // LED

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

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
   PCA0MD &= ~0x40;                    // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   unsigned char temp_byte = 0x00;
   FLADDR start_address = START_ADDRESS;

   char test_write_buff[8] = "ABCDEFG";
   char test_write_buff2[3] = "HIJ";
   char test_read_buff[8] = {0};
   char test_compare_buff[8] = "ABCDEFG";

   unsigned char i;

   bit error_flag = 0;


   VDDMon_Init();

   Port_Init();

   LED = 1;

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

   // FLASH test scratchpad----------------------------------------------------
//   FLASH_ByteWrite(0x0000, 0xA5);
//
//   temp_byte = FLASH_ByteRead(0x0000);
//
//   if (temp_byte != 0xA5)
//   {
//      error_flag = 1;
//   }
//   //--------------------------------------------------------------------------

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
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// VDDMon_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function enables the VDD Monitor and enables it as a reset source.
// The VDD Monitor is enabled following a power-on reset, but subsequent resets
// can accidentally disable the monitor.
//
//-----------------------------------------------------------------------------
void VDDMon_Init(void)
{
   VDM0CN |= 0x80;                     // Ensure the VDD Monitor is enabled

   // Select the VDD Monitor as a reset source (PORSF bit in RSTSRC = ‘1’)
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
//    P1.6 -> LED
//
//    all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P1MDOUT |= 0x02;                    // Set LED as push-pull

   XBR1     = 0x40;                    // Enable crossbar and weak pull-ups
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
//
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
      while (TCON_TF0 == 0);                // Wait for overflow
      TCON &= ~0x20;                   // Clear overflow indicator
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
