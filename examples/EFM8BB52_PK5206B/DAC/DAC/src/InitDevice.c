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
	PORTS_3_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	AMUXCP_0_enterDefaultMode_from_RESET();
	DAC_0_enter_DefaultMode_from_RESET();
	VREF_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
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
	P3MDIN = P3MDIN_B0__ANALOG;
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
// AMUXCP_0_enter_DefaultMode_from_RESET
//================================================================================
extern void AMUXCP_0_enterDefaultMode_from_RESET(void){
  uint8_t i;
  CLKGRP0 = CLKGRP0_EN_CPCLK__ENABLE;
  SFRPAGE = 0x30;
  CP0CN |= CP0CN_EN_CPLDO__ENABLED;
  for (i = 0; i < 200; i++); //us wait
  CP0CN |= CP0CN_EN_CP__ENABLED;
  while ((CP0CN & CP0CN_CP_STARTUP_DONE__BMASK) == CP0CN_CP_STARTUP_DONE__ONGOING);
}

//================================================================================
// DAC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void DAC_0_enter_DefaultMode_from_RESET(void) {
	// $[DAC0CF0 - DAC0 Configuration 0]
	/*
	 // EN (DAC0 Enable) = ENABLE (DAC0 is enabled and will drive the output
	 //     pin.)
	 // UPDATE (DAC0 Update Trigger Source) = TIMER3 (DAC0 output updates
	 //     occur on Timer 3 high byte overflow.)
	 // LJST (DAC0 Left Justify Enable) = RIGHT_JUSTIFY (DAC0 input is treated
	 //     as right-justified.)
	 // RSTMD (DAC0 Reset Mode) = NORMAL (All resets will reset DAC0 and its
	 //     associated registers.)
	 */
	SFRPAGE = 0x30;
	DAC0CF0 = DAC0CF0_EN__ENABLE | DAC0CF0_UPDATE__TIMER3
			| DAC0CF0_LJST__RIGHT_JUSTIFY | DAC0CF0_RSTMD__NORMAL;
	// [DAC0CF0 - DAC0 Configuration 0]$

	// $[DAC0CF1 - DAC0 Configuration 1]
	// [DAC0CF1 - DAC0 Configuration 1]$
	DAC0CF1 = DAC0CF1_SRC__DACALT | DAC0CF1_AMEN__FALSE | DAC0CF1_VREFSEL__VREF
	    | DAC0CF1_GAINADJ__FALSE;

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
	REF0CN = REF0CN_GAIN__GAIN2X | REF0CN_EN__ENABLE;
	// [REF0CN - Voltage Reference Control]$

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
	while (CLKSEL & CLKSEL_DIVRDY__BMASK == CLKSEL_DIVRDY__NOT_READY)
		;
	// [CLKSEL - Clock Select]$

}
