/******************************************************************************
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "efm8_device.h"
#include "boot.h"
#include "USB_main.h"

// Counts the number of bytes remaining in the receive buffer
uint8_t boot_rxSize;

// Index of the next byte to get from the receive buffer
uint8_t boot_rxTail;

// ----------------------------------------------------------------------------
// Wait for the next boot record to arrive.
// ----------------------------------------------------------------------------
void boot_nextRecord(void)
{
  // Wait indefinitely to receive a USB packet that begins with the frame 
  // start character
  do
  {
    // While waiting, reset the receive buffer head and tail pointer
    usb_rxHead = 0;
    boot_rxTail = 0;
  }
  while (BOOT_FRAME_START != boot_getByte());

  // Receive and save the frame length
  boot_rxSize = boot_getByte();
}

// ----------------------------------------------------------------------------
// Get the next byte in the boot record.
// ----------------------------------------------------------------------------
uint8_t boot_getByte(void)
{
  uint8_t next;

  // If the USB receive buffer is empty, wait for more data
  while (boot_rxTail == usb_rxHead)
  {
    USB_pollModule();
  } 

  // Retrieve next byte from USB receive buffer, then adjust index and counter.
  // The buffer size is 256, so indexes wrap properly without limit testing.
  next = usb_rxBuf[boot_rxTail];
  boot_rxTail++;
  boot_rxSize--;
  return next;
}

// ----------------------------------------------------------------------------
// Get the next word in the boot record.
// ----------------------------------------------------------------------------
uint16_t boot_getWord(void)
{
  SI_UU16_t word;

  // 16-bit words are received in big-endian order
  word.u8[0] = boot_getByte();
  word.u8[1] = boot_getByte();
  return word.u16;
}

// ----------------------------------------------------------------------------
// Send a one byte reply to the host.
// ----------------------------------------------------------------------------
void boot_sendReply(uint8_t reply)
{
  // Put reply byte in USB transmit buffer
  usb_txBuf[0] = reply;
  usb_txCount = USB_HID_IN_SIZE;

  // Wait for the buffer to be transmitted before returning
  while (usb_txCount != 0)
  {
    USB_sendReport();
    USB_pollModule();
  }
}

// ----------------------------------------------------------------------------
// Exit bootloader and start the user application.
// ----------------------------------------------------------------------------
void boot_runApp(void)
{
  // Use the USB tick to delay for 100 msec. This gives the host time to 
  // process the reply packet before dropping off the USB bus.
  usb_tick = 255-100;
  do
  {
    USB_pollModule();
  }
  while (usb_tick);

  // Clear R0 so we don't re-enter the bootloader, then do a software reset
  *((uint8_t SI_SEG_DATA *)0x00) = 0;
  RSTSRC = RSTSRC_SWRSF__SET | RSTSRC_PORSF__SET;
}
