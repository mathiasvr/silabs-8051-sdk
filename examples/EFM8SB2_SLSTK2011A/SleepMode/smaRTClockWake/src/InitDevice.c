//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8SB2_Register_Enums.h>
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
	PORTS_1_enter_DefaultMode_from_RESET();
	PORTS_2_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	RSTSRC_0_enter_DefaultMode_from_RESET();
	RTC_0_enter_DefaultMode_from_RESET();
	FLASH_0_enter_DefaultMode_from_RESET();
	TIMER16_2_enter_DefaultMode_from_RESET();
	INTERRUPT_0_enter_DefaultMode_from_RESET();
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
// PORTS_1_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_1_enter_DefaultMode_from_RESET(void) {
	// $[P1 - Port 1 Pin Latch]
	// [P1 - Port 1 Pin Latch]$

	// $[P1MDOUT - Port 1 Output Mode]
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
	// B6 (Port 1 Bit 6 Input Mode) = ANALOG (P1.6 pin is configured for
	//     analog mode.)
	// B7 (Port 1 Bit 7 Input Mode) = ANALOG (P1.7 pin is configured for
	//     analog mode.)
	*/
	P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__DIGITAL | P1MDIN_B2__DIGITAL
		 | P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__DIGITAL
		 | P1MDIN_B6__ANALOG | P1MDIN_B7__ANALOG;
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
	// B6 (Port 1 Bit 6 Skip) = SKIPPED (P1.6 pin is skipped by the
	//     crossbar.)
	// B7 (Port 1 Bit 7 Skip) = SKIPPED (P1.7 pin is skipped by the
	//     crossbar.)
	*/
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED | P1SKIP_B2__NOT_SKIPPED
		 | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__NOT_SKIPPED | P1SKIP_B5__NOT_SKIPPED
		 | P1SKIP_B6__SKIPPED | P1SKIP_B7__SKIPPED;
	// [P1SKIP - Port 1 Skip]$

	// $[P1MASK - Port 1 Mask]
	// [P1MASK - Port 1 Mask]$

	// $[P1MAT - Port 1 Match]
	// [P1MAT - Port 1 Match]$

	// $[P1DRV - Port 1 Drive Strength]
	// [P1DRV - Port 1 Drive Strength]$


}

//================================================================================
// PORTS_2_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_2_enter_DefaultMode_from_RESET(void) {
	// $[P2 - Port 2 Pin Latch]
	// [P2 - Port 2 Pin Latch]$

	// $[P2MDOUT - Port 2 Output Mode]
	/*
	// B0 (Port 2 Bit 0 Output Mode) = PUSH_PULL (P2.0 output is push-pull.)
	// B1 (Port 2 Bit 1 Output Mode) = PUSH_PULL (P2.1 output is push-pull.)
	// B2 (Port 2 Bit 2 Output Mode) = OPEN_DRAIN (P2.2 output is open-
	//     drain.)
	// B3 (Port 2 Bit 3 Output Mode) = OPEN_DRAIN (P2.3 output is open-
	//     drain.)
	// B4 (Port 2 Bit 4 Output Mode) = OPEN_DRAIN (P2.4 output is open-
	//     drain.)
	// B5 (Port 2 Bit 5 Output Mode) = OPEN_DRAIN (P2.5 output is open-
	//     drain.)
	// B6 (Port 2 Bit 6 Output Mode) = OPEN_DRAIN (P2.6 output is open-
	//     drain.)
	// B7 (Port 2 Bit 7 Output Mode) = OPEN_DRAIN (P2.7 output is open-
	//     drain.)
	*/
	P2MDOUT = P2MDOUT_B0__PUSH_PULL | P2MDOUT_B1__PUSH_PULL | P2MDOUT_B2__OPEN_DRAIN
		 | P2MDOUT_B3__OPEN_DRAIN | P2MDOUT_B4__OPEN_DRAIN | P2MDOUT_B5__OPEN_DRAIN
		 | P2MDOUT_B6__OPEN_DRAIN | P2MDOUT_B7__OPEN_DRAIN;
	// [P2MDOUT - Port 2 Output Mode]$

	// $[P2MDIN - Port 2 Input Mode]
	// [P2MDIN - Port 2 Input Mode]$

	// $[P2SKIP - Port 2 Skip]
	// [P2SKIP - Port 2 Skip]$

	// $[P2DRV - Port 2 Drive Strength]
	// [P2DRV - Port 2 Drive Strength]$


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
	// A variable for providing a delay for external oscillator startup
	uint16_t delayCounter;
	// Save the system clock (the system clock will be slowed during the startup delay)
	uint8_t CLKSEL_save = CLKSEL;

	// Unlock the RTC Interface
	RTC0KEY = RTC0KEY_RTC0ST__KEY1;
	RTC0KEY = RTC0KEY_RTC0ST__KEY2;

	// Enable power to the SmaRTClock oscillator circuit (RTC0EN = 1)
	// [RTC Initialization]$

	// $[RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]
	// Set SmaRTClock to Crystal Mode (XMODE = 1).
	// Disable Automatic Gain Control (AGCEN) and enable Bias Doubling (BIASX2) for fast crystal startup.
	RTC0ADR = RTC0XCN0;
	RTC0DAT = RTC0XCN0_XMODE__CRYSTAL | RTC0XCN0_BIASX2__ENABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	// [RTC0XCN0 - RTC Oscillator Control: Initial start-up setting]$

	// $[RTC0XCF - RTC Oscillator Configuration]
	/*
	// AUTOSTP (Automatic Load Capacitance Stepping Enable) = ENABLED (Enable
	//     load capacitance stepping.)
	// LOADCAP (Load Capacitance Programmed Value) = 0x03
	*/
	RTC0ADR = RTC0XCF;
	RTC0DAT = RTC0XCF_AUTOSTP__ENABLED | (0x03 << RTC0XCF_LOADCAP__SHIFT);
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
	/*
	// ALARM2 (RTC Alarm Programmed Value 2) = 0x01
	*/
	RTC0ADR = ALARM2;
	RTC0DAT = (0x01 << ALARM2_ALARM2__SHIFT);
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	// [ALARM2 - RTC Alarm Programmed Value 2]$

	// $[ALARM3 - RTC Alarm Programmed Value 3]
	// [ALARM3 - RTC Alarm Programmed Value 3]$

	// $[RTC0CN - RTC Control]
	/*
	// RTC0EN (RTC Enable) = ENABLED (Enable RTC oscillator.)
	*/
	RTC0ADR = RTC0CN0;
	RTC0DAT = 0;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	RTC0ADR = RTC0CN0;
	RTC0DAT |= RTC0CN0_RTC0EN__ENABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// [RTC0CN - RTC Control]$

	// $[External Oscillator Setup]
	// Set the clock to a known value for the delay
	CLKSEL = CLKSEL_CLKSL__LPOSC | CLKSEL_CLKDIV__SYSCLK_DIV_128;

	// Delay for > 20 ms
	for (delayCounter=0x150;delayCounter!=0;delayCounter--);

	// Poll the SmaRTClock Clock Valid Bit (CLKVLD) until the crystal oscillator stabilizes
	do {
	RTC0ADR = RTC0ADR_BUSY__SET | RTC0ADR_ADDR__RTC0XCN0;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit
	} while ((RTC0DAT & RTC0XCN0_CLKVLD__BMASK)== RTC0XCN0_CLKVLD__NOT_SET);

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
	RTC0ADR = RTC0XCN0;
	RTC0DAT = RTC0XCN0_XMODE__CRYSTAL | RTC0XCN0_AGCEN__ENABLED | RTC0XCN0_BIASX2__DISABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// Enable the SmaRTClock missing clock detector.
	/*
	// MCLKEN (Missing RTC Detector Enable) = ENABLED (Enable missing RTC
	//     detector.)
	*/
	RTC0ADR = RTC0CN0;
	RTC0DAT |= RTC0CN0_MCLKEN__ENABLED;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// Delay for > 2 ms
	for (delayCounter=0x100;delayCounter!=0;delayCounter--);

	// Clear PMU wake-up source flags
	PMU0CF = PMU0CF_CLEAR__ALL_FLAGS;

	// Restore system clock
	CLKSEL = CLKSEL_save;
	// Poll CLKRDY to wait for the clock to stabilize
	while(!((CLKSEL & CLKSEL_CLKRDY__BMASK) == CLKSEL_CLKRDY__SET));

	/*
	// RTC0EN (RTC Enable) = ENABLED (Enable RTC oscillator.)
	// RTC0TR (RTC Timer Run Control) = RUN (RTC timer is running.)
	// MCLKEN (Missing RTC Detector Enable) = ENABLED (Enable missing RTC
	//     detector.)
	// RTC0AEN (RTC Alarm Enable) = DISABLED (Disable RTC alarm.)
	// ALRM (RTC Alarm Event Flag and Auto Reset Enable) = NOT_SET (Alarm
	//     event flag is not set or disable the auto reset function.)
	// RTC0CAP (RTC Timer Capture) = NOT_SET (Do not start a capture
	//     operation.)
	// RTC0SET (RTC Timer Set) = NOT_SET (Do not start a set operation.)
	*/
	RTC0ADR = RTC0CN0;
	RTC0DAT = RTC0CN0_RTC0EN__ENABLED | RTC0CN0_RTC0TR__RUN | RTC0CN0_MCLKEN__ENABLED
		 | RTC0CN0_RTC0AEN__DISABLED | RTC0CN0_ALRM__NOT_SET | RTC0CN0_RTC0CAP__NOT_SET
		 | RTC0CN0_RTC0SET__NOT_SET;
	while((RTC0ADR & RTC0ADR_BUSY__BMASK) == RTC0ADR_BUSY__SET);    //Poll Busy Bit

	// [External Oscillator Setup]$


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
// TIMER16_2_enter_DefaultMode_from_RESET
//================================================================================
extern void TIMER16_2_enter_DefaultMode_from_RESET(void) {
	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR2CN0_TR2_save = TMR2CN0 & TMR2CN0_TR2__BMASK;
	// Stop Timer
	TMR2CN0 &= ~(TMR2CN0_TR2__BMASK);
	// [Timer Initialization]$

	// $[TMR2CN0 - Timer 2 Control]
	/*
	// T2XCLK (Timer 2 External Clock Select) = RTC_DIV_8_CAP_CMP0 (External
	//     Clock is RTC/8. Capture trigger is Comparator 0.)
	*/
	TMR2CN0 |= TMR2CN0_T2XCLK__RTC_DIV_8_CAP_CMP0;
	// [TMR2CN0 - Timer 2 Control]$

	// $[TMR2H - Timer 2 High Byte]
	/*
	// TMR2H (Timer 2 High Byte) = 0xF9
	*/
	TMR2H = (0xF9 << TMR2H_TMR2H__SHIFT);
	// [TMR2H - Timer 2 High Byte]$

	// $[TMR2L - Timer 2 Low Byte]
	/*
	// TMR2L (Timer 2 Low Byte) = 0x9A
	*/
	TMR2L = (0x9A << TMR2L_TMR2L__SHIFT);
	// [TMR2L - Timer 2 Low Byte]$

	// $[TMR2RLH - Timer 2 Reload High Byte]
	/*
	// TMR2RLH (Timer 2 Reload High Byte) = 0xFF
	*/
	TMR2RLH = (0xFF << TMR2RLH_TMR2RLH__SHIFT);
	// [TMR2RLH - Timer 2 Reload High Byte]$

	// $[TMR2RLL - Timer 2 Reload Low Byte]
	/*
	// TMR2RLL (Timer 2 Reload Low Byte) = 0x33
	*/
	TMR2RLL = (0x33 << TMR2RLL_TMR2RLL__SHIFT);
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

//================================================================================
// INTERRUPT_0_enter_DefaultMode_from_RESET
//================================================================================
extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void) {
	// $[EIE1 - Extended Interrupt Enable 1]
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
	// ET2 (Timer 2 Interrupt Enable) = ENABLED (Enable interrupt requests
	//     generated by the TF2L or TF2H flags.)
	// ES0 (UART0 Interrupt Enable) = DISABLED (Disable UART0 interrupt.)
	*/
	IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__DISABLED
		 | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__ENABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

	// $[EIE2 - Extended Interrupt Enable 2]
	// [EIE2 - Extended Interrupt Enable 2]$

	// $[EIP2 - Extended Interrupt Priority 2]
	// [EIP2 - Extended Interrupt Priority 2]$


}

