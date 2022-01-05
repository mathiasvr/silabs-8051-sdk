//-----------------------------------------------------------------------------
// F3xx_USB0_Standard_Requests.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This source file contains the subroutines used to handle incoming
// setup packets. These are called by Handle_Setup in USB_ISR.c and used for
// USB chapter 9 compliance.
//
//
// Target:         C8051F3xx
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
// Project Name:   USB0_Bulk
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//
//
//-----------------------------------------------------------------------------
// Header Files
//-----------------------------------------------------------------------------

#include "c8051f3xx.h"
#include "F3xx_USB0_Register.h"
#include "F3xx_USB0_Descriptor.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"

//-----------------------------------------------------------------------------
// Static Global Variables
//-----------------------------------------------------------------------------

// These are response packets used for communication with host
static SI_SEGMENT_VARIABLE(ONES_PACKET[2], const uint8_t, SI_SEG_CODE) = {0x01, 0x00};
static SI_SEGMENT_VARIABLE(ZERO_PACKET[2], const uint8_t, SI_SEG_CODE) = {0x00, 0x00};

//-----------------------------------------------------------------------------
// Global Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Get_Status
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change for custom HID designs.
//
// This routine returns a two byte status packet to the host.
//
// ----------------------------------------------------------------------------
void Get_Status (void)
{
   if (Setup.wValue.u16 != 0 ||
       Setup.wLength.u16 != 2)
   {
      Force_Stall ();
   }

   // Determine if recipient was device, interface, or EP
   switch(Setup.bmRequestType)
   {
      // If recipient was device
      case OUT_DEVICE:
         if (Setup.wIndex.u16 != 0)
         {
            // Send stall if request is invalid
            Force_Stall ();
         }
         else
         {
            // Otherwise send 0x00, indicating bus power and no
            // remote wake-up supported
            DataPtr = (uint8_t*) &ZERO_PACKET;
            DataSize = 2;
         }
         break;

      // See if recipient was interface
      case OUT_INTERFACE:
         // Only valid if device is configured and non-zero index
         if ((USB0_State != DEV_CONFIGURED) || Setup.wIndex.u16 != 0)
         {
            // Otherwise send stall to host
            Force_Stall ();
         }
         else
         {
            // Status packet always returns 0x00
            DataPtr = (uint8_t*)&ZERO_PACKET;
            DataSize = 2;
         }
         break;

      // See if recipient was an endpoint
      case OUT_ENDPOINT:
         // Make sure device is configured and index msb = 0x00
         if ((USB0_State != DEV_CONFIGURED) || Setup.wIndex.u8[MSB])
         {
            // otherwise return stall to host
            Force_Stall();
         }
         else
         {
            // Handle case if request is directed to IN_EP1
            if (Setup.wIndex.u8[LSB] == IN_EP1)
            {
               // If endpoint is halted, return 0x01,0x00
               if (EP_Status[1] == EP_HALT)
               {
                  DataPtr = (uint8_t*)&ONES_PACKET;
                  DataSize = 2;
               }
               else
               {
                  // Otherwise return 0x00,0x00 to indicate endpoint active
                  DataPtr = (uint8_t*)&ZERO_PACKET;
                  DataSize = 2;
               }
            }
            // Handle case if request is directed to OUT_EP1
            else if (Setup.wIndex.u8[LSB] == OUT_EP1)
            {
               // If endpoint is halted, return 0x01,0x00
               if (EP_Status[2] == EP_HALT)
               {
                  DataPtr = (uint8_t*)&ONES_PACKET;
                  DataSize = 2;
               }
               else
               {
                  // Otherwise return 0x00,0x00 to indicate endpoint active
                  DataPtr = (uint8_t*)&ZERO_PACKET;
                  DataSize = 2;
               }
            }
            else
            {
               // Send stall if unexpected data encountered
               Force_Stall ();
            }
         }
         break;

      default:
         Force_Stall ();
         break;
   }

   if (EP_Status[0] != EP_STALL)
   {
      // Set serviced Setup Packet, Endpoint 0 in transmit mode, and
      // reset DataSent counter
      POLL_WRITE_BYTE (E0CSR, rbSOPRDY);
      EP_Status[0] = EP_TX;
      DataSent = 0;
   }
}

//-----------------------------------------------------------------------------
// Clear_Feature
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change in custom HID designs.
//
// This routine can clear Halt Endpoint features on endpoint 1
//
//-----------------------------------------------------------------------------
void Clear_Feature (void)
{
   // Send procedural stall if device isn't configured
   // Or request is made to host(remote wakeup not supported)
   // Or request is made to interface
   // Or msbs of value or index set to non-zero value
   // Or data length set to non-zero.
   if ((USB0_State != DEV_CONFIGURED) ||
       (Setup.bmRequestType == IN_DEVICE) ||
       (Setup.bmRequestType == IN_INTERFACE) ||
       (Setup.wValue.u16 != 0) ||
       (Setup.wLength.u16 != 0))
   {
      Force_Stall ();
   }
   else
   {
      // Verify that packet was directed at an endpoint
      // The feature selected was HALT_ENDPOINT
      // And that the request was directed at IN_EP1 in
      if ((Setup.bmRequestType == IN_ENDPOINT) &&
          (Setup.wValue.u8[LSB] == ENDPOINT_HALT) &&
          (Setup.wIndex.u8[LSB] == IN_EP1))
      {
         // Clear feature endpoint 1 halt
         POLL_WRITE_BYTE (INDEX, 1);
         POLL_WRITE_BYTE (EINCSR1, rbInCLRDT);
         EP_Status[1] = EP_IDLE;
      }
      // Verify that packet was directed at an endpoint
      // The feature selected was HALT_ENDPOINT
      // And that the request was directed at OUT_EP1 in
      else if ((Setup.bmRequestType == IN_ENDPOINT) &&
               (Setup.wValue.u8[LSB] == ENDPOINT_HALT) &&
               (Setup.wIndex.u8[LSB] == OUT_EP1))
      {
         // Clear feature endpoint 1 halt
         POLL_WRITE_BYTE (INDEX, 1);
         POLL_WRITE_BYTE (EOUTCSR1, rbOutCLRDT);
         EP_Status[2] = EP_IDLE;
      }
      else
      {
         // Send procedural stall
         Force_Stall ();
      }
   }

   // Reset Index to 0
   POLL_WRITE_BYTE (INDEX, 0);

   if (EP_Status[0] != EP_STALL)
   {
      // Set Serviced Out packet ready and
      // data end to indicate transaction
      // is over
      POLL_WRITE_BYTE (E0CSR, (rbSOPRDY | rbDATAEND));
   }
}

//-----------------------------------------------------------------------------
// Set_Feature
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change in custom HID designs.
//
// This routine will set the EP Halt feature for endpoint 1
//
//-----------------------------------------------------------------------------
void Set_Feature (void)
{
   // Make sure device is configured, Setup data
   // is all valid and that request is directed at an endpoint
   if ((USB0_State != DEV_CONFIGURED) ||
       (Setup.bmRequestType == IN_DEVICE) ||
       (Setup.bmRequestType == IN_INTERFACE) ||
       (Setup.wValue.u16 != 0) ||
       (Setup.wLength.u16 != 0))
   {
      // Otherwise send stall to host
      Force_Stall ();
   }
   else
   {
      // Make sure endpoint exists and that halt
      // endpoint feature is selected
      if ((Setup.bmRequestType == IN_ENDPOINT) &&
          (Setup.wValue.u8[LSB] == ENDPOINT_HALT) &&
          (Setup.wIndex.u8[LSB] == IN_EP1))
      {
         // Set feature endpoint 1 halt
         POLL_WRITE_BYTE (INDEX, 1);
         POLL_WRITE_BYTE (EINCSR1, rbInSDSTL);
         EP_Status[1] = EP_HALT;
      }
      // Make sure endpoint exists and that halt
      // endpoint feature is selected
      else if ((Setup.bmRequestType == IN_ENDPOINT) &&
               (Setup.wValue.u8[LSB] == ENDPOINT_HALT) &&
               (Setup.wIndex.u8[LSB] == OUT_EP1))
      {
         // Set feature endpoint 1 halt
         POLL_WRITE_BYTE (INDEX, 1);
         POLL_WRITE_BYTE (EOUTCSR1, rbOutSDSTL);
         EP_Status[2] = EP_HALT;
      }
      else
      {
         // Send procedural stall
         Force_Stall ();
      }
   }

   POLL_WRITE_BYTE (INDEX, 0);

   if (EP_Status[0] != EP_STALL)
   {
      // Indicate Setup packet has been serviced
      POLL_WRITE_BYTE (E0CSR, (rbSOPRDY | rbDATAEND));
   }
}

//-----------------------------------------------------------------------------
// Set_Address
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change in custom HID designs.
//
// Set new function address
//
//-----------------------------------------------------------------------------
void Set_Address (void)
{
   // Request must be directed to device
   // with index and length set to zero.
   if ((Setup.bmRequestType != IN_DEVICE) ||
       (Setup.wIndex.u16 != 0) ||
       (Setup.wLength.u16 != 0) ||
       (Setup.wValue.u16 == 0x0000) ||
       (Setup.wValue.u16 > 0x007F))
   {
      // Send stall if Setup data invalid
      Force_Stall ();
   }

   // Set endpoint zero to update address next status phase
   EP_Status[0] = EP_ADDRESS;

   if (Setup.wValue.u8[LSB] != 0)
   {
      // Indicate that device state is now address
      USB0_State = DEV_ADDRESS;
   }
   else
   {
      // If new address was 0x00, return device to default state
      USB0_State = DEV_DEFAULT;
   }

   if (EP_Status[0] != EP_STALL)
   {
      // Indicate Setup packet has been serviced
      POLL_WRITE_BYTE (E0CSR, (rbSOPRDY | rbDATAEND));
   }
}

//-----------------------------------------------------------------------------
// Get_Descriptor
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change in custom HID designs.
//
// This routine sets the data pointer and size to correct descriptor and
// sets the endpoint status to transmit
//
//-----------------------------------------------------------------------------
void Get_Descriptor (void)
{
   // Determine which type of descriptor
   // was requested, and set data ptr and
   // size accordingly
   switch (Setup.wValue.u8[MSB])
   {
      case DSC_DEVICE:
         DataPtr = (uint8_t*) &DEVICE_DESC;
         DataSize = DEVICE_DESC.bLength;
         break;

      case DSC_CONFIG:
         DataPtr = (uint8_t*) &CONFIG_DESC;
         DataSize = CONFIG_DESC.wTotalLengthLsb +
                    (CONFIG_DESC.wTotalLengthMsb << 8);
         break;

      case DSC_STRING:
         // NOTE: if strings are added to this project, the hard-coded
         // value of 2 will need to be increased
         if (Setup.wValue.u8[LSB] > 2)
         {
            // Invalid string descriptor index
            Force_Stall();
         }
         else
         {
            DataPtr = STRING_DESC_TABLE[Setup.wValue.u8[LSB]];
            DataSize = DataPtr[0];
         }
         break;

      case DSC_INTERFACE:
         DataPtr = (uint8_t*) &INTERFACE_DESC;
         DataSize = INTERFACE_DESC.bLength;
         break;

      case DSC_ENDPOINT:
         if ((Setup.wValue.u8[LSB] == IN_EP1) ||
             (Setup.wValue.u8[LSB] == OUT_EP1))
         {
            if (Setup.wValue.u8[LSB] == IN_EP1)
            {
               DataPtr = (uint8_t*) &IN_EP1_DESC;
               DataSize = IN_EP1_DESC.bLength;
            }
            else
            {
               DataPtr = (uint8_t*) &OUT_EP1_DESC;
               DataSize = OUT_EP1_DESC.bLength;
            }
         }
         else
         {
            Force_Stall();
         }
         break;

      default:
         // Send Stall if unsupported request
         Force_Stall ();
         break;
   }

   // Verify that the requested descriptor is valid
   if (Setup.wValue.u8[MSB] == DSC_DEVICE ||
       Setup.wValue.u8[MSB] == DSC_CONFIG ||
       Setup.wValue.u8[MSB] == DSC_STRING ||
       Setup.wValue.u8[MSB] == DSC_INTERFACE ||
       Setup.wValue.u8[MSB] == DSC_ENDPOINT)
   {
      if ((Setup.wLength.u8[LSB] < DataSize) &&
          (Setup.wLength.u8[MSB] == 0))
      {
         // Send only requested amount of data
         DataSize = Setup.wLength.u16;
      }
   }

   // Make sure endpoint not in stall mode
   if (EP_Status[0] != EP_STALL)
   {
      POLL_WRITE_BYTE (E0CSR, rbSOPRDY); // Service Setup Packet
      EP_Status[0] = EP_TX;              // Put endpoint in transmit mode
      DataSent = 0;                      // Reset Data Sent counter
   }
}

//-----------------------------------------------------------------------------
// Get_Configuration
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change in custom HID designs.
//
// This routine returns current configuration value
//
//-----------------------------------------------------------------------------
void Get_Configuration (void)
{
   // This request must be directed to the device
   // With value word set to zero
   // And index set to zero
   // And Setup length set to one
   if ((Setup.bmRequestType != OUT_DEVICE) ||
       (Setup.wValue.u16 != 0) ||
       (Setup.wIndex.u16 != 0) ||
       (Setup.wLength.u16 != 1))
   {
      // Otherwise send a stall to host
      Force_Stall ();
   }

   else
   {
      // If the device is configured, then
      // return value 0x01 since this software
      // only supports one configuration
      if (USB0_State == DEV_CONFIGURED)
      {
         DataPtr = (uint8_t*)&ONES_PACKET;
         DataSize = 1;
      }
      // If the device is in address state, it
      // is not configured, so return 0x00
      if (USB0_State == DEV_ADDRESS)
      {
         DataPtr = (uint8_t*)&ZERO_PACKET;
         DataSize = 1;
      }
   }
   if (EP_Status[0] != EP_STALL)
   {
      // Set Serviced Out Packet bit
      POLL_WRITE_BYTE (E0CSR, rbSOPRDY);
      EP_Status[0] = EP_TX;            // Put endpoint into transmit mode
      DataSent = 0;                    // Reset Data Sent counter to zero
   }
}

//-----------------------------------------------------------------------------
// Set_Configuration
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change in custom HID designs.
//
// This routine allows host to change current device configuration value
//
//-----------------------------------------------------------------------------
void Set_Configuration (void)
{
   // Device must be addressed before configured
   // and request recipient must be the device
   // the index and length words must be zero
   // This software only supports config = 0,1
   if ((USB0_State == DEV_DEFAULT) ||
       (Setup.bmRequestType != IN_DEVICE) ||
       (Setup.wIndex.u16 != 0) ||
       (Setup.wLength.u16 != 0) ||
       (Setup.wValue.u16 > 1))
   {
      // Send stall if Setup data is invalid
      Force_Stall ();
   }
   else
   {
      // Any positive configuration request
      // results in configuration being set
      // to 1
      if (Setup.wValue.u8[LSB] > 0)
      {
         USB0_State = DEV_CONFIGURED;

         // Set endpoint status to idle (enabled)
         EP_Status[1] = EP_IDLE;
         EP_Status[2] = EP_IDLE;

         // Change index to endpoint 1
         POLL_WRITE_BYTE (INDEX, 1);

         // Set DIRSEL to indicate endpoint 1 is IN/OUT
         POLL_WRITE_BYTE (EINCSR2, rbInSPLIT);

         Handle_In1();

         // Set index back to endpoint 0
         POLL_WRITE_BYTE (INDEX, 0);
      }
      else
      {
         USB0_State = DEV_ADDRESS;     // Unconfigures device by setting state
         EP_Status[1] = EP_HALT;       // to address, and changing IN_EP1
         EP_Status[2] = EP_HALT;       // to address, and changing OUT_EP1
      }
   }

   if (EP_Status[0] != EP_STALL)
   {
      // Indicate Setup packet has been serviced
      POLL_WRITE_BYTE (E0CSR, (rbSOPRDY | rbDATAEND));
   }
}

//-----------------------------------------------------------------------------
// Get_Interface
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - Non
//
// Standard request that should not change in custom HID designs.
//
// This routine returns 0x00, since only one interface is supported by
// this firmware
//
//-----------------------------------------------------------------------------
void Get_Interface (void)
{
   // If device is not configured
   // or recipient is not an interface
   // or non-zero value or index fields
   // or data length not equal to one
   if ((USB0_State != DEV_CONFIGURED) ||
       (Setup.bmRequestType != OUT_INTERFACE) ||
       (Setup.wValue.u16 != 0) ||
       (Setup.wIndex.u16 != 0) ||
       (Setup.wLength.u16 != 1))
   {
      // Then return stall due to invalid request
      Force_Stall ();
   }
   else
   {
      // Otherwise, return 0x00 to host
      DataPtr = (uint8_t*)&ZERO_PACKET;
      DataSize = 1;
   }

   if (EP_Status[0] != EP_STALL)
   {
      // Set Serviced Setup packet, put endpoint in transmit mode and reset
      // Data sent counter
      POLL_WRITE_BYTE (E0CSR, rbSOPRDY);
      EP_Status[0] = EP_TX;
      DataSent = 0;
   }
}

//-----------------------------------------------------------------------------
// Set_Interface
//-----------------------------------------------------------------------------
//
// Return Value - None
// Parameters - None
//
// Standard request that should not change in custom HID designs.
//
//-----------------------------------------------------------------------------
void Set_Interface (void)
{
   // Make sure request is directed at interface
   // and all other packet values are set to zero
   if ((Setup.bmRequestType != IN_INTERFACE) ||
       (Setup.wLength.u16 != 0) ||
       (Setup.wValue.u16 != 0) ||
       (Setup.wIndex.u16 != 0))
   {
      // Othewise send a stall to host
      Force_Stall ();
   }
   if (EP_Status[0] != EP_STALL)
   {
      // Indicate Setup packet has been
      // serviced
      POLL_WRITE_BYTE (E0CSR, (rbSOPRDY | rbDATAEND));
   }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------