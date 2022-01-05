//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8LB1_Register_Enums.h>
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
  DAC_0_enter_DefaultMode_from_RESET();
  DAC_1_enter_DefaultMode_from_RESET();
  DAC_2_enter_DefaultMode_from_RESET();
  DAC_3_enter_DefaultMode_from_RESET();
  CIP51_0_enter_DefaultMode_from_RESET();
  CLOCK_0_enter_DefaultMode_from_RESET();
  TIMER16_4_enter_DefaultMode_from_RESET();
  TIMER_SETUP_0_enter_DefaultMode_from_RESET();
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
      | P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__PUSH_PULL
      | P1MDOUT_B5__OPEN_DRAIN | P1MDOUT_B6__OPEN_DRAIN
      | P1MDOUT_B7__OPEN_DRAIN;
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
   // B4 (Port 1 Bit 4 Skip) = NOT_SKIPPED (P1.4 pin is not skipped by the
   //     crossbar.)
   // B5 (Port 1 Bit 5 Skip) = NOT_SKIPPED (P1.5 pin is not skipped by the
   //     crossbar.)
   // B6 (Port 1 Bit 6 Skip) = NOT_SKIPPED (P1.6 pin is not skipped by the
   //     crossbar.)
   // B7 (Port 1 Bit 7 Skip) = NOT_SKIPPED (P1.7 pin is not skipped by the
   //     crossbar.)
   */
  P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED
      | P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__NOT_SKIPPED
      | P1SKIP_B5__NOT_SKIPPED | P1SKIP_B6__NOT_SKIPPED
      | P1SKIP_B7__NOT_SKIPPED;
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
      | P3MDOUT_B2__OPEN_DRAIN | P3MDOUT_B3__OPEN_DRAIN | P3MDOUT_B4__PUSH_PULL
      | P3MDOUT_B7__OPEN_DRAIN;
  // [P3MDOUT - Port 3 Output Mode]$

  // $[P3MDIN - Port 3 Input Mode]
  /*
   // B0 (Port 3 Bit 0 Input Mode) = ANALOG (P3.0 pin is configured for
   //     analog mode.)
   // B1 (Port 3 Bit 1 Input Mode) = ANALOG (P3.1 pin is configured for
   //     analog mode.)
   // B2 (Port 3 Bit 2 Input Mode) = ANALOG (P3.2 pin is configured for
   //     analog mode.)
   // B3 (Port 3 Bit 3 Input Mode) = ANALOG (P3.3 pin is configured for
   //     analog mode.)
   // B4 (Port 3 Bit 4 Input Mode) = DIGITAL (P3.4 pin is configured for
   //     digital mode.)
   // B7 (Port 3 Bit 7 Input Mode) = DIGITAL (P3.7 pin is configured for
   //     digital mode.)
   */
  P3MDIN = P3MDIN_B0__ANALOG | P3MDIN_B1__ANALOG | P3MDIN_B2__ANALOG
      | P3MDIN_B3__ANALOG | P3MDIN_B4__DIGITAL | P3MDIN_B7__DIGITAL;
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

//================================================================================
// DAC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void DAC_0_enter_DefaultMode_from_RESET(void) {
  // $[DAC0CF0 - DAC0 Configuration 0]
  /*
   // EN (DAC0 Enable) = ENABLE (DAC0 is enabled and will drive the output
   //     pin.)
   // UPDATE (DAC0 Update Trigger Source) = TIMER4 (DAC0 output updates
   //     occur on Timer 4 high byte overflow.)
   // LJST (DAC0 Left Justify Enable) = RIGHT_JUSTIFY (DAC0 input is treated
   //     as right-justified.)
   // RSTMD (DAC0 Reset Mode) = NORMAL (All resets will reset DAC0 and its
   //     associated registers.)
   */
  SFRPAGE = 0x30;
  DAC0CF0 = DAC0CF0_EN__ENABLE | DAC0CF0_UPDATE__TIMER4
      | DAC0CF0_LJST__RIGHT_JUSTIFY | DAC0CF0_RSTMD__NORMAL;
  // [DAC0CF0 - DAC0 Configuration 0]$

  // $[DAC0CF1 - DAC0 Configuration 1]
  // [DAC0CF1 - DAC0 Configuration 1]$

}

//================================================================================
// DAC_1_enter_DefaultMode_from_RESET
//================================================================================
extern void DAC_1_enter_DefaultMode_from_RESET(void) {
  // $[DAC1CF0 - DAC1 Configuration 0]
  /*
   // EN (DAC1 Enable) = ENABLE (DAC1 is enabled and will drive the output
   //     pin.)
   // UPDATE (DAC1 Update Trigger Source) = TIMER4 (DAC1 output updates
   //     occur on Timer 4 high byte overflow.)
   // LJST (DAC1 Left Justify Enable) = RIGHT_JUSTIFY (DAC1 input is treated
   //     as right-justified.)
   // RSTMD (DAC1 Reset Mode) = NORMAL (All resets will reset DAC1 and its
   //     associated registers.)
   */
  DAC1CF0 = DAC1CF0_EN__ENABLE | DAC1CF0_UPDATE__TIMER4
      | DAC1CF0_LJST__RIGHT_JUSTIFY | DAC1CF0_RSTMD__NORMAL;
  // [DAC1CF0 - DAC1 Configuration 0]$

  // $[DAC1CF1 - DAC1 Configuration 1]
  // [DAC1CF1 - DAC1 Configuration 1]$

}

//================================================================================
// DAC_2_enter_DefaultMode_from_RESET
//================================================================================
extern void DAC_2_enter_DefaultMode_from_RESET(void) {
  // $[DAC2CF0 - DAC2 Configuration 0]
  /*
   // EN (DAC2 Enable) = ENABLE (DAC2 is enabled and will drive the output
   //     pin.)
   // UPDATE (DAC2 Update Trigger Source) = TIMER4 (DAC2 output updates
   //     occur on Timer 4 high byte overflow.)
   // LJST (DAC2 Left Justify Enable) = RIGHT_JUSTIFY (DAC2 input is treated
   //     as right-justified.)
   // RSTMD (DAC2 Reset Mode) = NORMAL (All resets will reset DAC2 and its
   //     associated registers.)
   */
  DAC2CF0 = DAC2CF0_EN__ENABLE | DAC2CF0_UPDATE__TIMER4
      | DAC2CF0_LJST__RIGHT_JUSTIFY | DAC2CF0_RSTMD__NORMAL;
  // [DAC2CF0 - DAC2 Configuration 0]$

  // $[DAC2CF1 - DAC2 Configuration 1]
  // [DAC2CF1 - DAC2 Configuration 1]$

}

//================================================================================
// DAC_3_enter_DefaultMode_from_RESET
//================================================================================
extern void DAC_3_enter_DefaultMode_from_RESET(void) {
  // $[DAC3CF0 - DAC3 Configuration 0]
  /*
   // EN (DAC3 Enable) = ENABLE (DAC3 is enabled and will drive the output
   //     pin.)
   // UPDATE (DAC3 Update Trigger Source) = TIMER4 (DAC3 output updates
   //     occur on Timer 4 high byte overflow.)
   // LJST (DAC3 Left Justify Enable) = RIGHT_JUSTIFY (DAC3 input is treated
   //     as right-justified.)
   // RSTMD (DAC3 Reset Mode) = NORMAL (All resets will reset DAC3 and its
   //     associated registers.)
   */
  DAC3CF0 = DAC3CF0_EN__ENABLE | DAC3CF0_UPDATE__TIMER4
      | DAC3CF0_LJST__RIGHT_JUSTIFY | DAC3CF0_RSTMD__NORMAL;
  // [DAC3CF0 - DAC3 Configuration 0]$

  // $[DAC3CF1 - DAC3 Configuration 1]
  // [DAC3CF1 - DAC3 Configuration 1]$

}

//================================================================================
// DACGCF_0_enter_DefaultMode_from_RESET
//================================================================================
extern void DACGCF_0_enter_DefaultMode_from_RESET(void) {
  // $[DACGCF0 - DAC Global Configuration 0]
  /*
   // D01REFSL (DAC0 and DAC1 Reference Voltage Select) = VDD (Select the
   //     VDD supply.)
   // D23REFSL (DAC2 and DAC3 Reference Voltage Select) = VDD (Select the
   //     VDD supply.)
   // D1SRC (DAC1 Data Source) = DAC0_INVERT (Input = Inverse of DAC0H:DAC0L
   //     (one's complement).)
   // D3SRC (DAC3 Data Source) = DAC2_INVERT (Input = Inverse of DAC2H:DAC2L
   //     (one's complement).)
   // D1AMEN (DAC1 Alternating Mode Enable) = NORMAL (DAC1 always updates
   //     from the data source selected in D1SRC. This mode may be used with any
   //     trigger source.)
   // D3AMEN (DAC3 Alternating Mode Enable) = NORMAL (DAC3 always updates
   //     from the data source selected in D3SRC. This mode may be used with any
   //     trigger source.)
   */
  DACGCF0 = DACGCF0_D01REFSL__VDD | DACGCF0_D23REFSL__VDD
      | DACGCF0_D1SRC__DAC0_INVERT | DACGCF0_D3SRC__DAC2_INVERT
      | DACGCF0_D1AMEN__NORMAL | DACGCF0_D3AMEN__NORMAL;
  // [DACGCF0 - DAC Global Configuration 0]$

  // $[DACGCF2 - DAC Global Configuration 2]
  // [DACGCF2 - DAC Global Configuration 2]$

}

//================================================================================
// VREF_0_enter_DefaultMode_from_RESET
//================================================================================
extern void VREF_0_enter_DefaultMode_from_RESET(void) {
  // $[REF0CN - Voltage Reference Control]
  /*
   // VREFSL (Voltage Reference Output Select) = VREF_2P4 (2.4 V reference
   //     output to VREF pin.)
   */
  SFRPAGE = 0x00;
  REF0CN = REF0CN_VREFSL__VREF_2P4;
  // [REF0CN - Voltage Reference Control]$

}

//================================================================================
// CLOCK_0_enter_DefaultMode_from_RESET
//================================================================================
extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
  // $[HFOSC1 Setup]
  // Ensure SYSCLK is > 24 MHz before switching to HFOSC1
  SFRPAGE = 0x00;
  CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  while (CLKSEL & CLKSEL_DIVRDY__BMASK == CLKSEL_DIVRDY__NOT_READY)
    ;
  // [HFOSC1 Setup]$

  // $[CLKSEL - Clock Select]
  /*
   // CLKSL (Clock Source Select) = HFOSC1 (Clock derived from the Internal
   //     High Frequency Oscillator 1.)
   // CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
   //     selected clock source divided by 1.)
   // CLKSL (Clock Source Select) = HFOSC1 (Clock derived from the Internal
   //     High Frequency Oscillator 1.)
   // CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
   //     selected clock source divided by 1.)
   */
  CLKSEL = CLKSEL_CLKSL__HFOSC1 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  CLKSEL = CLKSEL_CLKSL__HFOSC1 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  while (CLKSEL & CLKSEL_DIVRDY__BMASK == CLKSEL_DIVRDY__NOT_READY)
    ;
  // [CLKSEL - Clock Select]$

}

extern void TIMER16_3_enter_DefaultMode_from_RESET(void) {
  // $[Timer Initialization]
  // Save Timer Configuration
  uint8_t TMR3CN0_TR3_save;
  TMR3CN0_TR3_save = TMR3CN0 & TMR3CN0_TR3__BMASK;
  // Stop Timer
  TMR3CN0 &= ~(TMR3CN0_TR3__BMASK);
  // [Timer Initialization]$

  // $[TMR3CN1 - Timer 3 Control 1]
  // [TMR3CN1 - Timer 3 Control 1]$

  // $[TMR3CN0 - Timer 3 Control]
  // [TMR3CN0 - Timer 3 Control]$

  // $[TMR3H - Timer 3 High Byte]
  /*
   // TMR3H (Timer 3 High Byte) = 0xFF
   */
  TMR3H = (0xFF << TMR3H_TMR3H__SHIFT);
  // [TMR3H - Timer 3 High Byte]$

  // $[TMR3L - Timer 3 Low Byte]
  /*
   // TMR3L (Timer 3 Low Byte) = 0xA0
   */
  TMR3L = (0xA0 << TMR3L_TMR3L__SHIFT);
  // [TMR3L - Timer 3 Low Byte]$

  // $[TMR3RLH - Timer 3 Reload High Byte]
  /*
   // TMR3RLH (Timer 3 Reload High Byte) = 0xF5
   */
  TMR3RLH = (0xF5 << TMR3RLH_TMR3RLH__SHIFT);
  // [TMR3RLH - Timer 3 Reload High Byte]$

  // $[TMR3RLL - Timer 3 Reload Low Byte]
  /*
   // TMR3RLL (Timer 3 Reload Low Byte) = 0x04
   */
  TMR3RLL = (0x04 << TMR3RLL_TMR3RLL__SHIFT);
  // [TMR3RLL - Timer 3 Reload Low Byte]$

  // $[TMR3CN0]
  /*
   // TR3 (Timer 3 Run Control) = RUN (Start Timer 3 running.)
   */
  TMR3CN0 |= TMR3CN0_TR3__RUN;
  // [TMR3CN0]$

  // $[Timer Restoration]
  // Restore Timer Configuration
  TMR3CN0 |= TMR3CN0_TR3_save;
  // [Timer Restoration]$

}

extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void) {
  // $[CKCON0 - Clock Control 0]
  // [CKCON0 - Clock Control 0]$

  // $[CKCON1 - Clock Control 1]
  /*
   // T4MH (Timer 4 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 4 high
   //     byte uses the clock defined by T4XCLK in TMR4CN0.)
   // T4ML (Timer 4 Low Byte Clock Select) = SYSCLK (Timer 4 low byte uses
   //     the system clock.)
   // T5MH (Timer 5 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 5 high
   //     byte uses the clock defined by T5XCLK in TMR5CN0.)
   // T5ML (Timer 5 Low Byte Clock Select) = EXTERNAL_CLOCK (Timer 5 low
   //     byte uses the clock defined by T5XCLK in TMR5CN0.)
   */
  CKCON1 = CKCON1_T4MH__EXTERNAL_CLOCK | CKCON1_T4ML__SYSCLK
      | CKCON1_T5MH__EXTERNAL_CLOCK | CKCON1_T5ML__EXTERNAL_CLOCK;
  // [CKCON1 - Clock Control 1]$

  // $[TMOD - Timer 0/1 Mode]
  // [TMOD - Timer 0/1 Mode]$

  // $[TCON - Timer 0/1 Control]
  // [TCON - Timer 0/1 Control]$

}

extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void) {
  // $[EIE1 - Extended Interrupt Enable 1]
  // [EIE1 - Extended Interrupt Enable 1]$

  // $[EIE2 - Extended Interrupt Enable 2]
  /*
   // CL0 (Configurable Logic (CL0) Interrupt Enable) = DISABLED (Disable
   //     CL0 interrupts.)
   // EI2C0 (I2C0 Slave Interrupt Enable) = DISABLED (Disable all I2C0 slave
   //     interrupts.)
   // ET4 (Timer 4 Interrupt Enable) = ENABLED (Enable interrupt requests
   //     generated by the TF4L or TF4H flags.)
   // ET5 (Timer 5 Interrupt Enable) = DISABLED (Disable Timer 5
   //     interrupts.)
   // ES1 (UART1 Interrupt Enable) = DISABLED (Disable UART1 interrupts.)
   */
  SFRPAGE = 0x00;
  EIE2 = EIE2_CL0__DISABLED | EIE2_EI2C0__DISABLED | EIE2_ET4__ENABLED
      | EIE2_ET5__DISABLED | EIE2_ES1__DISABLED;
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

extern void CIP51_0_enter_DefaultMode_from_RESET(void) {
  // $[PFE0CN - Prefetch Engine Control]
  /*
   // FLRT (Flash Read Timing) = SYSCLK_BELOW_75_MHZ (SYSCLK < 75 MHz.)
   */
  SFRPAGE = 0x10;
  PFE0CN = PFE0CN_FLRT__SYSCLK_BELOW_75_MHZ;
  // [PFE0CN - Prefetch Engine Control]$

}

extern void TIMER16_4_enter_DefaultMode_from_RESET(void) {
  // $[Timer Initialization]
  // Save Timer Configuration
  uint8_t TMR4CN0_TR4_save;
  SFRPAGE = 0x10;
  TMR4CN0_TR4_save = TMR4CN0 & TMR4CN0_TR4__BMASK;
  // Stop Timer
  TMR4CN0 &= ~(TMR4CN0_TR4__BMASK);
  // [Timer Initialization]$

  // $[TMR4CN1 - Timer 4 Control 1]
  // [TMR4CN1 - Timer 4 Control 1]$

  // $[TMR4CN0 - Timer 4 Control]
  // [TMR4CN0 - Timer 4 Control]$

  // $[TMR4H - Timer 4 High Byte]
  /*
   // TMR4H (Timer 4 High Byte) = 0xF9
   */
  TMR4H = (0xF9 << TMR4H_TMR4H__SHIFT);
  // [TMR4H - Timer 4 High Byte]$

  // $[TMR4L - Timer 4 Low Byte]
  /*
   // TMR4L (Timer 4 Low Byte) = 0x9C
   */
  TMR4L = (0x9C << TMR4L_TMR4L__SHIFT);
  // [TMR4L - Timer 4 Low Byte]$

  // $[TMR4RLH - Timer 4 Reload High Byte]
  /*
   // TMR4RLH (Timer 4 Reload High Byte) = 0xF9
   */
  TMR4RLH = (0xF9 << TMR4RLH_TMR4RLH__SHIFT);
  // [TMR4RLH - Timer 4 Reload High Byte]$

  // $[TMR4RLL - Timer 4 Reload Low Byte]
  /*
   // TMR4RLL (Timer 4 Reload Low Byte) = 0x9C
   */
  TMR4RLL = (0x9C << TMR4RLL_TMR4RLL__SHIFT);
  // [TMR4RLL - Timer 4 Reload Low Byte]$

  // $[TMR4CN0]
  /*
   // TR4 (Timer 4 Run Control) = RUN (Start Timer 4 running.)
   */
  TMR4CN0 |= TMR4CN0_TR4__RUN;
  // [TMR4CN0]$

  // $[Timer Restoration]
  // Restore Timer Configuration
  TMR4CN0 |= TMR4CN0_TR4_save;
  // [Timer Restoration]$

}

