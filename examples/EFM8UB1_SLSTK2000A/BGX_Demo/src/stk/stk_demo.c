/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <stdlib.h>
#include <bsp_config.h>
#include <si_toolchain.h>
#include <stdbool.h>
#include <stdint.h>
#include <SI_EFM8UB1_Defs.h>
#include <STRING.H>
#include <tick.h>
#include <joystick.h>
#include <stk_demo.h>
#include "stk_tx.h"
#include "bgx.h"
#include "stk_display.h"
#include "stk_rx.h"
#include <uart_1.h>

// Row number of where the RX section begins
#define RX_START 2
// Number of rows allocated for the RX section
#define NUM_RX_ROWS 13
// Maximum horizontal columns on the LCD
#define MAX_SCREEN_WIDTH 19

// CENTRAL_DEMO or PERIPHERAL_DEMO
bool mode;

/**************************************************************************//**
 * Initializes the UART1 peripheral.
 *****************************************************************************/
void STK_DEMO_UART1_init()
{
  UART1_init(6125000, 115200, UART1_DATALEN_8, UART1_STOPLEN_SHORT,
             UART1_FEATURE_DISABLE, UART1_PARITY_ODD, UART1_RX_ENABLE,
    	     UART1_MULTIPROC_DISABLE);
  UART1_initTxFifo(UART1_TXTHRSH_ZERO, UART1_TXFIFOINT_ENABLE);
  UART1_initRxFifo(UART1_RXTHRSH_ZERO, UART1_RXTIMEOUT_16,
                   UART1_RXFIFOINT_ENABLE);
}

/**************************************************************************//**
 * Initializes the demo to a Central or Peripheral.
 *****************************************************************************/
void STK_DEMO_initMode()
{
  STK_DISPLAY_initChooseMode();
  STK_DEMO_chooseMode();
}

/**************************************************************************//**
 * Displays a screen prompting the user to pick Central of Peripheral mode.
 *
 * If PB1 is pressed, enter Central mode
 * If PB0 is pressed, enter Peripheral mode
 *****************************************************************************/
void STK_DEMO_chooseMode()
{
  while (true)
  {
    // If user chooses CENTRAL
    if (BSP_PB1 == BSP_PB_PRESSED)
    {
      STK_DEMO_initCentral();
      mode = CENTRAL_DEMO;
      break;
    }
    // If user chooses PERIPHERAL
    else if (BSP_PB0 == BSP_PB_PRESSED)
    {
      BGX_enableAdvertising();
      mode = PERIPHERAL_DEMO;
      break;
    }
  }
}

/**************************************************************************//**
 * Initializes the scan and connect demo if CENTRAL_DEMO is chosen.
 *
 * First, scans and connects to a peripheral BGX.
 * Then, launches RX/TX demo.
 *****************************************************************************/
void STK_DEMO_initCentral()
{
  if (MODE_PIN == COMMAND_MODE)
  {
    // Display Central Mode UI
    STK_DISPLAY_initCentral();
    // Initialize buffer for collecting device info
    STK_RX_init();
    // Scan for nearby BGX devices
    STK_DEMO_scanningScreen();
    // Gather device information from scan's output
    STK_RX_parseDevicesFromBuffer();
    // Prompt user to pick BGX device
    STK_TX_getInput();
  }
}

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
void STK_DEMO_scanningScreen()
{
  BGX_scan();
  STK_DISPLAY_scanning();
  BGX_scanOff();
}
