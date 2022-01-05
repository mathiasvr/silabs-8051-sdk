/**************************************************************************//**
 * Copyright 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <SI_EFM8BB3_Register_Enums.h>
#include <si_toolchain.h>

#include "pd.h"

#ifdef DEBUG
void appcfg_Debug_RespConnstate(conn_State_t newconnstate)
{
  UNREFERENCED_ARGUMENT(newconnstate);
}
#endif
