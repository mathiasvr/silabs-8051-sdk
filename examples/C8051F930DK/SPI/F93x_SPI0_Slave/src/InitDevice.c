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
	HFOSC_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	SPI_0_enter_DefaultMode_from_RESET();
	RSTSRC_0_enter_DefaultMode_from_RESET();
	INTERRUPT_0_enter_DefaultMode_from_RESET();
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
// HFOSC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void HFOSC_0_enter_DefaultMode_from_RESET(void) {
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
// CLOCK_0_enter_DefaultMode_from_RESET
//================================================================================
extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	/*
	// CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
	//     selected clock source divided by 1.)
	// CLKSL (Clock Source Select) = HFOSC (Clock derived from the internal
	//     precision High-Frequency Oscillator.)
	*/
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__HFOSC;
	// [CLKSEL - Clock Select]$


}

//================================================================================
// PORTS_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_0_enter_DefaultMode_from_RESET(void) {
	// $[P0 - Port 0 Pin Latch]
	// [P0 - Port 0 Pin Latch]$

	// $[P0MDOUT - Port 0 Output Mode]
	/*
	// B0 (Port 0 Bit 0 Output Mode) = OPEN_DRAIN (P0.0 output is open-
	//     drain.)
	// B1 (Port 0 Bit 1 Output Mode) = PUSH_PULL (P0.1 output is push-pull.)
	// B2 (Port 0 Bit 2 Output Mode) = OPEN_DRAIN (P0.2 output is open-
	//     drain.)
	// B3 (Port 0 Bit 3 Output Mode) = OPEN_DRAIN (P0.3 output is open-
	//     drain.)
	// B4 (Port 0 Bit 4 Output Mode) = OPEN_DRAIN (P0.4 output is open-
	//     drain.)
	// B5 (Port 0 Bit 5 Output Mode) = OPEN_DRAIN (P0.5 output is open-
	//     drain.)
	// B6 (Port 0 Bit 6 Output Mode) = OPEN_DRAIN (P0.6 output is open-
	//     drain.)
	// B7 (Port 0 Bit 7 Output Mode) = OPEN_DRAIN (P0.7 output is open-
	//     drain.)
	*/
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__PUSH_PULL | P0MDOUT_B2__OPEN_DRAIN
		 | P0MDOUT_B3__OPEN_DRAIN | P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN
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
	// B7 (Port 1 Bit 7 Skip) = NOT_SKIPPED (P1.7 pin is not skipped by the
	//     crossbar.)
	*/
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED | P1SKIP_B2__NOT_SKIPPED
		 | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__NOT_SKIPPED | P1SKIP_B5__NOT_SKIPPED
		 | P1SKIP_B6__SKIPPED | P1SKIP_B7__NOT_SKIPPED;
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
	/*
	// URT0E (UART I/O Output Enable) = DISABLED (UART I/O unavailable at
	//     Port pin.)
	// SPI0E (SPI I/O Enable) = ENABLED (SPI I/O routed to Port pins. The SPI
	//     can be assigned either 3 or 4 GPIO pins.)
	// SMB0E (SMBus0 I/O Enable) = DISABLED (SMBus0 I/O unavailable at Port
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
	XBR0 = XBR0_URT0E__DISABLED | XBR0_SPI0E__ENABLED | XBR0_SMB0E__DISABLED
		 | XBR0_CP0E__DISABLED | XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
		 | XBR0_CP1AE__DISABLED | XBR0_SYSCKE__DISABLED;
	// [XBR0 - Port I/O Crossbar 0]$

	// $[XBR1 - Port I/O Crossbar 1]
	// [XBR1 - Port I/O Crossbar 1]$


}

//================================================================================
// SPI_0_enter_DefaultMode_from_RESET
//================================================================================
extern void SPI_0_enter_DefaultMode_from_RESET(void) {
	// $[SPI0CKR - SPI0 Clock Rate]
	/*
	// SPI0CKR (SPI0 Clock Rate) = 7
	*/
	SPI0CKR = (7 << SPI0CKR_SPI0CKR__SHIFT);
	// [SPI0CKR - SPI0 Clock Rate]$

	// $[SPI0CFG - SPI0 Configuration]
	// [SPI0CFG - SPI0 Configuration]$

	// $[SPI0CN - SPI0 Control]
	/*
	// SPIEN (SPI0 Enable) = ENABLED (Enable the SPI module.)
	*/
	SPI0CN |= SPI0CN_SPIEN__ENABLED;
	// [SPI0CN - SPI0 Control]$


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
	// ESPI0 (SPI0 Interrupt Enable) = ENABLED (Enable interrupt requests
	//     generated by SPI0.)
	// ET0 (Timer 0 Interrupt Enable) = DISABLED (Disable all Timer 0
	//     interrupt.)
	// ET1 (Timer 1 Interrupt Enable) = DISABLED (Disable all Timer 1
	//     interrupt.)
	// ET2 (Timer 2 Interrupt Enable) = DISABLED (Disable Timer 2 interrupt.)
	// ES0 (UART0 Interrupt Enable) = DISABLED (Disable UART0 interrupt.)
	*/
	IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__ENABLED
		 | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__DISABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

	// $[EIE2 - Extended Interrupt Enable 2]
	// [EIE2 - Extended Interrupt Enable 2]$

	// $[EIP2 - Extended Interrupt Priority 2]
	// [EIP2 - Extended Interrupt Priority 2]$


}



