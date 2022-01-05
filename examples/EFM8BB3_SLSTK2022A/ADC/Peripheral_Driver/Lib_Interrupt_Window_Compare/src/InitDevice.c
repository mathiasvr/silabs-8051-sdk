//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB3_Register_Enums.h>
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
  PORTS_0_enter_DefaultMode_from_RESET();
  PORTS_1_enter_DefaultMode_from_RESET();
  PORTS_2_enter_DefaultMode_from_RESET();
  PBCFG_0_enter_DefaultMode_from_RESET();
  ADC_0_enter_DefaultMode_from_RESET();
  CLOCK_0_enter_DefaultMode_from_RESET();
  TIMER01_0_enter_DefaultMode_from_RESET();
  TIMER16_2_enter_DefaultMode_from_RESET();
  INTERRUPT_0_enter_DefaultMode_from_RESET();
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
// PORTS_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_0_enter_DefaultMode_from_RESET(void) {
  // $[P0 - Port 0 Pin Latch]
  /*
   // B0 (Port 0 Bit 0 Latch) = HIGH (P0.0 is high. Set P0.0 to drive or
   //     float high.)
   // B1 (Port 0 Bit 1 Latch) = HIGH (P0.1 is high. Set P0.1 to drive or
   //     float high.)
   // B2 (Port 0 Bit 2 Latch) = HIGH (P0.2 is high. Set P0.2 to drive or
   //     float high.)
   // B3 (Port 0 Bit 3 Latch) = HIGH (P0.3 is high. Set P0.3 to drive or
   //     float high.)
   // B4 (Port 0 Bit 4 Latch) = HIGH (P0.4 is high. Set P0.4 to drive or
   //     float high.)
   // B5 (Port 0 Bit 5 Latch) = HIGH (P0.5 is high. Set P0.5 to drive or
   //     float high.)
   // B6 (Port 0 Bit 6 Latch) = HIGH (P0.6 is high. Set P0.6 to drive or
   //     float high.)
   // B7 (Port 0 Bit 7 Latch) = HIGH (P0.7 is high. Set P0.7 to drive or
   //     float high.)
   */
  P0 = P0_B0__HIGH | P0_B1__HIGH | P0_B2__HIGH | P0_B3__HIGH | P0_B4__HIGH
      | P0_B5__HIGH | P0_B6__HIGH | P0_B7__HIGH;
  // [P0 - Port 0 Pin Latch]$

  // $[P0MDOUT - Port 0 Output Mode]
  /*
   // B0 (Port 0 Bit 0 Output Mode) = OPEN_DRAIN (P0.0 output is open-
   //     drain.)
   // B1 (Port 0 Bit 1 Output Mode) = OPEN_DRAIN (P0.1 output is open-
   //     drain.)
   // B2 (Port 0 Bit 2 Output Mode) = OPEN_DRAIN (P0.2 output is open-
   //     drain.)
   // B3 (Port 0 Bit 3 Output Mode) = OPEN_DRAIN (P0.3 output is open-
   //     drain.)
   // B4 (Port 0 Bit 4 Output Mode) = PUSH_PULL (P0.4 output is push-pull.)
   // B5 (Port 0 Bit 5 Output Mode) = OPEN_DRAIN (P0.5 output is open-
   //     drain.)
   // B6 (Port 0 Bit 6 Output Mode) = OPEN_DRAIN (P0.6 output is open-
   //     drain.)
   // B7 (Port 0 Bit 7 Output Mode) = OPEN_DRAIN (P0.7 output is open-
   //     drain.)
   */
  P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN
      | P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__OPEN_DRAIN | P0MDOUT_B4__PUSH_PULL
      | P0MDOUT_B5__OPEN_DRAIN | P0MDOUT_B6__OPEN_DRAIN
      | P0MDOUT_B7__OPEN_DRAIN;
  // [P0MDOUT - Port 0 Output Mode]$

  // $[P0MDIN - Port 0 Input Mode]
  /*
   // B0 (Port 0 Bit 0 Input Mode) = DIGITAL (P0.0 pin is configured for
   //     digital mode.)
   // B1 (Port 0 Bit 1 Input Mode) = DIGITAL (P0.1 pin is configured for
   //     digital mode.)
   // B2 (Port 0 Bit 2 Input Mode) = DIGITAL (P0.2 pin is configured for
   //     digital mode.)
   // B3 (Port 0 Bit 3 Input Mode) = DIGITAL (P0.3 pin is configured for
   //     digital mode.)
   // B4 (Port 0 Bit 4 Input Mode) = DIGITAL (P0.4 pin is configured for
   //     digital mode.)
   // B5 (Port 0 Bit 5 Input Mode) = DIGITAL (P0.5 pin is configured for
   //     digital mode.)
   // B6 (Port 0 Bit 6 Input Mode) = DIGITAL (P0.6 pin is configured for
   //     digital mode.)
   // B7 (Port 0 Bit 7 Input Mode) = DIGITAL (P0.7 pin is configured for
   //     digital mode.)
   */
  P0MDIN = P0MDIN_B0__DIGITAL | P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL
      | P0MDIN_B3__DIGITAL | P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL
      | P0MDIN_B6__DIGITAL | P0MDIN_B7__DIGITAL;
  // [P0MDIN - Port 0 Input Mode]$

  // $[P0SKIP - Port 0 Skip]
  /*
   // B0 (Port 0 Bit 0 Skip) = NOT_SKIPPED (P0.0 pin is not skipped by the
   //     crossbar.)
   // B1 (Port 0 Bit 1 Skip) = NOT_SKIPPED (P0.1 pin is not skipped by the
   //     crossbar.)
   // B2 (Port 0 Bit 2 Skip) = NOT_SKIPPED (P0.2 pin is not skipped by the
   //     crossbar.)
   // B3 (Port 0 Bit 3 Skip) = NOT_SKIPPED (P0.3 pin is not skipped by the
   //     crossbar.)
   // B4 (Port 0 Bit 4 Skip) = NOT_SKIPPED (P0.4 pin is not skipped by the
   //     crossbar.)
   // B5 (Port 0 Bit 5 Skip) = NOT_SKIPPED (P0.5 pin is not skipped by the
   //     crossbar.)
   // B6 (Port 0 Bit 6 Skip) = NOT_SKIPPED (P0.6 pin is not skipped by the
   //     crossbar.)
   // B7 (Port 0 Bit 7 Skip) = NOT_SKIPPED (P0.7 pin is not skipped by the
   //     crossbar.)
   */
  P0SKIP = P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED
      | P0SKIP_B2__NOT_SKIPPED | P0SKIP_B3__NOT_SKIPPED | P0SKIP_B4__NOT_SKIPPED
      | P0SKIP_B5__NOT_SKIPPED | P0SKIP_B6__NOT_SKIPPED
      | P0SKIP_B7__NOT_SKIPPED;
  // [P0SKIP - Port 0 Skip]$

  // $[P0MASK - Port 0 Mask]
  /*
   // B0 (Port 0 Bit 0 Mask Value) = IGNORED (P0.0 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B1 (Port 0 Bit 1 Mask Value) = IGNORED (P0.1 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B2 (Port 0 Bit 2 Mask Value) = IGNORED (P0.2 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B3 (Port 0 Bit 3 Mask Value) = IGNORED (P0.3 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B4 (Port 0 Bit 4 Mask Value) = IGNORED (P0.4 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B5 (Port 0 Bit 5 Mask Value) = IGNORED (P0.5 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B6 (Port 0 Bit 6 Mask Value) = IGNORED (P0.6 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   // B7 (Port 0 Bit 7 Mask Value) = IGNORED (P0.7 pin logic value is
   //     ignored and will not cause a port mismatch event.)
   */
  P0MASK = P0MASK_B0__IGNORED | P0MASK_B1__IGNORED | P0MASK_B2__IGNORED
      | P0MASK_B3__IGNORED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
      | P0MASK_B6__IGNORED | P0MASK_B7__IGNORED;
  // [P0MASK - Port 0 Mask]$

  // $[P0MAT - Port 0 Match]
  /*
   // B0 (Port 0 Bit 0 Match Value) = HIGH (P0.0 pin logic value is compared
   //     with logic HIGH.)
   // B1 (Port 0 Bit 1 Match Value) = HIGH (P0.1 pin logic value is compared
   //     with logic HIGH.)
   // B2 (Port 0 Bit 2 Match Value) = HIGH (P0.2 pin logic value is compared
   //     with logic HIGH.)
   // B3 (Port 0 Bit 3 Match Value) = HIGH (P0.3 pin logic value is compared
   //     with logic HIGH.)
   // B4 (Port 0 Bit 4 Match Value) = HIGH (P0.4 pin logic value is compared
   //     with logic HIGH.)
   // B5 (Port 0 Bit 5 Match Value) = HIGH (P0.5 pin logic value is compared
   //     with logic HIGH.)
   // B6 (Port 0 Bit 6 Match Value) = HIGH (P0.6 pin logic value is compared
   //     with logic HIGH.)
   // B7 (Port 0 Bit 7 Match Value) = HIGH (P0.7 pin logic value is compared
   //     with logic HIGH.)
   */
  P0MAT = P0MAT_B0__HIGH | P0MAT_B1__HIGH | P0MAT_B2__HIGH | P0MAT_B3__HIGH
      | P0MAT_B4__HIGH | P0MAT_B5__HIGH | P0MAT_B6__HIGH | P0MAT_B7__HIGH;
  // [P0MAT - Port 0 Match]$

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
   // B5 (Port 1 Bit 5 Output Mode) = PUSH_PULL (P1.5 output is push-pull.)
   // B6 (Port 1 Bit 6 Output Mode) = PUSH_PULL (P1.6 output is push-pull.)
   // B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
   //     drain.)
   */
  P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
      | P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__PUSH_PULL
      | P1MDOUT_B5__PUSH_PULL | P1MDOUT_B6__PUSH_PULL | P1MDOUT_B7__OPEN_DRAIN;
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
   // B7 (Port 1 Bit 7 Input Mode) = ANALOG (P1.7 pin is configured for
   //     analog mode.)
   */
  P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__DIGITAL | P1MDIN_B2__DIGITAL
      | P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__DIGITAL
      | P1MDIN_B6__DIGITAL | P1MDIN_B7__ANALOG;
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
   // B4 (Port 1 Bit 4 Skip) = NOT_SKIPPED (P1.4 pin is not skipped by the
   //     crossbar.)
   // B5 (Port 1 Bit 5 Skip) = NOT_SKIPPED (P1.5 pin is not skipped by the
   //     crossbar.)
   // B6 (Port 1 Bit 6 Skip) = NOT_SKIPPED (P1.6 pin is not skipped by the
   //     crossbar.)
   // B7 (Port 1 Bit 7 Skip) = SKIPPED (P1.7 pin is skipped by the
   //     crossbar.)
   */
  P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED
      | P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__NOT_SKIPPED
      | P1SKIP_B5__NOT_SKIPPED | P1SKIP_B6__NOT_SKIPPED | P1SKIP_B7__SKIPPED;
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
// PORTS_2_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_2_enter_DefaultMode_from_RESET(void) {
  // $[P2 - Port 2 Pin Latch]
  // [P2 - Port 2 Pin Latch]$

  // $[P2MDOUT - Port 2 Output Mode]
  /*
   // B0 (Port 2 Bit 0 Output Mode) = PUSH_PULL (P2.0 output is push-pull.)
   // B1 (Port 2 Bit 1 Output Mode) = OPEN_DRAIN (P2.1 output is open-
   //     drain.)
   // B2 (Port 2 Bit 2 Output Mode) = OPEN_DRAIN (P2.2 output is open-
   //     drain.)
   // B3 (Port 2 Bit 3 Output Mode) = PUSH_PULL (P2.3 output is push-pull.)
   // B4 (Port 2 Bit 4 Output Mode) = OPEN_DRAIN (P2.4 output is open-
   //     drain.)
   // B5 (Port 2 Bit 5 Output Mode) = OPEN_DRAIN (P2.5 output is open-
   //     drain.)
   // B6 (Port 2 Bit 6 Output Mode) = OPEN_DRAIN (P2.6 output is open-
   //     drain.)
   */
  P2MDOUT = P2MDOUT_B0__PUSH_PULL | P2MDOUT_B1__OPEN_DRAIN
      | P2MDOUT_B2__OPEN_DRAIN | P2MDOUT_B3__PUSH_PULL | P2MDOUT_B4__OPEN_DRAIN
      | P2MDOUT_B5__OPEN_DRAIN | P2MDOUT_B6__OPEN_DRAIN;
  // [P2MDOUT - Port 2 Output Mode]$

  // $[P2MDIN - Port 2 Input Mode]
  // [P2MDIN - Port 2 Input Mode]$

  // $[P2SKIP - Port 2 Skip]
  /*
   // B0 (Port 2 Bit 0 Skip) = SKIPPED (P2.0 pin is skipped by the
   //     crossbar.)
   // B1 (Port 2 Bit 1 Skip) = NOT_SKIPPED (P2.1 pin is not skipped by the
   //     crossbar.)
   // B2 (Port 2 Bit 2 Skip) = NOT_SKIPPED (P2.2 pin is not skipped by the
   //     crossbar.)
   // B3 (Port 2 Bit 3 Skip) = SKIPPED (P2.3 pin is skipped by the
   //     crossbar.)
   */
  SFRPAGE = 0x20;
  P2SKIP = P2SKIP_B0__SKIPPED | P2SKIP_B1__NOT_SKIPPED | P2SKIP_B2__NOT_SKIPPED
      | P2SKIP_B3__SKIPPED;
  // [P2SKIP - Port 2 Skip]$

  // $[P2MASK - Port 2 Mask]
  // [P2MASK - Port 2 Mask]$

  // $[P2MAT - Port 2 Match]
  // [P2MAT - Port 2 Match]$

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

//================================================================================
// ADC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void ADC_0_enter_DefaultMode_from_RESET(void) {
  // $[ADC0CN2 - ADC0 Control 2]
  /*
   // ADCM (Start of Conversion Mode Select) = TIMER2 (ADC0 conversion
   //     initiated on overflow of Timer 2.)
   // PACEN (Preserve Accumulator Enable) = PAC_DISABLED (The ADC
   //     accumulator is over-written with the results of any conversion (or set
   //     of conversions as specified by ADRPT).)
   */
  ADC0CN2 = ADC0CN2_ADCM__TIMER2 | ADC0CN2_PACEN__PAC_DISABLED;
  // [ADC0CN2 - ADC0 Control 2]$

  // $[ADC0CN1 - ADC0 Control 1]
  // [ADC0CN1 - ADC0 Control 1]$

  // $[ADC0MX - ADC0 Multiplexer Selection]
  /*
   // ADC0MX (AMUX0 Positive Input Selection) = ADC0P13 (Select ADC0.13.)
   */
  ADC0MX = ADC0MX_ADC0MX__ADC0P13;
  // [ADC0MX - ADC0 Multiplexer Selection]$

  // $[ADC0CF2 - ADC0 Power Control]
  /*
   // GNDSL (Analog Ground Reference) = GND_PIN (The ADC0 ground reference
   //     is the GND pin.)
   // REFSL (Voltage Reference Select) = INTERNAL_VREF (The ADC0 voltage
   //     reference is the internal voltage reference.)
   // ADPWR (Power Up Delay Time) = 0x03
   */
  ADC0CF2 = ADC0CF2_GNDSL__GND_PIN | ADC0CF2_REFSL__INTERNAL_VREF
      | (0x03 << ADC0CF2_ADPWR__SHIFT);
  // [ADC0CF2 - ADC0 Power Control]$

  // $[ADC0CF0 - ADC0 Configuration]
  /*
   // ADCLKSEL (ADC Clock Select) = HFOSC0 (ADCCLK = HFOSC0.)
   // ADSC (SAR Clock Divider) = 0x01
   */
  ADC0CF0 = ADC0CF0_ADCLKSEL__HFOSC0 | (0x01 << ADC0CF0_ADSC__SHIFT);
  // [ADC0CF0 - ADC0 Configuration]$

  // $[ADC0CF1 - ADC0 Configuration]
  /*
   // ADLPM (Low Power Mode Enable) = LP_ENABLED (Enable low power mode.)
   // ADTK (Conversion Tracking Time) = 0x17
   */
  ADC0CF1 =  (0x17 << ADC0CF1_ADTK__SHIFT);
  // [ADC0CF1 - ADC0 Configuration]$

  // $[ADC0ASAL - ADC0 Autoscan Start Address Low Byte]
  // [ADC0ASAL - ADC0 Autoscan Start Address Low Byte]$

  // $[ADC0GTH - ADC0 Greater-Than High Byte]
  /*
   // ADC0GTH (Greater-Than High Byte) = 0x00
   */
  ADC0GTH = (0x00 << ADC0GTH_ADC0GTH__SHIFT);
  // [ADC0GTH - ADC0 Greater-Than High Byte]$

  // $[ADC0GTL - ADC0 Greater-Than Low Byte]
  /*
   // ADC0GTL (Greater-Than Low Byte) = 0x00
   */
  ADC0GTL = (0x00 << ADC0GTL_ADC0GTL__SHIFT);
  // [ADC0GTL - ADC0 Greater-Than Low Byte]$

  // $[ADC0LTH - ADC0 Less-Than High Byte]
  /*
   // ADC0LTH (Less-Than High Byte) = 0xFF
   */
  ADC0LTH = (0xFF << ADC0LTH_ADC0LTH__SHIFT);
  // [ADC0LTH - ADC0 Less-Than High Byte]$

  // $[ADC0LTL - ADC0 Less-Than Low Byte]
  /*
   // ADC0LTL (Less-Than Low Byte) = 0xFF
   */
  ADC0LTL = (0xFF << ADC0LTL_ADC0LTL__SHIFT);
  // [ADC0LTL - ADC0 Less-Than Low Byte]$

  // $[ADC0ASCF - ADC0 Autoscan Configuration]
  // [ADC0ASCF - ADC0 Autoscan Configuration]$

  // $[ADC0CN0 - ADC0 Control 0]
  /*
   // ADEN (ADC Enable) = ENABLED (Enable ADC0 (active and ready for data
   //     conversions).)
   // ADGN (Gain Control) = GAIN_0P5 (The on-chip PGA gain is 0.5.)
   // IPOEN (Idle Powered-off Enable) = POWER_DOWN (Power down when ADC is
   //     idle (not converting).)
   */
  ADC0CN0 &= ~ADC0CN0_ADGN__FMASK;
  ADC0CN0 |= ADC0CN0_ADEN__ENABLED | ADC0CN0_ADGN__GAIN_0P5
      | ADC0CN0_IPOEN__POWER_DOWN;
  // [ADC0CN0 - ADC0 Control 0]$

}

//================================================================================
// CLOCK_0_enter_DefaultMode_from_RESET
//================================================================================
extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
  // $[HFOSC1 Setup]
  // [HFOSC1 Setup]$

  // $[CLKSEL - Clock Select]
  /*
   // CLKSL (Clock Source Select) = HFOSC0 (Clock derived from the Internal
   //     High Frequency Oscillator 0.)
   // CLKDIV (Clock Source Divider) = SYSCLK_DIV_32 (SYSCLK is equal to
   //     selected clock source divided by 32.)
   */
  CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_32;
  while (CLKSEL & CLKSEL_DIVRDY__BMASK == CLKSEL_DIVRDY__NOT_READY)
    ;
  // [CLKSEL - Clock Select]$

}

//================================================================================
// TIMER01_0_enter_DefaultMode_from_RESET
//================================================================================
extern void TIMER01_0_enter_DefaultMode_from_RESET(void) {
  // $[Timer Initialization]
  //Save Timer Configuration
  uint8_t TCON_save;
  TCON_save = TCON;
  //Stop Timers
  TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;

  // [Timer Initialization]$

  // $[TH0 - Timer 0 High Byte]
  // [TH0 - Timer 0 High Byte]$

  // $[TL0 - Timer 0 Low Byte]
  // [TL0 - Timer 0 Low Byte]$

  // $[TH1 - Timer 1 High Byte]
  // [TH1 - Timer 1 High Byte]$

  // $[TL1 - Timer 1 Low Byte]
  // [TL1 - Timer 1 Low Byte]$

  // $[Timer Restoration]
  //Restore Timer Configuration
  TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

  // [Timer Restoration]$

}

//================================================================================
// TIMER16_2_enter_DefaultMode_from_RESET
//================================================================================
extern void TIMER16_2_enter_DefaultMode_from_RESET(void) {
  // $[Timer Initialization]
  // Save Timer Configuration
  uint8_t TMR2CN0_TR2_save;
  TMR2CN0_TR2_save = TMR2CN0 & TMR2CN0_TR2__BMASK;
  // Stop Timer
  TMR2CN0 &= ~(TMR2CN0_TR2__BMASK);
  // [Timer Initialization]$

  // $[TMR2CN1 - Timer 2 Control 1]
  // [TMR2CN1 - Timer 2 Control 1]$

  // $[TMR2CN0 - Timer 2 Control]
  // [TMR2CN0 - Timer 2 Control]$

  // $[TMR2H - Timer 2 High Byte]
  /*
   // TMR2H (Timer 2 High Byte) = 0xFF
   */
  TMR2H = (0xFF << TMR2H_TMR2H__SHIFT);
  // [TMR2H - Timer 2 High Byte]$

  // $[TMR2L - Timer 2 Low Byte]
  /*
   // TMR2L (Timer 2 Low Byte) = 0xFF
   */
  TMR2L = (0xFF << TMR2L_TMR2L__SHIFT);
  // [TMR2L - Timer 2 Low Byte]$

  // $[TMR2RLH - Timer 2 Reload High Byte]
  /*
   // TMR2RLH (Timer 2 Reload High Byte) = 0xFD
   */
  TMR2RLH = (0xFD << TMR2RLH_TMR2RLH__SHIFT);
  // [TMR2RLH - Timer 2 Reload High Byte]$

  // $[TMR2RLL - Timer 2 Reload Low Byte]
  /*
   // TMR2RLL (Timer 2 Reload Low Byte) = 0x82
   */
  TMR2RLL = (0x82 << TMR2RLL_TMR2RLL__SHIFT);
  // [TMR2RLL - Timer 2 Reload Low Byte]$

  // $[TMR2CN0]
  /*
   // TR2 (Timer 2 Run Control) = RUN (Start Timer 2 running.)
   */
  TMR2CN0 |= TMR2CN0_TR2__RUN;
  // [TMR2CN0]$

  // $[Timer Restoration]
  // Restore Timer Configuration
  TMR2CN0 |= TMR2CN0_TR2_save;
  // [Timer Restoration]$

}

//================================================================================
// INTERRUPT_0_enter_DefaultMode_from_RESET
//================================================================================
extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void) {
  // $[EIE1 - Extended Interrupt Enable 1]
  /*
   // EADC0 (ADC0 Conversion Complete Interrupt Enable) = DISABLED (Disable
   //     ADC0 Conversion Complete interrupt.)
   // EWADC0 (ADC0 Window Comparison Interrupt Enable) = ENABLED (Enable
   //     interrupt requests generated by ADC0 Window Compare flag (ADWINT).)
   // ECP0 (Comparator0 (CP0) Interrupt Enable) = DISABLED (Disable CP0
   //     interrupts.)
   // ECP1 (Comparator1 (CP1) Interrupt Enable) = DISABLED (Disable CP1
   //     interrupts.)
   // EMAT (Port Match Interrupts Enable) = DISABLED (Disable all Port Match
   //     interrupts.)
   // EPCA0 (Programmable Counter Array (PCA0) Interrupt Enable) = DISABLED
   //     (Disable all PCA0 interrupts.)
   // ESMB0 (SMBus (SMB0) Interrupt Enable) = DISABLED (Disable all SMB0
   //     interrupts.)
   // ET3 (Timer 3 Interrupt Enable) = DISABLED (Disable Timer 3
   //     interrupts.)
   */
  EIE1 = EIE1_EADC0__DISABLED | EIE1_EWADC0__ENABLED | EIE1_ECP0__DISABLED
      | EIE1_ECP1__DISABLED | EIE1_EMAT__DISABLED | EIE1_EPCA0__DISABLED
      | EIE1_ESMB0__DISABLED | EIE1_ET3__DISABLED;
  // [EIE1 - Extended Interrupt Enable 1]$

  // $[EIE2 - Extended Interrupt Enable 2]
  // [EIE2 - Extended Interrupt Enable 2]$

  // $[EIP1H - Extended Interrupt Priority 1 High]
  // [EIP1H - Extended Interrupt Priority 1 High]$

  // $[EIP1 - Extended Interrupt Priority 1 Low]
  // [EIP1 - Extended Interrupt Priority 1 Low]$

  // $[EIP2 - Extended Interrupt Priority 2]
  // [EIP2 - Extended Interrupt Priority 2]$

  // $[EIP2H - Extended Interrupt Priority 2 High]
  // [EIP2H - Extended Interrupt Priority 2 High]$

  // $[IE - Interrupt Enable]
  /*
   // EA (All Interrupts Enable) = ENABLED (Enable each interrupt according
   //     to its individual mask setting.)
   // EX0 (External Interrupt 0 Enable) = DISABLED (Disable external
   //     interrupt 0.)
   // EX1 (External Interrupt 1 Enable) = DISABLED (Disable external
   //     interrupt 1.)
   // ESPI0 (SPI0 Interrupt Enable) = DISABLED (Disable all SPI0
   //     interrupts.)
   // ET0 (Timer 0 Interrupt Enable) = DISABLED (Disable all Timer 0
   //     interrupt.)
   // ET1 (Timer 1 Interrupt Enable) = DISABLED (Disable all Timer 1
   //     interrupt.)
   // ET2 (Timer 2 Interrupt Enable) = DISABLED (Disable Timer 2 interrupt.)
   // ES0 (UART0 Interrupt Enable) = DISABLED (Disable UART0 interrupt.)
   */
  IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__DISABLED
      | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__DISABLED
      | IE_ES0__DISABLED;
  // [IE - Interrupt Enable]$

  // $[IP - Interrupt Priority]
  // [IP - Interrupt Priority]$

  // $[IPH - Interrupt Priority High]
  // [IPH - Interrupt Priority High]$

}

