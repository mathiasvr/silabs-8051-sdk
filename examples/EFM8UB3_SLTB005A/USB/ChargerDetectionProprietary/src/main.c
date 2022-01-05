//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates the USB charger detection hardware feature.
// The device will detect whether it's connected to a standard downstream
// port (SDP) or dedicated charging port (DCP) and display "SDP Detected"
// or "DCP Detected" on the LCD.
//
// In addition to the hardware charger detection functionality, this demo
// measures the voltage on D+/D- proportional to VBUS to detect other proprietary
// chargers based on voltage dividers.
//
// Proprietary Charger Update:
// - Added support for proprietary charger types by measuring VBUS, D+
// and D- and comparing against a known list of voltages (with 10% tolerance).
// - To use VREGIN/4 to measure VBUS, VBUS must be tied to VREGIN. On the STK
// the switch must be set to USB, otherwise VBUS will always read 3.3V (VDD)
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB3 STK
//-----------------------------------------------------------------------------
// 1) Place the "Power Source" switch in "DBG USB" mode.
// 2) Connect the EFM8UB3 STK board to a PC using a micro USB cable.
// 3) Compile and download code to the EFM8UB3 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Move the "Power Source" switch to "EFM8 USB" mode to connect VBUS to the
//    VREGIN pin, which is used to scale D+/D- voltage divider voltages relative
//    to VBUS (using VREGIN/4). The measured VBUS voltage will not be correct
//    if VREGIN is connected to VMCU/VBAT in "DBG USB" mode.
// 6) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 7) Connect the STK board to a USB host or charger via micro USB cable.
// 8) The terminal should display "SDP Detected", "DCP Detected", or one of the
//    proprietary charger types from charger_descriptions based on detected
//    charger type.
// 9) The terminal will also display the measured VBUS, D+, and D- voltages. VBUS
//    should measure between 4.4 to 5.25 V to meet USB specifications.
// 9) Disconnect the micro USB cable.
// 10) Connect the micro USB cable to another charger to detect a different
//     charger type.
//
// Target:         EFM8UB3
// Tool chain:     Generic
//
// Release 0.1 (CM)
//    - Initial Revision
//    - 10 OCT 2014
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 24.5 MHz HFOSC1 / 1
// ADC0   - 10-bit, 6.25 MHz SARCLK, 2.4V VREF, 0.5x gain (AIN limited to VDD)
// SPI0   - 1 MHz
// USB0   - Full speed
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.1 - CS (Active High)
// P0.2 - push button
// P0.3 - push button
// P0.6 - SCK
// P1.0 - MOSI
// P1.4 - LED Green
// P1.5 - LED Blue
// P1.6 - LED Red
//
//-----------------------------------------------------------------------------
// Project Modules
//-----------------------------------------------------------------------------
// efm8.Assert
// efm8.bsp.Efm8 Bsp
// efm8.PeripheralDrivers.Usb0
// efm8.USB

/////////////////////////////////////////////////////////////////////////////
// main.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "InitDevice.h"
#include <stdio.h>
#include <string.h>
#include "tick.h"
#include "retargetserial.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

// Charger type definition
#define NO_CHARGER                        0
#define SDP_CHARGER                       1
#define DCP_CHARGER                       2
#define CDP_CHARGER                       3
// Proprietary chargers
#define APPLE_2A1_CHARGER                 4
#define APPLE_1A_CHARGER                  5
#define APPLE_0A5_CHARGER                 6
#define TOMTOM_CHARGER                    7
#define SAMSUNG_CHARGER                   8

// Number of proprietary chargers
#define PROPRIETARY_CHARGER_LIST_SIZE     5

// Largest charger type value used to bounds check
// array lookup
#define MAX_CHARGER_TYPE                  SAMSUNG_CHARGER

// Percent error in measured D+/D- used for proprietary charger
// detection
#define TOLERANCE_PCT                     10

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

// Proprietary charger descriptor
typedef struct proprietary_charger_desc
{
  uint8_t charger_type;   // Charger type (ie APPLE_2A1_CHARGER)
  uint16_t dp_mv;         // Nominal D+ voltage in mV
  uint16_t dm_mv;         // Nominal D- voltage in mV

} proprietary_charger_desc_t;

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t tmpBuffer;
uint8_t chargerType = 0;

// Store measured voltages from VREGIN, D+, and D- (mV)
SI_SEGMENT_VARIABLE(vregin_mv, uint16_t, SI_SEG_IDATA) = 0;
SI_SEGMENT_VARIABLE(dp_mv, uint16_t, SI_SEG_IDATA) = 0;
SI_SEGMENT_VARIABLE(dm_mv, uint16_t, SI_SEG_IDATA) = 0;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

// Charger string descriptions
SI_VARIABLE_SEGMENT_POINTER(charger_descriptions[], char, SI_SEG_CODE) =
{
  "",
  "SDP Detected",
  "DCP Detected",
  "CDP Detected",
  "Apple 2.1 A",
  "Apple 1 A",
  "Apple 500 mA",
  "Tom Tom",
  "Samsung",
};

// Proprietary charger descriptors with nominal voltages
SI_SEGMENT_VARIABLE(proprietary_charger_list[], proprietary_charger_desc_t, SI_SEG_CODE) =
{
  {APPLE_2A1_CHARGER, 2800, 2000},
  {APPLE_1A_CHARGER, 2000, 2800},
  {APPLE_0A5_CHARGER, 2000, 2000},
  {TOMTOM_CHARGER, 2000, 3100},
  {SAMSUNG_CHARGER, 1250, 1250}
};

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

static uint16_t ADC_GetSample(uint8_t muxSelect);
static uint16_t AbsErrorPct(int16_t actual, int16_t expected);
static uint8_t USB_Detect_Non_Compliant_Charger();
static uint8_t USB_Detect_Charger(void);

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------
void main(void)
{
  bool runDemo = false;

  enter_DefaultMode_from_RESET();

  // Enable VBUS detection on P3.1 (disable weak pull up)
  SFRPAGE = PG3_PAGE;
  USB0CF |= USB0CF_VBUSEN__ENABLED;
  SFRPAGE = 0;

  SCON1_TI = 1;                 // Set transmit flag to 1

  while (1)
  {
    // Wait for VBUS signal high (charger connected)
    while (BSP_VBUS == 0);

    // Measure VBUS (VREGIN), D+, and D-
    // Detect proprietary and standard charger types
    chargerType = USB_Detect_Charger();

    // Display charger information
    if (chargerType <= MAX_CHARGER_TYPE)
    {
      // Display charger type
    	RETARGET_PRINTF("%s\n",charger_descriptions[chargerType]);

      // Display VBUS voltage in mV
    	RETARGET_PRINTF("VBUS: %u mV\n", vregin_mv);

      // Display D+ voltage in mV
    	RETARGET_PRINTF("D+: %u mV\n", dp_mv);

      // Display D- voltage in mV
    	RETARGET_PRINTF("D-: %u mV\n", dm_mv);
    }

    // Wait for VBUS signal LOW (charger unplugged)
    while (BSP_VBUS);
  }
}

// Take ADC sample using the specified ADC0MX analog input
static uint16_t ADC_GetSample(uint8_t muxSelect)
{
  // Change analog mux input
  ADC0MX = muxSelect;

  // Wait 10 ms for input to settle
  Wait(10);

  // Clear conversion complete flag
  ADC0CN0_ADINT = 0;

  // Start conversion
  ADC0CN0_ADBUSY = 1;

  // Wait for conversion complete
  while (!ADC0CN0_ADINT);

  // Read sample
  return ADC0;
}

// Return the absolute value of the error between actual and expected
// values
//
// Error % = (actual - expected) * 100 / expected
static uint16_t AbsErrorPct(int16_t actual, int16_t expected)
{
  int32_t error = (int32_t)actual - expected;

  // Take absolute value
  if (error < 0)
    error = -error;

  // Calculate percent
  error = (error * 100) / expected;

  // Truncate to 16-bits
  return (uint16_t)error;
}

// Measure VBUS (via VREGIN), D+, and D- and detect
// proprietary charger types
static uint8_t USB_Detect_Non_Compliant_Charger()
{
  uint8_t i;
  uint16_t sample;
  uint16_t scaled_dp_mv;
  uint16_t scaled_dm_mv;

  // Enable ADC
  ADC0CN0_ADEN = 1;

  // NOTE:
  // If VREGIN is tied to VBUS, the VBUS voltage can be
  // sampled directly by using the VREGIN/4 analog input
  //
  // -otherwise-
  //
  // Add an external VBUS resistor divider connected
  // to an available analog input to accurately measure VBUS

  // Get VREGIN voltage
  // VREGIN = ADC0 x VREF x VREGIN_GAIN x (1/ADC0_GAIN) / 2^10
  // VREGIN = ADC0 x 2.4V x 4 x 2 / 1024
  // VREGIN in mV = ADC0 x 2.4 x 4 x 2 x 1000 / 1024
  sample = ADC_GetSample(ADC0MX_ADC0MX__VREGIN_DIV_4);
  vregin_mv = (uint16_t)(((uint32_t)sample * 24 * 4 * 2 * 1000) / (1024 * 10));

  // Get D+ voltage
  // D+ = ADC0 x VREF x (1/ADC0_GAIN) / 2^10
  // D+ in mV = ADC0 x 2.4 x 2 x 1000 / 2^10
  sample = ADC_GetSample(ADC0MX_ADC0MX__USB_DP);
  dp_mv = (uint16_t)(((uint32_t)sample * 24 * 2 * 1000) / (1024 * 10));

  // Get D- voltage
  // D- = ADC0 x VREF x (1/ADC0_GAIN) / 2^10
  // D- in mV = ADC0 x 2.4 x 2 x 1000 / 2^10
  sample = ADC_GetSample(ADC0MX_ADC0MX__USB_DM);
  dm_mv = (uint16_t)(((uint32_t)sample * 24 * 2 * 1000) / (1024 * 10));

  // Scale D+ & D- voltages to VBUS = 5V
  //
  // We assume D+/D- voltages are created with resistor dividers
  // that scale with VBUS.
  scaled_dp_mv = (uint16_t)(((uint32_t)dp_mv * 5000) / vregin_mv);
  scaled_dm_mv = (uint16_t)(((uint32_t)dm_mv * 5000) / vregin_mv);

  // Disable ADC
  ADC0CN0_ADEN = 0;

  // Loop through each proprietary charger and check for a close
  // match to the nominal D+/D- voltages
  for (i = 0; i < PROPRIETARY_CHARGER_LIST_SIZE; i++)
  {
    if (AbsErrorPct(scaled_dp_mv, proprietary_charger_list[i].dp_mv) < TOLERANCE_PCT &&
        AbsErrorPct(scaled_dm_mv, proprietary_charger_list[i].dm_mv) < TOLERANCE_PCT)
    {
      return proprietary_charger_list[i].charger_type;
    }
  }

  // Otherwise, no proprietary charger detected
  return NO_CHARGER;
}

//-----------------------------------------------------------------------------
// USB_Detect_Charger
//-----------------------------------------------------------------------------
//
// Description - Detect charger type and return it to caller.
//
//-----------------------------------------------------------------------------
static uint8_t USB_Detect_Charger(void)
{
  uint8_t sfrPageSave = SFRPAGE;
  uint8_t ret = NO_CHARGER;

  if (BSP_VBUS == 0)
  {
    return ret;
  }

  // Measure VBUS, D+, and D- and detect any proprietary chargers
  ret = USB_Detect_Non_Compliant_Charger();

  // Return non-compliant charger without performing USB charge detection
  if (ret >= APPLE_2A1_CHARGER)
  {
    return ret;
  }

  SFRPAGE = PG3_PAGE;

  USB0CDSTA = 0;

  // Enable charger detection using primary detection and data contact detection
  USB0CDCN = USB0CDCN_CHDEN__ENABLED
             | USB0CDCN_PDEN__ENABLED
             | USB0CDCN_DCDEN__ENABLED;

  // Wait for primary detection to complete
  while ((USB0CDSTA & USB0CDSTA_PDI__SET) == 0);

  // SDP bit will indicate if a Standard Downstream Port is detected
  if (USB0CDSTA & USB0CDSTA_SDP__DETECTED)
  {
    ret = SDP_CHARGER;
  }
  else
  {
    USB0CDSTA = 0;

    // Do Secondary Detection for DCP and CDP
    USB0CDCN = USB0CDCN_CHDEN__ENABLED | USB0CDCN_SDEN__ENABLED;

    // Wait for Secondary Detection to complete
    while ((USB0CDSTA & USB0CDSTA_SDI__SET) == 0);

    if (USB0CDSTA & USB0CDSTA_CDP__DETECTED)
    {
      ret = CDP_CHARGER;
    }
    else if (USB0CDSTA & USB0CDSTA_DCP__DETECTED)
    {
      ret = DCP_CHARGER;
    }
  }

  SFRPAGE = sfrPageSave;

  return ret;
}
