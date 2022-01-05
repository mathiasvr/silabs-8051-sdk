//-----------------------------------------------------------------------------
// F3xx_USB0_ReportHandler.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Stub file for Firmware Template.
//
//
// How To Test:    See Readme.txt
//
//
// FID             
// Target:         C8051F32x/C8051F340
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   HIDtoUART
//
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//


// ----------------------------------------------------------------------------
// Header files
// ----------------------------------------------------------------------------

#include "F3xx_USB0_ReportHandler.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_HIDtoUART.h"
#include "c8051f3xx.h"
// ----------------------------------------------------------------------------
// Local Function Prototypes
// ----------------------------------------------------------------------------
void IN_Control (void);
void OUT_Control (void);
void IN_Data (void);
void OUT_Data (void);

// ----------------------------------------------------------------------------
// Local Definitions
// ----------------------------------------------------------------------------

// ****************************************************************************
// Set Definitions to sizes corresponding to the number of reports
// ****************************************************************************

#define IN_VECTORTABLESize 2
#define OUT_VECTORTABLESize 2

// ----------------------------------------------------------------------------
// Global Constant Declaration
// ----------------------------------------------------------------------------


// ****************************************************************************
// Link all Report Handler functions to corresponding Report IDs
// ****************************************************************************

const VectorTableEntry IN_VECTORTABLE[IN_VECTORTABLESize] =
{
   // FORMAT: Report ID, Report Handler
   IN_DATA, IN_Data,
   IN_CONTROL, IN_Control

};

// ****************************************************************************
// Link all Report Handler functions to corresponding Report IDs
// ****************************************************************************
const VectorTableEntry OUT_VECTORTABLE[OUT_VECTORTABLESize] =
{
   // FORMAT: Report ID, Report Handler
   OUT_DATA, OUT_Data,
   OUT_CONTROL, OUT_Control
};


// ----------------------------------------------------------------------------
// Global Variable Declaration
// ----------------------------------------------------------------------------

BufferStructure IN_BUFFER, OUT_BUFFER;

// ----------------------------------------------------------------------------
// Local Variable Declaration
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Local Functions
// ----------------------------------------------------------------------------

// ****************************************************************************
// Add all Report Handler routines here
// ****************************************************************************


// ****************************************************************************
// For Input Reports:
// Point IN_BUFFER.Ptr to the buffer containing the report
// Set IN_BUFFER.Length to the number of bytes that will be
// transmitted.
//
// REMINDER:
// Systems using more than one report must define Report IDs
// for each report.  These Report IDs must be included as the first
// byte of an IN report.
// Systems with Report IDs should set the FIRST byte of their buffer
// to the value for the Report ID
// AND
// must transmit Report Size + 1 to include the full report PLUS
// the Report ID.
//
// ****************************************************************************
void IN_Control (void)
{
#ifndef BAUDRATE_HARDCODED
   ULONG baud_rate;
   baud_rate.L = BaudRate;
   // Transmit system configuration to host
   IN_PACKET[0] = IN_CONTROL;
   IN_PACKET[1] = baud_rate.C[0];
   IN_PACKET[2] = baud_rate.C[1];
   IN_PACKET[3] = baud_rate.C[2];
   IN_PACKET[4] = baud_rate.C[3];

   IN_BUFFER.Ptr = IN_PACKET;
   IN_BUFFER.Length = IN_CONTROL_SIZE + 1;
#endif
}

void IN_Data (void)
{
   unsigned char index,terminal_value;

   IN_PACKET[0] = IN_DATA;

  // IE_EA = 0;
   // critical section for UART buffer
   // first byte after report ID shows how many valid bytes contained
   // within buffer
   IN_PACKET[1] = UART_INPUT_SIZE;
 //  IE_EA = 1;

   index = 2;
   terminal_value = IN_PACKET[1];
   while (terminal_value != 0)
   {
      // Enter critical section
  //    IE_EA = 0;
      // Increment pointer and wrap if necessary
      UART_INPUT_LAST++;
      UART_INPUT_LAST &= ~0x80;
      // Add byte to report to be transmitted
      IN_PACKET[index++] = UART_INPUT[UART_INPUT_LAST];
      // Decrement size and 
      UART_INPUT_SIZE--;
      terminal_value--;
      // Leave critical section
    //  IE_EA = 1;
   }

   IN_BUFFER.Ptr = IN_PACKET;
   IN_BUFFER.Length = IN_DATA_SIZE + 1;

}


// ****************************************************************************
// For Output Reports:
// Data contained in the buffer OUT_BUFFER.Ptr will not be
// preserved after the Report Handler exits.
// Any data that needs to be preserved should be copyed from
// the OUT_BUFFER.Ptr and into other user-defined memory.
//
// ****************************************************************************

void OUT_Control (void)
{
#ifndef BAUDRATE_HARDCODED
   ULONG baudrate;

   baudrate.C[0] = OUT_PACKET[1];
   baudrate.C[1] = OUT_PACKET[2];
   baudrate.C[2] = OUT_PACKET[3];
   baudrate.C[3] = OUT_PACKET[4];
   BaudRate = baudrate.L;
   UART0_Init ();
#endif
}
void OUT_Data (void)
{
   unsigned char size,index;
   unsigned char xdata* ptr = OUT_PACKET;

   size = OUT_PACKET[1];               // First byte of report shows
                                       // number of valid bytes
                                       // contained in report

   // Use a local pointer to write to OUT_PACKET quickly
   ptr++;
   ptr++;


    //  IE_EA = 0;
   for(index = 0; index < size; index++)
   {
      // Enter critical section
      IE_EA = 0;
      // Move pointer and wrap if necessary
      UART_OUTPUT_FIRST++;
      UART_OUTPUT_FIRST &= ~0x80;
      // Save received byte onto UART buffer
      UART_OUTPUT[UART_OUTPUT_FIRST] = *ptr;
      ptr++;

      UART_OUTPUT_SIZE++;
      if (UART_OUTPUT_SIZE > 127)
      {
         while(1);
      }
      // Exit critical section
      IE_EA = 1;
      

   }
}

// ----------------------------------------------------------------------------
// Global Functions
// ----------------------------------------------------------------------------

// ****************************************************************************
// Configure Setup_OUT_BUFFER
//
// Reminder:
// This function should set OUT_BUFFER.Ptr so that it
// points to an array in data space big enough to store
// any output report.
// It should also set OUT_BUFFER.Length to the size of
// this buffer.
//
// ****************************************************************************

void Setup_OUT_BUFFER(void)
{
   OUT_BUFFER.Ptr = OUT_PACKET;
   OUT_BUFFER.Length = 63;
}

// ----------------------------------------------------------------------------
// Vector Routines
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ReportHandler_IN...
// ----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - Report ID
//
// These functions match the Report ID passed as a parameter
// to an Input Report Handler.
// the ...FG function is called in the SendPacket foreground routine,
// while the ...ISR function is called inside the USB ISR.  A lock
// is set whenever one function is called to prevent a call from the
// other from disrupting the routine.
// However, this should never occur, as interrupts are disabled by SendPacket
// before USB operation begins.
// ----------------------------------------------------------------------------
void ReportHandler_IN_ISR(unsigned char R_ID)
{
   unsigned char index;

   index = 0;

   while(index <= IN_VECTORTABLESize)
   {
      // Check to see if Report ID passed into function
  	  // matches the Report ID for this entry in the Vector Table
      if(IN_VECTORTABLE[index].ReportID == R_ID)
      {
         IN_VECTORTABLE[index].hdlr();
         break;
      }

      // If Report IDs didn't match, increment the index pointer
      index++;
   }

}
void ReportHandler_IN_Foreground(unsigned char R_ID)
{
   unsigned char index;

   index = 0;

   while(index <= IN_VECTORTABLESize)
   {
      // Check to see if Report ID passed into function
      // matches the Report ID for this entry in the Vector Table
      if(IN_VECTORTABLE[index].ReportID == R_ID)
      {
         IN_VECTORTABLE[index].hdlr();
         break;
      }

      // If Report IDs didn't match, increment the index pointer
      index++;
   }

}

// ----------------------------------------------------------------------------
// ReportHandler_OUT
// ----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// This function matches the Report ID passed as a parameter
// to an Output Report Handler.
//
// ----------------------------------------------------------------------------
void ReportHandler_OUT(unsigned char R_ID){

   unsigned char index;

   index = 0;

   while(index <= OUT_VECTORTABLESize)
   {
      // Check to see if Report ID passed into function
      // matches the Report ID for this entry in the Vector Table
      if(OUT_VECTORTABLE[index].ReportID == R_ID)
      {
         OUT_VECTORTABLE[index].hdlr();
         break;
      }

      // If Report IDs didn't match, increment the index pointer
      index++;
   }
}
