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
	VREG_0_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	RSTSRC_0_enter_DefaultMode_from_RESET();
	HFOSC_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	FLASH_0_enter_DefaultMode_from_RESET();
	TIMER01_0_enter_DefaultMode_from_RESET();
	TIMER16_2_enter_DefaultMode_from_RESET();
	TIMER_SETUP_0_enter_DefaultMode_from_RESET();
	UART_0_enter_DefaultMode_from_RESET();
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
// VREG_0_enter_DefaultMode_from_RESET
//================================================================================
extern void VREG_0_enter_DefaultMode_from_RESET(void) {
	// $[REG0CN - Voltage Regulator Control]
	/*
	// OSCBIAS (High Frequency Oscillator Bias) = ENABLED (Enable the
	//     precision High Frequency Oscillator bias.)
	*/
	REG0CN = REG0CN_OSCBIAS__ENABLED;
	// [REG0CN - Voltage Regulator Control]$


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

	// $[P0MDIN - Port 0 Input Mode]
	// [P0MDIN - Port 0 Input Mode]$

	// $[P0SKIP - Port 0 Skip]
	/*
	// B0 (Port 0 Bit 0 Skip) = NOT_SKIPPED (P0.0 pin is not skipped by the
	//     crossbar.)
	// B1 (Port 0 Bit 1 Skip) = NOT_SKIPPED (P0.1 pin is not skipped by the
	//     crossbar.)
	// B2 (Port 0 Bit 2 Skip) = SKIPPED (P0.2 pin is skipped by the
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
	P0SKIP = P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED | P0SKIP_B2__SKIPPED
		 | P0SKIP_B3__NOT_SKIPPED | P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
		 | P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__NOT_SKIPPED;
	// [P0SKIP - Port 0 Skip]$

	// $[P0MASK - Port 0 Mask]
	/*
	// B0 (Port 0 Bit 0 Mask Value) = IGNORED (P0.0 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B1 (Port 0 Bit 1 Mask Value) = IGNORED (P0.1 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B2 (Port 0 Bit 2 Mask Value) = COMPARED (P0.2 pin logic value is
	//     compared to P0MAT.2.)
	// B3 (Port 0 Bit 3 Mask Value) = IGNORED (P0.3 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B4 (Port 0 Bit 4 Mask Value) = IGNORED (P0.4 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B5 (Port 0 Bit 5 Mask Value) = COMPARED (P0.5 pin logic value is
	//     compared to P0MAT.5.)
	// B6 (Port 0 Bit 6 Mask Value) = IGNORED (P0.6 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B7 (Port 0 Bit 7 Mask Value) = IGNORED (P0.7 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	*/
	P0MASK = P0MASK_B0__IGNORED | P0MASK_B1__IGNORED | P0MASK_B2__COMPARED
		 | P0MASK_B3__IGNORED | P0MASK_B4__IGNORED | P0MASK_B5__COMPARED
		 | P0MASK_B6__IGNORED | P0MASK_B7__IGNORED;
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
	// B5 (Port 1 Bit 5 Output Mode) = PUSH_PULL (P1.5 output is push-pull.)
	// B6 (Port 1 Bit 6 Output Mode) = OPEN_DRAIN (P1.6 output is open-
	//     drain.)
	// B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
	//     drain.)
	*/
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__PUSH_PULL | P1MDOUT_B2__OPEN_DRAIN
		 | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__PUSH_PULL
		 | P1MDOUT_B6__OPEN_DRAIN | P1MDOUT_B7__OPEN_DRAIN;
	// [P1MDOUT - Port 1 Output Mode]$

	// $[P1MDIN - Port 1 Input Mode]
	// [P1MDIN - Port 1 Input Mode]$

	// $[P1SKIP - Port 1 Skip]
	/*
	// B0 (Port 1 Bit 0 Skip) = NOT_SKIPPED (P1.0 pin is not skipped by the
	//     crossbar.)
	// B1 (Port 1 Bit 1 Skip) = SKIPPED (P1.1 pin is skipped by the
	//     crossbar.)
	// B2 (Port 1 Bit 2 Skip) = NOT_SKIPPED (P1.2 pin is not skipped by the
	//     crossbar.)
	// B3 (Port 1 Bit 3 Skip) = NOT_SKIPPED (P1.3 pin is not skipped by the
	//     crossbar.)
	// B4 (Port 1 Bit 4 Skip) = NOT_SKIPPED (P1.4 pin is not skipped by the
	//     crossbar.)
	// B5 (Port 1 Bit 5 Skip) = SKIPPED (P1.5 pin is skipped by the
	//     crossbar.)
	// B6 (Port 1 Bit 6 Skip) = NOT_SKIPPED (P1.6 pin is not skipped by the
	//     crossbar.)
	// B7 (Port 1 Bit 7 Skip) = NOT_SKIPPED (P1.7 pin is not skipped by the
	//     crossbar.)
	*/
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__SKIPPED | P1SKIP_B2__NOT_SKIPPED
		 | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__NOT_SKIPPED | P1SKIP_B5__SKIPPED
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
	/*
	// URT0E (UART I/O Output Enable) = ENABLED (UART TX, RX routed to Port
	//     pins P0.4 and P0.5.)
	// SPI0E (SPI I/O Enable) = DISABLED (SPI I/O unavailable at Port pins.)
	// SMB0E (SMBus0 I/O Enable) = DISABLED (SMBus0 I/O unavailable at Port
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
// HFOSC_0_enter_DefaultMode_from_RESET
//================================================================================
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
// TIMER01_0_enter_DefaultMode_from_RESET
//================================================================================
extern void TIMER01_0_enter_DefaultMode_from_RESET(void) {
	// $[Timer Initialization]
	//Save Timer Configuration
	uint8_t TCON_save = TCON;
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
	// TL1 (Timer 1 Low Byte) = 0x2C
	*/
	TL1 = (0x2C << TL1_TL1__SHIFT);
	// [TL1 - Timer 1 Low Byte]$

	// $[Timer Restoration]
	//Restore Timer Configuration
	TCON = TCON_save;

	// [Timer Restoration]$


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
	// TMR2RLH (Timer 2 Reload High Byte) = 0xF8
	*/
	TMR2RLH = (0xF8 << TMR2RLH_TMR2RLH__SHIFT);
	// [TMR2RLH - Timer 2 Reload High Byte]$

	// $[TMR2RLL - Timer 2 Reload Low Byte]
	/*
	// TMR2RLL (Timer 2 Reload Low Byte) = 0x06
	*/
	TMR2RLL = (0x06 << TMR2RLL_TMR2RLL__SHIFT);
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
// TIMER_SETUP_0_enter_DefaultMode_from_RESET
//================================================================================
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
	//     on the clock defined by T0M in the CKCON register.)
	// GATE0 (Timer 0 Gate Control) = DISABLED (Timer 0 enabled when TR0 = 1
	//     irrespective of INT0 logic level.)
	// CT1 (Counter/Timer 1 Select) = TIMER (Timer Mode. Timer 1 increments
	//     on the clock defined by T1M in the CKCON register.)
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
	// ET2 (Timer 2 Interrupt Enable) = DISABLED (Disable Timer 2 interrupt.)
	// ES0 (UART0 Interrupt Enable) = DISABLED (Disable UART0 interrupt.)
	*/
	IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__DISABLED
		 | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__DISABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

	// $[EIE2 - Extended Interrupt Enable 2]
	/*
	// ECSCPT (Capacitive Sense Conversion Complete Interrupt Enable) =
	//     DISABLED (Disable Capacitive Sense Conversion Complete interrupt.)
	// ECSDC (Capacitive Sense Digital Comparator Interrupt Enable) =
	//     DISABLED (Disable Capacitive Sense Digital Comparator interrupt.)
	// ECSEOS (Capacitive Sense End of Scan Interrupt Enable) = DISABLED
	//     (Disable Capacitive Sense End of Scan interrupt.)
	// EMAT (Port Match Interrupts Enable) = ENABLED (Enable interrupt
	//     requests generated by a Port Match.)
	// ERTC0F (RTC Oscillator Fail Interrupt Enable) = DISABLED (Disable RTC
	//     Oscillator Fail interrupts.)
	// EWARN (Supply Monitor Early Warning Interrupt Enable) = DISABLED
	//     (Disable the Supply Monitor Early Warning interrupt.)
	*/
	EIE2 = EIE2_ECSCPT__DISABLED | EIE2_ECSDC__DISABLED | EIE2_ECSEOS__DISABLED
		 | EIE2_EMAT__ENABLED | EIE2_ERTC0F__DISABLED | EIE2_EWARN__DISABLED;
	// [EIE2 - Extended Interrupt Enable 2]$

	// $[EIP2 - Extended Interrupt Priority 2]
	// [EIP2 - Extended Interrupt Priority 2]$


}

