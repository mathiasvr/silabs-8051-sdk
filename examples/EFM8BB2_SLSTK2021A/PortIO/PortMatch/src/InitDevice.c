//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB2_Register_Enums.h>
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
	PORTS_2_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	CIP51_0_enter_DefaultMode_from_RESET();
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
	// [P0MDOUT - Port 0 Output Mode]$

	// $[P0MDIN - Port 0 Input Mode]
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
	// B7 (Port 0 Bit 7 Skip) = NOT_SKIPPED (P0.7 pin is not skipped by the
	//     crossbar.)
	*/
	SFRPAGE = 0x00;
	P0SKIP = P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED | P0SKIP_B2__SKIPPED
		 | P0SKIP_B3__SKIPPED | P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
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
	// [P0MAT - Port 0 Match]$


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
	// B1 (Port 2 Bit 1 Output Mode) = OPEN_DRAIN (P2.1 output is open-
	//     drain.)
	// B2 (Port 2 Bit 2 Output Mode) = OPEN_DRAIN (P2.2 output is open-
	//     drain.)
	// B3 (Port 2 Bit 3 Output Mode) = PUSH_PULL (P2.3 output is push-pull.)
	*/
	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN | P2MDOUT_B2__OPEN_DRAIN
		 | P2MDOUT_B3__PUSH_PULL;
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
	P2SKIP = P2SKIP_B0__NOT_SKIPPED | P2SKIP_B1__NOT_SKIPPED | P2SKIP_B2__NOT_SKIPPED
		 | P2SKIP_B3__SKIPPED;
	// [P2SKIP - Port 2 Skip]$

	// $[P2MASK - Port 2 Mask]
	// [P2MASK - Port 2 Mask]$

	// $[P2MAT - Port 2 Match]
	// [P2MAT - Port 2 Match]$


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
	XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED | XBR2_URT1E__DISABLED
		 | XBR2_URT1RTSE__DISABLED | XBR2_URT1CTSE__DISABLED;
	// [XBR2 - Port I/O Crossbar 2]$

	// $[PRTDRV - Port Drive Strength]
	// [PRTDRV - Port Drive Strength]$

	// $[XBR0 - Port I/O Crossbar 0]
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
	CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	while(CLKSEL & CLKSEL_DIVRDY__BMASK == CLKSEL_DIVRDY__NOT_READY);
	// [CLKSEL - Clock Select]$


}

//================================================================================
// CIP51_0_enter_DefaultMode_from_RESET
//================================================================================
extern void CIP51_0_enter_DefaultMode_from_RESET(void) {
	// $[PFE0CN - Prefetch Engine Control]
	// [PFE0CN - Prefetch Engine Control]$


}

//================================================================================
// INTERRUPT_0_enter_DefaultMode_from_RESET
//================================================================================
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
	// ET3 (Timer 3 Interrupt Enable) = DISABLED (Disable Timer 3
	//     interrupts.)
	*/
	EIE1 = EIE1_EADC0__DISABLED | EIE1_EWADC0__DISABLED | EIE1_ECP0__DISABLED
		 | EIE1_ECP1__DISABLED | EIE1_EMAT__ENABLED | EIE1_EPCA0__DISABLED
		 | EIE1_ESMB0__DISABLED | EIE1_ET3__DISABLED;
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
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

	// $[IPH - Interrupt Priority High]
	// [IPH - Interrupt Priority High]$


}


