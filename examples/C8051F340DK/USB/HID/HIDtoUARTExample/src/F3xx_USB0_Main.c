//-----------------------------------------------------------------------------
// F3xx_USB_Main.c
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
// Release 1.1
//    -Changed "Usb_Init" to "USB_Init" (TP)
//    -07 OCT 2010
//
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//
//-----------------------------------------------------------------------------
// Header Files
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_USB0_Register.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_Descriptor.h"
#include "F3xx_HIDtoUART.h"

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Local Variables
//-----------------------------------------------------------------------------

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
   PCA0MD &= ~0x40;
}
 
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main(void)
{
   System_Init ();
   USB_Init ();

   P2 = 0;

   IE_EA = 1;

   while (1)
   {
      // If bytes have been received across UART interface, initiate a USB
      // transfer to send them to the host  
      if ((UART_INPUT_SIZE != 0) && (!SendPacketBusy))
      {
         SendPacket (IN_DATA);
      }

      // If bytes have been received across the USB interface, transmit
      // them across UART
      if ((UART_OUTPUT_SIZE != 0) && (TX_Ready == 1))
      {
         TX_Ready = 0;
         SCON0_TI = 1;                      // Start UART transmit
      }

      // This code functions when the firmware has suspended reception of
      // USB OUT reports because the UART buffer size is above the 
      // critical boundary
      IE_EA = 0;
      if (USB_OUT_SUSPENDED == 1)
      {
         // Check to see if the buffer size has fallen back below the
         // boundary
         if (UART_OUTPUT_SIZE < UART_OUTPUT_OVERFLOW_BOUNDARY)
         {
            // If it has, resume USB communication
            USB_OUT_SUSPENDED = 0;
            POLL_WRITE_BYTE (INDEX, 2);// Set index to endpoint 2 registers
            // Clear Out Packet ready bit
            POLL_WRITE_BYTE (EOUTCSR1, 0);   
         }
      }
      IE_EA = 1;
   }

}

