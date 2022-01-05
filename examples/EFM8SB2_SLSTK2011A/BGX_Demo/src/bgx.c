/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <SI_EFM8SB2_Defs.h>
#include <STRING.H>
#include <tick.h>
#include <uart_0.h>
#include "bgx.h"
#include "stk_rx.h"
#include "stk_display.h"

// Length to hold 1 device name
#define DEVICE_NAME_LENGTH              15
// Command length
#define COMMAND_LENGTH					22

// String to hold command
SI_SEGMENT_VARIABLE(commandBuffer[COMMAND_LENGTH], uint8_t, SI_SEG_XDATA);
// String to hold device name
SI_SEGMENT_VARIABLE(deviceName[DEVICE_NAME_LENGTH], uint8_t, SI_SEG_XDATA);
// Buffer to gather BGX device info
extern SI_SEGMENT_VARIABLE(rxBuffer[BUFFER_LENGTH], uint8_t, SI_SEG_XDATA);
// String to hold "#\r" where # is the index of the BGX
extern SI_SEGMENT_VARIABLE(pickDevicePointer[2], uint8_t, SI_SEG_XDATA);

/**************************************************************************//**
 * Initializes the BGX to be demo compatible and in a lower-power mode.
 *
 * This involves:
 * Reset BGX.
 * Optional: Call BGX_initBaudRate() to enable lower power mode
 * Enable a gpio pin to detect the GPIO mode.
 * Enable a gpio pin to detect the connection status.
 * Save the BGX config.
 * Set up Rx buffer to store BGX data received data
 * Gather the BGX device name.
 * Disable it from advertising itself until the user makes it a peripheral.
 *****************************************************************************/
void BGX_init()
{
  BGX_reset();
  BGX_initBaudRate();
  BGX_setModePin();
  BGX_setConnectionPin();
  BGX_save();
  STK_RX_init();
  BGX_getDeviceName();
  BGX_disableAdvertising();
}

/**************************************************************************//**
 * Resets the BGX by toggling the BGX board reset pin.
 *****************************************************************************/
void BGX_reset()
{
  BGX_RESET_PIN = BGX_RESET_PIN&0;
  Wait(20);
  BGX_RESET_PIN = BGX_RESET_PIN|1;
  Wait(1500);
  // Clear any residual characters
  BGX_sendCommand("\r");
}

/**************************************************************************//**
 * Sets the BGX and STK baud rate to 9600 to enable a lower power mode.
 * 
 * If the BGX is set to 115200, it is changed to 9600.
 * If the BGX is set to 9600, it is unchanged.
 * If the BGX is set to neither, the STK will not be able to communicate to it.
 *****************************************************************************/
void BGX_initBaudRate()
{
  // In COMMAND_MODE execute initialization commands
  if (MODE_PIN == COMMAND_MODE)
  {
    // Set SB2 UART Baud Rate to 115200
    CKCON0 = CKCON0 | CKCON0_T1M__SYSCLK;

    // Change BGX UART Baud Rate to 9600
    BGX_sendCommand("");
    BGX_sendCommand("set ua b 9600\ruartu");

    // Change SB2 UART Baud Rate to 9600
    Wait(300);
    CKCON0 = CKCON0 & ~CKCON0_T1M__SYSCLK;
  }
  // In STREAM_MODE assume BGX is connected and initialized and only change SB2 UART Baud Rate to 9600
  else if (MODE_PIN == STREAM_MODE)
  {
    CKCON0 = CKCON0 & ~CKCON0_T1M__SYSCLK;
  }
}

/**************************************************************************//**
 * Initializes the BGX-Mode-Detecting GPIO Pin.
 *****************************************************************************/
void BGX_setModePin()
{
  // In COMMAND_MODE execute initialization commands
  if (MODE_PIN == COMMAND_MODE)
  {
	BGX_sendCommand("gfu 6 none");
	BGX_sendCommand("gfu 6 str_active_n");
  }
}

/**************************************************************************//**
 * Initializes the BGX-Connection-Detecting GPIO Pin.
 *****************************************************************************/
void BGX_setConnectionPin()
{
  Wait(100);
  BGX_sendCommand("gfu 4 none");
  BGX_sendCommand("gfu 4 con_active");
}

/**************************************************************************//**
 * Saves the BGX configuration.
 *****************************************************************************/
void BGX_save()
{
  Wait(100);
  BGX_sendCommand("save");
}

/**************************************************************************//**
 * Gets and stores the device name in the global deviceName.
 *****************************************************************************/
void BGX_getDeviceName()
{
  SI_SEG_XDATA char *lineToken;
  char *deviceNameCommandPointer = NULL;
  const char returnDelim[3] = "\r\n";

  // Sends command to get device name
  Wait(100);
  BGX_sendCommand("get sy d n");
  Wait(100);
  STK_DISPLAY_clearLog(0);

  // Parses the result to retrieve the device name
  lineToken = strtok(rxBuffer, returnDelim);
  while (lineToken != NULL)
  {
    lineToken = strtok(NULL, returnDelim);
    deviceNameCommandPointer = strstr(lineToken, "get sy d n");
    if (deviceNameCommandPointer)
    {
      lineToken = strtok(NULL, returnDelim);
      strcpy(deviceName, lineToken);
      break;
    }
  }
}

/**************************************************************************//**
 * Disables the BGX from sending advertisements.
 *****************************************************************************/
void BGX_disableAdvertising()
{
  BGX_sendCommand("adv off");
}

/**************************************************************************//**
 * Enables the BGX to being sending advertisements at a high rate.
 *****************************************************************************/
void BGX_enableAdvertising()
{
  BGX_sendCommand("adv high");
}

/**************************************************************************//**
 * BGX starts scanning for advertising BGX devices and streams their info back.
 *****************************************************************************/
void BGX_scan()
{
  BGX_sendCommand("scan");
}

/**************************************************************************//**
 * BGX stops scanning for advertising BGX devices.
 *****************************************************************************/
void BGX_scanOff()
{
  BGX_sendCommand("scan off");
}

/**************************************************************************//**
 * The BGX connects to a specified peripheral BGX device.
 *****************************************************************************/
void BGX_connect()
{
  char connectCommand[6] = "con ";
  strcat(connectCommand, pickDevicePointer);
  BGX_sendCommand(connectCommand);
}

/**************************************************************************//**
 * Gets the Bluetooth address of the BGX.
 *****************************************************************************/
void BGX_getBluetoothAddress()
{
  BGX_sendCommand("get bl a");
}

/**************************************************************************//**
 * Gets the version of the BGX.
 *****************************************************************************/
void BGX_getVersion()
{
  BGX_sendCommand("get sy v");
}

/**************************************************************************//**
 * Sends the command to the BGX via UART.
 *****************************************************************************/
void BGX_sendCommand(char* command)
{
  strcpy(commandBuffer, command);
  strcat(commandBuffer, "\r\0");

  IE_EA = 0;
  UART0_writeBuffer(commandBuffer, strlen(commandBuffer));
  IE_EA = 1;
}
