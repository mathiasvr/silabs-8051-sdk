//=============================================================================
// TestVectors.c
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
//-----------------------------------------------------------------------------
// uncomment pragma SRC to generate assemble code
//-----------------------------------------------------------------------------
//#pragma SRC
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F960_Register_Enums.h>
#include "CRC1_TestVectors.h"
//-----------------------------------------------------------------------------
// CRC test vectors from mbus spec
//-----------------------------------------------------------------------------
const SI_SEGMENT_VARIABLE(BlockOne[10], uint8_t, SI_SEG_CODE) =
{
   0x0F, 0x44, 0xAE, 0x0C, 0x78, 0x56, 0x34, 0x12, 0x01, 0x07
};
//--------------------------------------------------------------------------------------------------
const SI_SEGMENT_VARIABLE(BlockTwo[6], uint8_t, SI_SEG_CODE) =
{
   0x78, 0x0B, 0x13, 0x43, 0x65, 0x87
};
//=============================================================================
// End of File
//=============================================================================
