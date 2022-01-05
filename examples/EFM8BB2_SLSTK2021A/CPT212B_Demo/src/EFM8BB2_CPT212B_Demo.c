//----------------------------------------------------------------------------
//	CPT212B Demo
//----------------------------------------------------------------------------
//
//	This demo demonstrates the capacitive button function of CPT212B fixed
//	function board. This demo runs on the EFM8BB2 STK, and communicates with
//	CPT212B through the SMBUS pins on the EXP header. Virtual buttons on
//	STK's LCD screen indicate whether the capacitive sensing buttons on CPT212B
//	are pressed or released.
//
//	This demo also shows the run time config load feature of the CPT212B device.
//	When either of the two push buttons are pressed, different configurations are
//	loaded onto the CPT device. Pushing PB0 on the STK uses a long duration of
//	the buzzer and uses a low frequency buzzer output while PB1 uses a long duration
//	of the buzzer and uses a high frequency buzzer output.
//
//
//	The following points are critical to note before running the example code.
//
//	1) The code assumed that you have a valid configuration profile loaded
//	   prior to executing the code. If that is not the case, first navigate
//	   to Xpress configurator and load a valid configuration there.
//	2) This code is written to function with a slave address of 0xF0. So, make
//	   sure the I2C slave address is set to 0xF0 in the Xpress configurator.
//
//----------------------------------------------------------------------------
//	How To Test: EFM8BB2 STK
//----------------------------------------------------------------------------
//	1) Place the switch in "AEM" mode.
//	2) Attach CPT212B board to the STK through EXP headers
//	3) Connect the EFM8BB2 STK board to a PC using a mini USB cable.
//	4) Compile and download code to the EFM8BB2 STK board.
//	   In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//	   click the Debug button in the quick menu, or press F11.
//	5) Run the code.
//	   In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//	   click the Resume button in the quick menu, or press F8.
//	6) The demo program should start with a set of circles on display and by
//	   pressing/releasing capacitive buttons on CPT212B, the circles turns
//	   ON/OFF.
//	7) Press Push Button 1. A different configuration is
//	   loaded onto the device, and you should be able to hear the buzzer outputting
//	   a long duration buzzer at a low pitch frequency.
//	8) Press Push Button 0. Now, the buzzer duration for
//	   a touch will be long but with a high pitch frequency.
//
//	Target:         EFM8BB2
//	Tool chain:     Generic
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
#include "SI_EFM8BB2_Register_Enums.h"
#include "InitDevice.h"
#include "bsp.h"
#include "project_config.h"
#include "cpt212b_a01_gm_init.h"

/* project library */
#include "smbus.h"
#include "i2c_0.h"
#include "lcd.h"
#include "uart.h"
#include "tick.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
SI_LOCATED_VARIABLE_NO_INIT(reserved, uint8_t, SI_SEG_XDATA, 0x0000);
SI_SBIT(PB0, SFR_P0, 2);
SI_SBIT(PB1, SFR_P0, 3);
SI_SBIT(LED, SFR_P1, 4);
void disable_SPI(void);
void enable_SPI(void);

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

  // Initialize the SMBus
  initSMBUS();

  disable_SPI();

  while(CPT_config((uint8_t*)&CPT_CONFIG) < RESET_STATE);

  // Enable SPI
  enable_SPI();

  //Initialize and update LCD
  initLCD();
  updateLCD();

  // Indicate TX0 ready
  SCON0_TI = 1;

  while (1)
  {
	// Enable port match only when there is a pin transition
	if(CAPSENSE_INT == 1)
	{
		EIE1 |= 0x02;
	}

	if(!PB0)
	{
		disable_SPI();
		CPT_CONFIG.buzzer_output_duration = 0xff;
		CPT_CONFIG.buzzer_output_frequency = 10;
		while(CPT_config((uint8_t*)&CPT_CONFIG) < RESET_STATE);

		// Enable SPI
		enable_SPI();
		PB0 = 1;
	}
	if(!PB1)
	{

		disable_SPI();
		CPT_CONFIG.buzzer_output_duration = 0xff;
		CPT_CONFIG.buzzer_output_frequency = 100;
		while(CPT_config((uint8_t*)&CPT_CONFIG) < RESET_STATE);

		// Enable SPI
		enable_SPI();
		PB1 = 1;
	}

	// Update UART and LCD when a button is pressed/released
	if(capsenseCurrent != capsensePrevious)
    {
      updateLCD();
      updateUART();
      // Wait till the next touch/release event (Update only once)
      capsensePrevious = capsenseCurrent;
    }

  }                             // Spin forever
}


/******************************************************************************
 *
 * Enable_SPI() Routine
 *
 * Changes pin skipping and pin settings to allow SPI to be enable without
 * any impact to I2C. If this is not done, the I2C pins will be temporarily
 * shifted causing an error/NAK which puts the device in a safe state.
 * INT pin cannot be controlled in this state.
 *
 *****************************************************************************/
void enable_SPI()
{
  // Disable crossbar
  XBR2 &= ~XBR2_XBARE__ENABLED;

  SFRPAGE = 0x00;
  // Change port pin configurations for SPI
  P0MDOUT &= ~P0MDOUT_B7__PUSH_PULL;
  // Remove the skip from the SPI pins
  P0SKIP &= ~(P0SKIP_B6__SKIPPED
    | P0SKIP_B7__SKIPPED);
  P1SKIP &= ~(P1SKIP_B0__SKIPPED);
  // Enable port match on pin 0.7
  P0MASK |= P0MASK_B7__COMPARED;
  // Enable 3-wire SPI Master
  SPI0CFG |= SPI0CFG_MSTEN__MASTER_ENABLED;
  SPI0CN0 &= ~SPI0CN0_NSSMD__FMASK;	// [P1SKIP - Port 1 Skip]$
  SPI0CN0 |= SPI0CN0_SPIEN__ENABLED;
  // Enable SPI on the XBar
  XBR0 |= XBR0_SPI0E__ENABLED;
  // Enable XBar
  XBR2 |= XBR2_XBARE__ENABLED;
}

/******************************************************************************
 *
 * Disable_SPI() Routine
 *
 * Changes pin skipping and pin settings to allow SPI to be disabled without
 * any impact to I2C. If this is not done, the I2C pins will be temporarily
 * shifted causing an error/NAK which puts the device in a safe state.
 * INT pin cannot be controlled in this state.
 * INT pin will be able to be controlled by MCU after this routine.
 *
 *****************************************************************************/
void disable_SPI()
{
  // Disable crossbar
  XBR2 &= ~XBR2_XBARE__ENABLED;

  SFRPAGE = 0x00;
  // Change port pin configurations for INT pin to be push pull
  P0MDOUT |= P0MDOUT_B7__PUSH_PULL;
  // Add skip to replace the SPI pins
  P0SKIP |= (P0SKIP_B6__SKIPPED
    | P0SKIP_B7__SKIPPED);
  P1SKIP |= (P1SKIP_B0__SKIPPED);
  // Disable port match on pin 0.7
  P0MASK &= ~P0MASK_B7__COMPARED;
  // Disablee SPI Master
  SPI0CN0 &= ~SPI0CN0_SPIEN__ENABLED;
  // Disable SPI on the XBar
  XBR0 &= ~XBR0_SPI0E__ENABLED;
  // Enable XBar
  XBR2 |= XBR2_XBARE__ENABLED;
}

/******************************************************************************
 * Port Match ISR
 *
 * Whenever the capsense interrupt pin goes low, the SMBus is updated, data is
 * read from the CPT device and parsed to see if the packet is a touch/release/
 * status packet.
 *
 *****************************************************************************/
SI_INTERRUPT(PMATCH_ISR, PMATCH_IRQn)
{
   // If the Port Match event occurred on Capsense interrupt pin
  // then read I2C packets.
   if(CAPSENSE_INT == 0)
   {
	  updateSMBUS();
   }

   SFRPAGE = LEGACY_PAGE;              // EIE1 on SFRPAGE 0x00
   EIE1 &= ~0x02;                      // Disable Port Match interrupts to
									   // prevent multiple interrupts from
									   // occurring while the switches are
									   // pressed
}
