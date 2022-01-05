//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES      
#include <SI_EFM8BB51_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
// Overflow rates of timer2L and timer2H are set to 1ms in Simplicity Configurator
#define LED_0_TOGGLE_RATE        100   // LED_GREEN toggle rate in milliseconds
                                       // if LED_GREEN_TOGGLE_RATE = 1, LED_GREEN will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond
#define LED_1_TOGGLE_RATE        30    // LED_BLUE toggle rate in milliseconds
                                       // if LED_BLUE_TOGGLE_RATE = 1, LED_BLUE will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (LED0, SFR_P1, 5);             // LED0
SI_SBIT (LED1, SFR_P1, 6);             // LED1

//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN::TF2L (Timer # Low Byte Overflow Flag)
//
// This function processes the Timer2 interrupt. It checks both counters for
// matches and, upon a match, toggles the LEDs.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER2_ISR, TIMER2_IRQn)
{
  static uint16_t low_counter = 0;
  static uint16_t high_counter = 0;

  if (TMR2CN0_TF2L == 1)
  {
    low_counter++;

    if (low_counter == LED_0_TOGGLE_RATE)
    {
      LED0 = !LED0;       // Toggle LED

      low_counter = 0;              // Reset interrupt counter
    }

    TMR2CN0_TF2L = 0;
   }
   if (TMR2CN0_TF2H == 1)
   {
    high_counter++;

    if (high_counter == LED_1_TOGGLE_RATE)
    {
      LED1 = !LED1;         // Toggle LED

      high_counter = 0;             // Reset interrupt counter
    }

    TMR2CN0_TF2H = 0;
   }
}


