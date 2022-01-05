/*! @file bsp.h
 * @brief This file contains application specific definitions and includes.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef BSP_H
#define BSP_H

/*------------------------------------------------------------------------*/
/*            Application specific global definitions                     */
/*------------------------------------------------------------------------*/
/*! Platform definition */
/* Note: Plaform is defined in Silabs IDE project file as
 * a command line flag for the compiler. */


/*! Extended driver support
 * Known issues: Some of the example projects
 * might not build with some extended drivers
 * due to data memory overflow */
#define RADIO_DRIVER_EXTENDED_SUPPORT


#undef  RADIO_DRIVER_EXTENDED_SUPPORT
#undef  RADIO_DRIVER_FULL_SUPPORT
#undef  SPI_DRIVER_EXTENDED_SUPPORT
#undef  HMI_DRIVER_EXTENDED_SUPPORT
#undef  TIMER_DRIVER_EXTENDED_SUPPORT
#undef  UART_DRIVER_EXTENDED_SUPPORT


#define BIT bit
#define BITS(bitArray, bitPos)  BIT bitArray ## bitPos
#define FALSE 0
#define TRUE 1

/*------------------------------------------------------------------------*/
/*            Application specific includes                               */
/*------------------------------------------------------------------------*/


#include "platform_defs.h"
#include "hardware_defs.h"
#include "..\inc\InitDevice.h"
#include "main.h"
#include <string.h>
#include "retargetserial.h"
#include "drivers\radio_spi.h"
#include "si_toolchain.h"
#include "SI_EFM8BB1_Register_Enums.h"
#include "application/sRange.h"


#define SI_SEG_GENERIC
#define SI_SEG_XDATA   xdata
#define SI_SEG_DATA  data
#define SI_SEG_DATA  data
#define SI_SEG_IDATA idata
#define SI_SEG_XDATA xdata
#define SI_SEG_PDATA pdata
#define SI_SEG_CODE  code
#define SI_SEG_BDATA bdata
#define RADIO_MAX_PACKET_LENGTH     64u


#define lSys_SysClockFreq_c (24500000.0)
#define lSys_RadioCrystalFreq_c (30000000.0)


SI_SBIT(LEDG, SFR_P2, 0); // LEDS, 1 is off, 0 is on
SI_SBIT(LEDB, SFR_P2, 1);
SI_SBIT(LEDR, SFR_P2, 2);
SI_SBIT(PB0, SFR_P0, 2);  // Push buttons, pushed = 0
SI_SBIT(PB1, SFR_P0, 3);
SI_SBIT(NSEL, SFR_P1, 3); //select of the radio, to comm with radio pull low
SI_SBIT(PWRDN, SFR_P1, 4);  //GPIO 1.6 going to SDN of radio, if high radio will shutdown
SI_SBIT(CTS, SFR_P1, 4); //Clear to send radio flag


#include "application/radio.h"
#include "application/radio_config.h"

#include "drivers\radio\radio_hal.h"
#include "drivers\radio\radio_comm.h"

#ifdef SILABS_RADIO_SI446X
#include "drivers\radio\Si446x\si446x_api_lib.h"
#include "drivers/radio/Si446x/si446x_api_lib_add.h"
#include "drivers\radio\Si446x\si446x_cmd.h"
#include "drivers\radio\Si446x\si446x_nirq.h"
#include "drivers\radio\Si446x\si446x_patch.h"
#include "drivers\radio\Si446x\si446x_defs.h"
#endif

#ifdef SILABS_RADIO_SI4455
#include "drivers\radio\Si4455\si4455_api_lib.h"
#include "drivers\radio\Si4455\si4455_defs.h"
#include "drivers\radio\Si4455\si4455_nirq.h"
#endif

#endif //BSP_H
