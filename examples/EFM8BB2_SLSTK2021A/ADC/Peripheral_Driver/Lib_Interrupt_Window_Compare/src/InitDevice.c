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
	PORTS_1_enter_DefaultMode_from_RESET();
	PORTS_2_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	ADC_0_enter_DefaultMode_from_RESET();
	TIMER16_2_enter_DefaultMode_from_RESET();
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
	 // B5 (Port 1 Bit 5 Output Mode) = PUSH_PULL (P1.5 output is push-pull.)
	 // B6 (Port 1 Bit 6 Output Mode) = PUSH_PULL (P1.6 output is push-pull.)
	 // B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
	 //     drain.)
	 */
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
			| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN
			| P1MDOUT_B4__PUSH_PULL | P1MDOUT_B5__PUSH_PULL
			| P1MDOUT_B6__PUSH_PULL | P1MDOUT_B7__OPEN_DRAIN;
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
	 // B7 (Port 1 Bit 7 Input Mode) = ANALOG (P1.7 pin is configured for
	 //     analog mode.)
	 */
	P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__DIGITAL | P1MDIN_B2__DIGITAL
			| P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__DIGITAL
			| P1MDIN_B6__DIGITAL | P1MDIN_B7__ANALOG;
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

	// $[P1MASK - Port 1 Mask]
	// [P1MASK - Port 1 Mask]$

	// $[P1MAT - Port 1 Match]
	// [P1MAT - Port 1 Match]$

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
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR1 - Port I/O Crossbar 1]
	// [XBR1 - Port I/O Crossbar 1]$

}

extern void ADC_0_enter_DefaultMode_from_RESET(void) {
	// $[ADC0CN1 - ADC0 Control 1]
	// [ADC0CN1 - ADC0 Control 1]$

	// $[ADC0MX - ADC0 Multiplexer Selection]
	/*
	 // ADC0MX (AMUX0 Positive Input Selection) = ADC0P15 (Select ADC0.15.)
	 */
	ADC0MX = ADC0MX_ADC0MX__ADC0P15;
	// [ADC0MX - ADC0 Multiplexer Selection]$

	// $[ADC0CF - ADC0 Configuration]
	/*
	 // ADSC (SAR Clock Divider) = 0x00
	 // AD8BE (8-Bit Mode Enable) = NORMAL (ADC0 operates in 10-bit or 12-bit
	 //     mode (normal operation).)
	 // ADGN (Gain Control) = GAIN_0P5 (The on-chip PGA gain is 0.5.)
	 // ADTM (Track Mode) = TRACK_NORMAL (Normal Track Mode. When ADC0 is
	 //     enabled, conversion begins immediately following the start-of-
	 //     conversion signal.)
	 */
	ADC0CF = (0x00 << ADC0CF_ADSC__SHIFT) | ADC0CF_AD8BE__NORMAL
			| ADC0CF_ADGN__GAIN_0P5 | ADC0CF_ADTM__TRACK_NORMAL;
	// [ADC0CF - ADC0 Configuration]$

	// $[ADC0AC - ADC0 Accumulator Configuration]
	// [ADC0AC - ADC0 Accumulator Configuration]$

	// $[ADC0TK - ADC0 Burst Mode Track Time]
	// [ADC0TK - ADC0 Burst Mode Track Time]$

	// $[ADC0PWR - ADC0 Power Control]
	/*
	 // ADPWR (Burst Mode Power Up Time) = 0x0F
	 // ADLPM (Low Power Mode Enable) = LP_BUFFER_ENABLED (Enable low power
	 //     mode (requires extended tracking time).)
	 // ADMXLP (Mux and Reference Low Power Mode Enable) = LP_MUX_VREF_ENABLED
	 //     (Low power mode enabled (SAR clock < 4 MHz).)
	 // ADBIAS (Bias Power Select) = MODE3 (Select bias current mode 3 (SARCLK
	 //     <= 4 MHz).)
	 */
	ADC0PWR = (0x0F << ADC0PWR_ADPWR__SHIFT) | ADC0PWR_ADLPM__LP_BUFFER_ENABLED
			| ADC0PWR_ADMXLP__LP_MUX_VREF_ENABLED | ADC0PWR_ADBIAS__MODE3;
	// [ADC0PWR - ADC0 Power Control]$

	// $[ADC0GTH - ADC0 Greater-Than High Byte]
	// [ADC0GTH - ADC0 Greater-Than High Byte]$

	// $[ADC0GTL - ADC0 Greater-Than Low Byte]
	// [ADC0GTL - ADC0 Greater-Than Low Byte]$

	// $[ADC0LTH - ADC0 Less-Than High Byte]
	/*
	 // ADC0LTH (Less-Than High Byte) = 0xFF
	 */
	ADC0LTH = (0xFF << ADC0LTH_ADC0LTH__SHIFT);
	// [ADC0LTH - ADC0 Less-Than High Byte]$

	// $[ADC0LTL - ADC0 Less-Than Low Byte]
	/*
	 // ADC0LTL (Less-Than Low Byte) = 0xFF
	 */
	ADC0LTL = (0xFF << ADC0LTL_ADC0LTL__SHIFT);
	// [ADC0LTL - ADC0 Less-Than Low Byte]$

	// $[ADC0CN0 - ADC0 Control 0]
	/*
	 // ADEN (ADC Enable) = ENABLED (Enable ADC0 (active and ready for data
	 //     conversions).)
	 // ADCM (Start of Conversion Mode Select) = TIMER2 (ADC0 conversion
	 //     initiated on overflow of Timer 2.)
	 */
	ADC0CN0 &= ~ADC0CN0_ADCM__FMASK;
	ADC0CN0 |= ADC0CN0_ADEN__ENABLED | ADC0CN0_ADCM__TIMER2;
	// [ADC0CN0 - ADC0 Control 0]$

}

extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
	// $[HFOSC1 Setup]
	// [HFOSC1 Setup]$

	// $[CLKSEL - Clock Select]
	/*
	 // CLKSL (Clock Source Select) = HFOSC0 (Clock derived from the Internal
	 //     High Frequency Oscillator 0.)
	 // CLKDIV (Clock Source Divider) = SYSCLK_DIV_4 (SYSCLK is equal to
	 //     selected clock source divided by 4.)
	 */
	CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_4;
	while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
		;
	// [CLKSEL - Clock Select]$

}

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
	 // B4 (Port 0 Bit 4 Output Mode) = PUSH_PULL (P0.4 output is push-pull.)
	 // B5 (Port 0 Bit 5 Output Mode) = OPEN_DRAIN (P0.5 output is open-
	 //     drain.)
	 // B6 (Port 0 Bit 6 Output Mode) = OPEN_DRAIN (P0.6 output is open-
	 //     drain.)
	 // B7 (Port 0 Bit 7 Output Mode) = OPEN_DRAIN (P0.7 output is open-
	 //     drain.)
	 */
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN
			| P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__OPEN_DRAIN
			| P0MDOUT_B4__PUSH_PULL | P0MDOUT_B5__OPEN_DRAIN
			| P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__OPEN_DRAIN;
	// [P0MDOUT - Port 0 Output Mode]$

	// $[P0MDIN - Port 0 Input Mode]
	// [P0MDIN - Port 0 Input Mode]$

	// $[P0SKIP - Port 0 Skip]
	// [P0SKIP - Port 0 Skip]$

	// $[P0MASK - Port 0 Mask]
	// [P0MASK - Port 0 Mask]$

	// $[P0MAT - Port 0 Match]
	// [P0MAT - Port 0 Match]$

}

extern void PORTS_2_enter_DefaultMode_from_RESET(void) {
	// $[P2 - Port 2 Pin Latch]
	// [P2 - Port 2 Pin Latch]$

	// $[P2MDOUT - Port 2 Output Mode]
	/*
	 // B0 (Port 2 Bit 0 Output Mode) = PUSH_PULL (P2.0 output is push-pull.)
	 // B1 (Port 2 Bit 1 Output Mode) = OPEN_DRAIN (P2.1 output is open-
	 //     drain.)
	 // B2 (Port 2 Bit 2 Output Mode) = OPEN_DRAIN (P2.2 output is open-
	 //     drain.)
	 // B3 (Port 2 Bit 3 Output Mode) = PUSH_PULL (P2.3 output is push-pull.)
	 */
	P2MDOUT = P2MDOUT_B0__PUSH_PULL | P2MDOUT_B1__OPEN_DRAIN
			| P2MDOUT_B2__OPEN_DRAIN | P2MDOUT_B3__PUSH_PULL;
	// [P2MDOUT - Port 2 Output Mode]$

	// $[P2MDIN - Port 2 Input Mode]
	// [P2MDIN - Port 2 Input Mode]$

	// $[P2SKIP - Port 2 Skip]
	/*
	 // B0 (Port 2 Bit 0 Skip) = SKIPPED (P2.0 pin is skipped by the
	 //     crossbar.)
	 // B1 (Port 2 Bit 1 Skip) = NOT_SKIPPED (P2.1 pin is not skipped by the
	 //     crossbar.)
	 // B2 (Port 2 Bit 2 Skip) = NOT_SKIPPED (P2.2 pin is not skipped by the
	 //     crossbar.)
	 // B3 (Port 2 Bit 3 Skip) = NOT_SKIPPED (P2.3 pin is not skipped by the
	 //     crossbar.)
	 */
	SFRPAGE = 0x20;
	P2SKIP = P2SKIP_B0__SKIPPED | P2SKIP_B1__NOT_SKIPPED
			| P2SKIP_B2__NOT_SKIPPED | P2SKIP_B3__NOT_SKIPPED;
	// [P2SKIP - Port 2 Skip]$

	// $[P2MASK - Port 2 Mask]
	// [P2MASK - Port 2 Mask]$

	// $[P2MAT - Port 2 Match]
	// [P2MAT - Port 2 Match]$

}

extern void TIMER16_2_enter_DefaultMode_from_RESET(void) {
	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR2CN0_TR2_save;
	TMR2CN0_TR2_save = TMR2CN0 & TMR2CN0_TR2__BMASK;
	// Stop Timer
	TMR2CN0 &= ~(TMR2CN0_TR2__BMASK);
	// [Timer Initialization]$

	// $[TMR2CN1 - Timer 2 Control 1]
	// [TMR2CN1 - Timer 2 Control 1]$

	// $[TMR2CN0 - Timer 2 Control]
	// [TMR2CN0 - Timer 2 Control]$

	// $[TMR2H - Timer 2 High Byte]
	/*
	 // TMR2H (Timer 2 High Byte) = 0xFF
	 */
	TMR2H = (0xFF << TMR2H_TMR2H__SHIFT);
	// [TMR2H - Timer 2 High Byte]$

	// $[TMR2L - Timer 2 Low Byte]
	/*
	 // TMR2L (Timer 2 Low Byte) = 0xFF
	 */
	TMR2L = (0xFF << TMR2L_TMR2L__SHIFT);
	// [TMR2L - Timer 2 Low Byte]$

	// $[TMR2RLH - Timer 2 Reload High Byte]
	/*
	 // TMR2RLH (Timer 2 Reload High Byte) = 0xF6
	 */
	TMR2RLH = (0xF6 << TMR2RLH_TMR2RLH__SHIFT);
	// [TMR2RLH - Timer 2 Reload High Byte]$

	// $[TMR2RLL - Timer 2 Reload Low Byte]
	/*
	 // TMR2RLL (Timer 2 Reload Low Byte) = 0x08
	 */
	TMR2RLL = (0x08 << TMR2RLL_TMR2RLL__SHIFT);
	// [TMR2RLL - Timer 2 Reload Low Byte]$

	// $[TMR2CN0]
	/*
	 // TR2 (Timer 2 Run Control) = RUN (Start Timer 2 running.)
	 */
	TMR2CN0 |= TMR2CN0_TR2__RUN;
	// [TMR2CN0]$

	// $[Timer Restoration]
	// Restore Timer Configuration
	TMR2CN0 |= TMR2CN0_TR2_save;
	// [Timer Restoration]$

}

extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void) {
	// $[EIE1 - Extended Interrupt Enable 1]
	/*
	 // EADC0 (ADC0 Conversion Complete Interrupt Enable) = DISABLED (Disable
	 //     ADC0 Conversion Complete interrupt.)
	 // EWADC0 (ADC0 Window Comparison Interrupt Enable) = ENABLED (Enable
	 //     interrupt requests generated by ADC0 Window Compare flag (ADWINT).)
	 // ECP0 (Comparator0 (CP0) Interrupt Enable) = DISABLED (Disable CP0
	 //     interrupts.)
	 // ECP1 (Comparator1 (CP1) Interrupt Enable) = DISABLED (Disable CP1
	 //     interrupts.)
	 // EMAT (Port Match Interrupts Enable) = DISABLED (Disable all Port Match
	 //     interrupts.)
	 // EPCA0 (Programmable Counter Array (PCA0) Interrupt Enable) = DISABLED
	 //     (Disable all PCA0 interrupts.)
	 // ESMB0 (SMBus (SMB0) Interrupt Enable) = DISABLED (Disable all SMB0
	 //     interrupts.)
	 // ET3 (Timer 3 Interrupt Enable) = DISABLED (Disable Timer 3
	 //     interrupts.)
	 */
	EIE1 = EIE1_EADC0__DISABLED | EIE1_EWADC0__ENABLED | EIE1_ECP0__DISABLED
			| EIE1_ECP1__DISABLED | EIE1_EMAT__DISABLED | EIE1_EPCA0__DISABLED
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
	IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED
			| IE_ESPI0__DISABLED | IE_ET0__DISABLED | IE_ET1__DISABLED
			| IE_ET2__DISABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

	// $[IPH - Interrupt Priority High]
	// [IPH - Interrupt Priority High]$

}

