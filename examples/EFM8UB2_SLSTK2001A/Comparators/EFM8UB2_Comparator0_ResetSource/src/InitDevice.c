//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8UB2_Register_Enums.h>
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
	PCACH_4_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PORTS_2_enter_DefaultMode_from_RESET();
	PORTS_3_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	CMP_0_enter_DefaultMode_from_RESET();
	VREF_0_enter_DefaultMode_from_RESET();
	RSTSRC_0_enter_DefaultMode_from_RESET();
	HFOSC_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	TIMER01_0_enter_DefaultMode_from_RESET();
	TIMER_SETUP_0_enter_DefaultMode_from_RESET();
	// Restore the SFRPAGE
	SFRPAGE = SFRPAGE_save;
	// [Config Calls]$

}

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
	PCA0MD = PCA0MD_CPS__SYSCLK_DIV_12 | PCA0MD_CIDL__NORMAL
			| PCA0MD_ECF__OVF_INT_DISABLED | PCA0MD_WDTE__DISABLED
			| PCA0MD_WDLCK__UNLOCKED;
	// [PCA0MD - PCA Mode]$

	// $[PCA0H - PCA Counter/Timer High Byte]
	// [PCA0H - PCA Counter/Timer High Byte]$

	// $[PCA0L - PCA Counter/Timer Low Byte]
	// [PCA0L - PCA Counter/Timer Low Byte]$

	// $[PCA0CN0 - PCA Control 0]
	// [PCA0CN0 - PCA Control 0]$

}

extern void PCACH_4_enter_DefaultMode_from_RESET(void) {
	// $[PCA0CPM4 - PCA Channel 4 Capture/Compare Mode]
	/*
	 // CAPN (Channel 4 Capture Negative Function Enable) = DISABLED (Disable
	 //     negative edge capture.)
	 // ECCF (Channel 4 Capture/Compare Flag Interrupt Enable) = DISABLED
	 //     (Disable CCF4 interrupts.)
	 // MAT (Channel 4 Match Function Enable) = ENABLED (Enable match
	 //     function.)
	 // PWM16 (Channel 4 16-bit Pulse Width Modulation Enable) = 8_BIT (8-bit
	 //     PWM selected.)
	 // CAPP (Channel 4 Capture Positive Function Enable) = DISABLED (Disable
	 //     positive edge capture.)
	 // ECOM (Channel 4 Comparator Function Enable) = DISABLED (Disable
	 //     comparator function.)
	 // PWM (Channel 4 Pulse Width Modulation Mode Enable) = DISABLED (Disable
	 //     PWM function.)
	 // TOG (Channel 4 Toggle Function Enable) = DISABLED (Disable toggle
	 //     function.)
	 */
	PCA0CPM4 = PCA0CPM4_CAPN__DISABLED | PCA0CPM4_ECCF__DISABLED
			| PCA0CPM4_MAT__ENABLED | PCA0CPM4_PWM16__8_BIT
			| PCA0CPM4_CAPP__DISABLED | PCA0CPM4_ECOM__DISABLED
			| PCA0CPM4_PWM__DISABLED | PCA0CPM4_TOG__DISABLED;
	// [PCA0CPM4 - PCA Channel 4 Capture/Compare Mode]$

	// $[PCA0CPL4 - PCA Channel 4 Capture Module Low Byte]
	// [PCA0CPL4 - PCA Channel 4 Capture Module Low Byte]$

	// $[PCA0CPH4 - PCA Channel 4 Capture Module High Byte]
	// [PCA0CPH4 - PCA Channel 4 Capture Module High Byte]$

	// $[PCA0 Start/Run restore]
	// [PCA0 Start/Run restore]$

}

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
	 // B2 (Port 0 Bit 2 Skip) = SKIPPED (P0.2 pin is skipped by the
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
			| P0SKIP_B2__SKIPPED | P0SKIP_B3__NOT_SKIPPED
			| P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
			| P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__NOT_SKIPPED;
	// [P0SKIP - Port 0 Skip]$

}

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
	 // B4 (Port 1 Bit 4 Output Mode) = OPEN_DRAIN (P1.4 output is open-
	 //     drain.)
	 // B5 (Port 1 Bit 5 Output Mode) = OPEN_DRAIN (P1.5 output is open-
	 //     drain.)
	 // B6 (Port 1 Bit 6 Output Mode) = OPEN_DRAIN (P1.6 output is open-
	 //     drain.)
	 // B7 (Port 1 Bit 7 Output Mode) = PUSH_PULL (P1.7 output is push-pull.)
	 */
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
			| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN
			| P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__OPEN_DRAIN
			| P1MDOUT_B6__OPEN_DRAIN | P1MDOUT_B7__PUSH_PULL;
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
	 // B7 (Port 1 Bit 7 Skip) = SKIPPED (P1.7 pin is skipped by the
	 //     crossbar.)
	 */
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED
			| P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED
			| P1SKIP_B4__NOT_SKIPPED | P1SKIP_B5__NOT_SKIPPED
			| P1SKIP_B6__NOT_SKIPPED | P1SKIP_B7__SKIPPED;
	// [P1SKIP - Port 1 Skip]$

}

extern void PORTS_2_enter_DefaultMode_from_RESET(void) {
	// $[P2 - Port 2 Pin Latch]
	/*
	 // B0 (Port 2 Bit 0 Latch) = HIGH (P2.0 is high. Set P2.0 to drive or
	 //     float high.)
	 // B1 (Port 2 Bit 1 Latch) = HIGH (P2.1 is high. Set P2.1 to drive or
	 //     float high.)
	 // B2 (Port 2 Bit 2 Latch) = HIGH (P2.2 is high. Set P2.2 to drive or
	 //     float high.)
	 // B3 (Port 2 Bit 3 Latch) = HIGH (P2.3 is high. Set P2.3 to drive or
	 //     float high.)
	 // B4 (Port 2 Bit 4 Latch) = HIGH (P2.4 is high. Set P2.4 to drive or
	 //     float high.)
	 // B5 (Port 2 Bit 5 Latch) = HIGH (P2.5 is high. Set P2.5 to drive or
	 //     float high.)
	 // B6 (Port 2 Bit 6 Latch) = HIGH (P2.6 is high. Set P2.6 to drive or
	 //     float high.)
	 // B7 (Port 2 Bit 7 Latch) = HIGH (P2.7 is high. Set P2.7 to drive or
	 //     float high.)
	 */
	P2 = P2_B0__HIGH | P2_B1__HIGH | P2_B2__HIGH | P2_B3__HIGH | P2_B4__HIGH
			| P2_B5__HIGH | P2_B6__HIGH | P2_B7__HIGH;
	// [P2 - Port 2 Pin Latch]$

	// $[P2MDOUT - Port 2 Output Mode]
	/*
	 // B0 (Port 2 Bit 0 Output Mode) = OPEN_DRAIN (P2.0 output is open-
	 //     drain.)
	 // B1 (Port 2 Bit 1 Output Mode) = OPEN_DRAIN (P2.1 output is open-
	 //     drain.)
	 // B2 (Port 2 Bit 2 Output Mode) = OPEN_DRAIN (P2.2 output is open-
	 //     drain.)
	 // B3 (Port 2 Bit 3 Output Mode) = OPEN_DRAIN (P2.3 output is open-
	 //     drain.)
	 // B4 (Port 2 Bit 4 Output Mode) = OPEN_DRAIN (P2.4 output is open-
	 //     drain.)
	 // B5 (Port 2 Bit 5 Output Mode) = OPEN_DRAIN (P2.5 output is open-
	 //     drain.)
	 // B6 (Port 2 Bit 6 Output Mode) = OPEN_DRAIN (P2.6 output is open-
	 //     drain.)
	 // B7 (Port 2 Bit 7 Output Mode) = OPEN_DRAIN (P2.7 output is open-
	 //     drain.)
	 */
	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN
			| P2MDOUT_B2__OPEN_DRAIN | P2MDOUT_B3__OPEN_DRAIN
			| P2MDOUT_B4__OPEN_DRAIN | P2MDOUT_B5__OPEN_DRAIN
			| P2MDOUT_B6__OPEN_DRAIN | P2MDOUT_B7__OPEN_DRAIN;
	// [P2MDOUT - Port 2 Output Mode]$

	// $[P2MDIN - Port 2 Input Mode]
	/*
	 // B0 (Port 2 Bit 0 Input Mode) = DIGITAL (P2.0 pin is configured for
	 //     digital mode.)
	 // B1 (Port 2 Bit 1 Input Mode) = DIGITAL (P2.1 pin is configured for
	 //     digital mode.)
	 // B2 (Port 2 Bit 2 Input Mode) = DIGITAL (P2.2 pin is configured for
	 //     digital mode.)
	 // B3 (Port 2 Bit 3 Input Mode) = DIGITAL (P2.3 pin is configured for
	 //     digital mode.)
	 // B4 (Port 2 Bit 4 Input Mode) = DIGITAL (P2.4 pin is configured for
	 //     digital mode.)
	 // B5 (Port 2 Bit 5 Input Mode) = ANALOG (P2.5 pin is configured for
	 //     analog mode.)
	 // B6 (Port 2 Bit 6 Input Mode) = ANALOG (P2.6 pin is configured for
	 //     analog mode.)
	 // B7 (Port 2 Bit 7 Input Mode) = DIGITAL (P2.7 pin is configured for
	 //     digital mode.)
	 */
	P2MDIN = P2MDIN_B0__DIGITAL | P2MDIN_B1__DIGITAL | P2MDIN_B2__DIGITAL
			| P2MDIN_B3__DIGITAL | P2MDIN_B4__DIGITAL | P2MDIN_B5__ANALOG
			| P2MDIN_B6__ANALOG | P2MDIN_B7__DIGITAL;
	// [P2MDIN - Port 2 Input Mode]$

	// $[P2SKIP - Port 2 Skip]
	/*
	 // B0 (Port 2 Bit 0 Skip) = NOT_SKIPPED (P2.0 pin is not skipped by the
	 //     crossbar.)
	 // B1 (Port 2 Bit 1 Skip) = NOT_SKIPPED (P2.1 pin is not skipped by the
	 //     crossbar.)
	 // B2 (Port 2 Bit 2 Skip) = NOT_SKIPPED (P2.2 pin is not skipped by the
	 //     crossbar.)
	 // B3 (Port 2 Bit 3 Skip) = NOT_SKIPPED (P2.3 pin is not skipped by the
	 //     crossbar.)
	 // B4 (Port 2 Bit 4 Skip) = NOT_SKIPPED (P2.4 pin is not skipped by the
	 //     crossbar.)
	 // B5 (Port 2 Bit 5 Skip) = SKIPPED (P2.5 pin is skipped by the
	 //     crossbar.)
	 // B6 (Port 2 Bit 6 Skip) = SKIPPED (P2.6 pin is skipped by the
	 //     crossbar.)
	 // B7 (Port 2 Bit 7 Skip) = NOT_SKIPPED (P2.7 pin is not skipped by the
	 //     crossbar.)
	 */
	P2SKIP = P2SKIP_B0__NOT_SKIPPED | P2SKIP_B1__NOT_SKIPPED
			| P2SKIP_B2__NOT_SKIPPED | P2SKIP_B3__NOT_SKIPPED
			| P2SKIP_B4__NOT_SKIPPED | P2SKIP_B5__SKIPPED | P2SKIP_B6__SKIPPED
			| P2SKIP_B7__NOT_SKIPPED;
	// [P2SKIP - Port 2 Skip]$

}

extern void PBCFG_0_enter_DefaultMode_from_RESET(void) {
	// $[XBR1 - Port I/O Crossbar 1]
	/*
	 // WEAKPUD (Port I/O Weak Pullup Disable) = PULL_UPS_ENABLED (Weak
	 //     Pullups enabled (except for Ports whose I/O are configured for analog
	 //     mode).)
	 // XBARE (Crossbar Enable) = ENABLED (Crossbar enabled.)
	 // PCA0ME (PCA Module I/O Enable) = DISABLED (All PCA I/O unavailable at
	 //     Port pins.)
	 // ECIE (PCA0 External Counter Input Enable) = DISABLED (ECI unavailable
	 //     at Port pin.)
	 // T0E (T0 Enable) = DISABLED (T0 unavailable at Port pin.)
	 // T1E (T1 Enable) = DISABLED (T1 unavailable at Port pin.)
	 */
	XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED
			| XBR1_PCA0ME__DISABLED | XBR1_ECIE__DISABLED | XBR1_T0E__DISABLED
			| XBR1_T1E__DISABLED;
	// [XBR1 - Port I/O Crossbar 1]$

	// $[XBR0 - Port I/O Crossbar 0]
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR2 - Port I/O Crossbar 2]
	// [XBR2 - Port I/O Crossbar 2]$

}

extern void CMP_0_enter_DefaultMode_from_RESET(void) {
	// $[CMP0MX - Comparator 0 Multiplexer Selection]
	/*
	 // CMXP (Comparator Positive Input MUX Selection) = CMP0P2 (External pin
	 //     CMP0P.2.)
	 // CMXN (Comparator Negative Input MUX Selection) = CMP0N1 (External pin
	 //     CMP0N.1.)
	 */
	CMP0MX = CMP0MX_CMXP__CMP0P2 | CMP0MX_CMXN__CMP0N1;
	// [CMP0MX - Comparator 0 Multiplexer Selection]$

	// $[CMP0MD - Comparator 0 Mode]
	// [CMP0MD - Comparator 0 Mode]$

	// $[CMP0CN0 - Comparator 0 Control 0]
	/*
	 // CPEN (Comparator Enable) = ENABLED (Comparator enabled.)
	 */
	CMP0CN0 |= CMP0CN0_CPEN__ENABLED;
	// [CMP0CN0 - Comparator 0 Control 0]$

}

extern void VREF_0_enter_DefaultMode_from_RESET(void) {
	// $[REF0CN - Voltage Reference Control]
	/*
	 // TEMPE (Temperature Sensor Enable) = DISABLED (Disable the internal
	 //     Temperature Sensor.)
	 // REFBE (Internal Reference Buffer Enable) = DISABLED (Disable the
	 //     internal reference buffer.)
	 // REFBGS (Reference Buffer Gain Select) = GAIN_2 (The on-chip voltage
	 //     reference buffer gain is 2.)
	 // REGOVR (Regulator Reference Override) = VREG (Use the output of the
	 //     internal regulator as the voltage reference source.)
	 // REFSL (Voltage Reference Select) = VREF (Use the VREF pin as the
	 //     voltage reference.)
	 */
	REF0CN = REF0CN_TEMPE__DISABLED | REF0CN_REFBE__DISABLED
			| REF0CN_REFBGS__GAIN_2 | REF0CN_REGOVR__VREG | REF0CN_REFSL__VREF;
	// [REF0CN - Voltage Reference Control]$

}

extern void RSTSRC_0_enter_DefaultMode_from_RESET(void) {
	// $[RSTSRC - Reset Source]
	/*
	 // PORSF (Power-On / Supply Monitor Reset Flag, and Supply Monitor Reset
	 //     Enable) = SET (A power-on or supply monitor reset occurred.)
	 // MCDRSF (Missing Clock Detector Enable and Flag) = NOT_SET (A missing
	 //     clock detector reset did not occur.)
	 // C0RSEF (Comparator0 Reset Enable and Flag) = SET (A Comparator 0 reset
	 //     occurred.)
	 // USBRSF (USB Reset Enable and Flag) = NOT_SET (A USB0 reset did not
	 //     occur.)
	 */
	RSTSRC = RSTSRC_PORSF__SET | RSTSRC_MCDRSF__NOT_SET | RSTSRC_C0RSEF__SET
			| RSTSRC_USBRSF__NOT_SET;
	// [RSTSRC - Reset Source]$

}

extern void HFOSC_0_enter_DefaultMode_from_RESET(void) {
	// $[HFO0CN - High Frequency Oscillator Control]
	/*
	 // IFCN (Oscillator Frequency Divider Control) = SYSCLK_DIV_1 (SYSCLK can
	 //     be derived from Internal H-F Oscillator divided by 1 (12 MHz).)
	 */
	HFO0CN |= HFO0CN_IFCN__SYSCLK_DIV_1;
	// [HFO0CN - High Frequency Oscillator Control]$

}

extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	// [CLKSEL - Clock Select]$

}

extern void TIMER01_0_enter_DefaultMode_from_RESET(void) {
	// $[Timer Initialization]
	//Save Timer Configuration
	uint8_t TCON_save;
	TCON_save = TCON;
	//Stop Timers
	TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;

	// [Timer Initialization]$

	// $[TH0 - Timer 0 High Byte]
	/*
	 // TH0 (Timer 0 High Byte) = 0x8A
	 */
	TH0 = (0x8A << TH0_TH0__SHIFT);
	// [TH0 - Timer 0 High Byte]$

	// $[TL0 - Timer 0 Low Byte]
	/*
	 // TL0 (Timer 0 Low Byte) = 0xCF
	 */
	TL0 = (0xCF << TL0_TL0__SHIFT);
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

extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void) {
	// $[CKCON0 - Clock Control 0]
	/*
	 // SCA (Timer 0/1 Prescale) = SYSCLK_DIV_12 (System clock divided by 12.)
	 // T0M (Timer 0 Clock Select) = SYSCLK (Counter/Timer 0 uses the system
	 //     clock.)
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
	CKCON0 = CKCON0_SCA__SYSCLK_DIV_12 | CKCON0_T0M__SYSCLK
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

extern void PORTS_3_enter_DefaultMode_from_RESET(void) {
	// $[P3 - Port 3 Pin Latch]
	/*
	 // B0 (Port 3 Bit 0 Latch) = HIGH (P3.0 is high. Set P3.0 to drive or
	 //     float high.)
	 // B1 (Port 3 Bit 1 Latch) = HIGH (P3.1 is high. Set P3.1 to drive or
	 //     float high.)
	 // B2 (Port 3 Bit 2 Latch) = HIGH (P3.2 is high. Set P3.2 to drive or
	 //     float high.)
	 // B3 (Port 3 Bit 3 Latch) = HIGH (P3.3 is high. Set P3.3 to drive or
	 //     float high.)
	 // B4 (Port 3 Bit 4 Latch) = HIGH (P3.4 is high. Set P3.4 to drive or
	 //     float high.)
	 // B5 (Port 3 Bit 5 Latch) = HIGH (P3.5 is high. Set P3.5 to drive or
	 //     float high.)
	 // B6 (Port 3 Bit 6 Latch) = HIGH (P3.6 is high. Set P3.6 to drive or
	 //     float high.)
	 // B7 (Port 3 Bit 7 Latch) = HIGH (P3.7 is high. Set P3.7 to drive or
	 //     float high.)
	 */
	P3 = P3_B0__HIGH | P3_B1__HIGH | P3_B2__HIGH | P3_B3__HIGH | P3_B4__HIGH
			| P3_B5__HIGH | P3_B6__HIGH | P3_B7__HIGH;
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
	 // B4 (Port 3 Bit 4 Output Mode) = OPEN_DRAIN (P3.4 output is open-
	 //     drain.)
	 // B5 (Port 3 Bit 5 Output Mode) = OPEN_DRAIN (P3.5 output is open-
	 //     drain.)
	 // B6 (Port 3 Bit 6 Output Mode) = OPEN_DRAIN (P3.6 output is open-
	 //     drain.)
	 // B7 (Port 3 Bit 7 Output Mode) = OPEN_DRAIN (P3.7 output is open-
	 //     drain.)
	 */
	P3MDOUT = P3MDOUT_B0__OPEN_DRAIN | P3MDOUT_B1__OPEN_DRAIN
			| P3MDOUT_B2__OPEN_DRAIN | P3MDOUT_B3__OPEN_DRAIN
			| P3MDOUT_B4__OPEN_DRAIN | P3MDOUT_B5__OPEN_DRAIN
			| P3MDOUT_B6__OPEN_DRAIN | P3MDOUT_B7__OPEN_DRAIN;
	// [P3MDOUT - Port 3 Output Mode]$

	// $[P3MDIN - Port 3 Input Mode]
	/*
	 // B0 (Port 3 Bit 0 Input Mode) = DIGITAL (P3.0 pin is configured for
	 //     digital mode.)
	 // B1 (Port 3 Bit 1 Input Mode) = DIGITAL (P3.1 pin is configured for
	 //     digital mode.)
	 // B2 (Port 3 Bit 2 Input Mode) = DIGITAL (P3.2 pin is configured for
	 //     digital mode.)
	 // B3 (Port 3 Bit 3 Input Mode) = DIGITAL (P3.3 pin is configured for
	 //     digital mode.)
	 // B4 (Port 3 Bit 4 Input Mode) = ANALOG (P3.4 pin is configured for
	 //     analog mode.)
	 // B5 (Port 3 Bit 5 Input Mode) = DIGITAL (P3.5 pin is configured for
	 //     digital mode.)
	 // B6 (Port 3 Bit 6 Input Mode) = DIGITAL (P3.6 pin is configured for
	 //     digital mode.)
	 // B7 (Port 3 Bit 7 Input Mode) = DIGITAL (P3.7 pin is configured for
	 //     digital mode.)
	 */
	P3MDIN = P3MDIN_B0__DIGITAL | P3MDIN_B1__DIGITAL | P3MDIN_B2__DIGITAL
			| P3MDIN_B3__DIGITAL | P3MDIN_B4__ANALOG | P3MDIN_B5__DIGITAL
			| P3MDIN_B6__DIGITAL | P3MDIN_B7__DIGITAL;
	// [P3MDIN - Port 3 Input Mode]$

	// $[P3SKIP - Port 3 Skip]
	/*
	 // B0 (Port 3 Bit 0 Skip) = NOT_SKIPPED (P3.0 pin is not skipped by the
	 //     crossbar.)
	 // B1 (Port 3 Bit 1 Skip) = NOT_SKIPPED (P3.1 pin is not skipped by the
	 //     crossbar.)
	 // B2 (Port 3 Bit 2 Skip) = NOT_SKIPPED (P3.2 pin is not skipped by the
	 //     crossbar.)
	 // B3 (Port 3 Bit 3 Skip) = NOT_SKIPPED (P3.3 pin is not skipped by the
	 //     crossbar.)
	 // B4 (Port 3 Bit 4 Skip) = SKIPPED (P3.4 pin is skipped by the
	 //     crossbar.)
	 // B5 (Port 3 Bit 5 Skip) = NOT_SKIPPED (P3.5 pin is not skipped by the
	 //     crossbar.)
	 // B6 (Port 3 Bit 6 Skip) = NOT_SKIPPED (P3.6 pin is not skipped by the
	 //     crossbar.)
	 // B7 (Port 3 Bit 7 Skip) = NOT_SKIPPED (P3.7 pin is not skipped by the
	 //     crossbar.)
	 */
	P3SKIP = P3SKIP_B0__NOT_SKIPPED | P3SKIP_B1__NOT_SKIPPED
			| P3SKIP_B2__NOT_SKIPPED | P3SKIP_B3__NOT_SKIPPED
			| P3SKIP_B4__SKIPPED | P3SKIP_B5__NOT_SKIPPED
			| P3SKIP_B6__NOT_SKIPPED | P3SKIP_B7__NOT_SKIPPED;
	// [P3SKIP - Port 3 Skip]$

}
