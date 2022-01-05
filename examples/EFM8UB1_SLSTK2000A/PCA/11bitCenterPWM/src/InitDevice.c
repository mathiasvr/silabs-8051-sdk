//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8UB1_Register_Enums.h>
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
	PORTS_2_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	CIP51_0_enter_DefaultMode_from_RESET();
	PCA_0_enter_DefaultMode_from_RESET();
	PCACH_0_enter_DefaultMode_from_RESET();
	PCACH_1_enter_DefaultMode_from_RESET();
	PCACH_2_enter_DefaultMode_from_RESET();
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
	/*
	// CLSEL (Cycle Length Select) = 11_BITS (11 bits.)
	// ECOV (Cycle Overflow Interrupt Enable) = COVF_MASK_ENABLED (A PCA
	//     interrupt will be generated when COVF is set.)
	*/
	PCA0PWM &= ~PCA0PWM_CLSEL__FMASK;
	PCA0PWM |= PCA0PWM_CLSEL__11_BITS
		 | PCA0PWM_ECOV__COVF_MASK_ENABLED;
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
	// ECOM (Channel 0 Comparator Function Enable) = ENABLED (Enable
	//     comparator function.)
	// PWM (Channel 0 Pulse Width Modulation Mode Enable) = ENABLED (Enable
	//     PWM function.)
	// TOG (Channel 0 Toggle Function Enable) = DISABLED (Disable toggle
	//     function.)
	*/
	PCA0CPM0 = PCA0CPM0_CAPN__DISABLED | PCA0CPM0_ECCF__ENABLED | PCA0CPM0_MAT__DISABLED
		 | PCA0CPM0_PWM16__8_BIT | PCA0CPM0_CAPP__DISABLED | PCA0CPM0_ECOM__ENABLED
		 | PCA0CPM0_PWM__ENABLED | PCA0CPM0_TOG__DISABLED;
	// [PCA0CPM0 - PCA Channel 0 Capture/Compare Mode]$

	// $[PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]
	// [PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]$

	// $[PCA0CPH0 - PCA Channel 0 Capture Module High Byte]
	// [PCA0CPH0 - PCA Channel 0 Capture Module High Byte]$

	// $[Auto-reload]
	PCA0PWM |= PCA0PWM_ARSEL__AUTORELOAD;


	PCA0PWM &= ~PCA0PWM_ARSEL__BMASK;
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
	// EMAT (Port Match Interrupts Enable) = DISABLED (Disable all Port Match
	//     interrupts.)
	// EPCA0 (Programmable Counter Array (PCA0) Interrupt Enable) = ENABLED
	//     (Enable interrupt requests generated by PCA0.)
	// ESMB0 (SMBus (SMB0) Interrupt Enable) = DISABLED (Disable all SMB0
	//     interrupts.)
	// ET3 (Timer 3 Interrupt Enable) = DISABLED (Disable Timer 3
	//     interrupts.)
	*/
	EIE1 = EIE1_EADC0__DISABLED | EIE1_EWADC0__DISABLED | EIE1_ECP0__DISABLED
		 | EIE1_ECP1__DISABLED | EIE1_EMAT__DISABLED | EIE1_EPCA0__ENABLED
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
		 | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__DISABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

	// $[IPH - Interrupt Priority High]
	// [IPH - Interrupt Priority High]$


}


extern void PORTS_0_enter_DefaultMode_from_RESET(void) {
	// $[P0 - Port 0 Pin Latch]
	// [P0 - Port 0 Pin Latch]$

	// $[P0MDOUT - Port 0 Output Mode]
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
	SFRPAGE = 0x00;
	P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED
		 | P0SKIP_B3__SKIPPED | P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
		 | P0SKIP_B6__SKIPPED | P0SKIP_B7__SKIPPED;
	// [P0SKIP - Port 0 Skip]$

	// $[P0MASK - Port 0 Mask]
	// [P0MASK - Port 0 Mask]$

	// $[P0MAT - Port 0 Match]
	// [P0MAT - Port 0 Match]$


}

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

