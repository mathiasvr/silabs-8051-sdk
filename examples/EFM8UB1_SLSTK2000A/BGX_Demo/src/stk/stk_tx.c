/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <bsp_config.h>
#include <si_toolchain.h>
#include <stdbool.h>
#include <stdint.h>
#include <SI_EFM8UB1_Defs.h>
#include <STRING.H>
#include <joystick.h>
#include <tick.h>
#include "stk_display.h"
#include "bgx.h"
#include "stk_demo.h"
#include "stk_tx.h"
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

// Length of "PB_ was Pressed!\n"
#define STK_TX_LENGTH 18
// Length of "get __ _\r"
#define COMMAND_LENGTH 10
// Index for choosing BGX as a string
#define INDEX_STRING index+48

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// Index for choosing BGX
extern SI_SEG_XDATA uint8_t index;
// String to hold "get sy v\r"
SI_SEGMENT_VARIABLE(getFirmwareVersionCommand[COMMAND_LENGTH], uint8_t, SI_SEG_XDATA);
// String to hold "get bl a\r"
SI_SEGMENT_VARIABLE(getBluetoothAddressCommand[COMMAND_LENGTH], uint8_t, SI_SEG_XDATA);
// String to hold "PB0 was Pressed!\n"
SI_SEGMENT_VARIABLE(rightButtonStream[STK_TX_LENGTH], uint8_t, SI_SEG_XDATA);
// String to hold "PB1 was Pressed!\n"
SI_SEGMENT_VARIABLE(leftButtonStream[STK_TX_LENGTH], uint8_t, SI_SEG_XDATA);
// Cardinal direction and pressed state of the joystick
SI_SEG_XDATA uint8_t joystickDirection = JOYSTICK_NONE;
// String to hold "#\r" where # is the index of the BGX
SI_SEGMENT_VARIABLE(pickDevicePointer[2], uint8_t, SI_SEG_XDATA);
// Flag to indicate if PB0 is currently being pressed
bool ButtonRPressed;
// Flag to indicate if PB1 is currently being pressed
bool ButtonLPressed;
// Loop Index
uint8_t i;

/**************************************************************************//**
 * Initializes the two on-board buttons and the text they will transmit.
 * Also sets up BGX-Mode-Detecting GPIO Pin and 9600 baud rate.
 *
 * Both buttons have two modes:
 *
 * 1) STREAM_MODE
 * Sends "PB_ was Pressed!" text to the BGX board over UART.
 *
 * STREAM_MODE is set when the STREAM_GPIO (P1.6) is set high. This pin is set
 * high when the BGX board is currently set to STREAM_MODE.
 *
 * 2) COMMAND_MODE
 * Sends (get BlueTooth address) or (get firmware version) commands to the
 * BGX board over UART.
 *
 * COMMAND_MODE is set when the STREAM_GPIO (P1.6) is set low. This pin is set
 * low when the BGX board is set to LOCAL_COMMAND_MODE or REMOTE_COMMAND_MODE.
 * In LOCAL_COMMAND_MODE, the BGX responds to commands over UART.
 * However, in REMOTE_COMMAND_MODE, the BGX responds to commands over mobile app
 * so the commands sent by button presses will not be executed.
 *****************************************************************************/
void STK_TX_init()
{
  ButtonRPressed = false;
  ButtonLPressed = false;

  strcpy(rightButtonStream, "PB0 was Pressed!\n");
  strcpy(leftButtonStream, "PB1 was Pressed!\n");

  strcpy(getFirmwareVersionCommand, "get sy v\r");
  strcpy(getBluetoothAddressCommand, "get bl a\r");
}

/**************************************************************************//**
 * Check if either button has been pressed.
 *
 * Both buttons are handled by sw flags and can be pressed simultaneously.
 *****************************************************************************/
void STK_TX_handleTx()
{
  STK_TX_checkRightButton();
  STK_TX_checkLeftButton();
}

/**************************************************************************//**
 * Check if the right button press was completed.
 *
 * If a press did occur, sends stream text or a command depending on the
 * current mode of the BGX.
 *****************************************************************************/
void STK_TX_checkRightButton()
{
  //If right button was pressed
  if(BSP_PB0 == BSP_PB_PRESSED)
  {
    ButtonRPressed = true;
  }
  //If right button was not pressed OR it was released
  else
  {
  //If button was released
    if (ButtonRPressed)
    {
      if (MODE_PIN == COMMAND_MODE)
      {
        STK_TX_sendRightButtonCommand();
      }
      else if (MODE_PIN == STREAM_MODE)
	  {
        STK_TX_sendRightButtonStream();
	  }
      ButtonRPressed = false;
    }
  }
}

/**************************************************************************//**
 * Check if the left button press was completed.
 *
 * If a press did occur, sends stream text or a command depending on the
 * current mode of the BGX.
 *****************************************************************************/
void STK_TX_checkLeftButton()
{
  //If left button was pressed
  if (BSP_PB1 == BSP_PB_PRESSED)
  {
    ButtonLPressed = true;
  }
  //If left button was not pressed OR it was released
  else
  {
    //If button was released
    if (ButtonLPressed)
    {
      if (MODE_PIN == COMMAND_MODE)
      {
        STK_TX_sendLeftButtonCommand();
      }
      else if (MODE_PIN == STREAM_MODE)
      {
        STK_TX_sendLeftButtonStream();
      }
      ButtonLPressed = false;
    }
  }
}

/**************************************************************************//**
 * Print right button STREAM_MODE text.
 *****************************************************************************/
void STK_TX_sendRightButtonStream()
{
  DECL_PAGE;

  // Copy data to display
  // Do this first so UART doesn't flush data before
  // display update is finished.
  for (i = 0; i < STK_TX_LENGTH - 1; i++)
  {
    STK_DISPLAY_printTx(rightButtonStream[i]);
  }

  // We need to avoid having the UART ISR fire when we are
  // modifying the library. However we also don't want lower
  // priority interrupts to fire so we turn off all interrupts
  IE_EA = 0;
  SET_PAGE(UART1_SFR_PAGE);
  UART1_writeBuffer(rightButtonStream, STK_TX_LENGTH - 1);
  RESTORE_PAGE;
  IE_EA = 1;
}

/**************************************************************************//**
 * Print right button COMMAND_MODE text.
 *****************************************************************************/
void STK_TX_sendRightButtonCommand()
{
  // Copy data to display
  // Do this first so UART doesn't flush data before
  // display update is finished.
  for (i = 0; i < COMMAND_LENGTH - 1; i++)
  {
    STK_DISPLAY_printTx(getFirmwareVersionCommand[i]);
  }
  BGX_getVersion();
}

/**************************************************************************//**
 * Print left button STREAM_MODE text.
 *****************************************************************************/
void STK_TX_sendLeftButtonStream()
{
	DECL_PAGE;
  // Copy data to display
  // Do this first so UART doesn't flush data before
  // display update is finished.
  for (i = 0; i < STK_TX_LENGTH - 1; i++)
  {
    STK_DISPLAY_printTx(leftButtonStream[i]);
  }
  // We need to avoid having the UART ISR fire when we are
  // modifying the library. However we also don't want lower
  // priority interrupts to fire so we turn off all interrupts
  IE_EA = 0;
  SET_PAGE(UART1_SFR_PAGE);
  UART1_writeBuffer(leftButtonStream, STK_TX_LENGTH - 1);
  RESTORE_PAGE;
  IE_EA = 1;
}

/**************************************************************************//**
 * Print left button COMMAND_MODE text.
 *****************************************************************************/
void STK_TX_sendLeftButtonCommand()
{
  // Copy data to display
  // Do this first so UART doesn't flush data before
  // display update is finished.
  for (i = 0; i < COMMAND_LENGTH - 1; i++)
  {
    STK_DISPLAY_printTx(getBluetoothAddressCommand[i]);
  }
  BGX_getBluetoothAddress();
}

/**************************************************************************//**
 * Prompts the user to pick a BGX, then connects to the specified BGX.
 *
 * The user will scroll through the BGX indexes via the STK joystick.
 * The joystick is pressed to confirm the chosen BGX and will proceed to
 * connect to it.
 *****************************************************************************/
void STK_TX_getInput()
{
  STK_DISPLAY_selectDevice();
  strcpy(pickDevicePointer, "1");

  // Allow user to choose BGX to connect to
  while (true)
  {
    // Detect joystick direction
    STK_TX_getJoystickDirection();

    // Move up the scanned BGX list if joystick is pointed up
    if (joystickDirection == JOYSTICK_N)
    {
      STK_DISPLAY_clearLog(14);
      if (pickDevicePointer[0] > '1')
      {
        pickDevicePointer[0]--;
      }
      STK_DISPLAY_printStr(14, 10, pickDevicePointer);
      Wait(200);
    }

    // Move down the scanned BGX list if joystick if pointed down
    else if (joystickDirection == JOYSTICK_S)
    {
      STK_DISPLAY_clearLog(14);
      if (pickDevicePointer[0] < INDEX_STRING)
      {
        pickDevicePointer[0]++;
      }
      STK_DISPLAY_printStr(14, 10, pickDevicePointer);
      Wait(200);
    }

    // Connect to the specified BGX if any button is pressed
    else if (joystickDirection == JOYSTICK_C)
    {
      BGX_connect();
      break;
    }
  }
}

/**************************************************************************//**
 * Retrieves and stores the state of the joystick.
 *
 * joystickDirection will be one of the following:
 * North, South, East, West, None (not pressed), or Center (pressed)
 *****************************************************************************/
void STK_TX_getJoystickDirection()
{
  uint32_t mV;

  ADC0CN0_ADBUSY = 1;
  while (!ADC0CN0_ADINT);
  ADC0CN0_ADINT = 0;

  mV = (ADC0 * (uint32_t) 3300) / 1023;
  joystickDirection = JOYSTICK_convert_mv_to_direction(mV);
}

/***************************************************************************//**
 * Callback for transmission of a data.
 *
 * This function is called when all bytes in the buffer have been transferred.
 * Clears ISR flags.
 *
 * @warning
 * This function is called from an ISR and should be as short as possible.
 ******************************************************************************/
void UART1_transmitCompleteCb()
{
  // Clear flags
  UART1FCN0 &= ~UART1FCN0_TFRQE__ENABLED;
}
