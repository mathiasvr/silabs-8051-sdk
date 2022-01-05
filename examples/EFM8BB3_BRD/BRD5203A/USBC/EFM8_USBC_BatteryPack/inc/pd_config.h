/***************************************************************************//**
 * @file
 * @brief   Header file for user configuration of USB Type-C application
 *          definition
 * @author  Silicon Laboratories
 * @version 1.0.0
 *
 * This header file contains the macro definition, function declarations
 * that support the definition of the USB Type-C application.
 *******************************************************************************
 * @section License
 * (C) Copyright 2015 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *******************************************************************************
 * @addtogroup usbtc_pd   USB Type-C Power Delivery Framework
 * @{
 *******************************************************************************/
#ifndef __PD_CONFIG_H__
#define __PD_CONFIG_H__

#include <SI_EFM8BB3_Register_Enums.h>                  // SFR declarations
#include "bsp.h"
#include "pd.h"

/***************************************************************************
 * PD_CONFIG_H_USER_VERSION - This number should match the value evaluated
 * by the arithmetic operation in PD_CONFIG_H_SYSTEM_VERSION in pd.h.
 *
 * THE FOLLOWING LINE MUST NOT BE USED:
 *
 * #define PD_CONFIG_H_USER_VERSION   MAGIC_VERSION(PD_CONFIG_H_SYSTEM_VERSION)
 *
 * to workaround user responsibility to use the correct version of
 * pd_config.h.
 *
 * #define PD_CONFIG_H_USER_VERSION   MAGIC_VERSION(10)
 *
 ***************************************************************************/
#define PD_CONFIG_H_USER_VERSION  MAGIC_VERSION(10)
#if BADUSAGE_VER(PD_CONFIG_H)
#error "You cannot put PD_CONFIG_H_SYSTEM_VERSION inside MAGIC_VERSION()"
#elif MISMATCH_VER(PD_CONFIG_H)
#error "Mismatched version of pd_config.h and pd.h"
#error "Please use the correct version of pd_config.h to build the application."
#error "Refer to pd_config.h in ufp_dongle project for the correct version"
#endif

/***************************************************************************//**
 * @addtogroup pdappdef    USB Type-C Power Delivery application definition
 *
 * Application definition
 * ----------------------
 * This section contains the definitions that define the type of
 * USB Type-C product application and allows user to fine-tune the
 * performance of their application.
 *
 * @{
 ******************************************************************************/

/** @name Application configuration definitions
 * @{
 * */
/***************************************************************************//**
 * @brief
 *    Power capability of the application.
 *
 * Specifies the power capability of the application.  This should be one of
 * PD_APP_PWRCAP_XXX defined in pd.h.
 * Battery Pack application is a PD_APP_PWRCAP_DYNAMIC_SELECTION.
 ******************************************************************************/
#define PD_APP_PWRCAP             PD_APP_PWRCAP_DYNAMIC_SELECTION

/***************************************************************************//**
 * @brief
 *    Type of port interface of the application.
 *
 * Specifies the type of USB Type C port interface of the application.
 * This should be one of PD_APP_INTERFACE_TYPE_XXX defined in pd.h.
 * Battery Pack application uses Type-C receptacle
 ******************************************************************************/
#define PD_APP_INTERFACE_TYPE     PD_APP_INTERFACE_TYPE_RECEPTACLE

/***************************************************************************//**
 * @brief
 *    Data role of the application.
 *
 * Specifies the data role capability of the application. This should be one of
 * PD_APP_DR_XXX defined in pd.h.
 * Battery Pack application starts as a DRP
 ******************************************************************************/
#define PD_APP_DR                 PD_APP_DR_DRP

/***************************************************************************//**
 * @brief
 *    Specifies whether VCONN swap capability is enabled in the application
 *    and hardware.
 *
 * Specifies whether the application support swapping VCONN.  This should be
 * one of PD_APP_VCONN_SWAP_XXX defined in pd.h.  See section 6.3.11. of
 * USB Type-C specification to comprehend the hardware requirements for
 * this capability.
 * Battery Pack application does not allow VCONN SOURCE swap
 ******************************************************************************/
#define PD_APP_VCONN_SWAP         PD_APP_VCONN_SWAP_DISABLED

/***************************************************************************//**
 * @brief
 *    USB Type C port type of the application.
 *
 * Specifies the Type-C port type of the application. This should be one of
 * PD_APP_PORT_XXX defined in pd.h.
 * Battery Pack application is a DRP
 ******************************************************************************/
#define PD_APP_PORT               PD_APP_PORT_DRP

/***************************************************************************//**
 * @brief
 *    Specify the ability of the application and hardware to float the Rd
 *    termination resistors
 *
 * If the application can function as both a source and a sink, then it
 * must have the ability to float the Rd termination resistors.  This
 * definition is to specify whether the hardware supports this
 * capability.  This should be one of PD_APP_CAN_FLOAT_RD_XXX defined in
 * pd.h.  This feature is required if the application needs to support
 * Disabled state, or ErrorRecovery state, or has both consumer and provider
 * capabilities
 ******************************************************************************/
#define PD_FEATURE_CAN_FLOAT_RD   PD_APP_CAN_FLOAT_RD_ENABLED

/***************************************************************************//**
 * @brief
 *    Specifies the ADC Mux channel connected to the CC1 pin in the
 *    application.
 ******************************************************************************/
#define HWCFG_CC1_ADCMX           0x07      // P1.1

/***************************************************************************//**
 * @brief
 *    Specifies the CMP0 Mux channel connected to the CC1 pin in the
 *    application.
 ******************************************************************************/
#define HWCFG_CC1_CP0MX           0x07      // P1.1

/***************************************************************************//**
 * @brief
 *    Specifies the ADC Mux channel connected to the attenuated VBUS pin in the
 *    application.
 ******************************************************************************/
#define HWCFG_VBUS_ADCMX          19        // P2.6

/** @name Connection Attach/Detach detection
 * @{
 */
/***************************************************************************//**
 * @brief
 *    Specifies the CMP1 Mux channel connected to the pin (either VBUS or VCONN)
 *    for detecting attach or detach by a sink device
 ******************************************************************************/
#define HWCFG_ATTACHDETACH_CP1MX  0x08    // P2.6

/***************************************************************************//**
 * @brief
 *    Specifies the attach threshold level (in mV) for the CMP1 Mux channel
 *    at HWCFG_ATTACHDETACH_CP1MX.  Normally, this level should be slightly
 *    below vSafe5V (4.00V) to give sufficient margin to achieve robust
 *    performance.
 ******************************************************************************/
#define HWCFG_ATTACHDETACH_MILLIVOLTS   396

/***************************************************************************//**
 * @brief
 *    Specifies the resistor value (in kohms) used on the high-side of the
 *    VBUS/VCONN resistor divider - this should be the one connected to VBUS.
 ******************************************************************************/
#define HWCFG_RVBUS             91

/***************************************************************************//**
 * @brief
 *    Specifies the resistor value (in kohms) used on the low-side of the
 *    VBUS/VCONN resistor divider - this should be the one connected to GND.
 ******************************************************************************/
#define HWCFG_RGND              10
/** @} (end name Connection Attach/Detach detection)*/


/** @name Hardware debugging features
 * @{
 */
/***************************************************************************//**
 * @brief
 *    When set to 1, enables viewing of the SPI MOSI signal during BMC RX.
 *    User can observe the decoded received BMC data on the DBG_SPI_MOSI pin
 *    on P2.5 of the EFM8BB3 MCU.
 ******************************************************************************/
#define HWCFG_DEBUG_VIEW_RXMOSI 0

/***************************************************************************//**
 * @brief
 *    When set to 1, enables viewing of the SPI SCK signal during BMC RX and TX.
 *    User can observe the decoded clock on the DBG_SPI_SCK pin
 *    on P1.0 of the EFM8BB3 MCU.  User should expect to observe a 300 kHz
 *    clock.
 ******************************************************************************/
#define HWCFG_DEBUG_VIEW_SPISCK 0

/***************************************************************************//**
 * @brief
 *    When set to 1, enables viewing of the end of TX signal of the BMC TX
 *    operation.  This pin will be logic high when BMC TX completes.  User
 *    can observe this signal on the DBG_EOTX pin on P2.2 of the EFM8BB3 MCU.
 ******************************************************************************/
#define HWCFG_DEBUG_VIEW_EOTX   0

/***************************************************************************//**
 * @brief
 *    When set to 1, enables viewing of the digitized version of the BMC TX
 *    signal at DBG_BMC_OUT on P0.2 of the EFM8BB3 MCU.  The signal observed
 *    on this pin should be in phase with the analog signal observed on the
 *    actual CC pin.
 ******************************************************************************/
#define HWCFG_DEBUG_VIEW_DBGBMC 0

/** @} (end name Hardware debugging features)*/

#message "NOTE:\
Source power is based on Rp value on the board. In this application \
Rp value is 12Kohm which is 1.5A."
#define APP_SRCPWR_CAP	USB_POWER_1P5A_5V

/** @name Timing-related fine-tuning
 *
 * Normally, these parameters should not be changed by the user.  It is
 * probably best to accept the defaults specified in these timing parameters
 * to ensure the application is robustly compliant with the USB Type-C
 * specs and the USB Type-C PD specs.
 *
 * @{
 */
/***************************************************************************//**
 * @brief
 *    Defines the tInterFrameGap timing in units of microseconds.  Refer
 *    to USB Type-C Specs Table 5-14.  This parameter is used to fill
 *    the neg_interframe_gap_ticks field in the pd_config_t structure.  The
 *    helper macro US_TO_SYSCLKDIV12_TICKS() is used to convert this
 *    value to fill in neg_interframe_gap_ticks.
 *
 *    Default: 27 us
 ******************************************************************************/
#define PD_INTERFRAMEGAP_US   27

/***************************************************************************//**
 * @brief
 *    Defines the nominal BMC RX bit rate in units of kbps.  The EFM8BB3
 *    has a clock recovery mechanism that allows it to track the received
 *    BMC signal properly even if there is drift in the clock.  The
 *    nominal BMC RX bit period is used to determine other values like
 *    the BMC RX timeout to detect end of reception, and to fill in fields
 *    like neg_bmc_rx_0_75_period_systicks within pd_config_system structure.
 *
 *    Default: 297
 ******************************************************************************/
#define PD_NOMINAL_BMCRX_BITRATE_KBPS   297

/***************************************************************************//**
 * @brief
 *    Defines the BMC RX timeout in units of nominal 0.1 BMC bit period.  Note
 *    that trimming this too small may lead to missed packets because this may
 *    cause early termination of reception of a packet.
 *
 *    Default: 12  (x 0.1 BMC bit period)
 ******************************************************************************/
#define PD_BMCRX_TIMEOUT_PT1BITPERIOD   12

/***************************************************************************//**
 * @brief
 *    Defines the tBISTContMode timeout in units of msec.
 *    See USB Type-C PD Spec, Table 6-30.
 *
 *    Default: 45 ms
 ******************************************************************************/
#define TBISTCONTMODE_MS    45

/***************************************************************************//**
 * @brief
 *    Defines the tErrorRecovery timeout in units of msec.
 *    See USB Type-C Spec, Table 4-20.
 *
 *    Default: 30 ms
 ******************************************************************************/
#define TERRORRECOVERY_MS 30

/***************************************************************************//**
 * @brief
 *    Defines the tPDDebounce timeout in units of msec.
 *    See USB Type-C Spec, Table 4-20.
 *
 *    Default: 15 ms
 ******************************************************************************/
#define TPDDEBOUNCE_MS  15

/***************************************************************************//**
 * @brief
 *    Defines the tCCDebounce timeout in units of msec.
 *    See USB Type-C Spec, Table 4-20.
 *
 *    Default: 150 ms
 ******************************************************************************/
#define TCCDEBOUNCE_MS  150

/***************************************************************************//**
 * @brief
 *    Defines the tReceive timeout in units of microseconds.
 *    See USB Type-C Spec, Table 4-20.
 *
 *    Default: 1000 us
 ******************************************************************************/
#define TRECEIVE_US     1000

/***************************************************************************//**
 * @brief
 *    Defines the tHardResetComplete timeout in units of microseconds.
 *    See USB Type-C Spec, Table 4-20.
 *
 *    Default: 4500 us
 ******************************************************************************/
#define THARDRESETCOMPLETE_US   4500

/***************************************************************************//**
 * @brief
 *    Defines the tNoResponse timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30.
 *
 *    Default: 5000 ms
 ******************************************************************************/
#define TNORESPONSE_MS    5000

/***************************************************************************//**
 * @brief
 *    Defines the tSinkRequest timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30.
 *
 *    Default: 108 ms
 ******************************************************************************/
#define TSINKREQUEST_MS   108

/***************************************************************************//**
 * @brief
 *    Defines the tTypeCSinkWaitCap timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30.
 *
 *    Default: 465 ms
 ******************************************************************************/
#define TTYPECSINKWAITCAP_MS    465

/***************************************************************************//**
 * @brief
 *    Defines the tPSTransition timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30.
 *
 *    Default: 500 ms
 ******************************************************************************/
#define TPSTRANSITION_MS      500

/***************************************************************************//**
 * @brief
 *    Defines the tSenderResponse timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30.
 *
 *    Default: 27 ms
 ******************************************************************************/
#define TSENDERRESPONSE_MS    27

/***************************************************************************//**
 * @brief
 *    Defines the tDRP in units of milliseconds and multiple of 10ms (50,60,..)
 *    See USB Type-C Spec, Table 4-19.
 *
 *    Default: 100 ms
 ******************************************************************************/
#define TDRP_MS								100

/***************************************************************************//**
 * @brief
 *    Debounce for detach event as a sink in order to do the last
 *    minute communication.
 *    This will delay to call app_Detached() as a sink.
 *
 *    For charging port in the reference board: smaller value is more time for
 *    last minute communication,
 *    because it report Battery Pack disconnection info to PD0(plug side)
 *    quickly.
 *
 *    For normal sink port in the reference board: bigger value is more chance
 *    to success communication.
 *
 *    SINK SIDE (CHARGING PORT)
 *
 *    ---|
 *       |_________  VBUS
 *
 *    ---|
 *       |_________  PHY (CMP1)
 *
 *
 *    ---+-+--|
 *            |_______ app_Detached() and inform to PD0
 *
 *
 *    THE OTHER SINK connected to plug.
 *
 *    ---+-+------|
 *                |_____ app_Detached()
 *
 *    Default: 20 ms for plug side - no preference value. Application choice.
 ******************************************************************************/
#define TSNKDETACHDEBOUNCE_MS 20

/***************************************************************************//**
 * @brief
 *    Defines the tDiscoverIdentity timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 45 ms
 ******************************************************************************/
#define TDISCOVERIDENTITY_MS  45

/***************************************************************************//**
 * @brief
 *    Defines the tPSHardReset timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 30 ms
 ******************************************************************************/
#define TPSHARDRESET_MS       30

/***************************************************************************//**
 * @brief
 *    Defines the tPSSourceOff timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 835 ms
 ******************************************************************************/
#define TPSSOURCEOFF_MS       835

/***************************************************************************//**
 * @brief
 *    Defines the tSourceOn timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 435 ms
 ******************************************************************************/
#define TSOURCEON_MS          435

/***************************************************************************//**
 * @brief
 *    Defines the tSinkActivity timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 135 ms
 ******************************************************************************/
#define TSINKACTIVITY_MS      135

/***************************************************************************//**
 * @brief
 *    Defines the tSourceActivity timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 45 ms
 ******************************************************************************/
#define TSOURCEACTIVITY_MS    45

/***************************************************************************//**
 * @brief
 *    Defines the tSourceCapability timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 150 ms
 ******************************************************************************/
#define TSOURCECAPABILITY_MS  150

/***************************************************************************//**
 * @brief
 *    Defines the tSwapRecover timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 750 ms
 ******************************************************************************/
#define TSWAPRECOVER_MS       750

/***************************************************************************//**
 * @brief
 *    Defines the tSwapSourceStart timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 23 ms
 ******************************************************************************/
#define TSWAPSOURCESTART_MS   23

/***************************************************************************//**
 * @brief
 *    Defines the tVconnSourceOn timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 50 ms
 ******************************************************************************/
#define TVCONNON_MS           50

/***************************************************************************//**
 * @brief
 *    Defines the tVDMWaitModeEntry timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 45 ms
 ******************************************************************************/
#define TVDMMODEENTRY         45

/***************************************************************************//**
 * @brief
 *    Defines the tVDMWaitModeExit timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 45 ms
 ******************************************************************************/
#define TVDDMODEEXIT          45

/***************************************************************************//**
 * @brief
 *    Defines the tVDMSenderResponse timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 27 ms
 ******************************************************************************/
#define TVDMRESPONSE          27

/***************************************************************************//**
 * @brief
 *    Defines the tSrcTransition timeout in units of milliseconds.
 *    See USB Type-C PD Spec, Table 6-30, 6-31
 *
 *    Default: 30 ms
 ******************************************************************************/
#define TSRCTRANSITION        30

/** @} (end name Timing-related fine-tuning) */
/** @} (end name Application configuration definitions) */
/** @} (end addtogroup pdappdef)  */

/** @}  (end addtogroup usbtc_pd)  */

#endif  // __PD_CONFIG_H__
