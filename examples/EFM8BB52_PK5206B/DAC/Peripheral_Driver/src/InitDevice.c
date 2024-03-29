//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB52_Register_Enums.h>
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
  WDT_0_enter_DefaultMode_from_RESET();
  PORTS_0_enter_DefaultMode_from_RESET();
  PORTS_2_enter_DefaultMode_from_RESET();
  PORTS_3_enter_DefaultMode_from_RESET();
  PBCFG_0_enter_DefaultMode_from_RESET();
  AMUXCP_0_enterDefaultMode_from_RESET();
  DAC_0_enter_DefaultMode_from_RESET();
  CLOCK_0_enter_DefaultMode_from_RESET();
  TIMER16_4_enter_DefaultMode_from_RESET();
  TIMER_SETUP_0_enter_DefaultMode_from_RESET();
  INTERRUPT_0_enter_DefaultMode_from_RESET();
  // Restore the SFRPAGE
  SFRPAGE = SFRPAGE_save;
  // [Config Calls]$

}

//================================================================================
// WDT_0_enter_DefaultMode_from_RESET
//================================================================================
extern void WDT_0_enter_DefaultMode_from_RESET(void) {
    // $[Watchdog Timer Init Variable Declarations]
    // [Watchdog Timer Init Variable Declarations]$

    // $[WDTCN - Watchdog Timer Control]
    SFRPAGE = 0x00;
    //Disable Watchdog with key sequence
    WDTCN = 0xDE; //First key
    WDTCN = 0xAD; //Second key
    // [WDTCN - Watchdog Timer Control]$
}

//================================================================================
// PORTS_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_0_enter_DefaultMode_from_RESET(void) {
    // $[P0 - Port 0 Pin Latch]
    // [P0 - Port 0 Pin Latch]$

    // $[P0MDOUT - Port 0 Output Mode]
    /***********************************************************************
     - P0.0 output is open-drain
     - P0.1 output is open-drain
     - P0.2 output is open-drain
     - P0.3 output is open-drain
     - P0.4 output is open-drain
     - P0.5 output is open-drain
     - P0.6 output is open-drain
     - P0.7 output is open-drain
     ***********************************************************************/
    SFRPAGE = 0x00;
    P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN
            | P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__OPEN_DRAIN
            | P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN
            | P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__OPEN_DRAIN;
    // [P0MDOUT - Port 0 Output Mode]$

    // $[P0MDIN - Port 0 Input Mode]
    /***********************************************************************
     - P0.0 pin is configured for analog mode
     - P0.1 pin is configured for analog mode
     - P0.2 pin is configured for digital mode
     - P0.3 pin is configured for analog mode
     - P0.4 pin is configured for analog mode
     - P0.5 pin is configured for analog mode
     - P0.6 pin is configured for analog mode
     - P0.7 pin is configured for analog mode
     ***********************************************************************/
    P0MDIN = P0MDIN_B1__ANALOG | P0MDIN_B1__ANALOG | P0MDIN_B2__DIGITAL
            | P0MDIN_B3__ANALOG | P0MDIN_B4__ANALOG | P0MDIN_B5__ANALOG
            | P0MDIN_B6__ANALOG | P0MDIN_B7__ANALOG;
    // [P0MDIN - Port 0 Input Mode]$

    // $[P0SKIP - Port 0 Skip]
    /***********************************************************************
     - P0.0 pin is not skipped by the crossbar
     - P0.1 pin is not skipped by the crossbar
     - P0.2 pin is skipped by the crossbar
     - P0.3 pin is not skipped by the crossbar
     - P0.4 pin is not skipped by the crossbar
     - P0.5 pin is not skipped by the crossbar
     - P0.6 pin is not skipped by the crossbar
     - P0.7 pin is not skipped by the crossbar
     ***********************************************************************/
    P0SKIP = P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED
            | P0SKIP_B2__SKIPPED | P0SKIP_B3__NOT_SKIPPED
            | P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
            | P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__NOT_SKIPPED;
    // [P0SKIP - Port 0 Skip]$

    // $[P0MASK - Port 0 Mask]
    // [P0MASK - Port 0 Mask]$

    // $[P0MAT - Port 0 Match]
    // [P0MAT - Port 0 Match]$

}

//================================================================================
// PORTS_2_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_2_enter_DefaultMode_from_RESET(void) {
    // $[P2 - Port 2 Pin Latch]
    // [P2 - Port 2 Pin Latch]$

    // $[P2MDOUT - Port 2 Output Mode]
    // [P2MDOUT - Port 2 Output Mode]$

    // $[P2MDIN - Port 2 Input Mode]
    /***********************************************************************
     - P2.0 pin is configured for analog mode
     - P2.1 pin is configured for analog mode
     - P2.2 pin is configured for digital mode
     - P2.3 pin is configured for analog mode
     - P2.4 pin is configured for analog mode
     - P2.5 pin is configured for analog mode
     - P2.6 pin is configured for analog mode
     - P2.7 pin is configured for analog mode
     ***********************************************************************/
    SFRPAGE = 0x20;
    P2MDIN = P2MDIN_B0__ANALOG | P2MDIN_B1__ANALOG | P2MDIN_B2__DIGITAL
            | P2MDIN_B3__ANALOG | P2MDIN_B4__ANALOG | P2MDIN_B5__ANALOG
            | P2MDIN_B6__ANALOG | P2MDIN_B7__ANALOG;
    // [P2MDIN - Port 2 Input Mode]$

    // $[P2SKIP - Port 2 Skip]
    /***********************************************************************
     - P2.0 pin is not skipped by the crossbar
     - P2.1 pin is not skipped by the crossbar
     - P2.2 pin is skipped by the crossbar
     - P2.3 pin is not skipped by the crossbar
     - P2.4 pin is not skipped by the crossbar
     - P2.5 pin is not skipped by the crossbar
     - P2.6 pin is not skipped by the crossbar
     - P2.7 pin is not skipped by the crossbar
     ***********************************************************************/
    P2SKIP = P2SKIP_B0__NOT_SKIPPED | P2SKIP_B1__NOT_SKIPPED
            | P2SKIP_B2__SKIPPED | P2SKIP_B3__NOT_SKIPPED
            | P2SKIP_B4__NOT_SKIPPED | P2SKIP_B5__NOT_SKIPPED
            | P2SKIP_B6__NOT_SKIPPED | P2SKIP_B7__NOT_SKIPPED;
    // [P2SKIP - Port 2 Skip]$

    // $[P2MASK - Port 2 Mask]
    // [P2MASK - Port 2 Mask]$

    // $[P2MAT - Port 2 Match]
    // [P2MAT - Port 2 Match]$

}

//================================================================================
// PORTS_3_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_3_enter_DefaultMode_from_RESET(void) {
    // $[P3 - Port 3 Pin Latch]
    // [P3 - Port 3 Pin Latch]$

    // $[P3MDOUT - Port 3 Output Mode]
    /***********************************************************************
     - P3.0 output is open-drain
     - P3.1 output is open-drain
     - P3.2 output is open-drain
     - P3.3 output is open-drain
     - P3.4 output is push-pull
     - P3.7 output is open-drain
     ***********************************************************************/
    SFRPAGE = 0x20;
    P3MDOUT = P3MDOUT_B0__OPEN_DRAIN | P3MDOUT_B1__OPEN_DRAIN
        | P3MDOUT_B2__OPEN_DRAIN | P3MDOUT_B3__OPEN_DRAIN
        | P3MDOUT_B4__PUSH_PULL | P3MDOUT_B7__OPEN_DRAIN;
    // [P3MDOUT - Port 3 Output Mode]$

    // $[P3MDIN - Port 3 Input Mode]
    /***********************************************************************
     - P3.0 pin is configured for analog mode
     - P3.1 pin is configured for analog mode
     - P3.2 pin is configured for analog mode
     - P3.3 pin is configured for analog mode
     - P3.4 pin is configured for digital mode
     - P3.7 pin is configured for analog mode
     ***********************************************************************/
    P3MDIN = P3MDIN_B0__ANALOG | P3MDIN_B1__ANALOG | P3MDIN_B2__DIGITAL
            | P3MDIN_B3__ANALOG | P3MDIN_B4__DIGITAL | P3MDIN_B7__ANALOG;
    // [P3MDIN - Port 3 Input Mode]$

    // $[P3SKIP - Port 3 Skip]
    /***********************************************************************
     - P3.0 pin is not skipped by the crossbar
     - P3.1 pin is not skipped by the crossbar
     - P3.2 pin is not skipped by the crossbar
     - P3.3 pin is not skipped by the crossbar
     - P3.4 pin is skipped by the crossbar
     - P3.7 pin is not skipped by the crossbar
     ***********************************************************************/
    P2SKIP = P2SKIP_B0__NOT_SKIPPED | P2SKIP_B1__NOT_SKIPPED
            | P2SKIP_B2__SKIPPED | P2SKIP_B3__NOT_SKIPPED
            | P2SKIP_B4__NOT_SKIPPED | P2SKIP_B5__NOT_SKIPPED
            | P2SKIP_B6__NOT_SKIPPED | P2SKIP_B7__NOT_SKIPPED;
    // [P2SKIP - Port 2 Skip]$

    // $[P2MASK - Port 2 Mask]
    // [P2MASK - Port 2 Mask]$

    // $[P2MAT - Port 2 Match]
    // [P2MAT - Port 2 Match]$

}

//================================================================================
// PBCFG_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PBCFG_0_enter_DefaultMode_from_RESET(void) {
    // $[XBR2 - Port I/O Crossbar 2]
    /***********************************************************************
     - Weak Pullups enabled
     - Crossbar enabled
     - UART1 I/O unavailable at Port pin
     - UART1 RTS1 unavailable at Port pin
     - UART1 CTS1 unavailable at Port pin
     ***********************************************************************/
    SFRPAGE = 0x00;
    XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED
            | XBR2_URT1E__DISABLED | XBR2_URT1RTSE__DISABLED
            | XBR2_URT1CTSE__DISABLED;
    // [XBR2 - Port I/O Crossbar 2]$

    // $[PRTDRV - Port Drive Strength]
    // [PRTDRV - Port Drive Strength]$

    // $[XBR0 - Port I/O Crossbar 0]
    // [XBR0 - Port I/O Crossbar 0]$

    // $[XBR1 - Port I/O Crossbar 1]
    // [XBR1 - Port I/O Crossbar 1]$

}

//================================================================================
// AMUXCP_0_enter_DefaultMode_from_RESET
//================================================================================
extern void AMUXCP_0_enterDefaultMode_from_RESET(void){
    uint8_t i;

    SFRPAGE = 0x30;
    // $[CLKGRP0 - Clock Group 0]
    CLKGRP0 |= CLKGRP0_EN_CPCLK__ENABLE;
    // [CLKGRP0 - Clock Group 0]$

    // $[CP0CN - AMUXCP Control 0]
    CP0CN |= CP0CN_EN_CPLDO__ENABLED;
    for (i = 0; i < 200; i++);
    CP0CN |= CP0CN_EN_CP__ENABLED;
    while ((CP0CN & CP0CN_CP_STARTUP_DONE__BMASK) == CP0CN_CP_STARTUP_DONE__ONGOING);
    // [CP0CN - AMUXCP Control 0]$

}

//================================================================================
// DAC_0_enter_DefaultMode_from_RESET
//================================================================================
extern void DAC_0_enter_DefaultMode_from_RESET(void) {
  // $[DAC0CF0 - DAC0 Configuration 0]
  /*
   // EN (DAC0 Enable) = ENABLE (DAC0 is enabled and will drive the output
   //     pin.)
   // UPDATE (DAC0 Update Trigger Source) = TIMER4 (DAC0 output updates
   //     occur on Timer 4 high byte overflow.)
   // LJST (DAC0 Left Justify Enable) = RIGHT_JUSTIFY (DAC0 input is treated
   //     as right-justified.)
   // RSTMD (DAC0 Reset Mode) = NORMAL (All resets will reset DAC0 and its
   //     associated registers.)
   */
  SFRPAGE = 0x30;
  DAC0CF0 = DAC0CF0_EN__ENABLE | DAC0CF0_UPDATE__TIMER4
      | DAC0CF0_LJST__RIGHT_JUSTIFY | DAC0CF0_RSTMD__NORMAL;
  // [DAC0CF0 - DAC0 Configuration 0]$

  // $[DAC0CF1 - DAC0 Configuration 1]
  DAC0CF1 = DAC0CF1_SRC__DACALT | DAC0CF1_AMEN__FALSE | DAC0CF1_VREFSEL__VDD
      | DAC0CF1_GAINADJ__FALSE;
  // [DAC0CF1 - DAC0 Configuration 1]$

}

//================================================================================
// CLOCK_0_enter_DefaultMode_from_RESET
//================================================================================
extern void CLOCK_0_enter_DefaultMode_from_RESET(void) {
    // $[HFOSC1 Setup]
    // [HFOSC1 Setup]$

    // $[CLKSEL - Clock Select]
    /***********************************************************************
     - Clock derived from the Internal High Frequency Oscillator 0
     - SYSCLK is equal to selected clock source divided by 1
     ***********************************************************************/
    CLKSEL = CLKSEL_CLKSL__HFOSC0_clk24p5 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
    while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY);
    // [CLKSEL - Clock Select]$

    // $[CLKGRP0 - Clock Group 0]
    // [CLKGRP0 - Clock Group 0]$
}

//================================================================================
// TIMER16_4_enter_DefaultMode_from_RESET
//================================================================================
extern void
TIMER16_4_enter_DefaultMode_from_RESET (void)
{
  // $[Timer Initialization]
  // Save Timer Configuration
  uint8_t TMR4CN0_TR4_save;
  SFRPAGE = 0x10;
  TMR4CN0_TR4_save = TMR4CN0 & TMR4CN0_TR4__BMASK;
  // Stop Timer
  TMR4CN0 &= ~(TMR4CN0_TR4__BMASK);
  // [Timer Initialization]$

  // $[TMR4CN1 - Timer 4 Control 1]
  // [TMR4CN1 - Timer 4 Control 1]$

  // $[TMR4CN0 - Timer 4 Control]
  // [TMR4CN0 - Timer 4 Control]$

  // $[TMR4H - Timer 4 High Byte]
  /***********************************************************************
   - Timer 4 High Byte = 0xFF
   ***********************************************************************/
  TMR4H = (0xFF << TMR4H_TMR4H__SHIFT);
  // [TMR4H - Timer 4 High Byte]$

  // $[TMR4L - Timer 4 Low Byte]
  /***********************************************************************
   - Timer 4 Low Byte = 0x41
   ***********************************************************************/
  TMR4L = (0x41 << TMR4L_TMR4L__SHIFT);
  // [TMR4L - Timer 4 Low Byte]$

  // $[TMR4RLH - Timer 4 Reload High Byte]
  /***********************************************************************
   - Timer 4 Reload High Byte = 0xFF
   ***********************************************************************/
  TMR4RLH = (0xFF << TMR4RLH_TMR4RLH__SHIFT);
  // [TMR4RLH - Timer 4 Reload High Byte]$

  // $[TMR4RLL - Timer 4 Reload Low Byte]
  /***********************************************************************
   - Timer 4 Reload Low Byte = 0x41
   ***********************************************************************/
  TMR4RLL = (0x41 << TMR4RLL_TMR4RLL__SHIFT);
  // [TMR4RLL - Timer 4 Reload Low Byte]$

  // $[TMR4CN0]
  /***********************************************************************
   - Start Timer 4 running
   ***********************************************************************/
  TMR4CN0 |= TMR4CN0_TR4__RUN;
  // [TMR4CN0]$

  // $[Timer Restoration]
  // Restore Timer Configuration
  TMR4CN0 |= TMR4CN0_TR4_save;
  // [Timer Restoration]$

}

//================================================================================
// TIMER_SETUP_0_enter_DefaultMode_from_RESET
//================================================================================
extern void
TIMER_SETUP_0_enter_DefaultMode_from_RESET (void)
{
  // $[CKCON0 - Clock Control 0]
  // [CKCON0 - Clock Control 0]$

  // $[CKCON1 - Clock Control 1]
  /***********************************************************************
   - Timer 4 high byte uses the clock defined by T4XCLK in TMR4CN0
   - Timer 4 low byte uses the system clock
   - Timer 5 high byte uses the clock defined by T5XCLK in TMR5CN0
   - Timer 5 low byte uses the clock defined by T5XCLK in TMR5CN0
   ***********************************************************************/
  CKCON1 = CKCON1_T4MH__EXTERNAL_CLOCK | CKCON1_T4ML__SYSCLK
      | CKCON1_T5MH__EXTERNAL_CLOCK | CKCON1_T5ML__EXTERNAL_CLOCK;
  // [CKCON1 - Clock Control 1]$

  // $[TMOD - Timer 0/1 Mode]
  // [TMOD - Timer 0/1 Mode]$

  // $[TCON - Timer 0/1 Control]
  // [TCON - Timer 0/1 Control]$

}

//================================================================================
// INTERRUPT_0_enter_DefaultMode_from_RESET
//================================================================================
extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void) {
    // $[EIE1 - Extended Interrupt Enable 1]
    /***********************************************************************
     - Disable interrupt requests generated by the ADINT flag
     - Disable ADC0 Window Comparison interrupt
     - Disable CP0 interrupts
     - Disable CP1 interrupts
     - Disable all Port Match interrupts
     - Disable all PCA0 interrupts
     - Disable all SMB0 interrupts
     - Disable Timer 3 interrupts
     ***********************************************************************/
    SFRPAGE = 0x00;
    EIE1 = EIE1_EADC0__DISABLED | EIE1_EWADC0__DISABLED | EIE1_ECP0__DISABLED
            | EIE1_ECP1__DISABLED | EIE1_EMAT__DISABLED | EIE1_EPCA0__DISABLED
            | EIE1_ESMB0__DISABLED | EIE1_ET3__DISABLED;
    // [EIE1 - Extended Interrupt Enable 1]$

    // $[EIE2 - Extended Interrupt Enable 2]
    EIE2 = EIE2_EPWM0__DISABLED | EIE2_CL0__DISABLED | EIE2_ET5__DISABLED
            | EIE2_ET4__ENABLED | EIE2_ESMB1__DISABLED | EIE2_ES1__DISABLED;
    // [EIE2 - Extended Interrupt Enable 2]$

    // $[EIP1H - Extended Interrupt Priority 1 High]
    // [EIP1H - Extended Interrupt Priority 1 High]$

    // $[EIP1 - Extended Interrupt Priority 1 Low]
    // [EIP1 - Extended Interrupt Priority 1 Low]$

    // $[EIP2 - Extended Interrupt Priority 2]
    // [EIP2 - Extended Interrupt Priority 2]$

    // $[EIP2H - Extended Interrupt Priority 2 High]
    // [EIP2H - Extended Interrupt Priority 2 High]$

    // $[IE - Interrupt Enable]
    /***********************************************************************
     - Enable each interrupt according to its individual mask setting
     - Disable external interrupt 0
     - Disable external interrupt 1
     - Disable all SPI0 interrupts
     - Disable all Timer 0 interrupt
     - Disable all Timer 1 interrupt
     - Disable Timer 2 interrupt
     - Disable UART0 interrupt
     ***********************************************************************/
    IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED
            | IE_ESPI0__DISABLED | IE_ET0__DISABLED | IE_ET1__DISABLED
            | IE_ET2__DISABLED | IE_ES0__DISABLED;
    // [IE - Interrupt Enable]$

    // $[IP - Interrupt Priority]
    // [IP - Interrupt Priority]$

    // $[IPH - Interrupt Priority High]
    // [IPH - Interrupt Priority High]$

}
