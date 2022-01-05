#ifndef TEST_VECTORS_H
#define TEST_VECTORS_H
//=============================================================================
// TestVectors.h
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
//    C8051F960 AES Code Examples
//
// This software must be used in accordance with the End User License Agreement.
//
//=============================================================================
#ifndef COMPILER_DEFS_H
#endif
//=============================================================================
// extern'ed Public variables
//=============================================================================
extern SI_SEGMENT_VARIABLE(ReferencePlainText[64], uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ReferenceEncryptionKey128[16], uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ReferenceEncryptionKey192[24] , uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ReferenceEncryptionKey256[32] , uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ReferenceDecryptionKey128[16], uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ReferenceDecryptionKey192[24] , uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ReferenceDecryptionKey256[32] , uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ReferenceCipherText_ECB_128[64], uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ReferenceCipherText_ECB_192[64], uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ReferenceCipherText_ECB_256[64], uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ReferenceInitialVector[16] , uint8_t, code);
extern SI_SEGMENT_VARIABLE(ReferenceCipherText_CBC_128[64] , uint8_t, code);
extern SI_SEGMENT_VARIABLE(ReferenceCipherText_CBC_192[64] , uint8_t, code);
extern SI_SEGMENT_VARIABLE(ReferenceCipherText_CBC_256[64] , uint8_t, code);
extern SI_SEGMENT_VARIABLE(Nonce[16], uint8_t, code);
extern SI_SEGMENT_VARIABLE(ReferenceCipherText_CTR_128[64], uint8_t, code);
extern SI_SEGMENT_VARIABLE(ReferenceCipherText_CTR_192[64], uint8_t, code);
extern SI_SEGMENT_VARIABLE(ReferenceCipherText_CTR_256[64], uint8_t, code);
//=============================================================================
// End of file
//=============================================================================
#endif  // #ifdef TEST_VECTORS_H
//=============================================================================
