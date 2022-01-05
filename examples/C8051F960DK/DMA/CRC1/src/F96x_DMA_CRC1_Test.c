//=============================================================================
// CRC1_Test.c
//=============================================================================
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the use of the built-in DMA-enabled CRC engine,
// using DMA to calculate a CRC directly from data stored in XRAM.
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
// 8) LED1 should be on if the program generated CRCs correctly.
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
#include "CRC1.h"
#include "CRC1_TestVectors.h"

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
// myData located in XRAM.
//
//=============================================================================
SI_SEGMENT_VARIABLE(myData[32], uint8_t, SI_SEG_XDATA);
//=============================================================================
// Function Prototypes
//=============================================================================
//-----------------------------------------------------------------------------
// Interrupt proto (for SDCC compatibility)
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(DMA_ISR, DMA0_IRQn);
//-----------------------------------------------------------------------------
// Functions used to initialize arrays.
//-----------------------------------------------------------------------------
void initCRCData (SI_VARIABLE_SEGMENT_POINTER(xData, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(cData, uint8_t, SI_SEG_CODE), uint8_t bytes);

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
   volatile uint16_t results;
   uint8_t status;


   XBR2    = 0x40;                     // Enable crossbar and enable
                                       // weak pull-ups
   LED1 = LED_OFF;

   IE_EA = 1;                             // enable global interrupts

   initCRCData(myData, BlockOne, 10);

   status = ComputeCRC1(10, 0x3d65, myData, &results);

   while(status);

   results = ~results;

   while(results!=0x4447);

   initCRCData(myData, BlockTwo, 6);

   status = ComputeCRC1(6, 0x3d65, myData, &results);

   while(status);

   results = ~results;

   while(results!=0x1E6D);

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
// initCRCData()
//
// parameters:
//   xData          - xdata pointer to data in xram
//   cData          - code pointer to data in flash
//   bytes          - the number of bytes to copy
//
// returns:
//
// description:
//
// Copies reference cData in Flash to xData in xdata RAM.
// The code constants must be copied to xdata for use by the DMA.
//
//-----------------------------------------------------------------------------
void initCRCData (SI_VARIABLE_SEGMENT_POINTER(xData, uint8_t, SI_SEG_XDATA),
   SI_VARIABLE_SEGMENT_POINTER(cData, uint8_t, SI_SEG_CODE),
   uint8_t bytes)
{
   while(bytes--)
   {
      *xData++ = *cData++;
   }
}
//=============================================================================
// End of file
//=============================================================================

