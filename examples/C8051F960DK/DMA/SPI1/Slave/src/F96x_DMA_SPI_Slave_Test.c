//=============================================================================
// F96x_DMA_SPI_SlaveTest.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This file will test the SPI1 Slave mode using the DMA to transmit packets to
// and from XRAM.
//
// To use connect P0.0-P0.3 to P2.0-P2.3. This will connect SPI1 to SPI0.
// SPI1 is used in slave mode with the DMA. SPI0 is used in Master mode
// using polling.
//
// The MISO data is copied to XRAM for the DMA. This data will be sent
// out by the slave as the the Slave receives MOSI data.
//
// The SPI1_SlaveNext function will prepare the DMA for the next SPI
// transfer.
//
// Then SPI0 is used to send data over the SPI connection. After sending
// all the MOSI data bytes, the DMA interrupt should occur.
//
// After a successful SPI transfer the MOSI data should be in the
// SPI1_SlaveInBuffer and the MISO data should be in the
// SPI0_MasterInBuffer
//
// How To Test:
//
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Make the following connections to connect SPI0 to SPI1:
//       P0.0 <-> P2.0
//       P0.1 <-> P2.1
//       P0.2 <-> P2.2
//       P0.3 <-> P2.3
//
// 3) Connect the USB Debug Adapter to J13.
// 4) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 5) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 6) Connect a 9V power adapter to P1 of the development board.
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) LED1 will blink slowly if the packets were transferred correctly.
//
//
// Target:         C8051F96x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Updated Description / How to Test
//    - Added an LED indicator for success/failure.
//    - 27 JAN 2014
//
//=============================================================================
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F960_Register_Enums.h>
#include "SPI1_Slave.h"
#include "DMA_defs.h"

//-----------------------------------------------------------------------------
// Define Hardware
//-----------------------------------------------------------------------------
//#define UDP_F960_MCU_MUX_LCD
#define UDP_F960_MCU_EMIF
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

//-----------------------------------------------------------------------------
// DEFINE HARDWARE BITS
//-----------------------------------------------------------------------------
SI_SBIT(NSS0, SFR_P0, 3);
//=============================================================================
// Test Packet
//=============================================================================
#define TEST_PACKET_LENGTH 16
const SI_SEGMENT_VARIABLE(MISO_Data[TEST_PACKET_LENGTH], uint8_t, SI_SEG_CODE) =
{
   0xA0, 0xA1, 0xA2, 0xA3,
   0xA4, 0xA5, 0xA6, 0xA7,
   0xA8, 0xA9, 0xAA, 0xAB,
   0xAC, 0xAD, 0xAE, 0xAF,
};

const SI_SEGMENT_VARIABLE(MOSI_Data[TEST_PACKET_LENGTH], uint8_t, SI_SEG_CODE) =
{
   0xBF, 0xBE, 0xBD, 0xBC,
   0xBB, 0xBA, 0xB9, 0xB8,
   0xB7, 0xB6, 0xB5, 0xB4,
   0xB3, 0xB2, 0xB1, 0xB0,
};

//=============================================================================
// File global variables
//=============================================================================
//-----------------------------------------------------------------------------
// Global arrays for data stored in xram
//-----------------------------------------------------------------------------
SI_SEGMENT_VARIABLE(SPI1_SlaveOutBuffer[TEST_PACKET_LENGTH], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(SPI1_SlaveInBuffer[TEST_PACKET_LENGTH], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(SPI0_MasterInBuffer[TEST_PACKET_LENGTH], uint8_t, SI_SEG_XDATA);
//=============================================================================
// Function Prototypes
//=============================================================================void
void SYSCLK_Init (void);
void SPI_Init(void);
void PortInit(void);
void SPI_Disable(void);


void memCopyCX(uint16_t n,
   const SI_VARIABLE_SEGMENT_POINTER(cPointer, uint8_t, SI_SEG_CODE),
   SI_VARIABLE_SEGMENT_POINTER(xPointer, uint8_t, SI_SEG_XDATA));

void SPI0_MasterOutIn (uint16_t length,
   const SI_VARIABLE_SEGMENT_POINTER(writePointer, uint8_t, SI_SEG_CODE),
   SI_VARIABLE_SEGMENT_POINTER(readPointer, uint8_t, SI_SEG_XDATA));

uint16_t countErrors (uint16_t n,
   SI_VARIABLE_SEGMENT_POINTER(results, uint8_t, SI_SEG_XDATA),
   const SI_VARIABLE_SEGMENT_POINTER(expectedResults, uint8_t, SI_SEG_CODE));

//-----------------------------------------------------------------------------
// Interrupt proto (for SDCC compatibility)
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(DMA_ISR, DMA0_IRQn);

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
   uint16_t i, j;
   uint16_t errors;


   SYSCLK_Init();
   SPI_Init();
   PortInit();

   IE_EA = 1;

   memCopyCX(TEST_PACKET_LENGTH, MISO_Data, SPI1_SlaveOutBuffer);

   SPI1_SlaveOutIn(TEST_PACKET_LENGTH, SPI1_SlaveInBuffer, SPI1_SlaveOutBuffer);

   SPI0_MasterOutIn(TEST_PACKET_LENGTH, MOSI_Data, SPI0_MasterInBuffer);

   while(!SPI_SlaveTransferDone);

   errors = countErrors(TEST_PACKET_LENGTH, SPI1_SlaveInBuffer, MOSI_Data);

   while(errors);

   errors = countErrors(TEST_PACKET_LENGTH, SPI0_MasterInBuffer, MISO_Data);

   while(errors);

   //If no errors exist - shut off both SPI modules
   //Then blink LED1 slowly.
   SPI_Disable();

   while(1)
   {
      LED1 = !LED1;
      for (i = 0; i < 0xFFFF; i++) {
         for (j = 0; j < 0x0002; j++);
      }
   }
}
//-----------------------------------------------------------------------------
// SYSCLK_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator as its clock source.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{

    SFRPAGE = LEGACY_PAGE;
    OSCICN |= 0x80;
    CLKSEL    = 0x00;
}
//-----------------------------------------------------------------------------
// SPI_Init()
//-----------------------------------------------------------------------------
void SPI_Init(void)
{
   SFRPAGE = LEGACY_PAGE;
   SPI0CFG   = 0x40;  // 3-wire master mode
   SPI0CN    = 0x01;
   SPI0CKR   = 0x04;  // SYSCLK/10 - Maximum for Slave Mode


   SFRPAGE = SPI1_PAGE;
   SPI1CFG   = 0x00;
   SPI1CN    = 0x05;
   SPI1CKR   = 0x00;
}
//-----------------------------------------------------------------------------
// PortInit()
//-----------------------------------------------------------------------------
void PortInit(void)
{
   SFRPAGE = LEGACY_PAGE;

   XBR0      = 0x02; // enable SP0
   XBR1      = 0x40; // enable SPI1
   XBR2      = 0x40; // enable crossbar

   P0MDOUT   = 0x0D;
   P2MDOUT   = 0x02;

   SFRPAGE = CONFIG_PAGE;
   P0DRV   = 0x0D;
   P2DRV   = 0x02;

}

//-----------------------------------------------------------------------------
// SPI_Disable()
//-----------------------------------------------------------------------------
void SPI_Disable(void)
{
   XBR0      &= ~0x02; // disable SPI0;
   XBR1      &= ~0x40; // disable SPI1;
   P2MDOUT &= ~0x02;    // revert SPI1 pin drive settings
   P0MDOUT &= ~0x0D;    // revert SPI0 pin drive settings
   P2 |= 0x02;          // Pull SPI1 drive pins high
   P0 |= 0x0D;          // Pull SPI0 drive pins high
}


//-----------------------------------------------------------------------------
//  SPI0_MasterOutIn ()
//-----------------------------------------------------------------------------

void SPI0_MasterOutIn (uint16_t length,
   const SI_VARIABLE_SEGMENT_POINTER(writePointer, uint8_t, SI_SEG_CODE),
   SI_VARIABLE_SEGMENT_POINTER(readPointer, uint8_t, SI_SEG_XDATA))
{

   NSS0 = 0;

   SFRPAGE = LEGACY_PAGE;
   do
   {
      SPI0CN_SPIF = 0;
      SPI0DAT = *writePointer++;
      while(!SPI0CN_SPIF);
      *readPointer++ = SPI0DAT;
  } while(--length);

  NSS0 = 1;
}
//-----------------------------------------------------------------------------
// memCopyCX()
//
// parameters:
//    uint8_t n - number of bytes to copy
//
// This function copies the Reference plain Text to the global xram variable.
//
//-----------------------------------------------------------------------------
void memCopyCX(uint16_t n,
   const SI_VARIABLE_SEGMENT_POINTER(cPointer, uint8_t, SI_SEG_CODE),
   SI_VARIABLE_SEGMENT_POINTER(xPointer, uint8_t, SI_SEG_XDATA))
{
   do
   {
      *xPointer++ = *cPointer++;
   } while (--n);
}
//-----------------------------------------------------------------------------
// countErrors()
//
// parameters:
//-----------------------------------------------------------------------------
uint16_t countErrors (uint16_t bytes,
   SI_VARIABLE_SEGMENT_POINTER(results, uint8_t, SI_SEG_XDATA),
   const SI_VARIABLE_SEGMENT_POINTER(expectedResults, uint8_t, SI_SEG_CODE))
{
   uint16_t errors;

   errors = 0;

   while(bytes)
   {
      if(*results++ != *expectedResults++)
         errors++;
      bytes--;
   }

   return errors;
}

