#include <SI_EFM8BB3_Register_Enums.h>                  // SFR declarations

/*****
 * This file belongs in user application.
 *
 * Users should modify this file to suit the application
 *
 */

#include "pd.h"
#include "pd_config.h"

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

extern SI_SEGMENT_VARIABLE(deviceIsSource, uint8_t, SI_SEG_XDATA);

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
   * Settings for Type C use
   * P0.0: VREF
   * P0.1: AGND
   * P0.2: Regulator Enable
   * P0.3: SPI0_SCK slave - not used, but must be assigned to crossbar
   * P0.4: Regulator Current Setting
   * P0.5: SPI0_MISO
   * P0.6: SPI0_MOSI
   * P0.7: Charger Enable
   *
   * P1.0: Charger Current Setting
   * P1.1: CC1_RX
   * P1.2: CC2_RX
   * P1.3: Button input
   * P1.4: REG CC
   * P1.5: CMP0_SYNCOUT
   * P1.6: CC1_RP output to enable/disable RP on CC1
   * P1.7: UART1 TX output - for debugging only
   *
   * P2.0: UART1 RX input - for debugging only
   * P2.1: VBUS Enable
   * P2.2: VBUS Discharge
   * P2.3: CC1_RD output to enable/disable RD on CC1
   * P2.4: CC2_RD output to enable/disable RD on CC2
   * P2.5: VBAT Sense
   * P2.6: VBUS Sense
   *
   * P3.0: LED0
   * P3.1: CC1 TX
   * P3.2: LED1
   * P3.3: CC2 TX
   * P3.4:CC2_RP output to enable/disable RP on CC2
   *
   */


  // Skipping pins such that SPI SCK, MISO, MOSI and I2C SDA signals are routed
  // to P0.3, P0.5, P0.6 and P0.7 respectively via Crossbar
  P0SKIP = 0x97;

  // Skip DBG_SPI_SCK, CC1_RX, CC2_RX and grounded pin such that
  // CMP0_SYNCOUT is routed to P1.4 via Crossbar
  P1SKIP |= (0x1F
#ifdef DEBUG
           | 0x40  // Skip P1.6 such that UART1 TX is routed to P1.7 via Crossbar
#endif
            );

  P0MDIN &= ~0x03;    // VREF and AGND
  P1MDIN &= ~0x16;    // CC1_RX, CC2_RX, and REG_CC
  P2MDIN &= ~0x20;    // VBAT_SEN
  P3MDIN &= ~0x0A;    // CC1_TX and CC2_TX

  // P0MDOUT:
  // P0.7 for Charger Enabled
  // P0.5 for SPI MISO.
  // P0.4 for Boost Regulator Current Mode
  // P0.3 for viewing actual SPI SCK - debug purpose
  // P0.2 for Boost Regulator Enable
  P0MDOUT |= (0xB4
#if HWCFG_DEBUG_VIEW_SPISCK
            | 0x08
#endif
#if HWCFG_DEBUG_VIEW_DBGBMC
            | 0x20
#endif
             );

  // P0:
  // P0.7: Charger disabled
  // P0.4: Boost regulator in high current mode
  // P0.2: Boost regulator enabled
  P0 &= ~0x80;
  REG_EN = REG_ENABLE;
  REG_CURRENT = (deviceIsSource) ? REG_CURRENT_HIGH : REG_CURRENT_LOW;

  // P1MDOUT:
  // P1.7 for UART1 TX - only when DEBUG == 1
  // P1.6 for USBC CC1 Rp Enable
  // P1.5 for CMP0_SYNCOUT - used for BMC input
  // P1.0 for Battery Charger Current Setting
  P1MDOUT |= (0x61
#ifdef DEBUG
            | 0x81
#endif
              );

  // P2MDOUT:
  // P2.1 for VBUS Control
  // P2.2 for VBUS Discharge
  // P2.3 for USBC CC1 Rd Enable
  // P2.4 for USBC CC2 Rd Enable
  // P2.5 for viewing BMC RX decoded MOSI signal - debug purpose
  // P2.2 for viewing BMC End of TX signal - debug purpose
  P2MDOUT |= (0x1E
/*#if HWCFG_DEBUG_VIEW_RXMOSI
            | 0x20    // Make the BMC RX decoded MOSI push-pull
#endif
#if HWCFG_DEBUG_VIEW_EOTX
            | 0x04    // Make the BMC End of TX signal push-pull
#endif*/
             );

  // By default, disable VBUS, VBUS Discharge, and CCx Rd
  P2 &= ~0x1E;
  USBC_VBUS_EN = USBC_VBUS_DISABLE;

  // By default, do no short VBus to ground
  USBC_VBUS_DISCHARGE = USBC_VBUS_DISCHARGE_DISABLE;

  // P3MDOUT:
  // P3.0 for LED0 Enable
  // P3.2 for LED1 Enable
  // P3.4 for USBC CC2 Rp Enable
  P3MDOUT |= 0x15;  // Push-pull for USBC_CC2_RP_EN

  // By default, disable LED0, LED1, and CC2 Rp
  P3 |= 0x15;

  // Turn on UIF_LED0 when acting as a Source
  UIF_LED0 = deviceIsSource ? UIF_LED0_ON : UIF_LED0_OFF;

  // Turn on UIF_LED1 when acting as a Sink
  UIF_LED1 = deviceIsSource ? UIF_LED1_OFF : UIF_LED1_ON;

  USBC_CC1_RD_EN = USBC_CC1_RD_DISABLE;   // Disable CC1 RD
  USBC_CC2_RD_EN = USBC_CC2_RD_DISABLE;   // Disable CC2 RD
  USBC_CC1_RP_EN = USBC_CC1_RP_DISABLE;   // Disable CC1 RP
  USBC_CC2_RP_EN = USBC_CC2_RP_DISABLE;   // Disable CC2 RP

  // Set up Port Match to detect MODE_SW
  P1MASK = 0x08;
  P1MAT = 0x08;
  SFRPAGE = PG2_PAGE;
  EIE1 |= EIE1_EMAT__ENABLED;
  SFRPAGE = PG3_PAGE;

  /*
   * Crossbar use:
   * - SPI0 (3-wire)
   * - CMP0 Sync output
   * - UART1 (debug only)
   */
  SPI0CN0 &= ~SPI0CN0_NSSMD__FMASK;   // Make SPI 3-wire before enabling crossbar
  XBR0 |= XBR0_CP0E__ENABLED | XBR0_SPI0E__ENABLED;
#ifdef DEBUG
  XBR2 |= XBR2_XBARE__ENABLED | XBR2_WEAKPUD__PULL_UPS_DISABLED | XBR2_URT1E__ENABLED;
#else
  XBR2 |= XBR2_XBARE__ENABLED | XBR2_WEAKPUD__PULL_UPS_DISABLED;
#endif

  SFRPAGE = SFRPAGE_save;
}


/*
 * Returns the power status of the system with respect to VBUS and
 * VCONN.  Should indicate power status of system - whether it is
 * sourcing or sinking or neutral with respect to VBUS and VCONN.
 */
usbtc_power_status_t appcfg_GetPowerStatus()
{
  if(pol_is_source_role())
  {
	  return usbtc_power_sourcing;
  }
  else
  {
    return usbtc_power_neutral;
  }
}
