/*! @file spi.h
 * @brief This file is the interface file for SPI0 and SPI1 routines.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef SPI_H
#define SPI_H

#include "si_toolchain.h"

/*------------------------------------------------------------------------*/
/*                          Global typedefs & definitions                 */
/*------------------------------------------------------------------------*/

/*! SPI device select enum */

typedef enum
{
  eSpi_Nsel_RF,
  eSpi_Nsel_LCD
} eSpi_Nsel;

/*------------------------------------------------------------------------*/
/*                          External variables                            */
/*------------------------------------------------------------------------*/

extern SI_SEGMENT_VARIABLE(bSpi_SelectStateSpi1, uint8_t , SI_SEG_DATA);
extern bool gSpi_Spi1Disabled;


/*------------------------------------------------------------------------*/
/*                           Function prototypes                          */
/*------------------------------------------------------------------------*/



void vSpi_WriteDataSpi1(uint8_t biDataInLength, uint8_t *pabiDataIn);
void vSpi_ReadDataSpi1(uint8_t biDataOutLength, uint8_t *paboDataOut);
uint8_t bSpi_ReadWriteSpi1(uint8_t biDataIn);

void vSpi_ClearNsel(eSpi_Nsel qiSelect);
void vSpi_SetNsel(eSpi_Nsel qiSelect);

#endif
