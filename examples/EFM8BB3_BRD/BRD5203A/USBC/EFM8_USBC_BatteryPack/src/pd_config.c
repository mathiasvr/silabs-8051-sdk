#include <SI_EFM8BB3_Register_Enums.h>                  // SFR declarations

/*****
 * This file belongs in user application.  Users must not modify this file.  This file
 * is configured via #define macro settings
 *
 * User-modifiable configuration code are in pd_config_usermodify.c
 *
 */
#include "pd.h"
#include "pd_config.h"

/***************************************************************************
 * PD_CONFIG_USER_VERSION - This number should match the value evaluated
 * by the arithmetic operation in PD_CONFIG_SYSTEM_VERSION in pd.h.
 *
 * THE FOLLOWING LINE MUST NOT BE USED:
 *
 * #define PD_CONFIG_USER_VERSION   MAGIC_VERSION(PD_CONFIG_SYSTEM_VERSION)
 *
 * to workaround user responsibility to use the correct version of
 * pd_config.c.
 *
 * #define PD_CONFIG_USER_VERSION   MAGIC_VERSION(10)
 *
 ***************************************************************************/
#define PD_CONFIG_USER_VERSION  MAGIC_VERSION(10)
#if BADUSAGE_VER(PD_CONFIG)
#error "You cannot put PD_CONFIG_SYSTEM_VERSION inside MAGIC_VERSION()"
#elif MISMATCH_VER(PD_CONFIG)
#error "Mismatched version of pd_config_t structure between pd_config.c and pd.h."
#error "Please use the correct version of pd_config.c to build the application."
#error "Refer to pd_config.h in ufp_dongle project for the correct version"
#endif


#ifdef DEBUG
bmc_state_t old_bmcstate = stbmc_disabled;
conn_State_t old_connstate = conn_Disabled;
#endif

#define HWCFG_NUM_CC    2                          // Number of CC pins
#define HWCFG_CC2_ADCMX   (HWCFG_CC1_ADCMX+1)      // CC2 ADC MX always immediately after CC1
#define HWCFG_CC2_CP0MX   (HWCFG_CC1_CP0MX+1)      // CC2 CP0 MX always immediately after CC1


/*** Start of error checking of application tuning options in pd_config.h ***/
#if (PD_APP_DR != PD_APP_DR_UFP) && (PD_FEATURE_CAN_FLOAT_RD == PD_APP_CAN_FLOAT_RD_DISABLED)
#error "Application and hardware MUST be able to float Rd termination resistors to support DRP or DFP roles"
#endif

#if PD_NOMINAL_BMCRX_BITRATE_KBPS < 270
#warning "PD_NOMINAL_BMCRX_BITRATE_KBPS must be greater than 270"
#warning "  otherwise, the application may violate USB Type-C PD Specs."
#elif PD_NOMINAL_BMCRX_BITRATE_KBPS > 330
#warning "PD_NOMINAL_BMCRX_BITRATE_KBPS must be less than 330"
#warning "  otherwise, the application may violate USB Type-C PD Specs."
#endif

#if PD_BMCRX_TIMEOUT_PT1BITPERIOD < 10
#error "PD_BMCRX_TIMEOUT_PT1BITPERIOD cannot be less than 10"
#error " - application will not be able to receive any packet"
#elif PD_BMCRX_TIMEOUT_PT1BITPERIOD < 11
#warning "PD_BMCRX_TIMEOUT_PT1BITPERIOD should not be less than 11"
#warning " - application may miss reception of many packets"
#endif

#if (TBISTCONTMODE_MS < 30) || (TBISTCONTMODE_MS > 60)
#error "TBISTCONTMODE_MS must be between 30 and 60 ms"
#error " - violation of USB Type-C PD Specs, Table 6-30 Time Values"
#endif

#if (TERRORRECOVERY_MS < 25)
#error "TERRORRECOVERY_MS must be more than 25 ms"
#error " - violation of USB Type-C Specs, Table 4-20 CC Timing"
#endif

#if (TPDDEBOUNCE_MS < 10) || (TPDDEBOUNCE_MS > 20)
#error "TPDDEBOUNCE_MS must be between 10 and 20 ms"
#error " - violation of USB Type-C Specs, Table 4-20 CC Timing"
#endif

#if (TCCDEBOUNCE_MS < 100) || (TCCDEBOUNCE_MS > 200)
#error "TCCDEBOUNCE_MS must be between 100 and 200 ms"
#error " - violation of USB Type-C Specs, Table 4-20 CC Timing"
#endif

#if (TRECEIVE_US < 900) || (TRECEIVE_US > 1100)
#error "TRECEIVE_US must be between 900 and 1100 us"
#error " - violation of USB Type-C PD Specs, Table 6-30 Time Values"
#endif

#if (THARDRESETCOMPLETE_US < 4000) || (THARDRESETCOMPLETE_US > 5000)
#error "THARDRESETCOMPLETE_US must be between 4000 and 5000 us"
#error " - violation of USB Type-C PD Specs, Table 6-30 Time Values"
#endif

#if (TNORESPONSE_MS < 4500) || (TNORESPONSE_MS > 5500)
#error "TNORESPONSE_MS must be between 4500 and 5500 ms"
#error " - violation of USB Type-C PD Specs, Table 6-30 Time Values"
#endif

#if (TSINKREQUEST_MS < 100)
#error "TSINKREQUEST_MS must be greater than 100 ms"
#error " - violation of USB Type-C PD Specs, Table 6-30 Time Values"
#endif

#if (TTYPECSINKWAITCAP_MS < 310) || (TTYPECSINKWAITCAP_MS > 620)
#error "TTYPECSINKWAITCAP_MS must be between 310 and 620 ms"
#error " - violation of USB Type-C PD Specs, Table 6-30 Time Values"
#endif

#if (TPSTRANSITION_MS < 450) || (TPSTRANSITION_MS > 500)
#error "TPSTRANSITION_MS must be between 450 and 500 ms"
#error " - violation of USB Type-C PD Specs, Table 6-30 Time Values"
#endif

#if (TSENDERRESPONSE_MS < 24) || (TSENDERRESPONSE_MS > 30)
#error "TSENDERRESPONSE_MS must be between 24 and 30 ms"
#error " - violation of USB Type-C PD Specs, Table 6-30 Time Values"
#endif

/*** End of error checking of application tuning options in pd_config.h ***/


// Start of DERIVED values from pd_config.h
/** @cond HIDE_DOC */

// The formula below is to compute the BMC RX buffer timeout for 1 bit period in
// units of T[sysclk] * 32:
// = roundup( (1/PD_NOMINAL_BMCRX_BITRATE_KBPS) / (32 * T[sysclk_khz]) )
// = roundup( SYSCLK_KHZ / (32 * PD_NOMINAL_BMCRX_BITRATE_KBPS) )
// This number is meant to be assigned to SPI0CKR - see Reference Manual
//   of EFM8BB3 for more details.
#define BMCRX_BUF_TIMEOUT   (uint8_t)ROUNDUPDIV(SYSCLK_KHZ, 32 * PD_NOMINAL_BMCRX_BITRATE_KBPS)

// Helper macro to convert PD_BMCRX_TIMEOUT_PT1BITPERIOD to fill in
// neg_bmc_rxtimeout_ticks.
// Perform rounding up to nearest integer
#define BMCRX_TIMEOUT_TICKS   (uint16_t)ROUNDUPDIV((uint32_t)SYSCLK_KHZ   \
                                  * PD_BMCRX_TIMEOUT_PT1BITPERIOD,        \
                                  12UL * PD_NOMINAL_BMCRX_BITRATE_KBPS * 10)
/** @endcond */


pd_config_t SI_SEG_CODE pd_config_system =
{
 PD_APP_INTERFACE_TYPE,
 PD_APP_PWRCAP,
 PD_APP_DR,
 PD_APP_PORT,

 PD_APP_VCONN_SWAP,
 PD_APP_CAN_FLOAT_RD_ENABLED,
 HWCFG_DEBUG_VIEW_RXMOSI,
 HWCFG_DEBUG_VIEW_SPISCK,
 HWCFG_DEBUG_VIEW_EOTX,
 HWCFG_DEBUG_VIEW_DBGBMC,
 0,     // spare - unused
 0,     // spare - unused

 HWCFG_NUM_CC,        /* number of CC pins */
 HWCFG_CC1_ADCMX,     /* CC1 ADC0MX */
 HWCFG_CC1_CP0MX,     /* CC1 CP0MX */
 HWCFG_VBUS_ADCMX,    /* VBUS measurement ADC0MX */
 HWCFG_ATTACHDETACH_CP1MX,    /* Attach/detach CP1MX */

 HWCFG_ATTACHDETACH_MILLIVOLTS,
           /* Attach detach voltage level in mV, after voltage divider */

 VBUS_ADC_MULTIPLIER(HWCFG_RVBUS, HWCFG_RGND),

 -((uint16_t)US_TO_SYSCLKDIV12_TICKS(PD_INTERFRAMEGAP_US)),
       /* This specifies the tInterFrameGap as referenced in the
          USB Type-C PD Specs, Table 5-14.
          The specs calls for a minimum of 25 us
          */

 -(BMCRX_TIMEOUT_TICKS),
       /* This specifies the negative timeout in units of SYSCLK/12 ticks
          that is used by the firmware to determine that the
          BMC RX has stopped activity
          */

 BMCRX_BUF_TIMEOUT,
       /* This specifies the BMC RX buffer timeout
        */

 TBISTCONTMODE_MS * KNLTMR_TICKS_PER_MS,
                   /* Specifies tBISTContMode timeout */
 TERRORRECOVERY_MS * KNLTMR_TICKS_PER_MS,
                   /* Specifies tErrorRecovery timeout */
 TPDDEBOUNCE_MS,   /* Specifies tPDDebounce timeout */
 TCCDEBOUNCE_MS,   /* Specifies tCCDebounce timeout */

 ROUNDDIV(TRECEIVE_US, KNLTMR_US_PER_TICK),   /* Specifies tReceive timeout */

 ROUNDDIV(THARDRESETCOMPLETE_US, KNLTMR_US_PER_TICK),
               /* Specifies
                  tHardResetComplete timeout */

 TNORESPONSE_MS * KNLTMR_TICKS_PER_MS,
               /* Specifies tNoResponse timeout */

 TSINKREQUEST_MS * KNLTMR_TICKS_PER_MS,
               /* Specifies tSinkRequest timeout */

 TTYPECSINKWAITCAP_MS * KNLTMR_TICKS_PER_MS,
               /* Specifies tTypeCSinkWaitCap timeout */

 TPSTRANSITION_MS * KNLTMR_TICKS_PER_MS,
               /* Specifies tPSTransition timeout */

 TSENDERRESPONSE_MS * KNLTMR_TICKS_PER_MS,
               /* Specifies tSenderResponse timeout */
 APP_SRCPWR_CAP,
 TDRP_MS,			// ms Unit.
 ((TDRP_MS * 30) / 100) * KNLTMR_TICKS_PER_MS,		// tick unit
 TSNKDETACHDEBOUNCE_MS,
 TDISCOVERIDENTITY_MS * KNLTMR_TICKS_PER_MS,
 TPSHARDRESET_MS * KNLTMR_TICKS_PER_MS,
 TPSSOURCEOFF_MS * KNLTMR_TICKS_PER_MS,
 TSOURCEON_MS * KNLTMR_TICKS_PER_MS,
 TSINKACTIVITY_MS * KNLTMR_TICKS_PER_MS,
 TSOURCEACTIVITY_MS * KNLTMR_TICKS_PER_MS,
 TSOURCECAPABILITY_MS * KNLTMR_TICKS_PER_MS,
 TSWAPRECOVER_MS * KNLTMR_TICKS_PER_MS,
 TSWAPSOURCESTART_MS * KNLTMR_TICKS_PER_MS,
 TVCONNON_MS * KNLTMR_TICKS_PER_MS,
 TVDMMODEENTRY * KNLTMR_TICKS_PER_MS,
 TVDDMODEEXIT * KNLTMR_TICKS_PER_MS,
 TVDMRESPONSE * KNLTMR_TICKS_PER_MS,
 TSRCTRANSITION * KNLTMR_TICKS_PER_MS,
};

/*
 * Get the first ADC0MX value to start sensing orientation
 */
uint8_t appcfg_Get_StartCC_ADCMX()
{
  return HWCFG_CC1_ADCMX;
}

/*
 * Get the first CP0MX value to start sensing orientation
 */
uint8_t appcfg_Get_StartCC_CP0MX()
{
  return HWCFG_CC1_CP0MX;
}


bit appcfg_Disable_CC()
{
  USBC_CC1_RD_EN = USBC_CC1_RD_DISABLE;
  USBC_CC2_RD_EN = USBC_CC2_RD_DISABLE;
  USBC_CC1_RP_EN = USBC_CC1_RP_DISABLE;
  USBC_CC2_RP_EN = USBC_CC2_RP_DISABLE;
  return true;
}

void appcfg_Enable_CC_Rd()
{
  USBC_CC1_RP_EN = USBC_CC1_RP_DISABLE;
  USBC_CC2_RP_EN = USBC_CC2_RP_DISABLE;
  USBC_CC1_RD_EN = USBC_CC1_RD_ENABLE;
  USBC_CC2_RD_EN = USBC_CC2_RD_ENABLE;
}

void appcfg_Enable_CC_Rp()
{
  USBC_CC1_RD_EN = USBC_CC1_RD_DISABLE;
  USBC_CC2_RD_EN = USBC_CC2_RD_DISABLE;
  USBC_CC1_RP_EN = USBC_CC1_RP_ENABLE;
  USBC_CC2_RP_EN = USBC_CC2_RP_ENABLE;
}

void appcfg_Enable_CC_Init()
{
  // Initial state Provider - present Rp
  if (pol_is_source_role())
  {
    USBC_CC1_RD_EN = USBC_CC1_RD_DISABLE;
    USBC_CC2_RD_EN = USBC_CC2_RD_DISABLE;
    USBC_CC1_RP_EN = USBC_CC1_RP_ENABLE;
    USBC_CC2_RP_EN = USBC_CC2_RP_ENABLE;
  }
  // Initial state Consumer - present Rd
  else
  {
    USBC_CC1_RD_EN = USBC_CC1_RD_ENABLE;
    USBC_CC2_RD_EN = USBC_CC2_RD_ENABLE;
    USBC_CC1_RP_EN = USBC_CC1_RP_ENABLE;
    USBC_CC2_RP_EN = USBC_CC2_RP_ENABLE;
  }
}
