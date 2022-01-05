/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *
 * I2C data structure
 *
 * For CPT212B/213B there are 12/13 capsense outputs respectively.
 * Each pin corresponds to a capsenseCurrent bit showing whether a
 * cap-sense button pressed or not, and capsensePrevious
 * bit showing the status of previous cycle.
 *
 * If capsenseCurrent==1 && capsensePrevious==0, detects a "press"
 * If capsenseCurrent==0 && capsensePrevious==1, detects a "release"
 *
 * Example - capsenseCurrent bit-map byte:
 * |capsenseCurrent11|capsenseCurrent10|capsenseCurrent9 |capsenseCurrent8
 * |capsenseCurrent7 |capsenseCurrent6 |capsenseCurrent5 |capsenseCurrent4
 * |capsenseCurrent3 |capsenseCurrent2 |capsenseCurrent1 |capsenseCurrent0
 *
 * Example - capsensePrevious bit-map byte:
 * |capsensePrevious11|capsensePrevious10|capsensePrevious9 |capsensePrevious8
 * |capsensePrevious7 |capsensePrevious6 |capsensePrevious5 |capsensePrevious4
 * |capsensePrevious3 |capsensePrevious2 |capsensePrevious1 |capsensePrevious0
 *
 *****************************************************************************/
// Standard library
#include <si_toolchain.h>
// project library
#include "project_config.h"
#include "i2c_0.h"
#include "smbus.h"
#include "spi.h"
#include "lcd.h"
#include "cpt212b_a01_gm_init.h"



//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define SYSCLK              24500000          // Internal oscillator frequency in Hz
#define PCA_CH2_FREQUENCY	   20000          // Frequency of the CPT timer
// SYSCLK cycles per interrupt
#define PCA_CH2_TIMEOUT (SYSCLK/PCA_CH2_FREQUENCY)


//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// All receive data is written here
SI_SEGMENT_VARIABLE(CPT_RXbuffer[CPT_MAX_RXBUFFER_SIZE], uint8_t, EFM8PDL_I2C0_RX_BUFTYPE) = {0x00,0x00,0x00};
// All transmit data is read from here
SI_SEGMENT_VARIABLE(CPT_TXbuffer[CPT_MAX_TXBUFFER_SIZE], uint8_t, EFM8PDL_I2C0_TX_BUFTYPE);
// CPT Config profile
SI_SEGMENT_VARIABLE(CPT_CONFIG, config_profile_t, SI_SEG_XDATA) = CPT212B_A01_GM_DEFAULT_CONFIG;


// GPIO data structure declaration
uint16_t capsenseCurrent = 0;
uint16_t capsensePrevious = 0;

volatile bool transferInProgress = false;

// Counter for the number of errors.
uint16_t numErrors;
uint16_t Time_50us = 0;

uint16_t Next_Compare_Value_CH2;       // Next edge to be sent out in HSO mode

// Function declarations
void parseSMBUS(void);
void waitCPT(uint16_t);


/******************************************************************************
 *
 * initSMBUS() Routine
 *
 * Initializes I2C peripheral
 *****************************************************************************/
void initSMBUS(void)
{
  I2C0_reset();

  // Use Timer 0 for SCL clock rate
  I2C0_init(I2C0_TIMER0, false);

}

/******************************************************************************
 *
 * CPT_INTlow() Routine
 *
 * function for clearing the CPT INT pin low
 *****************************************************************************/
void CPT_INTlow(void)
{
  // Pull the interrupt pin low
  CAPSENSE_INT = 0;
}

/******************************************************************************
 *
 * CPT_INThigh() Routine
 *
 * function for setting the CPT INT pin high
 *****************************************************************************/
void CPT_INThigh(void)
{
  // Pull the interrupt pin high
  CAPSENSE_INT = 1;
}

/******************************************************************************
*
* calculate_profile_CRC() Routine
*
* Calculate CRC of full configprofile flash page minus the last two bytes
* because CRC is written to last two bytes in CPT_configCRC()
*
* Return calculated CRC
*****************************************************************************/
uint16_t calculate_profile_CRC(uint8_t * configprofile)
{
	uint16_t xdata output;
	uint16_t xdata pointer;

	CRC0CN0 |= 0x04;        // Write CRCVAL to start CRC with 0xFFFF
	CRC0CN0 |= 0x08;        // Write CRCINIT to initialize CRC


	//Feed the whole config profile into CRC and fill the rest with 0xFF
	for(pointer = 0; pointer < 510; pointer++)
	{
		if(pointer < CPT_CONFIGPROFILE_SIZE)
		{
			CRC0IN   = configprofile[pointer];
		} else
		{
			CRC0IN = 0xFF;
		}
	}

	CRC0CN0 |= 0x01;        //Access upper byte first

	output = CRC0DAT << 8;  //Reading CRC0DAT switches to lower byte
	output |= CRC0DAT;

	return output;
}

/******************************************************************************
*
* calculate_message_CRC() Routine
*
* Calculate CRC of i2c output buffer given the size input
*
* Return calculated CRC
*****************************************************************************/
uint16_t calculate_message_CRC(uint8_t * buffer, uint8_t size)
{
	uint16_t xdata output;
	uint16_t xdata pointer;

	CRC0CN0 |= 0x04;        // Write CRCVAL to start CRC with 0xFFFF
	CRC0CN0 |= 0x08;        // Write CRCINIT to initialize CRC

	//Feed the message into CRC
	for(pointer = 0; pointer < size; pointer++)
	{
		CRC0IN = buffer[pointer];
	}

	CRC0CN0 |= 0x01;        //Access upper byte first

	output = CRC0DAT << 8;  //Reading CRC0DAT switches to lower byte
	output |= CRC0DAT;

	return output;
}

/******************************************************************************
 *
 * CPT_config() Routine
 *
 * State machine to initialize and configure the CPT device
 *
 * Call repeatedly to progress through the state machine. Keep calling until
 * the return value is 7. When value is 7, the device is initialized
 * and in sensing mode. If called again after reporting 7, the state machine
 * will start over and reinitialize the device.
 *
 * If changing configuration of the device, make configuration changes only
 * when state returned in 0, 1, 2, or 7. Then continue to call CPT_config
 * until the returned value is 7.
 *
 *****************************************************************************/
uint8_t CPT_config(uint8_t * configprofile)
{
	uint8_t config_validity = 0;
	static uint8_t state = CPT_RESET;

	switch(state)
	{
	    case CPT_RESET :
			// Pull INT low to start sending config events
			CPT_reset();
			state = CONFIG_UNLOCK;
			break;

		case CONFIG_UNLOCK :
			// Pull INT low to start sending config events
			CPT_INTlow();
			// Configuration Unlock to allow writing profile
			CPT_configUnlock();
			state = CONFIG_ERASE;
			break;

		case CONFIG_ERASE :
			// Configuration Erase to clear out old profile
			CPT_configErase();
			state = CONFIG_WRITE;
			break;

		case CONFIG_WRITE :
			// Configuration Write new profile
			if (CPT_configWrite(configprofile))
			{
				state = CONFIG_CRC;
			}
			break;

		case CONFIG_CRC :
			// Configure the profile's CRC in the device
			CPT_configCRC(configprofile);
			state = CONFIG_CHECK;
			break;

		case CONFIG_CHECK :
			// Check the configuration profile validity
			if(CPT_configCheck() == VALID_CONFIG)
			{
				state = ENTER_SENSING_MODE;
				// Release INT pin
				CPT_INThigh();
			}
			break;

		case ENTER_SENSING_MODE :
			CPT_enterSensingMode();
			state = RESET_STATE;
			break;

		case RESET_STATE :
			state = CPT_RESET;
			break;
	}

	return state;
}

/******************************************************************************
 *
 * CPT_config_Unlock() Routine
 *
 * Config unlock the CPT device
 *****************************************************************************/
void CPT_configUnlock(void)
{
	uint8_t bufferpointer = 0;

	// Wait for any current SMBus transfer to complete
	while(transferInProgress);

	// Send config unlock event and unlock sequence
	CPT_TXbuffer[bufferpointer] = CPT_CONFIG_UNLOCK; // event type 1001b
	bufferpointer++;
	CPT_TXbuffer[bufferpointer] = CPT_FLASH_CODE_A5;
	bufferpointer++;
	CPT_TXbuffer[bufferpointer] = CPT_FLASH_CODE_F1;
	bufferpointer++;

	//Set transferInProgress flag to block other transfers from starting
	transferInProgress = true;

	// Transmit buffer through peripheral library
	I2C0_transfer(CPT_COMM_CONFIGMODE_ADDRESS, (uint8_t *) &CPT_TXbuffer, (uint8_t *) &CPT_RXbuffer, bufferpointer, 0);

	// Wait for current SMBus transfer to complete
	while(transferInProgress);

}

/******************************************************************************
 *
 * CPT_configErase() Routine
 *
 * Config Erase the CPT device
 *****************************************************************************/
void CPT_configErase(void)
{
	uint8_t bufferpointer = 0;

	// Wait for any current SMBus transfer to complete
	while(transferInProgress);

	// Send config erase event
	CPT_TXbuffer[bufferpointer] = CPT_CONFIG_ERASE; // event type 1010b
	bufferpointer++;

  	//Set transferInProgress flag to block other transfers from starting
  	transferInProgress = true;

  	// Transmit buffer through peripheral library
  	I2C0_transfer(CPT_COMM_CONFIGMODE_ADDRESS, (uint8_t *) &CPT_TXbuffer, (uint8_t *) &CPT_RXbuffer, bufferpointer, 0);

  	// Wait for SMBus transfer to complete
  	while(transferInProgress);

	// Erase command needs time to complete before sending any other commands
	waitCPT(35000); // Delay of 35 ms
}

/******************************************************************************
 *
 * CPT_configWrite() Routine
 *
 * Config Write the CPT device
 *****************************************************************************/
uint8_t CPT_configWrite(uint8_t * configprofile)
{
	static uint8_t outpointer = 0;
	uint8_t bufferpointer;

	// Wait for any current SMBus transfer to complete
	while(transferInProgress);

	bufferpointer = 0;

	while(bufferpointer < CPT_MAX_TXBUFFER_SIZE)
	{
		if(bufferpointer == 0)
		{
			// Send write config event
			CPT_TXbuffer[bufferpointer] = CPT_CONFIG_WRITE; // event type 1011b
			bufferpointer++;
		}

		// load next byte of config into the TX buffer
		CPT_TXbuffer[bufferpointer] = configprofile[outpointer];
		bufferpointer++;
		outpointer++;

		// if at the end of config profile, fill 8-byte buffer with 0xFF's
		if(outpointer == (sizeof(config_profile_t) ))
		{
			while(bufferpointer < CPT_MAX_TXBUFFER_SIZE)
			{
				CPT_TXbuffer[bufferpointer] = 0xFF;
				bufferpointer++;
			}
		}
	}

	// When the buffer is completely filled out, send to CPT
	if(bufferpointer == CPT_MAX_TXBUFFER_SIZE)
	{
	  transferInProgress = true;

	  // Transmit buffer through peripheral library
	  I2C0_transfer(CPT_COMM_CONFIGMODE_ADDRESS, (uint8_t *) &CPT_TXbuffer, (uint8_t *) &CPT_RXbuffer, bufferpointer, 0);
	  // Wait for any current SMBus transfer to complete
	  while(transferInProgress);
	  // Wait some time for this write to complete
	  waitCPT(1000); //TODO: wait of 1ms
	  bufferpointer= 0;

	}

	if(outpointer == sizeof(config_profile_t) )
	{
		outpointer = 0;
		return 1;
	} else
	{
		return 0;
	}

}

/******************************************************************************
 *
 * CPT_configCRC() Routine
 *
 * Write Config CRC of the CPT device
 *****************************************************************************/
void CPT_configCRC(uint8_t * configprofile)
{
	uint8_t bufferpointer = 0;
	uint16_t xdata ProfileCRC = calculate_profile_CRC(configprofile);

	while(transferInProgress);

	// Send write crc event with the calculated CRC for config profile
	CPT_TXbuffer[bufferpointer] = CPT_CONFIG_CRC; // event type 1100b
	bufferpointer++;
	CPT_TXbuffer[bufferpointer] = (ProfileCRC>>8);
	bufferpointer++;
	CPT_TXbuffer[bufferpointer] = (ProfileCRC & 0xFF);
	bufferpointer++;

	transferInProgress = true;

	// Transmit buffer through peripheral library
	I2C0_transfer(CPT_COMM_CONFIGMODE_ADDRESS, (uint8_t *) &CPT_TXbuffer, (uint8_t *) &CPT_RXbuffer, bufferpointer, 0);

	// Wait for transfer to complete
	while(transferInProgress);
	// Wait some time for CRC to complete
	waitCPT(35000); // wait for ~35 ms
}

/******************************************************************************
 *
 * CPT_configCheck
 *
 * Config Check the CPT device, return value read from device
 *
 * 0x80 is a valid profile
 * 0x01 is an invalid profile
 * 0x00 can happen if the device is busy with a command such as config erase
 *
 *****************************************************************************/
uint8_t CPT_configCheck(void)
{
	// Wait for bus to be free
	while(transferInProgress);

	transferInProgress = true;

	// Transmit buffer through peripheral library
	I2C0_transfer(CPT_COMM_CONFIGMODE_ADDRESS, (uint8_t *) &CPT_TXbuffer, (uint8_t *) &CPT_RXbuffer, 0, CPT_CONFIGCHECK_SIZE);

	// Wait for bus to be free
	while(transferInProgress);
	// Wait some time for this write to complete
	waitCPT(100); // wait for 100 us

	return CPT_RXbuffer[CONFIG_STATE_BYTE];
}


/******************************************************************************
 *
 * CPT_enterSensingMode()
 *
 * Enter sensing mode
 *****************************************************************************/
void CPT_enterSensingMode(void)
{
	uint8_t bufferpointer = 0;

	// Wait for bus to be free
	while(transferInProgress);

	// Pull INT pin low
	CPT_INTlow();

	// Send sensing mode event
	CPT_TXbuffer[bufferpointer] = CPT_ENTER_SENSING_MODE;
	bufferpointer++;
	CPT_TXbuffer[bufferpointer] = CPT_ENTER_SENSING_MODE_1;
	bufferpointer++;

	transferInProgress = true;

	// Transmit buffer through peripheral library
	I2C0_transfer(CPT_COMM_CONFIGMODE_ADDRESS, (uint8_t *) &CPT_TXbuffer, (uint8_t *) &CPT_RXbuffer, bufferpointer, 0);
	while(transferInProgress);

	// Wait till the stop condition has been set and cleared by hw
	while(SMB0CN0_STO == SMB0CN0_STO__SET);

	// Pull the interrupt pin high
	CPT_INThigh();
}

/******************************************************************************
 * 1. Toggle CPT reset pin to reset CPT
 *****************************************************************************/
void CPT_reset(void)
{
  uint16_t xdata low_time = 50; // wait 50 us
  uint16_t xdata high_time = 15000; // Wait 15 ms

  CPT_RST = 0;
  waitCPT(low_time);

  CPT_RST = 1;
  waitCPT(high_time);
}

/******************************************************************************
 * 1. parse smbus packet received from slave, update smbus data structure
 *****************************************************************************/
void updateSMBUS(void)
{
  // get previous states of Cap-sense button array
  parseSMBUS();
}

/******************************************************************************
 * 1. parse smbus packet received from slave, update smbus data structure
 *****************************************************************************/
void parseSMBUS()
{
  // Wait for bus to be free
  while(transferInProgress);

  transferInProgress = true;

  // Start write/read transfer
  // Send out data
  // Receive in data (loopback data from slave)
  I2C0_transfer(CPT_COMM_DEVICE_ADDRESS, (uint8_t *) &CPT_TXbuffer, (uint8_t *) &CPT_RXbuffer, 0, RX_EVENT_SIZE);

  // Wait until transfer complete callback is called
  while(transferInProgress);
  // Wait till the stop condition has been set and cleared by hw
  while(SMB0CN0_STO == SMB0CN0_STO__SET);

  // Parse the packet
  if((CPT_RXbuffer[EVENT_BYTE] & 0x0F) == 0) //Touch Event
  {
	capsenseCurrent |= 0x01 << CPT_RXbuffer[RX_CSINDEX_BYTE];
  }
  else if((CPT_RXbuffer[EVENT_BYTE] & 0x0F) == 1) // Touch Release
  {
	capsenseCurrent &= (~(0x01 << CPT_RXbuffer[RX_CSINDEX_BYTE]));
  }

}

//-----------------------------------------------------------------------------
// Callbacks
//-----------------------------------------------------------------------------

void I2C0_commandReceivedCb()
{
  // Not used
}

// Callback is called when an SMBus error occurs
void I2C0_errorCb (I2C0_TransferError_t error)
{
  switch (error)
  {
    case I2C0_ARBLOST_ERROR:
    case I2C0_NACK_ERROR:
    case I2C0_UNKNOWN_ERROR:
    case I2C0_TXUNDER_ERROR:
      numErrors++;
      break;
  }

  // Abort the transfer
  I2C0_abortTransfer();

  // Transfer is complete (error)
  transferInProgress = false;
}

void I2C0_transferCompleteCb()
{
  // Transfer is complete (success)
  transferInProgress = false;
}





//---------------------------------------------------------------------------
// GetTickCount
//---------------------------------------------------------------------------
//
// Description - Return the system up time in milliseconds
//
// return - Number of milliseconds since system start.
//---------------------------------------------------------------------------

uint16_t GetTime50usCount()
{
  uint16_t xdata ticks;
  PCA0CPM2 &= ~(PCA0CPM2_ECCF__BMASK);
  ticks = Time_50us;
  PCA0CPM2 |= (PCA0CPM2_ECCF__BMASK);
  return ticks;
}

//---------------------------------------------------------------------------
// Wait
//---------------------------------------------------------------------------
//
// Description - Wait the specified number of milliseconds
//
// us - The number of microseconds to wait
//---------------------------------------------------------------------------
void waitCPT(uint16_t us)
{
  uint16_t xdata ticks = GetTime50usCount();

  while ((GetTime50usCount() - ticks) < (us/50));
}

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// PCA0 ISR Content goes here. Remember to clear flag bits:
// PCA0CN0::CCF0 (PCA Module 0 Capture/Compare Flag)
// PCA0CN0::CCF1 (PCA Module 1 Capture/Compare Flag)
// PCA0CN0::CCF2 (PCA Module 2 Capture/Compare Flag)
// PCA0CN0::CF (PCA Counter/Timer Overflow Flag)
// PCA0PWM::COVF (Cycle Overflow Flag)
//
// This is the ISR for the PCA.  It handles the case when a match occurs on
// channel 0, and updates the PCA0CPn compare register with the value held in
// the global variable "CEX0_Compare_Value".
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
  if(PCA0CN0_CCF2)
  {
	PCA0CN0_CCF2 = 0;                           // Clear module 2 interrupt flag
	Time_50us++;
	Next_Compare_Value_CH2 += PCA_CH2_TIMEOUT;
	PCA0CPL2 = (Next_Compare_Value_CH2 & 0x00FF);
	PCA0CPH2 = (Next_Compare_Value_CH2 & 0xFF00)>>8;
  }

}

