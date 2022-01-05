/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
#ifndef SMBUS_H_
#define SMBUS_H_

// project library
#include "project_config.h"
#include "iec60730.h"
#if CPT_DEVICE == 212
#include "cpt212b_a01_gm_init.h"
#elif CPT_DEVICE == 213
#include "cpt213b_a01_gm_init.h"
#endif

/// Sets the address used by the transmitter and checked by receiver in the comm protocol.
#define CPT_COMM_CONFIGMODE_ADDRESS   0xC0
#define CPT_COMM_DEVICE_ADDRESS    0xF0

#define CPT_FLASH_CODE_A5            0xA5
#define CPT_FLASH_CODE_F1            0xF1
#define CPT_CONFIG_UNLOCK            0x09
#define CPT_CONFIG_ERASE             0x0A
#define CPT_CONFIG_WRITE             0x0B
#define CPT_CONFIG_CRC               0x0C
#define CPT_ENTER_SENSING_MODE       0x08
#define CPT_ENTER_SENSING_MODE_1     0X01

#if CPT_DEVICE == 213

#define CPT_MAX_TXBUFFER_SIZE        0x0C
#define CPT_MAX_RXBUFFER_SIZE        0x06

#define EVENT_BYTE           1

#define CONFIG_STATE_BYTE           1

#define RX_CSINDEX_BYTE         2

#define CPT_CONFIGPROFILE_SIZE       222

#define  TX_SENSING_MODE_SIZE   5         // Number of bytes to write to start sensing mode
                                       	// Master -> Slave
#define  RX_EVENT_SIZE     6         // Number of bytes to read from CPT device
                                       	// Master <- Slave
#define CPT_CONFIGCHECK_SIZE   2
#elif CPT_DEVICE == 212

#define EVENT_BYTE           0

#define CONFIG_STATE_BYTE           0

#define RX_CSINDEX_BYTE         1

#define CPT_CONFIGPROFILE_SIZE       207

#define CPT_MAX_TXBUFFER_SIZE        0x09
#define CPT_MAX_RXBUFFER_SIZE        0x03

#define  TX_SENSING_MODE_SIZE   2         // Number of bytes to write to start sensing mode
                                        // Master -> Slave
#define  RX_EVENT_SIZE     3         // Number of bytes to read from CPT device
                                        // Master <- Slave
#define CPT_CONFIGCHECK_SIZE   1
#endif

#define DISABLE_SPI 		0
#define CPT_RESET			1
#define CONFIG_UNLOCK		2
#define CONFIG_ERASE		3
#define CONFIG_WRITE		4
#define CONFIG_CRC			5
#define CONFIG_CHECK		6
#define RESET_STATE			7

#define VALID_CONFIG        0x80
#define INVALID_CONFIG      0x00
#define CONFIG_BUSY         0x01
// Status packet
extern bool noConfidenceMode;
extern volatile bool transferInProgress;
extern SI_SEGMENT_VARIABLE(CPT_CONFIG, config_profile_t, SI_SEG_XDATA);

/******************************************************************************
 * SMBUS data structure initialization
 *****************************************************************************/
void initSMBUS(void);

/******************************************************************************
 * Reset the CPT device
 *****************************************************************************/
void CPT_reset(void);

/******************************************************************************
 * Config the CPT device
 *****************************************************************************/
uint8_t CPT_config(uint8_t * configprofile);

/******************************************************************************
 * Config unlock the CPT device
 *****************************************************************************/
void CPT_configUnlock(void);

/******************************************************************************
 * Config Erase the CPT device
 *****************************************************************************/
void CPT_configErase(void);

/******************************************************************************
 * Config Write the CPT device
 *****************************************************************************/
uint8_t CPT_configWrite(uint8_t * configprofile);

/******************************************************************************
 * Write Config CRC of the CPT device
 *****************************************************************************/
void CPT_configCRC(uint8_t * configprofile);

/******************************************************************************
 * Config Check the CPT device
 *****************************************************************************/
uint8_t CPT_configCheck(void);

/******************************************************************************
 * Put the CPT device in sensing mode
 *****************************************************************************/
void CPT_enterSensingMode(void);

/******************************************************************************
 * update SMBUS data structure from current packet of SMBUS
 *****************************************************************************/
void updateSMBUS(void);

void enable_SPI(void);
void disable_SPI(void);

#endif /* SMBUS_H_ */
