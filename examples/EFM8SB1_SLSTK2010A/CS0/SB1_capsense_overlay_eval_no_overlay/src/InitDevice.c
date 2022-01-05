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
#include "cslib.h"

// [Library Includes]$

// =============================================================================
// SiLabs_Startup
// 
// If SILABS_STARTUP=1 is defined as a Control Symbol in the Assembler
// settings, this function will be called from SILABS_STARTUP.A51 immediately 
// after a reset, before any variable initialization. This is primarily used to
// disable the Watchdog Timer, in the case where it may trigger a reset when
// the device is initializing a large amount of variables.
// =============================================================================
extern void SiLabs_Startup(void) {
	// $[SiLabs Startup]
	/*
	// WDTE (Watchdog Timer Enable) = DISABLED (Disable Watchdog Timer.)
	*/
	PCA0MD &= ~PCA0MD_WDTE__BMASK;

	// [SiLabs Startup]$


}


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
	PBCFG_0_enter_DefaultMode_from_RESET();
	HFOSC_0_enter_DefaultMode_from_RESET();
	RTC_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	FLASH_0_enter_DefaultMode_from_RESET();
	TIMER01_0_enter_DefaultMode_from_RESET();
	TIMER_SETUP_0_enter_DefaultMode_from_RESET();
	CSLIB_0_enter_DefaultMode_from_RESET();
	// Restore the SFRPAGE
	SFRPAGE = SFRPAGE_save;
	// [Config Calls]$


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
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN | P0MDOUT_B2__OPEN_DRAIN
		 | P0MDOUT_B3__OPEN_DRAIN | P0MDOUT_B4__PUSH_PULL | P0MDOUT_B5__OPEN_DRAIN
		 | P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__OPEN_DRAIN;
	// [P0MDOUT - Port 0 Output Mode]$
	P0MDOUT = P0MDOUT_B4__PUSH_PULL;
	// $[P0MDIN - Port 0 Input Mode]
	/*
	// B0 (Port 0 Bit 0 Input Mode) = DIGITAL (P0.0 pin is configured for
	//     digital mode.)
	// B1 (Port 0 Bit 1 Input Mode) = ANALOG (P0.1 pin is configured for
	//     analog mode.)
	// B2 (Port 0 Bit 2 Input Mode) = ANALOG (P0.2 pin is configured for
	//     analog mode.)
	// B3 (Port 0 Bit 3 Input Mode) = ANALOG (P0.3 pin is configured for
	//     analog mode.)
	// B4 (Port 0 Bit 4 Input Mode) = DIGITAL (P0.4 pin is configured for
	//     digital mode.)
	// B5 (Port 0 Bit 5 Input Mode) = DIGITAL (P0.5 pin is configured for
	//     digital mode.)
	// B6 (Port 0 Bit 6 Input Mode) = DIGITAL (P0.6 pin is configured for
	//     digital mode.)
	// B7 (Port 0 Bit 7 Input Mode) = DIGITAL (P0.7 pin is configured for
	//     digital mode.)
	*/
	P0MDIN = P0MDIN_B0__DIGITAL | P0MDIN_B1__ANALOG | P0MDIN_B2__ANALOG
		 | P0MDIN_B3__ANALOG | P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL
		 | P0MDIN_B6__DIGITAL | P0MDIN_B7__DIGITAL;
	// [P0MDIN - Port 0 Input Mode]$

	// $[P0SKIP - Port 0 Skip]
	/*
	// B0 (Port 0 Bit 0 Skip) = NOT_SKIPPED (P0.0 pin is not skipped by the
	//     crossbar.)
	// B1 (Port 0 Bit 1 Skip) = SKIPPED (P0.1 pin is skipped by the
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
	P0SKIP = P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED
		 | P0SKIP_B3__SKIPPED | P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
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
	// $[P1 - Port 1 Pin Latch]
	// [P1 - Port 1 Pin Latch]$

	// $[P1MDOUT - Port 1 Output Mode]
	// [P1MDOUT - Port 1 Output Mode]$
	P1MDOUT = P0MDOUT_B1__PUSH_PULL;
	// $[P1MDIN - Port 0 Input Mode]
	// [P1MDIN - Port 0 Input Mode]$

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
	// B5 (Port 1 Bit 5 Skip) = SKIPPED (P1.5 pin is skipped by the
	//     crossbar.)
	// B6 (Port 1 Bit 6 Skip) = SKIPPED (P1.6 pin is skipped by the
	//     crossbar.)
	// B7 (Port 1 Bit 7 Skip) = SKIPPED (P1.7 pin is skipped by the
	//     crossbar.)
	*/
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED | P1SKIP_B2__NOT_SKIPPED
		 | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__NOT_SKIPPED | P1SKIP_B5__SKIPPED
		 | P1SKIP_B6__SKIPPED | P1SKIP_B7__SKIPPED;
	// [P1SKIP - Port 1 Skip]$

	// $[P1MASK - Port 1 Mask]
	// [P1MASK - Port 1 Mask]$

	// $[P0MAT - Port 1 Match]
	// [P0MAT - Port 1 Match]$

	// $[P1DRV - Port 1 Drive Strength]
	// [P1DRV - Port 1 Drive Strength]$


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
	// B5 (Port 1 Bit 5 Input Mode) = ANALOG (P1.5 pin is configured for
	//     analog mode.)
	// B6 (Port 1 Bit 6 Input Mode) = ANALOG (P1.6 pin is configured for
	//     analog mode.)
	// B7 (Port 1 Bit 7 Input Mode) = ANALOG (P1.7 pin is configured for
	//     analog mode.)
	*/
	P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__DIGITAL | P1MDIN_B2__DIGITAL
		 | P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__ANALOG
		 | P1MDIN_B6__ANALOG | P1MDIN_B7__ANALOG;
	// [P1MDIN - Port 1 Input Mode]$

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
	*/
	XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;
	// [XBR2 - Port I/O Crossbar 2]$
	XBR2 = XBR2_XBARE__ENABLED;
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
	// SYSCKE (SYSCLK Output Enable) = DISABLED (SYSCLK unavailable at Port
	//     pin.)
	*/
	XBR0 = XBR0_URT0E__ENABLED | XBR0_SPI0E__DISABLED | XBR0_SMB0E__DISABLED
		 | XBR0_CP0E__DISABLED | XBR0_CP0AE__DISABLED | XBR0_SYSCKE__DISABLED;
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR1 - Port I/O Crossbar 1]
	// [XBR1 - Port I/O Crossbar 1]$


}

extern void RTC_0_enter_DefaultMode_from_RESET(void) {
	// $[RTC Initialization]
	// [RTC Initialization]$

	// $[RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]
	// [RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]$

	// $[RTC0XCN - RTC Oscillator Control]
	// [RTC0XCN - RTC Oscillator Control]$

	// $[RTC0XCF - RTC Oscillator Configuration]
	/*
	// AUTOSTP (Automatic Load Capacitance Stepping Enable) = DISABLED
	//     (Disable load capacitance stepping.)
	// LOADCAP (Load Capacitance Programmed Value) = 0x07
	*/
	RTC0ADR = RTC0XCF;
	RTC0DAT = RTC0XCF_AUTOSTP__DISABLED | (0x07 << RTC0XCF_LOADCAP__SHIFT);
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
	/*
	// ALARM0 (RTC Alarm Programmed Value 0) = 0x14
	*/
	RTC0ADR = ALARM0;
	RTC0DAT = (0x14 << ALARM0_ALARM0__SHIFT);
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
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
	// MCLKEN (Missing RTC Detector Enable) = DISABLED (Disable missing RTC
	//     detector.)
	// RTC0AEN (RTC Alarm Enable) = DISABLED (Disable RTC alarm.)
	// ALRM (RTC Alarm Event Flag and Auto Reset Enable) = NOT_SET (Alarm
	//     event flag is not set or disable the auto reset function.)
	// RTC0CAP (RTC Timer Capture) = NOT_SET (Do not start a capture
	//     operation.)
	// RTC0SET (RTC Timer Set) = NOT_SET (Do not start a set operation.)
	*/
	RTC0ADR = RTC0CN0;
	RTC0DAT = RTC0CN0_RTC0EN__ENABLED | RTC0CN0_RTC0TR__STOP | RTC0CN0_MCLKEN__DISABLED
		 | RTC0CN0_RTC0AEN__DISABLED | RTC0CN0_ALRM__NOT_SET | RTC0CN0_RTC0CAP__NOT_SET
		 | RTC0CN0_RTC0SET__NOT_SET;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// [RTC0CN - RTC Control]$

	// $[External Oscillator Setup]
	// [External Oscillator Setup]$


}

extern void CSLIB_0_enter_DefaultMode_from_RESET(void) {
	// $[CSLIB Init]

	// -----------------------------------------------------------------------------
	// Configures all peripherals controlled by capsense, including the sensing block
	// and port pins
	// -----------------------------------------------------------------------------
	CSLIB_initHardware();

	// -----------------------------------------------------------------------------
	// Initializes capsense variables and performs some scans to initialize the
	// baselines
	// -----------------------------------------------------------------------------
	CSLIB_initLibrary();

	SCON0_TI = 1;     // Indicate UART ready for transmission
	// [CSLIB Init]$


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
	while((HFO0CN & HFO0CN_IFRDY__BMASK) == HFO0CN_IFRDY__NOT_SET);
	// [Oscillator Ready]$


}

extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	/*
	// CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
	//     selected clock source divided by 1.)
	// CLKSL (Clock Source Select) = HFOSC (Clock derived from the internal
	//     precision High-Frequency Oscillator.)
	*/
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__HFOSC;
	// Wait for the clock to be ready
	while((CLKSEL & CLKSEL_CLKRDY__BMASK) != CLKSEL_CLKRDY__SET);
	// [CLKSEL - Clock Select]$


}

extern void TIMER01_0_enter_DefaultMode_from_RESET(void) {
	// $[Timer Initialization]
	//Save Timer Configuration
	uint8_t TCON_save;
	TCON_save = TCON;
	//Stop Timers
	TCON &= TCON_TR0__BMASK & TCON_TR1__BMASK;

	// [Timer Initialization]$

	// $[TH0 - Timer 0 High Byte]
	// [TH0 - Timer 0 High Byte]$

	// $[TL0 - Timer 0 Low Byte]
	// [TL0 - Timer 0 Low Byte]$

	// $[TH1 - Timer 1 High Byte]
	/*
	// TH1 (Timer 1 High Byte) = 0x96
	*/
	TH1 = (0x96 << TH1_TH1__SHIFT);
	// [TH1 - Timer 1 High Byte]$

	// $[TL1 - Timer 1 Low Byte]
	/*
	// TL1 (Timer 1 Low Byte) = 0xFF
	*/
	TL1 = (0xFF << TL1_TL1__SHIFT);
	// [TL1 - Timer 1 Low Byte]$

	// $[Timer Restoration]
	//Restore Timer Configuration
	TCON = TCON_save;

	// [Timer Restoration]$


}

extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void) {
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
	CKCON0 = CKCON0_SCA__SYSCLK_DIV_12 | CKCON0_T0M__PRESCALE | CKCON0_T2MH__EXTERNAL_CLOCK
		 | CKCON0_T2ML__EXTERNAL_CLOCK | CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
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
	TMOD = TMOD_T0M__MODE0 | TMOD_T1M__MODE2 | TMOD_CT0__TIMER | TMOD_GATE0__DISABLED
		 | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
	// [TMOD - Timer 0/1 Mode]$

	// $[TCON - Timer 0/1 Control]
	/*
	// TR1 (Timer 1 Run Control) = RUN (Start Timer 1 running.)
	*/
	TCON |= TCON_TR1__RUN;
	// [TCON - Timer 0/1 Control]$


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

extern void PCACH_2_enter_DefaultMode_from_RESET(void) {
	// $[PCA0 Settings Save]
	uint8_t PCA0CN0_CR_save;
	uint8_t PCA0PWM_ARSEL_save;
	PCA0CN0_CR_save = PCA0CN0 & PCA0CN0_CR__BMASK;
	PCA0PWM_ARSEL_save = PCA0PWM & PCA0PWM_ARSEL__BMASK;

	// Turn PCA0 off while updating PCACH2
	PCA0CN0 &= ~PCA0CN0_CR__BMASK;
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
	PCA0CPM2 = PCA0CPM2_CAPN__DISABLED | PCA0CPM2_ECCF__DISABLED | PCA0CPM2_MAT__ENABLED
		 | PCA0CPM2_PWM16__8_BIT | PCA0CPM2_CAPP__DISABLED | PCA0CPM2_ECOM__DISABLED
		 | PCA0CPM2_PWM__DISABLED | PCA0CPM2_TOG__DISABLED;
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

