/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *
 * CPT213B state machine
 *
 * This state machine cycles the CPT device through four states, viz. Config
 * load, Enter sensing mode, Update SMBus, Enter safe state.
 *
 * Update SMBUS state
 *
 * When the device enters the state machine, it is always in the update smbus
 * state. In this state, whenever the capsense interrupt pin is low, the host
 * I2C device reads the CPT device's status packets. If there are any capacitive
 * button touch or release occurring during this time, it will be reflected on
 * the LCD screen.
 *
 * Config Load state
 *
 * In order to enter the config load mode, press and release either Push Button
 * 0 or 1. This will load two different configurations -
 *
 * PB0 - Enables all buttons
 * PB1 - Enables only the middle row
 *
 * The config loading process is a 7-state machine in itself and is explained
 * in the smbus.c file
 *
 * Once the configuration is loaded, we move to the "enter sensing mode" state.
 * During the config load state, SPI and hence, LCD, is disabled
 *
 * Enter Sensing Mode
 *
 * In this state, the LCD is enabled and the device is put in the sensing mode.
 * The next state from here is the Update SMBus State
 *
 * Enter Safe State
 *
 * This state is entered ONLY when the device is in Update SMBus state and a
 * status packet was not received for more than 300 ms. The only way to exit
 * out of the safe state is to restart the host device. The packets are
 * checked in the smbus.c and cpt213b_oem_timer.c functions.
 *
 *****************************************************************************/

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "cpt213b_state_machine.h"
#include "bsp.h"
#include "smbus.h"
#include "lcd.h"

// Global variables
bool PB0_pushed = false;
bool PB1_pushed = false;
uint8_t pkt_count;
// Always start at Update SMBus state
uint8_t cpt_state = UPDATE_SMBUS;

// Function declaration
void enable_SPI(void);

// Enter the state machine

uint8_t CPT_state_machine()
{
  switch(cpt_state)
  {
    case ENTER_CONFIG_MODE:
  	    if(PB0_pushed)
  	    {
  	      if(BSP_PB0 != BSP_PB_UNPRESSED)
  	      {
  	        break;
  	      }
  	      PB0_pushed = false;
  		  // enable all buttons
  		  CPT_CONFIG.active_mode_mask[0] = 0xFF;
  		  CPT_CONFIG.active_mode_mask[1] = 0x1F;
  	    }
  	    else if(PB1_pushed)
  	    {
		  if(BSP_PB1 != BSP_PB_UNPRESSED)
		  {
		    break;
		  }
  	      PB1_pushed = false;
  		  // enable only middle row
  		  CPT_CONFIG.active_mode_mask[0] = 0xF0;
  		  CPT_CONFIG.active_mode_mask[1] = 0x01;
  	    }
  	    // Config load state machine
    	if(CPT_config((uint8_t *)&CPT_CONFIG) < RESET_STATE)
    	{
    	  cpt_state = ENTER_CONFIG_MODE;
    	}
    	else
    	{
    	  CPT_reset();
    	  cpt_state = ENTER_SENSING_MODE;
    	}
        break;

    case ENTER_SENSING_MODE:
          CPT_enterSensingMode();
          enable_SPI();
          cpt_state = UPDATE_SMBUS;
    	break;

    case UPDATE_SMBUS:
    	  if((BSP_PB0 == BSP_PB_PRESSED) && (!noConfidenceMode))
    	  {
    	    PB0_pushed = true;
    	    cpt_state = ENTER_CONFIG_MODE;
    	    break;
    	  }
    	  else if((BSP_PB1 == BSP_PB_PRESSED)&& (!noConfidenceMode))
    	  {
    		PB1_pushed = true;
    		cpt_state = ENTER_CONFIG_MODE;
    		break;
    	  }
    	 else
    	 {
    	   pkt_count = 0;
		   while(CAPSENSE_INT == 0)
		   {
			 updateSMBUS();
		     pkt_count++;
		     if(pkt_count >= 13)
		     {
			   break;
		     }
		   }
		   cpt_state = UPDATE_SMBUS;
    	}
        break;

    case ENTER_SAFE_STATE:
			printNCM(SAFE_STATE);
			iec60730_SafetyCheck = iec60730_I2C0Fail;
		  break;

    default:
    	break;
  }
  return cpt_state;
}

