/**************************************************************************//**
 * Copyright 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

// -----------------------------------------------------------------------------
// Includes

#include <SI_EFM8BB3_Register_Enums.h>
#include <si_toolchain.h>
#include "smb_0.h"

#include <ncp81239_driver.h>

// -----------------------------------------------------------------------------
// Definitions

/// Size of I2C Buffers
#define I2C_DATA_BUF_SIZE   0x04
#define I2C_RETRIES         0x01

// -----------------------------------------------------------------------------
// Variables

/// State of I2C Bus
static volatile bool i2cTransferInProgress = false;

/// Error on I2C Bus
static bool i2cBusError = false;

/// Error during I2C Transfer
static SMB0_TransferError_t i2cTransferError = false;

/// Buffer for I2C Write Data
static SI_SEGMENT_VARIABLE(i2cDataOut[I2C_DATA_BUF_SIZE],
                           uint8_t,
                           EFM8PDL_SMB0_TX_BUFTYPE);

/// Buffer for I2C Read Data
static SI_SEGMENT_VARIABLE(i2cDataIn[I2C_DATA_BUF_SIZE],
                           uint8_t,
                           EFM8PDL_SMB0_TX_BUFTYPE);

// -----------------------------------------------------------------------------
// Function Prototypes

static uint8_t getErrorCode(void);

// -----------------------------------------------------------------------------
// Functions

// -------------------------------
// API Functions

ncp81239_error_t ncp81239_init(void)
{
  ncp81239_error_t status;
  uint16_t delay = 0x8000;

  SMB0_reset();
  SMB0_init(SMB0_TIMER1, false);

  i2cTransferInProgress = false;
  i2cBusError = false;
  i2cTransferError = false;

  // Delay for Power IC initialization
  while(delay--)
  {}

  // Disable regulator output
  status = ncp81239_disable();

  // Default to 5V
  if (status == NCP81239_STATUS_OK)
  {
    status = ncp81239_setOutputVoltage(5000);
  }
  // Set slew rate3
  if (status == NCP81239_STATUS_OK)
  {
    status = ncp81239_setSlewRate(NCP81239_SLEW_RATE_4P9_MVUS);
  }

  return status;
}

ncp81239_error_t ncp81239_enable(void)
{
  return ncp81239_writeReg(NCP81239_REG_ENABLE, NCP81239_REG_ENABLE_ENABLE);
}

ncp81239_error_t ncp81239_disable(void)
{
  return ncp81239_writeReg(NCP81239_REG_ENABLE, NCP81239_REG_ENABLE_DISABLE);
}

ncp81239_error_t ncp81239_readReg(uint8_t addr, uint8_t * dat)
{
  ncp81239_error_t status;
  uint8_t retries = I2C_RETRIES;

  if (i2cTransferInProgress)
  {
    return NCP81239_STATUS_BUSY_ERROR;
  }

  i2cDataOut[0] = addr;

  do
  {
    i2cTransferInProgress = true;
    i2cBusError = false;

    SMB0_transfer(NCP81239_I2C_SLAVE_ADDR, i2cDataOut, i2cDataIn, 1, 1);
    while (i2cTransferInProgress);

    status = getErrorCode();

    if (status == NCP81239_STATUS_OK)
    {
      break;
    }
  } while (retries--);

  *dat = i2cDataIn[0];

  return status;
}

ncp81239_error_t ncp81239_writeReg(uint8_t addr, uint8_t dat)
{
  ncp81239_error_t status;
  uint8_t retries = I2C_RETRIES;

  if (i2cTransferInProgress)
  {
    return NCP81239_STATUS_BUSY_ERROR;
  }

  i2cDataOut[0] = addr;
  i2cDataOut[1] = dat;

  do
  {
    i2cTransferInProgress = true;
    i2cBusError = false;

    SMB0_transfer(NCP81239_I2C_SLAVE_ADDR, i2cDataOut, i2cDataIn, 2, 0);
    while (i2cTransferInProgress);

    status = getErrorCode();

    if (status == NCP81239_STATUS_OK)
    {
      break;
    }
  } while (retries--);

  return status;
}

ncp81239_error_t ncp81239_setOutputVoltage(uint16_t outputMv)
{
  ncp81239_error_t status;

  // Do not allow voltage higher than 20V
  if (outputMv > 20000)
  {
    status = NCP81239_STATUS_INVALID_SETTING;
  }
  else
  {
    status = ncp81239_writeReg(NCP81239_REG_DAC_TARGET,
                               (uint8_t)(outputMv / 100));
  }

  return status;
}

ncp81239_error_t ncp81239_setSlewRate(ncp81239_slew_rate_t slewRate)
{
  ncp81239_error_t status;

  // Check for invalid setting
  if (slewRate > NCP81239_SLEW_RATE_4P9_MVUS)
  {
    status = NCP81239_STATUS_INVALID_SETTING;
  }
  else
  {
    status = ncp81239_writeReg(NCP81239_REG_SLEW_RATE, slewRate);
  }

  return status;
}


// -------------------------------
// I2C Peripheral Driver Callbacks

void SMB0_commandReceivedCb()
{
  i2cTransferInProgress = false;
}

// Callback is called when an SMBus error occurs
void SMB0_errorCb (SMB0_TransferError_t error)
{
  // Abort the transfer
  SMB0_abortTransfer();

  i2cBusError = true;
  i2cTransferError = error;
  i2cTransferInProgress = false;
}

void SMB0_transferCompleteCb()
{
  // Transfer is complete (success)
  i2cTransferInProgress = false;
}

// -------------------------------
// Helper Functions
static uint8_t getErrorCode(void)
{
  if (i2cBusError == false)
  {
    return NCP81239_STATUS_OK;
  }
  else
  {
    switch (i2cTransferError)
    {
    case SMB0_ARBLOST_ERROR:
      return NCP81239_STATUS_ARBLOST_ERROR;

    case SMB0_NACK_ERROR:
      return NCP81239_STATUS_NACK_ERROR;

    case SMB0_TXUNDER_ERROR:
      return NCP81239_STATUS_TXUNDER_ERROR;

    default:
      return NCP81239_STATUS_UNKNOWN_ERROR;
    }
  }
}
