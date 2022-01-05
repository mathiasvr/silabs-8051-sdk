/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef INC_BGX_H_
#define INC_BGX_H_

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
void BGX_init();

/**************************************************************************//**
 * Resets the BGX by toggling the BGX board reset pin.
 *****************************************************************************/
void BGX_reset();

/**************************************************************************//**
 * Sets the BGX and STK baud rate to 9600 to enable a lower power mode.
 *****************************************************************************/
void BGX_initBaudRate();

/**************************************************************************//**
 * Initializes the BGX-Mode-Detecting GPIO Pin.
 *****************************************************************************/
void BGX_setModePin();

/**************************************************************************//**
 * Initializes the BGX-Connection-Detecting GPIO Pin.
 *****************************************************************************/
void BGX_setConnectionPin();

/**************************************************************************//**
 * Saves the BGX configuration.
 *****************************************************************************/
void BGX_save();

/**************************************************************************//**
 * Gets and stores the device name in the global deviceName.
 *****************************************************************************/
void BGX_getDeviceName();

/**************************************************************************//**
 * Disables the BGX from sending advertisements.
 *****************************************************************************/
void BGX_disableAdvertising();

/**************************************************************************//**
 * Enables the BGX to being sending advertisements at a high rate.
 *****************************************************************************/
void BGX_enableAdvertising();

/**************************************************************************//**
 * BGX starts scanning for advertising BGX devices and streams their info back.
 *****************************************************************************/
void BGX_scan();

/**************************************************************************//**
 * BGX stops scanning for advertising BGX devices.
 *****************************************************************************/
void BGX_scanOff();

/**************************************************************************//**
 * The BGX connects to a specified peripheral BGX device.
 *****************************************************************************/
void BGX_connect();

/**************************************************************************//**
 * Gets the Bluetooth address of the BGX.
 *****************************************************************************/
void BGX_getBluetoothAddress();

/**************************************************************************//**
 * Gets the version of the BGX.
 *****************************************************************************/
void BGX_getVersion();

/**************************************************************************//**
 * Sends the command to the BGX via UART.
 *****************************************************************************/
void BGX_sendCommand(char* command);


#endif /* INC_BGX_H_ */
