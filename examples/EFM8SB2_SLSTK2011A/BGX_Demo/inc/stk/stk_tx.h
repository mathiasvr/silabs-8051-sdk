/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef INC_STK_TX_H_
#define INC_STK_TX_H_

/**************************************************************************//**
 * Initializes the two on-board buttons and the text they will transmit.
 * Also sets up BGX-Mode-Detecting GPIO Pin and 9600 baud rate.
 *
 * Both buttons have two modes:
 *
 * 1) STREAM_MODE
 * Sends "PB_ was Pressed!" text to the BGX board over UART.
 *
 * STREAM_MODE is set when the STREAM_GPIO_N(P1.6) is set low. This pin is set
 * low when the BGX board is currently set to STREAM_MODE.
 *
 * 2) COMMAND_MODE
 * Sends (get BlueTooth address) or (get firmware version) commands to the
 * BGX board over UART.
 *
 * COMMAND_MODE is set when the STREAM_GPIO_N(P1.6) is set high. This pin is set
 * high when the BGX board is set to LOCAL_COMMAND_MODE or REMOTE_COMMAND_MODE.
 * In LOCAL_COMMAND_MODE, the BGX responds to commands over UART.
 * However, in REMOTE_COMMAND_MODE, the BGX responds to commands over mobile
 * app so the commands sent by button presses will not be executed.
 *****************************************************************************/
void STK_TX_init();

/**************************************************************************//**
 * Check if either button has been pressed.
 *
 * Both buttons are handled by sw flags and can be pressed simultaneously.
 *****************************************************************************/
void STK_TX_handleTx();

/**************************************************************************//**
 * Check if the right button press was completed.
 *
 * If a press did occur, sends stream text or a command depending on the
 * current mode of the BGX.
 *****************************************************************************/
void STK_TX_checkRightButton();

/**************************************************************************//**
 * Check if the left button press was completed.
 *
 * If a press did occur, sends stream text or a command depending on the
 * current mode of the BGX.
 *****************************************************************************/
void STK_TX_checkLeftButton();

/**************************************************************************//**
 * Print right button STREAM_MODE text.
 *****************************************************************************/
void STK_TX_sendRightButtonStream();

/**************************************************************************//**
 * Print right button COMMAND_MODE text.
 *****************************************************************************/
void STK_TX_sendRightButtonCommand();

/**************************************************************************//**
 * Print left button STREAM_MODE text.
 *****************************************************************************/
void STK_TX_sendLeftButtonStream();

/**************************************************************************//**
 * Print left button COMMAND_MODE text.
 *****************************************************************************/
void STK_TX_sendLeftButtonCommand();

/**************************************************************************//**
 * Prompts the user to pick a BGX, then connects to the specified BGX.
 *
 * The user will scroll through the BGX indexes via the STK joystick.
 * The joystick is pressed to confirm the chosen BGX and will proceed to
 * connect to it.
 *****************************************************************************/
void STK_TX_getInput();

/**************************************************************************//**
 * Returns the state of the joystick.
 *
 * @returns North, South, East, West, None (not pressed), or Center (pressed)
 *****************************************************************************/
void STK_TX_getJoystickDirection();

/***************************************************************************//**
 * Callback for transmission of a data.
 *
 * This function is called when all bytes in the buffer have been transferred.
 * Left blank because it has no need in the demo.
 *
 * @warning
 * This function is called from an ISR and should be as short as possible.
 ******************************************************************************/
void UART0_transmitCompleteCb();

#endif /* INC_STK_TX_H_ */
