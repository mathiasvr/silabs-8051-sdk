//-----------------------------------------------------------------------------
// CapTouchSense_WakeOnRTC_Calibrate.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This file contains functions related to the calibration/configuration of 
// the Capacitive TouchSense switches with Wake on RTC.
//
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.0
//    -Initial Revision (PKC)
//    -20 MAY 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F930_Register_Enums.h>
#include "CapTouchSense_WakeOnRTC.h"

void Calibrate (void)
{
   uint8_t i, CLKSEL_save;
   SI_UU16_t timer_count_diff;
   uint16_t timer_count_a, timer_count_b;

   // Erase Flash page that will hold the calibration values
   FLASH_PageErase (CAL_CONSTANTS_PAGE_ADDR, 0);

   // Indicate calibration required
   RED_LED = 0;

   while (SW2 == SWITCH_IDLE); // Wait till user presses S2 switch

   RED_LED = 1;

   for (i = 0; i < NUM_SWITCHES; i++)
   {
      CPT0MX = CS_Switch_Mux_Setting[i];

      // Prepare Timer2 for the first TouchSense reading
      TMR2CN_TF2H = 0;                     // Clear Timer2 overflow flag
      IE_ET2 = 1;                      // Enable Timer2 interrupts
      PCON |= IDLE;                 // Enter IDLE mode
      // Timer2 interrupt will occur when a comparator rising edge is captured
      // MCU will exit IDLE mode when the Timer2 interrupt occurs
      // Code in the Timer2 ISR will clear the TMR2CN_TF2H overflow flag
      timer_count_a = TMR2RL;       // Capture count
      
      // Prepare Timer2 for the second TouchSense reading
      PCON |= IDLE;                 // Enter IDLE mode
      // Timer2 interrupt will occur when a comparator rising edge is captured
      // MCU will exit IDLE mode when the Timer2 interrupt occurs
      // Code in the Timer2 ISR will clear the TMR2CN_TF2H overflow flag
      timer_count_b = TMR2RL;        // Capture count

      IE_ET2 = 0;                      // Disable Timer2 interrupts

      // Calculate the oscillation period
      timer_count_diff.u16 = timer_count_b - timer_count_a;

      // Add standard threshold and subtract SWITCH_SENSITIVITY
      timer_count_diff.u16 = timer_count_diff.u16 + SWITCH_STANDARD_THRESHOLD - SWITCH_SENSITIVITY;

      // Store calibration threshold in non-volatile FLASH memory
      FLASH_ByteWrite (((uint16_t)(CS_Switch_Cal_Threshold) + MSB + i*2), timer_count_diff.u8[MSB], 0);
      FLASH_ByteWrite (((uint16_t)(CS_Switch_Cal_Threshold) + LSB + i*2), timer_count_diff.u8[LSB], 0);

      P1 &= ~LED_P1_Mask[i];
   }

   // Wait some time to allow enough time for user to notice the lit LEDs
   CLKSEL_save = CLKSEL;
   CLKSEL = 0x74;                     // Set system clock divisor to 128
   for (timer_count_diff.u16 = 0; timer_count_diff.u16 < 0x1FFF; timer_count_diff.u16++);
   CLKSEL = CLKSEL_save;               // Restore original clock setting
}

//-----------------------------------------------------------------------------
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of the byte to write to
//                    valid range is from 0x0000 to 0xFBFF for 64K devices
//                    valid range is from 0x0000 to 0x7FFF for 32K devices
//   2) char byte - byte to write to Flash.
//
// This routine writes <byte> to the linear FLASH address <addr>.
//-----------------------------------------------------------------------------
void FLASH_ByteWrite (uint16_t addr, uint8_t byte, uint8_t SFLE)
{
   uint8_t EA_Save = IE;                    // Preserve IE_EA
   SI_SEGMENT_VARIABLE_SEGMENT_POINTER (pwrite, uint8_t, xdata, data); 
   //unsigned char xdata * data pwrite;// FLASH write pointer

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor


   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (char xdata *) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x01;                      // PSWE = 1
   
   if(SFLE)
   {
      PSCTL |= 0x04;                   // Access Scratchpad
   }

   VDM0CN = 0x80;                      // Enable VDD monitor


   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   *pwrite = byte;                     // Write the byte

   PSCTL &= ~0x05;                     // SFLE = 0; PSWE = 0

   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }

}



//-----------------------------------------------------------------------------
// FLASH_PageErase
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of any byte in the page to erase
//                    valid range is from 0x0000 to 0xFBFF for 64K devices
//                    valid range is from 0x0000 to 0x7FFF for 32K devices
//
// This routine erases the FLASH page containing the linear FLASH address
// <addr>.  Note that the page of Flash containing the Lock Byte cannot be
// erased if the Lock Byte is set.
//
//-----------------------------------------------------------------------------
void FLASH_PageErase (uint16_t addr, uint8_t SFLE)
{
   uint8_t EA_Save = IE;                    // Preserve IE_EA
   
   SI_SEGMENT_VARIABLE_SEGMENT_POINTER (pwrite, uint8_t, xdata, data); 
   //unsigned char xdata * data pwrite;// FLASH write pointer

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (char xdata *) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1
   
   if(SFLE)
   {
      PSCTL |= 0x04;                   // Access Scratchpad
   }

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = 0;                        // Initiate page erase

   PSCTL &= ~0x07;                     // SFLE = 0; PSWE = 0; PSEE = 0

   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
