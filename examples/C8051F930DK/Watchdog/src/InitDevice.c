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
// enter_SlowBlink_from_RESET
//==============================================================================
extern void enter_SlowBlink_from_RESET(void) {
	// $[Config Calls]
	PCA_0_enter_SlowBlink_from_RESET();
	HFOSC_0_enter_SlowBlink_from_RESET();
	CLOCK_0_enter_SlowBlink_from_RESET();
	PORTS_1_enter_SlowBlink_from_RESET();
	PBCFG_0_enter_SlowBlink_from_RESET();
	TIMER16_2_enter_SlowBlink_from_RESET();
	TIMER_SETUP_0_enter_SlowBlink_from_RESET();
	RSTSRC_0_enter_SlowBlink_from_RESET();
	INTERRUPT_0_enter_SlowBlink_from_RESET();
	// [Config Calls]$


}


//================================================================================
// PCA_0_enter_SlowBlink_from_RESET
//================================================================================
extern void PCA_0_enter_SlowBlink_from_RESET(void) {
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
	/*
	// CR (PCA Counter/Timer Run Control) = RUN (Start the PCA Counter/Timer
	//     running.)
	*/
	PCA0CN = PCA0CN_CR__RUN;
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
// HFOSC_0_enter_SlowBlink_from_RESET
//================================================================================
extern void HFOSC_0_enter_SlowBlink_from_RESET(void) {
	// $[OSCICL - High Frequency Oscillator Calibration]
	// [OSCICL - High Frequency Oscillator Calibration]$

	// $[OSCICN - High Frequency Oscillator Control]
	/*
	// IOSCEN (High Frequency Oscillator Enable) = ENABLED (High Frequency
	//     Oscillator enabled.)
	*/
	OSCICN |= OSCICN_IOSCEN__ENABLED;
	// [OSCICN - High Frequency Oscillator Control]$


}

//================================================================================
// CLOCK_0_enter_SlowBlink_from_RESET
//================================================================================
extern void CLOCK_0_enter_SlowBlink_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	/*
	// CLKDIV (Clock Source Divider) = SYSCLK_DIV_8 (SYSCLK is equal to
	//     selected clock source divided by 8.)
	// CLKSL (Clock Source Select) = HFOSC (Clock derived from the internal
	//     precision High-Frequency Oscillator.)
	*/
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_8 | CLKSEL_CLKSL__HFOSC;
	// [CLKSEL - Clock Select]$


}

//================================================================================
// PORTS_1_enter_SlowBlink_from_RESET
//================================================================================
extern void PORTS_1_enter_SlowBlink_from_RESET(void) {
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
	// B5 (Port 1 Bit 5 Output Mode) = OPEN_DRAIN (P1.5 output is open-
	//     drain.)
	// B6 (Port 1 Bit 6 Output Mode) = PUSH_PULL (P1.6 output is push-pull.)
	// B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
	//     drain.)
	*/
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN | P1MDOUT_B2__OPEN_DRAIN
		 | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__OPEN_DRAIN
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
// PBCFG_0_enter_SlowBlink_from_RESET
//================================================================================
extern void PBCFG_0_enter_SlowBlink_from_RESET(void) {
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
// TIMER16_2_enter_SlowBlink_from_RESET
//================================================================================
extern void TIMER16_2_enter_SlowBlink_from_RESET(void) {
	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR2CN_TR2_save = TMR2CN & TMR2CN_TR2__BMASK;
	// Stop Timer
	TMR2CN &= ~(TMR2CN_TR2__BMASK);
	// [Timer Initialization]$

	// $[TMR2CN - Timer 2 Control]
	// [TMR2CN - Timer 2 Control]$

	// $[TMR2H - Timer 2 High Byte]
	/*
	// TMR2H (Timer 2 High Byte) = 255
	*/
	TMR2H = (255 << TMR2H_TMR2H__SHIFT);
	// [TMR2H - Timer 2 High Byte]$

	// $[TMR2L - Timer 2 Low Byte]
	/*
	// TMR2L (Timer 2 Low Byte) = 255
	*/
	TMR2L = (255 << TMR2L_TMR2L__SHIFT);
	// [TMR2L - Timer 2 Low Byte]$

	// $[TMR2RLH - Timer 2 Reload High Byte]
	/*
	// TMR2RLH (Timer 2 Reload High Byte) = 156
	*/
	TMR2RLH = (156 << TMR2RLH_TMR2RLH__SHIFT);
	// [TMR2RLH - Timer 2 Reload High Byte]$

	// $[TMR2RLL - Timer 2 Reload Low Byte]
	/*
	// TMR2RLL (Timer 2 Reload Low Byte) = 79
	*/
	TMR2RLL = (79 << TMR2RLL_TMR2RLL__SHIFT);
	// [TMR2RLL - Timer 2 Reload Low Byte]$

	// $[TMR2CN]
	/*
	// TR2 (Timer 2 Run Control) = RUN (Start Timer 2 running.)
	*/
	TMR2CN |= TMR2CN_TR2__RUN;
	// [TMR2CN]$

	// $[Timer Restoration]
	// Restore Timer Configuration
	TMR2CN |= TMR2CN_TR2_save;
	// [Timer Restoration]$


}

//================================================================================
// TIMER_SETUP_0_enter_SlowBlink_from_RESET
//================================================================================
extern void TIMER_SETUP_0_enter_SlowBlink_from_RESET(void) {
	// $[CKCON - Clock Control]
	// [CKCON - Clock Control]$

	// $[TMOD - Timer 0/1 Mode]
	// [TMOD - Timer 0/1 Mode]$

	// $[TCON - Timer 0/1 Control]
	// [TCON - Timer 0/1 Control]$


}

//================================================================================
// RSTSRC_0_enter_SlowBlink_from_RESET
//================================================================================
extern void RSTSRC_0_enter_SlowBlink_from_RESET(void) {
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
// INTERRUPT_0_enter_SlowBlink_from_RESET
//================================================================================
extern void INTERRUPT_0_enter_SlowBlink_from_RESET(void) {
	// $[EIE1 - Extended Interrupt Enable 1]
	// [EIE1 - Extended Interrupt Enable 1]$

	// $[EIP1 - Extended Interrupt Priority 1]
	// [EIP1 - Extended Interrupt Priority 1]$

	// $[IE - Interrupt Enable]
	/*
	// EA (All Interrupts Enable) = DISABLED (Disable all interrupt sources.)
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
	IE = IE_EA__DISABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__DISABLED
		 | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__ENABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

	// $[EIE2 - Extended Interrupt Enable 2]
	// [EIE2 - Extended Interrupt Enable 2]$

	// $[EIP2 - Extended Interrupt Priority 2]
	// [EIP2 - Extended Interrupt Priority 2]$


}

//================================================================================
// PCA_0_enter_FastBlink_from_RESET
//================================================================================
extern void PCA_0_enter_FastBlink_from_RESET(void) {
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
	/*
	// CR (PCA Counter/Timer Run Control) = RUN (Start the PCA Counter/Timer
	//     running.)
	*/
	PCA0CN = PCA0CN_CR__RUN;
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
// HFOSC_0_enter_FastBlink_from_RESET
//================================================================================
extern void HFOSC_0_enter_FastBlink_from_RESET(void) {
	// $[OSCICL - High Frequency Oscillator Calibration]
	// [OSCICL - High Frequency Oscillator Calibration]$

	// $[OSCICN - High Frequency Oscillator Control]
	/*
	// IOSCEN (High Frequency Oscillator Enable) = ENABLED (High Frequency
	//     Oscillator enabled.)
	*/
	OSCICN |= OSCICN_IOSCEN__ENABLED;
	// [OSCICN - High Frequency Oscillator Control]$


}

//================================================================================
// CLOCK_0_enter_FastBlink_from_RESET
//================================================================================
extern void CLOCK_0_enter_FastBlink_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	/*
	// CLKDIV (Clock Source Divider) = SYSCLK_DIV_8 (SYSCLK is equal to
	//     selected clock source divided by 8.)
	// CLKSL (Clock Source Select) = HFOSC (Clock derived from the internal
	//     precision High-Frequency Oscillator.)
	*/
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_8 | CLKSEL_CLKSL__HFOSC;
	// [CLKSEL - Clock Select]$


}

//================================================================================
// PORTS_1_enter_FastBlink_from_RESET
//================================================================================
extern void PORTS_1_enter_FastBlink_from_RESET(void) {
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
	// B5 (Port 1 Bit 5 Output Mode) = OPEN_DRAIN (P1.5 output is open-
	//     drain.)
	// B6 (Port 1 Bit 6 Output Mode) = PUSH_PULL (P1.6 output is push-pull.)
	// B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
	//     drain.)
	*/
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN | P1MDOUT_B2__OPEN_DRAIN
		 | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__OPEN_DRAIN
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
// PBCFG_0_enter_FastBlink_from_RESET
//================================================================================
extern void PBCFG_0_enter_FastBlink_from_RESET(void) {
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
// TIMER16_2_enter_FastBlink_from_RESET
//================================================================================
extern void TIMER16_2_enter_FastBlink_from_RESET(void) {
	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR2CN_TR2_save = TMR2CN & TMR2CN_TR2__BMASK;
	// Stop Timer
	TMR2CN &= ~(TMR2CN_TR2__BMASK);
	// [Timer Initialization]$

	// $[TMR2CN - Timer 2 Control]
	// [TMR2CN - Timer 2 Control]$

	// $[TMR2H - Timer 2 High Byte]
	/*
	// TMR2H (Timer 2 High Byte) = 255
	*/
	TMR2H = (255 << TMR2H_TMR2H__SHIFT);
	// [TMR2H - Timer 2 High Byte]$

	// $[TMR2L - Timer 2 Low Byte]
	/*
	// TMR2L (Timer 2 Low Byte) = 255
	*/
	TMR2L = (255 << TMR2L_TMR2L__SHIFT);
	// [TMR2L - Timer 2 Low Byte]$

	// $[TMR2RLH - Timer 2 Reload High Byte]
	/*
	// TMR2RLH (Timer 2 Reload High Byte) = 236
	*/
	TMR2RLH = (236 << TMR2RLH_TMR2RLH__SHIFT);
	// [TMR2RLH - Timer 2 Reload High Byte]$

	// $[TMR2RLL - Timer 2 Reload Low Byte]
	/*
	// TMR2RLL (Timer 2 Reload Low Byte) = 16
	*/
	TMR2RLL = (16 << TMR2RLL_TMR2RLL__SHIFT);
	// [TMR2RLL - Timer 2 Reload Low Byte]$

	// $[TMR2CN]
	/*
	// TR2 (Timer 2 Run Control) = RUN (Start Timer 2 running.)
	*/
	TMR2CN |= TMR2CN_TR2__RUN;
	// [TMR2CN]$

	// $[Timer Restoration]
	// Restore Timer Configuration
	TMR2CN |= TMR2CN_TR2_save;
	// [Timer Restoration]$


}

//================================================================================
// TIMER_SETUP_0_enter_FastBlink_from_RESET
//================================================================================
extern void TIMER_SETUP_0_enter_FastBlink_from_RESET(void) {
	// $[CKCON - Clock Control]
	// [CKCON - Clock Control]$

	// $[TMOD - Timer 0/1 Mode]
	// [TMOD - Timer 0/1 Mode]$

	// $[TCON - Timer 0/1 Control]
	// [TCON - Timer 0/1 Control]$


}

//================================================================================
// RSTSRC_0_enter_FastBlink_from_RESET
//================================================================================
extern void RSTSRC_0_enter_FastBlink_from_RESET(void) {
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
// INTERRUPT_0_enter_FastBlink_from_RESET
//================================================================================
extern void INTERRUPT_0_enter_FastBlink_from_RESET(void) {
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

//================================================================================
// PCA_0_enter_SlowBlink_WD_from_SlowBlink
//================================================================================
extern void PCA_0_enter_SlowBlink_WD_from_SlowBlink(void) {
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
	/*
	// PCA0CPL2 (PCA Channel 2 Capture Module Low Byte) = 255
	*/
	PCA0CPL2 = (255 << PCA0CPL2_PCA0CPL2__SHIFT);
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
	/*
	// PCA0CPL5 (PCA Channel 5 Capture Module Low Byte) = 253
	*/
	PCA0CPL5 = (253 << PCA0CPL5_PCA0CPL5__SHIFT);
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
	// Watchdog is enabled
	PCA0MD |= PCA0MD_WDTE__ENABLED;

	// Reset WDT timer
	PCA0CPH5 = 0;

	// [WDT - Watchdog run Control]$


}

//================================================================================
// TIMER16_2_enter_SlowBlink_WD_from_SlowBlink
//================================================================================
extern void TIMER16_2_enter_SlowBlink_WD_from_SlowBlink(void) {
	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR2CN_TR2_save = TMR2CN & TMR2CN_TR2__BMASK;
	// Stop Timer
	TMR2CN &= ~(TMR2CN_TR2__BMASK);
	// [Timer Initialization]$

	// $[TMR2CN - Timer 2 Control]
	// [TMR2CN - Timer 2 Control]$

	// $[TMR2H - Timer 2 High Byte]
	// [TMR2H - Timer 2 High Byte]$

	// $[TMR2L - Timer 2 Low Byte]
	// [TMR2L - Timer 2 Low Byte]$

	// $[TMR2RLH - Timer 2 Reload High Byte]
	/*
	// TMR2RLH (Timer 2 Reload High Byte) = 255
	*/
	TMR2RLH = (255 << TMR2RLH_TMR2RLH__SHIFT);
	// [TMR2RLH - Timer 2 Reload High Byte]$

	// $[TMR2RLL - Timer 2 Reload Low Byte]
	/*
	// TMR2RLL (Timer 2 Reload Low Byte) = 255
	*/
	TMR2RLL = (255 << TMR2RLL_TMR2RLL__SHIFT);
	// [TMR2RLL - Timer 2 Reload Low Byte]$

	// $[TMR2CN]
	// [TMR2CN]$

	// $[Timer Restoration]
	// Restore Timer Configuration
	TMR2CN |= TMR2CN_TR2_save;
	// [Timer Restoration]$


}

//================================================================================
// TIMER_SETUP_0_enter_SlowBlink_WD_from_SlowBlink
//================================================================================
extern void TIMER_SETUP_0_enter_SlowBlink_WD_from_SlowBlink(void) {
	// $[CKCON - Clock Control]
	// [CKCON - Clock Control]$

	// $[TMOD - Timer 0/1 Mode]
	// [TMOD - Timer 0/1 Mode]$

	// $[TCON - Timer 0/1 Control]
	// [TCON - Timer 0/1 Control]$


}

//================================================================================
// INTERRUPT_0_enter_SlowBlink_WD_from_SlowBlink
//================================================================================
extern void INTERRUPT_0_enter_SlowBlink_WD_from_SlowBlink(void) {
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


//==============================================================================
// enter_FastBlink_from_RESET
//==============================================================================
extern void enter_FastBlink_from_RESET(void) {
	// $[Config Calls]
	PCA_0_enter_FastBlink_from_RESET();
	HFOSC_0_enter_FastBlink_from_RESET();
	CLOCK_0_enter_FastBlink_from_RESET();
	PORTS_1_enter_FastBlink_from_RESET();
	PBCFG_0_enter_FastBlink_from_RESET();
	TIMER16_2_enter_FastBlink_from_RESET();
	TIMER_SETUP_0_enter_FastBlink_from_RESET();
	RSTSRC_0_enter_FastBlink_from_RESET();
	INTERRUPT_0_enter_FastBlink_from_RESET();
	// [Config Calls]$


}


//==============================================================================
// enter_SlowBlink_WD_from_SlowBlink
//==============================================================================
extern void enter_SlowBlink_WD_from_SlowBlink(void) {
	// $[Config Calls]
	PCA_0_enter_SlowBlink_WD_from_SlowBlink();
	TIMER16_2_enter_SlowBlink_WD_from_SlowBlink();
	TIMER_SETUP_0_enter_SlowBlink_WD_from_SlowBlink();
	INTERRUPT_0_enter_SlowBlink_WD_from_SlowBlink();
	// [Config Calls]$


}



