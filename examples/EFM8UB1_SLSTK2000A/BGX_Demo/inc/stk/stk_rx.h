/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef INC_STK_RX_H_
#define INC_STK_RX_H_

/**************************************************************************//**
 * Initializes Buffer A or Buffer B as the target reception buffer.
 *
 * @param useBufferA is a boolean to choose Buffer A (True) or Buffer B (False)
 *****************************************************************************/
void STK_RX_initRxBuffer();

/**************************************************************************//**
 * Clears buffers A and B and sets the target buffer as buffer A.
 *****************************************************************************/
void STK_RX_init();

/**************************************************************************//**
 * Prints any data received by the two buffers from the WirelessXpress board.
 *
 * For reception, two buffers and their sizes are provided. The UART1 library
 * receives data into the buffer until it is full and then calls the user
 * back. However, the data is also processed before the buffer is full by
 * polling the number of bytes remaining in the buffer and checking if it has
 * changed.
 *
 * This method is called ping-pong reception and if one buffer becomes full,
 * the next buffer will start filling up. Thus, in addition to printing the
 * current buffer's content, the previous buffer may also be printed in the
 * case of the data stretching over the two buffers.
 *
 * Example:
 * 8 character transmission into two 6-length ping-pong reception buffers.
 * Buffer A   Buffer B
 * {111111}   {11----}
 *****************************************************************************/
void STK_RX_handleRx();

/**************************************************************************//**
 * Prints the new content of the previously completed buffer.
 *****************************************************************************/
void STK_RX_printOverflow();

/**************************************************************************//**
 * Prints the new content of the current target buffer.
 *****************************************************************************/
void STK_RX_printRxBuffer();

/**************************************************************************//**
 * Parses the scan results and displays all the BGX devices' info.
 *
 *  .......................
 *  .  CONNECT TO A BGX:  .
 *  .---------------------.
 *  . #   RSSI    NAME    .
 *  . -   ----    ----    .
 *  . 1.  -9    BGX-F625  .
 *  . 2.  -79   BGX-F603  .
 *  . 3.  -75   BGX-84C4  .
 *  . 4.  -65   BGX-F629  .
 *  . 5.  -79   BGX-F6FB  .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .   PICK DEVICE #:    .
 *  .                     .
 *  .         1           .
 *  .......................
 *****************************************************************************/
void STK_RX_parseDevicesFromBuffer();

/***************************************************************************//**
 * Callback for reception of data.
 *
 * This function is called when all expected bytes have been received and
 * a buffer has been completed (filled). It will switch the target buffer from
 * buffer A to buffer B or vice versa.
 *
 * @warning
 * This function is called from an ISR and should be as short as possible.
 ******************************************************************************/
void UART1_receiveCompleteCb();

#endif /* INC_STK_RX_H_ */
