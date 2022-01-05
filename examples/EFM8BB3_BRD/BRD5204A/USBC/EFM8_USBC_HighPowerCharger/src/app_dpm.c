/**************************************************************************//**
 * Copyright 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

// -----------------------------------------------------------------------------
// Includes

#include <SI_EFM8BB3_Register_Enums.h>
#include <si_toolchain.h>

#define DBG_LEVEL DBG_VERBOSE

#include "pd.h"
#include "pd_config.h"
#include "smb_0.h"
#include "ncp81239_driver.h"

#define _HDRDEF_VARIABLES "app_dpm.h"
#include "externaldef.h"

// -----------------------------------------------------------------------------
// Constant Definitions

// -------------------------------
// Provider Related Constants
//
// Local: Source Capabilities (refer to PD Spec 6.4.1), pointer and count
// Remote: Sink Capabilities, pointer and count (for tracking port partner)
//
// The High-Power Charger is able to deliver 60W of power
// So it offers following power objects:
//
// 5V @ 3A (15W)
// 9V @ 3A (27W)
// 15V @ 3A (45W)
// 20V @ 3A (60W)
#define SRC_PDO_FIXED_SUPPLY_5V_3000MA                                \
          (SRC_PDO_FIXED_SUPPLY(SRC_PDO_FIXED_SUPPLY_PR_SWAP_NO,      \
                                SRC_PDO_FIXED_SUPPLY_USB_SUSPEND_NO,  \
                                SRC_PDO_FIXED_SUPPLY_EXT_POWER_NO,    \
                                SRC_PDO_FIXED_SUPPLY_USB_COMM_NO,     \
                                SRC_PDO_FIXED_SUPPLY_DR_SWAP_NO,      \
                                SRC_PDO_FIXED_SUPPLY_PEAK_CUR_IOC,    \
                                VOLTAGE_5000mV,                       \
                                3000))

#define SRC_PDO_FIXED_SUPPLY_9V_3000MA                                \
          (SRC_PDO_FIXED_SUPPLY(SRC_PDO_FIXED_SUPPLY_PR_SWAP_NO,      \
                                SRC_PDO_FIXED_SUPPLY_USB_SUSPEND_NO,  \
                                SRC_PDO_FIXED_SUPPLY_EXT_POWER_NO,    \
                                SRC_PDO_FIXED_SUPPLY_USB_COMM_NO,     \
                                SRC_PDO_FIXED_SUPPLY_DR_SWAP_NO,      \
                                SRC_PDO_FIXED_SUPPLY_PEAK_CUR_IOC,    \
                                VOLTAGE_9000mV,                       \
                                3000))

#define SRC_PDO_FIXED_SUPPLY_12V_3000MA                               \
          (SRC_PDO_FIXED_SUPPLY(SRC_PDO_FIXED_SUPPLY_PR_SWAP_NO,      \
                                SRC_PDO_FIXED_SUPPLY_USB_SUSPEND_NO,  \
                                SRC_PDO_FIXED_SUPPLY_EXT_POWER_NO,    \
                                SRC_PDO_FIXED_SUPPLY_USB_COMM_NO,     \
                                SRC_PDO_FIXED_SUPPLY_DR_SWAP_NO,      \
                                SRC_PDO_FIXED_SUPPLY_PEAK_CUR_IOC,    \
                                VOLTAGE_12000mV,                      \
                                3000))

#define SRC_PDO_FIXED_SUPPLY_15V_3000MA                               \
          (SRC_PDO_FIXED_SUPPLY(SRC_PDO_FIXED_SUPPLY_PR_SWAP_NO,      \
                                SRC_PDO_FIXED_SUPPLY_USB_SUSPEND_NO,  \
                                SRC_PDO_FIXED_SUPPLY_EXT_POWER_NO,    \
                                SRC_PDO_FIXED_SUPPLY_USB_COMM_NO,     \
                                SRC_PDO_FIXED_SUPPLY_DR_SWAP_NO,      \
                                SRC_PDO_FIXED_SUPPLY_PEAK_CUR_IOC,    \
                                VOLTAGE_15000mV,                      \
                                3000))

#define SRC_PDO_FIXED_SUPPLY_20V_3000MA                               \
          (SRC_PDO_FIXED_SUPPLY(SRC_PDO_FIXED_SUPPLY_PR_SWAP_NO,      \
                                SRC_PDO_FIXED_SUPPLY_USB_SUSPEND_NO,  \
                                SRC_PDO_FIXED_SUPPLY_EXT_POWER_NO,    \
                                SRC_PDO_FIXED_SUPPLY_USB_COMM_NO,     \
                                SRC_PDO_FIXED_SUPPLY_DR_SWAP_NO,      \
                                SRC_PDO_FIXED_SUPPLY_PEAK_CUR_IOC,    \
                                VOLTAGE_20000mV,                      \
                                3000))

SRC_PDO_t local_src_cap[MAX_NUM_DO];
SI_SEGMENT_POINTER(local_src_cap_ptr, SRC_PDO_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(num_local_src_cap, uint8_t, SI_SEG_XDATA);
SNK_PDO_t remote_snk_cap[MAX_NUM_DO];
SI_SEGMENT_POINTER(remote_snk_cap_ptr, SRC_PDO_t, SI_SEG_XDATA);
SI_SEGMENT_VARIABLE(num_remote_snk_cap, uint8_t, SI_SEG_XDATA);

const SI_SEGMENT_VARIABLE(TempSensorOffsetMv, uint16_t, SI_SEG_CODE) =
                          TEMP_SENSOR_OFFSET_MV;

const SI_SEGMENT_VARIABLE(TempSensorSlopeMvC, float, SI_SEG_CODE) =
                          TEMP_SENSOR_SLOPE_MV_C;

// -------------------------------
// Miscellaneous Constants

// Message buffer used to send messages to policy engine.
static Xknltcb_msg_t msg_to_PE;

// Message buffer used to send messages from library (call-back) to application
static Xknltcb_msg_t msg_src_changed;
static Xknltcb_msg_t msg_src_dr_swap;
static Xknltcb_msg_t msg_src_recover;
static Xknltcb_msg_t msg_vdm_resp;
static Xknltcb_msg_t msg_mode;
static Xknltcb_msg_t msg_vdm;
static Xknltcb_msg_t msg_mode_switch;
static Xknltcb_msg_t msg_adc;

// Message buffer used to send messages to System Policy Engine
static Xknltcb_msg_t msg_to_spm;

// Timers used to send messages to PE after a timeout
static Xknltmr_delay_t timer_src_changed;
static Xknltmr_delay_t timer_src_recover;
static Xknltmr_delay_t timer_adc;

// Data structure reflecting the present application configuration and status
static spm_SM_t sys_policy;

// VBus (mv) requested by Sink
static SI_SEGMENT_VARIABLE(requiredSrcVBus, uint16_t, SI_SEG_XDATA);
static SI_SEGMENT_VARIABLE(presentSrcVBus, uint16_t, SI_SEG_XDATA);

// Current (mA) requested by Sink
static SI_SEGMENT_VARIABLE(requiredSrcCurrent, uint16_t, SI_SEG_XDATA);

// ADC channel for measuring the input voltage to the regulator
SI_SEGMENT_VARIABLE(inputVoltageChannel, uint8_t, SI_SEG_XDATA);

// ADC channel for measuring the temperature
SI_SEGMENT_VARIABLE(tempSensorChannel, uint8_t, SI_SEG_XDATA);

// -----------------------------------------------------------------------------
// Function Prototypes

static void app_sendmsg2spm(uint8_t id, uint16_t d);
static void app_sendmsg2pe(uint8_t id, uint16_t d);
static void app_InitSystemPolicy(void);

// -----------------------------------------------------------------------------
// Functions

// -------------------------------
// Common Functions

/***************************************************************************//**
 * @brief
 *    Initializes application specific Device Policy Manager.
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when it initializes device policy manager.
 *
 * It initializes message buffers and timers.
 *****************************************************************************/
void app_dpm_Init(void)
{
  SI_SEGMENT_VARIABLE(i, uint8_t, SI_SEG_XDATA);

  // Setup Source variables
  local_src_cap[0].u32 = SRC_PDO_FIXED_SUPPLY_5V_3000MA;
  local_src_cap[1].u32 = SRC_PDO_FIXED_SUPPLY_9V_3000MA;
  local_src_cap[2].u32 = SRC_PDO_FIXED_SUPPLY_12V_3000MA;
  local_src_cap[3].u32 = SRC_PDO_FIXED_SUPPLY_15V_3000MA;
  local_src_cap[4].u32 = SRC_PDO_FIXED_SUPPLY_20V_3000MA;
  local_src_cap[5].u32 = 0L;
  local_src_cap[6].u32 = 0L;

  num_local_src_cap = 5;
  local_src_cap_ptr = &local_src_cap[0];

  presentSrcVBus = 0;
  requiredSrcVBus = 5000;
  requiredSrcCurrent = SRC_DEFAULT_CHARGING_CURRENT;

  for (i = 0; i < MAX_NUM_DO; i++)
  {
    remote_snk_cap[i].u32 = 0L;
  }
  num_remote_snk_cap = 0;
  remote_snk_cap_ptr = &remote_snk_cap[0];

  knlmsg_Init(&msg_to_PE);
  knlmsg_Init(&msg_src_changed);
  knlmsg_Init(&msg_src_dr_swap);
  knlmsg_Init(&msg_src_recover);
  knlmsg_Init(&msg_vdm_resp);
  knlmsg_Init(&msg_mode);
  knlmsg_Init(&msg_vdm);
  knlmsg_Init(&msg_mode_switch);
  knlmsg_Init(&msg_adc);

  knltmrst_Init(&timer_src_changed);
  knltmrst_Init(&timer_src_recover);
  knltmrst_Init(&timer_adc);

  app_InitSystemPolicy();
}

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
 * It sends message to High-Power Charger application-specific DPM and SPM to
 * indicate attachment.
 *****************************************************************************/
void app_Attached(void)
{
  UIF_LED0 = UIF_LED0_ON;
  dbgPrintf(DBG_INFO, ("APP: Attached\n"));

  // Let DPM and SPM know that something attached
  app_sendMessageWithId(MSG_APP_PORT_ATTACHED, 0, msg_to_PE);
  app_sendmsg2spm(get_msgid(MSG_SYS_PORT_ATTACHED), 0);
}

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
 * It sends message to High-Power Charger application-specific DPM and SPM to
 * indicate detachment. The application DPM will set VBus t0 vSafe0V and disable
 * it in order for sink to know the disconnection.
 *****************************************************************************/
void app_Detached(void)
{
  UIF_LED0 = UIF_LED0_OFF;
  UIF_LED1 = UIF_LED1_OFF;
  dbgPrintf(DBG_INFO, ("APP: Detached\n"));

  // Let DPM and SPM know that something detached
  app_sendMessageWithId(MSG_APP_PORT_DETACHED, 0, msg_to_PE);
  app_sendmsg2spm(get_msgid(MSG_SYS_PORT_DETACHED), 0);
}

/***************************************************************************//**
 * @brief
 *    Handles successful transmission of message to port partner
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when GoodCRC is received for last transmitted message or a HardReset is
 * generated.
 *****************************************************************************/
void app_SentMsgSuccessfully(void)
{
  dbgPrintf(DBG_INFO,
           ("APP: Unexpected call to function app_SentMsgSuccessfully\n"));
}

/***************************************************************************//**
 * @brief
 *    Handles completion of contract
 *
 * This is a call-back function required by USB PD library. Library calls it
 * once contract negotiation is completed (indicated by successful transmission
 * of PS Ready message)
 *****************************************************************************/
void app_ContractDone(void)
{
  dbgPrintf(DBG_INFO, ("APP: Contract Done\n"));
  dbgPrintf(DBG_INFO, ("------------------------------------\n"));

  // Let SPM know that Power contract with port partner has completed
  app_sendmsg2spm(get_msgid(MSG_SYS_CONTRACT_DONE), 0);
}

// -------------------------------
// Consumer-Related Functions
//
// High-Power Charger application has to provide these dummy functions for
// USBC PD Library.

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
void app_UpdateNewSrcCapability(PD_Packet_t *pkt)
{
  UNREFERENCED_ARGUMENT(pkt);
}

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
void app_EvaluateSrcCapability(uint32_t SI_SEG_XDATA *prdo)
{
  UNREFERENCED_ARGUMENT(prdo);
}

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
 * This is a call-back function required by USB PD library. Library calls it
 * with gotoMin set to 1 when GotoMin message is received from port partner. It
 * is called with gotoMin cleared to 0 when Accept message is received from port
 * partner. Sink power level is adjusted accordingly.
 *
 * See USB Power Delivery Specification, section 8.3.3.3.6 for more details.
 *****************************************************************************/
void app_TransitionToNewSinkPower(uint8_t gotoMin)
{
  UNREFERENCED_ARGUMENT(gotoMin);
}

/***************************************************************************//**
 * @brief
 *    Transition to sink default conditions.
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when a HardReset sent or received or BIST Carrier Mode 2 is timed-out.
 * Library assumes that appropriate actions (like switching VCONN OFF, Setting
 * Data Role to UFP etc), if required, are taken by this function.
 *
 * See USB Power Delivery Specification, section 8.3.3.3.9 for more details.
 *****************************************************************************/
void app_SinkTransitionToDefault()
{
  app_sendmsg2spm(get_msgid(MSG_SYS_HR_DONE), 0);
}

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
 * This is a call-back function required by USB PD library. Library calls it
 * when Get Sink Capabilities message is received from port partner. It copies
 * PDOs from local_snk_cap to the location pointed by prdo.
 *****************************************************************************/
uint8_t app_PresentSinkCapability(uint32_t SI_SEG_XDATA *prdo)
{
  UNREFERENCED_ARGUMENT(prdo);
  return 0;
}

// -------------------------------
// Provider-Related Functions

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
bool app_IsSourcePowerAavailable(void)
{
  dbgPrintf(DBG_INFO, ("\nAPP: Power Source available\n"));
  return true;
}

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
uint8_t app_PresentSrcCapability(uint32_t SI_SEG_XDATA *pdo)
{
  SI_SEGMENT_VARIABLE(i, uint8_t, SI_SEG_XDATA);

  dbgPrintf(DBG_INFO, ("APP: Tx SrcCap\n"));

  for (i = 0; i < num_local_src_cap; i++)
  {
    *pdo = local_src_cap_ptr->u32;
    pdo++; local_src_cap_ptr++;
  }

  local_src_cap_ptr = &local_src_cap[0];

  return num_local_src_cap;
}

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
uint8_t app_EvaluateSnkRequest(PD_Packet_t *pdo)
{
  SI_SEGMENT_VARIABLE(rdo, SI_UU32_t, SI_SEG_XDATA);
  SI_SEGMENT_VARIABLE(idx, uint8_t, SI_SEG_XDATA);
  SI_SEGMENT_VARIABLE(maxCurrent, uint16_t, SI_SEG_XDATA);
  SI_SEGMENT_VARIABLE(minCurrent, uint16_t, SI_SEG_XDATA);

  rdo.u32 = pdo->pkt.obj[0].u32;
  idx = RDO_FIXED_AND_VARIABLE_SUPPLY_OBJ_POS_GET(rdo);

  dbgPrintf(DBG_INFO, ("APP: Rx Request (Obj %d, Volt %d mV, Cur %d mA)\n",
            (uint16_t)idx,
            (uint16_t)SRC_PDO_FIXED_SUPPLY_VOLTAGE_MV(local_src_cap[idx-1]),
            (uint16_t)RDO_FIXED_AND_VARIABLE_OPR_CURRENT_MA(rdo)));

  if ((idx > num_local_src_cap) || (idx == 0))
  {
    dbgPrintf(DBG_INFO, ("APP: Wrong Object Position: PDO Obj %d\n",
                        (int)RDO_FIXED_AND_VARIABLE_SUPPLY_OBJ_POS_GET(rdo)));
    return REQ_CANNOT_BE_MET;
  }

  if (RDO_FIXED_AND_VARIABLE_SUPPLY_GIVEBACK(rdo))
  {
    dbgPrintf(DBG_INFO, ("APP: GiveBack Set\n"));
    sys_policy.giveback = 1;
	  minCurrent = RDO_FIXED_AND_VARIABLE_MIN_CURRENT_MA(rdo);
    if (minCurrent > RDO_FIXED_AND_VARIABLE_OPR_CURRENT_MA(rdo))
    {
      dbgPrintf(DBG_INFO, ("APP: Min Current > Operating Current\n"));
    }
  }
  else
  {
    dbgPrintf(DBG_INFO, ("APP: GiveBack Clear\n"));
    sys_policy.giveback = 0;
    maxCurrent = RDO_FIXED_AND_VARIABLE_MAX_CURRENT_MA(rdo);
    if (maxCurrent > SRC_PDO_FIXED_SUPPLY_MAX_CURRENT_MA(local_src_cap[idx-1]))
    {
      dbgPrintf(DBG_INFO,("APP: Required Max Current > Offered Max Current\n"));
    }
  }

  if (RDO_FIXED_AND_VARIABLE_SUPPLY_MISMATCH(rdo))
  {
	  dbgPrintf(DBG_INFO, ("APP: Cap Mismatch Set\n"));
	  sys_policy.capabilityMismatch = 1;
  }
  else
  {
    dbgPrintf(DBG_INFO, ("APP: Cap Mismatch Clear\n"));
    sys_policy.capabilityMismatch = 0;
  }

  if (RDO_FIXED_AND_VARIABLE_SUPPLY_USB_COMM(rdo))
  {
    dbgPrintf(DBG_INFO, ("APP: USB cap Set\n"));
  }

  if (RDO_FIXED_AND_VARIABLE_SUPPLY_USB_SUSPEND(rdo))
  {
	  dbgPrintf(DBG_INFO, ("APP: USB SUSP Set\n"));
  }

  if (RDO_FIXED_AND_VARIABLE_OPR_CURRENT_MA(rdo) <=
                      SRC_PDO_FIXED_SUPPLY_MAX_CURRENT_MA(local_src_cap[idx-1]))
  {
    presentSrcVBus = requiredSrcVBus;
    requiredSrcVBus = SRC_PDO_FIXED_SUPPLY_VOLTAGE_MV(local_src_cap[idx-1]);
    requiredSrcCurrent = RDO_FIXED_AND_VARIABLE_OPR_CURRENT_MA(rdo);
    return REQ_CAN_BE_MET;
  }
  else
  {
    dbgPrintf(DBG_INFO, ("APP: Required Current > Offered Current (%d, %d, %d)",
         (uint16_t)(idx-1),
         (uint16_t)RDO_FIXED_AND_VARIABLE_OPR_CURRENT_MA(rdo),
         (uint16_t)SRC_PDO_FIXED_SUPPLY_MAX_CURRENT_MA(local_src_cap[idx-1])));
  }

  return REQ_CANNOT_BE_MET;
}

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
bool app_NeedGotoMin(void)
{
  return false;
}

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
void app_PrepareNewSource(void)
{
  UIF_LED1 = UIF_LED1_ON;
  dbgPrintf(DBG_INFO, ("APP: New Power: Voltage %d mV, Current %d mA\n",
                       requiredSrcVBus, requiredSrcCurrent));

  app_sendMessageWithId(MSG_APP_PORT_NEW_SRC, 0, msg_to_PE);
}

/***************************************************************************//**
 * @brief
 *    Configures Power Supply and hardware to default settings
 *
 * This is a call-back function required by USB PD library. Library calls it
 * when HardReset is received from port partner or DPM. It performs following:
 * - Configures power supply to VSafe0V and start tSrcRecover timer for VSafe5V
 * - Resets local hardware
 * - Turns OFF VCONN
 *
 * Refer to PD Spec. 8.3.3.2.11
 *****************************************************************************/
void app_SrcTransitionToDefault(void)
{
  dbgPrintf(DBG_INFO, ("APP: Transition to default and Turn OFF VBus\n"));

  // Disable VBus
  UIF_LED1 = UIF_LED1_OFF;
  ncp81239_disable();

  // Note: It should discharge within 340ms such that tSrcRecover is 660ms or
  // more.
  timer_src_recover.pmsg = &msg_src_recover;
  timer_src_recover.pmsg->msg_id.u16 = MSG_APP_PORT_RECOVER;
  knltmr_SetDelayMS(&timer_src_recover, 1000);
  knltmr_SetDestTaskName(&timer_src_recover, PD_DPM_TSK);
  knltmr_AddTimer(&timer_src_recover);

  app_sendmsg2spm(get_msgid(MSG_SYS_HR_DONE), 0);
}

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
void app_UpdateNewSnkCapability(PD_Packet_t *pkt)
{
  SI_SEGMENT_VARIABLE(i, uint8_t, SI_SEG_XDATA);

  num_remote_snk_cap = 0;
  for (i = 0; i < MAX_NUM_DO; i++)
  {
    if (i < pkt->pkt.hdr.st.numObj)
    {
      remote_snk_cap[i].u32 = pkt->pkt.obj[i].u32;
      num_remote_snk_cap++;
    }
    else
    {
      remote_snk_cap[i].u32 = 0;
    }
  }
  app_sendmsg2spm(get_msgid(MSG_SYS_RX_SNK_CAP), 0);
}

// -------------------------------
// Role-Swap Functions

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
uint8_t app_UfpDfpDrSwapSupport(void)
{
  dbgPrintf(DBG_INFO, ("APP: Reject DR Swap\n"));
  return VDM_ANS_REJECT;
}

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
uint8_t app_DfpUfpDrSwapSupport(void)
{
  dbgPrintf(DBG_INFO, ("APP: Reject DR Swap\n"));
  return VDM_ANS_REJECT;
}

/***************************************************************************//**
 * @brief
 *    Determines if power role can be switched between SRC and SNK
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
uint8_t app_UfpDfpPrSwapSupport(void)
{
  dbgPrintf(DBG_INFO, ("APP: Reject PR Swap\n"));
  return VDM_ANS_REJECT;
}

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
uint8_t app_UfpDfpVconnSwapSupport(void)
{
  dbgPrintf(DBG_INFO, ("APP: Reject VCONN Swap\n"));
  return VDM_ANS_REJECT;
}

/***************************************************************************//**
 * @brief
 *    Data Role Swap from UFP to DFP
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during Data Role Swap with port partner. It changes port Data Role to DFP.
 *
 * Refer to PD Spec. 8.3.3.6.2.2.4
 *****************************************************************************/
void app_Change2Dfp(void)
{
  dbgPrintf(DBG_ERROR, ("ERROR: app_Change2Dfp() called\n"));
}

/***************************************************************************//**
 * @brief
 *    Data Role Swap from DFP to UFP
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during Data Role Swap with port partner. It changes port Data Role to UFP.
 *
 * Refer to PD Spec. 8.3.3.6.2.1.4
 *****************************************************************************/
void app_Change2Ufp(void)
{
  dbgPrintf(DBG_ERROR, ("ERROR: app_Change2Ufp() called\n"));
}

/***************************************************************************//**
 * @brief
 *    Turn OFF power sink
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during SNK to SRC Power Role Swap with port partner. It turns OFF power sink.
 *
 * Refer to PD Spec. 8.3.3.6.3.2.4
 *****************************************************************************/
void app_TurnOff_PowerSink(void)
{
  dbgPrintf(DBG_ERROR, ("ERROR: app_TurnOff_PowerSink() called\n"));
}

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
void app_TurnOn_Vsafe5V(void)
{
  dbgPrintf(DBG_ERROR, ("ERROR: app_TurnOn_Vsafe5V() called\n"));
}

/***************************************************************************//**
 * @brief
 *    Turn OFF Source power supply
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during SRC to SNK Power Role Swap with port partner. It sends message to SPM
 *
 * Refer to PD Spec. 8.3.3.6.3.1.4
 *****************************************************************************/
void app_TurnOff_PowerSource(void)
{
  // This should never be called because the application does not support
  // Power-Role Swap. However, we will still disable the Power Source capability
  // here as a safety measure.
  ncp81239_disable();

  dbgPrintf(DBG_ERROR, ("ERROR: app_TurnOff_PowerSource() called\n"));
}

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
void app_PDDisabled(void)
{
  dbgPrintf(DBG_INFO, ("APP: Turn on Default Power\n"));

  // Provide VSafe5V
  ncp81239_setOutputVoltage(5000);
  ncp81239_enable();
}

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
void app_DRSwapResponse(uint8_t resp)
{
  UNREFERENCED_ARGUMENT(resp);

  dbgPrintf(DBG_ERROR, ("ERROR: app_DRSwapResponse() called\n"));
}

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
void app_PRSwapResponse(uint8_t resp)
{
  UNREFERENCED_ARGUMENT(resp);

  dbgPrintf(DBG_ERROR, ("ERROR: app_PRSwapResponse() called\n"));
}

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
void app_VconnOn(void)
{
  dbgPrintf(DBG_ERROR, ("ERROR: app_VconnOn() called\n"));
}

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
void app_VconnOff(void)
{
  dbgPrintf(DBG_ERROR, ("ERROR: app_VconnOff() called\n"));
}

/***************************************************************************//**
 * @brief
 *    Determines if application is presently supplying VCONN
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during VCONN Swap with port partner to determine if application is presently
 * supplying VCONN.
 *****************************************************************************/
bool app_IsSupplyingVconn(void)
{
  return false;
}

/***************************************************************************//**
 * @brief
 *    Indicates the result of VCONN Swap
 *
 * @return
 *    0 if port partner rejected VCONN Swap request
 *    1 if port partner cannot do VCONN Swap at the moment
 *    2 if port partner did not responded to VCONN Swap request within a timeout
 *    3 if port partner accepted VCONN Swap request
 *
 * This is a call-back function required by USB PD library. Library calls it
 * during VCONN Swap with port partner to inform application about the result of
 * VCONN Swap.
 *****************************************************************************/
void app_VConnSwapDone(uint8_t result)
{
  UNREFERENCED_ARGUMENT(result);

  dbgPrintf(DBG_ERROR, ("ERROR: app_VConnSwapDone() called\n"));
}

// -------------------------------
// Alternate-Mode Functions
//
// Following functions are required by Silicon Labs USB PD Library but are not
// related to High-Power Charger Application. Hence implemented as empty
// functions.

uint8_t app_UfpVdmGetIdentity(uint32_t SI_SEG_XDATA *hdr,
                              uint32_t SI_SEG_XDATA *pdo)
{
  UNREFERENCED_ARGUMENT(hdr);
  UNREFERENCED_ARGUMENT(pdo);

  dbgPrintf(DBG_ERROR, ("App DPM: Call to app_UfpVdmGetIdentity()\n"));
  return 0;
}

uint8_t app_UfpVdmGetSVIDs(uint32_t SI_SEG_XDATA *hdr,
                           uint32_t SI_SEG_XDATA *pdo)
{
  UNREFERENCED_ARGUMENT(hdr);
  UNREFERENCED_ARGUMENT(pdo);

  dbgPrintf(DBG_ERROR, ("App DPM: Unexpected call to app_UfpVdmGetSVIDs()\n"));
  return 0;
}

uint8_t app_UfpVdmGetMODEs(uint16_t id, uint32_t SI_SEG_XDATA *pdo)
{
  UNREFERENCED_ARGUMENT(id);
  UNREFERENCED_ARGUMENT(pdo);

  dbgPrintf(DBG_ERROR, ("App DPM: Unexpected call to app_UfpVdmGetMODEs()\n"));
  return 0;
}

uint8_t app_UfpVdmEnterMode(uint32_t SI_SEG_XDATA *hdr,
                            uint32_t SI_SEG_XDATA *pdo)
{
  UNREFERENCED_ARGUMENT(hdr);
  UNREFERENCED_ARGUMENT(pdo);

  dbgPrintf(DBG_ERROR, ("App DPM: Unexpected call to app_UfpVdmEnterMode()\n"));
  return 0;
}

uint8_t app_UfpVdmExitMode(uint32_t SI_SEG_XDATA *hdr,
                           uint32_t SI_SEG_XDATA *pdo)
{
  UNREFERENCED_ARGUMENT(hdr);
  UNREFERENCED_ARGUMENT(pdo);

  dbgPrintf(DBG_ERROR, ("App DPM: Unexpected call to app_UfpVdmExitMode()\n"));
  return 0;
}

uint8_t app_HandleSVIDcmd(Packet_t *pkt, Packet_t *resp)
{
  UNREFERENCED_ARGUMENT(pkt);
  UNREFERENCED_ARGUMENT(resp);

  dbgPrintf(DBG_ERROR, ("App DPM: Unexpected call to app_HandleSVIDcmd()\n"));
  return 0;
}

void app_DfpVdmReceivedIdentityResp(PD_Packet_t *pkt)
{
  UNREFERENCED_ARGUMENT(pkt);
}

void app_DfpVdmReceivedSVIDsResp(PD_Packet_t *pkt)
{
  UNREFERENCED_ARGUMENT(pkt);
}

void app_DfpVdmReceivedMODEsResp(PD_Packet_t *pkt)
{
  UNREFERENCED_ARGUMENT(pkt);
}

void app_DfpVdmReceivedEnterResp(PD_Packet_t *pkt)
{
  UNREFERENCED_ARGUMENT(pkt);
}

void app_DfpVdmReceivedExitResp(PD_Packet_t *pkt)
{
  UNREFERENCED_ARGUMENT(pkt);
}

// -------------------------------
// Policy Manager Functions

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
 * This is a call-back function required by USB PD library. Library calls it
 * when DPM message ID is 0x8000 or greater. Library assumes that such DPM
 * messages are handled in application space.
 *
 * High-Power Charger Application DPM handles following messages:
 * MSG_APP_PORT_ATTACHED
 * MSG_APP_PORT_DETACHED
 * MSG_APP_PORT_RECOVER
 * MSG_APP_PORT_NEW_SRC
 *
 * It calls pd_policy_get_buffer() to get buffer pointer which is pointing to
 * _PD_Packet_t.pkt (refer to pd_prl.h for structure details).
 *****************************************************************************/
void app_msgHandler(Xknltcb_msg_t *pmsg)
{
  switch(pmsg->msg_id.u8[LSB])
  {
    case get_msgid(MSG_APP_PORT_ATTACHED):

      // Set VBus to VSafe5V and enable it in order for sink to know attachment
      ncp81239_init();
      ncp81239_enable();
      dbgPrintf(DBG_INFO, ("APP: SRC Defaults: Voltage 5V, Current 1.5 A\n"));
      break;

    case get_msgid(MSG_APP_PORT_DETACHED):
      // Set VBus to VSafe0V and disable it in order for sink to know detachment
      ncp81239_disable();

      // Set VBus back to 5V in case it is turned back on later without first
      // being configured.
      ncp81239_setOutputVoltage(5000);
      dbgPrintf(DBG_INFO, ("APP: SRC turned OFF VBus (0V)\n"));
      break;

    case get_msgid(MSG_APP_PORT_RECOVER):
      ncp81239_setOutputVoltage(5000);
      ncp81239_enable();
      dbgPrintf(DBG_INFO, ("APP: Recover to VSafe5V\n"));

      // Notify Policy Engine with MSG_DPM_POL_PWR_DEFAULT message
      app_sendmsg2pe(get_msgid(MSG_DPM_POL_PWR_DEFAULT), 0);
      break;

    case get_msgid(MSG_APP_PORT_NEW_SRC):
      ncp81239_setOutputVoltage(requiredSrcVBus);
      timer_src_changed.pmsg = &msg_src_changed;
      timer_src_changed.pmsg->msg_id.u16 = MSG_DPM_POL_SRC_TRANSITION_DONE;
      knltmr_SetDelayMS(&timer_src_changed, 250);
      knltmr_SetDestTaskName(&timer_src_changed, PD_POLICY_TSK);
      knltmr_AddTimer(&timer_src_changed);
      break;
  }
}

void app_SystemPolicy(Xknltcb_msg_t *pmsg)
{
  static bool temp_over_threshold = false;
#ifdef DEBUG
  SI_SEGMENT_VARIABLE(snk_idx, uint8_t, SI_SEG_XDATA);
#endif
  SI_SEGMENT_VARIABLE(input_voltage_mv, uint16_t, SI_SEG_XDATA);
  SI_SEGMENT_VARIABLE(vbus_voltage_mv, uint16_t, SI_SEG_XDATA);
  SI_SEGMENT_VARIABLE(temp_c, uint16_t, SI_SEG_XDATA);

  // Regardless of state, first check these messages (why?)
  switch (pmsg->msg_id.u8[LSB])
  {
    case get_msgid(MSG_SYS_PORT_DETACHED):
      sys_policy.state = SPM_Force_to_init;
      break;

    case get_msgid(MSG_SYS_HR_DONE):
      sys_policy.state = SPM_Src_Dfp_wait_for_contract;
      break;
  }

  // Check for ADC timer
  if (pmsg->msg_id.u8[LSB] == get_msgid(MSG_SYS_ADC_TIMEOUT))
  {
    temp_c = app_ConvertCodesToCelsius(phy_ReadUserADCmV(tempSensorChannel));
    input_voltage_mv = phy_ReadUserADCmV(inputVoltageChannel);
    input_voltage_mv += (uint16_t)(((uint32_t) VIN_SENSE_RES_HIGH_KOHM *
                        (uint32_t) input_voltage_mv) / VIN_SENSE_RES_LOW_KOHM);
    vbus_voltage_mv = pdhw_ReadVBUSmV();
    dbgPrintf(DBG_INFO, ("SPM:  Temp: %d C; Vin: %d mV; VBus: %d mV\n",
        temp_c,
                         input_voltage_mv,
                         vbus_voltage_mv));

    // If the temperature threshold was exceeded last time around, reset the
    // part here to disconnect.
    if (temp_over_threshold == true)
    {
      RSTSRC = RSTSRC_SWRSF__SET;
    }

    // If temperature is currently under the threshold, see if it has passed
    // over TEMPERATURE_OFF
    else if (temp_c > TEMPERATURE_C_OFF)
    {
      dbgPrintf(DBG_INFO, ("SPM:  Temperature %d C is above the limit %d C. ",
                           temp_c,
                           (uint16_t) TEMPERATURE_C_OFF));
      dbgPrintf(DBG_INFO, ("Stop sourcing current.\n"));
      temp_over_threshold = true;
    }

    // Start the next delay
    timer_adc.pmsg = &msg_adc;
    timer_adc.pmsg->msg_id.u16 = MSG_SYS_ADC_TIMEOUT;
    knltmr_SetDelayMS(&timer_adc, USER_ADC_POLL_RATE_MS);
    knltmr_SetDestTaskName(&timer_adc, PD_EXINF_TSK);
    knltmr_AddTimer(&timer_adc);
  }

  switch(sys_policy.state)
  {
    case SPM_Unattached:
      if (sys_policy.state != sys_policy.lastState)
      {
        sys_policy.lastState = sys_policy.state;
      }

      if (pmsg->msg_id.u8[LSB] == get_msgid(MSG_SYS_PORT_ATTACHED))
      {
        dbgPrintf(DBG_INFO, ("SPM: Attached as DFP\n"));
        sys_policy.state = SPM_Src_Dfp_wait_for_contract;
        app_sendmsg2spm(get_msgid(MSG_SYS_DUMMY), 0);

        // Enable monitoring of input voltage and temperature sensor
        timer_adc.pmsg = &msg_adc;
        timer_adc.pmsg->msg_id.u16 = MSG_SYS_ADC_TIMEOUT;
        knltmr_SetDelayMS(&timer_adc, USER_ADC_POLL_RATE_MS);
        knltmr_SetDestTaskName(&timer_adc, PD_EXINF_TSK);
        knltmr_AddTimer(&timer_adc);
      }
      break;

    case SPM_Src_Dfp_wait_for_contract:
      if (sys_policy.state != sys_policy.lastState)
      {
        sys_policy.lastState = sys_policy.state;
      }

      if (pmsg->msg_id.u8[LSB] == get_msgid(MSG_SYS_CONTRACT_DONE))
      {
        sys_policy.state = SPM_Src_Dfp_Ready;
        app_sendmsg2spm(get_msgid(MSG_SYS_DUMMY), 0);
      }
      else if (pmsg->msg_id.u8[LSB] == get_msgid(MSG_SYS_CONTRACT_FAILED))
      {
        sys_policy.state = SPM_Src_Dfp_contract_failed;
        app_sendmsg2spm(get_msgid(MSG_SYS_DUMMY), 0);
      }
      break;

    case SPM_Src_Dfp_Ready:
      if (sys_policy.state != sys_policy.lastState)
      {
        if (sys_policy.lastState == SPM_Src_Dfp_wait_for_contract)
        {
          sys_policy.lastState = SPM_Src_Dfp_Ready;
          dbgPrintf(DBG_INFO, ("SPM: Ready as DFP\n"));
          dbgPrintf(DBG_INFO, ("------------------------------------\n"));

          if (sys_policy.capabilityMismatch)
          {
            app_sendmsg2pe(get_msgid(MSG_DPM_POL_GET_SNK_CAP), 0);
            dbgPrintf(DBG_INFO, ("SPM: Tx Get SNK Capabilities\n"));
            sys_policy.state = SPM_Src_Dfp_wait_for_SNK_Cap;
            app_sendmsg2spm(get_msgid(MSG_SYS_DUMMY), 0);
          }
        }
        else
        {
          sys_policy.lastState = sys_policy.state;
        }
      }
      break;

    case SPM_Src_Dfp_contract_failed:
      if (sys_policy.state != sys_policy.lastState)
      {
        sys_policy.lastState = sys_policy.state;
        dbgPrintf(DBG_INFO, ("SPM: Contract Failed\n"));
        dbgPrintf(DBG_INFO, ("------------------------------------\n"));
      }
      break;

    case SPM_Src_Dfp_wait_for_SNK_Cap:
      if (pmsg->msg_id.u8[LSB] == get_msgid(MSG_SYS_RX_SNK_CAP))
      {
        dbgPrintf(DBG_INFO, ("SPM: Rx SNK Capabilities (%d PDO's)\n",
                            (uint16_t) num_remote_snk_cap));

#ifdef DEBUG
        for (snk_idx = 0; snk_idx < num_remote_snk_cap; snk_idx++)
        {
          // We can only show the first three PDO's due to dbgPrintf limitations
          if (snk_idx < 3)
          {
            dbgPrintf(DBG_INFO,
              ("SPM: SNK PDO %d:\n  DRP: %d\n  V: %d mV\n  C: %d mA\n\n",
              (uint16_t) snk_idx,
              (uint16_t)SRC_PDO_FIXED_SUPPLY_PR_SWAP((remote_snk_cap[snk_idx])),
              (uint16_t)SRC_PDO_FIXED_SUPPLY_VOLTAGE_MV((remote_snk_cap[snk_idx])),
              (uint16_t)SRC_PDO_FIXED_SUPPLY_MAX_CURRENT_MA((remote_snk_cap[snk_idx]))));
          }
        }
#endif
        dbgPrintf(DBG_INFO, ("\n"));

        sys_policy.state = sys_policy.lastState;
        app_sendmsg2spm(get_msgid(MSG_SYS_DUMMY), 0);
      }
      break;

    case SPM_Snk_Ufp_wait_for_contract:
      if (sys_policy.state != sys_policy.lastState)
      {
        sys_policy.lastState = sys_policy.state;
      }

      if (pmsg->msg_id.u8[LSB] == get_msgid(MSG_SYS_CONTRACT_DONE))
      {
        sys_policy.state = SPM_Snk_Ufp_Ready;
        app_sendmsg2spm(get_msgid(MSG_SYS_DUMMY), 0);
      }
      break;

    case SPM_Snk_Ufp_Ready:
      if (sys_policy.state != sys_policy.lastState)
      {
        sys_policy.lastState = sys_policy.state;
      }
      break;

    case SPM_Force_to_init:
      sys_policy.lastState = sys_policy.state;
      // start again
      sys_policy.state = SPM_Unattached;
      app_sendmsg2spm(get_msgid(MSG_SYS_DUMMY), 0);
      knltmr_DelTimer(&timer_adc);
      temp_over_threshold = false;
      break;

    default:
      break;
  }
}

// -------------------------------
// Utility Functions

uint16_t app_ConvertCodesToCelsius(uint16_t adc_codes)
{
  return (adc_codes - (TempSensorOffsetMv - (TempSensorSlopeMvC * 0.5))) /
          TempSensorSlopeMvC;
}

// -------------------------------
// Local Functions

/***************************************************************************//**
 * @brief
 *    Sends a message to the System Policy Manager.
 *
 * @param id
 *    Id of the message to send
 *
 * @param d
 *    Data to send in the message
 *
 * @return
 *    None
 *****************************************************************************/
static void app_sendmsg2spm(uint8_t id, uint16_t d)
{
  msg_to_spm.msg_id.u16 = id + 0x9000;
  msg_to_spm.msgdata.u16 = d;
  knltcb_SendMsg(PD_EXINF_TSK, &msg_to_spm);
}

/***************************************************************************//**
 * @brief
 *    Sends a message to the Policy Engine.
 *
 * @param id
 *    Id of the message to send
 *
 * @param d
 *    Data to send in the message
 *
 * @return
 *    None
 *****************************************************************************/
static void app_sendmsg2pe(uint8_t id, uint16_t d)
{
  msg_to_PE.msg_id.u16 = id + ((uint16_t)MSGGRP_POLICY << 8);
  msg_to_PE.msgdata.u16 = d;
  knltcb_SendMsg(PD_POLICY_TSK, &msg_to_PE);
}

/***************************************************************************//**
 * @brief
 *    Initializes the system policy state machine.
 *
 * @return
 *    None
 *****************************************************************************/
static void app_InitSystemPolicy(void)
{
  sys_policy.state = SPM_Unattached;
  sys_policy.lastState = SPM_last_state;
  sys_policy.giveback = 0;
  sys_policy.capabilityMismatch = 0;

  knlmsg_Init(&msg_to_spm);
}
