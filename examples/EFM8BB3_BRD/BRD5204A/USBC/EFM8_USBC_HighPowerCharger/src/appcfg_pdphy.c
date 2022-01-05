/**************************************************************************//**
 * Copyright 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

/*
 * User configurable function that can be overridden by users based on the type of product
 * that is being delivered.
 *
 * Source code must not be modified.  Configuration is based on pd_config.h
 */
#include <SI_EFM8BB3_Register_Enums.h>

#include "pd.h"
#include "pd_config.h"

/***************************************************************************
 * APPCFG_PDPHY_USER_VERSION - This number should match the value evaluated
 * by the arithmetic operation in APPCFG_PDPHY_SYSTEM_VERSION in pd.h.
 *
 * THE FOLLOWING LINE MUST NOT BE USED:
 *
 * #define APPCFG_PDPHY_USER_VERSION   MAGIC_VERSION(APPCFG_PDPHY_SYSTEM_VERSION)
 *
 * to workaround user responsibility to use the correct version of
 * appcfg_pdphy.c.
 *
 * #define APPCFG_PDPHY_USER_VERSION   MAGIC_VERSION(3)
 *
 ***************************************************************************/
#define APPCFG_PDPHY_USER_VERSION  MAGIC_VERSION(3)
#if BADUSAGE_VER(APPCFG_PDPHY)
#error "You cannot put APPCFG_PDPHY_SYSTEM_VERSION inside MAGIC_VERSION()"
#elif MISMATCH_VER(APPCFG_PDPHY)
#error "Mismatched version of appcfg_pdphy.c and pd.h"
#error "Please use the correct version of appcfg_pdphy.c to build the application."
#error "Refer to appcfg_pdphy.c in ufp_dongle project for the correct version"
#endif


KNLTCB_MSGHANDLER(PDPHY, pmsg)
{
  uint16_t lastmsg_id;
#ifdef DEBUG
  static bmc_state_t SI_SEG_XDATA old_bmcstate = stbmc_disabled;
  static conn_State_t SI_SEG_XDATA old_connstate = stbmc_disabled;
#endif


  do {
    lastmsg_id = pmsg->msg_id.u16;
    switch (pmsg->msg_id.u8[MSB])
    {
      case MSGGRP_PHY:
        phy_msghandler(pmsg);
        break;
      case MSGGRP_CONN:
        // in case of ErrorRecovery, back to Sink
        conn_SRC_msghandler(pmsg);
        break;
      case MSGGRP_PDHW:
      	pdhw_SRC_msghandler(pmsg);
        break;
      case MSGGRP_POWER:
        power_msghandler(pmsg);
        break;
    }
  } while (pmsg->msg_id.u16 != lastmsg_id);
  // Let the hardware process the message first

#ifdef DEBUG
  if (old_bmcstate != stbmc_state)
  {
    old_bmcstate = stbmc_state;
    appcfg_Debug_RespBMCstate(stbmc_state);
  }
  if (old_connstate != conn_sm_state)
  {
    old_connstate = conn_sm_state;
    appcfg_Debug_RespConnstate(conn_sm_state);
  }
#endif
}
