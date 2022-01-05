/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "usb_0.h"
#include <stdbool.h>
#include <stdint.h>

/** @addtogroup usb_0_runtime USB0 Runtime API */

// -----------------------------------------------------------------------------
// Functions

// -------------------------------
// Utility Functions

/**************************************************************************//**
 * @brief       Reads a 16-bit indirect USB register value
 * @param [in]  regAddr
 *   Address of high byte of 16-bit USB indirect register to read
 * @return      16-bit register value
 *****************************************************************************/
static uint16_t USB_GetShortRegister(uint8_t regAddr)
{
  uint16_t retVal;

  USB_READ_BYTE(regAddr);
  retVal = (USB0DAT << 8);
  USB_READ_BYTE((regAddr - 1));
  retVal |= USB0DAT;

  return retVal;
}

// -------------------------------
// USB0 Peripheral Driver Functions

void USB_SetIndex(uint8_t epsel)
{
  USB_WRITE_BYTE(INDEX, epsel);
}

uint8_t USB_GetCommonInts(void)
{
  USB_READ_BYTE(CMINT);
  return USB0DAT;
}

uint8_t USB_GetInInts(void)
{
  USB_READ_BYTE(IN1INT);
  return USB0DAT;
}

uint8_t USB_GetOutInts(void)
{
  USB_READ_BYTE(OUT1INT);
  return USB0DAT;
}

uint8_t USB_GetIndex(void)
{
  USB_READ_BYTE(INDEX);
  return USB0DAT;
}

bool USB_IsSuspended(void)
{
  USB_READ_BYTE(POWER);
  return USB0DAT & POWER_SUSMD__SUSPENDED;
}

bool USB_GetSetupEnd(void)
{
  USB_READ_BYTE(E0CSR);
  return USB0DAT & E0CSR_SUEND__SET;
}

bool USB_Ep0SentStall(void)
{
  USB_READ_BYTE(E0CSR);
  return USB0DAT & E0CSR_STSTL__SET;
}

bool USB_Ep0OutPacketReady(void)
{
  USB_READ_BYTE(E0CSR);
  return USB0DAT & E0CSR_OPRDY__SET;
}

bool USB_Ep0InPacketReady(void)
{
  USB_READ_BYTE(E0CSR);
  return USB0DAT & E0CSR_INPRDY__SET;
}

uint8_t USB_Ep0GetCount(void)
{
  USB_READ_BYTE(E0CNT);
  return USB0DAT;
}

bool USB_EpnInGetSentStall(void)
{
  USB_READ_BYTE(EINCSRL);
  return (bool)(USB0DAT & EINCSRL_STSTL__SET);
}

void USB_AbortInEp(uint8_t fifoNum)
{
  USB_SetIndex(fifoNum);
  USB_EpnInFlush();
  USB_EpnInFlush();
}

bool USB_EpnOutGetSentStall(void)
{
  USB_READ_BYTE(EOUTCSRL);
  return (bool)(USB0DAT & EOUTCSRL_STSTL__SET);
}

bool USB_EpnGetOutPacketReady(void)
{
  USB_READ_BYTE(EOUTCSRL);
  return (bool)(USB0DAT & EOUTCSRL_OPRDY__SET);
}

bool USB_EpnGetDataError(void)
{
  USB_READ_BYTE(EOUTCSRL);
  return (bool)(USB0DAT & EOUTCSRL_DATERR__SET);
}

uint16_t USB_EpOutGetCount(void)
{
  return USB_GetShortRegister(EOUTCNTH);
}

void USB_AbortOutEp(uint8_t fifoNum)
{
  USB_SetIndex(fifoNum);
  USB_EpnOutFlush();
  USB_EpnOutFlush();
}

void USB_ActivateEp(uint8_t ep,
                    uint16_t packetSize,
                    bool inDir,
                    bool splitMode,
                    bool isoMode)
{
  uint8_t CSRH_mask = 0;
  uint16_t fifoSize;

  USB_SetIndex(ep);

  // Determine the available fifoSize for a given endpoint based on the
  // splitMode setting
  fifoSize = (splitMode == true) ? (16 << ep) : (32 << ep);

  if (packetSize <= fifoSize)
  {
    CSRH_mask |= EINCSRH_DBIEN__ENABLED;
  }

  if (isoMode == true)
  {
    CSRH_mask |= EINCSRH_ISO__ENABLED;
  }

  if (inDir == true)
  {
    CSRH_mask |= EINCSRH_DIRSEL__IN;

    if (splitMode == true)
    {
      CSRH_mask |= EINCSRH_SPLIT__ENABLED;
    }
    USB_WRITE_BYTE(EINCSRL, EINCSRL_CLRDT__BMASK);
    USB_WRITE_BYTE(EINCSRH, CSRH_mask);
  }
  else // OUT
  {
    USB_WRITE_BYTE(EOUTCSRL, EOUTCSRL_CLRDT__BMASK);
    USB_WRITE_BYTE(EOUTCSRH, CSRH_mask);

    if (splitMode == false)
    {
      USB_WRITE_BYTE(EINCSRH, 0);
    }
  }
}

uint16_t USB_GetSofNumber(void)
{
  return USB_GetShortRegister(FRAMEH);
}

bool USB_GetIntsEnabled(void)
{
  SFRPAGE = PG2_PAGE;
  return (bool)(EIE2 & EIE2_EUSB0__ENABLED);
}

bool USB_IsRegulatorEnabled(void)
{
  SFRPAGE = PG3_PAGE;
  return !(REG1CN & REG1CN_REG1ENB__DISABLED);
}

void USB_SuspendOscillator(void)
{
  uint8_t pfe0cnSave;
  uint8_t clkselSave = CLKSEL & 0x7F;

  // Set SYSCLK to HFOSC0/8 SYSCLK.
  CLKSEL = (CLKSEL_CLKDIV__SYSCLK_DIV_8 | CLKSEL_CLKSL__HFOSC0);

  // Wait for ready bit before proceeding.
  while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
  {
  }

  // It is safe to disable prefetch and change to slow SYSCLK flash timings.
  SFRPAGE = PG2_PAGE;
  pfe0cnSave = PFE0CN;
  PFE0CN &= ~(PFE0CN_PFEN__ENABLED | PFE0CN_FLRT__SYSCLK_BELOW_50_MHZ);

  // Suspend the oscillator.
  SFRPAGE = LEGACY_PAGE;
  PCON1 |= PCON1_SUSPEND__SUSPEND;

  // Restore original prefetch and flash timings.
  SFRPAGE = PG2_PAGE;
  PFE0CN = pfe0cnSave;

  // Determine if the target frequency is over 25 MHz.
  if ((clkselSave & 0x7F) != (CLKSEL_CLKDIV__SYSCLK_DIV_1
                              | CLKSEL_CLKSL__HFOSC1))
  {
    // Target SYSCLK is not over 25 MHz, so just write to CLKSEL twice.
    CLKSEL = clkselSave;
    CLKSEL = clkselSave;

    // Wait for ready bit before proceeding.
    while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
    {
    }
  }
  else
  {
    // Target frequency is over 25MHz, so we must change clock frequency twice:
    // once to 24.5 MHz from HFOSC0 followed by the actual target frequency.

    // Set SYSCLK to 24.5 MHz from HFOSC0.
    CLKSEL = (CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1);
    CLKSEL = (CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1);

    // Wait for ready bit before proceeding.
    while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
    {
    }

    // Set SYSCLK to actual target frequency.
    CLKSEL = clkselSave;
    CLKSEL = clkselSave;

    // Wait for ready bit before proceeding.
    while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
    {
    }
  }
}

/**  @} (end addtogroup usb_0_runtime USB0 Runtime API) */
