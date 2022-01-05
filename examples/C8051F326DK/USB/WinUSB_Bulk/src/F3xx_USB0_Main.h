//-----------------------------------------------------------------------------
// F3xx_USB0_Main.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
//
// Target:         C8051F326/7
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   Non
// Project Name:   USB0_Bulk
//
// Release 2.0
//    -Rewrite (CM)
//    -02 NOV 2012
//
// Release 1.1
//    -Changed "Usb_Init" to "USB_Init" (TP)
//    -07 OCT 2010
//
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//

#ifndef _USB_MAIN_H_
#define _USB_MAIN_H_

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

#define SYSCLOCK           24000000

//-----------------------------------------------------------------------------
// Definitions - State Machine
//-----------------------------------------------------------------------------

// USB Command IDs
#define CMD_SET_FLASH_KEY     0
#define CMD_GET_PAGE_INFO     1
#define CMD_WRITE_PAGE        2
#define CMD_READ_PAGE         3

// USB Response Codes
#define RSP_SUCCESS           0
#define RSP_INVALID           1

// State Machine States
#define ST_IDLE               0x00
#define ST_SET_FLASH_KEY      0x10
#define ST_TX_PAGE_INFO       0x11
#define ST_READ_PAGE          0x12
#define ST_TX_BLOCK           0x13
#define ST_WRITE_PAGE         0x14
#define ST_RX_BLOCK           0x15
#define ST_TX_SUCCESS         0xF0
#define ST_TX_INVALID         0xF1

//-----------------------------------------------------------------------------
// External Global Variables
//-----------------------------------------------------------------------------

extern SI_SEGMENT_VARIABLE(In_Packet[IN_EP1_PACKET_SIZE], uint8_t, SI_SEG_XDATA);
extern SI_SEGMENT_VARIABLE(Out_Packet[OUT_EP1_PACKET_SIZE], uint8_t, SI_SEG_XDATA);
extern uint8_t In_Packet_Ready;
extern uint8_t Out_Packet_Ready;
extern uint8_t AsyncResetState;

#endif // _USB_MAIN_H_