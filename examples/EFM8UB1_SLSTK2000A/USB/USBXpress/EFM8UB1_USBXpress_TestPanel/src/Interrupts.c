//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// -----------------------------------------------------------------------------
// Includes

#include <SI_EFM8UB1_Register_Enums.h>
#include "bsp.h"
#include "descriptor.h"
#include "joystick.h"

// -----------------------------------------------------------------------------
// Constants

// -------------------------------
// Temperature Sensor Coefficients

#define SCALE       1000L   // Scale for temp calculations
#define SLOPE       2262    // Slope of the temp transfer function
#define OFFSET      265614L // Offset for the temp transfer function
#define OVER_ROUND  10      // Number of shifts (>>N) to reach the
                            // correct number of bits of precision

// -------------------------------
// Switch Locations

#define P0_PB0_BITMASK 0x04
#define P0_PB1_BITMASK 0x08

// -----------------------------------------------------------------------------
// Variables

uint16_t writeLen;
volatile uint8_t joystickDirection;

// -----------------------------------------------------------------------------
// Extern Variables

extern volatile uint8_t joystickValue;
extern volatile uint8_t temperature;
extern bool pb0State;
extern bool pb1State;
extern SI_SEGMENT_VARIABLE(inPacket[USB_PACKET_SIZE], uint8_t, SI_SEG_XDATA);

// -----------------------------------------------------------------------------
// Function Prototypes

static uint8_t remap_joystick(uint8_t joystickValue);

// -----------------------------------------------------------------------------
// Functions

//-----------------------------------------------------------------------------
// TIMER3_ISR
//-----------------------------------------------------------------------------
//
// TIMER3 ISR Content goes here. Remember to clear flag bits:
// TMR3CN0::TF3H (Timer # High Byte Overflow Flag)
// TMR3CN0::TF3L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
{
  uint8_t SFRPAGE_save = SFRPAGE;
  SFRPAGE = TIMER3_PAGE;

  // High byte overflow
  if (TMR3CN0 & TMR3CN0_TF3H__SET)
  {
    TMR3CN0 &= ~ TMR3CN0_TF3H__SET;
  }

  SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PMATCH_ISR
//-----------------------------------------------------------------------------
//
// PMATCH ISR Content goes here. Remember to clear flag bits:
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PMATCH_ISR, PMATCH_IRQn)
{
  uint8_t SFRPAGE_save = SFRPAGE;
  SFRPAGE = LEGACY_PAGE;

  // PB0
  if ((P0 & P0_PB0_BITMASK) ^ (P0MAT & P0_PB0_BITMASK))
  {
    if (BSP_PB0)
    {
      P0MAT |= P0_PB0_BITMASK;
    }
    else
    {
      // At this point, PB0 has been pressed and released.
      // Toggle pb0State
      pb0State = ~pb0State;
      P0MAT &= ~P0_PB0_BITMASK;

    }
  }
  // PB1
  else if ((P0 & P0_PB1_BITMASK) ^ (P0MAT & P0_PB1_BITMASK))
  {
    if (BSP_PB1)
    {
      P0MAT |= P0_PB1_BITMASK;
    }
    else
    {
      // At this point, PB1 has been pressed and released.
      // Toggle pb1State
      pb1State = ~pb1State;
      P0MAT &= ~P0_PB1_BITMASK;
    }
  }

  SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// ADC0EOC_ISR
//-----------------------------------------------------------------------------
//
// ADC0EOC ISR Content goes here. Remember to clear flag bits:
// ADC0CN0::ADINT (Conversion Complete Interrupt Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0EOC_ISR, ADC0EOC_IRQn)
{
  uint16_t sample;
  int32_t temp_scaled;
  int16_t temp_frac;
  uint32_t mv;
  uint8_t SFRPAGE_save = SFRPAGE;
  SFRPAGE = LEGACY_PAGE;

  // Temperature Sensor reading
  if (ADC0MX == ADC0MX_ADC0MX__TEMP)
  {
    temp_scaled = ADC0;
    temp_scaled *= SLOPE;

    // With a left-justified ADC, we have to shift the decimal place
    // of temp_scaled to the right so we can match the format of
    // OFFSET. Once the formats are matched, we can subtract OFFSET.
    temp_scaled = temp_scaled >> OVER_ROUND;
    temp_scaled -= OFFSET;

    // Calculate the temp's whole number portion by unscaling
    temperature = temp_scaled / SCALE;

    // Get the fractional part and round temperature as needed
    temp_frac = (int16_t)((temp_scaled - temperature * SCALE) / (SCALE / 10));

    if (temp_frac > 5)
    {
      temperature++;
    }
    else if (temp_frac < -5)
    {
      temperature--;
    }

    // Switch ADC0 setting for joystick
    ADC0MX = ADC0MX_ADC0MX__ADC0P15;
    ADC0AC = ADC0AC_ADSJST__RIGHT_NO_SHIFT
               | ADC0AC_AD12BE__12_BIT_DISABLED
               | ADC0AC_ADAE__ACC_DISABLED
               | ADC0AC_ADRPT__ACC_1;
  }
  // Joystick reading
  else
  {
    sample = ADC0;
    mv = ((uint32_t)sample) * 3300 / 1024;

    joystickDirection = JOYSTICK_convert_mv_to_direction(mv);
    joystickValue = remap_joystick(joystickDirection);

    // Switch ADC0 setting for temperature sensor
    ADC0MX = ADC0MX_ADC0MX__TEMP;
    ADC0AC = ADC0AC_ADSJST__LEFT_NO_SHIFT
               | ADC0AC_AD12BE__12_BIT_DISABLED
               | ADC0AC_ADAE__ACC_DISABLED
               | ADC0AC_ADRPT__ACC_1;
  }

  ADC0CN0_ADINT = 0;
  SFRPAGE = SFRPAGE_save;

  USBX_blockWrite(inPacket, USB_PACKET_SIZE, &writeLen);
}

/**************************************************************************//**
 * @brief
 *   Converts internal joystick direction value to a value between 0 and 255
 *   for display by the TestPanel application.
 *
 * @param   joystickValue:
 *   Direction of joystick as returned by the BSP function
 *   JOYSTICK_convert_mv_to_direction().
 * @return
 *   This function scales the value so that it will display nicely in
 *   the TestPanel application.
 *
 *****************************************************************************/
static uint8_t remap_joystick(uint8_t joystickValue)
{
  uint8_t retval = 0;

  // Treat the joystick as if it is an analog clock.
  switch (joystickValue)
  {
    case JOYSTICK_NONE:
        retval = 0;
        break;
    case JOYSTICK_C:
        retval = 128;
        break;
    case JOYSTICK_N:
        retval = 255;
        break;
    case JOYSTICK_E:
        retval = 64;
        break;
    case JOYSTICK_S:
        retval = 128;
        break;
    case JOYSTICK_W:
        retval = 192;
        break;
    case JOYSTICK_NE:
        retval = 32;
        break;
    case JOYSTICK_NW:
        retval = 224;
        break;
    case JOYSTICK_SE:
        retval = 96;
        break;
    case JOYSTICK_SW:
        retval = 160;
        break;
  }

  return retval;
}