#include <SI_EFM8BB3_Register_Enums.h>                  // SFR declarations
#include <si_toolchain.h>                 // SFR declarations

#include "pd.h"

#ifdef DEBUG
void appcfg_Debug_RespConnstate(conn_State_t newconnstate)
{
  UNREFERENCED_ARGUMENT(newconnstate);
}
#endif
