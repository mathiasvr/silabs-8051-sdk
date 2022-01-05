/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

//-----------------------------------------------------------------------------
// battery_pack_main.c
//-----------------------------------------------------------------------------
//
// Implement a USBC Battery Pack.
//
// The battery pack can act as a Source or Sink. To toggle between Source and
// Sink modes, press the switch labelled "MODE" on the board. The current power
// role is indicated by the LED's:
//
// - When LED0 is on, the device is a Source
// - When LED1 is on, the device is a Sink
//
// Role of this PD
// - Power Role Swap: No
// - Power Role: Provider Only
// - Power Role: Source Only
// - Data Role Swap: No
// - Data Role: DFP/UFP
// - VCONN Swap: No
//
// For use with the Battery Pack Demo Board (BRD5203A).
//-----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Includes

#include <SI_EFM8BB3_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
// $[Generated Includes]
// [Generated Includes]$

#define DBG_LEVEL DBG_VERBOSE

#include "pd.h"
#include "pd_config.h"
#include "app_dpm.h"

// -----------------------------------------------------------------------------
// Variables
static bool mode_sw_pressed = false;
extern SI_SEGMENT_VARIABLE(batteryVoltageChannel, uint8_t, SI_SEG_XDATA);
extern SI_SEGMENT_VARIABLE(tempSensorChannel, uint8_t, SI_SEG_XDATA);

// -----------------------------------------------------------------------------
// Functions

/**************************************************************************//**
 * Main Function
 *
 * Main initializes the device then continuously calls knltcb_RunSystem() to
 * keep the USBC Kernel running.
 *
 *****************************************************************************/
int main(void)
{
  uint8_t  count1;
  uint16_t count2;

  // Call hardware initialization routine
  enter_DefaultMode_from_RESET();

  // Determine if device is a Source or a Sink
  app_DetermineModeAtReset();

  pd_Init();

  // Add User ADC channels for the battery voltage and temperature sensor
  ADC0CN0_TEMPE = 1;
  batteryVoltageChannel = phy_AddUserADC(ADC0MX_ADC0MX__ADC0P18);
  tempSensorChannel = phy_AddUserADC(ADC0MX_ADC0MX__TEMP);

  USBC_VBUS_DISCHARGE = USBC_VBUS_DISCHARGE_ENABLE;

  count1 = 50;
  while (count1)
  {
    count2 = 10000;
    while (count2)
    {
      count2--;
    }
    count1--;
  }

  USBC_VBUS_DISCHARGE = USBC_VBUS_DISCHARGE_DISABLE;

  dbgPrintf(DBG_ERROR, ("Battery Pack Starting as "));
  dbgPrintf(DBG_ERROR, ((pol_is_source_role()) ? "Source\n" : "Sink\n"));
  
  while (1)
  {
// $[Generated Run-time code]
// [Generated Run-time code]$
    knltcb_RunSystem();

    if (mode_sw_pressed)
    {
      mode_sw_pressed = false;
      app_ModeSwitch();
    }
  }
}

/**************************************************************************//**
 * Port Match ISR
 *
 * Port Match is used to detect presses of the MODE_SW button. This toggles the
 * battery pack between Sink and Source mode.
 *
 *****************************************************************************/
SI_INTERRUPT(PMATCH_ISR, PMATCH_IRQn)
{
  if (P1MAT & 0x08) // MODE_SW is low
  {
    P1MAT &= ~0x08;
  }
  else              // MODE_SW is high
  {
    P1MAT |= 0x08;
    mode_sw_pressed = true;
  }
}
