/*! @file spi.c
 * @brief This file contains functions for controlling SPI0 and SPI1 interfaces.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#include "..\master_bsp.h"

/*------------------------------------------------------------------------*/
/*                          Global variables                              */
/*------------------------------------------------------------------------*/


/**< Current state of the NSEL of SPI1 */
SI_SEGMENT_VARIABLE(bSpi_SelectStateSpi1, uint8_t , SI_SEG_DATA);
/**< Current state of SPI1 */
bool gSpi_Spi1Disabled;

/*------------------------------------------------------------------------*/
/*                          Function implementations                      */
/*------------------------------------------------------------------------*/


/*!
 * This function is used to read/write one byte from/to SPI1.
 *
 * @param[in] biDataIn    Data to be sent.
 * @return  Read value of the SPI port after writing on it.
 */
uint8_t bSpi_ReadWriteSpi1(uint8_t biDataIn)
{  
  SI_SEGMENT_VARIABLE(bValue, uint8_t, SI_SEG_DATA);
  bool gRestoreEA;


  /* disable interrupts during SPI transfer */
  gRestoreEA = IE_EA;
  //ENTER_CRITICAL_SECTION;
  IE_EA = 0;


  // Send SPI data using double buffered write
  SPI0CN0_SPIF = 0;                                    // clear SPIF
  SPI0DAT = ( biDataIn );                       // write data register
  while (!SPI0CN0_TXBMT);                              // wait on TXBMT
  while ((SPI0CFG & SPI0CFG_SPIBSY__BMASK) == SPI0CFG_SPIBSY__BMASK);   // wait on SPIBSY
  bValue = SPI0DAT;                             // read value
  SPI0CN0_SPIF = 0;                                    // leave SPIF cleared

  // Restore interrupts after SPI transfer
  IE_EA = gRestoreEA;

  return bValue;
}


/*!
 * This function is used to send data over SPI1 no response expected.
 *
 *  @param[in] biDataInLength  The length of the data.
 *  @param[in] *pabiDataIn     Pointer to the first element of the data.
 *
 *  @return None
 */
void vSpi_WriteDataSpi1(uint8_t biDataInLength, uint8_t *pabiDataIn)
{

  while (biDataInLength--) {
    bSpi_ReadWriteSpi1(*pabiDataIn++);
  }
}

/*!
 * This function is used to read data from SPI1.
 *
 *  \param[in] biDataOutLength  The length of the data.
 *  \param[out] *paboDataOut    Pointer to the first element of the response.
 *
 *  \return None
 */
void vSpi_ReadDataSpi1(uint8_t biDataOutLength, uint8_t *paboDataOut)
{

  // send command and get response from the radio IC
  while (biDataOutLength--) {
    *paboDataOut++ = bSpi_ReadWriteSpi1(0xFF);
  }
}


  /**
 *  Pull down nSEL of the selected device.
 *
 *  @param[in] qiSelect Device select.
 *
 *  @note Input: qiSelect <br> 0 - \b DUT <br> 1 - \b LCD <br>
 *
 ******************************************************************************/
void vSpi_ClearNsel(eSpi_Nsel qiSelect)
{
  switch (qiSelect)
  {
      case eSpi_Nsel_RF:
        RF_NSEL = 0;
        break;

      default:
        break;
  }
}


/**
 *  Pull-up nSEL of the selected device.
 *
 *  @param[in] qiSelect Device select.
 *
 *  @note Input: qiSelect <br> 0 - \b DUT <br> 1 - \b LCD <br>
 *
 ******************************************************************************/
void vSpi_SetNsel(eSpi_Nsel qiSelect)
{
  switch (qiSelect)
  {
      case eSpi_Nsel_RF:
        RF_NSEL = 1;
        break;

      default:
        break;
  }
}

