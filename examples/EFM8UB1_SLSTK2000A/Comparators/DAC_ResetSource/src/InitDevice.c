//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8UB1_Register_Enums.h>
#include "InitDevice.h"

// USER PROTOTYPES
// USER FUNCTIONS

// $[Library Includes]
// [Library Includes]$

//==============================================================================
// enter_BlinkMode_from_RESET
//==============================================================================
extern void enter_BlinkMode_from_RESET(void)
{
  // $[Config Calls]
  // Save the SFRPAGE
  uint8_t SFRPAGE_save = SFRPAGE;
  WDT_0_enter_BlinkMode_from_RESET();
  PORTS_1_enter_BlinkMode_from_RESET();
  PORTS_2_enter_BlinkMode_from_RESET();
  PBCFG_0_enter_BlinkMode_from_RESET();
  CLOCK_0_enter_BlinkMode_from_RESET();
  TIMER01_0_enter_BlinkMode_from_RESET();
  TIMER_SETUP_0_enter_BlinkMode_from_RESET();
  // Restore the SFRPAGE
  SFRPAGE = SFRPAGE_save;
  // [Config Calls]$

}

//================================================================================
// WDT_0_enter_BlinkMode_from_RESET
//================================================================================
extern void WDT_0_enter_BlinkMode_from_RESET(void)
{
  // $[WDTCN - Watchdog Timer Control]
  SFRPAGE = 0x00;
  //Disable Watchdog with key sequence
  WDTCN = 0xDE; //First key
  WDTCN = 0xAD; //Second key
  // [WDTCN - Watchdog Timer Control]$

}

//================================================================================
// PORTS_1_enter_BlinkMode_from_RESET
//================================================================================
extern void PORTS_1_enter_BlinkMode_from_RESET(void)
{
  // $[P1 - Port 1 Pin Latch]
  // [P1 - Port 1 Pin Latch]$

  // $[P1MDOUT - Port 1 Output Mode]
  /***********************************************************************
   - P1.0 output is open-drain
   - P1.1 output is open-drain
   - P1.2 output is open-drain
   - P1.3 output is open-drain
   - P1.4 output is push-pull
   - P1.5 output is open-drain
   - P1.6 output is open-drain
   - P1.7 output is open-drain
   ***********************************************************************/
  P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
      | P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__PUSH_PULL
      | P1MDOUT_B5__OPEN_DRAIN | P1MDOUT_B6__OPEN_DRAIN
      | P1MDOUT_B7__OPEN_DRAIN;
  // [P1MDOUT - Port 1 Output Mode]$

  // $[P1MDIN - Port 1 Input Mode]
  // [P1MDIN - Port 1 Input Mode]$

  // $[P1SKIP - Port 1 Skip]
  /***********************************************************************
   - P1.0 pin is not skipped by the crossbar
   - P1.1 pin is not skipped by the crossbar
   - P1.2 pin is not skipped by the crossbar
   - P1.3 pin is not skipped by the crossbar
   - P1.4 pin is skipped by the crossbar
   - P1.5 pin is not skipped by the crossbar
   - P1.6 pin is not skipped by the crossbar
   - P1.7 pin is not skipped by the crossbar
   ***********************************************************************/
  P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED
      | P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__SKIPPED
      | P1SKIP_B5__NOT_SKIPPED | P1SKIP_B6__NOT_SKIPPED
      | P1SKIP_B7__NOT_SKIPPED;
  // [P1SKIP - Port 1 Skip]$

  // $[P1MASK - Port 1 Mask]
  // [P1MASK - Port 1 Mask]$

  // $[P1MAT - Port 1 Match]
  // [P1MAT - Port 1 Match]$

}

//================================================================================
// PORTS_2_enter_BlinkMode_from_RESET
//================================================================================
extern void PORTS_2_enter_BlinkMode_from_RESET(void)
{
  // $[P2 - Port 2 Pin Latch]
  // [P2 - Port 2 Pin Latch]$

  // $[P2MDOUT - Port 2 Output Mode]
  /***********************************************************************
   - P2.0 output is open-drain
   - P2.1 output is open-drain
   - P2.2 output is open-drain
   - P2.3 output is push-pull
   ***********************************************************************/
  P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN
      | P2MDOUT_B2__OPEN_DRAIN | P2MDOUT_B3__PUSH_PULL;
  // [P2MDOUT - Port 2 Output Mode]$

  // $[P2MDIN - Port 2 Input Mode]
  // [P2MDIN - Port 2 Input Mode]$

  // $[P2SKIP - Port 2 Skip]
  /***********************************************************************
   - P2.0 pin is not skipped by the crossbar
   - P2.1 pin is not skipped by the crossbar
   - P2.2 pin is not skipped by the crossbar
   - P2.3 pin is skipped by the crossbar
   ***********************************************************************/
  SFRPAGE = 0x20;
  P2SKIP = P2SKIP_B0__NOT_SKIPPED | P2SKIP_B1__NOT_SKIPPED
      | P2SKIP_B2__NOT_SKIPPED | P2SKIP_B3__SKIPPED;
  // [P2SKIP - Port 2 Skip]$

  // $[P2MASK - Port 2 Mask]
  // [P2MASK - Port 2 Mask]$

  // $[P2MAT - Port 2 Match]
  // [P2MAT - Port 2 Match]$

}

//================================================================================
// PBCFG_0_enter_BlinkMode_from_RESET
//================================================================================
extern void PBCFG_0_enter_BlinkMode_from_RESET(void)
{
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
// CLOCK_0_enter_BlinkMode_from_RESET
//================================================================================
extern void CLOCK_0_enter_BlinkMode_from_RESET(void)
{
  // $[HFOSC1 Setup]
  // [HFOSC1 Setup]$

  // $[CLKSEL - Clock Select]
  /***********************************************************************
   - Clock derived from the Internal High Frequency Oscillator 0
   - SYSCLK is equal to selected clock source divided by 1
   ***********************************************************************/
  CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
    ;
  // [CLKSEL - Clock Select]$

}

//================================================================================
// CIP51_0_enter_BlinkMode_from_RESET
//================================================================================
extern void CIP51_0_enter_BlinkMode_from_RESET(void)
{
  // $[PFE0CN - Prefetch Engine Control]
  // [PFE0CN - Prefetch Engine Control]$

}

//================================================================================
// TIMER01_0_enter_BlinkMode_from_RESET
//================================================================================
extern void TIMER01_0_enter_BlinkMode_from_RESET(void)
{
  // $[Timer Initialization]
  //Save Timer Configuration
  uint8_t TCON_save;
  TCON_save = TCON;
  //Stop Timers
  TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;

  // [Timer Initialization]$

  // $[TH0 - Timer 0 High Byte]
  /***********************************************************************
   - Timer 0 High Byte = 0x8A
   ***********************************************************************/
  TH0 = (0x8A << TH0_TH0__SHIFT);
  // [TH0 - Timer 0 High Byte]$

  // $[TL0 - Timer 0 Low Byte]
  /***********************************************************************
   - Timer 0 Low Byte = 0xCF
   ***********************************************************************/
  TL0 = (0xCF << TL0_TL0__SHIFT);
  // [TL0 - Timer 0 Low Byte]$

  // $[TH1 - Timer 1 High Byte]
  // [TH1 - Timer 1 High Byte]$

  // $[TL1 - Timer 1 Low Byte]
  // [TL1 - Timer 1 Low Byte]$

  // $[Timer Restoration]
  //Restore Timer Configuration
  TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

  // [Timer Restoration]$

}

//================================================================================
// TIMER_SETUP_0_enter_BlinkMode_from_RESET
//================================================================================
extern void TIMER_SETUP_0_enter_BlinkMode_from_RESET(void)
{
  // $[CKCON0 - Clock Control 0]
  /***********************************************************************
   - System clock divided by 12
   - Counter/Timer 0 uses the system clock
   - Timer 2 high byte uses the clock defined by T2XCLK in TMR2CN0
   - Timer 2 low byte uses the clock defined by T2XCLK in TMR2CN0
   - Timer 3 high byte uses the clock defined by T3XCLK in TMR3CN0
   - Timer 3 low byte uses the clock defined by T3XCLK in TMR3CN0
   - Timer 1 uses the clock defined by the prescale field, SCA
   ***********************************************************************/
  CKCON0 = CKCON0_SCA__SYSCLK_DIV_12 | CKCON0_T0M__SYSCLK
      | CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__EXTERNAL_CLOCK
      | CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
      | CKCON0_T1M__PRESCALE;
  // [CKCON0 - Clock Control 0]$

  // $[CKCON1 - Clock Control 1]
  // [CKCON1 - Clock Control 1]$

  // $[TMOD - Timer 0/1 Mode]
  /***********************************************************************
   - Mode 1, 16-bit Counter/Timer
   - Mode 0, 13-bit Counter/Timer
   - Timer Mode
   - Timer 0 enabled when TR0 = 1 irrespective of INT0 logic level
   - Timer Mode
   - Timer 1 enabled when TR1 = 1 irrespective of INT1 logic level
   ***********************************************************************/
  TMOD = TMOD_T0M__MODE1 | TMOD_T1M__MODE0 | TMOD_CT0__TIMER
      | TMOD_GATE0__DISABLED | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
  // [TMOD - Timer 0/1 Mode]$

  // $[TCON - Timer 0/1 Control]
  // [TCON - Timer 0/1 Control]$

}

//==============================================================================
// enter_CompareMode_from_RESET
//==============================================================================
extern void enter_CompareMode_from_RESET(void)
{
  // $[Config Calls]
  // Save the SFRPAGE
  uint8_t SFRPAGE_save = SFRPAGE;
  WDT_0_enter_CompareMode_from_RESET();
  PORTS_0_enter_CompareMode_from_RESET();
  PORTS_1_enter_CompareMode_from_RESET();
  PORTS_2_enter_CompareMode_from_RESET();
  PBCFG_0_enter_CompareMode_from_RESET();
  CMP_0_enter_CompareMode_from_RESET();
  RSTSRC_0_enter_CompareMode_from_RESET();
  CLOCK_0_enter_CompareMode_from_RESET();
  // Restore the SFRPAGE
  SFRPAGE = SFRPAGE_save;
  // [Config Calls]$

}

//================================================================================
// WDT_0_enter_CompareMode_from_RESET
//================================================================================
extern void WDT_0_enter_CompareMode_from_RESET(void)
{
  // $[WDTCN - Watchdog Timer Control]
  SFRPAGE = 0x00;
  //Disable Watchdog with key sequence
  WDTCN = 0xDE; //First key
  WDTCN = 0xAD; //Second key
  // [WDTCN - Watchdog Timer Control]$

}

//================================================================================
// PORTS_0_enter_CompareMode_from_RESET
//================================================================================
extern void PORTS_0_enter_CompareMode_from_RESET(void)
{
  // $[P0 - Port 0 Pin Latch]
  // [P0 - Port 0 Pin Latch]$

  // $[P0MDOUT - Port 0 Output Mode]
  // [P0MDOUT - Port 0 Output Mode]$

  // $[P0MDIN - Port 0 Input Mode]
  /***********************************************************************
   - P0.0 pin is configured for digital mode
   - P0.1 pin is configured for digital mode
   - P0.2 pin is configured for digital mode
   - P0.3 pin is configured for digital mode
   - P0.4 pin is configured for digital mode
   - P0.5 pin is configured for digital mode
   - P0.6 pin is configured for digital mode
   - P0.7 pin is configured for analog mode
   ***********************************************************************/
  P0MDIN = P0MDIN_B0__DIGITAL | P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL
      | P0MDIN_B3__DIGITAL | P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL
      | P0MDIN_B6__DIGITAL | P0MDIN_B7__ANALOG;
  // [P0MDIN - Port 0 Input Mode]$

  // $[P0SKIP - Port 0 Skip]
  /***********************************************************************
   - P0.0 pin is not skipped by the crossbar
   - P0.1 pin is not skipped by the crossbar
   - P0.2 pin is not skipped by the crossbar
   - P0.3 pin is not skipped by the crossbar
   - P0.4 pin is not skipped by the crossbar
   - P0.5 pin is not skipped by the crossbar
   - P0.6 pin is not skipped by the crossbar
   - P0.7 pin is skipped by the crossbar
   ***********************************************************************/
  P0SKIP = P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED
      | P0SKIP_B2__NOT_SKIPPED | P0SKIP_B3__NOT_SKIPPED | P0SKIP_B4__NOT_SKIPPED
      | P0SKIP_B5__NOT_SKIPPED | P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__SKIPPED;
  // [P0SKIP - Port 0 Skip]$

  // $[P0MASK - Port 0 Mask]
  // [P0MASK - Port 0 Mask]$

  // $[P0MAT - Port 0 Match]
  // [P0MAT - Port 0 Match]$

}

//================================================================================
// PORTS_1_enter_CompareMode_from_RESET
//================================================================================
extern void PORTS_1_enter_CompareMode_from_RESET(void)
{
  // $[P1 - Port 1 Pin Latch]
  // [P1 - Port 1 Pin Latch]$

  // $[P1MDOUT - Port 1 Output Mode]
  /***********************************************************************
   - P1.0 output is open-drain
   - P1.1 output is open-drain
   - P1.2 output is open-drain
   - P1.3 output is open-drain
   - P1.4 output is push-pull
   - P1.5 output is open-drain
   - P1.6 output is open-drain
   - P1.7 output is open-drain
   ***********************************************************************/
  P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
      | P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN | P1MDOUT_B4__PUSH_PULL
      | P1MDOUT_B5__OPEN_DRAIN | P1MDOUT_B6__OPEN_DRAIN
      | P1MDOUT_B7__OPEN_DRAIN;
  // [P1MDOUT - Port 1 Output Mode]$

  // $[P1MDIN - Port 1 Input Mode]
  // [P1MDIN - Port 1 Input Mode]$

  // $[P1SKIP - Port 1 Skip]
  /***********************************************************************
   - P1.0 pin is not skipped by the crossbar
   - P1.1 pin is not skipped by the crossbar
   - P1.2 pin is not skipped by the crossbar
   - P1.3 pin is not skipped by the crossbar
   - P1.4 pin is skipped by the crossbar
   - P1.5 pin is not skipped by the crossbar
   - P1.6 pin is not skipped by the crossbar
   - P1.7 pin is not skipped by the crossbar
   ***********************************************************************/
  P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__NOT_SKIPPED
      | P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__SKIPPED
      | P1SKIP_B5__NOT_SKIPPED | P1SKIP_B6__NOT_SKIPPED
      | P1SKIP_B7__NOT_SKIPPED;
  // [P1SKIP - Port 1 Skip]$

  // $[P1MASK - Port 1 Mask]
  // [P1MASK - Port 1 Mask]$

  // $[P1MAT - Port 1 Match]
  // [P1MAT - Port 1 Match]$

}

//================================================================================
// PORTS_2_enter_CompareMode_from_RESET
//================================================================================
extern void PORTS_2_enter_CompareMode_from_RESET(void)
{
  // $[P2 - Port 2 Pin Latch]
  // [P2 - Port 2 Pin Latch]$

  // $[P2MDOUT - Port 2 Output Mode]
  /***********************************************************************
   - P2.0 output is open-drain
   - P2.1 output is open-drain
   - P2.2 output is open-drain
   - P2.3 output is push-pull
   ***********************************************************************/
  P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN
      | P2MDOUT_B2__OPEN_DRAIN | P2MDOUT_B3__PUSH_PULL;
  // [P2MDOUT - Port 2 Output Mode]$

  // $[P2MDIN - Port 2 Input Mode]
  // [P2MDIN - Port 2 Input Mode]$

  // $[P2SKIP - Port 2 Skip]
  /***********************************************************************
   - P2.0 pin is not skipped by the crossbar
   - P2.1 pin is not skipped by the crossbar
   - P2.2 pin is not skipped by the crossbar
   - P2.3 pin is skipped by the crossbar
   ***********************************************************************/
  SFRPAGE = 0x20;
  P2SKIP = P2SKIP_B0__NOT_SKIPPED | P2SKIP_B1__NOT_SKIPPED
      | P2SKIP_B2__NOT_SKIPPED | P2SKIP_B3__SKIPPED;
  // [P2SKIP - Port 2 Skip]$

  // $[P2MASK - Port 2 Mask]
  // [P2MASK - Port 2 Mask]$

  // $[P2MAT - Port 2 Match]
  // [P2MAT - Port 2 Match]$

}

//================================================================================
// PBCFG_0_enter_CompareMode_from_RESET
//================================================================================
extern void PBCFG_0_enter_CompareMode_from_RESET(void)
{
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
// CMP_0_enter_CompareMode_from_RESET
//================================================================================
extern void CMP_0_enter_CompareMode_from_RESET(void)
{
  // $[CMP0MX - Comparator 0 Multiplexer Selection]
  /***********************************************************************
   - External pin CMP0P.2
   - External pin CMP0N.15
   ***********************************************************************/
  uint8_t delay;

  CMP0MX = CMP0MX_CMXP__CMP0P2 | CMP0MX_CMXN__CMP0N15;
  // Delay to allow the comparator inputs to settle
  for (delay = 0; delay < 0x20; delay++)
    ;
  // [CMP0MX - Comparator 0 Multiplexer Selection]$

  // $[CMP0CN1 - Comparator 0 Control 1]
  /***********************************************************************
   - Internal Comparator DAC Reference Level = 0x20
   - The comparator output will always reflect the input conditions
   ***********************************************************************/
  SFRPAGE = 0x10;
  CMP0CN1 = (0x20 << CMP0CN1_DACLVL__SHIFT) | CMP0CN1_CPINH__DISABLED;
  // [CMP0CN1 - Comparator 0 Control 1]$

  // $[CMP0MD - Comparator 0 Mode]
  /***********************************************************************
   - Connect the CMP- input to the internal DAC output, and CMP+ is
   selected by CMXP
   ***********************************************************************/
  SFRPAGE = 0x00;
  CMP0MD |= CMP0MD_INSL__CMXP_DAC;
  // [CMP0MD - Comparator 0 Mode]$

  // $[CMP0CN0 - Comparator 0 Control 0]
  /***********************************************************************
   - Comparator enabled
   ***********************************************************************/
  CMP0CN0 |= CMP0CN0_CPEN__ENABLED;
  // [CMP0CN0 - Comparator 0 Control 0]$

}

//================================================================================
// RSTSRC_0_enter_CompareMode_from_RESET
//================================================================================
extern void RSTSRC_0_enter_CompareMode_from_RESET(void)
{
  // $[RSTSRC - Reset Source]
  /***********************************************************************
   - A power-on or supply monitor reset occurred
   - A missing clock detector reset did not occur
   - A Comparator 0 reset occurred
   - A USB0 reset did not occur
   ***********************************************************************/
  RSTSRC = RSTSRC_PORSF__SET | RSTSRC_MCDRSF__NOT_SET | RSTSRC_C0RSEF__SET
      | RSTSRC_USBRSF__NOT_SET;
  // [RSTSRC - Reset Source]$

}

//================================================================================
// CLOCK_0_enter_CompareMode_from_RESET
//================================================================================
extern void CLOCK_0_enter_CompareMode_from_RESET(void)
{
  // $[HFOSC1 Setup]
  // [HFOSC1 Setup]$

  // $[CLKSEL - Clock Select]
  /***********************************************************************
   - Clock derived from the Internal High Frequency Oscillator 0
   - SYSCLK is equal to selected clock source divided by 1
   ***********************************************************************/
  CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
    ;
  // [CLKSEL - Clock Select]$

}

