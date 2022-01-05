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
// Configurator set for HFOSC0/8
#define SYSCLK             3062000

// Configurator set for timer overflow every 100 ms / 10 Hz.
#define LED_TOGGLE_RATE           100  // LED toggle rate in milliseconds
                          // if LED_TOGGLE_RATE = 1, the LED will
                          // be on for 1 millisecond and off for
                          // 1 millisecond

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------
SI_SBIT(LED0, SFR_P1, 5);                 // LED0

//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
//
// TIMER0 ISR Content goes here. Remember to clear flag bits:
// TCON::TF0 (Timer 0 Overflow Flag)
//
// Here we process the Timer0 interrupt and toggle the LED when appropriate
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
  static uint16_t counter = 0;

  counter++;

  if(counter == LED_TOGGLE_RATE)
  {
    LED0 = !LED0;                    // Toggle the LED
    counter = 0;
  }
}


