//-----------------------------------------------------------------------------
// F96x_AES_Test_main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the use of the built-in AES encryption engine,
// using DMA to write directly to and from XRAM. This example has functions for
// encrypting and decrypting from XRAM using 128, 192, and 256 bit encryption
// in ECB (Electronic Code-Book), CBC (Cipher Block Chaining), and CTR (Counter)
// modes.
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
// 8) LED1 should be on if the program generated AES keys and
//    encrypted/decrypted the test string successfully in all modes.
//
//
// Target:         C8051F96x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Updated Description / How to Test
//    - Added an LED indicator for a successful test.
//    - Modified the test string.
//    - 24 JAN 2014
//
//
//-----------------------------------------------------------------------------
// uncomment pragma SRC to generate assemble code
//-----------------------------------------------------------------------------
//#pragma SRC
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F960_Register_Enums.h>
#include "GenerateDecryptionKey.h"
#include "AES_BlockCipher.h"
#include "CBC_EncryptDecrypt.h"
#include "CTR_EncryptDecrypt.h"
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
// Reference Key and PlainText Copied to global variables located in XRAM.
//
//=============================================================================
SI_SEGMENT_VARIABLE(EncryptionKey[32], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(DecryptionKey[32], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(PlainText[64], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(CipherText[64], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(InitialVector[16], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(Counter[16], uint8_t, SI_SEG_XDATA);
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

void initKey (SI_VARIABLE_SEGMENT_POINTER(key, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceKey, uint8_t, SI_SEG_CODE), uint8_t keyLength);

uint8_t verifyKey (SI_VARIABLE_SEGMENT_POINTER(key, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceKey, uint8_t, SI_SEG_CODE), uint8_t keyLength);

void initPlainText (SI_VARIABLE_SEGMENT_POINTER(plainText, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referencePlainText, uint8_t, SI_SEG_CODE), uint8_t blocks);

uint8_t verifyPlainText (SI_VARIABLE_SEGMENT_POINTER(plainText, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referencePlainText, uint8_t, SI_SEG_CODE), uint8_t blocks);

void initCipherText (SI_VARIABLE_SEGMENT_POINTER(cipherText, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceCipherText, uint8_t, SI_SEG_CODE), uint8_t blocks);

uint8_t verifyCipherText (SI_VARIABLE_SEGMENT_POINTER(key, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceCipherText, uint8_t, SI_SEG_CODE), uint8_t blocks);

void initInitialVector (SI_VARIABLE_SEGMENT_POINTER(initialVector, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceInitialVector, uint8_t, SI_SEG_CODE));

void initCounter (SI_VARIABLE_SEGMENT_POINTER(counter, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(nonce, uint8_t, SI_SEG_CODE));

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
void main (void)
{
   uint8_t status;


   SFRPAGE = LEGACY_PAGE;

   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups

   IE_EA = 1;                             // enable global interrupts

   // Validate 128-bit key inversion
   initKey(EncryptionKey, ReferenceEncryptionKey128, 16);

   status = GenerateDecryptionKey(EncryptionKey, DecryptionKey, KEY_SIZE_128_BIT);

   while(status);                      // code will hang here on error.

   status = verifyKey(DecryptionKey, ReferenceDecryptionKey128, 16);

   while(status);                      // code will hang here on error.

   // Validate 192-bit key inversion
   initKey(EncryptionKey, ReferenceEncryptionKey192, 24);

   status = GenerateDecryptionKey(EncryptionKey, DecryptionKey, KEY_SIZE_192_BIT);

   while(status);                      // code will hang here on error.

   status = verifyKey(DecryptionKey, ReferenceDecryptionKey192, 24);

   while(status);                      // code will hang here on error.

   // Validate 256-bit key inversion
   initKey(EncryptionKey, ReferenceEncryptionKey256, 32);

   status = GenerateDecryptionKey (EncryptionKey, DecryptionKey, KEY_SIZE_256_BIT);

   while(status);                      // code will hang here on error.

   status = verifyKey(DecryptionKey, ReferenceDecryptionKey256, 32);

   while(status);                      // code will hang here on error.

   // Validate 128-bit AES block cipher encryption
   initPlainText(PlainText, ReferencePlainText, 4);

   initKey(EncryptionKey, ReferenceEncryptionKey128, 16);

   status = AES_BlockCipher (ENCRYPTION_128_BIT, PlainText, CipherText, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyCipherText(CipherText, ReferenceCipherText_ECB_128, 4);

   while(status);                      // code will hang here on error.

   // Validate 192-bit AES block cipher encryption
   initPlainText(PlainText, ReferencePlainText, 4);

   initKey(EncryptionKey, ReferenceEncryptionKey192, 24);

   status = AES_BlockCipher (ENCRYPTION_192_BIT, PlainText, CipherText, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyCipherText(CipherText, ReferenceCipherText_ECB_192, 4);

   while(status);                      // code will hang here on error.

   // Validate 256-bit AES block cipher encryption
   initPlainText(PlainText, ReferencePlainText, 4);

   initKey(EncryptionKey, ReferenceEncryptionKey256, 32);

   status = AES_BlockCipher (ENCRYPTION_256_BIT, PlainText, CipherText, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyCipherText(CipherText, ReferenceCipherText_ECB_256, 4);

   while(status);                      // code will hang here on error.

   // Validate 128-bit AES block cipher decryption
   initCipherText(CipherText, ReferenceCipherText_ECB_128, 4);

   initKey(DecryptionKey, ReferenceDecryptionKey128, 16);

   status = AES_BlockCipher (DECRYPTION_128_BIT, PlainText, CipherText, DecryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyPlainText(PlainText, ReferencePlainText, 4);

   while(status);                      // code will hang here on error.

   // Validate 192-bit AES block cipher decryption
   initCipherText(CipherText, ReferenceCipherText_ECB_192, 4);

   initKey(DecryptionKey, ReferenceDecryptionKey192, 24);

   status = AES_BlockCipher (DECRYPTION_192_BIT, PlainText, CipherText, DecryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyPlainText(PlainText, ReferencePlainText, 4);

   while(status);                      // code will hang here on error.

   // Validate 256-bit AES block cipher decryption
   initCipherText(CipherText, ReferenceCipherText_ECB_256, 4);

   initKey(DecryptionKey, ReferenceDecryptionKey256, 32);

   status = AES_BlockCipher (DECRYPTION_256_BIT, PlainText, CipherText, DecryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyPlainText(PlainText, ReferencePlainText, 4);

   while(status);                      // code will hang here on error.

   // Validate 128-bit CBC Mode encryption
   initPlainText(PlainText, ReferencePlainText, 4);

   initInitialVector(InitialVector, ReferenceInitialVector);

   initKey(EncryptionKey, ReferenceEncryptionKey128, 16);

   status = CBC_EncryptDecrypt (ENCRYPTION_128_BIT, PlainText, CipherText, InitialVector, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyCipherText(CipherText, ReferenceCipherText_CBC_128, 4);

   while(status);                      // code will hang here on error.

   // Validate 192-bit CBC Mode encryption
   initPlainText(PlainText, ReferencePlainText, 4);

   initInitialVector(InitialVector, ReferenceInitialVector);

   initKey(EncryptionKey, ReferenceEncryptionKey192, 24);

   status = CBC_EncryptDecrypt (ENCRYPTION_192_BIT, PlainText, CipherText, InitialVector, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyCipherText(CipherText, ReferenceCipherText_CBC_192, 4);

   while(status);                      // code will hang here on error.

   // Validate 256-bit CBC Mode encryption
   initPlainText(PlainText, ReferencePlainText, 4);

   initInitialVector(InitialVector, ReferenceInitialVector);

   initKey(EncryptionKey, ReferenceEncryptionKey256, 32);

   status = CBC_EncryptDecrypt (ENCRYPTION_256_BIT, PlainText, CipherText, InitialVector, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyCipherText(CipherText, ReferenceCipherText_CBC_256, 4);

   while(status);                      // code will hang here on error.

   // Validate 128-bit CBC Mode decryption
   initCipherText(CipherText, ReferenceCipherText_CBC_128, 4);

   initInitialVector(InitialVector, ReferenceInitialVector);

   initKey(DecryptionKey, ReferenceDecryptionKey128, 16);

   status = CBC_EncryptDecrypt (DECRYPTION_128_BIT, PlainText, CipherText, InitialVector, DecryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyPlainText(PlainText, ReferencePlainText, 4);

   while(status);                      // code will hang here on error.

   // Validate 192-bit CBC Mode decryption
   initCipherText(CipherText, ReferenceCipherText_CBC_192, 4);

   initInitialVector(InitialVector, ReferenceInitialVector);

   initKey(DecryptionKey, ReferenceDecryptionKey192, 24);

   status = CBC_EncryptDecrypt (DECRYPTION_192_BIT, PlainText, CipherText, InitialVector, DecryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyPlainText(PlainText, ReferencePlainText, 4);

   while(status);                      // code will hang here on error.

   // Validate 256-bit CBC Mode decryption
   initCipherText(CipherText, ReferenceCipherText_CBC_256, 4);

   initInitialVector(InitialVector, ReferenceInitialVector);

   initKey(DecryptionKey, ReferenceDecryptionKey256, 32);

   status = CBC_EncryptDecrypt (DECRYPTION_256_BIT, PlainText, CipherText, InitialVector, DecryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyPlainText(PlainText, ReferencePlainText, 4);

   while(status);                      // code will hang here on error.

   // Validate 128-bit CTR Mode encryption
   initPlainText(PlainText, ReferencePlainText, 4);

   initCounter(Counter, Nonce);

   initKey(EncryptionKey, ReferenceEncryptionKey128, 16);

   status = CTR_EncryptDecrypt (ENCRYPTION_128_BIT, PlainText, CipherText, Counter, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyCipherText(CipherText, ReferenceCipherText_CTR_128, 4);

   while(status);                      // code will hang here on error.

   // Validate 192-bit CTR Mode encryption
   initPlainText(PlainText, ReferencePlainText, 4);

   initCounter(Counter, Nonce);

   initKey(EncryptionKey, ReferenceEncryptionKey192, 24);

   status = CTR_EncryptDecrypt (ENCRYPTION_192_BIT, PlainText, CipherText, Counter, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyCipherText(CipherText, ReferenceCipherText_CTR_192, 4);

   while(status);                      // code will hang here on error.

   // Validate 256-bit CTR Mode encryption
   initPlainText(PlainText, ReferencePlainText, 4);

   initCounter(Counter, Nonce);

   initKey(EncryptionKey, ReferenceEncryptionKey256, 32);

   status = CTR_EncryptDecrypt (ENCRYPTION_256_BIT, PlainText, CipherText, Counter, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyCipherText(CipherText, ReferenceCipherText_CTR_256, 4);

   while(status);                      // code will hang here on error.

   // Validate 128-bit CTR Mode decryption
   initCipherText(CipherText, ReferenceCipherText_CTR_128, 4);

   initCounter(Counter, Nonce);

   initKey(EncryptionKey, ReferenceEncryptionKey128, 16);

   status = CTR_EncryptDecrypt (DECRYPTION_128_BIT, PlainText, CipherText, Counter, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyPlainText(PlainText, ReferencePlainText, 4);

   while(status);                      // code will hang here on error.

   // Validate 192-bit CTR Mode decryption
   initCipherText(CipherText, ReferenceCipherText_CTR_192, 4);

   initCounter(Counter, Nonce);

   initKey(EncryptionKey, ReferenceEncryptionKey192, 24);

   status = CTR_EncryptDecrypt (DECRYPTION_192_BIT, PlainText, CipherText, Counter, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyPlainText(PlainText, ReferencePlainText, 4);

   while(status);                      // code will hang here on error.

   // Validate 256-bit CTR Mode decryption
   initCipherText(CipherText, ReferenceCipherText_CTR_256, 4);

   initCounter(Counter, Nonce);

   initKey(EncryptionKey, ReferenceEncryptionKey256, 32);

   status = CTR_EncryptDecrypt (DECRYPTION_256_BIT, PlainText, CipherText, Counter, EncryptionKey, 4);

   while(status);                      // code will hang here on error.

   status = verifyPlainText(PlainText, ReferencePlainText, 4);

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
// initKey()
//
// parameters:
//   key          - xdata pointer to key
//   referenceKey - code pointer to reference key
//   keyLength    - key length in bytes
//
// returns:
//
// description:
//
// Copies reference key in Flash to key in xdata RAM.
// The key must be copied to xdata for use by the DMA.
//
//-----------------------------------------------------------------------------
void initKey (SI_VARIABLE_SEGMENT_POINTER(key, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceKey, uint8_t, SI_SEG_CODE),
   uint8_t keyLength)
{
   while(keyLength--)
   {
      *key++ = *referenceKey++;
   }
}
//-----------------------------------------------------------------------------
// verifyKey()
//
// parameters:
//   key          - xdata pointer to key
//   referenceKey - code pointer to reference key
//   keyLength    - key length in bytes
//
// returns:
//    badKeyBytes - the number of incorrect key bytes
//
// description:
//
// Compares key in RAM to reference key stored in Flash.
// The return value should be zero if the keys are identical.
//
//-----------------------------------------------------------------------------
uint8_t verifyKey (SI_VARIABLE_SEGMENT_POINTER(key, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceKey, uint8_t, SI_SEG_CODE),
   uint8_t keyLength)
{
   uint8_t badKeyBytes;

   badKeyBytes=0;

   while(keyLength--)
   {
         if(*key++ != *referenceKey++)
            badKeyBytes++;
   }

   return badKeyBytes;
}
//-----------------------------------------------------------------------------
// initPlainText()
//
// parameters:
//   plainText          - xdata pointer to key
//   referencePlainText - code pointer to reference key
//   blocks             - the number of 16-byte blocks to copy
//
// returns:
//
// description:
//
// Copies reference PlainText in Flash to plaintText in xdata RAM.
// The plainText must be copied to xdata for use by the DMA.
//
//-----------------------------------------------------------------------------
void initPlainText (SI_VARIABLE_SEGMENT_POINTER(plainText, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referencePlainText, uint8_t, SI_SEG_CODE),
   uint8_t blocks)
{
   uint8_t i;

   while(blocks--)
   {
      for(i=16;i>0;i--)
      {
         *plainText++ = *referencePlainText++;
      }
   }
}
//-----------------------------------------------------------------------------
// verifyPlainText()
//
// parameters:
//   plainText          - xdata pointer to plainText
//   referencePlainText - code pointer to reference PlainText
//   blocks             - the number of 16-byte blocks to copy
//
// returns:
//    badPlainTextBytes - the number of incorrect PlainText bytes
//
// description:
//
// Compares plainText in RAM to reference PlainText stored in Flash.
// The return value should be zero if the PlainTexts are identical.
//
//
//-----------------------------------------------------------------------------
uint8_t verifyPlainText (SI_VARIABLE_SEGMENT_POINTER(plainText, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referencePlainText, uint8_t, SI_SEG_CODE),
   uint8_t blocks)
{
   uint8_t badPlainTextBytes;
   uint8_t i;

   badPlainTextBytes=0;

   while(blocks--)
   {
      for(i=16;i>0;i--)
      {
         if(*plainText++ != *referencePlainText++)
            badPlainTextBytes++;
      }
   }

   return badPlainTextBytes;
}
//-----------------------------------------------------------------------------
// initCipherText()
//
// parameters:
//   cipherText          - xdata pointer to cipherText
//   referenceCipherText - code pointer to CipherText
//   blocks              - the number of 16-byte blocks to copy
//
// returns:
//
// description:
//
// Copies reference CipherText in Flash to cipherText in xdata RAM.
// The cipherText must be copied to xdata for use by the DMA.
//
//-----------------------------------------------------------------------------
void initCipherText (SI_VARIABLE_SEGMENT_POINTER(cipherText, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceCipherText, uint8_t, SI_SEG_CODE),
   uint8_t blocks)
{
   uint8_t i;

   while(blocks--)
   {
      for(i=16;i>0;i--)
      {
         *cipherText++ = *referenceCipherText++;
      }
   }
}
//-----------------------------------------------------------------------------
// verifyCipherText()
//
// parameters:
//   cipherText          - xdata pointer to cipherText
//   referenceCipherText - code pointer to reference CipherText
//   blocks             - the number of 16-byte blocks to copy
//
// returns:
//    badCipherTextBytes - the number of incorrect CipherText bytes
//
// description:
//
// Compares cipherText in RAM to reference CipherText stored in Flash.
// The return value should be zero if the CipherTexts are identical.
//
//
//-----------------------------------------------------------------------------
uint8_t verifyCipherText (SI_VARIABLE_SEGMENT_POINTER(cipherText, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceCipherText, uint8_t, SI_SEG_CODE),
   uint8_t blocks)
{
   uint8_t badCipherTextBytes;
   uint8_t i;

   badCipherTextBytes=0;

   while(blocks--)
   {
      for(i=16;i>0;i--)
      {
         if(*cipherText++ != *referenceCipherText++)
            badCipherTextBytes++;
      }
   }

   return badCipherTextBytes;
}

//-----------------------------------------------------------------------------
// initInitialVector()
//
// parameters:
//   initialVector          - xdata pointer to initialVector
//   referenceInitialVector - code pointer to referenceInitialVector
//
// returns:
//
// description:
//
// Copies reference InitialVector in Flash to InitialVector in xdata RAM.
// The InitialVector must be copied to xdata for use by the DMA.
//
//-----------------------------------------------------------------------------
void initInitialVector (SI_VARIABLE_SEGMENT_POINTER(initialVector, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(referenceInitialVector, uint8_t, SI_SEG_CODE))
{
   uint8_t i;

   for(i=16;i>0;i--)
   {
      *initialVector++ = *referenceInitialVector++;
   }
}
//-----------------------------------------------------------------------------
// initCounter()
//
// parameters:
//   counter      - xdata pointer to counter
//   nonce        - code pointer to nonce
//
// returns:
//
// description:
//
// Copies nonce in Flash to counter in xdata RAM.
// the nonce is a number used once. It is used for the initial value of the
// counter. The counter must be stored to xdata for use by the DMA.
//
//-----------------------------------------------------------------------------
void initCounter (SI_VARIABLE_SEGMENT_POINTER(counter, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(nonce, uint8_t, SI_SEG_CODE))
{
   uint8_t i;

   for(i=16;i>0;i--)
   {
      *counter++ = *nonce++;
   }
}
//=============================================================================
// End of file
//=============================================================================

