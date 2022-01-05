/**************************************************************************//**
 * Copyright 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <SI_EFM8BB3_Register_Enums.h>
#include <si_toolchain.h>

#define DBG_LEVEL DBG_INFO
#include "pd.h"

#ifdef DEBUG

// For debugging - override the appcfg_Debug_RespBMCstate
void appcfg_Debug_RespBMCstate(bmc_state_t newbmcstate)
{
  UNREFERENCED_ARGUMENT(newbmcstate);
}

#endif
