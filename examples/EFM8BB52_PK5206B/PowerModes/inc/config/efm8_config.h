/******************************************************************************
 * Copyright (c) 2020 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
 
#ifndef __EFM8_CONFIG_H__
#define __EFM8_CONFIG_H__

#define EFM8PDL_SPI0_USE                  1
#define EFM8PDL_SPI0_USE_BUFFER           1
#define EFM8PDL_SPI0_USE_FIFO             0
#define EFM8PDL_SPI0_TX_SEGTYPE           SI_SEG_PDATA

// Select Power Mode driver options
#define EFM8PDL_PWR_USE_STOP  1

#endif // __EFM8_CONFIG_H__