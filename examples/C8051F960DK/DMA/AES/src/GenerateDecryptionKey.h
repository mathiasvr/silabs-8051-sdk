#ifndef GENERATE_DECRYPTION_KEY_H
#define GENERATE_DECRYPTION_KEY_H
//=============================================================================
// GenerateDecryptionKey.h
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
#endif
//-----------------------------------------------------------------------------
// defines used with keysize parameter
// Also defined in AES_defs.h
//-----------------------------------------------------------------------------
#ifndef AES_DEFS_H
#define KEY_SIZE_128_BIT   0x00
#define KEY_SIZE_192_BIT   0x01
#define KEY_SIZE_256_BIT   0x02
#endif
//-----------------------------------------------------------------------------
// typedefs for key size and status
//-----------------------------------------------------------------------------
typedef uint8_t GENERATE_DECRYPTION_KEY_SIZE;
typedef uint8_t GENERATE_DECRYPTION_KEY_STATUS;
//=============================================================================
// Function Prototypes (API)
//=============================================================================
GENERATE_DECRYPTION_KEY_STATUS GenerateDecryptionKey (
   SI_VARIABLE_SEGMENT_POINTER(encryptionKey, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(decryptionKey, uint8_t, SI_SEG_XDATA),
   uint8_t keySize);
//=============================================================================
// End of file
//=============================================================================
#endif  // #ifdef GENERATE_DECRYPTION_KEY_H
//=============================================================================
