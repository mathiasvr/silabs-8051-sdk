/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef INC_STK_DEMO_H_
#define INC_STK_DEMO_H_

/**************************************************************************//**
 * Initializes the demo to a Central or Peripheral.
 *****************************************************************************/
void STK_DEMO_initMode();

/**************************************************************************//**
 * Displays a screen prompting the user to pick Central of Peripheral mode.
 *
 * If PB1 is pressed, enter Central mode
 * If PB0 is pressed, enter Peripheral mode
 *****************************************************************************/
void STK_DEMO_chooseMode();

/**************************************************************************//**
 * Initializes the scan and connect demo if CENTRAL_DEMO is chosen.
 *
 * First, scans and connects to a peripheral BGX.
 * Then, launches RX/TX demo.
 *****************************************************************************/
void STK_DEMO_initCentral();

/**************************************************************************//**
 * Displays a waiting screen while running a background scan for BGX devices.
 *
 * PB0 or PB1 must be pressed to halt the scan. This allows a variable,
 * user-defined amount of time to scan for BGX devices. Thus, this accounts
 * for the various advertisement rates a BGX can be set to. Once the scan is
 * halted, the results of the scan will be displayed on the next screen.
 *
 * The results of a scan are indexed as their first advertisement is seen.
 * Meaning the first detected BGX will be 1, second will be 2, etc. There
 * is no correlation to RSSI (received signal strength indicator).
 *****************************************************************************/
void STK_DEMO_scanningScreen();

#endif /* INC_STK_DEMO_H_ */
