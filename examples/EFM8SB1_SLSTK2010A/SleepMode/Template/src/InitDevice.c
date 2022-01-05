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
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	RSTSRC_0_enter_DefaultMode_from_RESET();
	RTC_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	FLASH_0_enter_DefaultMode_from_RESET();
	ADC_0_enter_DefaultMode_from_RESET();
	VREF_0_enter_DefaultMode_from_RESET();
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
	// [PCA0CN0 - PCA Control 0]$


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
	// B7 (Port 0 Bit 7 Input Mode) = ANALOG (P0.7 pin is configured for
	//     analog mode.)
	*/
	P0MDIN = P0MDIN_B0__DIGITAL | P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL
		 | P0MDIN_B3__DIGITAL | P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL
		 | P0MDIN_B6__DIGITAL | P0MDIN_B7__ANALOG;
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
	// B7 (Port 0 Bit 7 Skip) = SKIPPED (P0.7 pin is skipped by the
	//     crossbar.)
	*/
	P0SKIP = P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED | P0SKIP_B2__NOT_SKIPPED
		 | P0SKIP_B3__NOT_SKIPPED | P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
		 | P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__SKIPPED;
	// [P0SKIP - Port 0 Skip]$

	// $[P0MASK - Port 0 Mask]
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
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__PUSH_PULL | P1MDOUT_B2__OPEN_DRAIN
		 | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__OPEN_DRAIN
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
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED | P1SKIP_B2__NOT_SKIPPED
		 | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__SKIPPED | P1SKIP_B5__NOT_SKIPPED
		 | P1SKIP_B6__NOT_SKIPPED | P1SKIP_B7__NOT_SKIPPED;
	// [P1SKIP - Port 1 Skip]$

	// $[P1MASK - Port 1 Mask]
	// [P1MASK - Port 1 Mask]$

	// $[P1MAT - Port 1 Match]
	// [P1MAT - Port 1 Match]$

	// $[P1DRV - Port 1 Drive Strength]
	// [P1DRV - Port 1 Drive Strength]$


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
	// [XBR1 - Port I/O Crossbar 1]$


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
	// RTC0RE (RTC Reset Enable and Flag) = NOT_SET (A RTC alarm or
	//     oscillator fail reset did not occur.)
	*/
	RSTSRC = RSTSRC_PORSF__SET | RSTSRC_MCDRSF__SET | RSTSRC_C0RSEF__NOT_SET
		 | RSTSRC_RTC0RE__NOT_SET;
	// [RSTSRC - Reset Source]$


}

//================================================================================
// RTC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void RTC_0_enter_DefaultMode_from_RESET(void) {
	// $[RTC Initialization]
	// [RTC Initialization]$

	// $[RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]
	// [RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]$

	// $[RTC0XCN - RTC Oscillator Control]
	/*
	// XMODE (RTC Oscillator Mode) = SELF_OSCILLATE (Self-Oscillate Mode
	//     selected.)
	// AGCEN (RTC Oscillator Automatic Gain Control (AGC) Enable) = DISABLED
	//     (Disable AGC.)
	// BIASX2 (RTC Oscillator Bias Double Enable) = DISABLED (Disable the
	//     Bias Double feature.)
	// LFOEN (Low Frequency Oscillator Enable and Select) = ENABLED (LFOSC
	//     enabled and selected as RTC oscillator source.)
	*/
	RTC0ADR = RTC0XCN0;
	RTC0DAT = RTC0XCN0_XMODE__SELF_OSCILLATE | RTC0XCN0_AGCEN__DISABLED
		 | RTC0XCN0_BIASX2__DISABLED | RTC0XCN0_LFOEN__ENABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	// [RTC0XCN - RTC Oscillator Control]$

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

	// [RTC0CN - RTC Control]$

	// $[External Oscillator Setup]
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
	// CLKSL (Clock Source Select) = LPOSC (Clock derived from the Internal
	//     Low Power Oscillator.)
	*/
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__LPOSC;
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
// ADC_0_enter_DefaultMode_from_RESET
//================================================================================
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
	// ADPWR (Burst Mode Power Up Time) = 0x04
	// ADLPM (Low Power Mode Enable) = LOW_POWER_DISABLED (Disable low power
	//     mode.)
	*/
	ADC0PWR = (0x04 << ADC0PWR_ADPWR__SHIFT) | ADC0PWR_ADLPM__LOW_POWER_DISABLED;
	// [ADC0PWR - ADC0 Power Control]$

	// $[ADC0CF - ADC0 Configuration]
	/*
	// AD8BE (8-Bit Mode Enable) = NORMAL (ADC0 operates in 10-bit or 12-bit
	//     mode (normal operation).)
	// ADGN (Gain Control) = GAIN_0P5 (The on-chip PGA gain is 0.5.)
	// ADSC (SAR Clock Divider) = 0x02
	// ADTM (Track Mode) = TRACK_NORMAL (Normal Track Mode. When ADC0 is
	//     enabled, conversion begins immediately following the start-of-
	//     conversion signal.)
	*/
	ADC0CF = ADC0CF_AD8BE__NORMAL | ADC0CF_ADGN__GAIN_0P5 | (0x02 << ADC0CF_ADSC__SHIFT)
		 | ADC0CF_ADTM__TRACK_NORMAL;
	// [ADC0CF - ADC0 Configuration]$

	// $[ADC0GTH - ADC0 Greater-Than High Byte]
	// [ADC0GTH - ADC0 Greater-Than High Byte]$

	// $[ADC0GTL - ADC0 Greater-Than Low Byte]
	// [ADC0GTL - ADC0 Greater-Than Low Byte]$

	// $[ADC0LTH - ADC0 Less-Than High Byte]
	/*
	// ADC0LTH (Less-Than High Byte) = 0x03
	*/
	ADC0LTH = (0x03 << ADC0LTH_ADC0LTH__SHIFT);
	// [ADC0LTH - ADC0 Less-Than High Byte]$

	// $[ADC0LTL - ADC0 Less-Than Low Byte]
	/*
	// ADC0LTL (Less-Than Low Byte) = 0x07
	*/
	ADC0LTL = (0x07 << ADC0LTL_ADC0LTL__SHIFT);
	// [ADC0LTL - ADC0 Less-Than Low Byte]$

	// $[ADC0CN0 - ADC0 Control 0]
	/*
	// ADBMEN (Burst Mode Enable) = BURST_ENABLED (Enable ADC0 burst mode.)
	*/
	ADC0CN0 |= ADC0CN0_ADBMEN__BURST_ENABLED;
	// [ADC0CN0 - ADC0 Control 0]$


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
	// REFSL (Voltage Reference Select) = HIGH_SPEED_VREF (The ADC0 voltage
	//     reference is the internal 1.65 V high speed voltage reference.)
	*/
	REF0CN = REF0CN_TEMPE__TEMP_ENABLED | REF0CN_GNDSL__GND_PIN | REF0CN_REFSL__HIGH_SPEED_VREF;
	// [REF0CN - Voltage Reference Control]$


}

