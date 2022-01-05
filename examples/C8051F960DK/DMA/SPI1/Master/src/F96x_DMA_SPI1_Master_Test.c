//=============================================================================
// F96x_DMA_SPI1_Master_Test.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the use of the SPI module with DMA capability. It
// uses a SPI loopback to demonstrate SPI transfers directly to and from XRAM.
//
// To use P2.1/MOSI must be shorted to P2.2/MISO.
//
// How To Test:
//
// 1) In "Define Hardware", #define either "UDP_F960_MCU_MUX_LCD" or
//    "UDP_F960_MCU_EMIF" depending on which board you are using.
// 2) Connect P2.1 (MOSI) to P2.2 (MISO).
// 3) Connect the USB Debug Adapter to J13.
// 4) Move the VBAT SEL switch (SW5) to the middle VREG position.
// 5) Move the VIO switch (SW7) and VIORF switch (SW12) to the VBAT position.
// 6) Connect a 9V power adapter to P1 of the development board.
// 7) Compile and download code to a 'F96x device on a UDP F960 MCU MUX LCD or
//    UDP F960 MCU EMIF board by selecting Run -> Debug from the menus,
//    clicking the Debug button in the quick menu, or pressing F11.
// 8) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 9) LED1 should be on if the test packet was transmitted correctly.
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
#include "SPI1_Master.h"
#include "DMA_defs.h"

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
// code constants
//=============================================================================
//-----------------------------------------------------------------------------
// Test Packet Length
//-----------------------------------------------------------------------------
#define TEST_PACKET_LENGTH 64
//-----------------------------------------------------------------------------
// Test Packet
//-----------------------------------------------------------------------------
const SI_SEGMENT_VARIABLE(TestPacket[TEST_PACKET_LENGTH], uint8_t, SI_SEG_CODE) =
{
   0x00, 0x41, 0x82, 0xC3,
   0x04, 0x45, 0x86, 0xC7,
   0x08, 0x49, 0x8A, 0xCB,
   0x0C, 0x4D, 0x8E, 0xCF,
   0x10, 0x51, 0x92, 0xD3,
   0x14, 0x55, 0x96, 0xD7,
   0x18, 0x59, 0x9A, 0xDB,
   0x1C, 0x5D, 0x9E, 0xDF,
   0x20, 0x61, 0xA2, 0xE3,
   0x24, 0x65, 0xA6, 0xE7,
   0x28, 0x69, 0xAA, 0xEB,
   0x2C, 0x6D, 0xAE, 0xEF,
   0x30, 0x71, 0xB2, 0xF3,
   0x34, 0x75, 0xB6, 0xF7,
   0x38, 0x79, 0xBA, 0xFB,
   0x3C, 0x7D, 0xBE, 0xFF
};
//=============================================================================
// File global variables
//=============================================================================
//-----------------------------------------------------------------------------
// Global arrays for data stored in xram
//-----------------------------------------------------------------------------
SI_SEGMENT_VARIABLE(SPI1_OutBuffer[256], uint8_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(SPI1_InBuffer[256], uint8_t, SI_SEG_XDATA);
//=============================================================================
// Function Prototypes
//=============================================================================
void SPI1_Init(void);
void PortInit(void);

void memCopyCX(uint16_t n,
   const SI_VARIABLE_SEGMENT_POINTER(cPointer, uint8_t, SI_SEG_CODE),
   SI_VARIABLE_SEGMENT_POINTER(xPointer, uint8_t, SI_SEG_XDATA));

uint16_t countErrors (uint16_t bytes,
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
   uint8_t errors;


   SPI1_Init();
   PortInit();
   
   LED1 = LED_OFF;

   memCopyCX(TEST_PACKET_LENGTH, TestPacket, SPI1_OutBuffer);

   IE_EA = 1;

   SPI1_MasterOutIn (TEST_PACKET_LENGTH, SPI1_OutBuffer, SPI1_InBuffer);

   errors = countErrors (TEST_PACKET_LENGTH, SPI1_InBuffer, TestPacket);

   while(errors);

   // The write only function is not tested by code, but can be observed on an oscilloscope.
   SPI1_MasterOutOnly (TEST_PACKET_LENGTH, SPI1_OutBuffer);

   LED1 = LED_ON;
   while(1);
}
//-----------------------------------------------------------------------------
// SPI1_Init
//-----------------------------------------------------------------------------
void SPI1_Init(void)
{
   SFRPAGE = SPI1_PAGE;
   SPI1CFG   = 0x40;
   SPI1CN    = 0x01;
   SPI1CKR   = 0x00;
}
//-----------------------------------------------------------------------------
// SPI1_Init
//-----------------------------------------------------------------------------
void PortInit(void)
{
   SFRPAGE = LEGACY_PAGE;

   XBR1      = 0x40; // enable SPI1
   XBR2      = 0x40; // enable crossbar

   SFRPAGE = LEGACY_PAGE;
   P2MDOUT   = 0x05;
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

//=============================================================================
// End
//=============================================================================

