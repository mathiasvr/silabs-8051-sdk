#ifndef CBC_ENCRYPT_DECRYPT_H
#define CBC_ENCRYPT_DECRYPT_H
//=============================================================================
// CBC_EncryptDecrypt.h
//=============================================================================
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
//    C8051F960 AES Library
//
// This software must be used in accordance with the End User License Agreement.
//
//=============================================================================
#ifndef COMPILER_DEFS_H
#include "si_toolchain.h"
#endif
//=============================================================================
//-----------------------------------------------------------------------------
// MAX 16 bytes blocks for F960 xram
//-----------------------------------------------------------------------------
#ifndef MAX_BLOCKS
#define MAX_BLOCKS      4096
#endif
//-----------------------------------------------------------------------------
// defines used with operation parameter
// Also defined in AES_defs.h
//-----------------------------------------------------------------------------
#ifndef AES_DEFS_H
#define DECRYPTION_128_BIT    0x00
#define DECRYPTION_192_BIT    0x01
#define DECRYPTION_256_BIT    0x02
#define ENCRYPTION_128_BIT    0x04
#define ENCRYPTION_192_BIT    0x05
#define ENCRYPTION_256_BIT    0x06
#define DECRYPTION_MODE       0x00
#define ENCRYPTION_MODE       0x04
#endif
//-----------------------------------------------------------------------------
// typedefs for operation and status
//-----------------------------------------------------------------------------
typedef uint8_t CBC_ENCRYPT_DECRYPT_OPERATION;
typedef uint8_t CBC_ENCRYPT_DECRYPT_STATUS;
//=============================================================================
// Function Prototypes (API)
//=============================================================================
CBC_ENCRYPT_DECRYPT_STATUS CBC_EncryptDecrypt (uint8_t operation,
   SI_VARIABLE_SEGMENT_POINTER(plainText, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(cipherText, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(initialVector, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(key, uint8_t, SI_SEG_XDATA),
   uint16_t blocks);
//=============================================================================
// End of file
//=============================================================================
#endif  // #ifdef CBC_ENCRYPT_DECRYPT_H
//=============================================================================
