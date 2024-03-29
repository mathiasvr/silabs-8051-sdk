//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_C8051F930_Register_Enums.h>
#include "InitDevice.h"

// USER PROTOTYPES
// USER FUNCTIONS


//==============================================================================
// enter_DefaultMode_from_RESET
//==============================================================================
extern void enter_DefaultMode_from_RESET(void) {
	// $[Config Calls]
	PCA_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	FLASH_0_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	ADC_0_enter_DefaultMode_from_RESET();
	VREF_0_enter_DefaultMode_from_RESET();
	RSTSRC_0_enter_DefaultMode_from_RESET();
	// [Config Calls]$


}


//================================================================================
// PCA_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PCA_0_enter_DefaultMode_from_RESET(void) {
	// $[Watchdog Disable]
	// Stop Watchdog
	// If MCU is reset by WDT before it runs to main(), please disable it in SILABS_STARTUP.A51
	SFRPAGE = 0x00;
	PCA0MD &= ~PCA0MD_WDTE__BMASK;

	// [Watchdog Disable]$

	// $[PCA0CPM0 - PCA Channel 0 Capture/Compare Mode 0]
	// [PCA0CPM0 - PCA Channel 0 Capture/Compare Mode 0]$

	// $[PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]
	// [PCA0CPL0 - PCA Channel 0 Capture Module Low Byte]$

	// $[PCA0CPH0 - PCA Channel 0 Capture Module High Byte]
	// [PCA0CPH0 - PCA Channel 0 Capture Module High Byte]$

	// $[PCA0CPM1 - PCA Channel 1 Capture/Compare Mode]
	// [PCA0CPM1 - PCA Channel 1 Capture/Compare Mode]$

	// $[PCA0CPL1 - PCA Channel 1 Capture Module Low Byte]
	// [PCA0CPL1 - PCA Channel 1 Capture Module Low Byte]$

	// $[PCA0CPH1 - PCA Channel 1 Capture Module High Byte]
	// [PCA0CPH1 - PCA Channel 1 Capture Module High Byte]$

	// $[PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]
	// [PCA0CPM2 - PCA Channel 2 Capture/Compare Mode]$

	// $[PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]
	// [PCA0CPL2 - PCA Channel 2 Capture Module Low Byte]$

	// $[PCA0CPH2 - PCA Channel 2 Capture Module High Byte]
	// [PCA0CPH2 - PCA Channel 2 Capture Module High Byte]$

	// $[PCA0CPM3 - PCA Channel 3 Capture/Compare Mode]
	// [PCA0CPM3 - PCA Channel 3 Capture/Compare Mode]$

	// $[PCA0CPL3 - PCA Channel 3 Capture Module Low Byte]
	// [PCA0CPL3 - PCA Channel 3 Capture Module Low Byte]$

	// $[PCA0CPH3 - PCA Channel 3 Capture Module High Byte]
	// [PCA0CPH3 - PCA Channel 3 Capture Module High Byte]$

	// $[PCA0CPM4 - PCA Channel 4 Capture/Compare Mode]
	// [PCA0CPM4 - PCA Channel 4 Capture/Compare Mode]$

	// $[PCA0CPL4 - PCA Channel 4 Capture Module Low Byte]
	// [PCA0CPL4 - PCA Channel 4 Capture Module Low Byte]$

	// $[PCA0CPH4 - PCA Channel 4 Capture Module High Byte]
	// [PCA0CPH4 - PCA Channel 4 Capture Module High Byte]$

	// $[PCA0CPM5 - PCA Channel 5 Capture/Compare Mode]
	// [PCA0CPM5 - PCA Channel 5 Capture/Compare Mode]$

	// $[PCA0CPL5 - PCA Channel 5 Capture Module Low Byte]
	// [PCA0CPL5 - PCA Channel 5 Capture Module Low Byte]$

	// $[PCA0CPH5 - PCA Channel 5 Capture Module High Byte]
	// [PCA0CPH5 - PCA Channel 5 Capture Module High Byte]$

	// $[PCA0CN - PCA Control]
	// [PCA0CN - PCA Control]$

	// $[PCA0L - PCA Counter/Timer Low Byte]
	// [PCA0L - PCA Counter/Timer Low Byte]$

	// $[PCA0H - PCA Counter/Timer High Byte]
	// [PCA0H - PCA Counter/Timer High Byte]$

	// $[PCA0PWM - PCA PWM Configuration]
	// [PCA0PWM - PCA PWM Configuration]$

	// $[PCA0MD - PCA Mode]
	// [PCA0MD - PCA Mode]$

	// $[WDT - Watchdog run Control]
	// [WDT - Watchdog run Control]$


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
	// B6 (Port 0 Bit 6 Input Mode) = ANALOG (P0.6 pin is configured for
	//     analog mode.)
	// B7 (Port 0 Bit 7 Input Mode) = DIGITAL (P0.7 pin is configured for
	//     digital mode.)
	*/
	P0MDIN = P0MDIN_B0__DIGITAL | P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL
		 | P0MDIN_B3__DIGITAL | P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL
		 | P0MDIN_B6__ANALOG | P0MDIN_B7__DIGITAL;
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
	// B6 (Port 0 Bit 6 Skip) = SKIPPED (P0.6 pin is skipped by the
	//     crossbar.)
	// B7 (Port 0 Bit 7 Skip) = NOT_SKIPPED (P0.7 pin is not skipped by the
	//     crossbar.)
	*/
	P0SKIP = P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED | P0SKIP_B2__NOT_SKIPPED
		 | P0SKIP_B3__NOT_SKIPPED | P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
		 | P0SKIP_B6__SKIPPED | P0SKIP_B7__NOT_SKIPPED;
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
	// B1 (Port 1 Bit 1 Output Mode) = OPEN_DRAIN (P1.1 output is open-
	//     drain.)
	// B2 (Port 1 Bit 2 Output Mode) = OPEN_DRAIN (P1.2 output is open-
	//     drain.)
	// B3 (Port 1 Bit 3 Output Mode) = OPEN_DRAIN (P1.3 output is open-
	//     drain.)
	// B4 (Port 1 Bit 4 Output Mode) = OPEN_DRAIN (P1.4 output is open-
	//     drain.)
	// B5 (Port 1 Bit 5 Output Mode) = PUSH_PULL (P1.5 output is push-pull.)
	// B6 (Port 1 Bit 6 Output Mode) = PUSH_PULL (P1.6 output is push-pull.)
	// B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
	//     drain.)
	*/
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN | P1MDOUT_B2__OPEN_DRAIN
		 | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__PUSH_PULL
		 | P1MDOUT_B6__PUSH_PULL | P1MDOUT_B7__OPEN_DRAIN;
	// [P1MDOUT - Port 1 Output Mode]$

	// $[P1MDIN - Port 1 Input Mode]
	// [P1MDIN - Port 1 Input Mode]$

	// $[P1SKIP - Port 1 Skip]
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
// ADC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void ADC_0_enter_DefaultMode_from_RESET(void) {
	// $[ADC0CN - ADC0 Control]
	/*
	// ADEN (ADC Enable) = DISABLED (Disable ADC0 (low-power shutdown).)
	// ADBMEN (Burst Mode Enable) = BURST_ENABLED (Enable ADC0 burst mode.)
	// ADCM (Start of Conversion Mode Select) = ADBUSY (ADC0 conversion
	//     initiated on write of 1 to ADBUSY.)
	*/
	ADC0CN = ADC0CN_ADEN__DISABLED | ADC0CN_ADBMEN__BURST_ENABLED
		 | ADC0CN_ADCM__ADBUSY;
	// [ADC0CN - ADC0 Control]$

	// $[ADC0MX - ADC0 Multiplexer Selection]
	/*
	// ADC0MX (AMUX0 Positive Input Selection) = ADC0P6 (Select channel
	//     ADC0.6.)
	*/
	ADC0MX = ADC0MX_ADC0MX__ADC0P6;
	// [ADC0MX - ADC0 Multiplexer Selection]$

	// $[ADC0CF - ADC0 Configuration]
	/*
	// AD8BE (8-Bit Mode Enable) = NORMAL (ADC0 operates in 10-bit mode
	//     (normal operation).)
	// ADGN (Gain Control) = GAIN_0P5 (The on-chip PGA gain is 0.5.)
	// ADSC (SAR Clock Divider) = 2
	// ADTM (Track Mode) = TRACK_NORMAL (Normal Track Mode. When ADC0 is
	//     enabled, conversion begins immediately following the start-of-
	//     conversion signal.)
	*/
	ADC0CF = ADC0CF_AD8BE__NORMAL | ADC0CF_ADGN__GAIN_0P5 | (2 << ADC0CF_ADSC__SHIFT)
		 | ADC0CF_ADTM__TRACK_NORMAL;
	// [ADC0CF - ADC0 Configuration]$

	// $[ADC0TK - ADC0 Burst Mode Track Time]
	// [ADC0TK - ADC0 Burst Mode Track Time]$

	// $[ADC0PWR - ADC0 Power Control]
	// [ADC0PWR - ADC0 Power Control]$

	// $[ADC0AC - ADC0 Accumulator Configuration]
	// [ADC0AC - ADC0 Accumulator Configuration]$

	// $[ADC0GTH - ADC0 Greater-Than High Byte]
	// [ADC0GTH - ADC0 Greater-Than High Byte]$

	// $[ADC0GTL - ADC0 Greater-Than Low Byte]
	// [ADC0GTL - ADC0 Greater-Than Low Byte]$

	// $[ADC0LTH - ADC0 Less-Than High Byte]
	/*
	// ADC0LTH (Less-Than High Byte) = 3
	*/
	ADC0LTH = (3 << ADC0LTH_ADC0LTH__SHIFT);
	// [ADC0LTH - ADC0 Less-Than High Byte]$

	// $[ADC0LTL - ADC0 Less-Than Low Byte]
	/*
	// ADC0LTL (Less-Than Low Byte) = 7
	*/
	ADC0LTL = (7 << ADC0LTL_ADC0LTL__SHIFT);
	// [ADC0LTL - ADC0 Less-Than Low Byte]$


}

//================================================================================
// VREF_0_enter_DefaultMode_from_RESET
//================================================================================
extern void VREF_0_enter_DefaultMode_from_RESET(void) {
	// $[REF0CN - Voltage Reference Control]
	/*
	// REFGND (Analog Ground Reference) = GND_PIN (The ADC0 ground reference
	//     is the GND pin.)
	// REFOE (Internal Voltage Reference Output Enable) = DISABLED (Internal
	//     1.68 V Precision Voltage Reference disabled and not connected to
	//     P0.0/VREF.)
	// TEMPE (Temperature Sensor Enable) = TEMP_ENABLED (Enable the
	//     Temperature Sensor.)
	// REFSL (Voltage Reference Select) = HIGH_SPEED_VREF (The ADC0 voltage
	//     reference is the internal 1.65 V high speed voltage reference.)
	*/
	REF0CN = REF0CN_REFGND__GND_PIN | REF0CN_REFOE__DISABLED | REF0CN_TEMPE__TEMP_ENABLED
		 | REF0CN_REFSL__HIGH_SPEED_VREF;
	// [REF0CN - Voltage Reference Control]$


}

//================================================================================
// RSTSRC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void RSTSRC_0_enter_DefaultMode_from_RESET(void) {
	// $[RSTSRC - Reset Source]
	/*
	// C0RSEF (Comparator0 Reset Enable and Flag) = NOT_SET (A Comparator 0
	//     reset did not occur.)
	// MCDRSF (Missing Clock Detector Enable and Flag) = SET (A missing clock
	//     detector reset occurred.)
	// PORSF (Power-On / Supply Monitor Reset Flag, and Supply Monitor Reset
	//     Enable) = SET (A power-on or supply monitor reset occurred.)
	// RTC0RE (RTC Reset Enable and Flag) = NOT_SET (A RTC alarm or
	//     oscillator fail reset did not occur.)
	// SWRSF (Software Reset Force and Flag) = NOT_SET (A software reset did
	//     not occur.)
	*/
	RSTSRC = RSTSRC_C0RSEF__NOT_SET | RSTSRC_MCDRSF__SET | RSTSRC_PORSF__SET
		 | RSTSRC_RTC0RE__NOT_SET | RSTSRC_SWRSF__NOT_SET;
	// [RSTSRC - Reset Source]$


}

//================================================================================
// RTC_0_enter_SelfOscMode_from_DefaultMode
//================================================================================
extern void RTC_0_enter_SelfOscMode_from_DefaultMode(void) {
	// $[RTC Initialization]
	// Unlock the RTC Interface
	RTC0KEY = RTC0KEY_RTC0ST__KEY1;
	RTC0KEY = RTC0KEY_RTC0ST__KEY2;

	// [RTC Initialization]$

	// $[RTC0XCN - RTC Oscillator Control: Initial start-up setting]
	// [RTC0XCN - RTC Oscillator Control: Initial start-up setting]$

	// $[RTC0XCN - RTC Oscillator Control]
	// [RTC0XCN - RTC Oscillator Control]$

	// $[RTC0XCF - RTC Oscillator Configuration]
	// [RTC0XCF - RTC Oscillator Configuration]$

	// $[RTC0PIN - RTC Pin Configuration]
	/*
	// RTCPIN (RTC Pin Configuration) = SELF_OSCILLATE (XTAL3 and XTAL4 are
	//     internally shorted for use with self-oscillate mode.)
	*/
	SFRPAGE = 0x00;
	RTC0ADR = RTC0PIN;
	RTC0DAT = RTC0PIN_RTCPIN__SELF_OSCILLATE;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	// [RTC0PIN - RTC Pin Configuration]$

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
	// [ALARM1 - RTC Alarm Programmed Value 1]$

	// $[ALARM2 - RTC Alarm Programmed Value 2]
	// [ALARM2 - RTC Alarm Programmed Value 2]$

	// $[ALARM3 - RTC Alarm Programmed Value 3]
	// [ALARM3 - RTC Alarm Programmed Value 3]$

	// $[RTC0CN - RTC Control]
	/*
	// RTC0EN (RTC Enable) = ENABLED (Enable RTC oscillator.)
	// RTC0TR (RTC Timer Run Control) = STOP (RTC timer is stopped.)
	// MCLKEN (Missing RTC Detector Enable) = ENABLED (Enable missing RTC
	//     detector.)
	// RTC0AEN (RTC Alarm Enable) = DISABLED (Disable RTC alarm.)
	// ALRM (RTC Alarm Event Flag and Auto Reset Enable) = NOT_SET (Alarm
	//     event flag is not set or disable the auto reset function.)
	// RTC0CAP (RTC Timer Capture) = NOT_SET (Do not start a capture
	//     operation.)
	// RTC0SET (RTC Timer Set) = NOT_SET (Do not start a set operation.)
	*/
	RTC0ADR = RTC0CN;
	RTC0DAT = RTC0CN_RTC0EN__ENABLED | RTC0CN_RTC0TR__STOP | RTC0CN_MCLKEN__ENABLED
		 | RTC0CN_RTC0AEN__DISABLED | RTC0CN_ALRM__NOT_SET | RTC0CN_RTC0CAP__NOT_SET
		 | RTC0CN_RTC0SET__NOT_SET;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// [RTC0CN - RTC Control]$

	// $[External Oscillator Setup]
	// [External Oscillator Setup]$


}

//================================================================================
// RTC_0_enter_CrystalMode_from_DefaultMode
//================================================================================
extern void RTC_0_enter_CrystalMode_from_DefaultMode(void) {
	// $[RTC Initialization]
	// A variable for providing a delay for external oscillator startup
	uint16_t delayCounter;
	// Save the system clock (the system clock will be slowed during the startup delay)
	uint8_t CLKSEL_SAVE = CLKSEL;

	// Unlock the RTC Interface
	RTC0KEY = RTC0KEY_RTC0ST__KEY1;
	RTC0KEY = RTC0KEY_RTC0ST__KEY2;

	// Enable power to the SmaRTClock oscillator circuit (RTC0EN = 1)
	// [RTC Initialization]$

	// $[RTC0XCN - RTC Oscillator Control: Initial start-up setting]
	// Set SmaRTClock to Crystal Mode (XMODE = 1).
	// Disable Automatic Gain Control (AGCEN) and enable Bias Doubling (BIASX2) for fast crystal startup.
	RTC0ADR = RTC0XCN;
	RTC0DAT = RTC0XCN_XMODE__CRYSTAL | RTC0XCN_BIASX2__ENABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	// [RTC0XCN - RTC Oscillator Control: Initial start-up setting]$

	// $[RTC0XCF - RTC Oscillator Configuration]
	/*
	// AUTOSTP (Automatic Load Capacitance Stepping Enable) = ENABLED (Enable
	//     load capacitance stepping.)
	// LOADCAP (Load Capacitance Programmed Value) = 3
	*/
	SFRPAGE = 0x00;
	RTC0ADR = RTC0XCF;
	RTC0DAT = RTC0XCF_AUTOSTP__ENABLED | (3 << RTC0XCF_LOADCAP__SHIFT);
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	// [RTC0XCF - RTC Oscillator Configuration]$

	// $[RTC0PIN - RTC Pin Configuration]
	// [RTC0PIN - RTC Pin Configuration]$

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
	// [ALARM1 - RTC Alarm Programmed Value 1]$

	// $[ALARM2 - RTC Alarm Programmed Value 2]
	// [ALARM2 - RTC Alarm Programmed Value 2]$

	// $[ALARM3 - RTC Alarm Programmed Value 3]
	// [ALARM3 - RTC Alarm Programmed Value 3]$

	// $[RTC0CN - RTC Control]
	/*
	// RTC0EN (RTC Enable) = ENABLED (Enable RTC oscillator.)
	*/
	RTC0ADR = RTC0CN;
	RTC0DAT = 0;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	RTC0ADR = RTC0CN;
	RTC0DAT |= RTC0CN_RTC0EN__ENABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// [RTC0CN - RTC Control]$

	// $[External Oscillator Setup]
	// Set the clock to a known value for the delay
	CLKSEL = CLKSEL_CLKSL__LPOSC | CLKSEL_CLKDIV__SYSCLK_DIV_128;

	// Delay for > 20 ms
	for (delayCounter=0x150;delayCounter!=0;delayCounter--);

	// Poll the SmaRTClock Clock Valid Bit (CLKVLD) until the crystal oscillator stabilizes
	do {
	RTC0ADR = RTC0ADR_BUSY__SET | RTC0ADR_ADDR__RTC0XCN;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	} while ((RTC0DAT & RTC0XCN_CLKVLD__BMASK)== RTC0XCN_CLKVLD__NOT_SET);

	// Poll the SmaRTClock Load Capacitance Ready Bit (LOADRDY) until the load capacitance reaches its programmed value
	do {
	RTC0ADR = RTC0ADR_BUSY__SET | RTC0ADR_ADDR__RTC0XCF;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	} while ((RTC0DAT & RTC0XCF_LOADRDY__BMASK)== RTC0XCF_LOADRDY__NOT_SET);

	// Enable Automatic Gain Control (AGCEN) and disable Bias Doubling (BIASX2) for maximum power savings
	/*
	// XMODE (RTC Oscillator Mode) = CRYSTAL (Crystal Mode selected.)
	// AGCEN (RTC Oscillator Automatic Gain Control (AGC) Enable) = ENABLED
	//     (Enable AGC.)
	// BIASX2 (RTC Oscillator Bias Double Enable) = DISABLED (Disable the
	//     Bias Double feature.)
	*/
	RTC0ADR = RTC0XCN;
	RTC0DAT = RTC0XCN_XMODE__CRYSTAL | RTC0XCN_AGCEN__ENABLED | RTC0XCN_BIASX2__DISABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// Enable the SmaRTClock missing clock detector.
	/*
	// MCLKEN (Missing RTC Detector Enable) = ENABLED (Enable missing RTC
	//     detector.)
	*/
	RTC0ADR = RTC0CN;
	RTC0DAT |= RTC0CN_MCLKEN__ENABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// Delay for > 2 ms
	for (delayCounter=0x100;delayCounter!=0;delayCounter--);

	// Clear PMU wake-up source flags
	PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;

	// Restore system clock
	CLKSEL = CLKSEL_SAVE;
	// Poll CLKRDY to wait for the clock to stabilize
	while(!((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__SET));

	/*
	// RTC0EN (RTC Enable) = ENABLED (Enable RTC oscillator.)
	// RTC0TR (RTC Timer Run Control) = STOP (RTC timer is stopped.)
	// MCLKEN (Missing RTC Detector Enable) = ENABLED (Enable missing RTC
	//     detector.)
	// RTC0AEN (RTC Alarm Enable) = DISABLED (Disable RTC alarm.)
	// ALRM (RTC Alarm Event Flag and Auto Reset Enable) = NOT_SET (Alarm
	//     event flag is not set or disable the auto reset function.)
	// RTC0CAP (RTC Timer Capture) = NOT_SET (Do not start a capture
	//     operation.)
	// RTC0SET (RTC Timer Set) = NOT_SET (Do not start a set operation.)
	*/
	RTC0ADR = RTC0CN;
	RTC0DAT = RTC0CN_RTC0EN__ENABLED | RTC0CN_RTC0TR__STOP | RTC0CN_MCLKEN__ENABLED
		 | RTC0CN_RTC0AEN__DISABLED | RTC0CN_ALRM__NOT_SET | RTC0CN_RTC0CAP__NOT_SET
		 | RTC0CN_RTC0SET__NOT_SET;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// [External Oscillator Setup]$


}


//==============================================================================
// enter_SelfOscMode_from_DefaultMode
//==============================================================================
extern void enter_SelfOscMode_from_DefaultMode(void) {
	// $[Config Calls]
	RTC_0_enter_SelfOscMode_from_DefaultMode();
	// [Config Calls]$


}


//==============================================================================
// enter_CrystalMode_from_DefaultMode
//==============================================================================
extern void enter_CrystalMode_from_DefaultMode(void) {
	// $[Config Calls]
	RTC_0_enter_CrystalMode_from_DefaultMode();
	// [Config Calls]$


}



