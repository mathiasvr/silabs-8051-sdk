//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include <SI_EFM8BB2_Register_Enums.h>
#include "InitDevice.h"

// USER PROTOTYPES
// USER FUNCTIONS
//==============================================================================
// enter_DefaultMode_from_RESET
//==============================================================================
extern void enter_DefaultMode_from_RESET(void) {
	// $[Config Calls]
	WDT_0_enter_DefaultMode_from_RESET();
	CLOCK_0_enter_DefaultMode_from_RESET();
	PORTS_0_enter_DefaultMode_from_RESET();
	PORTS_1_enter_DefaultMode_from_RESET();
	PBCFG_0_enter_DefaultMode_from_RESET();
	ADC_0_enter_DefaultMode_from_RESET();
	SPI_0_enter_DefaultMode_from_RESET();
	TIMER01_0_enter_DefaultMode_from_RESET();
	TIMER16_2_enter_DefaultMode_from_RESET();
	TIMER16_3_enter_DefaultMode_from_RESET();
	TIMER_SETUP_0_enter_DefaultMode_from_RESET();
	VREF_0_enter_DefaultMode_from_RESET();
	INTERRUPT_0_enter_DefaultMode_from_RESET();
	// [Config Calls]$

}


//==============================================================================
// WDT_0_enter_DefaultMode_from_RESET
//==============================================================================
extern void WDT_0_enter_DefaultMode_from_RESET(void) {
	// $[Watchdog Timer Init Variable Declarations]
	// [Watchdog Timer Init Variable Declarations]$
	
	// $[WDTCN - Watchdog Timer Control]
	//Disable Watchdog with key sequence
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key
	// [WDTCN - Watchdog Timer Control]$

}

extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
	// $[CLKSEL - Clock Select]
	/*
	// CLKSL (Clock Source Select) = HFOSC (Clock derived from the Internal
	//     High-Frequency Oscillator.)
	// CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
	//     selected clock source divided by 1.)
	*/
	CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	// [CLKSEL - Clock Select]$

}

extern void PBCFG_0_enter_DefaultMode_from_RESET(void) {
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

extern void SPI_0_enter_DefaultMode_from_RESET(void) {
	// $[SPI0CKR - SPI0 Clock Rate]
	/*
	// SPI0CKR (SPI0 Clock Rate) = 11
	*/
	SPI0CKR = (11 << SPI0CKR_SPI0CKR__SHIFT);
	// [SPI0CKR - SPI0 Clock Rate]$

	// $[SPI0CFG - SPI0 Configuration]
	/*
	// CKPHA (SPI0 Clock Phase) = DATA_CENTERED_FIRST (Data centered on first
	//     edge of SCK period.)
	// CKPOL (SPI0 Clock Polarity) = IDLE_LOW (SCK line low in idle state.)
	// MSTEN (Master Mode Enable) = MASTER_ENABLED (Enable master mode.
	//     Operate as a master.)
	*/
	SPI0CFG = SPI0CFG_CKPHA__DATA_CENTERED_FIRST | SPI0CFG_CKPOL__IDLE_LOW
		 | SPI0CFG_MSTEN__MASTER_ENABLED;
	// [SPI0CFG - SPI0 Configuration]$

	// $[SPI0CN0 - SPI0 Control]
	/*
	// SPIEN (SPI0 Enable) = ENABLED (Enable the SPI module.)
	// NSSMD (Slave Select Mode) = 3_WIRE (3-Wire Slave or 3-Wire Master
	//     Mode. NSS signal is not routed to a port pin.)
	*/
	SPI0CN0 &= ~SPI0CN0_NSSMD__FMASK;
	SPI0CN0 |= SPI0CN0_SPIEN__ENABLED;
	// [SPI0CN0 - SPI0 Control]$

	// Workaround bug with 3-wire SPI master mode
	SPI0CN0 &= ~SPI0CN0_NSSMD__FMASK;
	SPI0CN0 |= SPI0CN0_NSSMD__3_WIRE;
}

extern void PORTS_0_enter_DefaultMode_from_RESET(void) {
	// $[P0MDIN - Port 1 Input Mode]
	// [P0MDIN - Port 1 Input Mode]$

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
	// B5 (Port 0 Bit 5 Mask Value) = IGNORED (P0.5 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B6 (Port 0 Bit 6 Mask Value) = IGNORED (P0.6 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	// B7 (Port 0 Bit 7 Mask Value) = IGNORED (P0.7 pin logic value is
	//     ignored and will not cause a port mismatch event.)
	*/
	P0MASK = P0MASK_B0__IGNORED | P0MASK_B1__IGNORED | P0MASK_B2__COMPARED
		 | P0MASK_B3__IGNORED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
		 | P0MASK_B6__IGNORED | P0MASK_B7__IGNORED;
	// [P0MASK - Port 0 Mask]$

	// $[P0MAT - Port 0 Match]
	// [P0MAT - Port 0 Match]$

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
	// B6 (Port 0 Bit 6 Output Mode) = PUSH_PULL (P0.6 output is push-pull.)
	// B7 (Port 0 Bit 7 Output Mode) = OPEN_DRAIN (P0.7 output is open-
	//     drain.)
	*/
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__PUSH_PULL | P0MDOUT_B2__OPEN_DRAIN
		 | P0MDOUT_B3__OPEN_DRAIN | P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN
		 | P0MDOUT_B6__PUSH_PULL | P0MDOUT_B7__OPEN_DRAIN;
	// [P0MDOUT - Port 0 Output Mode]$

	// $[P0SKIP - Port 0 Skip]
	/*
	// B0 (Port 0 Bit 0 Skip) = SKIPPED (P0.0 pin is skipped by the
	//     crossbar.)
	// B1 (Port 0 Bit 1 Skip) = SKIPPED (P0.1 pin is skipped by the
	//     crossbar.)
	// B2 (Port 0 Bit 2 Skip) = SKIPPED (P0.2 pin is skipped by the
	//     crossbar.)
	// B3 (Port 0 Bit 3 Skip) = SKIPPED (P0.3 pin is skipped by the
	//     crossbar.)
	// B4 (Port 0 Bit 4 Skip) = SKIPPED (P0.4 pin is skipped by the
	//     crossbar.)
	// B5 (Port 0 Bit 5 Skip) = SKIPPED (P0.5 pin is skipped by the
	//     crossbar.)
	// B6 (Port 0 Bit 6 Skip) = NOT_SKIPPED (P0.6 pin is not skipped by the
	//     crossbar.)
	// B7 (Port 0 Bit 7 Skip) = NOT_SKIPPED (P0.7 pin is not skipped by the
	//     crossbar.)
	*/
	P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED
		 | P0SKIP_B3__SKIPPED | P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
		 | P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__NOT_SKIPPED;
	// [P0SKIP - Port 0 Skip]$

}

extern void PORTS_1_enter_DefaultMode_from_RESET(void) {
	// $[P1MDIN - Port 1 Input Mode]
	/*
	// B0 (Port 1 Bit 0 Input Mode) = DIGITAL (P1.0 pin is configured for
	//     digital mode.)
	// B1 (Port 1 Bit 1 Input Mode) = ANALOG (P1.1 pin is configured for
	//     analog mode.)
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
	P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__ANALOG | P1MDIN_B2__DIGITAL
		 | P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__DIGITAL
		 | P1MDIN_B6__DIGITAL | P1MDIN_B7__ANALOG;
	// [P1MDIN - Port 1 Input Mode]$

	// $[P1 - Port 1 Pin Latch]
	// [P1 - Port 1 Pin Latch]$

	// $[P1MASK - Port 1 Mask]
	// [P1MASK - Port 1 Mask]$

	// $[P1MAT - Port 1 Match]
	// [P1MAT - Port 1 Match]$

	// $[P1MDOUT - Port 1 Output Mode]
	/*
	// B0 (Port 1 Bit 0 Output Mode) = PUSH_PULL (P1.0 output is push-pull.)
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
	// B6 (Port 1 Bit 6 Output Mode) = OPEN_DRAIN (P1.6 output is open-
	//     drain.)
	// B7 (Port 1 Bit 7 Output Mode) = OPEN_DRAIN (P1.7 output is open-
	//     drain.)
	*/
	P1MDOUT = P1MDOUT_B0__PUSH_PULL | P1MDOUT_B1__OPEN_DRAIN | P1MDOUT_B2__OPEN_DRAIN
		 | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__OPEN_DRAIN
		 | P1MDOUT_B6__OPEN_DRAIN | P1MDOUT_B7__OPEN_DRAIN;
	// [P1MDOUT - Port 1 Output Mode]$

	// $[P1SKIP - Port 1 Skip]
	/*
	// B0 (Port 1 Bit 0 Skip) = NOT_SKIPPED (P1.0 pin is not skipped by the
	//     crossbar.)
	// B1 (Port 1 Bit 1 Skip) = SKIPPED (P1.1 pin is not skipped by the
	//     crossbar.)
	// B2 (Port 1 Bit 2 Skip) = NOT_SKIPPED (P1.2 pin is not skipped by the
	//     crossbar.)
	// B3 (Port 1 Bit 3 Skip) = NOT_SKIPPED (P1.3 pin is skipped by the
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
	P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__SKIPPED | P1SKIP_B2__NOT_SKIPPED
		 | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__NOT_SKIPPED | P1SKIP_B5__NOT_SKIPPED
		 | P1SKIP_B6__NOT_SKIPPED | P1SKIP_B7__SKIPPED;
	// [P1SKIP - Port 1 Skip]$

}

extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void) {
	// $[CKCON0 - Clock Control]
	/*
	// SCA (Timer 0/1 Prescale) = SYSCLK_DIV_48 (System clock divided by 48.)
	// T0M (Timer 0 Clock Select) = PRESCALE (Counter/Timer 0 uses the clock
	//     defined by the prescale field, SCA.)
	// T2MH (Timer 2 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 2 high
	//     byte uses the clock defined by the T2XCLK bit in TMR2CN0.)
	// T2ML (Timer 2 Low Byte Clock Select) = EXTERNAL_CLOCK (Timer 2 low
	//     byte uses the clock defined by the T2XCLK bit in TMR2CN0.)
	// T3MH (Timer 3 High Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 high
	//     byte uses the clock defined by the T3XCLK bit in TMR3CN0.)
	// T3ML (Timer 3 Low Byte Clock Select) = EXTERNAL_CLOCK (Timer 3 low
	//     byte uses the clock defined by the T3XCLK bit in TMR3CN0.)
	// T1M (Timer 1 Clock Select) = PRESCALE (Timer 1 uses the clock defined
	//     by the prescale field, SCA.)
	*/
	CKCON0 = CKCON0_SCA__SYSCLK_DIV_48 | CKCON0_T0M__PRESCALE | CKCON0_T2MH__EXTERNAL_CLOCK
		 | CKCON0_T2ML__EXTERNAL_CLOCK | CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
		 | CKCON0_T1M__PRESCALE;
	// [CKCON0 - Clock Control]$

	// $[TMOD - Timer 0/1 Mode]
	/*
	// T0M (Timer 0 Mode Select) = MODE2 (Mode 2, 8-bit Counter/Timer with
	//     Auto-Reload)
	// CT0 (Counter/Timer 0 Select) = TIMER (Timer Mode. Timer 0 increments
	//     on the clock defined by T0M in the CKCON0 register.)
	// GATE0 (Timer 0 Gate Control) = DISABLED (Timer 0 enabled when TR0 = 1
	//     irrespective of /INT0 logic level.)
	// T1M (Timer 1 Mode Select) = MODE0 (Mode 0, 13-bit Counter/Timer)
	// CT1 (Counter/Timer 1 Select) = TIMER (Timer Mode. Timer 1 increments
	//     on the clock defined by T1M in the CKCON0 register.)
	// GATE1 (Timer 1 Gate Control) = DISABLED (Timer 1 enabled when TR1 = 1
	//     irrespective of /INT1 logic level.)
	*/
	TMOD = TMOD_T0M__MODE2 | TMOD_CT0__TIMER | TMOD_GATE0__DISABLED
		 | TMOD_T1M__MODE0 | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
	// [TMOD - Timer 0/1 Mode]$

	// $[TCON - Timer 0/1 Control]
	/*
	// TR0 (Timer 0 Run Control) = RUN (Start Timer 0 running.)
	*/
	TCON |= TCON_TR0__RUN;
	// [TCON - Timer 0/1 Control]$

}

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


extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void) {
	// $[EIE1 - Extended Interrupt Enable 1]
	/*
	// EADC0 (ADC0 Conversion Complete Interrupt Enable) = ENABLED (Enable
	//     interrupt requests generated by the ADINT flag.)
	// ECP0 (Comparator0 (CP0) Interrupt Enable) = DISABLED (Disable CP0
	//     interrupts.)
	// ECP1 (Comparator1 (CP1) Interrupt Enable) = DISABLED (Disable CP1
	//     interrupts.)
	// EMAT (Port Match Interrupts Enable) = ENABLED (Enable interrupt
	//     requests generated by a Port Match.)
	// EPCA0 (Programmable Counter Array (PCA0) Interrupt Enable) = DISABLED
	//     (Disable all PCA0 interrupts.)
	// ESMB0 (SMBus (SMB0) Interrupt Enable) = DISABLED (Disable all SMB0
	//     interrupts.)
	// ET3 (Timer 3 Interrupt Enable) = ENABLED (Enable interrupt requests
	//     generated by the TF3L or TF3H flags.)
	// EWADC0 (Window Comparison ADC0 Interrupt Enable) = DISABLED (Disable
	//     ADC0 Window Comparison interrupt.)
	*/
	EIE1 = EIE1_EADC0__ENABLED | EIE1_ECP0__DISABLED | EIE1_ECP1__DISABLED
		 | EIE1_EMAT__ENABLED | EIE1_EPCA0__DISABLED | EIE1_ESMB0__DISABLED
		 | EIE1_ET3__ENABLED | EIE1_EWADC0__DISABLED;
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
	// ESPI0 (SPI0 Interrupt Enable) = ENABLED (Enable interrupt requests
	//     generated by SPI0.)
	// ET0 (Timer 0 Interrupt Enable) = DISABLED (Disable all Timer 0
	//     interrupt.)
	// ET1 (Timer 1 Interrupt Enable) = DISABLED (Disable all Timer 1
	//     interrupt.)
	// ET2 (Timer 2 Interrupt Enable) = ENABLED (Enable interrupt requests
	//     generated by the TF2L or TF2H flags.)
	// ES0 (UART0 Interrupt Enable) = DISABLED (Disable UART0 interrupt.)
	*/
	IE = IE_EA__DISABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__ENABLED
		 | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__ENABLED | IE_ES0__DISABLED;
	// [IE - Interrupt Enable]$

	// $[IP - Interrupt Priority]
	// [IP - Interrupt Priority]$

}

extern void TIMER16_3_enter_DefaultMode_from_RESET(void) {

	// $[Timer Initialization]
	// Save Timer Configuration
	uint8_t TMR3CN0_TR3_save = TMR3CN0 & TMR3CN0_TR3__BMASK;
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
	/*
	// TMR3RLH (Timer 3 Reload High Byte) = 248
	*/
	TMR3RLH = (248 << TMR3RLH_TMR3RLH__SHIFT);
	// [TMR3RLH - Timer 3 Reload High Byte]$

	// $[TMR3RLL - Timer 3 Reload Low Byte]
	/*
	// TMR3RLL (Timer 3 Reload Low Byte) = 6
	*/
	TMR3RLL = (6 << TMR3RLL_TMR3RLL__SHIFT);
	// [TMR3RLL - Timer 3 Reload Low Byte]$

	// $[TMR3CN0]
	/*
	// TR3 (Timer 3 Run Control) = RUN (Start Timer 3 running.)
	*/
	TMR3CN0 |= TMR3CN0_TR3__RUN;
	// [TMR3CN0]$

	// $[Timer Restoration]
	// Restore Timer Configuration
	TMR3CN0 |= TMR3CN0_TR3_save;
	// [Timer Restoration]$

}

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
	// [ADC0PWR - ADC0 Power Control]$

	// $[ADC0TK - ADC0 Burst Mode Track Time]
	// [ADC0TK - ADC0 Burst Mode Track Time]$

	// $[ADC0MX - ADC0 Multiplexer Selection]
	/*
	// ADC0MX (AMUX0 Positive Input Selection) = ADC0P15 (Select ADC0.15.)
	*/
	ADC0MX = ADC0MX_ADC0MX__ADC0P15;
	// [ADC0MX - ADC0 Multiplexer Selection]$

	// $[ADC0AC - ADC0 Accumulator Configuration]
	// [ADC0AC - ADC0 Accumulator Configuration]$

	// $[ADC0CF - ADC0 Configuration]
	/*
	// ADSC (SAR Clock Divider) = 1
	// AD8BE (8-Bit Mode Enable) = NORMAL (ADC0 operates in 10-bit or 12-bit
	//     mode (normal operation).)
	// ADGN (Gain Control) = GAIN_1 (The on-chip PGA gain is 1.)
	// ADTM (Track Mode) = TRACK_NORMAL (Normal Track Mode. When ADC0 is
	//     enabled, conversion begins immediately following the start-of-
	//     conversion signal.)
	*/
	ADC0CF = (1 << ADC0CF_ADSC__SHIFT) | ADC0CF_AD8BE__NORMAL | ADC0CF_ADGN__GAIN_1
		 | ADC0CF_ADTM__TRACK_NORMAL;
	// [ADC0CF - ADC0 Configuration]$

	// $[ADC0CN1 - ADC0 Control 1]
	// [ADC0CN1 - ADC0 Control 1]$

	// $[ADC0CN0 - ADC0 Control 0]
	/*
	// ADEN (ADC Enable) = ENABLED (Enable ADC0 (active and ready for data
	//     conversions).)
	// ADCM (Start of Conversion Mode Select) = TIMER0 (ADC0 conversion
	//     initiated on overflow of Timer 0.)
	*/
	ADC0CN0 &= ~ADC0CN0_ADCM__FMASK;
	ADC0CN0 |= ADC0CN0_ADEN__ENABLED
		 | ADC0CN0_ADCM__TIMER0;
	// [ADC0CN0 - ADC0 Control 0]$

}

extern void VREF_0_enter_DefaultMode_from_RESET(void) {
	// $[REF0CN - Voltage Reference Control]
	/*
	// REFSL (Voltage Reference Select) = VDD_PIN (The ADC0 voltage reference
	//     is the VDD pin.)
	// IREFLVL (Internal Voltage Reference Level) = 1P65 (The internal
	//     reference operates at 1.65 V nominal.)
	// GNDSL (Analog Ground Reference) = GND_PIN (The ADC0 ground reference
	//     is the GND pin.)
	// TEMPE (Temperature Sensor Enable) = TEMP_DISABLED (Disable the
	//     Temperature Sensor.)
	*/
	REF0CN = REF0CN_REFSL__VDD_PIN | REF0CN_IREFLVL__1P65 | REF0CN_GNDSL__GND_PIN
		 | REF0CN_TEMPE__TEMP_DISABLED;
	// [REF0CN - Voltage Reference Control]$

}

extern void TIMER01_0_enter_DefaultMode_from_RESET(void) {
	// $[Timer Initialization]
	//Save Timer Configuration
	uint8_t TCON_save = TCON;
	//Stop Timers
	TCON &= TCON_TR0__BMASK & TCON_TR1__BMASK;

	// [Timer Initialization]$

	// $[TH0 - Timer 0 High Byte]
	/*
	// TH0 (Timer 0 High Byte) = 1
	*/
	TH0 = (1 << TH0_TH0__SHIFT);
	// [TH0 - Timer 0 High Byte]$

	// $[TL0 - Timer 0 Low Byte]
	// [TL0 - Timer 0 Low Byte]$

	// $[TH1 - Timer 1 High Byte]
	// [TH1 - Timer 1 High Byte]$

	// $[TL1 - Timer 1 Low Byte]
	// [TL1 - Timer 1 Low Byte]$

	// $[Timer Restoration]
	//Restore Timer Configuration
	TCON = TCON_save;

	// [Timer Restoration]$
}

