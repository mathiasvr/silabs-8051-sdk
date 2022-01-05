/******************************************************************************
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "efm8_device.h"
#include "boot.h"
#include "USB_main.h"

// ----------------------------------------------------------------------------
// Initialize all hardware resources needed by the bootloader.
// ----------------------------------------------------------------------------
void boot_initDevice(void) 
{
  // PCA0MD - PCA Mode
  // WDTE (Watchdog Timer Enable) = DISABLED (Disable Watchdog Timer.)
  PCA0MD &= ~PCA0MD_WDTE__BMASK;

  // FLSCL - Flash Scale
  // FOSE (Flash One-Shot Enable) = ENABLED
  // FLRT (Flash Read Timing) = SYSCLK_BELOW_48_MHZ
  FLSCL |= FLSCL_FOSE__ENABLED | FLSCL_FLRT__SYSCLK_BELOW_48_MHZ;
  
  // CLKSEL - Clock Select
  // CLKSL (System Clock Source Select Bits) = HFOSC (Clock (SYSCLK)
  //     derived from the Internal High-Frequency Oscillator.)
  // USBCLK (USB Clock Source Select Bits) = HFOSC (USB clock (USBCLK)
  //     derived from the Internal High-Frequency Oscillator.)
  CLKSEL = CLKSEL_CLKSL__HFOSC | CLKSEL_USBCLK__HFOSC;

  // Initialize the USB driver
  USB_initModule();
}
