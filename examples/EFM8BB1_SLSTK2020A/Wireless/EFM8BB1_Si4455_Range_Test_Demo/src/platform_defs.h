/*! @file platform_defs.h
 * @brief This file contains platform specific definitions.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef PLATFORM_DEFS_H_
#define PLATFORM_DEFS_H_
#include "master_bsp.h"
#define SILABS_PLATFORM_WMB930
/*---------------------------------------------------------------------*/
/*            Platform specific global definitions                     */
/*---------------------------------------------------------------------*/

#if (defined SILABS_PLATFORM_WMB930)
  #define SILABS_PLATFORM_WMB
  //#define SILABS_RADIO_SI446X
  #define  SILABS_RADIO_SI4455
  #define SILABS_MCU_F930

  #define SILABS_PLATFORM_COMPONENT_LED     4
  #define SILABS_PLATFORM_COMPONENT_PB      4

  #define SILABS_PLATFORM_COMPONENT_BUZZER  1

#else
  #error Platform is undefined or unidentified platform is used!
#endif

#ifdef SILABS_MCU_F930
  #include "SI_EFM8BB1_defs.h"
  #include "SI_EFM8BB1_Register_Enums.h"
#endif


#endif /* PLATFORM_DEFS_H_ */
