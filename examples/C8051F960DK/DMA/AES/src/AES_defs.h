//-----------------------------------------------------------------------------
//AES_defs.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// DMA definitions for F96x family with DMA.
//
// Target:         C8051F96x
// Tool chain:     Keil, SDCC
// Command Line:   None
//
//-----------------------------------------------------------------------------
// Include compiler_defs.h if not already defined.
//-----------------------------------------------------------------------------
#ifndef COMPILER_DEFS_H
#include "si_toolchain.h"
#endif
//-----------------------------------------------------------------------------
// Header file applied only if not already defined.
//-----------------------------------------------------------------------------
#ifndef AES_DEFS_H
#define AES_DEFS_H
//-----------------------------------------------------------------------------
// enum used for KEY_SIZE
//-----------------------------------------------------------------------------
enum KEY_SIZE_Enum
{
   KEY_SIZE_128_BITS = 0,             // 0x00
   KEY_SIZE_192_BITS,                 // 0x01
   KEY_SIZE_256_BITS,                 // 0x02
   KEY_SIZE_UNDEFINED                 // 0x03
};
//-----------------------------------------------------------------------------
// defines used for AES0BCF
//-----------------------------------------------------------------------------
#define KEY_SIZE_128_BIT         0x00
#define KEY_SIZE_192_BIT         0x01
#define KEY_SIZE_256_BIT         0x02
#define DECRYPTION_MODE          0x00
#define ENCRYPTION_MODE          0x04
#define DECRYPTION_128_BIT       0x00
#define DECRYPTION_192_BIT       0x01
#define DECRYPTION_256_BIT       0x02
#define ENCRYPTION_128_BIT       0x04
#define ENCRYPTION_192_BIT       0x05
#define ENCRYPTION_256_BIT       0x06
#define AES_ENABLE               0x08
#define DECRYPTION_UNDEFINED     0x03
#define ENCRYPTION_UNDEFINED     0x07
//-----------------------------------------------------------------------------
// defines used for AES0DCF
//-----------------------------------------------------------------------------
#define AES_OUTPUT               0x00
#define XOR_ON_INPUT             0x01
#define XOR_ON_OUTPUT            0x02
#define INVERSE_KEY              0x04
//-----------------------------------------------------------------------------
// Define SUCCESS and ERROR_INVALID_PARAMETER return codes
//-----------------------------------------------------------------------------
#ifndef SUCCESS
#define SUCCESS 0
#elif(SUCCESS!=0)
#error  "SUCCESS definition conflict!"
#endif
#ifndef ERROR_INVALID_PARAMETER
#define ERROR_INVALID_PARAMETER 1
#elif(ERROR_INVALID_PARAMETER!=1)
#error  "ERROR_INVALID_PARAMETER definition conflict!"
#endif
//-----------------------------------------------------------------------------
// End AES_defs.h
//-----------------------------------------------------------------------------
#endif                                 // AES_DEFS_H
