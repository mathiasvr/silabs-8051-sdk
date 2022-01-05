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

//==============================================================================
// enter_DefaultMode_from_RESET
//==============================================================================
extern void enter_DefaultMode_from_RESET(void) {
	// $[Config Calls]
	WDT_0_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	ADC_0_enter_DefaultMode_from_RESET();
	VREF_0_enter_DefaultMode_from_RESET();
	RSTSRC_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	TIMER01_0_enter_DefaultMode_from_RESET();
	TIMER16_2_enter_DefaultMode_from_RESET();
	TIMER_SETUP_0_enter_DefaultMode_from_RESET();
	UART_0_enter_DefaultMode_from_RESET();
	INTERRUPT_0_enter_DefaultMode_from_RESET();
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
	 // B2 (Port 0 Bit 2 Skip) = NOT_SKIPPED (P0.2 pin is not skipped by the
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
			| P0SKIP_B2__NOT_SKIPPED | P0SKIP_B3__NOT_SKIPPED
			| P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
			| P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__NOT_SKIPPED;
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
// PBCFG_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PBCFG_0_enter_DefaultMode_from_RESET(void) {
	// $[XBR0 - Port I/O Crossbar 0]
	/*
	 // URT0E (UART I/O Output Enable) = ENABLED (UART TX, RX routed to Port
	 //     pins P0.4 and P0.5.)
	 // SPI0E (SPI I/O Enable) = DISABLED (SPI I/O unavailable at Port pins.)
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
	XBR0 = XBR0_URT0E__ENABLED | XBR0_SPI0E__DISABLED | XBR0_SMB0E__DISABLED
			| XBR0_CP0E__DISABLED | XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
			| XBR0_CP1AE__DISABLED | XBR0_SYSCKE__DISABLED;
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR1 - Port I/O Crossbar 1]
	// [XBR1 - Port I/O Crossbar 1]$

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

}

//================================================================================
// ADC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void ADC_0_enter_DefaultMode_from_RESET(void) {
	// $[ADC0GTH - ADC0 Greater-Than High Byte]
	// [ADC0GTH - ADC0 Greater-Than High Byte]$

	// $[ADC0GTL - ADC0 Greater-Than Low Byte]
	// [ADC0GTL - ADC0 Greater-Than Low Byte]$

	// $[ADC0LTH - ADC0 Less-Than High Byte]
	// [ADC0LTH - ADC0 Less-Than High Byte]$

	// $[ADC0LTL - ADC0 Less-Than Low Byte]
	// [ADC0LTL - ADC0 Less-Than Low Byte]$

	// $[ADC0PWR - ADC0 Power Control]
	/*
	 // ADPWR (Burst Mode Power Up Time) = 0x00
	 // ADLPM (Low Power Mode Enable) = LP_BUFFER_DISABLED (Disable low power
	 //     mode.)
	 // ADMXLP (Mux and Reference Low Power Mode Enable) =
	 //     LP_MUX_VREF_DISABLED (Low power mode disabled.)
	 // ADBIAS (Bias Power Select) = MODE3 (Select bias current mode 3 (SARCLK
	 //     <= 4 MHz).)
	 */
	ADC0PWR = (0x00 << ADC0PWR_ADPWR__SHIFT) | ADC0PWR_ADLPM__LP_BUFFER_DISABLED
			| ADC0PWR_ADMXLP__LP_MUX_VREF_DISABLED | ADC0PWR_ADBIAS__MODE3;
	// [ADC0PWR - ADC0 Power Control]$

	// $[ADC0TK - ADC0 Burst Mode Track Time]
	// [ADC0TK - ADC0 Burst Mode Track Time]$

	// $[ADC0MX - ADC0 Multiplexer Selection]
	/*
	 // ADC0MX (AMUX0 Positive Input Selection) = TEMP (Select ADC0.16.)
	 */
	ADC0MX = ADC0MX_ADC0MX__TEMP;
	// [ADC0MX - ADC0 Multiplexer Selection]$

	// $[ADC0AC - ADC0 Accumulator Configuration]
	/*
	 // ADSJST (Accumulator Shift and Justify) = LEFT_NO_SHIFT (Left
	 //     justified. No shifting applied.)
	 // AD12BE (12-Bit Mode Enable) = 12_BIT_DISABLED (Disable 12-bit mode.)
	 // ADAE (Accumulate Enable) = ACC_DISABLED (ADC0H:ADC0L contain the
	 //     result of the latest conversion when Burst Mode is disabled.)
	 // ADRPT (Repeat Count) = ACC_1 (Perform and Accumulate 1 conversion (not
	 //     used in 12-bit mode).)
	 */
	ADC0AC = ADC0AC_ADSJST__LEFT_NO_SHIFT | ADC0AC_AD12BE__12_BIT_DISABLED
			| ADC0AC_ADAE__ACC_DISABLED | ADC0AC_ADRPT__ACC_1;
	// [ADC0AC - ADC0 Accumulator Configuration]$

	// $[ADC0CF - ADC0 Configuration]
	/*
	 // ADSC (SAR Clock Divider) = 0x1F
	 // AD8BE (8-Bit Mode Enable) = NORMAL (ADC0 operates in 10-bit or 12-bit
	 //     mode (normal operation).)
	 // ADGN (Gain Control) = GAIN_1 (The on-chip PGA gain is 1.)
	 // ADTM (Track Mode) = TRACK_NORMAL (Normal Track Mode. When ADC0 is
	 //     enabled, conversion begins immediately following the start-of-
	 //     conversion signal.)
	 */
	ADC0CF = (0x1F << ADC0CF_ADSC__SHIFT) | ADC0CF_AD8BE__NORMAL
			| ADC0CF_ADGN__GAIN_1 | ADC0CF_ADTM__TRACK_NORMAL;
	// [ADC0CF - ADC0 Configuration]$

	// $[ADC0CN1 - ADC0 Control 1]
	// [ADC0CN1 - ADC0 Control 1]$

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

//================================================================================
// TIMER01_0_enter_DefaultMode_from_RESET
//================================================================================
extern void TIMER01_0_enter_DefaultMode_from_RESET(void) {
	// $[Timer Initialization]
	//Save Timer Configuration
	uint8_t TCON_save;
	TCON_save = TCON;
	//Stop Timers
	TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;

	// [Timer Initialization]$

	// $[TH0 - Timer 0 High Byte]
	// [TH0 - Timer 0 High Byte]$

	// $[TL0 - Timer 0 Low Byte]
	// [TL0 - Timer 0 Low Byte]$

	// $[TH1 - Timer 1 High Byte]
	/*
	 // TH1 (Timer 1 High Byte) = 0xF7
	 */
	TH1 = (0xF7 << TH1_TH1__SHIFT);
	// [TH1 - Timer 1 High Byte]$

	// $[TL1 - Timer 1 Low Byte]
	/*
	 // TL1 (Timer 1 Low Byte) = 0x95
	 */
	TL1 = (0x95 << TL1_TL1__SHIFT);
	// [TL1 - Timer 1 Low Byte]$

	// $[Timer Restoration]
	//Restore Timer Configuration
	TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

	// [Timer Restoration]$

}

//================================================================================
// TIMER16_2_enter_DefaultMode_from_RESET
//================================================================================
extern void TIMER16_2_enter_DefaultMode_from_RESET(void) {
	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR2CN0_TR2_save;
	TMR2CN0_TR2_save = TMR2CN0 & TMR2CN0_TR2__BMASK;
	// Stop Timer
	TMR2CN0 &= ~(TMR2CN0_TR2__BMASK);
	// [Timer Initialization]$

	// $[TMR2CN - Timer 2 Control]
	// [TMR2CN - Timer 2 Control]$

	// $[TMR2H - Timer 2 High Byte]
	/*
	 // TMR2H (Timer 2 High Byte) = 0xF6
	 */
	TMR2H = (0xF6 << TMR2H_TMR2H__SHIFT);
	// [TMR2H - Timer 2 High Byte]$

	// $[TMR2L - Timer 2 Low Byte]
	/*
	 // TMR2L (Timer 2 Low Byte) = 0x6D
	 */
	TMR2L = (0x6D << TMR2L_TMR2L__SHIFT);
	// [TMR2L - Timer 2 Low Byte]$

	// $[TMR2RLH - Timer 2 Reload High Byte]
	/*
	 // TMR2RLH (Timer 2 Reload High Byte) = 0xA0
	 */
	TMR2RLH = (0xA0 << TMR2RLH_TMR2RLH__SHIFT);
	// [TMR2RLH - Timer 2 Reload High Byte]$

	// $[TMR2RLL - Timer 2 Reload Low Byte]
	/*
	 // TMR2RLL (Timer 2 Reload Low Byte) = 0x4C
	 */
	TMR2RLL = (0x4C << TMR2RLL_TMR2RLL__SHIFT);
	// [TMR2RLL - Timer 2 Reload Low Byte]$

	// $[TMR2CN]
	/*
	 // TR2 (Timer 2 Run Control) = RUN (Start Timer 2 running.)
	 */
	TMR2CN0 |= TMR2CN0_TR2__RUN;
	// [TMR2CN]$

	// $[Timer Restoration]
	// Restore Timer Configuration
	TMR2CN0 |= TMR2CN0_TR2_save;
	// [Timer Restoration]$

	// $[TMR2CN0 - Timer 2 Control]
	// [TMR2CN0 - Timer 2 Control]$

	// $[TMR2CN0]
	/*
	 // TR2 (Timer 2 Run Control) = RUN (Start Timer 2 running.)
	 */
	TMR2CN0 |= TMR2CN0_TR2__RUN;
	// [TMR2CN0]$

}

//================================================================================
// TIMER_SETUP_0_enter_DefaultMode_from_RESET
//================================================================================
extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void) {
	// $[CKCON - Clock Control]
	/*
	 // SCA (Timer 0/1 Prescale) = SYSCLK_DIV_12 (System clock divided by 12.)
	 // T0M (Timer 0 Clock Select) = PRESCALE (Counter/Timer 0 uses the clock
	 //     defined by the prescale field, SCA.)
	 // T2MH (Timer 2 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 2 high
	 //     byte uses the clock defined by the T2XCLK bit in TMR2CN.)
	 // T2ML (Timer 2 Low Byte Clock Select) = SYSCLK (Timer 2 low byte uses
	 //     the system clock.)
	 // T3MH (Timer 3 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 high
	 //     byte uses the clock defined by the T3XCLK bit in TMR3CN.)
	 // T3ML (Timer 3 Low Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 low
	 //     byte uses the clock defined by the T3XCLK bit in TMR3CN.)
	 // T1M (Timer 1 Clock Select) = PRESCALE (Timer 1 uses the clock defined
	 //     by the prescale field, SCA.)
	 */
	CKCON0 = CKCON0_SCA__SYSCLK_DIV_12 | CKCON0_T0M__PRESCALE
			| CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__SYSCLK
			| CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
			| CKCON0_T1M__PRESCALE;
	// [CKCON - Clock Control]$

	// $[TMOD - Timer 0/1 Mode]
	/*
	 // T0M (Timer 0 Mode Select) = MODE0 (Mode 0, 13-bit Counter/Timer)
	 // T1M (Timer 1 Mode Select) = MODE2 (Mode 2, 8-bit Counter/Timer with
	 //     Auto-Reload)
	 // CT0 (Counter/Timer 0 Select) = TIMER (Timer Mode. Timer 0 increments
	 //     on the clock defined by T0M in the CKCON0 register.)
	 // GATE0 (Timer 0 Gate Control) = DISABLED (Timer 0 enabled when TR0 = 1
	 //     irrespective of INT0 logic level.)
	 // CT1 (Counter/Timer 1 Select) = TIMER (Timer Mode. Timer 1 increments
	 //     on the clock defined by T1M in the CKCON0 register.)
	 // GATE1 (Timer 1 Gate Control) = DISABLED (Timer 1 enabled when TR1 = 1
	 //     irrespective of INT1 logic level.)
	 */
	TMOD = TMOD_T0M__MODE0 | TMOD_T1M__MODE2 | TMOD_CT0__TIMER
			| TMOD_GATE0__DISABLED | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
	// [TMOD - Timer 0/1 Mode]$

	// $[TCON - Timer 0/1 Control]
	/*
	 // TR1 (Timer 1 Run Control) = RUN (Start Timer 1 running.)
	 */
	TCON |= TCON_TR1__RUN;
	// [TCON - Timer 0/1 Control]$

	// $[CKCON0 - Clock Control 0]
	/*
	 // SCA (Timer 0/1 Prescale) = SYSCLK_DIV_12 (System clock divided by 12.)
	 // T0M (Timer 0 Clock Select) = PRESCALE (Counter/Timer 0 uses the clock
	 //     defined by the prescale field, SCA.)
	 // T2MH (Timer 2 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 2 high
	 //     byte uses the clock defined by T2XCLK in TMR2CN0.)
	 // T2ML (Timer 2 Low Byte Clock Select) = SYSCLK (Timer 2 low byte uses
	 //     the system clock.)
	 // T3MH (Timer 3 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 high
	 //     byte uses the clock defined by T3XCLK in TMR3CN0.)
	 // T3ML (Timer 3 Low Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 low
	 //     byte uses the clock defined by T3XCLK in TMR3CN0.)
	 // T1M (Timer 1 Clock Select) = PRESCALE (Timer 1 uses the clock defined
	 //     by the prescale field, SCA.)
	 */
	CKCON0 = CKCON0_SCA__SYSCLK_DIV_12 | CKCON0_T0M__PRESCALE
			| CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__SYSCLK
			| CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
			| CKCON0_T1M__PRESCALE;
	// [CKCON0 - Clock Control 0]$

}

//================================================================================
// UART_0_enter_DefaultMode_from_RESET
//================================================================================
extern void UART_0_enter_DefaultMode_from_RESET(void) {
	// $[SCON0 - UART0 Serial Port Control]
	/*
	 // REN (Receive Enable) = RECEIVE_ENABLED (UART0 reception enabled.)
	 */
	SCON0 |= SCON0_REN__RECEIVE_ENABLED;
	// [SCON0 - UART0 Serial Port Control]$

}

//================================================================================
// VREF_0_enter_DefaultMode_from_RESET
//================================================================================
extern void VREF_0_enter_DefaultMode_from_RESET(void) {
	// $[REF0CN - Voltage Reference Control]
	/*
	 // TEMPE (Temperature Sensor Enable) = TEMP_ENABLED (Enable the
	 //     Temperature Sensor.)
	 // GNDSL (Analog Ground Reference) = GND_PIN (The ADC0 ground reference
	 //     is the GND pin.)
	 // IREFLVL (Internal Voltage Reference Level) = 1P65 (The internal
	 //     reference operates at 1.65 V nominal.)
	 // REFSL (Voltage Reference Select) = INTERNAL_VREF (The ADC0 voltage
	 //     reference is the internal voltage reference.)
	 */
	REF0CN = REF0CN_TEMPE__TEMP_ENABLED | REF0CN_GNDSL__GND_PIN
			| REF0CN_IREFLVL__1P65 | REF0CN_REFSL__INTERNAL_VREF;
	// [REF0CN - Voltage Reference Control]$

}

//================================================================================
// RSTSRC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void RSTSRC_0_enter_DefaultMode_from_RESET(void) {
	// $[RSTSRC - Reset Source]
	/*
	 // PORSF (Power-On / Supply Monitor Reset Flag, and Supply Monitor Reset
	 //     Enable) = SET (A power-on or supply monitor reset occurred.)
	 // MCDRSF (Missing Clock Detector Enable and Flag) = SET (A missing clock
	 //     detector reset occurred.)
	 // C0RSEF (Comparator0 Reset Enable and Flag) = NOT_SET (A Comparator 0
	 //     reset did not occur.)
	 */
	RSTSRC = RSTSRC_PORSF__SET | RSTSRC_MCDRSF__SET | RSTSRC_C0RSEF__NOT_SET;
	// [RSTSRC - Reset Source]$

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
	 // EMAT (Port Match Interrupts Enable) = DISABLED (Disable all Port Match
	 //     interrupts.)
	 // EPCA0 (Programmable Counter Array (PCA0) Interrupt Enable) = DISABLED
	 //     (Disable all PCA0 interrupts.)
	 // ESMB0 (SMBus (SMB0) Interrupt Enable) = DISABLED (Disable all SMB0
	 //     interrupts.)
	 // ET3 (Timer 3 Interrupt Enable) = DISABLED (Disable Timer 3
	 //     interrupts.)
	 */
	EIE1 = EIE1_EADC0__ENABLED | EIE1_EWADC0__DISABLED | EIE1_ECP0__DISABLED
			| EIE1_ECP1__DISABLED | EIE1_EMAT__DISABLED | EIE1_EPCA0__DISABLED
			| EIE1_ESMB0__DISABLED | EIE1_ET3__DISABLED;
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
	 // ET2 (Timer 2 Interrupt Enable) = DISABLED (Disable Timer 2 interrupt.)
	 // ES0 (UART0 Interrupt Enable) = DISABLED (Disable UART0 interrupt.)
	 */
	IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED
			| IE_ESPI0__DISABLED | IE_ET0__DISABLED | IE_ET1__DISABLED
			| IE_ET2__DISABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

}

// $[Library Includes]
// [Library Includes]$
