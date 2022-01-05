//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8SB2_Register_Enums.h>
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
	PCA_0_enter_DefaultMode_from_RESET();
	PCACH_0_enter_DefaultMode_from_RESET();
	PCACH_1_enter_DefaultMode_from_RESET();
	PCACH_2_enter_DefaultMode_from_RESET();
	PCACH_5_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PORTS_2_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	RSTSRC_0_enter_DefaultMode_from_RESET();
	HFOSC_0_enter_DefaultMode_from_RESET();
	RTC_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	FLASH_0_enter_DefaultMode_from_RESET();
	TIMER16_3_enter_DefaultMode_from_RESET();
	ADC_0_enter_DefaultMode_from_RESET();
	VREF_0_enter_DefaultMode_from_RESET();
	SPI_1_enter_DefaultMode_from_RESET();
	INTERRUPT_0_enter_DefaultMode_from_RESET();
	// Restore the SFRPAGE
	SFRPAGE = SFRPAGE_save;
	// [Config Calls]$


}


//================================================================================
// PCA_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PCA_0_enter_DefaultMode_from_RESET(void) {
	// $[PCA0MD - PCA Mode]
	/*
	// WDTE (Watchdog Timer Enable) = DISABLED (Disable Watchdog Timer.)
	// CPS (PCA Counter/Timer Pulse Select) = SYSCLK_DIV_12 (System clock
	//     divided by 12.)
	// CIDL (PCA Counter/Timer Idle Control) = NORMAL (PCA continues to
	//     function normally while the system controller is in Idle Mode.)
	// ECF (PCA Counter/Timer Overflow Interrupt Enable) = OVF_INT_DISABLED
	//     (Disable the CF interrupt.)
	// WDTE (Watchdog Timer Enable) = DISABLED (Disable Watchdog Timer.)
	// WDLCK (Watchdog Timer Lock) = UNLOCKED (Watchdog Timer Enable
	//     unlocked.)
	*/
	SFRPAGE = 0x00;
	PCA0MD &= ~PCA0MD_WDTE__BMASK;
	PCA0MD = PCA0MD_CPS__SYSCLK_DIV_12 | PCA0MD_CIDL__NORMAL | PCA0MD_ECF__OVF_INT_DISABLED
		 | PCA0MD_WDTE__DISABLED | PCA0MD_WDLCK__UNLOCKED;
	// [PCA0MD - PCA Mode]$

	// $[PCA0H - PCA Counter/Timer High Byte]
	// [PCA0H - PCA Counter/Timer High Byte]$

	// $[PCA0L - PCA Counter/Timer Low Byte]
	// [PCA0L - PCA Counter/Timer Low Byte]$

	// $[PCA0PWM - PCA PWM Configuration]
	// [PCA0PWM - PCA PWM Configuration]$

	// $[PCA0CN0 - PCA Control 0]
	/*
	// CR (PCA Counter/Timer Run Control) = RUN (Start the PCA Counter/Timer
	//     running.)
	*/
	PCA0CN0 |= PCA0CN0_CR__RUN;
	// [PCA0CN0 - PCA Control 0]$


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
	// ECCF (Channel 0 Capture/Compare Flag Interrupt Enable) = DISABLED
	//     (Disable CCF0 interrupts.)
	// MAT (Channel 0 Match Function Enable) = DISABLED (Disable match
	//     function.)
	// PWM16 (Channel 0 16-bit Pulse Width Modulation Enable) = 8_BIT (8 to
	//     11-bit PWM selected.)
	// CAPP (Channel 0 Capture Positive Function Enable) = DISABLED (Disable
	//     positive edge capture.)
	// ECOM (Channel 0 Comparator Function Enable) = ENABLED (Enable
	//     comparator function.)
	// PWM (Channel 0 Pulse Width Modulation Mode Enable) = ENABLED (Enable
	//     PWM function.)
	// TOG (Channel 0 Toggle Function Enable) = DISABLED (Disable toggle
	//     function.)
	*/
	PCA0CPM0 = PCA0CPM0_CAPN__DISABLED | PCA0CPM0_ECCF__DISABLED | PCA0CPM0_MAT__DISABLED
		 | PCA0CPM0_PWM16__8_BIT | PCA0CPM0_CAPP__DISABLED | PCA0CPM0_ECOM__ENABLED
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

//================================================================================
// PCACH_1_enter_DefaultMode_from_RESET
//================================================================================
extern void PCACH_1_enter_DefaultMode_from_RESET(void) {
	// $[PCA0 Settings Save]
	uint8_t PCA0CN0_CR_save = PCA0CN0 & PCA0CN0_CR__BMASK;
	uint8_t PCA0PWM_ARSEL_save = PCA0PWM & PCA0PWM_ARSEL__BMASK;

	// Turn PCA0 off while updating PCACH1
	PCA0CN0 &= ~PCA0CN0_CR__BMASK;
	// Select Capture/Compare register)
	PCA0PWM &= ~PCA0PWM_ARSEL__BMASK;
	// [PCA0 Settings Save]$

	// $[PCA0CPM1 - PCA Channel 1 Capture/Compare Mode]
	/*
	// CAPN (Channel 1 Capture Negative Function Enable) = DISABLED (Disable
	//     negative edge capture.)
	// ECCF (Channel 1 Capture/Compare Flag Interrupt Enable) = DISABLED
	//     (Disable CCF1 interrupts.)
	// MAT (Channel 1 Match Function Enable) = DISABLED (Disable match
	//     function.)
	// PWM16 (Channel 1 16-bit Pulse Width Modulation Enable) = 8_BIT (8 to
	//     11-bit PWM selected.)
	// CAPP (Channel 1 Capture Positive Function Enable) = DISABLED (Disable
	//     positive edge capture.)
	// ECOM (Channel 1 Comparator Function Enable) = ENABLED (Enable
	//     comparator function.)
	// PWM (Channel 1 Pulse Width Modulation Mode Enable) = ENABLED (Enable
	//     PWM function.)
	// TOG (Channel 1 Toggle Function Enable) = DISABLED (Disable toggle
	//     function.)
	*/
	PCA0CPM1 = PCA0CPM1_CAPN__DISABLED | PCA0CPM1_ECCF__DISABLED | PCA0CPM1_MAT__DISABLED
		 | PCA0CPM1_PWM16__8_BIT | PCA0CPM1_CAPP__DISABLED | PCA0CPM1_ECOM__ENABLED
		 | PCA0CPM1_PWM__ENABLED | PCA0CPM1_TOG__DISABLED;
	// [PCA0CPM1 - PCA Channel 1 Capture/Compare Mode]$

	// $[PCA0CPL1 - PCA Channel 1 Capture Module Low Byte]
	// [PCA0CPL1 - PCA Channel 1 Capture Module Low Byte]$

	// $[PCA0CPH1 - PCA Channel 1 Capture Module High Byte]
	// [PCA0CPH1 - PCA Channel 1 Capture Module High Byte]$

	// $[Auto-reload]
	// [Auto-reload]$

	// $[PCA0 Settings Restore]
	PCA0CN0 |= PCA0CN0_CR_save;
	PCA0PWM |= PCA0PWM_ARSEL_save;
	// [PCA0 Settings Restore]$


}

//================================================================================
// PCACH_2_enter_DefaultMode_from_RESET
//================================================================================
extern void PCACH_2_enter_DefaultMode_from_RESET(void) {
	// $[PCA0 Settings Save]
	uint8_t PCA0CN0_CR_save = PCA0CN0 & PCA0CN0_CR__BMASK;
	uint8_t PCA0PWM_ARSEL_save = PCA0PWM & PCA0PWM_ARSEL__BMASK;

	// Turn PCA0 off while updating PCACH2
	PCA0CN0 &= ~PCA0CN0_CR__BMASK;
	// Select Capture/Compare register)
	PCA0PWM &= ~PCA0PWM_ARSEL__BMASK;
	// [PCA0 Settings Save]$

	// $[PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]
	/*
	// CAPN (Channel 2 Capture Negative Function Enable) = DISABLED (Disable
	//     negative edge capture.)
	// ECCF (Channel 2 Capture/Compare Flag Interrupt Enable) = DISABLED
	//     (Disable CCF2 interrupts.)
	// MAT (Channel 2 Match Function Enable) = DISABLED (Disable match
	//     function.)
	// PWM16 (Channel 2 16-bit Pulse Width Modulation Enable) = 8_BIT (8 to
	//     11-bit PWM selected.)
	// CAPP (Channel 2 Capture Positive Function Enable) = DISABLED (Disable
	//     positive edge capture.)
	// ECOM (Channel 2 Comparator Function Enable) = ENABLED (Enable
	//     comparator function.)
	// PWM (Channel 2 Pulse Width Modulation Mode Enable) = ENABLED (Enable
	//     PWM function.)
	// TOG (Channel 2 Toggle Function Enable) = DISABLED (Disable toggle
	//     function.)
	*/
	PCA0CPM2 = PCA0CPM2_CAPN__DISABLED | PCA0CPM2_ECCF__DISABLED | PCA0CPM2_MAT__DISABLED
		 | PCA0CPM2_PWM16__8_BIT | PCA0CPM2_CAPP__DISABLED | PCA0CPM2_ECOM__ENABLED
		 | PCA0CPM2_PWM__ENABLED | PCA0CPM2_TOG__DISABLED;
	// [PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]$

	// $[PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]
	// [PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]$

	// $[PCA0CPH2 - PCA Channel 2 Capture Module High Byte]
	// [PCA0CPH2 - PCA Channel 2 Capture Module High Byte]$

	// $[Auto-reload]
	// [Auto-reload]$

	// $[PCA0 Settings Restore]
	PCA0CN0 |= PCA0CN0_CR_save;
	PCA0PWM |= PCA0PWM_ARSEL_save;
	// [PCA0 Settings Restore]$


}

//================================================================================
// PCACH_5_enter_DefaultMode_from_RESET
//================================================================================
extern void PCACH_5_enter_DefaultMode_from_RESET(void) {
	// $[PCA0 Settings Save]
	uint8_t PCA0CN0_CR_save = PCA0CN0 & PCA0CN0_CR__BMASK;
	uint8_t PCA0PWM_ARSEL_save = PCA0PWM & PCA0PWM_ARSEL__BMASK;

	// Turn PCA0 off while updating PCACH5
	PCA0CN0 &= ~PCA0CN0_CR__BMASK;
	// Select Capture/Compare register)
	PCA0PWM &= ~PCA0PWM_ARSEL__BMASK;
	// [PCA0 Settings Save]$

	// $[PCA0CPM5 - PCA Channel 5 Capture/Compare Mode]
	/*
	// CAPN (Channel 5 Capture Negative Function Enable) = DISABLED (Disable
	//     negative edge capture.)
	// ECCF (Channel 5 Capture/Compare Flag Interrupt Enable) = DISABLED
	//     (Disable CCF5 interrupts.)
	// MAT (Channel 5 Match Function Enable) = ENABLED (Enable match
	//     function.)
	// PWM16 (Channel 5 16-bit Pulse Width Modulation Enable) = 8_BIT (8 to
	//     11-bit PWM selected.)
	// CAPP (Channel 5 Capture Positive Function Enable) = DISABLED (Disable
	//     positive edge capture.)
	// ECOM (Channel 5 Comparator Function Enable) = DISABLED (Disable
	//     comparator function.)
	// PWM (Channel 5 Pulse Width Modulation Mode Enable) = DISABLED (Disable
	//     PWM function.)
	// TOG (Channel 5 Toggle Function Enable) = DISABLED (Disable toggle
	//     function.)
	*/
	PCA0CPM5 = PCA0CPM5_CAPN__DISABLED | PCA0CPM5_ECCF__DISABLED | PCA0CPM5_MAT__ENABLED
		 | PCA0CPM5_PWM16__8_BIT | PCA0CPM5_CAPP__DISABLED | PCA0CPM5_ECOM__DISABLED
		 | PCA0CPM5_PWM__DISABLED | PCA0CPM5_TOG__DISABLED;
	// [PCA0CPM5 - PCA Channel 5 Capture/Compare Mode]$

	// $[PCA0CPL5 - PCA Channel 5 Capture Module Low Byte]
	// [PCA0CPL5 - PCA Channel 5 Capture Module Low Byte]$

	// $[PCA0CPH5 - PCA Channel 5 Capture Module High Byte]
	// [PCA0CPH5 - PCA Channel 5 Capture Module High Byte]$

	// $[Auto-reload]
	// [Auto-reload]$

	// $[PCA0 Settings Restore]
	PCA0CN0 |= PCA0CN0_CR_save;
	PCA0PWM |= PCA0PWM_ARSEL_save;
	// [PCA0 Settings Restore]$


}

//================================================================================
// PORTS_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_0_enter_DefaultMode_from_RESET(void) {
	// $[P0 - Port 0 Pin Latch]
	// [P0 - Port 0 Pin Latch]$

	// $[P0MDOUT - Port 0 Output Mode]
	// [P0MDOUT - Port 0 Output Mode]$

	// $[P0MDIN - Port 0 Input Mode]
	/*
	// B0 (Port 0 Bit 0 Input Mode) = ANALOG (P0.0 pin is configured for
	//     analog mode.)
	// B1 (Port 0 Bit 1 Input Mode) = ANALOG (P0.1 pin is configured for
	//     analog mode.)
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
	P0MDIN = P0MDIN_B0__ANALOG | P0MDIN_B1__ANALOG | P0MDIN_B2__DIGITAL
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
	/*
	// B0 (Port 0 Bit 0 Mask Value) = IGNORED (P0.0 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B1 (Port 0 Bit 1 Mask Value) = IGNORED (P0.1 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B2 (Port 0 Bit 2 Mask Value) = COMPARED (P0.2 pin logic value is
	//     compared to P0MAT.2.)
	// B3 (Port 0 Bit 3 Mask Value) = COMPARED (P0.3 pin logic value is
	//     compared to P0MAT.3.)
	// B4 (Port 0 Bit 4 Mask Value) = IGNORED (P0.4 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B5 (Port 0 Bit 5 Mask Value) = IGNORED (P0.5 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B6 (Port 0 Bit 6 Mask Value) = IGNORED (P0.6 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B7 (Port 0 Bit 7 Mask Value) = IGNORED (P0.7 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	*/
	P0MASK = P0MASK_B0__IGNORED | P0MASK_B1__IGNORED | P0MASK_B2__COMPARED
		 | P0MASK_B3__COMPARED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
		 | P0MASK_B6__IGNORED | P0MASK_B7__IGNORED;
	// [P0MASK - Port 0 Mask]$

	// $[P0MAT - Port 0 Match]
	// [P0MAT - Port 0 Match]$

	// $[P0DRV - Port 0 Drive Strength]
	// [P0DRV - Port 0 Drive Strength]$


}

//================================================================================
// PORTS_1_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_1_enter_DefaultMode_from_RESET(void) {
	// $[P1 - Port 1 Pin Latch]
	// [P1 - Port 1 Pin Latch]$

	// $[P1MDOUT - Port 1 Output Mode]
	/*
	// B0 (Port 1 Bit 0 Output Mode) = PUSH_PULL (P1.0 output is push-pull.)
	// B1 (Port 1 Bit 1 Output Mode) = OPEN_DRAIN (P1.1 output is open-
	//     drain.)
	// B2 (Port 1 Bit 2 Output Mode) = PUSH_PULL (P1.2 output is push-pull.)
	// B3 (Port 1 Bit 3 Output Mode) = OPEN_DRAIN (P1.3 output is open-
	//     drain.)
	// B4 (Port 1 Bit 4 Output Mode) = PUSH_PULL (P1.4 output is push-pull.)
	// B5 (Port 1 Bit 5 Output Mode) = PUSH_PULL (P1.5 output is push-pull.)
	// B6 (Port 1 Bit 6 Output Mode) = OPEN_DRAIN (P1.6 output is open-
	//     drain.)
	// B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
	//     drain.)
	*/
	P1MDOUT = P1MDOUT_B0__PUSH_PULL | P1MDOUT_B1__OPEN_DRAIN | P1MDOUT_B2__PUSH_PULL
		 | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__PUSH_PULL | P1MDOUT_B5__PUSH_PULL
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
	// B4 (Port 1 Bit 4 Skip) = SKIPPED (P1.4 pin is skipped by the
	//     crossbar.)
	// B5 (Port 1 Bit 5 Skip) = SKIPPED (P1.5 pin is skipped by the
	//     crossbar.)
	// B6 (Port 1 Bit 6 Skip) = SKIPPED (P1.6 pin is skipped by the
	//     crossbar.)
	// B7 (Port 1 Bit 7 Skip) = SKIPPED (P1.7 pin is skipped by the
	//     crossbar.)
	*/
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED | P1SKIP_B2__NOT_SKIPPED
		 | P1SKIP_B3__SKIPPED | P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED
		 | P1SKIP_B6__SKIPPED | P1SKIP_B7__SKIPPED;
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
	// B7 (Port 1 Bit 7 Mask Value) = COMPARED (P1.7 pin logic value is
	//     compared to P1MAT.7.)
	*/
	P1MASK = P1MASK_B0__IGNORED | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
		 | P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
		 | P1MASK_B6__IGNORED | P1MASK_B7__COMPARED;
	// [P1MASK - Port 1 Mask]$

	// $[P1MAT - Port 1 Match]
	// [P1MAT - Port 1 Match]$

	// $[P1DRV - Port 1 Drive Strength]
	// [P1DRV - Port 1 Drive Strength]$


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
	// B1 (Port 2 Bit 1 Output Mode) = PUSH_PULL (P2.1 output is push-pull.)
	// B2 (Port 2 Bit 2 Output Mode) = PUSH_PULL (P2.2 output is push-pull.)
	// B3 (Port 2 Bit 3 Output Mode) = OPEN_DRAIN (P2.3 output is open-
	//     drain.)
	// B4 (Port 2 Bit 4 Output Mode) = OPEN_DRAIN (P2.4 output is open-
	//     drain.)
	// B5 (Port 2 Bit 5 Output Mode) = OPEN_DRAIN (P2.5 output is open-
	//     drain.)
	// B6 (Port 2 Bit 6 Output Mode) = PUSH_PULL (P2.6 output is push-pull.)
	// B7 (Port 2 Bit 7 Output Mode) = OPEN_DRAIN (P2.7 output is open-
	//     drain.)
	*/
	P2MDOUT = P2MDOUT_B0__PUSH_PULL | P2MDOUT_B1__PUSH_PULL | P2MDOUT_B2__PUSH_PULL
		 | P2MDOUT_B3__OPEN_DRAIN | P2MDOUT_B4__OPEN_DRAIN | P2MDOUT_B5__OPEN_DRAIN
		 | P2MDOUT_B6__PUSH_PULL | P2MDOUT_B7__OPEN_DRAIN;
	// [P2MDOUT - Port 2 Output Mode]$

	// $[P2MDIN - Port 2 Input Mode]
	// [P2MDIN - Port 2 Input Mode]$

	// $[P2SKIP - Port 2 Skip]
	/*
	// B0 (Port 2 Bit 0 Skip) = SKIPPED (P2.0 pin is skipped by the
	//     crossbar.)
	// B1 (Port 2 Bit 1 Skip) = SKIPPED (P2.1 pin is skipped by the
	//     crossbar.)
	// B2 (Port 2 Bit 2 Skip) = SKIPPED (P2.2 pin is skipped by the
	//     crossbar.)
	// B3 (Port 2 Bit 3 Skip) = NOT_SKIPPED (P2.3 pin is not skipped by the
	//     crossbar.)
	// B4 (Port 2 Bit 4 Skip) = NOT_SKIPPED (P2.4 pin is not skipped by the
	//     crossbar.)
	// B5 (Port 2 Bit 5 Skip) = NOT_SKIPPED (P2.5 pin is not skipped by the
	//     crossbar.)
	// B6 (Port 2 Bit 6 Skip) = SKIPPED (P2.6 pin is skipped by the
	//     crossbar.)
	// B7 (Port 2 Bit 7 Skip) = NOT_SKIPPED (P2.7 pin is not skipped by the
	//     crossbar.)
	*/
	P2SKIP = P2SKIP_B0__SKIPPED | P2SKIP_B1__SKIPPED | P2SKIP_B2__SKIPPED
		 | P2SKIP_B3__NOT_SKIPPED | P2SKIP_B4__NOT_SKIPPED | P2SKIP_B5__NOT_SKIPPED
		 | P2SKIP_B6__SKIPPED | P2SKIP_B7__NOT_SKIPPED;
	// [P2SKIP - Port 2 Skip]$

	// $[P2DRV - Port 2 Drive Strength]
	// [P2DRV - Port 2 Drive Strength]$


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

	// $[XBR0 - Port I/O Crossbar 0]
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR1 - Port I/O Crossbar 1]
	/*
	// SPI1E (SPI1 I/O Enable) = ENABLED (SPI1 I/O routed to Port pins.)
	// PCA0ME (PCA Module I/O Enable) = DISABLED (All PCA I/O unavailable at
	//     Port pins.)
	// ECIE (PCA0 External Counter Input Enable) = DISABLED (ECI unavailable
	//     at Port pin.)
	// T0E (T0 Enable) = DISABLED (T0 unavailable at Port pin.)
	// T1E (T1 Enable) = DISABLED (T1 unavailable at Port pin.)
	*/
	XBR1 = XBR1_SPI1E__ENABLED | XBR1_PCA0ME__DISABLED | XBR1_ECIE__DISABLED
		 | XBR1_T0E__DISABLED | XBR1_T1E__DISABLED;
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
	// RTC0RE (RTC Reset Enable and Flag) = NOT_SET (A RTC alarm or
	//     oscillator fail reset did not occur.)
	*/
	RSTSRC = RSTSRC_PORSF__NOT_SET | RSTSRC_MCDRSF__SET | RSTSRC_C0RSEF__NOT_SET
		 | RSTSRC_RTC0RE__NOT_SET;
	// [RSTSRC - Reset Source]$


}

//================================================================================
// HFOSC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void HFOSC_0_enter_DefaultMode_from_RESET(void) {
	// $[HFO#CAL - High Frequency Oscillator Calibration]
	// [HFO#CAL - High Frequency Oscillator Calibration]$

	// $[HFO#CN - High Frequency Oscillator Control]
	/*
	// IOSCEN (High Frequency Oscillator Enable) = ENABLED (High Frequency
	//     Oscillator enabled.)
	*/
	HFO0CN |= HFO0CN_IOSCEN__ENABLED;
	// [HFO#CN - High Frequency Oscillator Control]$

	// $[Oscillator Ready]
	while((HFO0CN & HFO0CN_IFRDY__BMASK) == HFO0CN_IFRDY__NOT_SET);
	// [Oscillator Ready]$


}

//================================================================================
// RTC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void RTC_0_enter_DefaultMode_from_RESET(void) {
	// $[RTC Initialization]
	// A variable for providing a delay for external oscillator startup
	uint16_t delayCounter;
	// Save the system clock (the system clock will be slowed during the startup delay)
	uint8_t CLKSEL_save = CLKSEL;

	// Unlock the RTC Interface
	RTC0KEY = RTC0KEY_RTC0ST__KEY1;
	RTC0KEY = RTC0KEY_RTC0ST__KEY2;

	// Enable power to the SmaRTClock oscillator circuit (RTC0EN = 1)
	// [RTC Initialization]$

	// $[RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]
	// Set SmaRTClock to Crystal Mode (XMODE = 1).
	// Disable Automatic Gain Control (AGCEN) and enable Bias Doubling (BIASX2) for fast crystal startup.
	RTC0ADR = RTC0XCN0;
	RTC0DAT = RTC0XCN0_XMODE__CRYSTAL | RTC0XCN0_BIASX2__ENABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	// [RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]$

	// $[RTC0XCF - RTC Oscillator Configuration]
	/*
	// AUTOSTP (Automatic Load Capacitance Stepping Enable) = DISABLED
	//     (Disable load capacitance stepping.)
	// LOADCAP (Load Capacitance Programmed Value) = 0x00
	*/
	RTC0ADR = RTC0XCF;
	RTC0DAT = RTC0XCF_AUTOSTP__DISABLED | (0x00 << RTC0XCF_LOADCAP__SHIFT);
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	// [RTC0XCF - RTC Oscillator Configuration]$

	// $[RTC0PIN - RTC Pin Configuration]
	// [RTC0PIN - RTC Pin Configuration]$

	// $[CAPTURE0 - RTC Timer Capture 0]
	// [CAPTURE0 - RTC Timer Capture 0]$

	// $[CAPTURE1 - RTC Timer Capture 1]
	// [CAPTURE1 - RTC Timer Capture 1]$

	// $[CAPTURE2 - RTC Timer Capture 2]
	// [CAPTURE2 - RTC Timer Capture 2]$

	// $[CAPTURE3 - RTC Timer Capture 3]
	// [CAPTURE3 - RTC Timer Capture 3]$

	// $[ALARM0 - RTC Alarm Programmed Value 0]
	// [ALARM0 - RTC Alarm Programmed Value 0]$

	// $[ALARM1 - RTC Alarm Programmed Value 1]
	/*
	// ALARM1 (RTC Alarm Programmed Value 1) = 0x80
	*/
	RTC0ADR = ALARM1;
	RTC0DAT = (0x80 << ALARM1_ALARM1__SHIFT);
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	// [ALARM1 - RTC Alarm Programmed Value 1]$

	// $[ALARM2 - RTC Alarm Programmed Value 2]
	// [ALARM2 - RTC Alarm Programmed Value 2]$

	// $[ALARM3 - RTC Alarm Programmed Value 3]
	// [ALARM3 - RTC Alarm Programmed Value 3]$

	// $[RTC0CN - RTC Control]
	/*
	// RTC0EN (RTC Enable) = ENABLED (Enable RTC oscillator.)
	*/
	RTC0ADR = RTC0CN0;
	RTC0DAT = 0;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	RTC0ADR = RTC0CN0;
	RTC0DAT |= RTC0CN0_RTC0EN__ENABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// [RTC0CN - RTC Control]$

	// $[External Oscillator Setup]
	// Set the clock to a known value for the delay
	CLKSEL = CLKSEL_CLKSL__LPOSC | CLKSEL_CLKDIV__SYSCLK_DIV_128;

	// Delay for > 20 ms
	for (delayCounter=0x150;delayCounter!=0;delayCounter--);

	// Poll the SmaRTClock Clock Valid Bit (CLKVLD) until the crystal oscillator stabilizes
	do {
	RTC0ADR = RTC0ADR_BUSY__SET | RTC0ADR_ADDR__RTC0XCN0;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	} while ((RTC0DAT & RTC0XCN0_CLKVLD__BMASK)== RTC0XCN0_CLKVLD__NOT_SET);

	// Enable Automatic Gain Control (AGCEN) and disable Bias Doubling (BIASX2) for maximum power savings
	/*
	// XMODE (RTC Oscillator Mode) = CRYSTAL (Crystal Mode selected.)
	// AGCEN (RTC Oscillator Automatic Gain Control (AGC) Enable) = DISABLED
	//     (Disable AGC.)
	// BIASX2 (RTC Oscillator Bias Double Enable) = ENABLED (Enable the Bias
	//     Double feature.)
	*/
	RTC0ADR = RTC0XCN0;
	RTC0DAT = RTC0XCN0_XMODE__CRYSTAL | RTC0XCN0_AGCEN__DISABLED | RTC0XCN0_BIASX2__ENABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// Enable the SmaRTClock missing clock detector.
	/*
	// MCLKEN (Missing RTC Detector Enable) = ENABLED (Enable missing RTC
	//     detector.)
	*/
	RTC0ADR = RTC0CN0;
	RTC0DAT |= RTC0CN0_MCLKEN__ENABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// Delay for > 2 ms
	for (delayCounter=0x100;delayCounter!=0;delayCounter--);

	// Clear PMU wake-up source flags
	PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;

	// Restore system clock
	CLKSEL = CLKSEL_save;
	// Poll CLKRDY to wait for the clock to stabilize
	while(!((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__SET));

	/*
	// RTC0EN (RTC Enable) = ENABLED (Enable RTC oscillator.)
	// RTC0TR (RTC Timer Run Control) = RUN (RTC timer is running.)
	// MCLKEN (Missing RTC Detector Enable) = ENABLED (Enable missing RTC
	//     detector.)
	// RTC0AEN (RTC Alarm Enable) = ENABLED (Enable RTC alarm.)
	// ALRM (RTC Alarm Event Flag and Auto Reset Enable) = SET (Alarm event
	//     flag is set or enable the auto reset function.)
	// RTC0CAP (RTC Timer Capture) = NOT_SET (Do not start a capture
	//     operation.)
	// RTC0SET (RTC Timer Set) = NOT_SET (Do not start a set operation.)
	*/
	RTC0ADR = RTC0CN0;
	RTC0DAT = RTC0CN0_RTC0EN__ENABLED | RTC0CN0_RTC0TR__RUN | RTC0CN0_MCLKEN__ENABLED
		 | RTC0CN0_RTC0AEN__ENABLED | RTC0CN0_ALRM__SET | RTC0CN0_RTC0CAP__NOT_SET
		 | RTC0CN0_RTC0SET__NOT_SET;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// [External Oscillator Setup]$


}

//================================================================================
// CLOCK_0_enter_DefaultMode_from_RESET
//================================================================================
extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	/*
	// CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
	//     selected clock source divided by 1.)
	// CLKSL (Clock Source Select) = HFOSC (Clock derived from the internal
	//     precision High-Frequency Oscillator.)
	*/
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__HFOSC;
	// Wait for the clock to be ready
	while((CLKSEL & CLKSEL_CLKRDY__BMASK) != CLKSEL_CLKRDY__SET);
	// [CLKSEL - Clock Select]$


}

//================================================================================
// FLASH_0_enter_DefaultMode_from_RESET
//================================================================================
extern void FLASH_0_enter_DefaultMode_from_RESET(void) {
	// $[FLSCL - Flash Scale]
	/*
	// BYPASS (Flash Read Timing One-Shot Bypass) = SYSCLK (The system clock
	//     determines the flash read time. This setting should be used for
	//     frequencies greater than 14 MHz.)
	*/
	FLSCL = FLSCL_BYPASS__SYSCLK;
	// [FLSCL - Flash Scale]$


}

//================================================================================
// TIMER16_3_enter_DefaultMode_from_RESET
//================================================================================
extern void TIMER16_3_enter_DefaultMode_from_RESET(void) {
	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR3CN0_TR3_save = TMR3CN0 & TMR3CN0_TR3__BMASK;
	// Stop Timer
	TMR3CN0 &= ~(TMR3CN0_TR3__BMASK);
	// [Timer Initialization]$

	// $[TMR3CN0 - Timer 3 Control]
	// [TMR3CN0 - Timer 3 Control]$

	// $[TMR3H - Timer 3 High Byte]
	/*
	// TMR3H (Timer 3 High Byte) = 0xF8
	*/
	TMR3H = (0xF8 << TMR3H_TMR3H__SHIFT);
	// [TMR3H - Timer 3 High Byte]$

	// $[TMR3L - Timer 3 Low Byte]
	/*
	// TMR3L (Timer 3 Low Byte) = 0x06
	*/
	TMR3L = (0x06 << TMR3L_TMR3L__SHIFT);
	// [TMR3L - Timer 3 Low Byte]$

	// $[TMR3RLH - Timer 3 Reload High Byte]
	/*
	// TMR3RLH (Timer 3 Reload High Byte) = 0xF8
	*/
	TMR3RLH = (0xF8 << TMR3RLH_TMR3RLH__SHIFT);
	// [TMR3RLH - Timer 3 Reload High Byte]$

	// $[TMR3RLL - Timer 3 Reload Low Byte]
	/*
	// TMR3RLL (Timer 3 Reload Low Byte) = 0x06
	*/
	TMR3RLL = (0x06 << TMR3RLL_TMR3RLL__SHIFT);
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

//================================================================================
// ADC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void ADC_0_enter_DefaultMode_from_RESET(void) {
	// $[ADC0MX - ADC0 Multiplexer Selection]
	/*
	// ADC0MX (AMUX0 Positive Input Selection) = ADC0P15 (Select channel
	//     ADC0.15.)
	*/
	ADC0MX = ADC0MX_ADC0MX__ADC0P15;
	// [ADC0MX - ADC0 Multiplexer Selection]$

	// $[ADC0AC - ADC0 Accumulator Configuration]
	// [ADC0AC - ADC0 Accumulator Configuration]$

	// $[ADC0TK - ADC0 Burst Mode Track Time]
	// [ADC0TK - ADC0 Burst Mode Track Time]$

	// $[ADC0PWR - ADC0 Power Control]
	// [ADC0PWR - ADC0 Power Control]$

	// $[ADC0CF - ADC0 Configuration]
	/*
	// AD8BE (8-Bit Mode Enable) = NORMAL (ADC0 operates in 10-bit mode
	//     (normal operation).)
	// ADGN (Gain Control) = GAIN_1 (The on-chip PGA gain is 1.)
	// ADSC (SAR Clock Divider) = 0x03
	// ADTM (Track Mode) = TRACK_NORMAL (Normal Track Mode. When ADC0 is
	//     enabled, conversion begins immediately following the start-of-
	//     conversion signal.)
	*/
	ADC0CF = ADC0CF_AD8BE__NORMAL | ADC0CF_ADGN__GAIN_1 | (0x03 << ADC0CF_ADSC__SHIFT)
		 | ADC0CF_ADTM__TRACK_NORMAL;
	// [ADC0CF - ADC0 Configuration]$

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
	// [ADC0LTH - ADC0 Less-Than High Byte]$

	// $[ADC0LTL - ADC0 Less-Than Low Byte]
	// [ADC0LTL - ADC0 Less-Than Low Byte]$

	// $[ADC0CN0 - ADC0 Control 0]
	/*
	// ADEN (ADC Enable) = ENABLED (Enable ADC0 (active and ready for data
	//     conversions).)
	// ADCM (Start of Conversion Mode Select) = TIMER0 (ADC0 conversion
	//     initiated on overflow of Timer 0.)
	*/
	ADC0CN0 &= ~ADC0CN0_ADCM__FMASK;
	ADC0CN0 |= ADC0CN0_ADEN__ENABLED
		 | ADC0CN0_ADCM__TIMER0;
	// [ADC0CN0 - ADC0 Control 0]$


}

//================================================================================
// VREF_0_enter_DefaultMode_from_RESET
//================================================================================
extern void VREF_0_enter_DefaultMode_from_RESET(void) {
	// $[REF0CN - Voltage Reference Control]
	/*
	// TEMPE (Temperature Sensor Enable) = TEMP_DISABLED (Disable the
	//     Temperature Sensor.)
	// GNDSL (Analog Ground Reference) = GND_PIN (The ADC0 ground reference
	//     is the GND pin.)
	// REFOE (Internal Voltage Reference Output Enable) = DISABLED (Internal
	//     1.68 V Precision Voltage Reference disabled and not connected to
	//     VREF.)
	// REFSL (Voltage Reference Select) = VDD_PIN (The ADC0 voltage reference
	//     is the VDD pin.)
	*/
	REF0CN = REF0CN_TEMPE__TEMP_DISABLED | REF0CN_GNDSL__GND_PIN | REF0CN_REFOE__DISABLED
		 | REF0CN_REFSL__VDD_PIN;
	// [REF0CN - Voltage Reference Control]$


}

//================================================================================
// SPI_1_enter_DefaultMode_from_RESET
//================================================================================
extern void SPI_1_enter_DefaultMode_from_RESET(void) {
	// $[SPI1CKR - SPI1 Clock Rate]
	/*
	// SPI1CKR (SPI1 Clock Rate) = 0x0B
	*/
	SPI1CKR = (0x0B << SPI1CKR_SPI1CKR__SHIFT);
	// [SPI1CKR - SPI1 Clock Rate]$

	// $[SPI1CFG - SPI1 Configuration]
	/*
	// MSTEN (Master Mode Enable) = MASTER_ENABLED (Enable master mode.
	//     Operate as a master.)
	*/
	SPI1CFG |= SPI1CFG_MSTEN__MASTER_ENABLED;
	// [SPI1CFG - SPI1 Configuration]$

	// $[SPI1CN0 - SPI1 Control]
	/*
	// SPIEN (SPI1 Enable) = ENABLED (Enable the SPI module.)
	// NSSMD (Slave Select Mode) = 3_WIRE (3-Wire Slave or 3-Wire Master
	//     Mode. NSS signal is not routed to a port pin.)
	*/
	SPI1CN0 &= ~SPI1CN0_NSSMD__FMASK;
	SPI1CN0 |= SPI1CN0_SPIEN__ENABLED;
	// [SPI1CN0 - SPI1 Control]$


}

//================================================================================
// INTERRUPT_0_enter_DefaultMode_from_RESET
//================================================================================
extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void) {
	// $[EIE1 - Extended Interrupt Enable 1]
	/*
	// EADC0 (ADC0 Conversion Complete Interrupt Enable) = ENABLED (Enable
	//     interrupt requests generated by the ADINT flag.)
	// EWADC0 (ADC0 Window Comparison Interrupt Enable) = DISABLED (Disable
	//     ADC0 Window Comparison interrupt.)
	// ECP0 (Comparator0 (CP0) Interrupt Enable) = DISABLED (Disable CP0
	//     interrupts.)
	// ECP1 (Comparator1 (CP1) Interrupt Enable) = DISABLED (Disable CP1
	//     interrupts.)
	// EPCA0 (Programmable Counter Array (PCA0) Interrupt Enable) = DISABLED
	//     (Disable all PCA0 interrupts.)
	// ERTC0A (RTC Alarm Interrupt Enable) = DISABLED (Disable RTC Alarm
	//     interrupts.)
	// ESMB0 (SMBus (SMB0) Interrupt Enable) = DISABLED (Disable all SMB0
	//     interrupts.)
	// ET3 (Timer 3 Interrupt Enable) = ENABLED (Enable interrupt requests
	//     generated by the TF3L or TF3H flags.)
	*/
	EIE1 = EIE1_EADC0__ENABLED | EIE1_EWADC0__DISABLED | EIE1_ECP0__DISABLED
		 | EIE1_ECP1__DISABLED | EIE1_EPCA0__DISABLED | EIE1_ERTC0A__DISABLED
		 | EIE1_ESMB0__DISABLED | EIE1_ET3__ENABLED;
	// [EIE1 - Extended Interrupt Enable 1]$

	// $[EIP1 - Extended Interrupt Priority 1]
	// [EIP1 - Extended Interrupt Priority 1]$

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
	// ET2 (Timer 2 Interrupt Enable) = ENABLED (Enable interrupt requests
	//     generated by the TF2L or TF2H flags.)
	// ES0 (UART0 Interrupt Enable) = DISABLED (Disable UART0 interrupt.)
	*/
	IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__DISABLED
		 | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__ENABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

	// $[EIE2 - Extended Interrupt Enable 2]
	/*
	// EMAT (Port Match Interrupts Enable) = DISABLED (Disable all Port Match
	//     interrupts.)
	// ERTC0F (RTC Oscillator Fail Interrupt Enable) = DISABLED (Disable RTC
	//     Oscillator Fail interrupts.)
	// ESPI1 (Serial Peripheral Interface (SPI1) Interrupt Enable) = ENABLED
	//     (Enable interrupt requests generated by SPI1.)
	// EWARN (VDD Supply Monitor Early Warning Interrupt Enable) = DISABLED
	//     (Disable the Supply Monitor Early Warning interrupt.)
	*/
	EIE2 = EIE2_EMAT__DISABLED | EIE2_ERTC0F__DISABLED | EIE2_ESPI1__ENABLED
		 | EIE2_EWARN__DISABLED;
	// [EIE2 - Extended Interrupt Enable 2]$

	// $[EIP2 - Extended Interrupt Priority 2]
	// [EIP2 - Extended Interrupt Priority 2]$


}


