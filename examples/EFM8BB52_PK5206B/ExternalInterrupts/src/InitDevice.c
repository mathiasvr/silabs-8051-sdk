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
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	RSTSRC_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	TIMER_SETUP_0_enter_DefaultMode_from_RESET();
	EXTINT_0_enter_DefaultMode_from_RESET();
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
   // B7 (Port 0 Bit 7 Output Mode) = OPEN_DRAIN (P0.7 output is open-
   //     drain.)
   */
  P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN
      | P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__OPEN_DRAIN
      | P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN
      | P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__OPEN_DRAIN;
  // [P0MDOUT - Port 0 Output Mode]$

  // $[P0MDIN - Port 0 Input Mode]
  /*
   // P0.0 pin is configured for digital mode
   // P0.1 pin is configured for digital mode
   // P0.2 pin is configured for digital mode
   // P0.3 pin is configured for digital mode
   // P0.4 pin is configured for digital mode
   // P0.5 pin is configured for digital mode
   // P0.6 pin is configured for digital mode
   // P0.7 pin is configured for digital mode
   */
  P0MDIN = P0MDIN_B0__DIGITAL | P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL
      | P0MDIN_B3__DIGITAL | P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL
      | P0MDIN_B6__DIGITAL | P0MDIN_B7__DIGITAL;
  // [P0MDIN - Port 0 Input Mode]$

}

//================================================================================
// PORTS_1_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_1_enter_DefaultMode_from_RESET(void) {
  // $[P1 - Port 1 Pin Latch]
  // [P1 - Port 1 Pin Latch]$

  // $[P1MDOUT - Port 1 Output Mode]
  /*
   // P1.0 output is open-drain
   // P1.1 output is open-drain
   // P1.2 output is open-drain
   // P1.3 output is open-drain
   // P1.4 output is open-drain
   // P1.5 output is push-pull
   // P1.6 output is push-pull
   // P1.7 output is open-drain
  */
  P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
      | P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__OPEN_DRAIN
      | P1MDOUT_B5__PUSH_PULL | P1MDOUT_B6__PUSH_PULL | P1MDOUT_B7__OPEN_DRAIN;
  // [P1MDOUT - Port 1 Output Mode]$

  // $[P1MDIN - Port 1 Input Mode]
  /*
   // P1.0 pin is configured for analog mode
   // P1.1 pin is configured for analog mode
   // P1.2 pin is configured for analog mode
   // P1.3 pin is configured for analog mode
   // P1.4 pin is configured for analog mode
   // P1.5 pin is configured for digital mode
   // P1.6 pin is configured for digital mode
   // P1.7 pin is configured for analog mode
  */
  P1MDIN = P1MDIN_B0__ANALOG | P1MDIN_B1__ANALOG | P1MDIN_B2__ANALOG
      | P1MDIN_B3__ANALOG | P1MDIN_B4__ANALOG | P1MDIN_B5__DIGITAL
      | P1MDIN_B6__DIGITAL | P1MDIN_B7__ANALOG;
  // [P1MDIN - Port 1 Input Mode]$

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

  // $[P1SKIP - Port 1 Skip]
  /*
   // P1.0 pin is skipped by the crossbar
   // P1.1 pin is skipped by the crossbar
   // P1.2 pin is skipped by the crossbar
   // P1.3 pin is skipped by the crossbar
   // P1.4 pin is skipped by the crossbar
   // P1.5 pin is not skipped by the crossbar
   // P1.6 pin is not skipped by the crossbar
   // P1.7 pin is skipped by the crossbar
  */
  P1SKIP = P1SKIP_B0__SKIPPED | P1SKIP_B1__SKIPPED | P1SKIP_B2__SKIPPED
      | P1SKIP_B3__SKIPPED | P1SKIP_B4__SKIPPED | P1SKIP_B5__NOT_SKIPPED
      | P1SKIP_B6__NOT_SKIPPED | P1SKIP_B7__SKIPPED;
  // [P1SKIP - Port 1 Skip]$

  // $[P1MASK - Port 1 Mask]
  // [P1MASK - Port 1 Mask]$

  // $[P1MAT - Port 1 Match]
  // [P1MAT - Port 1 Match]$

}

//================================================================================
// PBCFG_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PBCFG_0_enter_DefaultMode_from_RESET(void) {
	// $[XBR2 - Port I/O Crossbar 2]
  /*
   // Weak Pullups enabled
   // Crossbar enabled
  */
	XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;
	// [XBR2 - Port I/O Crossbar 2]$

	// $[PRTDRV - Port Drive Strength]
	// [PRTDRV - Port Drive Strength]$

	// $[XBR0 - Port I/O Crossbar 0]
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR1 - Port I/O Crossbar 1]
	// [XBR1 - Port I/O Crossbar 1]$

}

//================================================================================
// RSTSRC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void RSTSRC_0_enter_DefaultMode_from_RESET(void) {
	// $[RSTSRC - Reset Source]
	/*
	 // PORSF (Power-On / Supply Monitor Reset Flag, and Supply Monitor Reset
	 //     Enable) = NOT_SET (A power-on or supply monitor reset did not occur.)
	 // MCDRSF (Missing Clock Detector Enable and Flag) = SET (A missing clock
	 //     detector reset occurred.)
	 // C0RSEF (Comparator0 Reset Enable and Flag) = NOT_SET (A Comparator 0
	 //     reset did not occur.)
  */
	RSTSRC = RSTSRC_PORSF__NOT_SET | RSTSRC_MCDRSF__SET
			| RSTSRC_C0RSEF__NOT_SET;
	// [RSTSRC - Reset Source]$

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
	// [CLKSEL - Clock Select]$

}

//================================================================================
// TIMER_SETUP_0_enter_DefaultMode_from_RESET
//================================================================================
extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void) {
	// $[TCON - Timer 0/1 Control]
	/*
	 // IT0 (Interrupt 0 Type Select) = EDGE (INT0 is edge triggered.)
	 // IT1 (Interrupt 1 Type Select) = EDGE (INT1 is edge triggered.)
	 */
	TCON |= TCON_IT0__EDGE | TCON_IT1__EDGE;
	// [TCON - Timer 0/1 Control]$

}

//================================================================================
// EXTINT_0_enter_DefaultMode_from_RESET
//================================================================================
extern void EXTINT_0_enter_DefaultMode_from_RESET(void) {
	// $[IT01CF - INT0/INT1 Configuration]
	/*
	 // IN0PL (INT0 Polarity) = ACTIVE_LOW (INT0 input is active low.)
	 // IN0SL (INT0 Port Pin Selection) = P0_2 (Select P0.2.)
	 // IN1PL (INT1 Polarity) = ACTIVE_LOW (INT1 input is active low.)
	 // IN1SL (INT1 Port Pin Selection) = P0_3 (Select P0.3.)
	 */
	IT01CF = IT01CF_IN0PL__ACTIVE_LOW | IT01CF_IN0SL__P0_2
			| IT01CF_IN1PL__ACTIVE_LOW | IT01CF_IN1SL__P0_3;
	// [IT01CF - INT0/INT1 Configuration]$

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
	 // EX0 (External Interrupt 0 Enable) = ENABLED (Enable interrupt requests
	 //     generated by the INT0 input.)
	 // EX1 (External Interrupt 1 Enable) = ENABLED (Enable interrupt requests
	 //     generated by the INT1 input.)
	 // ESPI0 (SPI0 Interrupt Enable) = DISABLED (Disable all SPI0
	 //     interrupts.)
	 // ET0 (Timer 0 Interrupt Enable) = DISABLED (Disable all Timer 0
	 //     interrupt.)
	 // ET1 (Timer 1 Interrupt Enable) = DISABLED (Disable all Timer 1
	 //     interrupt.)
	 // ET2 (Timer 2 Interrupt Enable) = DISABLED (Disable Timer 2 interrupt.)
	 // ES0 (UART0 Interrupt Enable) = DISABLED (Disable UART0 interrupt.)
	 */
	IE = IE_EA__DISABLED | IE_EX0__ENABLED | IE_EX1__ENABLED
			| IE_ESPI0__DISABLED | IE_ET0__DISABLED | IE_ET1__DISABLED
			| IE_ET2__DISABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

	// $[IPH - Interrupt Priority High]
	// [IPH - Interrupt Priority High]$

}

