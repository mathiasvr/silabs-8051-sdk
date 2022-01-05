//-----------------------------------------------------------------------------
// F700_Flash_Test.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program tests all the functions available in F700_FlashPrimitives.c
// and F700_FlashUtils.c.  It uses a test data array to mimic the changes
// made to the Flash and test for correctness.  While the test is running, the
// LED is on.  If the test fails, the LED will turn off.  If the test passes,
// the LED will blink.
//
// Pinout:
//
//    P1.0 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// Option 1:
// 1) Ensure shorting blocks are place on the following:
//    - J8:  P1.0 - DS3
//	  - J15: VDD - power source
// 2) Connect the USB Debug Adapter to P3.
// 3) Connect the USB cable to P4.
// 4) Compile and download code to the C8051F70x development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Verify that the LED is blinking, indicating a passing test.
//
// Option 2:
// 1) Ensure shorting blocks are place on the following:
//    - J8:  P1.0 - DS3
// 2) Connect the USB Debug Adapter to P3.
// 3) Connect the USB cable to P4.
// 4) Compile and download code to the C8051F70x development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) Open the memory window in the debug interface and verify the following
//    data at address <start_address>:
//
//        48 49 4A 44 45 46
//        47 00 48 49 4A 44
//        45 46 47 00 5A 5A
//        5A 5A 5A 5A 5A 5A
//
//
// Target:         C8051F70x
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
//    -Initial Revision (PD)
//    -21 APR 2010
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"
#include <SI_C8051F700_defs.h>         // SFR declarations
#include "F700_FlashUtils.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(LED, SFR_P1, 0);                 // LED == 1 means ON

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
   WDTCN = 0xDE;                       // Disable the watchdog timer
   WDTCN = 0xAD;                       // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint8_t temp_byte = 0x00;
   FLADDR start_address = 0x3000;

   uint8_t test_write_buff[8] = "ABCDEFG";
   uint8_t test_write_buff2[3] = "HIJ";
   uint8_t test_read_buff[8] = {0};
   uint8_t test_compare_buff[8] = "ABCDEFG";

   uint16_t i, j;

   bit error_flag = 0;

   SFRPAGE = LEGACY_PAGE;              // Set for PCA0MD and RSTSRC


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

   //END OF TEST===============================================================

   while (1)                           // Loop forever
   {
      // Blink LED to indicate success
      if (error_flag == 0)
      {
         LED = !LED;

         // Simple delay loop
         for (i = 650; i > 0; i--) {
            for (j = 650; j > 0; j--) {} }
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
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x83;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
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

   // Step 1. Enable the VDD Monitor
   VDM0CN = 0x80;

   // Step 2. Wait for the VDD Monitor to stabilize (approximately 5 �s).
   // Note: This delay should be omitted if software contains routines which
   // erase or write Flash memory.
   // Omitted in this code

   // Step 3. Select the VDD Monitor as a reset source (PORSF bit in
   // RSTSRC = �1�).

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
// P1.0   digital   push-pull     LED
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   P1MDOUT |= 0x01;                    // Enable P1.0 as push-pull output
   XBR1    = 0x40;                     // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
