//-----------------------------------------------------------------------------
// F500_Flash_Test.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program tests all the functions available in F500_FlashPrimitives.c
// and F500_FlashUtils.c.  It uses a test data array to mimic the changes
// made to the Flash and test for correctness.  While the test is running, the
// LED is on.  If the test fails, the LED will turn off.  If the test passes,
// the LED will blink.
//
// Pinout:
//
//    P1.3 -> LED
//
//    all other port pins unused
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    J19: P1.3 LED
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 2) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 3) Compile and download code to C8051F500 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 5) If the LED flashes, the program is working.
//    Or open the Code window in the Simplicity IDE and verify the following
//    data at address <start_address>:
//
//        48 49 4A 44 45 46
//        47 00 48 49 4A 44
//        45 46 47 00 5A 5A
//        5A 5A 5A 5A 5A 5A
//
//
// Target:         C8051F50x/1x (Side A of a C8051F500-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 11 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 11 AUG 2014 (JL)
//    - Updated to use Simplicity Studio
//
// Release 1.1 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//    -Updated to meet new data sheet guidelines for writing/erasing
//
// Release 1.0
//    -Initial Revision (GP)
//    -08 APR 2008


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>
#include "F500_FlashPrimitives.h"
#include "F500_FlashUtils.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P1, 3);                 // LED == 1 means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);           // Oscillator initialization routine
void VDDMon_Init (void);               // VDD Monitor initialization routine
void PORT_Init (void);                 // Port initialization routine

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint8_t temp_byte = 0x00;
   FLADDR start_address = 0x5FFE;

   uint8_t test_write_buff[8] = "ABCDEFG";
   uint8_t test_write_buff2[3] = "HIJ";
   uint8_t test_read_buff[8] = {0};
   uint8_t test_compare_buff[8] = "ABCDEFG";

   uint8_t i, j;

   bit error_flag = 0;

   SFRPAGE = LEGACY_PAGE;              // Set for PCA0MD and RSTSRC


   if ((RSTSRC & RSTSRC_PORSF__BMASK) != RSTSRC_PORSF__SET)
   {
      if ((RSTSRC & RSTSRC_FERROR__BMASK) == RSTSRC_FERROR__SET)
      {
         LED = 0;
         while(1);                     // Last reset was caused by a Flash
                                       // Error Device Reset
                                       // LED is off and loop forever to
                                       // indicate error
      }
   }

   OSCILLATOR_Init();                  // Initialize the internal oscillator
   VDDMon_Init();                      // Initialize the VDD monitor
   PORT_Init();                        // Initialize the crossbar

   LED = 1;

   // Initially erase the test pages of Flash
   FLASH_PageErase(start_address);
   FLASH_PageErase(start_address+2);

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
         LED = !LED;

         // Simple delay loop
         for (i = 65000; i > 0; i--) {
            for (j = 65000; j > 0; j--) {} }
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

   // Configure internal oscillator for 24 MHz / 8
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_8;

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
// P1.3   digital   push-pull     LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P1MDIN  |= 0x08;                    // P1.3 is digital
   P1MDOUT |= 0x08;                    // P1.3 is push-pull

   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------