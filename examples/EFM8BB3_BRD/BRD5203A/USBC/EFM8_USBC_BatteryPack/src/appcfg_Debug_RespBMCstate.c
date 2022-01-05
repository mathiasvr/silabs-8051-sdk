#include <SI_EFM8BB3_Register_Enums.h>                  // SFR declarations
#include <si_toolchain.h>                 // SFR declarations

#define DBG_LEVEL DBG_INFO
#include "pd.h"

#ifdef DEBUG

// For debugging - override the appcfg_Debug_RespBMCstate
void appcfg_Debug_RespBMCstate(bmc_state_t newbmcstate)
{
  UNREFERENCED_ARGUMENT(newbmcstate);
}

#endif
