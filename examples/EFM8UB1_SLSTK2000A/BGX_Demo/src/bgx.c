/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <SI_EFM8UB1_Defs.h>
#include <SI_EFM8UB1_Register_Enums.h>
#include <STRING.H>
#include <tick.h>
#include "bgx.h"
#include "stk_rx.h"
#include "stk_display.h"
#include <uart_1.h>

#define UART1_SFR_PAGE 0x20
#define DECL_PAGE uint8_t savedPage
// enter autopage section
#define SET_PAGE(p)     do                                                    \
                        {                                                     \
                          savedPage = SFRPAGE;  /* save current SFR page */   \
                          SFRPAGE = (p);        /* set SFR page */            \
                        } while(0)
// exit autopage section
#define RESTORE_PAGE    do                                                    \
                        {                                                     \
                          SFRPAGE = savedPage;  /* restore saved SFR page */  \
                        } while(0)

// Length to hold 1 device name
#define DEVICE_NAME_LENGTH              15
// Command length
#define COMMAND_LENGTH					30

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
  Wait(500);
  // Clear any residual characters
  BGX_sendCommand("\r");
}

/**************************************************************************//**
 * Initializes the BGX-Mode-Detecting GPIO Pin.
 *****************************************************************************/
void BGX_setModePin()
{
  Wait(1000);
  BGX_sendCommand("gfu 6 none");
  Wait(500);
  BGX_sendCommand("gfu 6 str_active_n");
}

/**************************************************************************//**
 * Initializes the BGX-Connection-Detecting GPIO Pin.
 *****************************************************************************/
void BGX_setConnectionPin()
{
  Wait(500);
  BGX_sendCommand("gfu 4 none");
  Wait(500);
  BGX_sendCommand("gfu 4 con_active");
}

/**************************************************************************//**
 * Saves the BGX configuration.
 *****************************************************************************/
void BGX_save()
{
  Wait(200);
  BGX_sendCommand("save");
  Wait(200);
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
  BGX_sendCommand("\r\nget sy d n");
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
  DECL_PAGE;

  strcpy(commandBuffer, command);
  strcat(commandBuffer, "\r\0");

  SET_PAGE(UART1_SFR_PAGE);
  IE_EA = 0;
  UART1_writeBuffer(commandBuffer, strlen(commandBuffer));
  IE_EA = 1;

  RESTORE_PAGE;
}
