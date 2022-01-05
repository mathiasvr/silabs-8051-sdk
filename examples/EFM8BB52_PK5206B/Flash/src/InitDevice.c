//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB52_Register_Enums.h>
#include "InitDevice.h"

// USER PROTOTYPES
// USER FUNCTIONS

// $[Library Includes]
// [Library Includes]$

//==============================================================================
// enter_DefaultMode_from_RESET
//==============================================================================
extern void enter_DefaultMode_from_RESET(void) {
  // $[Config Calls]
  // Save the SFRPAGE
  uint8_t SFRPAGE_save = SFRPAGE;
  WDT_0_enter_DefaultMode_from_RESET();
  PORTS_1_enter_DefaultMode_from_RESET();
  PORTS_3_enter_DefaultMode_from_RESET();
  PBCFG_0_enter_DefaultMode_from_RESET();
  CLOCK_0_enter_DefaultMode_from_RESET();
  TIMER_SETUP_0_enter_DefaultMode_from_RESET();
  // Restore the SFRPAGE
  SFRPAGE = SFRPAGE_save;
  // [Config Calls]$

}

//================================================================================
// WDT_0_enter_DefaultMode_from_RESET
//================================================================================
extern void WDT_0_enter_DefaultMode_from_RESET(void) {
  // $[Watchdog Timer Init Variable Declarations]
  // [Watchdog Timer Init Variable Declarations]$
  
  // $[WDTCN - Watchdog Timer Control]
  SFRPAGE = 0x00;
  //Disable Watchdog with key sequence
  WDTCN = 0xDE; //First key
  WDTCN = 0xAD; //Second key
  // [WDTCN - Watchdog Timer Control]$

}

//================================================================================
// PORTS_1_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_1_enter_DefaultMode_from_RESET(void) {
  // $[P1 - Port 1 Pin Latch]
  /*
   // B0 (Port 1 Bit 0 Latch) = HIGH (P1.0 is high. Set P1.0 to drive or
   //     float high.)
   // B1 (Port 1 Bit 1 Latch) = HIGH (P1.1 is high. Set P1.1 to drive or
   //     float high.)
   // B2 (Port 1 Bit 2 Latch) = HIGH (P1.2 is high. Set P1.2 to drive or
   //     float high.)
   // B3 (Port 1 Bit 3 Latch) = HIGH (P1.3 is high. Set P1.3 to drive or
   //     float high.)
   // B4 (Port 1 Bit 4 Latch) = HIGH (P1.4 is high. Set P1.4 to drive or
   //     float high.)
   // B5 (Port 1 Bit 5 Latch) = HIGH (P1.5 is high. Set P1.5 to drive or
   //     float high.)
   // B6 (Port 1 Bit 6 Latch) = HIGH (P1.6 is high. Set P1.6 to drive or
   //     float high.)
   // B7 (Port 1 Bit 7 Latch) = HIGH (P1.7 is high. Set P1.7 to drive or
   //     float high.)
   */
  P1 = P1_B0__HIGH | P1_B1__HIGH | P1_B2__HIGH | P1_B3__HIGH | P1_B4__HIGH
      | P1_B5__HIGH | P1_B6__HIGH | P1_B7__HIGH;
  // [P1 - Port 1 Pin Latch]$

  // $[P1MDOUT - Port 1 Output Mode]
  /*
   // B0 (Port 1 Bit 0 Output Mode) = OPEN_DRAIN (P1.0 output is open-
   //     drain.)
   // B1 (Port 1 Bit 1 Output Mode) = OPEN_DRAIN (P1.1 output is open-
   //     drain.)
   // B2 (Port 1 Bit 2 Output Mode) = OPEN_DRAIN (P1.2 output is open-
   //     drain.)
   // B3 (Port 1 Bit 3 Output Mode) = OPEN_DRAIN (P1.3 output is open-
   //     drain.)
   // B4 (Port 1 Bit 4 Output Mode) = PUSH_PULL (P1.4 output is push-pull.)
   // B5 (Port 1 Bit 5 Output Mode) = OPEN_DRAIN (P1.5 output is open-
   //     drain.)
   // B6 (Port 1 Bit 6 Output Mode) = OPEN_DRAIN (P1.6 output is open-
   //     drain.)
   // B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
   //     drain.)
   */
  P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
      | P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN
      | P1MDOUT_B4__PUSH_PULL | P1MDOUT_B5__OPEN_DRAIN
      | P1MDOUT_B6__OPEN_DRAIN | P1MDOUT_B7__OPEN_DRAIN;
  // [P1MDOUT - Port 1 Output Mode]$

  // $[P1MDIN - Port 1 Input Mode]
  /*
   // B0 (Port 1 Bit 0 Input Mode) = DIGITAL (P1.0 pin is configured for
   //     digital mode.)
   // B1 (Port 1 Bit 1 Input Mode) = DIGITAL (P1.1 pin is configured for
   //     digital mode.)
   // B2 (Port 1 Bit 2 Input Mode) = DIGITAL (P1.2 pin is configured for
   //     digital mode.)
   // B3 (Port 1 Bit 3 Input Mode) = DIGITAL (P1.3 pin is configured for
   //     digital mode.)
   // B4 (Port 1 Bit 4 Input Mode) = DIGITAL (P1.4 pin is configured for
   //     digital mode.)
   // B5 (Port 1 Bit 5 Input Mode) = DIGITAL (P1.5 pin is configured for
   //     digital mode.)
   // B6 (Port 1 Bit 6 Input Mode) = DIGITAL (P1.6 pin is configured for
   //     digital mode.)
   // B7 (Port 1 Bit 7 Input Mode) = DIGITAL (P1.7 pin is configured for
   //     digital mode.)
   */
  P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__DIGITAL | P1MDIN_B2__DIGITAL
      | P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__DIGITAL
      | P1MDIN_B6__DIGITAL | P1MDIN_B7__DIGITAL;
  // [P1MDIN - Port 1 Input Mode]$

  // $[P1SKIP - Port 1 Skip]
  /*
   // B0 (Port 1 Bit 0 Skip) = NOT_SKIPPED (P1.0 pin is not skipped by the
   //     crossbar.)
   // B1 (Port 1 Bit 1 Skip) = NOT_SKIPPED (P1.1 pin is not skipped by the
   //     crossbar.)
   // B2 (Port 1 Bit 2 Skip) = NOT_SKIPPED (P1.2 pin is not skipped by the
   //     crossbar.)
   // B3 (Port 1 Bit 3 Skip) = NOT_SKIPPED (P1.3 pin is not skipped by the
   //     crossbar.)
   // B4 (Port 1 Bit 4 Skip) = SKIPPED (P1.4 pin is skipped by the
   //     crossbar.)
   // B5 (Port 1 Bit 5 Skip) = NOT_SKIPPED (P1.5 pin is not skipped by the
   //     crossbar.)
   // B6 (Port 1 Bit 6 Skip) = NOT_SKIPPED (P1.6 pin is not skipped by the
   //     crossbar.)
   // B7 (Port 1 Bit 7 Skip) = NOT_SKIPPED (P1.7 pin is not skipped by the
   //     crossbar.)
   */
  P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED
      | P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED
      | P1SKIP_B4__SKIPPED | P1SKIP_B5__NOT_SKIPPED
      | P1SKIP_B6__NOT_SKIPPED | P1SKIP_B7__NOT_SKIPPED;
  // [P1SKIP - Port 1 Skip]$

  // $[P1MASK - Port 1 Mask]
  /*
   // B0 (Port 1 Bit 0 Mask Value) = IGNORED (P1.0 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B1 (Port 1 Bit 1 Mask Value) = IGNORED (P1.1 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B2 (Port 1 Bit 2 Mask Value) = IGNORED (P1.2 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B3 (Port 1 Bit 3 Mask Value) = IGNORED (P1.3 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B4 (Port 1 Bit 4 Mask Value) = IGNORED (P1.4 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B5 (Port 1 Bit 5 Mask Value) = IGNORED (P1.5 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B6 (Port 1 Bit 6 Mask Value) = IGNORED (P1.6 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B7 (Port 1 Bit 7 Mask Value) = IGNORED (P1.7 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   */
  P1MASK = P1MASK_B0__IGNORED | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
      | P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
      | P1MASK_B6__IGNORED | P1MASK_B7__IGNORED;
  // [P1MASK - Port 1 Mask]$

  // $[P1MAT - Port 1 Match]
  /*
   // B0 (Port 1 Bit 0 Match Value) = HIGH (P1.0 pin logic value is compared
   //     with logic HIGH.)
   // B1 (Port 1 Bit 1 Match Value) = HIGH (P1.1 pin logic value is compared
   //     with logic HIGH.)
   // B2 (Port 1 Bit 2 Match Value) = HIGH (P1.2 pin logic value is compared
   //     with logic HIGH.)
   // B3 (Port 1 Bit 3 Match Value) = HIGH (P1.3 pin logic value is compared
   //     with logic HIGH.)
   // B4 (Port 1 Bit 4 Match Value) = HIGH (P1.4 pin logic value is compared
   //     with logic HIGH.)
   // B5 (Port 1 Bit 5 Match Value) = HIGH (P1.5 pin logic value is compared
   //     with logic HIGH.)
   // B6 (Port 1 Bit 6 Match Value) = HIGH (P1.6 pin logic value is compared
   //     with logic HIGH.)
   // B7 (Port 1 Bit 7 Match Value) = HIGH (P1.7 pin logic value is compared
   //     with logic HIGH.)
   */
  P1MAT = P1MAT_B0__HIGH | P1MAT_B1__HIGH | P1MAT_B2__HIGH | P1MAT_B3__HIGH
      | P1MAT_B4__HIGH | P1MAT_B5__HIGH | P1MAT_B6__HIGH | P1MAT_B7__HIGH;
  // [P1MAT - Port 1 Match]$

}

//================================================================================
// PORTS_3_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_3_enter_DefaultMode_from_RESET(void) {
  // $[P3 - Port 3 Pin Latch]
  // [P3 - Port 3 Pin Latch]$

  // $[P3MDOUT - Port 3 Output Mode]
  /*
   // B0 (Port 3 Bit 0 Output Mode) = OPEN_DRAIN (P3.0 output is open-
   //     drain.)
   // B1 (Port 3 Bit 1 Output Mode) = OPEN_DRAIN (P3.1 output is open-
   //     drain.)
   // B2 (Port 3 Bit 2 Output Mode) = OPEN_DRAIN (P3.2 output is open-
   //     drain.)
   // B3 (Port 3 Bit 3 Output Mode) = OPEN_DRAIN (P3.3 output is open-
   //     drain.)
   // B4 (Port 3 Bit 4 Output Mode) = PUSH_PULL (P3.4 output is push-pull.)
   // B7 (Port 3 Bit 7 Output Mode) = OPEN_DRAIN (P3.7 output is open-
   //     drain.)
   */
  SFRPAGE = 0x20;
  P3MDOUT = P3MDOUT_B0__OPEN_DRAIN | P3MDOUT_B1__OPEN_DRAIN
      | P3MDOUT_B2__OPEN_DRAIN | P3MDOUT_B3__OPEN_DRAIN
      | P3MDOUT_B4__PUSH_PULL | P3MDOUT_B7__OPEN_DRAIN;
  // [P3MDOUT - Port 3 Output Mode]$

  // $[P3MDIN - Port 3 Input Mode]
  // [P3MDIN - Port 3 Input Mode]$

}

//================================================================================
// PBCFG_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PBCFG_0_enter_DefaultMode_from_RESET(void) {
  // $[XBR2 - Port I/O Crossbar 2]
  /*
   // WEAKPUD (Port I/O Weak Pullup Disable) = PULL_UPS_ENABLED (Weak
   //     Pullups enabled (except for Ports whose I/O are configured for analog
   //     mode).)
   // XBARE (Crossbar Enable) = ENABLED (Crossbar enabled.)
   // URT1E (UART1 I/O Enable) = DISABLED (UART1 I/O unavailable at Port
   //     pin.)
   // URT1RTSE (UART1 RTS Output Enable) = DISABLED (UART1 RTS1 unavailable
   //     at Port pin.)
   // URT1CTSE (UART1 CTS Input Enable) = DISABLED (UART1 CTS1 unavailable
   //     at Port pin.)
   */
  SFRPAGE = 0x00;
  XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED
      | XBR2_URT1E__DISABLED | XBR2_URT1RTSE__DISABLED
      | XBR2_URT1CTSE__DISABLED;
  // [XBR2 - Port I/O Crossbar 2]$

  // $[PRTDRV - Port Drive Strength]
  // [PRTDRV - Port Drive Strength]$

  // $[XBR0 - Port I/O Crossbar 0]
  // [XBR0 - Port I/O Crossbar 0]$

  // $[XBR1 - Port I/O Crossbar 1]
  // [XBR1 - Port I/O Crossbar 1]$

}

//==============================================================================
// CLOCK_0_enter_DefaultMode_from_RESET
//==============================================================================
extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
    // $[HFOSC1 Setup]
    // [HFOSC1 Setup]$

    // $[CLKSEL - Clock Select]
    /***********************************************************************
     - Clock derived from the Internal High Frequency Oscillator 0
     - SYSCLK is equal to selected clock source divided by 8
     ***********************************************************************/
    SFRPAGE = 0x0;
    CLKSEL = CLKSEL_CLKSL__HFOSC0_clk24p5 | CLKSEL_CLKDIV__SYSCLK_DIV_8;
    while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY);
    // [CLKSEL - Clock Select]$

    // $[CLKGRP0 - Clock Group 0]
    // [CLKGRP0 - Clock Group 0]$
}

//================================================================================
// TIMER_SETUP_0_enter_DefaultMode_from_RESET
//================================================================================
extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void) {
  // $[CKCON0 - Clock Control 0]
  /*
   // SCA (Timer 0/1 Prescale) = SYSCLK_DIV_48 (System clock divided by 48.)
   // T0M (Timer 0 Clock Select) = PRESCALE (Counter/Timer 0 uses the clock
   //     defined by the prescale field, SCA.)
   // T2MH (Timer 2 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 2 high
   //     byte uses the clock defined by T2XCLK in TMR2CN0.)
   // T2ML (Timer 2 Low Byte Clock Select) = EXTERNAL_CLOCK (Timer 2 low
   //     byte uses the clock defined by T2XCLK in TMR2CN0.)
   // T3MH (Timer 3 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 high
   //     byte uses the clock defined by T3XCLK in TMR3CN0.)
   // T3ML (Timer 3 Low Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 low
   //     byte uses the clock defined by T3XCLK in TMR3CN0.)
   // T1M (Timer 1 Clock Select) = PRESCALE (Timer 1 uses the clock defined
   //     by the prescale field, SCA.)
   */
  CKCON0 = CKCON0_SCA__SYSCLK_DIV_48 | CKCON0_T0M__PRESCALE
      | CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__EXTERNAL_CLOCK
      | CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
      | CKCON0_T1M__PRESCALE;
  // [CKCON0 - Clock Control 0]$

  // $[CKCON1 - Clock Control 1]
  // [CKCON1 - Clock Control 1]$

  // $[TMOD - Timer 0/1 Mode]
  /*
   // T0M (Timer 0 Mode Select) = MODE1 (Mode 1, 16-bit Counter/Timer)
   // T1M (Timer 1 Mode Select) = MODE0 (Mode 0, 13-bit Counter/Timer)
   // CT0 (Counter/Timer 0 Select) = TIMER (Timer Mode. Timer 0 increments
   //     on the clock defined by T0M in the CKCON0 register.)
   // GATE0 (Timer 0 Gate Control) = DISABLED (Timer 0 enabled when TR0 = 1
   //     irrespective of INT0 logic level.)
   // CT1 (Counter/Timer 1 Select) = TIMER (Timer Mode. Timer 1 increments
   //     on the clock defined by T1M in the CKCON0 register.)
   // GATE1 (Timer 1 Gate Control) = DISABLED (Timer 1 enabled when TR1 = 1
   //     irrespective of INT1 logic level.)
   */
  TMOD = TMOD_T0M__MODE1 | TMOD_T1M__MODE0 | TMOD_CT0__TIMER
      | TMOD_GATE0__DISABLED | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
  // [TMOD - Timer 0/1 Mode]$

  // $[TCON - Timer 0/1 Control]
  // [TCON - Timer 0/1 Control]$

}

