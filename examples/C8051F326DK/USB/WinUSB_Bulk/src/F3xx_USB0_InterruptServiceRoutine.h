//-----------------------------------------------------------------------------
// F3xx_USB0_InterruptServiceRoutine.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Header file for USB ISR, support routines, and variables.
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

#ifndef _USB_ISR_H_
#define _USB_ISR_H_

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

// Endpoint Packet Sizes
#define  EP0_PACKET_SIZE         0x40     // Control endpoint 0 size
#define  IN_EP1_PACKET_SIZE      0x0040   // Bulk IN endpoint 1 size
#define  OUT_EP1_PACKET_SIZE     0x0040   // Bulk OUT endpoint 1 size

// Standard Descriptor Types
#define  DSC_DEVICE              0x01  // Device Descriptor
#define  DSC_CONFIG              0x02  // Configuration Descriptor
#define  DSC_STRING              0x03  // String Descriptor
#define  DSC_INTERFACE           0x04  // Interface Descriptor
#define  DSC_ENDPOINT            0x05  // Endpoint Descriptor

// Control Transfer: Standard Requests
#define  GET_STATUS              0x00  // Code for Get Status
#define  CLEAR_FEATURE           0x01  // Code for Clear Feature
#define  SET_FEATURE             0x03  // Code for Set Feature
#define  SET_ADDRESS             0x05  // Code for Set Address
#define  GET_DESCRIPTOR          0x06  // Code for Get Descriptor
#define  SET_DESCRIPTOR          0x07  // Code for Set Descriptor(not used)
#define  GET_CONFIGURATION       0x08  // Code for Get Configuration
#define  SET_CONFIGURATION       0x09  // Code for Set Configuration
#define  GET_INTERFACE           0x0A  // Code for Get Interface
#define  SET_INTERFACE           0x0B  // Code for Set Interface
#define  SYNCH_FRAME             0x0C  // Code for Synch Frame(not used)

// Device States
#define  DEV_ATTACHED            0x00  // Device is in Attached State
#define  DEV_POWERED             0x01  // Device is in Powered State
#define  DEV_DEFAULT             0x02  // Device is in Default State
#define  DEV_ADDRESS             0x03  // Device is in Addressed State
#define  DEV_CONFIGURED          0x04  // Device is in Configured State
#define  DEV_SUSPENDED           0x05  // Device is in Suspended State

// bmRequestType Bitmaps

// bit 7
#define  DIR_BITMASK             0x80
#define  DIR_OUT                 0x00  // Data stage direction OUT (or no data)
#define  DIR_IN                  0x80  // Data stage direction IN

// bit 6..5
#define  REQ_TYPE_BITMASK        0x60
#define  REQ_TYPE_STD_REQ        0x00  // Standard request
#define  REQ_TYPE_CLASS_REQ      0x20  // Class request
#define  REQ_TYPE_VENDOR         0x40  // Vendor specific request

// bit 4..0
#define  RECP_BITMASK            0x1F
#define  RECP_DEVICE             0x00  // Recipient is device
#define  RECP_INTERFACE          0x01  // Recipient is interface
#define  RECP_ENDPOINT           0x02  // Recipient is endpoint
#define  RECP_OTHER              0x03  // Recipient is other

#define  IN_DEVICE               0x00  // Request made to device,
                                       // direction is IN
#define  OUT_DEVICE              0x80  // Request made to device,
                                       // direction is OUT
#define  IN_INTERFACE            0x01  // Request made to interface,
                                       // direction is IN
#define  OUT_INTERFACE           0x81  // Request made to interface,
                                       // direction is OUT
#define  IN_ENDPOINT             0x02  // Request made to endpoint,
                                       // direction is IN
#define  OUT_ENDPOINT            0x82  // Request made to endpoint,
                                       // direction is OUT

// wIndex bitmaps
#define  IN_EP1                  0x81
#define  OUT_EP1                 0x01

// wValue bitmaps for Standard Feature Selectors
#define  DEVICE_REMOTE_WAKEUP    0x01  // Remote wakeup feature(not used)
#define  ENDPOINT_HALT           0x00  // Endpoint_Halt feature selector

// Endpoint States
#define  EP_IDLE                 0x00  // This signifies Endpoint Idle State
#define  EP_TX                   0x01  // Endpoint Transmit State
#define  EP_RX                   0x02  // Endpoint Receive State
#define  EP_HALT                 0x03  // Endpoint Halt State (return stalls)
#define  EP_STALL                0x04  // Endpoint Stall (send procedural stall
                                       // next status phase)
#define  EP_ADDRESS              0x05  // Endpoint Address (change FADDR during
                                       // next status phase)

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Force_Stall (void);
void Handle_In1 (void);
void Send_Packet_Foreground (void);

//-----------------------------------------------------------------------------
// External Global Variables
//-----------------------------------------------------------------------------

extern uint8_t USB0_State;
extern Setup_Buffer Setup;
extern uint16_t DataSize;
extern uint16_t DataSent;
extern uint8_t const * DataPtr;
extern uint8_t EP_Status[];

#endif // _USB_ISR_H_