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

extern void enter_AccumulateMode_from_SingleSampleMode(void) {
	// $[Config Calls]
	ADC_0_enter_AccumulateMode_from_SingleSampleMode();
	CLOCK_0_enter_AccumulateMode_from_SingleSampleMode();
	// [Config Calls]$

}

extern void ADC_0_enter_AccumulateMode_from_SingleSampleMode(void) {
	// $[ADC0CN1 - ADC0 Control 1]
	// [ADC0CN1 - ADC0 Control 1]$

	// $[ADC0MX - ADC0 Multiplexer Selection]
	// [ADC0MX - ADC0 Multiplexer Selection]$

	// $[ADC0CF - ADC0 Configuration]
	/*
	 // ADSC (SAR Clock Divider) = 0x01
	 // AD8BE (8-Bit Mode Enable) = NORMAL (ADC0 operates in 10-bit or 12-bit
	 //     mode (normal operation).)
	 // ADGN (Gain Control) = GAIN_1 (The on-chip PGA gain is 1.)
	 // ADTM (Track Mode) = TRACK_NORMAL (Normal Track Mode. When ADC0 is
	 //     enabled, conversion begins immediately following the start-of-
	 //     conversion signal.)
	 */
	ADC0CF = (0x01 << ADC0CF_ADSC__SHIFT) | ADC0CF_AD8BE__NORMAL
			| ADC0CF_ADGN__GAIN_1 | ADC0CF_ADTM__TRACK_NORMAL;
	// [ADC0CF - ADC0 Configuration]$

	// $[ADC0AC - ADC0 Accumulator Configuration]
	/*
	 // ADSJST (Accumulator Shift and Justify) = RIGHT_SHIFT_3 (Right
	 //     justified. Shifted right by 3 bits.)
	 // AD12BE (12-Bit Mode Enable) = 12_BIT_DISABLED (Disable 12-bit mode.)
	 // ADAE (Accumulate Enable) = ACC_DISABLED (ADC0H:ADC0L contain the
	 //     result of the latest conversion when Burst Mode is disabled.)
	 // ADRPT (Repeat Count) = ACC_32 (Perform and Accumulate 32 conversions
	 //     (8 conversions in 12-bit mode).)
	 */
	ADC0AC = ADC0AC_ADSJST__RIGHT_SHIFT_3 | ADC0AC_AD12BE__12_BIT_DISABLED
			| ADC0AC_ADAE__ACC_DISABLED | ADC0AC_ADRPT__ACC_32;
	// [ADC0AC - ADC0 Accumulator Configuration]$

	// $[ADC0TK - ADC0 Burst Mode Track Time]
	// [ADC0TK - ADC0 Burst Mode Track Time]$

	// $[ADC0PWR - ADC0 Power Control]
	// [ADC0PWR - ADC0 Power Control]$

	// $[ADC0GTH - ADC0 Greater-Than High Byte]
	// [ADC0GTH - ADC0 Greater-Than High Byte]$

	// $[ADC0GTL - ADC0 Greater-Than Low Byte]
	// [ADC0GTL - ADC0 Greater-Than Low Byte]$

	// $[ADC0LTH - ADC0 Less-Than High Byte]
	// [ADC0LTH - ADC0 Less-Than High Byte]$

	// $[ADC0LTL - ADC0 Less-Than Low Byte]
	// [ADC0LTL - ADC0 Less-Than Low Byte]$

	// $[ADC0CN0 - ADC0 Control 0]
	/*
	 // ADBMEN (Burst Mode Enable) = BURST_ENABLED (Enable ADC0 burst mode.)
	 */
	ADC0CN0 |= ADC0CN0_ADBMEN__BURST_ENABLED;
	// [ADC0CN0 - ADC0 Control 0]$

}

extern void CLOCK_0_enter_AccumulateMode_from_SingleSampleMode(void) {
	// $[CLKSEL - Clock Select]
	// [CLKSEL - Clock Select]$

}

extern void enter_InitialMode_from_RESET(void) {
	// $[Config Calls]
	WDT_0_enter_InitialMode_from_RESET();
	PORTS_0_enter_InitialMode_from_RESET();
	PORTS_1_enter_InitialMode_from_RESET();
	PORTS_2_enter_InitialMode_from_RESET();
	PBCFG_0_enter_InitialMode_from_RESET();
	ADC_0_enter_InitialMode_from_RESET();
	VREF_0_enter_InitialMode_from_RESET();
	LFOSC_0_enter_InitialMode_from_RESET();
	CLOCK_0_enter_InitialMode_from_RESET();
	TIMER01_0_enter_InitialMode_from_RESET();
	TIMER16_2_enter_InitialMode_from_RESET();
	TIMER16_3_enter_InitialMode_from_RESET();
	TIMER_SETUP_0_enter_InitialMode_from_RESET();
	PCA_0_enter_InitialMode_from_RESET();
	PCACH_0_enter_InitialMode_from_RESET();
	INTERRUPT_0_enter_InitialMode_from_RESET();
	// [Config Calls]$

}

extern void WDT_0_enter_InitialMode_from_RESET(void) {
	// $[WDTCN - Watchdog Timer Control]
	//Disable Watchdog with key sequence
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key
	// [WDTCN - Watchdog Timer Control]$

}

extern void PORTS_0_enter_InitialMode_from_RESET(void) {
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
	 // B4 (Port 0 Bit 4 Output Mode) = PUSH_PULL (P0.4 output is push-pull.)
	 // B5 (Port 0 Bit 5 Output Mode) = OPEN_DRAIN (P0.5 output is open-
	 //     drain.)
	 // B6 (Port 0 Bit 6 Output Mode) = OPEN_DRAIN (P0.6 output is open-
	 //     drain.)
	 // B7 (Port 0 Bit 7 Output Mode) = OPEN_DRAIN (P0.7 output is open-
	 //     drain.)
	 */
	P0MDOUT = P0MDOUT_B0__PUSH_PULL | P0MDOUT_B1__OPEN_DRAIN
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

extern void PORTS_1_enter_InitialMode_from_RESET(void) {
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

extern void PORTS_2_enter_InitialMode_from_RESET(void) {
	// $[P2 - Port 2 Pin Latch]
	// [P2 - Port 2 Pin Latch]$

	// $[P2MDOUT - Port 2 Output Mode]
	/*
	 // B0 (Port 2 Bit 0 Output Mode) = OPEN_DRAIN (P2.0 output is open-
	 //     drain.)
	 // B1 (Port 2 Bit 1 Output Mode) = PUSH_PULL (P2.1 output is push-pull.)
	 */
	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__PUSH_PULL;
	// [P2MDOUT - Port 2 Output Mode]$

}

extern void PBCFG_0_enter_InitialMode_from_RESET(void) {
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

}

extern void ADC_0_enter_InitialMode_from_RESET(void) {
	// $[ADC0CN1 - ADC0 Control 1]
	// [ADC0CN1 - ADC0 Control 1]$

	// $[ADC0MX - ADC0 Multiplexer Selection]
	/*
	 // ADC0MX (AMUX0 Positive Input Selection) = TEMP (Select ADC0.16.)
	 */
	ADC0MX = ADC0MX_ADC0MX__TEMP;
	// [ADC0MX - ADC0 Multiplexer Selection]$

	// $[ADC0CF - ADC0 Configuration]
	/*
	 // ADSC (SAR Clock Divider) = 0x01
	 // AD8BE (8-Bit Mode Enable) = NORMAL (ADC0 operates in 10-bit or 12-bit
	 //     mode (normal operation).)
	 // ADGN (Gain Control) = GAIN_1 (The on-chip PGA gain is 1.)
	 // ADTM (Track Mode) = TRACK_NORMAL (Normal Track Mode. When ADC0 is
	 //     enabled, conversion begins immediately following the start-of-
	 //     conversion signal.)
	 */
	ADC0CF = (0x01 << ADC0CF_ADSC__SHIFT) | ADC0CF_AD8BE__NORMAL
			| ADC0CF_ADGN__GAIN_1 | ADC0CF_ADTM__TRACK_NORMAL;
	// [ADC0CF - ADC0 Configuration]$

	// $[ADC0AC - ADC0 Accumulator Configuration]
	/*
	 // ADSJST (Accumulator Shift and Justify) = RIGHT_SHIFT_3 (Right
	 //     justified. Shifted right by 3 bits.)
	 // AD12BE (12-Bit Mode Enable) = 12_BIT_DISABLED (Disable 12-bit mode.)
	 // ADAE (Accumulate Enable) = ACC_DISABLED (ADC0H:ADC0L contain the
	 //     result of the latest conversion when Burst Mode is disabled.)
	 // ADRPT (Repeat Count) = ACC_32 (Perform and Accumulate 32 conversions
	 //     (8 conversions in 12-bit mode).)
	 */
	ADC0AC = ADC0AC_ADSJST__RIGHT_SHIFT_3 | ADC0AC_AD12BE__12_BIT_DISABLED
			| ADC0AC_ADAE__ACC_DISABLED | ADC0AC_ADRPT__ACC_32;
	// [ADC0AC - ADC0 Accumulator Configuration]$

	// $[ADC0TK - ADC0 Burst Mode Track Time]
	// [ADC0TK - ADC0 Burst Mode Track Time]$

	// $[ADC0PWR - ADC0 Power Control]
	// [ADC0PWR - ADC0 Power Control]$

	// $[ADC0GTH - ADC0 Greater-Than High Byte]
	// [ADC0GTH - ADC0 Greater-Than High Byte]$

	// $[ADC0GTL - ADC0 Greater-Than Low Byte]
	// [ADC0GTL - ADC0 Greater-Than Low Byte]$

	// $[ADC0LTH - ADC0 Less-Than High Byte]
	// [ADC0LTH - ADC0 Less-Than High Byte]$

	// $[ADC0LTL - ADC0 Less-Than Low Byte]
	// [ADC0LTL - ADC0 Less-Than Low Byte]$

	// $[ADC0CN0 - ADC0 Control 0]
	/*
	 // ADEN (ADC Enable) = ENABLED (Enable ADC0 (active and ready for data
	 //     conversions).)
	 // ADBMEN (Burst Mode Enable) = BURST_ENABLED (Enable ADC0 burst mode.)
	 */
	ADC0CN0 |= ADC0CN0_ADEN__ENABLED | ADC0CN0_ADBMEN__BURST_ENABLED;
	// [ADC0CN0 - ADC0 Control 0]$

}

extern void VREF_0_enter_InitialMode_from_RESET(void) {
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

extern void LFOSC_0_enter_InitialMode_from_RESET(void) {
	// $[LFO0CN - Low Frequency Oscillator Control]
	/*
	 // OSCLEN (Internal L-F Oscillator Enable) = ENABLED (Internal L-F
	 //     Oscillator Enabled.)
	 // OSCLD (Internal L-F Oscillator Divider Select) = DIVIDE_BY_1 (Divide
	 //     by 1 selected.)
	 */
	LFO0CN |= LFO0CN_OSCLEN__ENABLED | LFO0CN_OSCLD__DIVIDE_BY_1;
	// [LFO0CN - Low Frequency Oscillator Control]$

	// $[Wait for LFOSC Ready]
	while ((LFO0CN & LFO0CN_OSCLRDY__BMASK) != LFO0CN_OSCLRDY__SET)
		;
	// [Wait for LFOSC Ready]$

}

extern void CLOCK_0_enter_InitialMode_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	/*
	 // CLKSL (Clock Source Select) = HFOSC (Clock derived from the Internal
	 //     High-Frequency Oscillator.)
	 // CLKDIV (Clock Source Divider) = SYSCLK_DIV_2 (SYSCLK is equal to
	 //     selected clock source divided by 2.)
	 */
	CLKSEL = CLKSEL_CLKSL__HFOSC | CLKSEL_CLKDIV__SYSCLK_DIV_2;
	// [CLKSEL - Clock Select]$

}

extern void TIMER01_0_enter_InitialMode_from_RESET(void) {
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
	 // TH1 (Timer 1 High Byte) = 0xCB
	 */
	TH1 = (0xCB << TH1_TH1__SHIFT);
	// [TH1 - Timer 1 High Byte]$

	// $[TL1 - Timer 1 Low Byte]
	/*
	 // TL1 (Timer 1 Low Byte) = 0xF2
	 */
	TL1 = (0xF2 << TL1_TL1__SHIFT);
	// [TL1 - Timer 1 Low Byte]$

	// $[Timer Restoration]
	//Restore Timer Configuration
	TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

	// [Timer Restoration]$

}

extern void TIMER16_2_enter_InitialMode_from_RESET(void) {
	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR2CN0_TR2_save;
	TMR2CN0_TR2_save = TMR2CN0 & TMR2CN0_TR2__BMASK;
	// Stop Timer
	TMR2CN0 &= ~(TMR2CN0_TR2__BMASK);
	// [Timer Initialization]$

	// $[TMR2CN0 - Timer 2 Control]
	// [TMR2CN0 - Timer 2 Control]$

	// $[TMR2H - Timer 2 High Byte]
	// [TMR2H - Timer 2 High Byte]$

	// $[TMR2L - Timer 2 Low Byte]
	// [TMR2L - Timer 2 Low Byte]$

	// $[TMR2RLH - Timer 2 Reload High Byte]
	// [TMR2RLH - Timer 2 Reload High Byte]$

	// $[TMR2RLL - Timer 2 Reload Low Byte]
	// [TMR2RLL - Timer 2 Reload Low Byte]$

	// $[TMR2CN0]
	// [TMR2CN0]$

	// $[Timer Restoration]
	// Restore Timer Configuration
	TMR2CN0 |= TMR2CN0_TR2_save;
	// [Timer Restoration]$

}

extern void TIMER16_3_enter_InitialMode_from_RESET(void) {
	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR3CN0_TR3_save;
	TMR3CN0_TR3_save = TMR3CN0 & TMR3CN0_TR3__BMASK;
	// Stop Timer
	TMR3CN0 &= ~(TMR3CN0_TR3__BMASK);
	// [Timer Initialization]$

	// $[TMR3CN0 - Timer 3 Control]
	// [TMR3CN0 - Timer 3 Control]$

	// $[TMR3H - Timer 3 High Byte]
	// [TMR3H - Timer 3 High Byte]$

	// $[TMR3L - Timer 3 Low Byte]
	// [TMR3L - Timer 3 Low Byte]$

	// $[TMR3RLH - Timer 3 Reload High Byte]
	// [TMR3RLH - Timer 3 Reload High Byte]$

	// $[TMR3RLL - Timer 3 Reload Low Byte]
	// [TMR3RLL - Timer 3 Reload Low Byte]$

	// $[TMR3CN0]
	// [TMR3CN0]$

	// $[Timer Restoration]
	// Restore Timer Configuration
	TMR3CN0 |= TMR3CN0_TR3_save;
	// [Timer Restoration]$

}

extern void TIMER_SETUP_0_enter_InitialMode_from_RESET(void) {
	// $[CKCON0 - Clock Control 0]
	/*
	 // SCA (Timer 0/1 Prescale) = SYSCLK_DIV_12 (System clock divided by 12.)
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
	 // T1M (Timer 1 Clock Select) = SYSCLK (Timer 1 uses the system clock.)
	 */
	CKCON0 = CKCON0_SCA__SYSCLK_DIV_12 | CKCON0_T0M__PRESCALE
			| CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__EXTERNAL_CLOCK
			| CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
			| CKCON0_T1M__SYSCLK;
	// [CKCON0 - Clock Control 0]$

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

}

extern void PCA_0_enter_InitialMode_from_RESET(void) {
	// $[PCA Off]
	PCA0CN0_CR = PCA0CN0_CR__STOP;
	// [PCA Off]$

	// $[PCA0MD - PCA Mode]
	/*
	 // CIDL (PCA Counter/Timer Idle Control) = NORMAL (PCA continues to
	 //     function normally while the system controller is in Idle Mode.)
	 // ECF (PCA Counter/Timer Overflow Interrupt Enable) = OVF_INT_DISABLED
	 //     (Disable the CF interrupt.)
	 // CPS (PCA Counter/Timer Pulse Select) = LFOSC_DIV_8 (Low frequency
	 //     oscillator divided by 8.)
	 */
	PCA0MD = PCA0MD_CIDL__NORMAL | PCA0MD_ECF__OVF_INT_DISABLED
			| PCA0MD_CPS__LFOSC_DIV_8;
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

extern void PCACH_0_enter_InitialMode_from_RESET(void) {
	// $[PCA0 Settings Save]
	// Select Capture/Compare register)
	PCA0PWM &= ~PCA0PWM_ARSEL__BMASK;
	// [PCA0 Settings Save]$

	// $[PCA0CPM0 - PCA Channel 0 Capture/Compare Mode]
	/*
	 // CAPN (Channel 0 Capture Negative Function Enable) = DISABLED (Disable
	 //     negative edge capture.)
	 // ECCF (Channel 0 Capture/Compare Flag Interrupt Enable) = ENABLED
	 //     (Enable a Capture/Compare Flag interrupt request when CCF0 is set.)
	 // MAT (Channel 0 Match Function Enable) = ENABLED (Enable match
	 //     function.)
	 // PWM16 (Channel 0 16-bit Pulse Width Modulation Enable) = 8_BIT (8 to
	 //     11-bit PWM selected.)
	 // CAPP (Channel 0 Capture Positive Function Enable) = DISABLED (Disable
	 //     positive edge capture.)
	 // ECOM (Channel 0 Comparator Function Enable) = ENABLED (Enable
	 //     comparator function.)
	 // PWM (Channel 0 Pulse Width Modulation Mode Enable) = DISABLED (Disable
	 //     PWM function.)
	 // TOG (Channel 0 Toggle Function Enable) = DISABLED (Disable toggle
	 //     function.)
	 */
	PCA0CPM0 = PCA0CPM0_CAPN__DISABLED | PCA0CPM0_ECCF__ENABLED
			| PCA0CPM0_MAT__ENABLED | PCA0CPM0_PWM16__8_BIT
			| PCA0CPM0_CAPP__DISABLED | PCA0CPM0_ECOM__ENABLED
			| PCA0CPM0_PWM__DISABLED | PCA0CPM0_TOG__DISABLED;
	// [PCA0CPM0 - PCA Channel 0 Capture/Compare Mode]$

	// $[PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]
	/*
	 // PCA0CPL0 (PCA Channel 0 Capture Module Low Byte) = 0x20
	 */
	PCA0CPL0 = (0x20 << PCA0CPL0_PCA0CPL0__SHIFT);
	// [PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]$

	// $[PCA0CPH0 - PCA Channel 0 Capture Module High Byte]
	/*
	 // PCA0CPH0 (PCA Channel 0 Capture Module High Byte) = 0x4E
	 */
	PCA0CPH0 = (0x4E << PCA0CPH0_PCA0CPH0__SHIFT);
	// [PCA0CPH0 - PCA Channel 0 Capture Module High Byte]$

	// $[Auto-reload]
	// [Auto-reload]$

	// $[PCA0 Settings Restore]
	// [PCA0 Settings Restore]$

}

extern void INTERRUPT_0_enter_InitialMode_from_RESET(void) {
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

extern void enter_AccumulateMode_from_InitialMode(void) {
	// $[Config Calls]
	// [Config Calls]$

}

extern void enter_SingleSampleMode_from_AccumulateMode(void) {
	// $[Config Calls]
	ADC_0_enter_SingleSampleMode_from_AccumulateMode();
	CLOCK_0_enter_SingleSampleMode_from_AccumulateMode();
	// [Config Calls]$

}

extern void ADC_0_enter_SingleSampleMode_from_AccumulateMode(void) {
	// $[ADC0CN1 - ADC0 Control 1]
	// [ADC0CN1 - ADC0 Control 1]$

	// $[ADC0MX - ADC0 Multiplexer Selection]
	// [ADC0MX - ADC0 Multiplexer Selection]$

	// $[ADC0CF - ADC0 Configuration]
	/*
	 // ADSC (SAR Clock Divider) = 0x00
	 // AD8BE (8-Bit Mode Enable) = NORMAL (ADC0 operates in 10-bit or 12-bit
	 //     mode (normal operation).)
	 // ADGN (Gain Control) = GAIN_1 (The on-chip PGA gain is 1.)
	 // ADTM (Track Mode) = TRACK_NORMAL (Normal Track Mode. When ADC0 is
	 //     enabled, conversion begins immediately following the start-of-
	 //     conversion signal.)
	 */
	ADC0CF = (0x00 << ADC0CF_ADSC__SHIFT) | ADC0CF_AD8BE__NORMAL
			| ADC0CF_ADGN__GAIN_1 | ADC0CF_ADTM__TRACK_NORMAL;
	// [ADC0CF - ADC0 Configuration]$

	// $[ADC0AC - ADC0 Accumulator Configuration]
	/*
	 // ADSJST (Accumulator Shift and Justify) = RIGHT_NO_SHIFT (Right
	 //     justified. No shifting applied.)
	 // AD12BE (12-Bit Mode Enable) = 12_BIT_DISABLED (Disable 12-bit mode.)
	 // ADAE (Accumulate Enable) = ACC_DISABLED (ADC0H:ADC0L contain the
	 //     result of the latest conversion when Burst Mode is disabled.)
	 // ADRPT (Repeat Count) = ACC_1 (Perform and Accumulate 1 conversion (not
	 //     used in 12-bit mode).)
	 */
	ADC0AC = ADC0AC_ADSJST__RIGHT_NO_SHIFT | ADC0AC_AD12BE__12_BIT_DISABLED
			| ADC0AC_ADAE__ACC_DISABLED | ADC0AC_ADRPT__ACC_1;
	// [ADC0AC - ADC0 Accumulator Configuration]$

	// $[ADC0TK - ADC0 Burst Mode Track Time]
	// [ADC0TK - ADC0 Burst Mode Track Time]$

	// $[ADC0PWR - ADC0 Power Control]
	// [ADC0PWR - ADC0 Power Control]$

	// $[ADC0GTH - ADC0 Greater-Than High Byte]
	// [ADC0GTH - ADC0 Greater-Than High Byte]$

	// $[ADC0GTL - ADC0 Greater-Than Low Byte]
	// [ADC0GTL - ADC0 Greater-Than Low Byte]$

	// $[ADC0LTH - ADC0 Less-Than High Byte]
	// [ADC0LTH - ADC0 Less-Than High Byte]$

	// $[ADC0LTL - ADC0 Less-Than Low Byte]
	// [ADC0LTL - ADC0 Less-Than Low Byte]$

	// $[ADC0CN0 - ADC0 Control 0]
	/*
	 // ADBMEN (Burst Mode Enable) = BURST_DISABLED (Disable ADC0 burst mode.)
	 */
	ADC0CN0 &= ~ADC0CN0_ADBMEN__BMASK;
	// [ADC0CN0 - ADC0 Control 0]$

}

extern void CLOCK_0_enter_SingleSampleMode_from_AccumulateMode(void) {
	// $[CLKSEL - Clock Select]
	// [CLKSEL - Clock Select]$

}

extern void ADC_0_enter_AccumulateMode_from_InitialMode(void) {

}

extern void PCA_0_enter_SingleSampleMode_from_AccumulateMode(void) {

}

extern void PCACH_0_enter_SingleSampleMode_from_AccumulateMode(void) {

}

extern void TIMER01_0_enter_AccumulateMode_from_SingleSampleMode(void) {

}

extern void CLOCK_0_enter_AccumulateMode_from_InitialMode(void) {

}

extern void TIMER01_0_enter_AccumulateMode_from_InitialMode(void) {

}

extern void TIMER01_0_enter_SingleSampleMode_from_AccumulateMode(void) {

}

