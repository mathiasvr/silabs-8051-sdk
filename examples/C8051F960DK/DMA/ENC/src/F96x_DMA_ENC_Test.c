//=============================================================================
// F96x_DMA_ENC_Test.c
//=============================================================================
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the use of the built-in DMA-enabled Encoder module
// ENC0, which can encode/decode data using Manchester or Three Out of Six
// encoding directly from and to XRAM.
//
// The test code in this module will test the four permutations of
// the two possible modes (encoding/decoding) and two possible
// encoding methods (Manchester/Three out of Six).
//
// The test vectors are located in the test Vectors.c module.
//
// This simple test code will hang just after the respective test
// if any test fails. If all tests pass, it will run to the end of the
// program and turn on LED1.
//
// How To Test:
//
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Connect the USB Debug Adapter to J13.
// 3) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 4) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 5) Connect a 9V power adapter to P1 of the development board.
// 6) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) LED1 should be on if the program encoded/decoded the test data correctly.
//
//
// Target:         C8051F96x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Updated Description / How to Test
//    - Added an LED indicator for success/failure.
//    - 24 JAN 2014
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
#include "EncoderDecoder.h"
#include "TestVectors.h"

//-----------------------------------------------------------------------------
// Define Hardware
//-----------------------------------------------------------------------------
#define UDP_F960_MCU_MUX_LCD
//#define UDP_F960_MCU_EMIF
//-----------------------------------------------------------------------------
// Hardware Dependent definitions
//-----------------------------------------------------------------------------
#ifdef UDP_F960_MCU_MUX_LCD
SI_SBIT(LED1, SFR_P0, 0);
#endif

#ifdef UDP_F960_MCU_EMIF
SI_SBIT(LED1, SFR_P3, 0);
#endif

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define LED_ON           0
#define LED_OFF          1

//=============================================================================
// File global variables
//
// Reference Key and UnencodedData Copied to global variables located in XRAM.
//
//=============================================================================
SI_SEGMENT_VARIABLE(UnencodedData[32], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(EncodedData[16*3], uint8_t, SI_SEG_XDATA);
//=============================================================================
// Function Prototypes
//=============================================================================
//-----------------------------------------------------------------------------
// Interrupt proto (for SDCC compatibility)
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(DMA_ISR, DMA0_IRQn);
//-----------------------------------------------------------------------------
// Functions used to initialize and verify arrays.
//-----------------------------------------------------------------------------

void initUnencodedData (SI_VARIABLE_SEGMENT_POINTER(unencodedData, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceUnencodedData, uint8_t, SI_SEG_CODE), uint8_t bytes);

uint8_t verifyUnencodedData (SI_VARIABLE_SEGMENT_POINTER(unencodedData, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceUnencodedData, uint8_t, SI_SEG_CODE), uint8_t bytes);

void initEncodedData (SI_VARIABLE_SEGMENT_POINTER(encodedData, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceEncodedData, uint8_t, SI_SEG_CODE), uint8_t bytes);

uint8_t verifyEncodedData (SI_VARIABLE_SEGMENT_POINTER(encodedData, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceEncodedData, uint8_t, SI_SEG_CODE), uint8_t bytes);


//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
   PCA0MD  &= ~0x40;                   // disable watchdog timer
}
 
//=============================================================================
// main
//=============================================================================
//-----------------------------------------------------------------------------
// main()
//
// Parameters: none
// Returns: none
//
// Description:
//
// This main module program serves as a code example for the ENCO reusable code
// modules and also validates all ENC0 operations.
//
// This reusable code modules for the ENC0 Encoder/Decoder is:
// EncoderDecoder.c
// AES_BlockCipher.c
// CBC_EncryptDecrypt.c
// CTR_EncryptDecrypt.c
//
// The test code in this module will test the four permutations of
// the two possible modes (encoding/decoding) and two possible
// encoding methods (Manchester/Three out of Six).
//
// The test vectors are located in the test Vectors.c module.
//
// Steps to use:
// Compile and download the code. Run code. If LED1 is not on, halt
// debugger and check code location.
//
// This simple test code will hang just after the respective test
// if any test fails. If all tests pass, it will run to the end of the
// program and turn on LED1.
//-----------------------------------------------------------------------------
void main (void)
{
   uint8_t status;


   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
   LED1 = LED_OFF;

   IE_EA = 1;                             // enable global interrupts

   // Validate Manchester Encoding
   initUnencodedData(UnencodedData, ManchesterUnencodedData, 16);

   status = EncodeDecode(MANCHESTER_ENCODE, 16, UnencodedData, EncodedData);

   while(status);                      // code will hang here on error.

   status = verifyEncodedData(EncodedData, ManchesterEncodedData, 32);

   while(status);                      // code will hang here on error.

   // Validate Three Out of Six Encoding
   initUnencodedData(UnencodedData, ThreeOutOfSixUnencodedData, 32);

   status = EncodeDecode(THREEOUTOFSIX_ENCODE, 32, UnencodedData, EncodedData);

   while(status);                      // code will hang here on error.

   status = verifyEncodedData(EncodedData, ThreeOutOfSixEncodedData, 48);

   while(status);                      // code will hang here on error.

   // Validate Manchester Decoding
   initEncodedData(EncodedData, ManchesterEncodedData, 32);

   status = EncodeDecode(MANCHESTER_DECODE, 32, UnencodedData, EncodedData);

   while(status);                      // code will hang here on error.

   status = verifyUnencodedData(UnencodedData, ManchesterUnencodedData, 16);

   while(status);                      // code will hang here on error.

    // Validate Three Out of Six Decoding
   initEncodedData(EncodedData, ThreeOutOfSixEncodedData, 48);

   status = EncodeDecode(THREEOUTOFSIX_DECODE, 48, UnencodedData, EncodedData);

   while(status);                      // code will hang here on error.

   status = verifyUnencodedData(UnencodedData, ThreeOutOfSixUnencodedData, 32);

   while(status);                      // code will hang here on error.

   LED1 = LED_ON;                      //turn the LED on to indicate success.

   while(1);
}
//-----------------------------------------------------------------------------
// DMA_ISR
// description:
//
// This ISR is needed to support the DMA Idle mode wake up, which is used
// in the AES functions. Bit 5 of EIE2 should be enabled before going into
// idle mode. This ISR will disable further interrupts. IE_EA must also be
// enabled.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(DMA_ISR, DMA0_IRQn)
{
  EIE2 &= ~0x20;                       // disable further interrupts
}
//-----------------------------------------------------------------------------
// initUnencodedData()
//
// parameters:
//   unencodedData          - xdata pointer to key
//   referenceUnencodedData - code pointer to reference key
//   blocks             - the number of 16-byte blocks to copy
//
// returns:
//
// description:
//
// Copies reference UnencodedData in Flash to unencodedtData in xdata RAM.
// The unencodedData must be copied to xdata for use by the DMA.
//
//-----------------------------------------------------------------------------
void initUnencodedData (SI_VARIABLE_SEGMENT_POINTER(unencodedData, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceUnencodedData, uint8_t, SI_SEG_CODE),
   uint8_t bytes)
{
   while(bytes--)
   {
      *unencodedData++ = *referenceUnencodedData++;
   }
}
//-----------------------------------------------------------------------------
// verifyUnencodedData()
//
// parameters:
//   unencodedData          - xdata pointer to unencodedData
//   referenceUnencodedData - code pointer to reference UnencodedData
//   blocks             - the number of 16-byte blocks to copy
//
// returns:
//    badUnencodedDataBytes - the number of incorrect UnencodedData bytes
//
// description:
//
// Compares unencodedData in RAM to reference UnencodedData stored in Flash.
// The return value should be zero if the UnencodedDatas are identical.
//
//
//-----------------------------------------------------------------------------
uint8_t verifyUnencodedData (SI_VARIABLE_SEGMENT_POINTER(unencodedData, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceUnencodedData, uint8_t, SI_SEG_CODE),
   uint8_t bytes)
{
   uint8_t badUnencodedDataBytes;

   badUnencodedDataBytes=0;

   while(bytes--)
   {
      if(*unencodedData++ != *referenceUnencodedData++)
         badUnencodedDataBytes++;
   }

   return badUnencodedDataBytes;
}
//-----------------------------------------------------------------------------
// initEncodedData()
//
// parameters:
//   cipherData          - xdata pointer to cipherData
//   referenceCipherData - code pointer to CipherData
//   blocks              - the number of 16-byte blocks to copy
//
// returns:
//
// description:
//
// Copies reference CipherData in Flash to cipherData in xdata RAM.
// The cipherData must be copied to xdata for use by the DMA.
//
//-----------------------------------------------------------------------------
void initEncodedData (SI_VARIABLE_SEGMENT_POINTER(encodedData, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceEncodedData, uint8_t, SI_SEG_CODE),
   uint8_t bytes)
{
   while(bytes--)
   {
      *encodedData++ = *referenceEncodedData++;
   }
}
//-----------------------------------------------------------------------------
// verifyEncodedData()
//
// parameters:
//   cipherData          - xdata pointer to cipherData
//   referenceCipherData - code pointer to reference CipherData
//   blocks             - the number of 16-byte blocks to copy
//
// returns:
//    badCipherDataBytes - the number of incorrect CipherData bytes
//
// description:
//
// Compares cipherData in RAM to reference CipherData stored in Flash.
// The return value should be zero if the CipherDatas are identical.
//
//
//-----------------------------------------------------------------------------
uint8_t verifyEncodedData (SI_VARIABLE_SEGMENT_POINTER(encodedData, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceEncodedData, uint8_t, SI_SEG_CODE),
   uint8_t bytes)
{
   uint8_t badEncodedDataBytes;

   badEncodedDataBytes=0;

   while(bytes--)
   {
      if(*encodedData++ != *referenceEncodedData++)
         badEncodedDataBytes++;
   }

   return badEncodedDataBytes;
}

//=============================================================================
// End of file
//=============================================================================

