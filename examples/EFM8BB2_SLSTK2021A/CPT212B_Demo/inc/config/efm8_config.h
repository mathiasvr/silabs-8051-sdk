/******************************************************************************
 * Copyright (c) 2014 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
 
#ifndef __EFM8_CONFIG_H__
#define __EFM8_CONFIG_H__

// SPI0
#define EFM8PDL_SPI0_USE                  1
#define EFM8PDL_SPI0_USE_BUFFER           1
#define EFM8PDL_SPI0_USE_FIFO             0
#define EFM8PDL_SPI0_TX_SEGTYPE           SI_SEG_PDATA

// SMBUS0
#define EFM8PDL_I2C0_USE_BUFFER           1

#define EFM8PDL_I2C0_MASTER_RETRIES       0

#define EFM8PDL_I2C0_RX_BUFTYPE           SI_SEG_IDATA
#define EFM8PDL_I2C0_TX_BUFTYPE           SI_SEG_IDATA
#define EFM8PDL_I2C0_DATA_BUFTYPE         SI_SEG_IDATA

#endif // __EFM8_CONFIG_H__
