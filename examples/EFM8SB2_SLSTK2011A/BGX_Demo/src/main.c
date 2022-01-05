/**************************************************************************//**
 * (C) Copyright 2018 Silicon Labs Inc
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <stk_demo.h>
#include <stk_tx.h>
#include "bsp.h"
#include "efm8_config.h"
#include "tick.h"
#include "InitDevice.h"
#include "stk_display.h"
#include "stk_rx.h"
#include "pwr.h"
#include "bgx.h"

/**************************************************************************//**
 * Program Description:
 *
 * This example demonstrates the stream and command functionality of the BGX
 * in Peripheral and Central mode using the STK, LCD, push buttons, joystick,
 * and the Silabs BGX Commander mobile app.
 *
 * ----------------------------------------------------------------------------
 * How To Test: EFM8SB2 STK + BGX Board
 * ----------------------------------------------------------------------------
 * 1)  Place the SW104 switch in "AEM" mode.
 * 2)  Connect the EFM8SB2 STK board to a PC using a mini USB cable.
 * 3)  Connect the EFM8SB2 STK board to a BGX Board via EXP header.
 * 4)  Compile and download code to the EFM8SB2 STK board.
 *     In Simplicity Studio IDE, select Run -> Debug from the menu bar,
 *     click the Debug button in the quick menu, or press F11.
 * 5)  Run the code.
 *     In Simplicity Studio IDE, select Run -> Resume from the menu bar,
 *     click the Resume button in the quick menu, or press F8.
 * 6)  The BGX DEMO should start and its UI should be displayed on the LCD.
 * 
 * PERIPHERAL MODE:
 * 7)  Follow the on screen instructions to set the BGX as a Peripheral 
 *     device. The screen will end up in the RX/TX UI display.
 * 8)  Press PB0 or PB1 to have the SB2 send text to the BGX Board via UART1.
 *     TX sent from the SB2 STK board will be displayed on the LCD.
 * 9)  If the BGX was set to an unconnected Peripheral mode, the buttons are in
 *     LOCAL_COMMAND_MODE and will send commands. Output of these commands can
 *     be seen in the RX Data on the LCD.
 *        PB0 is a get firmware version command.
 *        PB1 is a get Bluetooth address command.
 * 10) If set in an unconnected Peripheral, open the Silabs BGX Commander 
 *     mobile app.
 * 11) Connect to the BGX on the mobile app and enter STREAM_MODE
 * 12) Since the mobile app is connected to the BGX, the buttons are in
 *     STREAM_MODE and will send text. Use the mobile app's STREAM_MODE to have
 *     the BGX send text to the SB2 via UART1. RX received by the EFM8SB2
 *     STK board will be displayed on the LCD.
 *        PB0 sends "PB0 was Pressed!"
 *        PB1 sends "PB1 was Pressed!"
 * 13) Use the mobile app's COMMAND_MODE to toggle an LED on the EXP Board
 *        gfu 3 stdio  // set traffic LED as a stdio
 *        gdi 3 olo    // set output direction to low
 *        gdi 3 ohi    // set output direction to high
 * 
 * CENTRAL MODE:
 * 7)  Follow the on screen instructions to set the BGX as a Central.
 *     The instructions will help establish a connection to a nearby
 *     Peripheral BGX. The screen will end up in the RX/TX UI display.
 * 8)  Since the BGX is connected to another BGX, the buttons are in
 *     STREAM_MODE and will send text. Use either STK's buttons to send text
 *     to the SB2 via UART1. The text will be transmitted to the connected
 *     BGX via Bluetooth. RX received by the other EFM8SB2 STK board will be 
 *     displayed on the LCD.
 *        PB0 sends "PB0 was Pressed!"
 *        PB1 sends "PB1 was Pressed!"
 * 
 * NOTE: If a change is made to EFM8SB2_BGX.hwconf, an Interrupts.c will be
 *       generated. Interrupts.c must be deleted to avoid compilation
 *       errors.
 * 
 * Target:         EFM8SB2
 * Tool chain:     Generic
 *
 * Release 0.1 (MD;AT)
 *    - Initial Revision
 *    - 7 JUN 2018
 *
 *-----------------------------------------------------------------------------
 * Resources:
 *-----------------------------------------------------------------------------
 * SYSCLK - 24.5 MHz HFOSC / 4
 * SPI1   - 500 kHz
 * Timer2 - 2 MHz (SPI CS delay)
 * Timer3 - 1 kHz (1 ms tick)
 * P0.2 - Push Button 0
 * P0.3 - Push Button 1
 * P0.4 - UART1 TX
 * P0.5 - UART1 RX
 * P1.0 - LCD SCK
 * P1.2 - LCD MOSI
 * P1.5 - LCD CS (Active High)
 * P1.6 - BGX Mode GPIO (0 - COMMAND_MODE / 1 - STREAM_MODE)
 * P2.0 - LED Green
 * P2.1 - LED Blue
 * P2.2 - LED Red
 * P2.6 - Display enable
 * ----------------------------------------------------------------------------
 *
 *****************************************************************************/

/**************************************************************************//**
 * SiLabs_Startup Routine.
 *
 * This function is called immediately after reset, before the initialization
 * code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
 * useful place to disable the watchdog timer, which is enable by default
 * and may trigger before main() in some instances.
 *****************************************************************************/
void SiLabs_Startup(void)
{
  // Disable the watchdog here
}
 
/**************************************************************************//**
 * This example shows us the capabilities of the BGX board.
 *
 * This example demonstrates the stream and command functionality of the BGX
 * in Peripheral and Central mode using the STK, LCD, push buttons, joystick,
 * and the Silabs BGX Commander mobile app.
 *****************************************************************************/
void main(void)
{
  // Initialize the device
  enter_DefaultMode_from_RESET();
  // Board controller disconnected from EFM8 UART pins
  BSP_BC_EN = BSP_BC_DISCONNECTED;
  // Resets the BGX board and initializes it's baud rates and GPIOs
  BGX_init();
  // Initialize the commands and stream text used for TX
  STK_TX_init();
  // Initializes demo mode 1) CENTRAL 2) PERIPHERAL
  STK_DEMO_initMode();
  // Initialize LCD Screen
  STK_DISPLAY_init();
  // Clears and resets the buffers and targets Buffer A
  STK_RX_init();
  // Enable all interrupts
  IE_EA = 1;

  // Mainloop
  while (true)
  {
    STK_RX_handleRx();
    STK_TX_handleTx();
    PWR_enterIdle();
  }
}
