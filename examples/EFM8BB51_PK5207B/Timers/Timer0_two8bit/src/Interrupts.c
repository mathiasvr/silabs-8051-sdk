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
#define SYSCLK             24500000/8  // SYSCLK in Hz
#define TIMER_PRESCALER            48  // Based on Timer CKCON settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: TIMER_TICKS_PER_MS should not exceed 255 (0xFF) for the 8-bit timers

#define LED_0_TOGGLE_RATE         100  // LED_GREEN toggle rate in milliseconds
                          // If LED_GREEN_TOGGLE_RATE = 1, LED_GREEN will
                          // be on for 1 millisecond and off for
                          // 1 millisecond

#define LED_1_TOGGLE_RATE          30  // LED_BLUE toggle rate in milliseconds
                          // if LED_BLUE_TOGGLE_RATE = 1, LED_BLUE will
                          // be on for 1 millisecond and off for
                          // 1 millisecond

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT (LED0, SFR_P1, 5);             // LED0
SI_SBIT (LED1, SFR_P1, 6);             // LED1

//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
//
// TIMER0 ISR Content goes here. Remember to clear flag bits:
// TCON::TF0 (Timer 0 Overflow Flag)
//
// Here we process the Timer0 interrupt and toggle the green LED
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
  static uint16_t low_counter = 0;

  TL0 = -TIMER_TICKS_PER_MS;        // Reset Timer0 Low register

  low_counter++;

  if (low_counter == LED_0_TOGGLE_RATE)
  {
    LED0 = !LED0;                    // Toggle the LED

    low_counter = 0;                 // Reset interrupt counter
  }
}

//-----------------------------------------------------------------------------
// TIMER1_ISR
//-----------------------------------------------------------------------------
//
// TIMER1 ISR Content goes here. Remember to clear flag bits:
// TCON::TF1 (Timer 1 Overflow Flag)
//
// Note: In this mode the Timer0 is effectively using the Timer1 interrupt
// vector. For more info, check the Timers chapter of the datasheet.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER1_ISR, TIMER1_IRQn)
{
  static uint16_t high_counter = 0;

  TH0 = -TIMER_TICKS_PER_MS;        // Reset Timer0 High register

  high_counter++;

  if (high_counter == LED_1_TOGGLE_RATE)
  {
    LED1 = !LED1;                    // Toggle the LED

    high_counter = 0;                // Reset interrupt counter
  }
}

