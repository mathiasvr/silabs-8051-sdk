#ifndef __SILICON_LABS_EFM8_CONFIG_H
#define __SILICON_LABS_EFM8_CONFIG_H

// SPI0
#define EFM8PDL_SPI0_USE                  1
#define EFM8PDL_SPI0_USE_BUFFER           1
#define EFM8PDL_SPI0_USE_FIFO             0
#define EFM8PDL_SPI0_TX_SEGTYPE           SI_SEG_PDATA

// SMBUS0
#define EFM8PDL_I2C0_USE_BUFFER           1

#define EFM8PDL_I2C0_MASTER_RETRIES       0

#define EFM8PDL_I2C0_RX_BUFTYPE           SI_SEG_PDATA
#define EFM8PDL_I2C0_TX_BUFTYPE           SI_SEG_PDATA
#define EFM8PDL_I2C0_DATA_BUFTYPE         SI_SEG_IDATA

#endif // __SILICON_LABS_EFM8_CONFIG_H
