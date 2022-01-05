/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/*
 * 			I2C data structure
 * 		For CPT112S there are 12 capsense outputs, each pin
 * 		corresponds to a capsenseCurrent bit showing whether a
 * 		cap-sense button pressed or not, and capsensePrevious
 * 		bit showing the status of previous cycle.
 *
 * 		If capsenseCurrent==1 && capsensePrevious==0, detects a "press"
 * 		If capsenseCurrent==0 && capsensePrevious==1, detects a "release"
 *
 * 		capsenseCurrent bit-map byte:
 * 		|capsenseCurrent11|capsenseCurrent10|capsenseCurrent9 |capsenseCurrent8
 * 		|capsenseCurrent7 |capsenseCurrent6 |capsenseCurrent5 |capsenseCurrent4
 * 		|capsenseCurrent3 |capsenseCurrent2 |capsenseCurrent1 |capsenseCurrent0
 *
 * 		capsensePrevious bit-map byte:
 * 		|capsensePrevious11|capsensePrevious10|capsensePrevious9 |capsensePrevious8
 *    |capsensePrevious7 |capsensePrevious6 |capsensePrevious5 |capsensePrevious4
 *    |capsensePrevious3 |capsensePrevious2 |capsensePrevious1 |capsensePrevious0
 *
 */
// Standard library
#include <si_toolchain.h>
#include <string.h>
#include "retargetserial.h"
#include <stdlib.h>

// project library
#include "project_config.h"
#include <bsp.h>
#include <i2c_0.h>
#include "smbus.h"
#include "circle.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// Global holder for SMBus data
// All receive data is written here
SI_SEGMENT_VARIABLE(SMB_DATA_IN[NUM_BYTES_RD],
                    uint8_t,
                    EFM8PDL_I2C0_RX_BUFTYPE);

// Global holder for SMBus data.
// All transmit data is read from here
SI_SEGMENT_VARIABLE(SMB_DATA_OUT[NUM_BYTES_WR],
                    uint8_t,
                    EFM8PDL_I2C0_TX_BUFTYPE);

// GPIO data structure declaration
uint16_t capsenseCurrent, sliderCurrent, sliderValue;
uint16_t capsensePrevious, sliderPrevious;

volatile bool transferInProgress = false;

// Counter for the number of errors.
uint16_t numErrors;

void parseSMBUS(void);

/******************************************************************************
 * GPIO data structure initialization
 *****************************************************************************/
void initSMBUS(void)
{
  I2C0_reset();

  // Use Timer 0 for SCL clock rate
  I2C0_init(I2C0_TIMER0, false);

  capsenseCurrent = 0;
  capsensePrevious = 0;
  sliderCurrent = 0xFFFF;
  sliderPrevious = 0xFFFF;
  numErrors = 0;
  sliderValue = 0;
}

/******************************************************************************
 * 1. parse smbus packet received from slave, update smbus data structure
 *****************************************************************************/
void updateSMBUS(void)
{
  // get previous states of Cap-sense button array
  capsensePrevious = capsenseCurrent;
  sliderPrevious = sliderCurrent;
  while(!CAPSENSE_INT){
    // update current button states
    parseSMBUS();
  }
}

void parseSMBUS(){
  transferInProgress = true;

  // Start write/read transfer
  // Send out data
  // Receive in data (loopback data from slave)
  I2C0_transfer(SLAVE_ADDR, SMB_DATA_OUT, SMB_DATA_IN, 0, NUM_BYTES_RD);

  // Wait until transfer complete callback is called
  while(transferInProgress);

  // check press action   @ 0
  // check release action @ 1
  // check slider action @ 2
  if((SMB_DATA_IN[0] & 0x0F) == 0){
	  sliderCurrent = 0xFFFF;
	  capsenseCurrent |= (0x01 << SMB_DATA_IN[1]);

  }
  else if((SMB_DATA_IN[0] & 0x0F) == 1){
	  sliderCurrent = 0xFFFF;
	  capsenseCurrent &= (~(0x01 << SMB_DATA_IN[1]));


  }
  else if((SMB_DATA_IN[0] & 0x0F) == 2){
	  sliderCurrent = (SMB_DATA_IN[1] << 8) | (SMB_DATA_IN[2]);
	  sliderValue = (sliderCurrent * (128-circle_height))/0x40; //Slider value obtained to display on the LCD
  }
  else{
	  // error
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

