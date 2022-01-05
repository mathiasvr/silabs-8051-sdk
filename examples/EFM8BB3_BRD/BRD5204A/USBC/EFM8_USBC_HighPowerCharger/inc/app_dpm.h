/**************************************************************************//**
 * @file
 * @brief   Header file for application-specific Device Policy Manager
 *
 * @author  Silicon Laboratories
 * @version 1.0.0
 *
 * This header file contains the macro definition, function declarations
 * that support the definition.
 *******************************************************************************
 * @section License
 * (C) Copyright 2017 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *******************************************************************************
 * @addtogroup usbtc_pd   USB Type-C Power Delivery Framework
 * @{
 *
 *****************************************************************************/

/* Standard prologue for variable definition and declaration in header file */
#if defined(__DEFINE_VARIABLES) && !defined(APP_DPM_H_DEFINITIONS)
#undef __SILICON_LABS_APP_DPM_H__
#endif

#ifndef __SILICON_LABS_APP_DPM_H__
#define __SILICON_LABS_APP_DPM_H__

#include <si_toolchain.h>
#include "external.h"

#if !defined(__DEFINE_VARIABLES)


/***************************************************************************//**
 * @addtogroup high_power_charger
 * @brief USB Type-C High-Power Charger Reference Design
 *
 * @{
 *
 *   @section high_power_charger_contents Contents
 *
 *   @li @ref high_power_charger_revision
 *   @li @ref high_power_charger_intro
 *   @li @ref high_power_charger_instructions
 *
 *   @n @section high_power_charger_revision Revision
 *   Revision: 1.0.0
 *
 *   @n @section high_power_charger_intro Introduction
 *
 *   The USB Type-C (USBC) High-Power Charger Reference Design implements a
 *   high-power (up to 60W) charging port. It can provide up to 3 amps at the
 *   following voltages:
 *
 *     - 5V
 *     - 9V
 *     - 12V
 *     - 15V
 *     - 20V
 *
 *   This code is intended to run on the 60 Watt Charger Board (BRD5204A).
 *
 *   @n @section high_power_charger_instructions Instructions for Use
 *
 *   The High-Power Charger is Source only application. It connects to a
 *   Sink, offers all supported voltages to the Sink, and provides the
 *   voltage that the Sink requests on VBus.
 *
 *   The LED's are status LED's:
 *
 *     - When LED0 is on, the device is attached.
 *     - When LED1 is on, the device is charging the Sink.
 *
 *   @n @section high_power_charger_debug Debug Messages
 *
 *   The High-Power Charger outputs debug message on its UART Tx pin located on
 *   P1.7. The UART is configured to run at a high speed (921600 bps) to prevent
 *   the debug messages from interfering with the timing-sensitive USBC PD
 *   communication. In order to receive debug messages, the user must attach an
 *   external serial device capable of receiving UART data at 921600 to P1.7.
 *
 ******************************************************************************/


/***************************************************************************//**
 * @addtogroup appdpm_grp    User Application
 *
 * This is the application specific Device Policy Manager (DPM) of USB Type-C
 * based High-Power Charger Application.
 *
 * The DPM in Silicon Labs USB PD Library depends on application specific DPM to
 * fulfill application needs. It uses call-back functions based interface between
 * the DPMs. The library requires that all these functions must be implemented
 * by the application. These call-back functions are called by the library under
 * different conditions. It is the responsibility of the application to take
 * appropriate actions under these conditions.
 *
 * The High-Power Charger application implements all call-back functions
 * required by the library although some of these functions are irrelevant to
 * this application.
 *
 * This file defines:
 * - Prototypes of all call-back functions in accordance with USB PD Library
 * - Messages handled by the DPM specific to this application
 * - Macros to extract different fields from data objects
 * - Macros to build data objects from fields
 * - Other useful macros
 *
 * It also declares variables and other data objects maintained by application
 * specific DPM:
 * - Object to define capabilities of local port
 * - Object to track the capabilities of port partner

 *
 * @{
 ******************************************************************************/


/** @name Example application specific constant
 *
 * This is constant for this specific example application.
 *
 * @{
 */
#define VOLTAGE_5000mV        (5000)  /**< Default voltage (VSafe5V) */
#define VOLTAGE_9000mV        (9000)  /**< Offered voltage (9V) */
#define VOLTAGE_12000mV       (12000) /**< Offered voltage (12V) */
#define VOLTAGE_15000mV       (15000) /**< Offered voltage (15V) */
#define VOLTAGE_20000mV       (20000) /**< Offered voltage (20V) */

#define SRC_DEFAULT_CHARGING_CURRENT  1500  /**< Default charging current available */

#define TEMPERATURE_C_OFF       80      /**< Temperature in C at which to stop sourcing/sinking current */
#define TEMPERATURE_C_ON        50      /**< Temperature in C at which to resume sourcing/sinking current */

#define TEMP_SENSOR_SLOPE_MV_C  2.82    /**< Slope of temperature sensor in mV/C */
#define TEMP_SENSOR_OFFSET_MV   751     /**< Offset of temperature sensor in mV */

/** @} (end name Example application specific constant)*/


/** @name User Defined Message IDs
 *
 * This application defines following message IDs:
 * - MSG_APP_PORT_ATTACHED to handle attachment of port partner
 * - MSG_APP_PORT_DETACHED to handle detachment of port partner
 * - MSG_APP_PORT_RECOVER to handle recovery of power supply to VSafe5V
 * - MSG_APP_PORT_NEW_SRC to handle change of power supply level
 *
 * These message IDs are specific to this application and are handled by the
 * DPM of this application. USB PD library requires that these message IDs must
 * be 0x8000 or greater in order for library to ignore them in its internal DPM.
 *
 * @{
 */
#define MSG_APP_PORT_ATTACHED   0x8001 /**< Attached message */
#define MSG_APP_PORT_DETACHED   0x8002 /**< Detached message */
#define MSG_APP_PORT_RECOVER    0x8003 /**< Recover Power Supply to VSafe5V */
#define MSG_APP_PORT_NEW_SRC    0x8004 /**< Power Supply is set to new level */

/// Messages to System Policy
typedef enum
{
  MSG_SYS_DUMMY = 0x9000,
  MSG_SYS_PORT_ATTACHED,
  MSG_SYS_PORT_DETACHED,
  MSG_SYS_CONTRACT_DONE,
  MSG_SYS_CONTRACT_FAILED,
  MSG_SYS_RX_SNK_CAP,
  MSG_SYS_ADC_TIMEOUT,
  MSG_SYS_HR_DONE,
} system_msgid_t;


/// System Policy states
typedef enum
{
  // Unattached state
  SPM_Unattached = 0,

  // Sink/UFP states
  SPM_Snk_Ufp_Ready,
  SPM_Snk_Ufp_wait_for_contract,

  // Source/DFP states
  SPM_Src_Dfp_Ready,
  SPM_Src_Dfp_wait_for_contract,
  SPM_Src_Dfp_wait_for_SNK_Cap,
  SPM_Src_Dfp_contract_failed,

  // Error states
  SPM_Req_HardReset,
  SPM_Force_to_init,
  SPM_last_state,
}spm_State_t;


/// SPM State Machine struct
typedef struct
{
  spm_State_t state;
  spm_State_t lastState;
  uint8_t giveback: 1;
  uint8_t capabilityMismatch: 1;
  uint8_t reserved: 6;
} _spm_SM_t;

typedef _spm_SM_t SI_SEG_XDATA spm_SM_t;

/** @} (end name user defined message ID)*/

/***************************************************************************//**
 * @addtogroup apphelpmac    USB Type-C Capabilities Data Object Helper Macros
 *
 * Following macros are just example to help code in application based on spec.
 *
 * These are for this application. Depend on user's application (E.g. Battery
 * Supply Power Object), user can define more.
 *
 * @{
 ******************************************************************************/


/** @name Types of PDO
 * @{
 */
#define PDO_FIXED_SUPPLY_TYPE         0
#define PDO_BATTERY_TYPE              1
#define PDO_VARIABLE_SUPPLY_TYPE      2

/***************************************************************************//**
 * @brief
 *    Extracts the type of Source Supply from given Power Data Object (PDO)
 *
 * @param pdo
 *    A PDO of type SI_UU32_t
 *
 * @return
 *    Type of PDO. It is one of:
 *    PDO_FIXED_SUPPLY_TYPE
 *    PDO_BATTERY_TYPE
 *    PDO_VARIABLE_SUPPLY_TYPE
 *
 * See USB Power Delivery Specification, section 6.4.1 for more details.
 ******************************************************************************/
#define SRC_PDO_SUPPLY_TYPE(pdo)        ((pdo.uu16[MSB].u8[MSB] & 0xC0) >> 6)

/***************************************************************************//**
 * @brief
 *    Extracts the type of power required by Sink from given Power Data Object (PDO)
 *
 * @param pdo
 *    A PDO of type SI_UU32_t
 *
 * @return
 *    Type of PDO. It is one of:
 *    PDO_FIXED_SUPPLY_TYPE
 *    PDO_BATTERY_TYPE
 *    PDO_VARIABLE_SUPPLY_TYPE
 *
 * See USB Power Delivery Specification, section 6.4.1 for more details.
 ******************************************************************************/
#define SNK_PDO_SUPPLY_TYPE(pdo)        ((pdo.uu16[MSB].u8[MSB] & 0xC0) >> 6)

/** @} (end name types of PDO)*/

/***************************************************************************//**
 * @brief
 *    Converts given voltage in units of mV to a voltage in  units of 50mV
 ******************************************************************************/
#define   PDO_VOLTAGE_IN_50MV(mV)       ((mV + 25) / 50)

/***************************************************************************//**
 * @brief
 *    convert mA in 10mA unit
 ******************************************************************************/
#define   PDO_CURRENT_IN_10MA(mA)       ((mA + 5) / 10)


/** @name Source Fixed Supply PDO
 * @{
 */
#define SRC_PDO_FIXED_SUPPLY_PR_SWAP_NO        0
#define SRC_PDO_FIXED_SUPPLY_PR_SWAP_YES       1
#define SRC_PDO_FIXED_SUPPLY_USB_SUSPEND_NO    0
#define SRC_PDO_FIXED_SUPPLY_USB_SUSPEND_YES   1
#define SRC_PDO_FIXED_SUPPLY_EXT_POWER_NO      0
#define SRC_PDO_FIXED_SUPPLY_EXT_POWER_YES     1
#define SRC_PDO_FIXED_SUPPLY_USB_COMM_NO       0
#define SRC_PDO_FIXED_SUPPLY_USB_COMM_YES      1
#define SRC_PDO_FIXED_SUPPLY_DR_SWAP_NO        0
#define SRC_PDO_FIXED_SUPPLY_DR_SWAP_YES       1
#define SRC_PDO_FIXED_SUPPLY_PEAK_CUR_IOC      0
#define SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OVRLD_1  1
#define SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OVRLD_2  2
#define SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OVRLD_3  3

/***************************************************************************//**
 * @brief
 *    Constructs a Source Fixed Supply Power Data Object (PDO)
 *
 * @param drp_cap
 *    Dual Role Power capability. Select from:
 *    SRC_PDO_FIXED_SUPPLY_PR_SWAP_NO
 *    SRC_PDO_FIXED_SUPPLY_PR_SWAP_YES
 *
 * @param usb_suspend
 *    USB Suspend capability. Select from:
 *    SRC_PDO_FIXED_SUPPLY_USB_SUSPEND_NO
 *    SRC_PDO_FIXED_SUPPLY_USB_SUSPEND_YES
 *
 * @param ext_powered
 *    External Power Source. Select from:
 *    SRC_PDO_FIXED_SUPPLY_EXT_POWER_NO
 *    SRC_PDO_FIXED_SUPPLY_EXT_POWER_YES
 *
 * @param usb_comm
 *    USB Communications capability. Select from:
 *    SRC_PDO_FIXED_SUPPLY_USB_COMM_NO
 *    SRC_PDO_FIXED_SUPPLY_USB_COMM_YES
 *
 * @param drd_cap
 *    Dual Role Data capability. Select from:
 *    SRC_PDO_FIXED_SUPPLY_DR_SWAP_NO
 *    SRC_PDO_FIXED_SUPPLY_DR_SWAP_YES
 *
 * @param peak_current_cap
 *    Peak Current capability. Select from:
 *    SRC_PDO_FIXED_SUPPLY_PEAK_CUR_IOC
 *    SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OVRLD_1
 *    SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OVRLD_2
 *    SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OVRLD_3
 *
 * @param voltage
 *    Voltage in units of mV
 *
 * @param max_current
 *    Maximum Current in units of mA
 *
 * This macro can be used to construct a 32-bit source power data object
 * See USB Power Delivery Specification, section 6.4.1.2 for more details.
 ******************************************************************************/
#define SRC_PDO_FIXED_SUPPLY(drp_cap, usb_suspend, ext_powered, usb_comm, \
                             drd_cap, peak_current_cap, voltage, max_current) (\
  ((uint32_t)PDO_FIXED_SUPPLY_TYPE  << 30) | \
  ((uint32_t)((drp_cap)     & 0x01) << 29) | \
  ((uint32_t)((usb_suspend) & 0x01) << 28) | \
  ((uint32_t)((ext_powered) & 0x01) << 27) | \
  ((uint32_t)((usb_comm)    & 0x01) << 26) | \
  ((uint32_t)((drd_cap)     & 0x01) << 25) | \
  ((uint32_t)((peak_current_cap)               & 0x03)   << 20) | \
  ((uint32_t)(PDO_VOLTAGE_IN_50MV(voltage)     & 0x03FF) << 10) | \
  ((uint32_t)(PDO_CURRENT_IN_10MA(max_current) & 0x03FF) << 0) )

/***************************************************************************//**
 * @brief
 *    Extracts Dual Role Power capability from Source Fixed Supply PDO.
 *
 * @param pdo
 *    A PDO of type SI_UU32_t
 *
 * @return
 *    SRC_PDO_FIXED_SUPPLY_PR_SWAP_NO or
 *    SRC_PDO_FIXED_SUPPLY_PR_SWAP_YES
 *
 * See USB Power Delivery Specification, section 6.4.1.2 for more details.
 ******************************************************************************/
#define SRC_PDO_FIXED_SUPPLY_PR_SWAP(pdo)  ((pdo.uu16[MSB].u8[MSB] & 0x20) >> 5)

/***************************************************************************//**
 * @brief
 *    Extracts USB Suspend capability from Source Fixed Supply PDO.
 *
 * @param pdo
 *    A PDO of type SI_UU32_t
 *
 * @return
 *    SRC_PDO_FIXED_SUPPLY_USB_SUSPEND_NO
 *    SRC_PDO_FIXED_SUPPLY_USB_SUSPEND_YES
 *
 * See USB Power Delivery Specification, section 6.4.1.2 for more details.
 ******************************************************************************/
#define SRC_PDO_FIXED_SUPPLY_USB_SUSPEND(pdo) ((pdo.uu16[MSB].u8[MSB] & 0x10) >> 4)

/***************************************************************************//**
 * @brief
 *    Extracts External Power capability from Source Fixed Supply PDO.
 *
 * @param pdo
 *    A PDO of type SI_UU32_t
 *
 * @return
 *    SRC_PDO_FIXED_SUPPLY_EXT_POWER_NO
 *    SRC_PDO_FIXED_SUPPLY_EXT_POWER_YES
 *
 * See USB Power Delivery Specification, section 6.4.1.2 for more details.
 ******************************************************************************/
#define SRC_PDO_FIXED_SUPPLY_EXTERNAL_POWER(pdo) ((pdo.uu16[MSB].u8[MSB] & 0x08) >> 3)

/***************************************************************************//**
 * @brief
 *    Extracts USB Communications capability from Source Fixed Supply PDO.
 *
 * @param pdo
 *    A PDO of type SI_UU32_t
 *
 * @return
 *    SRC_PDO_FIXED_SUPPLY_USB_COMM_NO
 *    SRC_PDO_FIXED_SUPPLY_USB_COMM_YES
 *
 * See USB Power Delivery Specification, section 6.4.1.2 for more details.
 ******************************************************************************/
#define SRC_PDO_FIXED_SUPPLY_USB_COMM(pdo) ((pdo.uu16[MSB].u8[MSB] & 0x04) >> 2)

/***************************************************************************//**
 * @brief
 *    Extracts Dual Role Data capability from Source Fixed Supply PDO.
 *
 * @param pdo
 *    A PDO of type SI_UU32_t
 *
 * @return
 *    SRC_PDO_FIXED_SUPPLY_DR_SWAP_NO
 *    SRC_PDO_FIXED_SUPPLY_DR_SWAP_YES
 *
 * See USB Power Delivery Specification, section 6.4.1.2 for more details.
 ******************************************************************************/
#define SRC_PDO_FIXED_SUPPLY_DR_SWAP(pdo) ((pdo.uu16[MSB].u8[MSB] & 0x02) >> 1)

/***************************************************************************//**
 * @brief
 *    Extracts Peak Current capability from Source Fixed Supply PDO.
 *
 * @param pdo
 *    A PDO of type SI_UU32_t
 *
 * @return
 *    SRC_PDO_FIXED_SUPPLY_PEAK_CUR_IOC
 *    SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OVRLD_1
 *    SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OVRLD_2
 *    SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OVRLD_3
 *
 * See USB Power Delivery Specification, section 6.4.1.2 for more details.
 ******************************************************************************/
#define SRC_PDO_FIXED_SUPPLY_PEAK_CURRENT_MA(pdo) ((pdo.uu16[MSB].u8[LSB] & 0x30) >> 4)

/***************************************************************************//**
 * @brief
 *    Extracts Voltage in units of mV from Source Fixed Supply PDO.
 *
 * @param pdo
 *    A PDO of type SI_UU32_t
 *
 * @return
 *    Voltage in units of mV
 *
 * See USB Power Delivery Specification, section 6.4.1.2 for more details.
 ******************************************************************************/
#define SRC_PDO_FIXED_SUPPLY_VOLTAGE_MV(pdo) \
                                  ((((((uint16_t)(pdo.u8[1] & 0x0F)) << 8) + \
                                  (((uint16_t)(pdo.u8[2] & 0xFC)))) >> 2) * 50)

/***************************************************************************//**
 * @brief
 *    Extracts Maximum Current in units of mA from Source Fixed Supply PDO.
 *
 * @param pdo
 *    A PDO of type SI_UU32_t
 *
 * @return
 *    Maximum current in units of mA
 *
 * See USB Power Delivery Specification, section 6.4.1.2 for more details.
 ******************************************************************************/
#define SRC_PDO_FIXED_SUPPLY_MAX_CURRENT_MA(pdo) ((pdo.uu16[LSB].u16 & 0x03FF) * 10)
/** @} (end name Source Fixed Supply PDO)*/


/** @name Constant for type of fixed voltage source peak current
 * @{
 */
#define   SRC_PDO_FIXED_SUPPLY_PEAK_CUR_IOC      0
#define   SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OL_1     1
#define   SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OL_2     2
#define   SRC_PDO_FIXED_SUPPLY_PEAK_CUR_OL_3     3
/** @} (end name Constant for type of fixed voltage source peak current)*/


/** @name Sink Fixed Supply PDO
 * @{
 */
#define SNK_PDO_FIXED_SUPPLY_PR_SWAP_NO        0
#define SNK_PDO_FIXED_SUPPLY_PR_SWAP_YES       1
#define SNK_PDO_FIXED_SUPPLY_HIGH_CAP_NO       0
#define SNK_PDO_FIXED_SUPPLY_HIGH_CAP_YES      1
#define SNK_PDO_FIXED_SUPPLY_EXT_POWER_NO      0
#define SNK_PDO_FIXED_SUPPLY_EXT_POWER_YES     1
#define SNK_PDO_FIXED_SUPPLY_USB_COMM_NO       0
#define SNK_PDO_FIXED_SUPPLY_USB_COMM_YES      1
#define SNK_PDO_FIXED_SUPPLY_DR_SWAP_NO        0
#define SNK_PDO_FIXED_SUPPLY_DR_SWAP_YES       1

/***************************************************************************//**
 * @brief
 *    Constructs a Sink Fixed Supply Power Data Object (PDO)
 *
 * @param drp_cap
 *    Dual Role Power capability. Select from:
 *    SNK_PDO_FIXED_SUPPLY_PR_SWAP_NO
 *    SNK_PDO_FIXED_SUPPLY_PR_SWAP_YES
 *
 * @param higher_cap
 *    Need for voltage more than vSafe5V
 *
 * @param ext_powered
 *    External Power Source. Select from:
 *    SNK_PDO_FIXED_SUPPLY_EXT_POWER_NO
 *    SNK_PDO_FIXED_SUPPLY_EXT_POWER_YES
 *
 * @param usb_comm
 *    USB Communication capability. Select from:
 *    SNK_PDO_FIXED_SUPPLY_USB_COMM_NO
 *    SNK_PDO_FIXED_SUPPLY_USB_COMM_YES
 *
 * @param drd_cap
 *    Dual Role Data capability. Select from:
 *    SNK_PDO_FIXED_SUPPLY_DR_SWAP_NO
 *    SNK_PDO_FIXED_SUPPLY_DR_SWAP_YES
 *
 * @param voltage
 *    Voltage in units of 50mV
 *
 * @param max_current
 *    Maximum Current in units of 10mA
 *
 * This macro can be used to construct a 32-bit sink power data object
 * See USB Power Delivery Specification, section 6.4.1.3 for more details.
 ******************************************************************************/
#define SNK_PDO_FIXED_SUPPLY(drp_cap, higher_cap, ext_powered, usb_comm, \
                             drd_cap, voltage, max_current) (\
    ((uint32_t)PDO_FIXED_SUPPLY_TYPE  << 30) | \
    ((uint32_t)((drp_cap)     & 0x01) << 29) | \
    ((uint32_t)((higher_cap)  & 0x01) << 28) | \
    ((uint32_t)((ext_powered) & 0x01) << 27) | \
    ((uint32_t)((usb_comm)    & 0x01) << 26) | \
    ((uint32_t)((drd_cap)     & 0x01) << 25) | \
    ((uint32_t)(PDO_VOLTAGE_IN_50MV(voltage)     & 0x03FF) << 10) | \
    ((uint32_t)(PDO_CURRENT_IN_10MA(max_current) & 0x03FF) << 0) )

/** @} (end name Sink Fixed Supply PDO)*/

/** @name Fixed And Variable Supply RDO
 * @{
 */
#define RDO_FIXED_AND_VARIABLE_SUPPLY_OBJ_POS_SET(n)  ((n)  << 4)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_GIVEBACK_SET    (0x01 << 3)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_GIVEBACK_CLR    (0x00 << 3)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_MISMATCH_SET    (0x01 << 2)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_MISMATCH_CLR    (0x00 << 2)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_USB_COMM_SET    (0x01 << 1)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_USB_COMM_CLR    (0x00 << 1)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_USB_SUSPEND_SET (0x01 << 0)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_USB_SUSPEND_CLR (0x00 << 0)

/***************************************************************************//**
 * @brief
 *    Constructs a Fixed and Variable Supply Request Data Object (RDO)
 *
 * @param var
 *    A variable of type SI_UU32_t in memory where created RDO will be saved
 *
 * @param obj_pos
 *    Position of PDO in source capabilities message to be requested, value 1
 *    refers to the first PDO, 2 refers to second PDO and so on.
 *
 * @param giveback
 *    Indicates the ability of sink to respond to GotoMin message and reduce its
 *    load to Minimum Operating Current. Select from:
 *    RDO_FIXED_AND_VARIABLE_SUPPLY_GIVEBACK_SET
 *    RDO_FIXED_AND_VARIABLE_SUPPLY_GIVEBACK_CLR
 *
 * @param mismatch
 *    Indicates that none of capabilities offered by the source can satisfy the
 *    power requirements of the sink. Select from:
 *    RDO_FIXED_AND_VARIABLE_SUPPLY_MISMATCH_SET
 *    RDO_FIXED_AND_VARIABLE_SUPPLY_MISMATCH_CLR
 *    Note: Even if there is a mismatch, sink shall request one of offered PDOs
 *
 * @param usb_comm
 *    USB Communication capability. Select from:
 *    RDO_FIXED_AND_VARIABLE_SUPPLY_USB_COMM_SET
 *    RDO_FIXED_AND_VARIABLE_SUPPLY_USB_COMM_CLR
 *
 * @param no_usb_suspend
 *    Indicates that sink wants to continue power contract during USB Suspend.
 *    Select from:
 *    RDO_FIXED_AND_VARIABLE_SUPPLY_USB_SUSPEND_SET
 *    RDO_FIXED_AND_VARIABLE_SUPPLY_USB_SUSPEND_CLR
 *
 * @param opr_current
 *    The amount of current, in units of 10 mA, needed by sink to operate at a
 *    given time.
 *
 * @param max_current
 *    The amount of current, in units of 10mA, ever needed by sink to operate.
 *
 * This macro can be used to construct a 32-bit sink request data object and
 * save it a variable of type SI_UU32_t.
 * See USB Power Delivery Specification, section 6.4.2 for more details.
 ******************************************************************************/
#define RDO_FIXED_AND_VARIABLE_SUPPLY(var, obj_pos, giveback, mismatch, usb_comm,\
                                      no_usb_suspend, opr_current, max_current) \
do \
{  \
  var.u32 = 0L; \
  var.uu16[MSB].u8[MSB] += RDO_FIXED_AND_VARIABLE_SUPPLY_OBJ_POS_SET(obj_pos); \
  var.uu16[MSB].u8[MSB] += giveback; \
  var.uu16[MSB].u8[MSB] += mismatch; \
  var.uu16[MSB].u8[MSB] += usb_comm; \
  var.uu16[MSB].u8[MSB] += no_usb_suspend; \
  var.uu16[MSB].u8[LSB] += ((uint8_t)(((uint16_t)PDO_CURRENT_IN_10MA(opr_current) << 2) >> 8)); \
  var.uu16[LSB].u8[MSB] += ((uint8_t)((uint16_t)PDO_CURRENT_IN_10MA(opr_current) << 2)); \
  var.u16[LSB] += PDO_CURRENT_IN_10MA(max_current); \
} while(0)

#define RDO_FIXED_AND_VARIABLE_SUPPLY_OBJ_POS_GET(obj) ((obj.u8[MSB] & 0x70) >> 4)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_GIVEBACK(obj)    ((obj.u8[MSB] & 0x08) >> 3)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_MISMATCH(obj)    ((obj.u8[MSB] & 0x04) >> 2)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_USB_COMM(obj)    ((obj.u8[MSB] & 0x02) >> 1)
#define RDO_FIXED_AND_VARIABLE_SUPPLY_USB_SUSPEND(obj) ((obj.u8[MSB] & 0x01))
#define RDO_FIXED_AND_VARIABLE_OPR_CURRENT_MA(obj)     ((((((uint16_t)(obj.u8[1] & 0x0F)) << 8) + (((uint16_t)(obj.u8[2] & 0xFC)))) >> 2 ) * 10)
#define RDO_FIXED_AND_VARIABLE_MAX_CURRENT_MA(obj)     ((obj.uu16[LSB].u16 & 0x03FF) * 10)
#define RDO_FIXED_AND_VARIABLE_MIN_CURRENT_MA(obj)     ((obj.uu16[LSB].u16 & 0x03FF) * 10)

/** @} (end name Fixed And Variable Supply RDO))*/

#define USER_ADC_POLL_RATE_MS   500

/***************************************************************************//**
 * @addtogroup appvariable  Key variables for the High-Power Charger application
 *
 * This application declares, initializes and uses following variables.
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *    Source Capabilities of High-Power Charger Application
 *
 * It is an array of PDOs of type SRC_PDO_t. Collectively, these PDOs define the
 * power capabilities of source. Each PDO defines a specific power capability.
 * Source Capabilities are sent as a part of Source Capabilities Message or in
 * response to Get Source Capabilities message from port partner.
 ******************************************************************************/
SI_EXTERN SRC_PDO_t local_src_cap[];

/***************************************************************************//**
  * @brief
  *    Pointer to a Source Capabilities PDO
  *
  ******************************************************************************/
SI_EXTERN SRC_PDO_t * SI_SEG_XDATA local_src_cap_ptr;

/***************************************************************************//**
 * @brief
 *    Number of PDOs in Source Capabilities
 *
 ******************************************************************************/
SI_EXTERN uint8_t SI_SEG_XDATA num_local_src_cap;


/***************************************************************************//**
 * @brief
 *    Local copy of port partner Sink Capabilities
 *
 * It is an array of PDOs of type SNK_PDO_t and reflects the Sink Capabilities
 * of port partner. High-Power Charger application keeps track of Sink
 * Capabilities by updating these PDOs whenever a Sink Capabilities message is
 * received.
 ******************************************************************************/
SI_EXTERN SNK_PDO_t remote_snk_cap[];

/***************************************************************************//**
 * @brief
 *    Pointer to port partner Sink Capabilities
 *
 ******************************************************************************/
SI_EXTERN SNK_PDO_t * SI_SEG_XDATA remote_snk_cap_ptr;

/***************************************************************************//**
 * @brief
 *    Number of PDOs in port partner Sink Capabilities
 *
 ******************************************************************************/
SI_EXTERN uint8_t SI_SEG_XDATA num_remote_snk_cap;


/***************************************************************************//**
 * @brief
 *    Sink Capabilities of this Application
 *
 * It is an array of PDOs of type SNK_PDO_t. Collectively, these PDOs define the
 * power requirements of sink. Each PDO defines a specific power requirement.
 * Sink Capabilities are sent as a part of Sink Capabilities Message in response
 * to Get Sink Capabilities message from port partner.
 ******************************************************************************/
SI_EXTERN SNK_PDO_t local_snk_cap[];

/***************************************************************************//**
  * @brief
  *    Pointer to a Sink Capabilities PDO
  *
  ******************************************************************************/
SI_EXTERN SNK_PDO_t * SI_SEG_XDATA local_snk_cap_ptr;

/***************************************************************************//**
 * @brief
 *    Number of PDOs in Sink Capabilities
 *
 ******************************************************************************/
SI_EXTERN uint8_t SI_SEG_XDATA num_local_snk_cap;


/***************************************************************************//**
 * @brief
 *    Local copy of port partner Source Capabilities
 *
 * It is an array of PDOs of type SRC_PDO_t and reflects the Source Capabilities
 * of port partner. Application keeps track of Source Capabilities by updating
 * these PDOs whenever a Source Capabilities message is received.
 ******************************************************************************/
SI_EXTERN SRC_PDO_t remote_src_cap[];

/***************************************************************************//**
 * @brief
 *    Pointer to port partner Source Capabilities
 *
 ******************************************************************************/
SI_EXTERN SRC_PDO_t * SI_SEG_XDATA remote_src_cap_ptr;

/***************************************************************************//**
 * @brief
 *    Number of PDOs in port partner Source Capabilities
 *
 ******************************************************************************/
SI_EXTERN uint8_t SI_SEG_XDATA num_remote_src_cap;

/** @}  (end addtogroup appvariable)  */

/***************************************************************************//**
 * @addtogroup appfunc    Key functions in High-Power Charger Application
 *
 * This application implements following call-back functions required by Silicon
 * Labs USB PD Library.
 *
 * @{
 ******************************************************************************/










/***************************************************************************//**
 * @brief
 *    Initializes application specific Device Policy Manager.
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when it initializes device policy manager.
 *
 * It initializes message buffers and timers.
 *****************************************************************************/
void app_dpm_Init(void);

/***************************************************************************//**
 * @brief
 *    Handles port attach event
 *
 * @return
 *    None
 *
 * This is a call-back function required by USB PD library. Library calls it when
 * a port attach event is detected.
 *
 * High-Power Charger application needs to enable VBUS at vSafe5V in order for
 * sink to know the connection.
 *****************************************************************************/
void app_Attached(void);

/***************************************************************************//**
 * @brief
 *    Handles port detach event
 *
 * @return
 *    None
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when a port detach event is detected.
 *
 * High-Power Charger application needs to disable VBUS on detach event.
 *****************************************************************************/
void app_Detached(void);

/***************************************************************************//**
 * @brief
 *    Handles successful transmission of message to port partner
 *
 * This is a call-back function required by USB PD library. Library calls it when
 * GoodCRC is received for last transmitted message or a HardReset is generated
 *****************************************************************************/
void app_SentMsgSuccessfully(void);

/***************************************************************************//**
 * @brief
 *    Handles completion of contract
 *
 * This is a call-back function required by USB PD library. Library calls it
 * once contract negotiation is completed (indicated by successful transmission
 * of PS Ready message)
 *****************************************************************************/
void app_ContractDone(void);

//========================== CONSUMER RELATED FUNCTIONS ========================
// High-Power Charger application has to provide these dummy functions for USB
// PD Library
//==============================================================================

/***************************************************************************//**
 * @brief
 *    Tracks Source Capabilities of port partner
 *
 * @param pkt
 *    Pointer to the received packet containing Source Capabilities.
 *
 * @return
 *    None
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when a Source Capabilities Message is received from port partner.
 * It extracts Power Data Objects (PDOs) from the message and saves them to a
 * local copy of source PDOs maintained in the application space.
 * Refer to PD spec 8.3.3.3.3
 *****************************************************************************/
void app_UpdateNewSrcCapability(PD_Packet_t *pkt);

/***************************************************************************//**
 * @brief
 *    Evaluates Source Capabilities and prepares a request based on local policy
 *
 * @param prdo
 *    Pointer to the location where prepared request is to be saved
 *
 * @return
 *    None
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when a Source Capabilities Message is received from port partner.
 * It examines PDOs (saved from Source Capabilities Message), selects one of the
 * PDOs that satisfies local requirements (policy) and prepares a Request Data
 * Object (RDO) to be sent back to port partner. If none of PDOs can satisfy the
 * local requirements, it still selects one of the offered PDOs but sets the
 * Capability Mismatch bit in the RDO.
 * Refer to PD Spec. 8.3.3.3.4
 *****************************************************************************/
void app_EvaluateSrcCapability(uint32_t SI_SEG_XDATA *prdo);

/***************************************************************************//**
 * @brief
 *    Transitions Sink power level
 *
 * @param gotoMin
 *    If set, transitions to minimum operating current
 *    If clear, transitions to newly accepted power level
 *
 * @return
 *    None
 *
 * This is a call-back function required by USB PD library. Library calls it with
 * gotoMin set to 1 when GotoMin message is received from port partner. It is
 * called with gotoMin cleared to 0 when Accept message is received from port
 * partner. Sink power level is adjusted accordingly.
 *
 * See USB Power Delivery Specification, section 8.3.3.3.6 for more details.
 *****************************************************************************/
void app_TransitionToNewSinkPower(uint8_t gotoMin);

/***************************************************************************//**
 * @brief
 *    Transition to sink default conditions.
 *
 * This is a call-back function required by USB PD library. Library calls it when
 * a HardReset sent or received or BIST Carrier Mode 2 is timed-out. Library
 * assumes that appropriate actions (like switching VCONN OFF, Setting Data Role
 * to UFP etc), if required, are taken by this function.
 * See USB Power Delivery Specification, section 8.3.3.3.9 for more details.
 *****************************************************************************/
void app_SinkTransitionToDefault();

/***************************************************************************//**
 * @brief
 *    Prepares Sink Capabilities to be sent to port partner
 *
 * @param prdo
 *    Pointer to the location where Sink Capabilities are to be saved
 *
 * @return
 *    Number of PDOs in the prepared Sink Capabilities
 *
 * This is a call-back function required by USB PD library. Library calls it when
 * Get Sink Capabilities message is received from port partner. It copies PDOs
 * from local_snk_cap to the location pointed by prdo.
 *****************************************************************************/
uint8_t app_PresentSinkCapability(uint32_t SI_SEG_XDATA *prdo);


//========================== PROVIDER RELATED FUNCTIONS ========================
//==============================================================================

/***************************************************************************//**
 * @brief
 *    Determines Power Source availability
 *
 * @return
 *    True if power is available
 *    False if power is not available
 *
 * This is a call-back function required by USB PD library. Library calls it at
 * startup.
 *****************************************************************************/
bool app_IsSourcePowerAavailable(void);

/***************************************************************************//**
 * @brief
 *    Prepares Source Capabilities to be sent to port partner
 *
 * @param pdo
 *    Pointer to the location where Source Capabilities are to be saved
 *
 * @return
 *    Number of PDOs in the prepared Source Capabilities
 *
 * This is a call-back function required by USB PD library. Library calls it
 * before sending source capabilities to port partner. It copies PDOs from
 * local_src_cap to the location pointed by pdo.
 *****************************************************************************/
uint8_t app_PresentSrcCapability(uint32_t SI_SEG_XDATA *pdo);

/***************************************************************************//**
 * @brief
 *    Evaluates Sink Request
 *
 * @param prdo
 *    Pointer to the received sink request
 *
 * @return
 *    REQ_CANOT_BE_MET if sink request can not be satisfied
 *    REQ_CAN_BE_MET if sink request can be satisfied
 *    REQ_CAN_BE_MET_LATER if sink request can be satisfied later but not now
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when a Request Message is received from port partner. It examines the request
 * to determine whether it can satisfied or not.
 * Refer to PD Spec. 8.3.3.2.4
 *****************************************************************************/
uint8_t app_EvaluateSnkRequest(PD_Packet_t *pdo);

/***************************************************************************//**
 * @brief
 *    Determines whether source should direct sink to go minimum load
 *
 * @return
 *    True if sink should be directed to go to minimum load
 *    False if sink can be sourced to operate with present load
 * This is a call-back function required by USB PD library. Library calls it
 * after accepting port partner request.
 *****************************************************************************/
bool app_NeedGotoMin(void);

/***************************************************************************//**
 * @brief
 *    Configures Power Supply to generate requested power
 *
 * This is a call-back function required by USB PD library. Library calls it
 * after accepting port partner request. It configures power supply in order to
 * generate requested power. Once done, it sends MSG_DPM_POL_SRC_TRANSITION_DONE
 *
 * Refer to PD Spec. 8.3.3.2.5
 *****************************************************************************/
void app_PrepareNewSource(void);

/***************************************************************************//**
 * @brief
 *    Configures Power Supply and hardware to default settings
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when HardReset is received from port partner or DPM. It performs following:
 * - Configures power supply to VSafe0V and then to VSafe5V
 * - Resets local hardware
 * - Turns OFF VCONN
 * - Set Data Role to DFP
 * - Notify Policy Engine with MSG_DPM_POL_PWR_DEFAULT message
 *
 * Refer to PD Spec. 8.3.3.2.11
 *****************************************************************************/
void app_SrcTransitionToDefault(void);

/***************************************************************************//**
 * @brief
 *    Tracks Sink Capabilities of port partner
 *
 * @param pkt
 *    Pointer to the received packet containing Sink Capabilities.
 *
 * @return
 *    None
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when a Sink Capabilities Message is received from port partner. It extracts
 * Power Data Objects (PDOs) from the message and saves them to a local copy
 * of sink PDOs maintained in the application space.
 * Refer to PD spec 8.3.3.3.3
 *****************************************************************************/
void app_UpdateNewSnkCapability(PD_Packet_t *pkt);

//============================= ROLE SWAP FUNCTIONS ============================
//==============================================================================

/***************************************************************************//**
 * @brief
 *    Determines if data role can be switched from UFP to DFP
 *
 * @return
 *    VDM_ANS_ACCEPT if data role can be switched from UFP to DFP
 *    VDM_ANS_REJECT if data role can not be switched from UFP to DFP
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when a data role request is received to determine if data role swap is
 * allowed by application or not.
 *****************************************************************************/
uint8_t app_UfpDfpDrSwapSupport(void);

/***************************************************************************//**
 * @brief
 *    Determines if data role can be switched from DFP to UFP
 *
 * @return
 *    VDM_ANS_ACCEPT if data role can be switched from DFP to UFP
 *    VDM_ANS_REJECT if data role can not be switched from DFP to UFP
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when a data role request is received to determine if data role swap is
 * allowed by application or not.
 *****************************************************************************/
uint8_t app_DfpUfpDrSwapSupport(void);

/***************************************************************************//**
 * @brief
 *    Determines if power role can be switched between ARC and SNK
 *
 * @return
 *    VDM_ANS_ACCEPT if power role can be switched
 *    VDM_ANS_REJECT if power role can not be switched
 *    VDM_ANS_WAIT if power role can not be swapped at this time (may be later)
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when a power role request is received. It determine if power role swap can
 * be done.
 *****************************************************************************/
uint8_t app_UfpDfpPrSwapSupport(void);

/***************************************************************************//**
 * @brief
 *    Determines if VCONN role can be switched from SNK to SRC
 *
 * @return
 *    VDM_ANS_ACCEPT if VCONN role can be switched from SNK to SRC
 *    VDM_ANS_REJECT if VCONN role can not be switched from SNK to SRC
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when a VCONN role request is received to determine if VCONN role swap is
 * allowed by application or not.
 *****************************************************************************/
uint8_t app_UfpDfpVconnSwapSupport(void);

/***************************************************************************//**
 * @brief
 *    Data Role Swap from UFP to DFP
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during Data Role Swap with port partner. It changes port Data Role to DFP.
 *
 * Refer to PD Spec. 8.3.3.6.2.2.4
 *****************************************************************************/
void app_Change2Dfp(void);

/***************************************************************************//**
 * @brief
 *    Data Role Swap from DFP to UFP
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during Data Role Swap with port partner. It changes port Data Role to UFP.
 *
 * Refer to PD Spec. 8.3.3.6.2.1.4
 *****************************************************************************/
void app_Change2Ufp(void);

/***************************************************************************//**
 * @brief
 *    Turn OFF power sink
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during SNK to SRC Power Role Swap with port partner. It turns OFF power sink.
 *
 * Refer to PD Spec. 8.3.3.6.3.2.4
 *****************************************************************************/
void app_TurnOff_PowerSink(void);

/***************************************************************************//**
 * @brief
 *    Turn ON power supply at VSafe5V
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during SNK to SRC Power Role Swap with port partner. It turns ON power supply
 * on existing SNK to become a SRC.
 *
 * Refer to PD Spec. 8.3.3.6.3.2.6
 *****************************************************************************/
void app_TurnOn_Vsafe5V(void);

/***************************************************************************//**
 * @brief
 *    Turn OFF Source power supply
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during SRC to SNK Power Role Swap with port partner. It sends message to SPM
 *
 * Refer to PD Spec. 8.3.3.6.3.1.4
 *****************************************************************************/
void app_TurnOff_PowerSource(void);

/***************************************************************************//**
 * @brief
 *    Disable Power Delivery
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when it has not received a single Request message from port partner after
 * sending Source Capabilities message nCapsCount times. It provides VSafe5V on
 * VBUS.
 *
 * Refer to PD Spec. 8.3.3.2.7
 *****************************************************************************/
void app_PDDisabled(void);

/***************************************************************************//**
 * @brief
 *    Handles loss of VBUS
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when somehow it detects that VBUS is lost.
 *
 * Refer to PD Spec. 8.3.3.2.7
 *****************************************************************************/
void app_VbusGone(void);

/***************************************************************************//**
 * @brief
 *    Handles port partner response to Data Role Swap request
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when response to Data Role Swap request is received from port partner. Port
 * partner may respond with:
 * - Reject Message
 * - Wait Message
 * - Accept Message
 *
 * Refer to PD Spec. 8.3.3.6.2.1.5
 *****************************************************************************/
void app_DRSwapResponse(uint8_t resp);

/***************************************************************************//**
 * @brief
 *    Handles port partner response to Power Role Swap request
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when response to Power Role Swap request is received from port partner. Port
 * partner may respond with:
 * - Reject Message
 * - Wait Message
 * - Accept Message
 *
 * Refer to PD Spec. 8.3.3.6.3.1.7
 *****************************************************************************/
void app_PRSwapResponse(uint8_t resp);

/***************************************************************************//**
 * @brief
 *    Turn ON VCONN
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during VCONN Swap with port partner. It turns ON VCONN supply and notify
 * Policy Engine with MSG_DPM_POL_VCONN_ON message.
 *
 * Refer to PD Spec. 8.3.3.7.1.7
 *****************************************************************************/
void app_VconnOn(void);

/***************************************************************************//**
 * @brief
 *    Turn OFF VCONN
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during VCONN Swap with port partner. It turns OFF VCONN supply and notify
 * Policy Engine with MSG_DPM_POL_VCONN_OFF message.
 *
 * Refer to PD Spec. 8.3.3.7.1.6
 *****************************************************************************/
void app_VconnOff(void);


//====================== ALTERNATE MODE RELATED FUNCTIONS ======================
// Following functions are required by Silicon Labs USB PD Library but are not
// related to Charging Application. Hence implemented as empty functions.
//==============================================================================

uint8_t app_UfpVdmGetIdentity(uint32_t SI_SEG_XDATA *hdr, uint32_t SI_SEG_XDATA *pdo);

uint8_t app_UfpVdmGetSVIDs(uint32_t SI_SEG_XDATA *hdr, uint32_t SI_SEG_XDATA *pdo);

uint8_t app_UfpVdmGetMODEs(uint16_t id, uint32_t SI_SEG_XDATA *pdo);

uint8_t app_UfpVdmEnterMode(uint32_t SI_SEG_XDATA *hdr, uint32_t SI_SEG_XDATA *pdo);

uint8_t app_UfpVdmExitMode(uint32_t SI_SEG_XDATA *hdr, uint32_t SI_SEG_XDATA *pdo);

uint8_t app_HandleSVIDcmd(Packet_t *pkt, Packet_t *resp);

//========================== UTILITY FUNCTIONS ================================
//==============================================================================

/***************************************************************************//**
 * @brief
 *    Converts the ADC codes aquired from the temperature sensor channel to
 *    Celsius.
 *
 *  @param codes
 *    The ADC codes returned from the ADC's temperature sensor mux channel
 *
 * @return
 *    A rounded integer temperature in degrees Celsius.
 *
 *****************************************************************************/
uint16_t app_ConvertCodesToCelsius(uint16_t codes);

//================================ POLICY MANAGERS =============================

/***************************************************************************//**
 * @brief
 *    Device Policy Manager specific to High-Power Charger Application
 *
 * @param pmsg
 *    Pointer to message buffer
 *
 * @return
 *    None
 *
 * This is a call-back function required by USB PD library. Library calls it when
 * DPM message ID is 0x8000 or greater. Library assumes that such DPM messages
 * are handled in application space.
 *
 * Charging Application DPM handles following messages:
 * MSG_APP_PORT_ATTACHED
 * MSG_APP_PORT_DETACHED
 *
 * It calls pd_policy_get_buffer() to get buffer pointer which is pointing to
 * _PD_Packet_t.pkt (refer to pd_prl.h for structure details).
 *****************************************************************************/
void app_msgHandler(Xknltcb_msg_t *pmsg);

void app_SystemPolicy(Xknltcb_msg_t *pmsg);

#endif    // !defined(__DEFINE_VARIABLES)

/** @}  (end addtogroup appfunc)  */

/** @}  (end addtogroup appdpm_grp)  */

/** @}  (end addtogroup high_power_charger)  */

/** @}  (end addtogroup usbtc_pd)  */

/* Standard epilogue for variable definition and declaration in header file */
#if defined(__DEFINE_VARIABLES)
#define APP_DPM_H_DEFINITIONS
#undef __DEFINE_VARIABLES
#endif  /* __DEFINE_VARIABLES */

#endif /* __SILICON_LABS_APP_DPM_H__ */

