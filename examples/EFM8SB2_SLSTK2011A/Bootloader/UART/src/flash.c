/******************************************************************************
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "efm8_device.h"
#include "flash.h"

// Holds the flash keys received in the prefix command
uint8_t flash_key1;
uint8_t flash_key2;

// ----------------------------------------------------------------------------
// Writes one byte to flash memory.
// ----------------------------------------------------------------------------
void writeByte(uint16_t addr, uint8_t byte)
{
  uint8_t SI_SEG_XDATA * pwrite = (uint8_t SI_SEG_XDATA *) addr;

  // Unlock flash by writing the key sequence
  FLKEY = flash_key1;
  FLKEY = flash_key2;

  // Enable flash writes, then do the write
  PSCTL |= PSCTL_PSWE__WRITE_ENABLED;
  *pwrite = byte;
  PSCTL &= ~(PSCTL_PSEE__ERASE_ENABLED|PSCTL_PSWE__WRITE_ENABLED);
}

// ----------------------------------------------------------------------------
// Erases one page of flash memory.
// ----------------------------------------------------------------------------
void flash_erasePage(uint16_t addr)
{
  // Enable flash erasing, then start a write cycle on the selected page
  PSCTL |= PSCTL_PSEE__ERASE_ENABLED;
  writeByte(addr, 0);
}

// ----------------------------------------------------------------------------
// Writes one byte to flash memory.
// ----------------------------------------------------------------------------
void flash_writeByte(uint16_t addr, uint8_t byte)
{
  // Don't bother writing the erased value to flash
  if (byte != 0xFF)
  {
    writeByte(addr, byte);
  }
}

// ----------------------------------------------------------------------------
// Selects the active flash bank for erase, write and verify.
// ----------------------------------------------------------------------------
void flash_setBank(uint8_t bank)
{
  // Setting the bank to 1 selects scratchpad flash
  if (bank == 1)
  {
    PSCTL = PSCTL_SFLE__SCRATCHPAD_ENABLED;
  }
  else
  {
    PSCTL = PSCTL_SFLE__SCRATCHPAD_DISABLED;
  }
}

// ----------------------------------------------------------------------------
// Check if flash address range may be erased or written.
// ----------------------------------------------------------------------------
bool flash_isValidRange(uint16_t addr, uint8_t size)
{
  uint16_t limit = BL_LIMIT_ADDRESS;

  // Adjust upper limit if scratchpad is selected
  if (PSCTL & PSCTL_SFLE__SCRATCHPAD_ENABLED)
  {
    limit = BL_FLASH1_LIMIT;
  }
  // Test address range against the upper limit for this region
  if ((addr < limit) && (addr + size <= limit))
  {
    return true;
  }
  else
  {
    return false;
  }
}

// ----------------------------------------------------------------------------
// Initialize the CRC register.
// ----------------------------------------------------------------------------
void flash_initCRC(void)
{
  SET_SFRPAGE(CRC0_PAGE);
  CRC0CN0 = CRC0CN0_POLYSEL__16_BIT | CRC0CN0_CRCINIT__INIT;
}

// ----------------------------------------------------------------------------
// Return the CRC register value.
// ----------------------------------------------------------------------------
uint16_t flash_readCRC(void)
{
  uint16_t crc16;

  // Read the result starting with the MSB
  CRC0CN0 |= CRC0CN0_CRCPNT__ACCESS_B3;
  crc16  = CRC0DAT << 8;
  crc16 |= CRC0DAT;

  // Restore the default SFR page
  SET_SFRPAGE(0);
  return crc16;
}
