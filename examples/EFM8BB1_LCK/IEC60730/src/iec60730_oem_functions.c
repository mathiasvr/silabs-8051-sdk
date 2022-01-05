/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

// Disable warnings on unused labels.  Labels are used to set breakpoints for
// automated testing.
#pragma warninglevel(1)

#include "iec60730.h"

/**************************************************************************//**
 * @addtogroup efm8_iec60730
 * @{
 *****************************************************************************/

/**************************************************************************//**
 * @addtogroup IEC60730_INVARIABLE_MEMORY_Test
 * @{
 *****************************************************************************/

// For automated testing, there must be at least 1 area that is > IEC60730_BLOCK_SIZE
// and one area that has a remainder of bytes (not divisible by IEC60730_BLOCK_SIZE).
// These test values require space between 0x1400-0x1900 erased, all 0xFF.
// crc_generator.py will replace the values in this array with the correct CRC values.
#message "Modify with the correct number of areas, or replace with individual variables"
SI_SEGMENT_VARIABLE(iec60730_Invar_Crc[], uint16_t, SI_SEG_CODE) = 
{
  0x0000,  // 0xff00
  0x0000,  // 0xba64
  0x0000,  // 0x3cb2
  0x0000,  // 0x5b2f
  0x0000,  // 0xda6e
  0x0000,  // 0x7cd6
  0x0000,  // 0x77eb
  0x0000,  // 0xfb80
};
// End value is not inclusive.
#message "OEM must replace these values with device memory model, per the documentation."
SI_SEGMENT_VARIABLE(iec60730_Invariable[], iec60730_InvariableMemoryArea_t, SI_SEG_CODE) =
{
  {0x1400, 0x1401, &iec60730_Invar_Crc[0], IEC60730_CRC_MODE_STATIC,}, // 1 byte
  {0x1400, 0x14c8, &iec60730_Invar_Crc[1], IEC60730_CRC_MODE_STATIC,}, // 200 bytes
  {0x1400, 0x14ff, &iec60730_Invar_Crc[2], IEC60730_CRC_MODE_STATIC,}, // 255 bytes
  {0x1400, 0x1500, &iec60730_Invar_Crc[3], IEC60730_CRC_MODE_STATIC,}, // 256 bytes
  {0x1400, 0x1501, &iec60730_Invar_Crc[4], IEC60730_CRC_MODE_STATIC,}, // 257 bytes
  {0x1400, 0x17ff, &iec60730_Invar_Crc[5], IEC60730_CRC_MODE_STATIC,}, // 1023 bytes
  {0x1400, 0x1800, &iec60730_Invar_Crc[6], IEC60730_CRC_MODE_STATIC,}, // 1024 bytes
  {0x1400, 0x1801, &iec60730_Invar_Crc[7], IEC60730_CRC_MODE_DYNAMIC,}, // 1025 bytes
};

#message "OEM must set this variable to true once dynamic areas are ready"
SI_SEGMENT_VARIABLE(iec60730_InvariableCheckDynamicAreas, uint8_t, SI_SEG_XDATA) = false;

SI_SEGMENT_VARIABLE(iec60730_InvariableNumAreas, uint8_t, SI_SEG_CODE) =
  sizeof(iec60730_Invariable)/sizeof(iec60730_InvariableMemoryArea_t);

/** @} (end addtogroup IEC60730_INVARIABLE_MEMORY_Test) */

/**************************************************************************//**
 * @addtogroup IEC60730_VARIABLE_MEMORY_Test
 * @{
 *****************************************************************************/

SI_SEGMENT_VARIABLE(iec60730_XdataNumPartitions, uint8_t, SI_SEG_XDATA) =
  (DEVICE_XRAM_SIZE/IEC60730_PARTITION_SIZE);

/** @} (end addtogroup IEC60730_VARIABLE_MEMORY_Test) */

/**************************************************************************//**
 * @addtogroup IEC60730_IRQ_Test
 * @{
 *****************************************************************************/

#message "OEM must update this array with expected bounds for system's IRQs"
SI_SEGMENT_VARIABLE(iec60730_IRQFreqBounds[], iec60730_IRQExecutionBounds_t, SI_SEG_CODE) =
{
 {4,4},                                // Test timer
 {0,0},                                // System timer
 {0,2}                                 // UART 0
};

SI_SEGMENT_VARIABLE(iec60730_IRQFreqBoundsSize, uint8_t, SI_SEG_IDATA) = 
  sizeof(iec60730_IRQFreqBounds)/sizeof(iec60730_IRQExecutionBounds_t);

SI_SEGMENT_VARIABLE(iec60730_IRQExecCount[], uint8_t, SI_SEG_IDATA) =
{
  0,
  0,
  0
};
/** @} (end addtogroup IEC60730_IRQ_Test) */

/**************************************************************************//**
 * @addtogroup IEC60730_SAFE_STATE
 * @{
 *****************************************************************************/

#message "OEM must update this function with appropriate SafeState"
void iec60730_SafeState(iec60730_TestFailure_t failure)
{
#if 0  // TEST CODE
  // Set P1.2 as ground (light LED GPIO.6)
  SwGPIO_6 = 0;
#endif
  // Disable interrupts while in SafeState
  IE_EA = 0;

  IEC60730_SAFE_STATE_BKPT:
  while (1)
  {
    failure = failure; //Prevent compiler from complaining about unused variable.  'failure' is informative only, and not tested.
    iec60730_RestartWatchdog();
  }
}
/** @} (end addtogroup IEC60730_SAFE_STATE) */
/** @} (end addtogroup efm8_iec60730) */
