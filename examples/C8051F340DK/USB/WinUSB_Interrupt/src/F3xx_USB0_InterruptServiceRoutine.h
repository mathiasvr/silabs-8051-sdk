//-----------------------------------------------------------------------------
// F3xx_USB0_InterruptServiceRoutine.h
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
// Target:         C8051F320/1,
//                 C8051F326/7,
//                 C8051F34x,
//                 C8051F38x,
//                 C8051T620/1/6/7, C8051T320/3,
//                 C8051T622/3, C8051T326/7
//
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
//                 Silicon Laboratories IDE version 2.6
// Command Line:   See Readme.txt
// Project Name:   USB0_Interrupt
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

//#define _USB_LOW_SPEED_              // Change this comment to make
                                       // Full/Low speed

//-----------------------------------------------------------------------------
// Endpoint Packet Sizes
//-----------------------------------------------------------------------------

#ifdef _USB_LOW_SPEED_

#define  EP0_PACKET_SIZE         0x08

#else

#define  EP0_PACKET_SIZE         0x40

#endif // _USB_LOW_SPEED_

// Can range 0 - 1024 depending on data and transfer type
#define  IN_EP1_PACKET_SIZE      0x0008

// Can range 0 - 1024 depending on data and transfer type
#define  OUT_EP1_PACKET_SIZE     0x0008

//-----------------------------------------------------------------------------
// Standard Descriptor Types
//-----------------------------------------------------------------------------

#define  DSC_DEVICE              0x01  // Device Descriptor
#define  DSC_CONFIG              0x02  // Configuration Descriptor
#define  DSC_STRING              0x03  // String Descriptor
#define  DSC_INTERFACE           0x04  // Interface Descriptor
#define  DSC_ENDPOINT            0x05  // Endpoint Descriptor

//-----------------------------------------------------------------------------
// Control Transfer: Standard Requests
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// Device States
//-----------------------------------------------------------------------------

// Define device states
#define  DEV_ATTACHED            0x00  // Device is in Attached State
#define  DEV_POWERED             0x01  // Device is in Powered State
#define  DEV_DEFAULT             0x02  // Device is in Default State
#define  DEV_ADDRESS             0x03  // Device is in Addressed State
#define  DEV_CONFIGURED          0x04  // Device is in Configured State
#define  DEV_SUSPENDED           0x05  // Device is in Suspended State

//-----------------------------------------------------------------------------
// bmRequestType bitmaps
//-----------------------------------------------------------------------------

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
//-----------------------------------------------------------------------------
// wIndex bitmaps
//-----------------------------------------------------------------------------

// Index values used by Set and Clear feature
// commands for Endpoint_Halt
#define  IN_EP1                  0x81
#define  OUT_EP1                 0x01

//-----------------------------------------------------------------------------
// wValue bitmaps for Standard Feature Selectors
//-----------------------------------------------------------------------------

#define  DEVICE_REMOTE_WAKEUP    0x01  // Remote wakeup feature(not used)
#define  ENDPOINT_HALT           0x00  // Endpoint_Halt feature selector

//-----------------------------------------------------------------------------
// Endpoint States
//-----------------------------------------------------------------------------

#define  EP_IDLE                 0x00  // This signifies Endpoint Idle State
#define  EP_TX                   0x01  // Endpoint Transmit State
#define  EP_RX                   0x02  // Endpoint Receive State
#define  EP_HALT                 0x03  // Endpoint Halt State (return stalls)
#define  EP_STALL                0x04  // Endpoint Stall (send procedural stall
                                       // next status phase)
#define  EP_ADDRESS              0x05  // Endpoint Address (change FADDR during
                                       // next status phase)
#define  EP_GetReport            0x06  // Special Control Endpoint State for
                                       // GetReport HID Request
#define  EP_SetReport            0x07  // Special Control Endpoint State for
                                       // SetReport HID Request

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

// USB Routines
void Force_Stall(void);                // Forces a procedural stall on
                                       // Endpoint 0
void Handle_In1(void);                 // used by SetConfiguration in
                                       // USB_STD_REQ to initialize
                                       // ReadyToTransfer

// Standard Requests
void Get_Status(void);                 // These are called for each specific
                                       // standard request
void Clear_Feature(void);
void Set_Feature(void);
void Set_Address(void);
void Get_Descriptor(void);
void Get_Configuration(void);
void Set_Configuration(void);
void Get_Interface(void);
void Set_Interface(void);

void USB0_Suspend (void);              // This routine called when
                                       // Suspend signalling on bus

//-----------------------------------------------------------------------------
// External Global Variables
//-----------------------------------------------------------------------------

extern uint8_t EP_Status[];
extern SI_SEGMENT_VARIABLE(In_Packet[IN_EP1_PACKET_SIZE], uint8_t, SI_SEG_XDATA);
extern SI_SEGMENT_VARIABLE(Out_Packet[OUT_EP1_PACKET_SIZE], uint8_t, SI_SEG_XDATA);

#endif // _USB_ISR_H_
