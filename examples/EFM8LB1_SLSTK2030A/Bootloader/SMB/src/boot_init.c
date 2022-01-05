/******************************************************************************
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "efm8_device.h"
#include "boot.h"

// ----------------------------------------------------------------------------
// Initialize all hardware resources needed by the bootloader.
// ----------------------------------------------------------------------------
void boot_initDevice(void) 
{
  // Disable Watchdog with key sequence
  WDTCN = 0xDE;
  WDTCN = 0xAD;

  // Enable VDD monitor and set it as a reset source
  VDM0CN |= VDM0CN_VDMEN__ENABLED;
  RSTSRC = RSTSRC_PORSF__SET;

  // CLKSEL - Clock Select
  // CLKSL (Clock Source Select) = HFOSC0 (Clock derived from the Internal
  //     High Frequency Oscillator 0.)
  // CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
  //     selected clock source divided by 1.)
  CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;

  // P0SKIP/P1SKIP - Port 0/1 Skip settings
  // These settings are dictated by the SMBus bootloader pinout option
#if defined (S0_PINOUT)
  // Pinout of production devices with bootloader version S0
  // P0.2 = SDA, P0.3 = SCL
  // Skip P0.0, P0.1
  P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED;
#elif defined (S1_PINOUT)
  // Pinout of production devices with bootloader version S1
  // P0.2 = SDA, P0.4 = SCL
  // Skip P0.0, P0.1, P0.3
  P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B3__SKIPPED;
#else  // STK_PINOUT used by default
  // The device on the SLSTK2030A uses nonstandard SMBus pin assignments:
  // P1.2 = SDA, P1.3 = SCL
  // Skip P0.x, P1.0, P1.1
  P0SKIP = 0xFF;
  P1SKIP = P1SKIP_B0__SKIPPED | P1SKIP_B1__SKIPPED;
#endif

  // XBR0 - Port I/O Crossbar 0
  // SMB0E (SMB0 I/O Enable) = ENABLED (SMBus 0 I/O routed to Port pins.)
  XBR0 = XBR0_SMB0E__ENABLED;

  // XBR2 - Port I/O Crossbar 2
  // WEAKPUD (Port I/O Weak Pullup Disable) = PULL_UPS_ENABLED (Weak Pullups enabled.)
  // XBARE (Crossbar Enable) = ENABLED (Crossbar enabled.)
  XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;

  // SMB0ADR - SMBus 0 Slave Address
  // GC (General Call Address Enable) = IGNORED (General Call Address is ignored.)
  // SLV (SMBus Hardware Slave Address)
  SMB0ADR = SMB0ADR_GC__IGNORED | (BL_SMB_SLAVE_ADDRESS & SMB0ADR_SLV__FMASK);

  // SMB0ADM - SMBus 0 Slave Address Mask
  // EHACK (Hardware Acknowledge Enable) = ADR_ACK_AUTOMATIC (Automatic
  //     slave address recognition and hardware acknowledge is enabled.)
  // SLVM (SMBus Slave Address Mask) = 0x7F
  SMB0ADM = SMB0ADM_EHACK__ADR_ACK_AUTOMATIC | (0x7F << SMB0ADM_SLVM__SHIFT);

  // SMB0CF - SMBus 0 Configuration
  // ENSMB (SMBus Enable) = ENABLED (Enable the SMBus module.)
  // EXTHOLD (SMBus Setup and Hold Time Extension Enable) = ENABLED (Enable
  //     SDA extended setup and hold times.)
  SMB0CF = SMB0CF_ENSMB__ENABLED | SMB0CF_EXTHOLD__ENABLED;
}
