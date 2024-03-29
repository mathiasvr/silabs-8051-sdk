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
#include "function_generator.h"

// $[Library Includes]
// [Library Includes]$

extern void enter_DefaultMode_from_RESET(void) {
	// $[Config Calls]
	// Save the SFRPAGE
	uint8_t SFRPAGE_save = SFRPAGE;
	WDT_0_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PORTS_2_enter_DefaultMode_from_RESET();
	PORTS_3_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	ADC_0_enter_DefaultMode_from_RESET();
	DAC_0_enter_DefaultMode_from_RESET();
	DAC_1_enter_DefaultMode_from_RESET();
	DAC_2_enter_DefaultMode_from_RESET();
	DAC_3_enter_DefaultMode_from_RESET();
	DACGCF_0_enter_DefaultMode_from_RESET();
	VREF_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	TIMER01_0_enter_DefaultMode_from_RESET();
	TIMER16_3_enter_DefaultMode_from_RESET();
	TIMER16_4_enter_DefaultMode_from_RESET();
	TIMER_SETUP_0_enter_DefaultMode_from_RESET();
	SPI_0_enter_DefaultMode_from_RESET();
	INTERRUPT_0_enter_DefaultMode_from_RESET();
	// Restore the SFRPAGE
	SFRPAGE = SFRPAGE_save;
	// [Config Calls]$

}

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
	 // B6 (Port 0 Bit 6 Output Mode) = PUSH_PULL (P0.6 output is push-pull.)
	 // B7 (Port 0 Bit 7 Output Mode) = OPEN_DRAIN (P0.7 output is open-
	 //     drain.)
	 */
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN
			| P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__OPEN_DRAIN
			| P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN
			| P0MDOUT_B6__PUSH_PULL | P0MDOUT_B7__OPEN_DRAIN;
	// [P0MDOUT - Port 0 Output Mode]$

	// $[P0MDIN - Port 0 Input Mode]
	/*
	 // B0 (Port 0 Bit 0 Input Mode) = ANALOG (P0.0 pin is configured for
	 //     analog mode.)
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
	P0MDIN = P0MDIN_B0__ANALOG | P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL
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
	 // B0 (Port 1 Bit 0 Output Mode) = PUSH_PULL (P1.0 output is push-pull.)
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
	P1MDOUT = P1MDOUT_B0__PUSH_PULL | P1MDOUT_B1__OPEN_DRAIN
			| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN
			| P1MDOUT_B4__PUSH_PULL | P1MDOUT_B5__PUSH_PULL
			| P1MDOUT_B6__PUSH_PULL | P1MDOUT_B7__OPEN_DRAIN;
	// [P1MDOUT - Port 1 Output Mode]$

	// $[P1MDIN - Port 1 Input Mode]
	/*
	 // B0 (Port 1 Bit 0 Input Mode) = DIGITAL (P1.0 pin is configured for
	 //     digital mode.)
	 // B1 (Port 1 Bit 1 Input Mode) = ANALOG (P1.1 pin is configured for
	 //     analog mode.)
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
	P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__ANALOG | P1MDIN_B2__DIGITAL
			| P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__DIGITAL
			| P1MDIN_B6__DIGITAL | P1MDIN_B7__ANALOG;
	// [P1MDIN - Port 1 Input Mode]$

	// $[P1SKIP - Port 1 Skip]
	/*
	 // B0 (Port 1 Bit 0 Skip) = NOT_SKIPPED (P1.0 pin is not skipped by the
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
	 // B7 (Port 1 Bit 7 Skip) = SKIPPED (P1.7 pin is skipped by the
	 //     crossbar.)
	 */
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__SKIPPED | P1SKIP_B2__SKIPPED
			| P1SKIP_B3__SKIPPED | P1SKIP_B4__NOT_SKIPPED
			| P1SKIP_B5__NOT_SKIPPED | P1SKIP_B6__NOT_SKIPPED
			| P1SKIP_B7__SKIPPED;
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

extern void PORTS_2_enter_DefaultMode_from_RESET(void) {
	// $[P2 - Port 2 Pin Latch]
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
	 // B6 (Port 2 Bit 6 Output Mode) = PUSH_PULL (P2.6 output is push-pull.)
	 */
	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN
			| P2MDOUT_B2__OPEN_DRAIN | P2MDOUT_B3__OPEN_DRAIN
			| P2MDOUT_B4__OPEN_DRAIN | P2MDOUT_B5__OPEN_DRAIN
			| P2MDOUT_B6__PUSH_PULL;
	// [P2MDOUT - Port 2 Output Mode]$

	// $[P2MDIN - Port 2 Input Mode]
	// [P2MDIN - Port 2 Input Mode]$

	// $[P2SKIP - Port 2 Skip]
	/*
	 // B0 (Port 2 Bit 0 Skip) = NOT_SKIPPED (P2.0 pin is not skipped by the
	 //     crossbar.)
	 // B1 (Port 2 Bit 1 Skip) = NOT_SKIPPED (P2.1 pin is not skipped by the
	 //     crossbar.)
	 // B2 (Port 2 Bit 2 Skip) = NOT_SKIPPED (P2.2 pin is not skipped by the
	 //     crossbar.)
	 // B3 (Port 2 Bit 3 Skip) = SKIPPED (P2.3 pin is skipped by the
	 //     crossbar.)
	 */
	SFRPAGE = 0x20;
	P2SKIP = P2SKIP_B0__NOT_SKIPPED | P2SKIP_B1__NOT_SKIPPED
			| P2SKIP_B2__NOT_SKIPPED | P2SKIP_B3__SKIPPED;
	// [P2SKIP - Port 2 Skip]$

	// $[P2MASK - Port 2 Mask]
	// [P2MASK - Port 2 Mask]$

	// $[P2MAT - Port 2 Match]
	// [P2MAT - Port 2 Match]$

}

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
	/*
	 // PCA0ME (PCA Module I/O Enable) = CEX0_TO_CEX2 (CEX0, CEX1, CEX2 routed
	 //     to Port pins.)
	 // ECIE (PCA0 External Counter Input Enable) = DISABLED (ECI unavailable
	 //     at Port pin.)
	 // T0E (T0 Enable) = DISABLED (T0 unavailable at Port pin.)
	 // T1E (T1 Enable) = DISABLED (T1 unavailable at Port pin.)
	 // T2E (T2 Enable) = DISABLED (T2 unavailable at Port pin.)
	 */
	XBR1 = XBR1_PCA0ME__CEX0_TO_CEX2 | XBR1_ECIE__DISABLED | XBR1_T0E__DISABLED
			| XBR1_T1E__DISABLED | XBR1_T2E__DISABLED;
	// [XBR1 - Port I/O Crossbar 1]$

}

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

extern void DACGCF_0_enter_DefaultMode_from_RESET(void) {
	// $[DACGCF0 - DAC Global Configuration 0]
	/*
	 // D01REFSL (DAC0 and DAC1 Reference Voltage Select) = VREF (Select the
	 //     VREF pin.)
	 // D23REFSL (DAC2 and DAC3 Reference Voltage Select) = VREF (Select the
	 //     VREF pin.)
	 // D1SRC (DAC1 Data Source) = DAC1 (Input = DAC1H:DAC1L.)
	 // D3SRC (DAC3 Data Source) = DAC3 (Input = DAC3H:DAC3L.)
	 // D1AMEN (DAC1 Alternating Mode Enable) = NORMAL (DAC1 always updates
	 //     from the data source selected in D1SRC. This mode may be used with any
	 //     trigger source.)
	 // D3AMEN (DAC3 Alternating Mode Enable) = NORMAL (DAC3 always updates
	 //     from the data source selected in D3SRC. This mode may be used with any
	 //     trigger source.)
	 */
	DACGCF0 = DACGCF0_D01REFSL__VREF | DACGCF0_D23REFSL__VREF
			| DACGCF0_D1SRC__DAC1 | DACGCF0_D3SRC__DAC3 | DACGCF0_D1AMEN__NORMAL
			| DACGCF0_D3AMEN__NORMAL;
	// [DACGCF0 - DAC Global Configuration 0]$

	// $[DACGCF2 - DAC Global Configuration 2]
	// [DACGCF2 - DAC Global Configuration 2]$

}

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
	CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	while (CLKSEL & CLKSEL_DIVRDY__BMASK == CLKSEL_DIVRDY__NOT_READY)
		;
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
	 // TH0 (Timer 0 High Byte) = 0x01
	 */
	TH0 = (0x01 << TH0_TH0__SHIFT);
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
	 // TMR4H (Timer 4 High Byte) = 0xFF
	 */
	TMR4H = (0xFF << TMR4H_TMR4H__SHIFT);
	// [TMR4H - Timer 4 High Byte]$

	// $[TMR4L - Timer 4 Low Byte]
	/*
	 // TMR4L (Timer 4 Low Byte) = 0xFF
	 */
	TMR4L = (0xFF << TMR4L_TMR4L__SHIFT);
	// [TMR4L - Timer 4 Low Byte]$

	// $[TMR4RLH - Timer 4 Reload High Byte]
	/*
	 // TMR4RLH (Timer 4 Reload High Byte) = 0xFF
	 */
	TMR4RLH = (0xFE << TMR4RLH_TMR4RLH__SHIFT);
	// [TMR4RLH - Timer 4 Reload High Byte]$

	// $[TMR4RLL - Timer 4 Reload Low Byte]
	/*
	 // TMR4RLL (Timer 4 Reload Low Byte) = 0x5D
	 */
	TMR4RLL = (0x68 << TMR4RLL_TMR4RLL__SHIFT);
	// [TMR4RLL - Timer 4 Reload Low Byte]$

	TMR4RL = -SYSCLK / SAMPLE_RATE_DAC;

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

extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void) {
	// $[CKCON0 - Clock Control 0]
	/*
	 // SCA (Timer 0/1 Prescale) = SYSCLK_DIV_48 (System clock divided by 48.)
	 // T0M (Timer 0 Clock Select) = PRESCALE (Counter/Timer 0 uses the clock
	 //     defined by the prescale field, SCA.)
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
	SFRPAGE = 0x00;
	CKCON0 = CKCON0_SCA__SYSCLK_DIV_48 | CKCON0_T0M__PRESCALE
			| CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__EXTERNAL_CLOCK
			| CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
			| CKCON0_T1M__PRESCALE;
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
	SFRPAGE = 0x10;
	CKCON1 = CKCON1_T4MH__EXTERNAL_CLOCK | CKCON1_T4ML__SYSCLK
			| CKCON1_T5MH__EXTERNAL_CLOCK | CKCON1_T5ML__EXTERNAL_CLOCK;
	// [CKCON1 - Clock Control 1]$

	// $[TMOD - Timer 0/1 Mode]
	/*
	 // T0M (Timer 0 Mode Select) = MODE2 (Mode 2, 8-bit Counter/Timer with
	 //     Auto-Reload)
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
	SFRPAGE = 0x00;
	TMOD = TMOD_T0M__MODE2 | TMOD_T1M__MODE0 | TMOD_CT0__TIMER
			| TMOD_GATE0__DISABLED | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
	// [TMOD - Timer 0/1 Mode]$

	// $[TCON - Timer 0/1 Control]
	/*
	 // TR0 (Timer 0 Run Control) = RUN (Start Timer 0 running.)
	 */
	TCON |= TCON_TR0__RUN;
	// [TCON - Timer 0/1 Control]$

}

extern void SPI_0_enter_DefaultMode_from_RESET(void) {
	// $[SPI0CKR - SPI0 Clock Rate]
	/*
	 // SPI0CKR (SPI0 Clock Rate) = 0x0B
	 */
	SPI0CKR = (0x0B << SPI0CKR_SPI0CKR__SHIFT);
	// [SPI0CKR - SPI0 Clock Rate]$

	// $[SPI0CFG - SPI0 Configuration]
	/*
	 // MSTEN (Master Mode Enable) = MASTER_ENABLED (Enable master mode.
	 //     Operate as a master.)
	 */
	SPI0CFG |= SPI0CFG_MSTEN__MASTER_ENABLED;
	// [SPI0CFG - SPI0 Configuration]$

	// $[SPI0CN0 - SPI0 Control]
	/*
	 // SPIEN (SPI0 Enable) = ENABLED (Enable the SPI module.)
	 // NSSMD (Slave Select Mode) = 3_WIRE (3-Wire Slave or 3-Wire Master
	 //     Mode. NSS signal is not routed to a port pin.)
	 */
	SPI0CN0 &= ~SPI0CN0_NSSMD__FMASK;
	SPI0CN0 |= SPI0CN0_SPIEN__ENABLED;
	// [SPI0CN0 - SPI0 Control]$

	// $[SPI0PCF - SPI0 Pin Configuration]
	// [SPI0PCF - SPI0 Pin Configuration]$

}

extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void) {
	// $[EIE1 - Extended Interrupt Enable 1]
	/*
	 // EADC0 (ADC0 Conversion Complete Interrupt Enable) = DISABLED (Disable
	 //     ADC0 Conversion Complete interrupt.)
	 // EWADC0 (ADC0 Window Comparison Interrupt Enable) = DISABLED (Disable
	 //     ADC0 Window Comparison interrupt.)
	 // ECP0 (Comparator0 (CP0) Interrupt Enable) = DISABLED (Disable CP0
	 //     interrupts.)
	 // ECP1 (Comparator1 (CP1) Interrupt Enable) = DISABLED (Disable CP1
	 //     interrupts.)
	 // EMAT (Port Match Interrupts Enable) = ENABLED (Enable interrupt
	 //     requests generated by a Port Match.)
	 // EPCA0 (Programmable Counter Array (PCA0) Interrupt Enable) = DISABLED
	 //     (Disable all PCA0 interrupts.)
	 // ESMB0 (SMBus (SMB0) Interrupt Enable) = DISABLED (Disable all SMB0
	 //     interrupts.)
	 // ET3 (Timer 3 Interrupt Enable) = ENABLED (Enable interrupt requests
	 //     generated by the TF3L or TF3H flags.)
	 */
	EIE1 = EIE1_EADC0__DISABLED | EIE1_EWADC0__DISABLED | EIE1_ECP0__DISABLED
			| EIE1_ECP1__DISABLED | EIE1_EMAT__ENABLED | EIE1_EPCA0__DISABLED
			| EIE1_ESMB0__DISABLED | EIE1_ET3__ENABLED;
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
	EIE2 = EIE2_CL0__DISABLED | EIE2_EI2C0__DISABLED | EIE2_ET4__ENABLED
			| EIE2_ET5__DISABLED | EIE2_ES1__DISABLED;
	// [EIE2 - Extended Interrupt Enable 2]$

	// $[EIP1H - Extended Interrupt Priority 1 High]
	/*
	 // PHADC0 (ADC0 Conversion Complete Interrupt Priority Control MSB) = LOW
	 //     (ADC0 Conversion Complete interrupt priority MSB set to low.)
	 // PHWADC0 (ADC0 Window Comparator Interrupt Priority Control MSB) = LOW
	 //     (ADC0 Window interrupt priority MSB set to low.)
	 // PHCP0 (Comparator0 (CP0) Interrupt Priority Control MSB) = LOW (CP0
	 //     interrupt priority MSB set to low.)
	 // PHCP1 (Comparator1 (CP1) Interrupt Priority Control MSB) = LOW (CP1
	 //     interrupt priority MSB set to low.)
	 // PHMAT (Port Match Interrupt Priority Control MSB) = HIGH (Port Match
	 //     interrupt priority MSB set to high.)
	 // PHPCA0 (Programmable Counter Array (PCA0) Interrupt Priority Control
	 //     MSB) = LOW (PCA0 interrupt priority MSB set to low.)
	 // PHSMB0 (SMBus (SMB0) Interrupt Priority Control MSB) = LOW (SMB0
	 //     interrupt priority MSB set to low.)
	 // PHT3 (Timer 3 Interrupt Priority Control MSB) = HIGH (Timer 3
	 //     interrupt priority MSB set to high.)
	 */
	SFRPAGE = 0x10;
	EIP1H = EIP1H_PHADC0__LOW | EIP1H_PHWADC0__LOW | EIP1H_PHCP0__LOW
			| EIP1H_PHCP1__LOW | EIP1H_PHMAT__HIGH | EIP1H_PHPCA0__LOW
			| EIP1H_PHSMB0__LOW | EIP1H_PHT3__HIGH;
	// [EIP1H - Extended Interrupt Priority 1 High]$

	// $[EIP1 - Extended Interrupt Priority 1 Low]
	// [EIP1 - Extended Interrupt Priority 1 Low]$

	// $[EIP2 - Extended Interrupt Priority 2]
	/*
	 // PCL0 (Configurable Logic (CL0) Interrupt Priority Control LSB) = LOW
	 //     (CL0 interrupt priority LSB set to low.)
	 // PI2C0 (I2C0 Slave Interrupt Priority Control LSB) = LOW (I2C0 Slave
	 //     interrupt priority LSB set to low.)
	 // PT4 (Timer 4 Interrupt Priority Control LSB) = HIGH (Timer 4 interrupt
	 //     priority LSB set to high.)
	 // PT5 (Timer 5 Interrupt Priority Control LSB) = LOW (Timer 5 interrupt
	 //     priority LSB set to low.)
	 // PS1 (UART1 Interrupt Priority Control LSB) = LOW (UART1 interrupt
	 //     priority LSB set to low.)
	 */
	EIP2 = EIP2_PCL0__LOW | EIP2_PI2C0__LOW | EIP2_PT4__HIGH | EIP2_PT5__LOW
			| EIP2_PS1__LOW;
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
	 // ET2 (Timer 2 Interrupt Enable) = ENABLED (Enable interrupt requests
	 //     generated by the TF2L or TF2H flags.)
	 // ES0 (UART0 Interrupt Enable) = DISABLED (Disable UART0 interrupt.)
	 */
	SFRPAGE = 0x00;
	IE = IE_EA__DISABLED | IE_EX0__DISABLED | IE_EX1__DISABLED
			| IE_ESPI0__ENABLED | IE_ET0__DISABLED | IE_ET1__DISABLED
			| IE_ET2__ENABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

	// $[IPH - Interrupt Priority High]
	// [IPH - Interrupt Priority High]$

}

extern void ADC_0_enter_DefaultMode_from_RESET(void) {
	// $[ADC0CN2 - ADC0 Control 2]
	// [ADC0CN2 - ADC0 Control 2]$

	// $[ADC0CN1 - ADC0 Control 1]
	/*
	 // ADBITS (Resolution Control) = 10_BIT (ADC0 operates in 10-bit mode.)
	 // ADSJST (Accumulator Shift and Justify) = RIGHT_NO_SHIFT (Right
	 //     justified. No shifting applied.)
	 // ADRPT (Repeat Count) = ACC_1 (Perform and Accumulate 1 conversion.)
	 */
	ADC0CN1 = ADC0CN1_ADBITS__10_BIT | ADC0CN1_ADSJST__RIGHT_NO_SHIFT
			| ADC0CN1_ADRPT__ACC_1;
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
	 // REFSL (Voltage Reference Select) = VDD_PIN (The ADC0 voltage reference
	 //     is the VDD pin.)
	 // ADPWR (Power Up Delay Time) = 0x1F
	 */
	ADC0CF2 = ADC0CF2_GNDSL__GND_PIN | ADC0CF2_REFSL__VDD_PIN
			| (0x1F << ADC0CF2_ADPWR__SHIFT);
	// [ADC0CF2 - ADC0 Power Control]$

	// $[ADC0CF0 - ADC0 Configuration]
	/*
	 // ADCLKSEL (ADC Clock Select) = SYSCLK (ADCCLK = SYSCLK.)
	 // ADSC (SAR Clock Divider) = 0x01
	 */
	ADC0CF0 = ADC0CF0_ADCLKSEL__SYSCLK | (0x01 << ADC0CF0_ADSC__SHIFT);
	// [ADC0CF0 - ADC0 Configuration]$

	// $[ADC0CF1 - ADC0 Configuration]
	/*
	 // ADLPM (Low Power Mode Enable) = LP_DISABLED (Disable low power
	 //     mode.)
	 // ADTK (Conversion Tracking Time) = 0x01
	 */
	ADC0CF1 = ADC0CF1_ADLPM__LP_DISABLED | (0x01 << ADC0CF1_ADTK__SHIFT);
	// [ADC0CF1 - ADC0 Configuration]$

	// $[ADC0ASAL - ADC0 Autoscan Start Address Low Byte]
	// [ADC0ASAL - ADC0 Autoscan Start Address Low Byte]$

	// $[ADC0GTH - ADC0 Greater-Than High Byte]
	// [ADC0GTH - ADC0 Greater-Than High Byte]$

	// $[ADC0GTL - ADC0 Greater-Than Low Byte]
	// [ADC0GTL - ADC0 Greater-Than Low Byte]$

	// $[ADC0LTH - ADC0 Less-Than High Byte]
	// [ADC0LTH - ADC0 Less-Than High Byte]$

	// $[ADC0LTL - ADC0 Less-Than Low Byte]
	// [ADC0LTL - ADC0 Less-Than Low Byte]$

	// $[ADC0ASCF - ADC0 Autoscan Configuration]
	// [ADC0ASCF - ADC0 Autoscan Configuration]$

	// $[ADC0CN0 - ADC0 Control 0]
	/*
	 // ADEN (ADC Enable) = ENABLED (Enable ADC0 (active and ready for data
	 //     conversions).)
	 */
	ADC0CN0 |= ADC0CN0_ADEN__ENABLED;
	// [ADC0CN0 - ADC0 Control 0]$

}

extern void PORTS_3_enter_DefaultMode_from_RESET(void) {
	// $[P3 - Port 3 Pin Latch]
	// [P3 - Port 3 Pin Latch]$

	// $[P3MDOUT - Port 3 Output Mode]
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

