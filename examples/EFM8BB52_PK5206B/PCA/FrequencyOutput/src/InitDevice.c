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
	PORTS_3_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	PCA_0_enter_DefaultMode_from_RESET();
	PCACH_0_enter_DefaultMode_from_RESET();
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
			| P1SKIP_B3__SKIPPED | P1SKIP_B4__NOT_SKIPPED
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
      | P3MDOUT_B2__OPEN_DRAIN | P3MDOUT_B3__OPEN_DRAIN
      | P3MDOUT_B4__PUSH_PULL | P3MDOUT_B7__OPEN_DRAIN;
  // [P3MDOUT - Port 3 Output Mode]$

  // $[P3MDIN - Port 3 Input Mode]
  /*
   // B0 (Port 3 Bit 0 Input Mode) = DIGITAL (P3.0 pin is configured for
   //     analog mode.)
   // B1 (Port 3 Bit 1 Input Mode) = DIGITAL (P3.1 pin is configured for
   //     analog mode.)
   // B2 (Port 3 Bit 2 Input Mode) = DIGITAL (P3.2 pin is configured for
   //     analog mode.)
   // B3 (Port 3 Bit 3 Input Mode) = DIGITAL (P3.3 pin is configured for
   //     analog mode.)
   // B4 (Port 3 Bit 4 Input Mode) = DIGITAL (P3.4 pin is configured for
   //     digital mode.)
   // B7 (Port 3 Bit 7 Input Mode) = DIGITAL (P3.7 pin is configured for
   //     analog mode.)
   */
  P3MDIN = P3MDIN_B0__ANALOG | P3MDIN_B1__ANALOG | P3MDIN_B2__ANALOG
      | P3MDIN_B3__ANALOG | P3MDIN_B4__DIGITAL | P3MDIN_B7__ANALOG;
  // [P3MDIN - Port 3 Input Mode]$
  SFRPAGE = 0x00;

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
	XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED
			| XBR2_URT1E__DISABLED | XBR2_URT1RTSE__DISABLED
			| XBR2_URT1CTSE__DISABLED;
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
	 // CPS (PCA Counter/Timer Pulse Select) = SYSCLK_DIV_4 (System clock
	 //     divided by 4.)
	 */
	PCA0MD = PCA0MD_CIDL__NORMAL | PCA0MD_ECF__OVF_INT_DISABLED
			| PCA0MD_CPS__SYSCLK_DIV_4;
	// [PCA0MD - PCA Mode]$

	// $[PCA0CENT - PCA Center Alignment Enable]
	// [PCA0CENT - PCA Center Alignment Enable]$

	// $[PCA0CLR - PCA Comparator Clear Control]
	// [PCA0CLR - PCA Comparator Clear Control]$

	// $[PCA0L - PCA Counter/Timer Low Byte]
	// [PCA0L - PCA Counter/Timer Low Byte]$

	// $[PCA0H - PCA Counter/Timer High Byte]
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
	 // TOG (Channel 0 Toggle Function Enable) = ENABLED (Enable toggle
	 //     function.)
	 */
	PCA0CPM0 = PCA0CPM0_CAPN__DISABLED | PCA0CPM0_ECCF__DISABLED
			| PCA0CPM0_MAT__DISABLED | PCA0CPM0_PWM16__8_BIT
			| PCA0CPM0_CAPP__DISABLED | PCA0CPM0_ECOM__ENABLED
			| PCA0CPM0_PWM__ENABLED | PCA0CPM0_TOG__ENABLED;
	// [PCA0CPM0 - PCA Channel 0 Capture/Compare Mode]$

	// $[PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]
	// [PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]$

	// $[PCA0CPH0 - PCA Channel 0 Capture Module High Byte]
	/*
	 // PCA0CPH0 (PCA Channel 0 Capture Module High Byte) = 0x3E
	 */
	PCA0CPH0 = (0x3E << PCA0CPH0_PCA0CPH0__SHIFT);
	// [PCA0CPH0 - PCA Channel 0 Capture Module High Byte]$

	// $[Auto-reload]
	// [Auto-reload]$

	// $[PCA0 Settings Restore]
	// [PCA0 Settings Restore]$

}

