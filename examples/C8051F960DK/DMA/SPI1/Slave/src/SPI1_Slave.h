#ifndef SPI1_SLAVE_H
#define SPI1_SLAVE_H
//=============================================================================
// SPI1_Master.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// C File Description:
//
//
// Target:
//    C8051F960
//
// IDE:
//    Silicon Laboratories IDE
//
// Tool Chains:
//    Keil
//    SDCC
//    Raisonance
//
// Project Name:
//    C8051F960 Code Examples
//
// This software must be used in accordance with the End User License Agreement.
//
//=============================================================================
#ifndef COMPILER_DEFS_H
#endif
//=============================================================================
extern bit SPI_SlaveTransferDone;
//=============================================================================
// Function Prototypes (API)
//=============================================================================
void SPI1_SlaveOutIn (uint16_t size,
   SI_VARIABLE_SEGMENT_POINTER(slaveOutPointer, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(slaveInPointer, uint8_t, SI_SEG_XDATA));

//=============================================================================
// End of file
//=============================================================================
#endif  // #ifdef SPI1_MASTER_H
//=============================================================================
