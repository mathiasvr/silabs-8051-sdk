//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8SB1_Register_Enums.h>
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
	PCACH_2_enter_DefaultMode_from_RESET();
	VREG_0_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PORTS_2_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	HFOSC_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	TIMER16_2_enter_DefaultMode_from_RESET();
	ADC_0_enter_DefaultMode_from_RESET();
	INTERRUPT_0_enter_DefaultMode_from_RESET();
	// Restore the SFRPAGE
	SFRPAGE = SFRPAGE_save;
	// [Config Calls]$

}

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
	/*
	 // B0 (Port 0 Bit 0 Skip) = SKIPPED (P0.0 pin is skipped by the
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
	P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__NOT_SKIPPED
			| P0SKIP_B2__NOT_SKIPPED | P0SKIP_B3__NOT_SKIPPED
			| P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
			| P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__NOT_SKIPPED;
	// [P0SKIP - Port 0 Skip]$

	// $[P0MASK - Port 0 Mask]
	// [P0MASK - Port 0 Mask]$

	// $[P0MAT - Port 0 Match]
	// [P0MAT - Port 0 Match]$

	// $[P0DRV - Port 0 Drive Strength]
	// [P0DRV - Port 0 Drive Strength]$

}

extern void PORTS_1_enter_DefaultMode_from_RESET(void) {
	// $[Missing Pin Skip]
	// [Missing Pin Skip]$

	// $[P1 - Port 1 Pin Latch]
	// [P1 - Port 1 Pin Latch]$

	// $[P1MDOUT - Port 1 Output Mode]
	/*
	 // B0 (Port 1 Bit 0 Output Mode) = OPEN_DRAIN (P1.0 output is open-
	 //     drain.)
	 // B1 (Port 1 Bit 1 Output Mode) = PUSH_PULL (P1.1 output is push-pull.)
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
	 // B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
	 //     drain.)
	 */
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__PUSH_PULL
			| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN
			| P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__OPEN_DRAIN
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
	 // B4 (Port 1 Bit 4 Input Mode) = ANALOG (P1.4 pin is configured for
	 //     analog mode.)
	 // B5 (Port 1 Bit 5 Input Mode) = DIGITAL (P1.5 pin is configured for
	 //     digital mode.)
	 // B6 (Port 1 Bit 6 Input Mode) = DIGITAL (P1.6 pin is configured for
	 //     digital mode.)
	 // B7 (Port 1 Bit 7 Input Mode) = DIGITAL (P1.7 pin is configured for
	 //     digital mode.)
	 */
	P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__DIGITAL | P1MDIN_B2__DIGITAL
			| P1MDIN_B3__DIGITAL | P1MDIN_B4__ANALOG | P1MDIN_B5__DIGITAL
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
	 // B4 (Port 1 Bit 4 Skip) = SKIPPED (P1.4 pin is skipped by the
	 //     crossbar.)
	 // B5 (Port 1 Bit 5 Skip) = NOT_SKIPPED (P1.5 pin is not skipped by the
	 //     crossbar.)
	 // B6 (Port 1 Bit 6 Skip) = NOT_SKIPPED (P1.6 pin is not skipped by the
	 //     crossbar.)
	 // B7 (Port 1 Bit 7 Skip) = NOT_SKIPPED (P1.7 pin is not skipped by the
	 //     crossbar.)
	 */
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED
			| P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED
			| P1SKIP_B4__SKIPPED | P1SKIP_B5__NOT_SKIPPED
			| P1SKIP_B6__NOT_SKIPPED | P1SKIP_B7__NOT_SKIPPED;
	// [P1SKIP - Port 1 Skip]$

	// $[P1MASK - Port 1 Mask]
	// [P1MASK - Port 1 Mask]$

	// $[P1MAT - Port 1 Match]
	// [P1MAT - Port 1 Match]$

	// $[P1DRV - Port 1 Drive Strength]
	// [P1DRV - Port 1 Drive Strength]$

}

extern void PORTS_2_enter_DefaultMode_from_RESET(void) {
	// $[P2 - Port 2 Pin Latch]
	// [P2 - Port 2 Pin Latch]$

	// $[P2MDOUT - Port 2 Output Mode]
	// [P2MDOUT - Port 2 Output Mode]$

	// $[P2DRV - Port 2 Drive Strength]
	// [P2DRV - Port 2 Drive Strength]$

}

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
	// [XBR1 - Port I/O Crossbar 1]$

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

	// $[PCA0PWM - PCA PWM Configuration]
	// [PCA0PWM - PCA PWM Configuration]$

	// $[PCA0CN0 - PCA Control 0]
	// [PCA0CN0 - PCA Control 0]$

}

extern void PCACH_2_enter_DefaultMode_from_RESET(void) {
	// $[PCA0 Settings Save]
	// Select Capture/Compare register)
	PCA0PWM &= ~PCA0PWM_ARSEL__BMASK;
	// [PCA0 Settings Save]$

	// $[PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]
	/*
	 // CAPN (Channel 2 Capture Negative Function Enable) = DISABLED (Disable
	 //     negative edge capture.)
	 // ECCF (Channel 2 Capture/Compare Flag Interrupt Enable) = DISABLED
	 //     (Disable CCF2 interrupts.)
	 // MAT (Channel 2 Match Function Enable) = ENABLED (Enable match
	 //     function.)
	 // PWM16 (Channel 2 16-bit Pulse Width Modulation Enable) = 8_BIT (8 to
	 //     11-bit PWM selected.)
	 // CAPP (Channel 2 Capture Positive Function Enable) = DISABLED (Disable
	 //     positive edge capture.)
	 // ECOM (Channel 2 Comparator Function Enable) = DISABLED (Disable
	 //     comparator function.)
	 // PWM (Channel 2 Pulse Width Modulation Mode Enable) = DISABLED (Disable
	 //     PWM function.)
	 // TOG (Channel 2 Toggle Function Enable) = DISABLED (Disable toggle
	 //     function.)
	 */
	PCA0CPM2 = PCA0CPM2_CAPN__DISABLED | PCA0CPM2_ECCF__DISABLED
			| PCA0CPM2_MAT__ENABLED | PCA0CPM2_PWM16__8_BIT
			| PCA0CPM2_CAPP__DISABLED | PCA0CPM2_ECOM__DISABLED
			| PCA0CPM2_PWM__DISABLED | PCA0CPM2_TOG__DISABLED;
	// [PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]$

	// $[PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]
	// [PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]$

	// $[PCA0CPH2 - PCA Channel 2 Capture Module High Byte]
	// [PCA0CPH2 - PCA Channel 2 Capture Module High Byte]$

	// $[Auto-reload]
	// [Auto-reload]$

	// $[PCA0 Settings Restore]
	// [PCA0 Settings Restore]$

}

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
	while ((HFO0CN & HFO0CN_IFRDY__BMASK) == HFO0CN_IFRDY__NOT_SET)
		;
	// [Oscillator Ready]$

}

extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	/*
	 // CLKDIV (Clock Source Divider) = SYSCLK_DIV_8 (SYSCLK is equal to
	 //     selected clock source divided by 8.)
	 // CLKSL (Clock Source Select) = HFOSC (Clock derived from the internal
	 //     precision High-Frequency Oscillator.)
	 */
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_8 | CLKSEL_CLKSL__HFOSC;
	// Wait for the clock to be ready
	while ((CLKSEL & CLKSEL_CLKRDY__BMASK) != CLKSEL_CLKRDY__SET)
		;
	// [CLKSEL - Clock Select]$

}

extern void ADC_0_enter_DefaultMode_from_RESET(void) {
	// $[ADC0MX - ADC0 Multiplexer Selection]
	/*
	 // ADC0MX (AMUX0 Positive Input Selection) = ADC0P12 (Select channel
	 //     ADC0.12.)
	 */
	ADC0MX = ADC0MX_ADC0MX__ADC0P12;
	// [ADC0MX - ADC0 Multiplexer Selection]$

	// $[ADC0AC - ADC0 Accumulator Configuration]
	// [ADC0AC - ADC0 Accumulator Configuration]$

	// $[ADC0TK - ADC0 Burst Mode Track Time]
	// [ADC0TK - ADC0 Burst Mode Track Time]$

	// $[ADC0PWR - ADC0 Power Control]
	/*
	 // ADPWR (Burst Mode Power Up Time) = 0x0F
	 // ADLPM (Low Power Mode Enable) = LOW_POWER_ENABLED (Enable low power
	 //     mode (requires extended tracking time).)
	 */
	ADC0PWR = (0x0F << ADC0PWR_ADPWR__SHIFT) | ADC0PWR_ADLPM__LOW_POWER_ENABLED;
	// [ADC0PWR - ADC0 Power Control]$

	// $[ADC0CF - ADC0 Configuration]
	/*
	 // AD8BE (8-Bit Mode Enable) = NORMAL (ADC0 operates in 10-bit or 12-bit
	 //     mode (normal operation).)
	 // ADGN (Gain Control) = GAIN_0P5 (The on-chip PGA gain is 0.5.)
	 // ADSC (SAR Clock Divider) = 0x00
	 // ADTM (Track Mode) = TRACK_NORMAL (Normal Track Mode. When ADC0 is
	 //     enabled, conversion begins immediately following the start-of-
	 //     conversion signal.)
	 */
	ADC0CF = ADC0CF_AD8BE__NORMAL | ADC0CF_ADGN__GAIN_0P5
			| (0x00 << ADC0CF_ADSC__SHIFT) | ADC0CF_ADTM__TRACK_NORMAL;
	// [ADC0CF - ADC0 Configuration]$

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

extern void VREG_0_enter_DefaultMode_from_RESET(void) {
	// $[REG0CN - Voltage Regulator Control]
	/*
	 // OSCBIAS (High Frequency Oscillator Bias) = ENABLED (Enable the
	 //     precision High Frequency Oscillator bias.)
	 */
	REG0CN = REG0CN_OSCBIAS__ENABLED;
	// [REG0CN - Voltage Regulator Control]$

}

extern void TIMER16_2_enter_DefaultMode_from_RESET(void) {
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
	 // EPCA0 (Programmable Counter Array (PCA0) Interrupt Enable) = DISABLED
	 //     (Disable all PCA0 interrupts.)
	 // ERTC0A (RTC Alarm Interrupt Enable) = DISABLED (Disable RTC Alarm
	 //     interrupts.)
	 // ESMB0 (SMBus (SMB0) Interrupt Enable) = DISABLED (Disable all SMB0
	 //     interrupts.)
	 // ET3 (Timer 3 Interrupt Enable) = DISABLED (Disable Timer 3
	 //     interrupts.)
	 */
	EIE1 = EIE1_EADC0__DISABLED | EIE1_EWADC0__ENABLED | EIE1_ECP0__DISABLED
			| EIE1_EPCA0__DISABLED | EIE1_ERTC0A__DISABLED
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

	// $[EIE2 - Extended Interrupt Enable 2]
	// [EIE2 - Extended Interrupt Enable 2]$

	// $[EIP2 - Extended Interrupt Priority 2]
	// [EIP2 - Extended Interrupt Priority 2]$

}

