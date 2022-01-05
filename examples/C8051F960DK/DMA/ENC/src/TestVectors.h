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
//    C8051F960 Code Examples
//
// This software must be used in accordance with the End User License Agreement.
//
//=============================================================================
#ifndef COMPILER_DEFS_H
#endif
//=============================================================================
// Defines used with Table
//=============================================================================
#define NUM_MANCHESTER_VECTORS               16
#define NUM_MANCHESTER_UNENCODED_BYTES       (NUM_MANCHESTER_VECTORS)
#define NUM_MANCHESTER_ENCODED_BYTES         (NUM_MANCHESTER_VECTORS*2)
#define NUM_THREEOUTOFSIX_VECTORS            16
#define NUM_THREEOUTOFSIX_UNENCODED_BYTES    (NUM_THREEOUTOFSIX_VECTORS*2)
#define NUM_THREEOUTOFSIX_ENCODED_BYTES      (NUM_THREEOUTOFSIX_VECTORS*3)
//=============================================================================
// extern'ed Public variables
//=============================================================================
extern SI_SEGMENT_VARIABLE(ManchesterUnencodedData[NUM_MANCHESTER_UNENCODED_BYTES],
			 uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ManchesterEncodedData[NUM_MANCHESTER_ENCODED_BYTES],
			 uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ThreeOutOfSixUnencodedData[NUM_THREEOUTOFSIX_UNENCODED_BYTES],
			 uint8_t, SI_SEG_CODE);
extern SI_SEGMENT_VARIABLE(ThreeOutOfSixEncodedData[NUM_THREEOUTOFSIX_ENCODED_BYTES],
			 uint8_t, SI_SEG_CODE);
//=============================================================================
// End of file
//=============================================================================
#endif  // #ifdef TEST_VECTORS_H
//=============================================================================
