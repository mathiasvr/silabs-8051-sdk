//-----------------------------------------------------------------------------
// EFM8SB1_CPT112S_Demo.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// This demo demonstrates the capacitive button function of CPT112S fixed
// function board. This demo runs on the EFM8SB1 STK, and communicates with
// CPT112S through the SMBUS pins on the EXP header. Virtual buttons on
// STK's LCD screen indicate whether the capacitive sensing buttons on CPT112S
//are pressed or released.
// This demo supports 11 buttons on the CPT112S device.(The CS11 button is
// disabled in the default configuration of the board to enable the buzzer)
//-----------------------------------------------------------------------------
// How To Test: EFM8SB1 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Attach CPT112S board to the STK through EXP headers
// 3) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) The demo program should start with a set of circles on display and by
//    pressing/releasing capacitive buttons on CPT112S, the circles turns
//    ON/OFF. On touching the slider, a circle appears on the LCD screen
//	  indicating the position of the finger on the slider
//
// Target:         EFM8SB1
// Tool chain:     Generic
//
// Release 0.1 (MR/CC)
//    - Initial Revision
//    - 29 Oct 2015
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 24.5 MHz HFOSC / 1
// UART0  - 115200 baud, 8-N-1
// SPI0   - 1 MHz
// Timer0 - 400kHz (SMBUS/i2c)
// Timer1 - 231 kHz (UART0)
// Timer2 - 2 MHz (SPI0 CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.4 - UART0_TX
// P0.5 - UART0_RX
// P0.6 - LCD SCK
// P1.0 - LCD MOSI
// P0.1 - LCD CS (Active High)
// P2.6 - Display enable
// P1.2 -> SDA (SMBus)
// P1.3 -> SCL (SMBus)

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
/* basic libraries */
#include <si_toolchain.h>
#include <stdint.h>
#include <stdbool.h>
#include "retargetserial.h"

/* configs and init libraries */
#include "SI_EFM8SB1_Register_Enums.h"
#include "InitDevice.h"
#include "bsp.h"
#include "project_config.h"

/* project library */
#include "smbus.h"
#include "lcd.h"
#include "uart.h"
#include "tick.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
  // Disable the watchdog here
}
 
/******************************************************************************
 * main() Routine
 *****************************************************************************/
int main(void)
{
  // Enter default mode
  enter_DefaultMode_from_RESET();

  // Extra initializations
  initSMBUS();
  initLCD();

  // Indicate TX0 ready
  SCON0_TI = 1;

  updateLCD();
  while (1)
  {
    updateSMBUS();
    if((capsenseCurrent != capsensePrevious) || (sliderCurrent != sliderPrevious))
    {
      updateLCD();
      updateUART();
    }
  }                             // Spin forever
}
