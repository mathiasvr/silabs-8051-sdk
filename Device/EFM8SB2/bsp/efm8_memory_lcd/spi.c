/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// Spi.c
/////////////////////////////////////////////////////////////////////////////

// SPI FIFO Format
// ---------------
//
// All SPI transfers are pushed to the TX FIFO with the transfer size in
// bytes followed by the data to transmit.

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "spi.h"
#include "spi_1.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////

static SI_SEGMENT_VARIABLE(TxBuffer[SPI_BUF_SIZE], uint8_t, EFM8PDL_SPI1_TX_SEGTYPE);

static volatile uint8_t TransferState = ST_IDLE;
static volatile uint8_t TransferSize = 0;

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

void SPI_StartTransfer(SI_VARIABLE_SEGMENT_POINTER(buffer, uint8_t, SI_SEG_GENERIC), uint8_t size)
{
  uint8_t i;

  // Wait for previous transfer to complete
  while (TransferState != ST_IDLE);

  // Start new transfer
  TransferState = ST_CS_SETUP;
  TransferSize = size;

  for (i = 0; i < size; i++)
  {
    TxBuffer[i] = buffer[i];
  }

  // CS setup time
  // Timer 2 overflows at end of setup time
  TMR2 = (uint16_t)-(DELAY_TIMER_FREQ / 1000000 * SPI_CS_SETUP_US);
  BSP_DISP_CS = SPI_CS_ASSERT_LVL;
  TMR2CN0 |= TMR2CN0_TR2__BMASK;
}

/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Handlers
/////////////////////////////////////////////////////////////////////////////

void SPI1_transferCompleteCb(void)
{
  // CS hold time
  TransferState = ST_CS_HOLD;

  // CS hold time
  // Timer 2 overflows at end of hold time
  TMR2 = (uint16_t)-(DELAY_TIMER_FREQ / 1000000 * SPI_CS_HOLD_US);
  TMR2CN0 |= TMR2CN0_TR2__BMASK;
}

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN::TF2L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
  TMR2CN0 &= ~TMR2CN0_TF2H__BMASK;

  // Stop timer
  TMR2CN0 &= ~TMR2CN0_TR2__BMASK;

  // CS setup complete
  if (TransferState == ST_CS_SETUP)
  {
    TransferState = ST_TX;
    SPI1_transfer(TxBuffer, NULL, SPI1_TRANSFER_TX, TransferSize);
  }
  // CS hold complete
  else if (TransferState == ST_CS_HOLD)
  {
    // Deassert CS
    BSP_DISP_CS = SPI_CS_DEASSERT_LVL;

    // Transfer complete
    TransferState = ST_IDLE;
  }
}
