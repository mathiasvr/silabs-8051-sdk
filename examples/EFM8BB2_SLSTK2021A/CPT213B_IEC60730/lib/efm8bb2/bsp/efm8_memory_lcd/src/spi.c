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
#include "spi_0.h"
#include <string.h>
#include "tick.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK              49000000          // Internal oscillator frequency in Hz
#define PCA_CH0_FREQUENCY       1000          // Frequency for the 1ms tick
#define PCA_CH1_FREQUENCY       5500          // Frequency to start SPI transfer/deassert pin
#define PCA_CH2_FREQUENCY	   20000          // Frequency of the CPT timer
// SYSCLK cycles per interrupt
#define PCA_CH0_TIMEOUT (SYSCLK/PCA_CH0_FREQUENCY)
#define PCA_CH1_TIMEOUT (SYSCLK/PCA_CH1_FREQUENCY)
#define PCA_CH2_TIMEOUT (SYSCLK/PCA_CH2_FREQUENCY)

/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////

static volatile uint16_t Ticks = 0;
uint16_t Time_50us = 0;
uint8_t count_ISR = 0;
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uint16_t Next_Compare_Value_CH0;       // Next edge to be sent out in HSO mode
uint16_t Next_Compare_Value_CH1;       // Next edge to be sent out in HSO mode
uint16_t Next_Compare_Value_CH2;       // Next edge to be sent out in HSO mode
/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////

static SI_SEGMENT_VARIABLE(TxBuffer[SPI_BUF_SIZE], uint8_t, EFM8PDL_SPI0_TX_SEGTYPE);

static volatile uint8_t TransferState = ST_IDLE;
static volatile uint8_t TransferSize = 0;
/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

void SPI_StartTransfer(SI_VARIABLE_SEGMENT_POINTER(buffer, uint8_t, SI_SEG_GENERIC), uint8_t size)
{
  uint8_t data sfrPageSave;
  uint8_t data i;

  // Wait for previous transfer to complete
  while (TransferState != ST_IDLE);

  // Start new transfer
  TransferState = ST_CS_SETUP;
  TransferSize = size;

  for (i = 0; i < size; i++)
  {
    TxBuffer[i] = buffer[i];
  }

  sfrPageSave = SFRPAGE;
  SFRPAGE = LEGACY_PAGE;

  // CS setup time
  // PCA capture compare at end of setup time

  BSP_DISP_CS = SPI_CS_ASSERT_LVL;
  // PCA capture compare at end of hold time
  Next_Compare_Value_CH1 = PCA0 + PCA_CH1_TIMEOUT;
  PCA0CPL1 = (Next_Compare_Value_CH1 & 0x00FF);
  PCA0CPH1 = (Next_Compare_Value_CH1 & 0xFF00)>>8;
  // Activate capture compare flag
  PCA0CPM1 |= PCA0CPM1_ECCF__BMASK;

  SFRPAGE = sfrPageSave;
}

/////////////////////////////////////////////////////////////////////////////
// Interrupt Service Handlers
/////////////////////////////////////////////////////////////////////////////

void SPI0_transferCompleteCb(void)
{
  uint8_t sfrPageSave;

  // CS hold time
  TransferState = ST_CS_HOLD;

  sfrPageSave = SFRPAGE;
  SFRPAGE = LEGACY_PAGE;
  // PCA capture compare at end of hold time
  Next_Compare_Value_CH1 = PCA0 + PCA_CH1_TIMEOUT;
  PCA0CPL1 = (Next_Compare_Value_CH1 & 0x00FF);
  PCA0CPH1 = (Next_Compare_Value_CH1 & 0xFF00)>>8;
  // Activate capture compare flag
  PCA0CPM1 |= PCA0CPM1_ECCF__BMASK;

  SFRPAGE = sfrPageSave;
}

//---------------------------------------------------------------------------
// GetTickCount
//---------------------------------------------------------------------------
//
// Description - Return the system up time in milliseconds
//
// return - Number of milliseconds since system start.
//---------------------------------------------------------------------------

uint16_t GetTickCount()
{
  uint16_t ticks;
  // Deactivate capture compare flag
  PCA0CPM0 &= ~(PCA0CPM0_ECCF__BMASK);
  ticks = Ticks;
  PCA0CPM0 |= PCA0CPM0_ECCF__BMASK;
  return ticks;
}

//---------------------------------------------------------------------------
// Wait
//---------------------------------------------------------------------------
//
// Description - Wait the specified number of milliseconds
//
// ms - The number of milliseconds to wait
//---------------------------------------------------------------------------
void Wait(uint16_t ms)
{
  uint16_t ticks = GetTickCount();

  while ((GetTickCount() - ticks) < ms);
}

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// PCA0 ISR Content goes here. Remember to clear flag bits:
// PCA0CN0::CCF0 (PCA Module 0 Capture/Compare Flag)
// PCA0CN0::CCF1 (PCA Module 1 Capture/Compare Flag)
// PCA0CN0::CCF2 (PCA Module 2 Capture/Compare Flag)
// PCA0CN0::CF (PCA Counter/Timer Overflow Flag)
// PCA0PWM::COVF (Cycle Overflow Flag)
//
// This is the ISR for the PCA.  It handles the case when a match occurs on
// channel 0, and updates the PCA0CPn compare register with the value held in
// the global variable "CEX0_Compare_Value".
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
  if(PCA0CN0_CCF2)
  {
	PCA0CN0_CCF2 = 0;                           // Clear module 2 interrupt flag
	Time_50us++;
	Next_Compare_Value_CH2 += PCA_CH2_TIMEOUT;
	PCA0CPL2 = (Next_Compare_Value_CH2 & 0x00FF);
	PCA0CPH2 = (Next_Compare_Value_CH2 & 0xFF00)>>8;
  }

  else if (PCA0CN0_CCF0)                   // If Module 0 caused the interrupt
  {
	PCA0CN0_CCF0 = 0;                           // Clear module 0 interrupt flag.
    Ticks++;
    Next_Compare_Value_CH0 += PCA_CH0_TIMEOUT;
    PCA0CPL0 = (Next_Compare_Value_CH0 & 0x00FF);
    PCA0CPH0 = (Next_Compare_Value_CH0 & 0xFF00)>>8;
  }

  else if(PCA0CN0_CCF1)
  {
	PCA0CN0_CCF1 = 0;                           // Clear module 1 interrupt flag.
    // CS setup complete
    if (TransferState == ST_CS_SETUP)
    {
      TransferState = ST_TX;
      SPI0_transfer(TxBuffer, NULL, SPI0_TRANSFER_TX, TransferSize);
    }
    // CS hold complete
    else if (TransferState == ST_CS_HOLD)
    {
      // Deassert CS
      BSP_DISP_CS = SPI_CS_DEASSERT_LVL;
      // Transfer complete
      TransferState = ST_IDLE;
    }
    // CS hold time
    // Deactivate capture compare flag
    PCA0CPM1 &= ~(PCA0CPM1_ECCF__BMASK);
  }
}

