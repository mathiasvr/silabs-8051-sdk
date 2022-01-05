//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB51_Register_Enums.h>
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
  CLOCK_0_enter_DefaultMode_from_RESET();
  SPI_0_enter_DefaultMode_from_RESET();
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
   // B4 (Port 0 Bit 4 Output Mode) = OPEN_DRAIN (P0.4 output is open-
   //     drain.)
   // B5 (Port 0 Bit 5 Output Mode) = OPEN_DRAIN (P0.5 output is open-
   //     drain.)
   // B6 (Port 0 Bit 6 Output Mode) = OPEN_DRAIN (P0.6 output is open-
   //     drain.)
   // B7 (Port 0 Bit 7 Output Mode) = PUSH_PULL (P0.7 output is push-pull.)
   */
  P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN
      | P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__OPEN_DRAIN | P0MDOUT_B4__OPEN_DRAIN
      | P0MDOUT_B5__OPEN_DRAIN | P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__PUSH_PULL;
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
   // B0 (Port 0 Bit 0 Skip) = SKIPPED (P0.0 pin is skipped by the
   //     crossbar.)
   // B1 (Port 0 Bit 1 Skip) = SKIPPED (P0.1 pin is skipped by the
   //     crossbar.)
   // B2 (Port 0 Bit 2 Skip) = SKIPPED (P0.2 pin is skipped by the
   //     crossbar.)
   // B3 (Port 0 Bit 3 Skip) = SKIPPED (P0.3 pin is skipped by the
   //     crossbar.)
   // B4 (Port 0 Bit 4 Skip) = SKIPPED (P0.4 pin is skipped by the
   //     crossbar.)
   // B5 (Port 0 Bit 5 Skip) = SKIPPED (P0.5 pin is skipped by the
   //     crossbar.)
   // B6 (Port 0 Bit 6 Skip) = NOT_SKIPPED (P0.6 pin is not skipped by the
   //     crossbar.)
   // B7 (Port 0 Bit 7 Skip) = NOT_SKIPPED (P0.7 pin is not skipped by the
   //     crossbar.)
   */
  P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED
      | P0SKIP_B3__SKIPPED | P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
      | P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__NOT_SKIPPED;
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
   */
  P1 = P1_B0__HIGH | P1_B1__HIGH | P1_B2__HIGH | P1_B3__HIGH | P1_B4__HIGH
      | P1_B5__HIGH | P1_B6__HIGH;
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
   // B6 (Port 1 Bit 6 Output Mode) = OPEN_DRAIN (P1.6 output is open-
   //     drain.)
   */
  P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
      | P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__PUSH_PULL
      | P1MDOUT_B5__PUSH_PULL | P1MDOUT_B6__OPEN_DRAIN;
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
   */
  P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__DIGITAL | P1MDIN_B2__DIGITAL
      | P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__DIGITAL
      | P1MDIN_B6__DIGITAL;
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
   */
  P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED
      | P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__NOT_SKIPPED
      | P1SKIP_B5__NOT_SKIPPED | P1SKIP_B6__NOT_SKIPPED;
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
   */
  P1MASK = P1MASK_B0__IGNORED | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
      | P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
      | P1MASK_B6__IGNORED;
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
   */
  P1MAT = P1MAT_B0__HIGH | P1MAT_B1__HIGH | P1MAT_B2__HIGH | P1MAT_B3__HIGH
      | P1MAT_B4__HIGH | P1MAT_B5__HIGH | P1MAT_B6__HIGH;
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
   // B0 (Port 2 Bit 0 Output Mode) = OPEN_DRAIN (P2.0 output is open-
   //     drain.)
   */
  P2MDOUT = P2MDOUT_B0__OPEN_DRAIN;
  // [P2MDOUT - Port 2 Output Mode]$

  // $[P2MDIN - Port 2 Input Mode]
  // [P2MDIN - Port 2 Input Mode]$
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
  /*
   // URT0E (UART0 I/O Enable) = DISABLED (UART0 I/O unavailable at Port
   //     pin.)
   // SPI0E (SPI I/O Enable) = ENABLED (SPI I/O routed to Port pins. The SPI
   //     can be assigned either 3 or 4 GPIO pins.)
   // SMB0E (SMB0 I/O Enable) = DISABLED (SMBus 0 I/O unavailable at Port
   //     pins.)
   // CP0E (Comparator0 Output Enable) = DISABLED (CP0 unavailable at Port
   //     pin.)
   // CP0AE (Comparator0 Asynchronous Output Enable) = DISABLED
   //     (Asynchronous CP0 unavailable at Port pin.)
   // CP1E (Comparator1 Output Enable) = DISABLED (CP1 unavailable at Port
   //     pin.)
   // CP1AE (Comparator1 Asynchronous Output Enable) = DISABLED
   //     (Asynchronous CP1 unavailable at Port pin.)
   // SYSCKE (SYSCLK Output Enable) = DISABLED (SYSCLK unavailable at Port
   //     pin.)
   */
  XBR0 = XBR0_URT0E__DISABLED | XBR0_SPI0E__ENABLED | XBR0_SMB0E__DISABLED
      | XBR0_CP0E__DISABLED | XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
      | XBR0_CP1AE__DISABLED | XBR0_SYSCKE__DISABLED;
  // [XBR0 - Port I/O Crossbar 0]$

  // $[XBR1 - Port I/O Crossbar 1]
  // [XBR1 - Port I/O Crossbar 1]$

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
   // CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
   //     selected clock source divided by 1.)
   // CLKSL (Clock Source Select) = HFOSC0 (Clock derived from the Internal
   //     High Frequency Oscillator 0.)
   // CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
   //     selected clock source divided by 1.)
   */
  CLKSEL = CLKSEL_CLKSL__HFOSC0_clk24p5 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  CLKSEL = CLKSEL_CLKSL__HFOSC0_clk24p5 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  while (CLKSEL & CLKSEL_DIVRDY__BMASK == CLKSEL_DIVRDY__NOT_READY);
    ;
  // [CLKSEL - Clock Select]$

}

//================================================================================
// SPI_0_enter_DefaultMode_from_RESET
//================================================================================
extern void SPI_0_enter_DefaultMode_from_RESET(void) {
  // $[SPI0CKR - SPI0 Clock Rate]
  /*
   // SPI0CKR (SPI0 Clock Rate) = 0x17
   */
  SPI0CKR = (0x17 << SPI0CKR_SPI0CKR__SHIFT);
  // [SPI0CKR - SPI0 Clock Rate]$

  // $[SPI0CFG - SPI0 Configuration]
  // [SPI0CFG - SPI0 Configuration]$

  // $[SPI0PCF - SPI0 Pin Configuration]
  // [SPI0PCF - SPI0 Pin Configuration]$

  // $[SPI0CN0 - SPI0 Control]
  /*
   // SPIEN (SPI0 Enable) = ENABLED (Enable the SPI module.)
   */
  SPI0CN0 |= SPI0CN0_SPIEN__ENABLED;
  // [SPI0CN0 - SPI0 Control]$

}

//================================================================================
// INTERRUPT_0_enter_DefaultMode_from_RESET
//================================================================================
extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void) {
  // $[EIE1 - Extended Interrupt Enable 1]
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
   // EA (All Interrupts Enable) = DISABLED (Disable all interrupt sources.)
   // EX0 (External Interrupt 0 Enable) = DISABLED (Disable external
   //     interrupt 0.)
   // EX1 (External Interrupt 1 Enable) = DISABLED (Disable external
   //     interrupt 1.)
   // ESPI0 (SPI0 Interrupt Enable) = ENABLED (Enable interrupt requests
   //     generated by SPI0.)
   // ET0 (Timer 0 Interrupt Enable) = DISABLED (Disable all Timer 0
   //     interrupt.)
   // ET1 (Timer 1 Interrupt Enable) = DISABLED (Disable all Timer 1
   //     interrupt.)
   // ET2 (Timer 2 Interrupt Enable) = DISABLED (Disable Timer 2 interrupt.)
   // ES0 (UART0 Interrupt Enable) = DISABLED (Disable UART0 interrupt.)
   */
  IE = IE_EA__DISABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__ENABLED
      | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__DISABLED
      | IE_ES0__DISABLED;
  // [IE - Interrupt Enable]$

  // $[IP - Interrupt Priority]
  // [IP - Interrupt Priority]$

  // $[IPH - Interrupt Priority High]
  // [IPH - Interrupt Priority High]$

}

