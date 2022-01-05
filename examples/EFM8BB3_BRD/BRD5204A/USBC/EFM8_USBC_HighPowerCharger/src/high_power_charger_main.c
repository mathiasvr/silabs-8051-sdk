/**************************************************************************//**
 * Copyright 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

//-----------------------------------------------------------------------------
// high_power_charger_main.c
//-----------------------------------------------------------------------------
//
// USB Type-C High-Power Charger
//
// The high-power charger is a USBType-C Source that can provide up to 60W to
// the connected Sink.
//
// Role of this PD
// - Power Role Swap: No
// - Power Role: Source Only
// - Data Role Swap: No
// - Data Role: DFP
// - VCONN Swap: No
//
// For use with the USB Type-C 60 W Charger Board (BRD5204A).
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
#include "ncp81239_driver.h"

// -----------------------------------------------------------------------------
// Variables

extern SI_SEGMENT_VARIABLE(inputVoltageChannel, uint8_t, SI_SEG_XDATA);
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
  SI_SEGMENT_VARIABLE(temperature_c, uint16_t, SI_SEG_XDATA);
  SI_SEGMENT_VARIABLE(i, uint16_t, SI_SEG_XDATA);

  // Call hardware initialization routine
  enter_DefaultMode_from_RESET();

  pd_Init();

  // Add User ADC channels for the input voltage and temperature sensor
  ADC0CN0_TEMPE = 1;
  inputVoltageChannel = phy_AddUserADC(ADC0MX_ADC0MX__ADC0P18);
  tempSensorChannel = phy_AddUserADC(ADC0MX_ADC0MX__TEMP);

  // Wait for the temperature to drop below user-defined TEMPERATURE_C_ON
  // before enabling VBus.
  do
  {
    // Delay for next temperature sensor reading to be available
    for (i = 0; i < 0x2000; i++)
    {}

    temperature_c = app_ConvertCodesToCelsius(phy_ReadUserADCmV(tempSensorChannel));
  } while (temperature_c > TEMPERATURE_C_ON);

  dbgPrintf(DBG_INFO, ("High-Power Charger Starting\n"));
  dbgPrintf(DBG_INFO, ("------------------------------------\n\n"));
  dbgPrintf(DBG_INFO, ("Initial temperature: %d C\n", temperature_c));


  while (1)
  {
// $[Generated Run-time code]
// [Generated Run-time code]$
    knltcb_RunSystem();
  }
}
