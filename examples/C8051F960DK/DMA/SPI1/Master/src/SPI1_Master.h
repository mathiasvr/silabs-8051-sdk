#ifndef SPI1_MASTER_H
#define SPI1_MASTER_H
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
// Function Prototypes (API)
//=============================================================================
void SPI1_MasterOutIn (uint16_t length,
   SI_VARIABLE_SEGMENT_POINTER(outPointer, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(inPointer, uint8_t, SI_SEG_XDATA));

void SPI1_MasterOutOnly (uint16_t size,
   SI_VARIABLE_SEGMENT_POINTER(outPointer, uint8_t, SI_SEG_XDATA));

//=============================================================================
#endif  // #ifdef SPI1_MASTER_H
//=============================================================================
// End of file
//=============================================================================
