/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/\

#ifndef INC_IEC60730_OEM_COMM_H_
#define INC_IEC60730_OEM_COMM_H_

#include "si_toolchain.h"
#include "iec60730.h"

/**************************************************************************//**
 * @addtogroup efm8_iec60730
 * @{ 
 * @addtogroup IEC60730_OEM_COMM_Test
 * @{
 * @details These files provide an example communications protocol that meet
 * IEC60730 requirements, which include address redundancy, a CRC check on data payloads,
 * and a periodic transfer of data.  This example uses UART as its protocol.
 *
 * This protocol simulates both the transmitter and receiver in an IEC60730 compliant
 * application.  The UART RX and TX pins must be shorted together in order for the
 * example to successfully receive transmitted data.
 *****************************************************************************/

/**************************************************************************//**
 * public Ready peripheral to read a status message
 *
 * @return void
 *
 * This function calls into the UART peripheral library to receive 
 * the status packet.
 *****************************************************************************/
void iec60730_commInitReceiveStatusMessage(void);
/**************************************************************************//**
 * public Transmit status message
 *
 * @return void
 *
 * This function passes a status message buffer into the UART peripheral library
 * for transmit using @ref iec60730_commSendMessage.
 *****************************************************************************/
void iec60730_commTransmitStatusMessage(void);
/**************************************************************************//**
 * public Check received message
 *
 * @return void
 *
 * This function examines the received buffer to check that the CRC is valid.
 * @returns Returns #iec60730_TestFailed if message does not provide 
 * correct CRC and address, #iec60730_TestPassed otherwise.
 *****************************************************************************/
iec60730_TestResult_t iec60730_commCheckReceivedMessage(void);
/**************************************************************************//**
 * public Transmit message
 *
 * @param message_byte 1-byte data payload to be transmitted
 *
 * @return void
 *
 * This function sends a 1-byte payload with IEC-compliant header 
 * and footer overhead.
 *****************************************************************************/
void iec60730_commSendMessage(uint8_t message_byte);

/**************************************************************************//**
 * private Module test for the CRC check function
 *
 * @return None
 *
 * This function is used only for automated testing of 
 * iec60730_updateCRCWithDataBuffer(). Do not include it in any OEM code.
 *****************************************************************************/
void iec60730_crcCheckFunction();
/// This variable is only used for automated testing of 
/// iec60730_updateCRCWithDataBuffer(). Do not include it in any OEM code.
extern SI_SEGMENT_VARIABLE(iec60730_crcCheckBufferSize, uint8_t, SI_SEG_IDATA);
/// This variable is only used for automated testing of 
/// iec60730_updateCRCWithDataBuffer(). Do not include it in any OEM code.
extern SI_SEGMENT_VARIABLE(iec60730_crcCheckResult, uint16_t, SI_SEG_IDATA);
/// This variable is only used for automated testing of 
/// iec60730_updateCRCWithDataBuffer(). Do not include it in any OEM code.
extern SI_SEGMENT_VARIABLE(iec60730_crcCheckTestData, uint16_t, SI_SEG_XDATA);

/// Sets the address used by the transmitter and checked by receiver in the comm protocol.
#define IEC60730_COMM_DEVICE_ADDRESS 0x01

/// Buffer used in comm that includes IEC60730-compliant address and CRC.
typedef struct messageBuffer
{
  uint8_t   device_address;     ///< 8-bit address of transmitting device
  uint8_t   message;            ///< Payload of buffer
  uint16_t  crc;                ///< CRC covering all other bytes in packet
} messageBuffer_t;

/** @} (end addtogroup IEC60730_OEM_COMM_Test) */
/** @} (end addtogroup efm8_iec60730) */

#endif /* INC_IEC60730_OEM_COMM_H_ */
