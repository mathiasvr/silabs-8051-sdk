//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_C8051F990_Register_Enums.h>
#include "InitDevice.h"

// USER PROTOTYPES
// USER FUNCTIONS


//==============================================================================
// enter_LowPowerMode_from_RESET
//==============================================================================
extern void enter_LowPowerMode_from_RESET(void) {
	// $[Config Calls]
	PCA_0_enter_LowPowerMode_from_RESET();
	PORTS_0_enter_LowPowerMode_from_RESET();
	PORTS_1_enter_LowPowerMode_from_RESET();
	PBCFG_0_enter_LowPowerMode_from_RESET();
	CLOCK_0_enter_LowPowerMode_from_RESET();
	TIMER01_0_enter_LowPowerMode_from_RESET();
	TIMER_SETUP_0_enter_LowPowerMode_from_RESET();
	IREF_0_enter_LowPowerMode_from_RESET();
	// [Config Calls]$


}


//================================================================================
// PCA_0_enter_LowPowerMode_from_RESET
//================================================================================
extern void PCA_0_enter_LowPowerMode_from_RESET(void) {
	// $[Watchdog Disable]
	// Stop Watchdog
	// If MCU is reset by WDT before it runs to main(), please disable it in SILABS_STARTUP.A51
	SFRPAGE = 0x00;
	PCA0MD &= ~PCA0MD_WDTE__BMASK;

	// [Watchdog Disable]$

	// $[PCA0CPM0 - PCA Channel 0 Capture/Compare Mode 0]
	// [PCA0CPM0 - PCA Channel 0 Capture/Compare Mode 0]$

	// $[PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]
	// [PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]$

	// $[PCA0CPH0 - PCA Channel 0 Capture Module High Byte]
	// [PCA0CPH0 - PCA Channel 0 Capture Module High Byte]$

	// $[PCA0CPM1 - PCA Channel 1 Capture/Compare Mode]
	// [PCA0CPM1 - PCA Channel 1 Capture/Compare Mode]$

	// $[PCA0CPL1 - PCA Channel 1 Capture Module Low Byte]
	// [PCA0CPL1 - PCA Channel 1 Capture Module Low Byte]$

	// $[PCA0CPH1 - PCA Channel 1 Capture Module High Byte]
	// [PCA0CPH1 - PCA Channel 1 Capture Module High Byte]$

	// $[PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]
	// [PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]$

	// $[PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]
	// [PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]$

	// $[PCA0CPH2 - PCA Channel 2 Capture Module High Byte]
	// [PCA0CPH2 - PCA Channel 2 Capture Module High Byte]$

	// $[PCA0CN - PCA Control]
	// [PCA0CN - PCA Control]$

	// $[PCA0L - PCA Counter/Timer Low Byte]
	// [PCA0L - PCA Counter/Timer Low Byte]$

	// $[PCA0H - PCA Counter/Timer High Byte]
	// [PCA0H - PCA Counter/Timer High Byte]$

	// $[PCA0PWM - PCA PWM Configuration]
	// [PCA0PWM - PCA PWM Configuration]$

	// $[PCA0MD - PCA Mode]
	// [PCA0MD - PCA Mode]$

	// $[WDT - Watchdog run Control]
	// [WDT - Watchdog run Control]$


}

//================================================================================
// PORTS_0_enter_LowPowerMode_from_RESET
//================================================================================
extern void PORTS_0_enter_LowPowerMode_from_RESET(void) {
	// $[P0 - Port 0 Pin Latch]
	// [P0 - Port 0 Pin Latch]$

	// $[P0MDOUT - Port 0 Output Mode]
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
	// B7 (Port 0 Bit 7 Input Mode) = ANALOG (P0.7 pin is configured for
	//     analog mode.)
	*/
	P0MDIN = P0MDIN_B0__DIGITAL | P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL
		 | P0MDIN_B3__DIGITAL | P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL
		 | P0MDIN_B6__DIGITAL | P0MDIN_B7__ANALOG;
	// [P0MDIN - Port 0 Input Mode]$

	// $[P0SKIP - Port 0 Skip]
	/*
	// B0 (Port 0 Bit 0 Skip) = NOT_SKIPPED (P0.0 pin is not skipped by the
	//     crossbar.)
	// B1 (Port 0 Bit 1 Skip) = NOT_SKIPPED (P0.1 pin is not skipped by the
	//     crossbar.)
	// B2 (Port 0 Bit 2 Skip) = SKIPPED (P0.2 pin is skipped by the
	//     crossbar.)
	// B3 (Port 0 Bit 3 Skip) = SKIPPED (P0.3 pin is skipped by the
	//     crossbar.)
	// B4 (Port 0 Bit 4 Skip) = NOT_SKIPPED (P0.4 pin is not skipped by the
	//     crossbar.)
	// B5 (Port 0 Bit 5 Skip) = NOT_SKIPPED (P0.5 pin is not skipped by the
	//     crossbar.)
	// B6 (Port 0 Bit 6 Skip) = NOT_SKIPPED (P0.6 pin is not skipped by the
	//     crossbar.)
	// B7 (Port 0 Bit 7 Skip) = SKIPPED (P0.7 pin is skipped by the
	//     crossbar.)
	*/
	P0SKIP = P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED | P0SKIP_B2__SKIPPED
		 | P0SKIP_B3__SKIPPED | P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
		 | P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__SKIPPED;
	// [P0SKIP - Port 0 Skip]$

	// $[P0MASK - Port 0 Mask]
	// [P0MASK - Port 0 Mask]$

	// $[P0MAT - Port 0 Match]
	// [P0MAT - Port 0 Match]$

	// $[P0DRV - Port 0 Drive Strength]
	// [P0DRV - Port 0 Drive Strength]$


}

//================================================================================
// PORTS_1_enter_LowPowerMode_from_RESET
//================================================================================
extern void PORTS_1_enter_LowPowerMode_from_RESET(void) {
	// $[P1 - Port 1 Pin Latch]
	// [P1 - Port 1 Pin Latch]$

	// $[P1MDOUT - Port 1 Output Mode]
	/*
	// B0 (Port 1 Bit 0 Output Mode) = OPEN_DRAIN (P1.0 output is open-
	//     drain.)
	// B1 (Port 1 Bit 1 Output Mode) = OPEN_DRAIN (P1.1 output is open-
	//     drain.)
	// B2 (Port 1 Bit 2 Output Mode) = OPEN_DRAIN (P1.2 output is open-
	//     drain.)
	// B3 (Port 1 Bit 3 Output Mode) = PUSH_PULL (P1.3 output is push-pull.)
	// B4 (Port 1 Bit 4 Output Mode) = OPEN_DRAIN (P1.4 output is open-
	//     drain.)
	// B5 (Port 1 Bit 5 Output Mode) = PUSH_PULL (P1.5 output is push-pull.)
	// B6 (Port 1 Bit 6 Output Mode) = OPEN_DRAIN (P1.6 output is open-
	//     drain.)
	// B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
	//     drain.)
	*/
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN | P1MDOUT_B2__OPEN_DRAIN
		 | P1MDOUT_B3__PUSH_PULL | P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__PUSH_PULL
		 | P1MDOUT_B6__OPEN_DRAIN | P1MDOUT_B7__OPEN_DRAIN;
	// [P1MDOUT - Port 1 Output Mode]$

	// $[P1MDIN - Port 1 Input Mode]
	// [P1MDIN - Port 1 Input Mode]$

	// $[P1SKIP - Port 1 Skip]
	/*
	// B0 (Port 1 Bit 0 Skip) = NOT_SKIPPED (P1.0 pin is not skipped by the
	//     crossbar.)
	// B1 (Port 1 Bit 1 Skip) = NOT_SKIPPED (P1.1 pin is not skipped by the
	//     crossbar.)
	// B2 (Port 1 Bit 2 Skip) = NOT_SKIPPED (P1.2 pin is not skipped by the
	//     crossbar.)
	// B3 (Port 1 Bit 3 Skip) = SKIPPED (P1.3 pin is skipped by the
	//     crossbar.)
	// B4 (Port 1 Bit 4 Skip) = NOT_SKIPPED (P1.4 pin is not skipped by the
	//     crossbar.)
	// B5 (Port 1 Bit 5 Skip) = SKIPPED (P1.5 pin is skipped by the
	//     crossbar.)
	// B6 (Port 1 Bit 6 Skip) = NOT_SKIPPED (P1.6 pin is not skipped by the
	//     crossbar.)
	// B7 (Port 1 Bit 7 Skip) = NOT_SKIPPED (P1.7 pin is not skipped by the
	//     crossbar.)
	*/
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED | P1SKIP_B2__NOT_SKIPPED
		 | P1SKIP_B3__SKIPPED | P1SKIP_B4__NOT_SKIPPED | P1SKIP_B5__SKIPPED
		 | P1SKIP_B6__NOT_SKIPPED | P1SKIP_B7__NOT_SKIPPED;
	// [P1SKIP - Port 1 Skip]$

	// $[P1MASK - Port 1 Mask]
	// [P1MASK - Port 1 Mask]$

	// $[P1MAT - Port 1 Match]
	// [P1MAT - Port 1 Match]$

	// $[P1DRV - Port 1 Drive Strength]
	// [P1DRV - Port 1 Drive Strength]$


}

//================================================================================
// PBCFG_0_enter_LowPowerMode_from_RESET
//================================================================================
extern void PBCFG_0_enter_LowPowerMode_from_RESET(void) {
	// $[XBR2 - Port I/O Crossbar 2]
	/*
	// WEAKPUD (Port I/O Weak Pullup Disable) = PULL_UPS_ENABLED (Weak
	//     Pullups enabled (except for Ports whose I/O are configured for analog
	//     mode).)
	// XBARE (Crossbar Enable) = ENABLED (Crossbar enabled.)
	*/
	XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;
	// [XBR2 - Port I/O Crossbar 2]$

	// $[XBR0 - Port I/O Crossbar 0]
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR1 - Port I/O Crossbar 1]
	// [XBR1 - Port I/O Crossbar 1]$


}

//================================================================================
// CLOCK_0_enter_LowPowerMode_from_RESET
//================================================================================
extern void CLOCK_0_enter_LowPowerMode_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	/*
	// CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
	//     selected clock source divided by 1.)
	// CLKSL (Clock Source Select) = LPOSC (Clock derived from the Internal
	//     Low Power Oscillator.)
	*/
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__LPOSC;
	// [CLKSEL - Clock Select]$


}

//================================================================================
// TIMER01_0_enter_LowPowerMode_from_RESET
//================================================================================
extern void TIMER01_0_enter_LowPowerMode_from_RESET(void) {
	// $[Timer Initialization]
	//Save Timer Configuration
	uint8_t TCON_save = TCON;
	//Stop Timers
	TCON &= TCON_TR0__BMASK & TCON_TR1__BMASK;

	// [Timer Initialization]$

	// $[TH0 - Timer 0 High Byte]
	/*
	// TH0 (Timer 0 High Byte) = 214
	*/
	TH0 = (214 << TH0_TH0__SHIFT);
	// [TH0 - Timer 0 High Byte]$

	// $[TL0 - Timer 0 Low Byte]
	/*
	// TL0 (Timer 0 Low Byte) = 214
	*/
	TL0 = (214 << TL0_TL0__SHIFT);
	// [TL0 - Timer 0 Low Byte]$

	// $[TH1 - Timer 1 High Byte]
	// [TH1 - Timer 1 High Byte]$

	// $[TL1 - Timer 1 Low Byte]
	// [TL1 - Timer 1 Low Byte]$

	// $[Timer Restoration]
	//Restore Timer Configuration
	TCON = TCON_save;

	// [Timer Restoration]$


}

//================================================================================
// TIMER_SETUP_0_enter_LowPowerMode_from_RESET
//================================================================================
extern void TIMER_SETUP_0_enter_LowPowerMode_from_RESET(void) {
	// $[CKCON - Clock Control]
	/*
	// SCA (Timer 0/1 Prescale) = SYSCLK_DIV_48 (System clock divided by 48.)
	// T0M (Timer 0 Clock Select) = PRESCALE (Counter/Timer 0 uses the clock
	//     defined by the prescale field, SCA.)
	// T2MH (Timer 2 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 2 high
	//     byte uses the clock defined by the T2XCLK bit in TMR2CN.)
	// T2ML (Timer 2 Low Byte Clock Select) = EXTERNAL_CLOCK (Timer 2 low
	//     byte uses the clock defined by the T2XCLK bit in TMR2CN.)
	// T3MH (Timer 3 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 high
	//     byte uses the clock defined by the T3XCLK bit in TMR3CN.)
	// T3ML (Timer 3 Low Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 low
	//     byte uses the clock defined by the T3XCLK bit in TMR3CN.)
	// T1M (Timer 1 Clock Select) = PRESCALE (Timer 1 uses the clock defined
	//     by the prescale field, SCA.)
	*/
	CKCON = CKCON_SCA__SYSCLK_DIV_48 | CKCON_T0M__PRESCALE | CKCON_T2MH__EXTERNAL_CLOCK
		 | CKCON_T2ML__EXTERNAL_CLOCK | CKCON_T3MH__EXTERNAL_CLOCK | CKCON_T3ML__EXTERNAL_CLOCK
		 | CKCON_T1M__PRESCALE;
	// [CKCON - Clock Control]$

	// $[TMOD - Timer 0/1 Mode]
	/*
	// T0M (Timer 0 Mode Select) = MODE2 (Mode 2, 8-bit Counter/Timer with
	//     Auto-Reload)
	// CT0 (Counter/Timer 0 Select) = TIMER (Timer Mode. Timer 0 increments
	//     on the clock defined by T0M in the CKCON register.)
	// GATE0 (Timer 0 Gate Control) = DISABLED (Timer 0 enabled when TR0 = 1
	//     irrespective of /INT0 logic level.)
	// T1M (Timer 1 Mode Select) = MODE0 (Mode 0, 13-bit Counter/Timer)
	// CT1 (Counter/Timer 1 Select) = TIMER (Timer Mode. Timer 1 increments
	//     on the clock defined by T1M in the CKCON register.)
	// GATE1 (Timer 1 Gate Control) = DISABLED (Timer 1 enabled when TR1 = 1
	//     irrespective of /INT1 logic level.)
	*/
	TMOD = TMOD_T0M__MODE2 | TMOD_CT0__TIMER | TMOD_GATE0__DISABLED
		 | TMOD_T1M__MODE0 | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
	// [TMOD - Timer 0/1 Mode]$

	// $[TCON - Timer 0/1 Control]
	// [TCON - Timer 0/1 Control]$


}

//================================================================================
// IREF_0_enter_LowPowerMode_from_RESET
//================================================================================
extern void IREF_0_enter_LowPowerMode_from_RESET(void) {
	// $[IREF0CN - Current Reference Control]
	// [IREF0CN - Current Reference Control]$

	// $[IREF0CF - Current Reference Configuration]
	// [IREF0CF - Current Reference Configuration]$


}

//================================================================================
// PCA_0_enter_HighCurrentMode_from_RESET
//================================================================================
extern void PCA_0_enter_HighCurrentMode_from_RESET(void) {
	// $[Watchdog Disable]
	// Stop Watchdog
	// If MCU is reset by WDT before it runs to main(), please disable it in SILABS_STARTUP.A51
	SFRPAGE = 0x00;
	PCA0MD &= ~PCA0MD_WDTE__BMASK;

	// [Watchdog Disable]$

	// $[PCA0CPM0 - PCA Channel 0 Capture/Compare Mode 0]
	// [PCA0CPM0 - PCA Channel 0 Capture/Compare Mode 0]$

	// $[PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]
	// [PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]$

	// $[PCA0CPH0 - PCA Channel 0 Capture Module High Byte]
	// [PCA0CPH0 - PCA Channel 0 Capture Module High Byte]$

	// $[PCA0CPM1 - PCA Channel 1 Capture/Compare Mode]
	// [PCA0CPM1 - PCA Channel 1 Capture/Compare Mode]$

	// $[PCA0CPL1 - PCA Channel 1 Capture Module Low Byte]
	// [PCA0CPL1 - PCA Channel 1 Capture Module Low Byte]$

	// $[PCA0CPH1 - PCA Channel 1 Capture Module High Byte]
	// [PCA0CPH1 - PCA Channel 1 Capture Module High Byte]$

	// $[PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]
	// [PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]$

	// $[PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]
	// [PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]$

	// $[PCA0CPH2 - PCA Channel 2 Capture Module High Byte]
	// [PCA0CPH2 - PCA Channel 2 Capture Module High Byte]$

	// $[PCA0CN - PCA Control]
	// [PCA0CN - PCA Control]$

	// $[PCA0L - PCA Counter/Timer Low Byte]
	// [PCA0L - PCA Counter/Timer Low Byte]$

	// $[PCA0H - PCA Counter/Timer High Byte]
	// [PCA0H - PCA Counter/Timer High Byte]$

	// $[PCA0PWM - PCA PWM Configuration]
	// [PCA0PWM - PCA PWM Configuration]$

	// $[PCA0MD - PCA Mode]
	// [PCA0MD - PCA Mode]$

	// $[WDT - Watchdog run Control]
	// [WDT - Watchdog run Control]$


}

//================================================================================
// PORTS_0_enter_HighCurrentMode_from_RESET
//================================================================================
extern void PORTS_0_enter_HighCurrentMode_from_RESET(void) {
	// $[P0 - Port 0 Pin Latch]
	// [P0 - Port 0 Pin Latch]$

	// $[P0MDOUT - Port 0 Output Mode]
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
	// B7 (Port 0 Bit 7 Input Mode) = ANALOG (P0.7 pin is configured for
	//     analog mode.)
	*/
	P0MDIN = P0MDIN_B0__DIGITAL | P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL
		 | P0MDIN_B3__DIGITAL | P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL
		 | P0MDIN_B6__DIGITAL | P0MDIN_B7__ANALOG;
	// [P0MDIN - Port 0 Input Mode]$

	// $[P0SKIP - Port 0 Skip]
	/*
	// B0 (Port 0 Bit 0 Skip) = NOT_SKIPPED (P0.0 pin is not skipped by the
	//     crossbar.)
	// B1 (Port 0 Bit 1 Skip) = NOT_SKIPPED (P0.1 pin is not skipped by the
	//     crossbar.)
	// B2 (Port 0 Bit 2 Skip) = SKIPPED (P0.2 pin is skipped by the
	//     crossbar.)
	// B3 (Port 0 Bit 3 Skip) = SKIPPED (P0.3 pin is skipped by the
	//     crossbar.)
	// B4 (Port 0 Bit 4 Skip) = NOT_SKIPPED (P0.4 pin is not skipped by the
	//     crossbar.)
	// B5 (Port 0 Bit 5 Skip) = NOT_SKIPPED (P0.5 pin is not skipped by the
	//     crossbar.)
	// B6 (Port 0 Bit 6 Skip) = NOT_SKIPPED (P0.6 pin is not skipped by the
	//     crossbar.)
	// B7 (Port 0 Bit 7 Skip) = SKIPPED (P0.7 pin is skipped by the
	//     crossbar.)
	*/
	P0SKIP = P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED | P0SKIP_B2__SKIPPED
		 | P0SKIP_B3__SKIPPED | P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
		 | P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__SKIPPED;
	// [P0SKIP - Port 0 Skip]$

	// $[P0MASK - Port 0 Mask]
	// [P0MASK - Port 0 Mask]$

	// $[P0MAT - Port 0 Match]
	// [P0MAT - Port 0 Match]$

	// $[P0DRV - Port 0 Drive Strength]
	// [P0DRV - Port 0 Drive Strength]$


}

//================================================================================
// PORTS_1_enter_HighCurrentMode_from_RESET
//================================================================================
extern void PORTS_1_enter_HighCurrentMode_from_RESET(void) {
	// $[P1 - Port 1 Pin Latch]
	// [P1 - Port 1 Pin Latch]$

	// $[P1MDOUT - Port 1 Output Mode]
	/*
	// B0 (Port 1 Bit 0 Output Mode) = OPEN_DRAIN (P1.0 output is open-
	//     drain.)
	// B1 (Port 1 Bit 1 Output Mode) = OPEN_DRAIN (P1.1 output is open-
	//     drain.)
	// B2 (Port 1 Bit 2 Output Mode) = OPEN_DRAIN (P1.2 output is open-
	//     drain.)
	// B3 (Port 1 Bit 3 Output Mode) = PUSH_PULL (P1.3 output is push-pull.)
	// B4 (Port 1 Bit 4 Output Mode) = OPEN_DRAIN (P1.4 output is open-
	//     drain.)
	// B5 (Port 1 Bit 5 Output Mode) = PUSH_PULL (P1.5 output is push-pull.)
	// B6 (Port 1 Bit 6 Output Mode) = OPEN_DRAIN (P1.6 output is open-
	//     drain.)
	// B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
	//     drain.)
	*/
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN | P1MDOUT_B2__OPEN_DRAIN
		 | P1MDOUT_B3__PUSH_PULL | P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__PUSH_PULL
		 | P1MDOUT_B6__OPEN_DRAIN | P1MDOUT_B7__OPEN_DRAIN;
	// [P1MDOUT - Port 1 Output Mode]$

	// $[P1MDIN - Port 1 Input Mode]
	// [P1MDIN - Port 1 Input Mode]$

	// $[P1SKIP - Port 1 Skip]
	/*
	// B0 (Port 1 Bit 0 Skip) = NOT_SKIPPED (P1.0 pin is not skipped by the
	//     crossbar.)
	// B1 (Port 1 Bit 1 Skip) = NOT_SKIPPED (P1.1 pin is not skipped by the
	//     crossbar.)
	// B2 (Port 1 Bit 2 Skip) = NOT_SKIPPED (P1.2 pin is not skipped by the
	//     crossbar.)
	// B3 (Port 1 Bit 3 Skip) = SKIPPED (P1.3 pin is skipped by the
	//     crossbar.)
	// B4 (Port 1 Bit 4 Skip) = NOT_SKIPPED (P1.4 pin is not skipped by the
	//     crossbar.)
	// B5 (Port 1 Bit 5 Skip) = SKIPPED (P1.5 pin is skipped by the
	//     crossbar.)
	// B6 (Port 1 Bit 6 Skip) = NOT_SKIPPED (P1.6 pin is not skipped by the
	//     crossbar.)
	// B7 (Port 1 Bit 7 Skip) = NOT_SKIPPED (P1.7 pin is not skipped by the
	//     crossbar.)
	*/
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED | P1SKIP_B2__NOT_SKIPPED
		 | P1SKIP_B3__SKIPPED | P1SKIP_B4__NOT_SKIPPED | P1SKIP_B5__SKIPPED
		 | P1SKIP_B6__NOT_SKIPPED | P1SKIP_B7__NOT_SKIPPED;
	// [P1SKIP - Port 1 Skip]$

	// $[P1MASK - Port 1 Mask]
	// [P1MASK - Port 1 Mask]$

	// $[P1MAT - Port 1 Match]
	// [P1MAT - Port 1 Match]$

	// $[P1DRV - Port 1 Drive Strength]
	// [P1DRV - Port 1 Drive Strength]$


}

//================================================================================
// PBCFG_0_enter_HighCurrentMode_from_RESET
//================================================================================
extern void PBCFG_0_enter_HighCurrentMode_from_RESET(void) {
	// $[XBR2 - Port I/O Crossbar 2]
	/*
	// WEAKPUD (Port I/O Weak Pullup Disable) = PULL_UPS_ENABLED (Weak
	//     Pullups enabled (except for Ports whose I/O are configured for analog
	//     mode).)
	// XBARE (Crossbar Enable) = ENABLED (Crossbar enabled.)
	*/
	XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;
	// [XBR2 - Port I/O Crossbar 2]$

	// $[XBR0 - Port I/O Crossbar 0]
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR1 - Port I/O Crossbar 1]
	// [XBR1 - Port I/O Crossbar 1]$


}

//================================================================================
// CLOCK_0_enter_HighCurrentMode_from_RESET
//================================================================================
extern void CLOCK_0_enter_HighCurrentMode_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	/*
	// CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
	//     selected clock source divided by 1.)
	// CLKSL (Clock Source Select) = LPOSC (Clock derived from the Internal
	//     Low Power Oscillator.)
	*/
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__LPOSC;
	// [CLKSEL - Clock Select]$


}

//================================================================================
// TIMER01_0_enter_HighCurrentMode_from_RESET
//================================================================================
extern void TIMER01_0_enter_HighCurrentMode_from_RESET(void) {
	// $[Timer Initialization]
	//Save Timer Configuration
	uint8_t TCON_save = TCON;
	//Stop Timers
	TCON &= TCON_TR0__BMASK & TCON_TR1__BMASK;

	// [Timer Initialization]$

	// $[TH0 - Timer 0 High Byte]
	/*
	// TH0 (Timer 0 High Byte) = 214
	*/
	TH0 = (214 << TH0_TH0__SHIFT);
	// [TH0 - Timer 0 High Byte]$

	// $[TL0 - Timer 0 Low Byte]
	/*
	// TL0 (Timer 0 Low Byte) = 214
	*/
	TL0 = (214 << TL0_TL0__SHIFT);
	// [TL0 - Timer 0 Low Byte]$

	// $[TH1 - Timer 1 High Byte]
	// [TH1 - Timer 1 High Byte]$

	// $[TL1 - Timer 1 Low Byte]
	// [TL1 - Timer 1 Low Byte]$

	// $[Timer Restoration]
	//Restore Timer Configuration
	TCON = TCON_save;

	// [Timer Restoration]$


}

//================================================================================
// TIMER_SETUP_0_enter_HighCurrentMode_from_RESET
//================================================================================
extern void TIMER_SETUP_0_enter_HighCurrentMode_from_RESET(void) {
	// $[CKCON - Clock Control]
	/*
	// SCA (Timer 0/1 Prescale) = SYSCLK_DIV_48 (System clock divided by 48.)
	// T0M (Timer 0 Clock Select) = PRESCALE (Counter/Timer 0 uses the clock
	//     defined by the prescale field, SCA.)
	// T2MH (Timer 2 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 2 high
	//     byte uses the clock defined by the T2XCLK bit in TMR2CN.)
	// T2ML (Timer 2 Low Byte Clock Select) = EXTERNAL_CLOCK (Timer 2 low
	//     byte uses the clock defined by the T2XCLK bit in TMR2CN.)
	// T3MH (Timer 3 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 high
	//     byte uses the clock defined by the T3XCLK bit in TMR3CN.)
	// T3ML (Timer 3 Low Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 low
	//     byte uses the clock defined by the T3XCLK bit in TMR3CN.)
	// T1M (Timer 1 Clock Select) = PRESCALE (Timer 1 uses the clock defined
	//     by the prescale field, SCA.)
	*/
	CKCON = CKCON_SCA__SYSCLK_DIV_48 | CKCON_T0M__PRESCALE | CKCON_T2MH__EXTERNAL_CLOCK
		 | CKCON_T2ML__EXTERNAL_CLOCK | CKCON_T3MH__EXTERNAL_CLOCK | CKCON_T3ML__EXTERNAL_CLOCK
		 | CKCON_T1M__PRESCALE;
	// [CKCON - Clock Control]$

	// $[TMOD - Timer 0/1 Mode]
	/*
	// T0M (Timer 0 Mode Select) = MODE2 (Mode 2, 8-bit Counter/Timer with
	//     Auto-Reload)
	// CT0 (Counter/Timer 0 Select) = TIMER (Timer Mode. Timer 0 increments
	//     on the clock defined by T0M in the CKCON register.)
	// GATE0 (Timer 0 Gate Control) = DISABLED (Timer 0 enabled when TR0 = 1
	//     irrespective of /INT0 logic level.)
	// T1M (Timer 1 Mode Select) = MODE0 (Mode 0, 13-bit Counter/Timer)
	// CT1 (Counter/Timer 1 Select) = TIMER (Timer Mode. Timer 1 increments
	//     on the clock defined by T1M in the CKCON register.)
	// GATE1 (Timer 1 Gate Control) = DISABLED (Timer 1 enabled when TR1 = 1
	//     irrespective of /INT1 logic level.)
	*/
	TMOD = TMOD_T0M__MODE2 | TMOD_CT0__TIMER | TMOD_GATE0__DISABLED
		 | TMOD_T1M__MODE0 | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
	// [TMOD - Timer 0/1 Mode]$

	// $[TCON - Timer 0/1 Control]
	// [TCON - Timer 0/1 Control]$


}

//================================================================================
// IREF_0_enter_HighCurrentMode_from_RESET
//================================================================================
extern void IREF_0_enter_HighCurrentMode_from_RESET(void) {
	// $[IREF0CN - Current Reference Control]
	/*
	// SINK (IREF0 Current Sink Enable) = DISABLED (IREF0 is a current
	//     source.)
	// MDSEL (IREF0 Output Mode Select) = HIGH_CURRENT (High Current Mode is
	//     selected (step size = 8 uA).)
	// IREF0DAT (IREF0 Data Word) = 0
	*/
	IREF0CN = IREF0CN_SINK__DISABLED | IREF0CN_MDSEL__HIGH_CURRENT
		 | (0 << IREF0CN_IREF0DAT__SHIFT);
	// [IREF0CN - Current Reference Control]$

	// $[IREF0CF - Current Reference Configuration]
	// [IREF0CF - Current Reference Configuration]$


}


//==============================================================================
// enter_HighCurrentMode_from_RESET
//==============================================================================
extern void enter_HighCurrentMode_from_RESET(void) {
	// $[Config Calls]
	PCA_0_enter_HighCurrentMode_from_RESET();
	PORTS_0_enter_HighCurrentMode_from_RESET();
	PORTS_1_enter_HighCurrentMode_from_RESET();
	PBCFG_0_enter_HighCurrentMode_from_RESET();
	CLOCK_0_enter_HighCurrentMode_from_RESET();
	TIMER01_0_enter_HighCurrentMode_from_RESET();
	TIMER_SETUP_0_enter_HighCurrentMode_from_RESET();
	IREF_0_enter_HighCurrentMode_from_RESET();
	// [Config Calls]$


}


