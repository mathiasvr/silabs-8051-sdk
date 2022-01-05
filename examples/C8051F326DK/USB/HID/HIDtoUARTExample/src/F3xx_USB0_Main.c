//-----------------------------------------------------------------------------
// F3xx_USB_Main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how the Firmware Template can be used to
// create a USB HID to UART bridge that communicates with a PC application.
// The PC application is a simple terminal-like program that sends data to and
// receives data from the Target Board.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F326-TB are place on the following:
//    J2: VDD  - +3VD
//    J3: P0.4 - TX
//    J3: P0.5 - RX
//    J7: VDD  - VIO
//    J8: Power selection, P1 9V or J4 Debugger
// 2) Connect a serial cable to the J5 RS232 connector on the target board.
// 3) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1.
// 4) Compile and download code to C8051F326 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 6) Run the GUI application HIDtoUART that comes with the firmware
//    and test the features.
//
//
// Target:         C8051F326/7
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
// Project Name:   HIDtoUART
//
//
// Release 1.2 (JL)
//    - Updated description
//    - 13 MAY 2014
//
// Release 1.1
//    -Changed "Usb_Init" to "USB_Init" (TP)
//    -07 OCT 2010
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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main(void)
{
   System_Init ();
   USB_Init ();

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
            POLL_WRITE_BYTE (INDEX, 1);// Set index to endpoint 2 registers
            // Clear Out Packet ready bit
            POLL_WRITE_BYTE (EOUTCSR1, 0);
         }
      }
      IE_EA = 1;
   }

}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
