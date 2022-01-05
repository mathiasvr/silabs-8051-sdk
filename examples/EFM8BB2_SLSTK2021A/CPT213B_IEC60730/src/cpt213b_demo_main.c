//-----------------------------------------------------------------------------
// EFM8BB2_CPT213B_Demo.c
//-----------------------------------------------------------------------------
// Copyright 2017 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This demo demonstrates the capacitive button function of CPT213B fixed
// function board with a IEC compliant host device. This demo runs on the
// IEC60730 software compliant EFM8BB2 STK, and communicates with
// CPT213B through the SMBUS pins on the EXP header. Virtual buttons on
// STK's LCD screen indicate whether the capacitive sensing buttons on CPT213B
// are pressed or released.
//
// This demo also shows the run time config load feature of the CPT213B device.
// When either of the two push buttons are pressed, different configurations are
// loaded onto the CPT device. Pushing PB0 on the STK activates all the 13
// capactive touch buttons on the CPT device while PB1 activates only the middle
// row.
//
// The following points are critical to note before running the example code.
//
// 1) The code assumed that you have a valid configuration profile loaded
//    prior to executing the code. If that is not the case, first navigate
//    to Xpress configurator and load a valid configuration there.
// 2) This code is written to function with a slave address of 0xF0. So, make
//    sure the I2C slave address is set to 0xF0 in the Xpress configurator.
// 3) Since the code is time sensitive, make sure to release the push buttons
//    as soon as they are pressed. If you press and hold the push buttons for a
//    long time, the host device can go into a safe state.
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB2 STK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Attach CPT213B board to the STK through EXP headers
// 3) Connect the EFM8BB2 STK board to a PC using a mini USB cable.
// 4) Compile and download code to the EFM8BB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) The demo program should start with a set of circles on display and by
//    pressing/releasing capacitive buttons on CPT213B, the circles turns
//    ON/OFF.
// 7) Press Push Button 1 and release it quickly. A different configuration is
//    loaded onto the device and now only the middle row is active on the CPT
//    board. Touching the top or bottom row will not activate the buttons on the
//    LCD screen.
// 8) Press Push Button 0 and release it quickly. Now, all the 13 buttons are active
//    and the same is reflected on the LCD screen
//
// Target:         EFM8BB2
// Tool chain:     Generic
//
// Release 0.1 (MR/AS)
//    - Initial Revision
//    - 30 Jun 2017
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 24.5 MHz HFOSC / 1
// UART0  - 115200 baud, 8-N-1
// SPI0   - 1 MHz
// Timer0 - 400kHz  (SMBUS/i2c)
// Timer1 - 231 kHz (UART0)
// PCA CH1 - 2 MHz  (SPI0 CS delay)
// PCA CH2 - 1 kHz  (1 ms tick)
// Timer 2 - 100 Hz (IEC tick timer)
// Timer 3 - Used for WDOG errata on EFM8 (refer to errata for more info)
// Timer 4 - 10 Hz  (IEC system timer)
// P0.4 - UART0_TX
// P0.5 - UART0_RX
// P0.6 - LCD SCK
// P0.7 - CPT Interrupt Pin
// P1.0 - LCD MOSI
// P1.1 - Pin to reset the CPT device
// P0.1 - LCD CS (Active High)
// P2.6 - Display enable
// P1.2 -> SDA (SMBus)
// P1.3 -> SCL (SMBus)
// WDOG -> Resets every 51 ms

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>
#include <stdint.h>
#include <stdbool.h>
#include <cpt213b_oem_plausibility.h>
#include "iec60730.h"

/* configs and init libraries */
#include "SI_EFM8BB2_Register_Enums.h"
#include "InitDevice.h"
#include "project_config.h"
#include "retargetserial.h"
#include "bsp.h"
#include "cpt213b_state_machine.h"
/* project library */
#include "i2c_0.h"
#include "lcd.h"
#include "uart.h"
#include "tick.h"
#include "smbus.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

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
 
// $[Generated Includes]
// [Generated Includes]$
/**************************************************************************//**
 * public main() routine
 *
 * @returns int Not used.
 * 
 * Main function for the IEC60730 demo. Includes initialization, POST,
 * BIST, Analog Plausibility and GPIO Plausibility.
 *
 * Before the host enters the infinite loop, the CPT device has been configured
 * to enter sensing mode. The assumption made here is that the CPT device is
 * pre-loaded with a valid configuration. Properly following all the steps listed
 * in the Readme or the main file is critical to get the CPT device and the host
 * to work in cohesion.
 *
 * The state machine is called continuously in the infinite while loop in
 * order to make sure that the timing constraints of the IEC60730 are
 * maintained. Only when a button press and release is detected, the LCD
 * and UART are updated.
 *
 * Infinite loop, does not return.
 *****************************************************************************/
int main(void) {

  // Call hardware initialization routine
  enter_DefaultMode_from_RESET();

  // Call the IEC post function
  iec60730_Post();

  // Reset the CPT device
  CPT_reset();
  // Initialize the SMBus
  initSMBUS();
  // Put the CPT device in sensing mode
  CPT_enterSensingMode();
  // Disable I2C, enable SPI and re-enable I2C
  enable_SPI();
  // Initialize SMBus after re-enabling it
  initSMBUS();

  //Initialize and update LCD
  initLCD();
  // Update the Sprites
  updateLCD();
  // Indicate TX0 ready
  SCON0_TI = 1;

  while (1)
  {
	// OEM defined GPIO check
	iec60730_GpioCheck();
	// OEM defined Analog check
	iec60730_AnalogCheck();
	// OEM code will set IEC60730_OEMx_COMPLETE in their code and interrupts
	// Set unused IEC60730_OEMx_COMPLETE bits to 1
	iec60730_programmeCounterCheck |= IEC60730_OEM0_COMPLETE
									| IEC60730_OEM1_COMPLETE
									| IEC60730_OEM2_COMPLETE
									| IEC60730_OEM3_COMPLETE
									| IEC60730_OEM4_COMPLETE
									| IEC60730_OEM5_COMPLETE
									| IEC60730_OEM6_COMPLETE
									| IEC60730_OEM7_COMPLETE;
	iec60730_Bist();

	// Enter the CPT state machine
	if(CPT_state_machine() < ENTER_SAFE_STATE)
	{}

    // Update UART and LCD when a capacitive button is pressed/released
	if(capsenseCurrent != capsensePrevious)
	{
	  updateLCD();
	  updateUART();
	  // Wait till the next touch/release event (Update only once)
	  capsensePrevious = capsenseCurrent;
    }
  }
}
