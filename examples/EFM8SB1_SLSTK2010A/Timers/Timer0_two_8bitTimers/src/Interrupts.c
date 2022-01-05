//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include <SI_EFM8SB1_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             20000000/8  // SYSCLK in Hz (20 MHz internal
                                       // low power oscillator / 8)
                                       // the low power oscillator has a
                                       // tolerance of +/- 10%
                                       // the precision oscillator has a
                                       // tolerance of +/- 2%


#define TIMER_PRESCALER            48  // Based on Timer CKCON0 settings

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS  SYSCLK/TIMER_PRESCALER/1000

// Note: TIMER_TICKS_PER_MS should not exceed 255 (0xFF) for the 8-bit timers

#define AUX1     TIMER_TICKS_PER_MS
#define AUX2     -AUX1

#define LED0_TOGGLE_RATE          100  // LED0 toggle rate in milliseconds
                                       // if LED0_TOGGLE_RATE = 1, the LED will
                                       // be on for 1 millisecond and off for
                                       // 1 millisecond

#define TEST_OUT_TOGGLE_RATE      250  // TEST_OUT pin toggle rate in milliseconds
                                       // if TEST_OUT_TOGGLE_RATE = 1, the
                                       // SIGNAL output will be on for 1
                                       // millisecond and off for 1
                                       // millisecond

#define TIMER0_RELOAD_HIGH       AUX2  // Reload value for Timer0 high byte
#define TIMER0_RELOAD_LOW        AUX2  // Reload value for Timer0 low byte

SI_SBIT (LED0, SFR_P1, 1);             // LED0 == 1 means ON
SI_SBIT (TEST_OUT, SFR_P0, 1);

//---
//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
//
// TIMER0 ISR Content goes here. Remember to clear flag bits:
// TCON::TF0 (Timer 0 Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
	static uint16_t low_counter=0;  // Define counter variable

	TL0 = TIMER0_RELOAD_LOW;            // Reinit Timer0 Low register
	if((low_counter++) == LED0_TOGGLE_RATE)
	{
	  low_counter = 0;                 // Reset interrupt counter
	  LED0 = !LED0;        // Toggle the LED
	}
}


//-----------------------------------------------------------------------------
// TIMER1_ISR
//-----------------------------------------------------------------------------
//
// TIMER1 ISR Content goes here. Remember to clear flag bits:
// TCON::TF1 (Timer 1 Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER1_ISR, TIMER1_IRQn)
{
	static uint16_t high_counter=0;

	TH0 = TIMER0_RELOAD_HIGH;           // Reinit Timer0 High register
	if((high_counter++) == TEST_OUT_TOGGLE_RATE)
	{
	  high_counter = 0;                // Reset interrupt counter
	  TEST_OUT = !TEST_OUT;              // Toggle TEST_OUT
	}
}

