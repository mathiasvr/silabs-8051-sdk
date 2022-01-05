//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB1_Register_Enums.h>
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
	WDT_0_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	PCA_0_enter_DefaultMode_from_RESET();
	PCACH_0_enter_DefaultMode_from_RESET();
	// [Config Calls]$


}


//================================================================================
// WDT_0_enter_DefaultMode_from_RESET
//================================================================================
extern void WDT_0_enter_DefaultMode_from_RESET(void) {
	// $[Watchdog Timer Init Variable Declarations]
	// [Watchdog Timer Init Variable Declarations]$
	
	// $[WDTCN - Watchdog Timer Control]
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
	// B0 (Port 0 Bit 0 Output Mode) = PUSH_PULL (P0.0 output is push-pull.)
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
	P0MDOUT = P0MDOUT_B0__PUSH_PULL | P0MDOUT_B1__OPEN_DRAIN | P0MDOUT_B2__OPEN_DRAIN
		 | P0MDOUT_B3__OPEN_DRAIN | P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN
		 | P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__OPEN_DRAIN;
	// [P0MDOUT - Port 0 Output Mode]$

	// $[P0MDIN - Port 0 Input Mode]
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
	// B6 (Port 0 Bit 6 Skip) = SKIPPED (P0.6 pin is skipped by the
	//     crossbar.)
	// B7 (Port 0 Bit 7 Skip) = SKIPPED (P0.7 pin is skipped by the
	//     crossbar.)
	*/
	P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED
		 | P0SKIP_B3__SKIPPED | P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
		 | P0SKIP_B6__SKIPPED | P0SKIP_B7__SKIPPED;
	// [P0SKIP - Port 0 Skip]$

	// $[P0MASK - Port 0 Mask]
	// [P0MASK - Port 0 Mask]$

	// $[P0MAT - Port 0 Match]
	// [P0MAT - Port 0 Match]$


}

//================================================================================
// PORTS_1_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_1_enter_DefaultMode_from_RESET(void) {
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
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN | P1MDOUT_B2__OPEN_DRAIN
		 | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__PUSH_PULL | P1MDOUT_B5__OPEN_DRAIN
		 | P1MDOUT_B6__OPEN_DRAIN | P1MDOUT_B7__OPEN_DRAIN;
	// [P1MDOUT - Port 1 Output Mode]$

	// $[P1MDIN - Port 1 Input Mode]
	// [P1MDIN - Port 1 Input Mode]$

	// $[P1SKIP - Port 1 Skip]
	/*
	// B0 (Port 1 Bit 0 Skip) = SKIPPED (P1.0 pin is skipped by the
	//     crossbar.)
	// B1 (Port 1 Bit 1 Skip) = SKIPPED (P1.1 pin is skipped by the
	//     crossbar.)
	// B2 (Port 1 Bit 2 Skip) = SKIPPED (P1.2 pin is skipped by the
	//     crossbar.)
	// B3 (Port 1 Bit 3 Skip) = SKIPPED (P1.3 pin is skipped by the
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
	P1SKIP = P1SKIP_B0__SKIPPED | P1SKIP_B1__SKIPPED | P1SKIP_B2__SKIPPED
		 | P1SKIP_B3__SKIPPED | P1SKIP_B4__NOT_SKIPPED | P1SKIP_B5__NOT_SKIPPED
		 | P1SKIP_B6__NOT_SKIPPED | P1SKIP_B7__NOT_SKIPPED;
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
	// WEAKPUD (Port I/O Weak Pullup Disable) = PULL_UPS_ENABLED (Weak
	//     Pullups enabled (except for Ports whose I/O are configured for analog
	//     mode).)
	// XBARE (Crossbar Enable) = ENABLED (Crossbar enabled.)
	*/
	XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;
	// [XBR2 - Port I/O Crossbar 2]$

	// $[PRTDRV - Port Drive Strength]
	// [PRTDRV - Port Drive Strength]$

	// $[XBR0 - Port I/O Crossbar 0]
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR1 - Port I/O Crossbar 1]
	/*
	// PCA0ME (PCA Module I/O Enable) = CEX0 (CEX0 routed to Port pin.)
	// ECIE (PCA0 External Counter Input Enable) = DISABLED (ECI unavailable
	//     at Port pin.)
	// T0E (T0 Enable) = DISABLED (T0 unavailable at Port pin.)
	// T1E (T1 Enable) = DISABLED (T1 unavailable at Port pin.)
	// T2E (T2 Enable) = DISABLED (T2 unavailable at Port pin.)
	*/
	XBR1 = XBR1_PCA0ME__CEX0 | XBR1_ECIE__DISABLED | XBR1_T0E__DISABLED
		 | XBR1_T1E__DISABLED | XBR1_T2E__DISABLED;
	// [XBR1 - Port I/O Crossbar 1]$


}

//================================================================================
// CLOCK_0_enter_DefaultMode_from_RESET
//================================================================================
extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	/*
	// CLKSL (Clock Source Select) = HFOSC (Clock derived from the Internal
	//     High-Frequency Oscillator.)
	// CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
	//     selected clock source divided by 1.)
	*/
	CLKSEL = CLKSEL_CLKSL__HFOSC | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	// [CLKSEL - Clock Select]$


}

//================================================================================
// PCA_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PCA_0_enter_DefaultMode_from_RESET(void) {
	// $[PCA Off]
	PCA0CN0_CR = PCA0CN0_CR__STOP;
	// [PCA Off]$

	// $[PCA0MD - PCA Mode]
	/*
	// CIDL (PCA Counter/Timer Idle Control) = NORMAL (PCA continues to
	//     function normally while the system controller is in Idle Mode.)
	// ECF (PCA Counter/Timer Overflow Interrupt Enable) = OVF_INT_DISABLED
	//     (Disable the CF interrupt.)
	// CPS (PCA Counter/Timer Pulse Select) = SYSCLK (System clock.)
	*/
	PCA0MD = PCA0MD_CIDL__NORMAL | PCA0MD_ECF__OVF_INT_DISABLED | PCA0MD_CPS__SYSCLK;
	// [PCA0MD - PCA Mode]$

	// $[PCA0CENT - PCA Center Alignment Enable]
	/*
	// CEX0CEN (CEX0 Center Alignment Enable) = CENTER (Center-aligned.)
	// CEX1CEN (CEX1 Center Alignment Enable) = EDGE (Edge-aligned.)
	// CEX2CEN (CEX2 Center Alignment Enable) = EDGE (Edge-aligned.)
	*/
	PCA0CENT = PCA0CENT_CEX0CEN__CENTER | PCA0CENT_CEX1CEN__EDGE | PCA0CENT_CEX2CEN__EDGE;
	// [PCA0CENT - PCA Center Alignment Enable]$

	// $[PCA0CLR - PCA Comparator Clear Control]
	// [PCA0CLR - PCA Comparator Clear Control]$

	// $[PCA0L - PCA Counter/Timer Low Byte]
	// [PCA0L - PCA Counter/Timer Low Byte]$

	// $[PCA0H - PCA Counter/Timer High Byte]
	/*
	// PCA0H (PCA Counter/Timer High Byte) = 0x04
	*/
	PCA0H = (0x04 << PCA0H_PCA0H__SHIFT);
	// [PCA0H - PCA Counter/Timer High Byte]$

	// $[PCA0POL - PCA Output Polarity]
	// [PCA0POL - PCA Output Polarity]$

	// $[PCA0PWM - PCA PWM Configuration]
	// [PCA0PWM - PCA PWM Configuration]$

	// $[PCA On]
	PCA0CN0_CR = PCA0CN0_CR__RUN;
	// [PCA On]$


}

//================================================================================
// PCACH_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PCACH_0_enter_DefaultMode_from_RESET(void) {
	// $[PCA0 Settings Save]
	uint8_t PCA0CN0_CR_save = PCA0CN0 & PCA0CN0_CR__BMASK;
	uint8_t PCA0PWM_ARSEL_save = PCA0PWM & PCA0PWM_ARSEL__BMASK;

	// Turn PCA0 off while updating PCACH0
	PCA0CN0 &= ~PCA0CN0_CR__BMASK;
	// Select Capture/Compare register)
	PCA0PWM &= ~PCA0PWM_ARSEL__BMASK;
	// [PCA0 Settings Save]$

	// $[PCA0CPM0 - PCA Channel 0 Capture/Compare Mode]
	/*
	// CAPN (Channel 0 Capture Negative Function Enable) = DISABLED (Disable
	//     negative edge capture.)
	// ECCF (Channel 0 Capture/Compare Flag Interrupt Enable) = ENABLED
	//     (Enable a Capture/Compare Flag interrupt request when CCF0 is set.)
	// MAT (Channel 0 Match Function Enable) = DISABLED (Disable match
	//     function.)
	// PWM16 (Channel 0 16-bit Pulse Width Modulation Enable) = 8_BIT (8 to
	//     11-bit PWM selected.)
	// CAPP (Channel 0 Capture Positive Function Enable) = DISABLED (Disable
	//     positive edge capture.)
	// ECOM (Channel 0 Comparator Function Enable) = DISABLED (Disable
	//     comparator function.)
	// PWM (Channel 0 Pulse Width Modulation Mode Enable) = ENABLED (Enable
	//     PWM function.)
	// TOG (Channel 0 Toggle Function Enable) = DISABLED (Disable toggle
	//     function.)
	*/
	PCA0CPM0 = PCA0CPM0_CAPN__DISABLED | PCA0CPM0_ECCF__ENABLED | PCA0CPM0_MAT__DISABLED
		 | PCA0CPM0_PWM16__8_BIT | PCA0CPM0_CAPP__DISABLED | PCA0CPM0_ECOM__DISABLED
		 | PCA0CPM0_PWM__ENABLED | PCA0CPM0_TOG__DISABLED;
	// [PCA0CPM0 - PCA Channel 0 Capture/Compare Mode]$

	// $[PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]
	// [PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]$

	// $[PCA0CPH0 - PCA Channel 0 Capture Module High Byte]
	// [PCA0CPH0 - PCA Channel 0 Capture Module High Byte]$

	// $[Auto-reload]
	// [Auto-reload]$

	// $[PCA0 Settings Restore]
	PCA0CN0 |= PCA0CN0_CR_save;
	PCA0PWM |= PCA0PWM_ARSEL_save;
	// [PCA0 Settings Restore]$


}


