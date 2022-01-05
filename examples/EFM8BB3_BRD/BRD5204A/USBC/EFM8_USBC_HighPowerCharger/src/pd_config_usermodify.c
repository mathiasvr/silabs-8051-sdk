/**************************************************************************//**
 * Copyright 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <SI_EFM8BB3_Register_Enums.h>

/*****
 * This file belongs in user application.
 *
 * Users should modify this file to suit the application
 *
 */

#include "pd.h"
#include "pd_config.h"
#include "ncp81239_driver.h"

/***************************************************************************
 * PD_CONFIG_UM_USER_VERSION - This number should match the value evaluated
 * by the arithmetic operation in PD_CONFIG_UM_SYSTEM_VERSION in pd.h.
 *
 * THE FOLLOWING LINE MUST NOT BE USED:
 *
 * #define PD_CONFIG_UM_USER_VERSION   MAGIC_VERSION(PD_CONFIG_UM_SYSTEM_VERSION)
 *
 * to workaround user responsibility to use the correct version of
 * pd_config_usermodify.c.
 *
 * #define PD_CONFIG_UM_USER_VERSION   MAGIC_VERSION(3)
 *
 ***************************************************************************/
#define PD_CONFIG_UM_USER_VERSION  MAGIC_VERSION(3)
#if BADUSAGE_VER(PD_CONFIG_UM)
#error "You cannot put PD_CONFIG_UM_SYSTEM_VERSION inside MAGIC_VERSION()"
#elif MISMATCH_VER(PD_CONFIG_UM)
#error "Mismatched version of pd_config_usermodify.c and pd.h."
#error "Please use the correct version of pd_config_usermodify.c to build the application."
#error "Refer to pd_config_usermodify.c in ufp_dongle project for the correct version"
#endif

/*
 * The appcfg_Configure_GPIOs function should be called
 * Responsibility of application to configure the following pins to be analog:
 *
 * VREF
 * AGND
 * CC1
 * CC2 (or only 1 CC for VCONN powered accessory)
 * VCONN_DET
 * VBUS_DET
 * DAC output pins
 *
 */
void appcfg_Configure_GPIOs()
{
  uint8_t SFRPAGE_save = SFRPAGE;
  SFRPAGE = PG3_PAGE;

  /*
   * Settings for Type-C use
   * P0.0: VREF
   * P0.1: AGND
   * P0.2: Unused
   * P0.3: SPI0_SCK slave - Unused, but must be assigned to crossbar
   * P0.4: Unused
   * P0.5: SPI0_MISO
   * P0.6: SPI0_MOSI
   * P0.7: Regulator I2C SDA
   *
   * P1.0: Unused
   * P1.1: USBC_CC1_RX
   * P1.2: USBC_CC2_RX
   * P1.3: Unused
   * P1.4: Regulator I2C SCL
   * P1.5: CMP0_SYNCOUT
   * P1.6: Unused
   * P1.7: UART1 TX output - for debugging only
   *
   * P2.0: UART1 RX input - for debugging only
   * P2.1: Regulator Interrupt Pin
   * P2.2: Unused
   * P2.3: Unused
   * P2.4: Unused
   * P2.5: VIN Sense
   * P2.6: VBUS Sense
   *
   * P3.0: LED0
   * P3.1: USBC_CC1_TX
   * P3.2: LED1
   * P3.3: USBC_CC2 TX
   * P3.4: Unused
   *
   */


  // Skipping pins such that SPI SCK, MISO, MOSI and SMB SDA signals are routed
  // to P0.3, P0.5, P0.6 and P0.7 respectively via Crossbar
  P0SKIP = 0x17;

  // Skipping pins such that SMB SCL is routed to P1.4 and CMP0_SYNCOUT is
  // routed to P1.5 via Crossbar
  P1SKIP |= (0x0F
#ifdef DEBUG
           | 0x40  // Skip P1.6 such that UART1 TX is routed
                   // to P1.7 via Crossbar
#endif
            );

  P0MDIN &= ~0x03;    // VREF and AGND
  P1MDIN &= ~0x06;    // CC1_RX and CC2_RX
  P2MDIN &= ~0x20;    // VIN_SEN
  P3MDIN &= ~0x0A;    // CC1_TX and CC2_TX

  // P0MDOUT:
  // P0.7 for I2C SDA
  // P0.5 for SPI MISO.
  // P0.3 for viewing actual SPI SCK - debug purpose
  P0MDOUT |= (0x20
#if HWCFG_DEBUG_VIEW_SPISCK
            | 0x08
#endif
#if HWCFG_DEBUG_VIEW_DBGBMC
            | 0x20
#endif
             );

  // P1MDOUT:
  // P1.7 for UART1 TX - only when DEBUG == 1
  // P1.5 for CMP0_SYNCOUT - used for BMC input
  // P1.4 for I2C SCL
  P1MDOUT |= (0x20
#ifdef DEBUG
            | 0x80
#endif
              );

  // P2MDOUT:
  // P2.1 for VBUS Control
  // P2.2 for VBUS Discharge
  // P2.3 for USBC CC1 Rd Enable
  // P2.4 for USBC CC2 Rd Enable
  // P2.5 for viewing BMC RX decoded MOSI signal - debug purpose
  // P2.2 for viewing BMC End of TX signal - debug purpose
  P2MDOUT |= 0x00;

  // P3MDOUT:
  // P3.0 for LED0 Enable
  // P3.2 for LED1 Enable
  P3MDOUT |= 0x05;

  // By default, disable LED0 and LRF1
  P3 |= 0x05;

  // Turn off user LED's
  UIF_LED0 = UIF_LED0_OFF;
  UIF_LED1 = UIF_LED1_OFF;

  /*
   * Crossbar use:
   * - SPI0 (3-wire)
   * - CMP0 Sync output
   * - SMBUS
   * - UART1 (debug only)
   */
  SPI0CN0 &= ~SPI0CN0_NSSMD__FMASK;   // Make SPI 3-wire before enabling crossbar
  XBR0 |= XBR0_SMB0E__ENABLED | XBR0_CP0E__ENABLED | XBR0_SPI0E__ENABLED;
#ifdef DEBUG
  XBR2 |= XBR2_XBARE__ENABLED | XBR2_WEAKPUD__PULL_UPS_DISABLED | XBR2_URT1E__ENABLED;
#else
  XBR2 |= XBR2_XBARE__ENABLED | XBR2_WEAKPUD__PULL_UPS_DISABLED;
#endif

  SFRPAGE = SFRPAGE_save;

  ncp81239_init();
}


/*
 * Returns the power status of the system with respect to VBUS and
 * VCONN.  Should indicate power status of system - whether it is
 * sourcing or sinking or neutral with respect to VBUS and VCONN.
 */
usbtc_power_status_t appcfg_GetPowerStatus()
{
  return usbtc_power_sourcing;
}
