/**************************************************************************//**
 * Copyright (c) 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

// Disable warnings on unused labels.  Labels are used to set breakpoints for
// automated testing.
#pragma warninglevel(1)

#include "si_toolchain.h"
#include "iec60730.h"
#include "iec60730_oem_comm.h"
#include "si_toolchain.h"
#include "uart_0.h"

SI_SEGMENT_VARIABLE(iec60730_TXbuffer, messageBuffer_t, SI_SEG_IDATA); /*!< Transmit Buffer */
SI_SEGMENT_VARIABLE(iec60730_RXbuffer, messageBuffer_t, SI_SEG_IDATA); /*!< Receive Buffer */
SI_SEGMENT_VARIABLE(message_byte, uint8_t, SI_SEG_IDATA) = 0; /*!< Byte to send */
SI_SEGMENT_VARIABLE(iec60730_crcCheckBufferSize, uint8_t, SI_SEG_IDATA);   ///< Test variable, not needed for OEM code
SI_SEGMENT_VARIABLE(iec60730_crcCheckResult, uint16_t, SI_SEG_IDATA); ///< Test variable, not needed for OEM code
SI_SEGMENT_VARIABLE(iec60730_crcCheckTestData, uint16_t, SI_SEG_XDATA) _at_ 0x7f; ///< Test variable, not needed for OEM code


/**************************************************************************//**
 * Transmit periodic status message
 *****************************************************************************/
void iec60730_commTransmitStatusMessage(void)
{
  iec60730_commSendMessage(message_byte);
}

/**************************************************************************//**
 * Send message with address and CRC
 *****************************************************************************/
void iec60730_commSendMessage(uint8_t message_byte)
{
  static uint8_t configured = 0;

  // This function is for automated testing. Do not include in OEM code.
  iec60730_crcCheckFunction();

  // Configure UART if not already configured
  if (!configured)
  {
    UART0_init(UART0_RX_ENABLE, UART0_WIDTH_8, UART0_MULTIPROC_DISABLE);
    IE_ES0 = 1;
    configured = 1;
  }
  IEC60730_TRANSMIT_DISABLE_BKPT:
  // Packet begins with transmitting device address
  iec60730_TXbuffer.device_address = IEC60730_COMM_DEVICE_ADDRESS;

  // Payload in this example is one byte
  iec60730_TXbuffer.message = message_byte;

  // Calculate CRC using CRC generating function included in library
  iec60730_TXbuffer.crc = iec60730_updateCRCWithDataBuffer(0xFFFF,
                                                           (uint8_t *) (&iec60730_TXbuffer),
                                                           sizeof(messageBuffer_t) - sizeof(uint16_t));

  // Transmit buffer through peripheral library
  UART0_writeBuffer((uint8_t *) &iec60730_TXbuffer, sizeof(messageBuffer_t));
}

/**************************************************************************//**
 * Initialize UART for next receiption of message
 *****************************************************************************/
void iec60730_commInitReceiveStatusMessage(void)
{
  UART0_readBuffer((uint8_t *) &iec60730_RXbuffer, sizeof(messageBuffer_t));
}

/// Check that received message passes IEC60730 plausibility check
/// @return iec60730_TestResult_t Result of the test.
iec60730_TestResult_t iec60730_commCheckStatusMessage(void)
{
  uint16_t received_crc;
  uint16_t calculated_crc;

  // If received and expected address do not match, enter safe state
  if (iec60730_RXbuffer.device_address != IEC60730_COMM_DEVICE_ADDRESS)
  {
    return iec60730_Uart0Fail;
  }
  // Check the CRC by generating one and comparing generated to received
  // The starting address is the beginning of the buffer, including
  // the address. The size is the messageBuffer_t size minus the size of 
  // a CRC.
  calculated_crc = iec60730_updateCRCWithDataBuffer(0xFFFF,
                                                    (uint8_t *) (&iec60730_RXbuffer),
                                                    sizeof(messageBuffer_t) - sizeof(uint16_t));
  received_crc = iec60730_RXbuffer.crc;

  // If CRCs don't match, enter safe state
  if (calculated_crc != received_crc)
  {
    return iec60730_Uart0Fail;
  }

  // Also check payload to make sure that 'status' message is what is expected
  if (iec60730_RXbuffer.message != message_byte)
  {
    return iec60730_Uart0Fail;
  }

  // For the purposes of this example, the status message is an incrementing value
  message_byte = message_byte + 1;

  return iec60730_TestPassed;
}

/**************************************************************************//**
 * UART0 receive buffer complete callback
 *****************************************************************************/
void UART0_transmitCompleteCb(void)
{
  if (iec60730_IRQExecCount[2] < 0xFF)
  {
    iec60730_IRQExecCount[2]++;
  }
}

/**************************************************************************//**
 * UART0 receive buffer complete callback
 *****************************************************************************/
void UART0_receiveCompleteCb(void)
{
  if (iec60730_IRQExecCount[2] < 0xFF)
  {
    iec60730_IRQExecCount[2]++;
  }
  IEC60730_COMM_CHECK_STATUS_BKPT:
  if (iec60730_commCheckStatusMessage() == iec60730_Uart0Fail)
  {
    iec60730_SafetyCheck = iec60730_Uart0Fail;
  }

  iec60730_programmeCounterCheck |= IEC60730_COMMS_COMPLETE;
  IEC60730_COMM_BIST_FLAG_BKPT:
  iec60730_programmeCounterCheck = iec60730_programmeCounterCheck;
}

/**************************************************************************//**
 * This function is for automated testing of iec60730_updateCRCWithDataBuffer()
 *
 * Do not include in OEM code.
 *****************************************************************************/
void iec60730_crcCheckFunction()
{
  IEC60730_COMM_CRC_CHECK_BKPT:
  iec60730_crcCheckResult = iec60730_updateCRCWithDataBuffer(0xFFFF,
                                                            (uint8_t xdata*) 0x0000,
                                                             iec60730_crcCheckBufferSize);
}

