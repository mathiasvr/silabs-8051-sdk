/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <efm8_config.h>
#include <si_toolchain.h>
#include <stdbool.h>
#include <stdint.h>
#include <STRING.H>
#include <uart_0.h>
#include "stk_rx.h"
#include "stk_display.h"

// RX buffer set up for reception
SI_SEGMENT_VARIABLE(rxBuffer[BUFFER_LENGTH], uint8_t, SI_SEG_XDATA);
// Number of bytes remaining the last time RX was printed on the LCD
uint8_t previousRxBytesRemaining = BUFFER_LENGTH;
// Current number of bytes remaining in the target RX buffer
uint8_t currentRxBytesRemaining;
// Pointer in the target RX buffer
uint8_t rxBufferPointer = 0;
// Set when a buffer is filled and has to target the other buffer
bool bufferOverflow = false;
// Index for choosing BGX
SI_SEG_XDATA uint8_t index = 3;

/**************************************************************************//**
 * Initializes Buffer A or Buffer B as the target reception buffer.
 *
 * @param useBufferA is a boolean to choose Buffer A (True) or Buffer B (False)
 *****************************************************************************/
void STK_RX_initRxBuffer()
{
  UART0_readBuffer(rxBuffer, BUFFER_LENGTH);
}

/**************************************************************************//**
 * Clears buffers A and B and sets the target buffer as buffer A.
 *****************************************************************************/
void STK_RX_init()
{
  for (rxBufferPointer = 0; rxBufferPointer < BUFFER_LENGTH; rxBufferPointer++)
  {
    rxBuffer[rxBufferPointer] = 0;
  }
  STK_RX_initRxBuffer();
  previousRxBytesRemaining = BUFFER_LENGTH;
}

/**************************************************************************//**
 * Prints any data received.
 *
 * For reception, a buffer and it's size is provided. The UART1 library
 * receives data into the buffer until it is full and then calls the user
 * back. However, the data is processed before the buffer is full by
 * polling the number of bytes remaining in the buffer and checking if it has
 * changed.
 *****************************************************************************/
void STK_RX_handleRx()
{
  STK_RX_printRxBuffer();
}


/**************************************************************************//**
 * Prints the new content of the current buffer.
 *****************************************************************************/
void STK_RX_printRxBuffer()
{
  currentRxBytesRemaining = UART0_rxBytesRemaining();
  STK_RX_printOverflow();

  if (previousRxBytesRemaining != currentRxBytesRemaining)
  {
    for (rxBufferPointer = previousRxBytesRemaining; rxBufferPointer > currentRxBytesRemaining; rxBufferPointer--)
    {
      STK_DISPLAY_printRx(rxBuffer[BUFFER_LENGTH - rxBufferPointer]);
    }
    previousRxBytesRemaining = currentRxBytesRemaining;
  }
  STK_DISPLAY_header();
}

/**************************************************************************//**
 * Prints the new content of the previously completed buffer.
 *****************************************************************************/
void STK_RX_printOverflow()
{
  // Only print if there is leftover data from previous buffer
  if (bufferOverflow)
  {
    for (rxBufferPointer = previousRxBytesRemaining; rxBufferPointer > 0; rxBufferPointer--)
    {
	  STK_DISPLAY_printRx(rxBuffer[BUFFER_LENGTH - rxBufferPointer]);
    }
    // Reset bytes remaining to max after switching buffers
    previousRxBytesRemaining = BUFFER_LENGTH;
    bufferOverflow = false;
  }
}

/**************************************************************************//**
 * Parses the scan results and displays all the BGX devices' info.
 *  .......................
 *  .      BGX-XXXX       .
 *  .---------------------.
 *  . #   RSSI    NAME    .
 *  . -   ----    ----    .
 *  . 1.  -24   BGX-F625  .
 *  . 2.  -79   BGX-F603  .
 *  . 3.  -75   BGX-84C4  .
 *  . 4.  -65   BGX-F629  .
 *  . 5.  -79   BGX-F6FB  .
 *  .                     .
 *  .                     .
 *  .                     .
 *  .   Use joystick to   .
 *  .   select device #   .
 *  .         1           .
 *  .......................
 *****************************************************************************/
void STK_RX_parseDevicesFromBuffer()
{
  SI_SEG_XDATA char bgxInfo[25];
  SI_SEG_XDATA char *lineToken;
  SI_SEG_XDATA char* bgxInfoList[15];
  const char spaceDelim[2] = " ";
  const char returnDelim[3] = "\r\n";
  char *element;
  uint8_t i;
  uint8_t j;

  // Gather all device info into an array
  lineToken = strtok(rxBuffer, returnDelim);
  while (lineToken != NULL)
  {
    lineToken = strtok(NULL, returnDelim);
    if (lineToken[0]=='#')
    {
      bgxInfoList[index] = lineToken;
      index++;
    }
  }

  // Set up UI
  STK_DISPLAY_printStr(2, 1, "#   RSSI    NAME");
  STK_DISPLAY_printStr(3, 1, "-   ----    ----");

  // Parse each element and gather the device name
  for (i = 3; i < index; i++)
  {
    // Collect BGX index
    bgxInfo[0] = '\0';   // ensures the memory is an empty string
    element = strtok(bgxInfoList[i], spaceDelim); // element = '#'
    element = strtok(NULL, spaceDelim); // element = Index
    strcat(bgxInfo, element);
    strcat(bgxInfo, ".  ");

    // Cap the devices displayed to 5
    if (!strcmp(element, "6"))
    {
      break;
    }

    // Collect BGX RSSI
    element = strtok(NULL, spaceDelim);
    strcat(bgxInfo, element);

    // Adjusts the formatting of the string depending on the RSSI str length
    for (j = 6; j > strlen(element); j--)
    {
      strcat(bgxInfo, " ");
    }

    // Collect device name
    element = strtok(NULL, spaceDelim); // element = Address
    element = strtok(NULL, spaceDelim); // element = Device Name
    strcat(bgxInfo, element);

    // Print the collected bgx info (ex: "1.  -24   BGX-F625")
    STK_DISPLAY_printStr(i + 1, 1, bgxInfo);
  }
  index = i - 3;
}

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
void UART0_receiveCompleteCb()
{
  STK_RX_initRxBuffer();
  bufferOverflow = true;
}
