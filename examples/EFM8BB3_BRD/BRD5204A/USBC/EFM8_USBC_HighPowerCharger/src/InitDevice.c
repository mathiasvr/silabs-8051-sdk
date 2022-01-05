/**************************************************************************//**
 * Copyright 2017 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!!
//=========================================================
// USER INCLUDES
#include <SI_EFM8BB3_Register_Enums.h>
#include "InitDevice.h"

// USER PROTOTYPES
// USER FUNCTIONS

// $[Library Includes]
// [Library Includes]$

//==============================================================================
// enter_DefaultMode_from_RESET
//==============================================================================
extern void enter_DefaultMode_from_RESET(void)
{
  // $[Config Calls]
  // Save the SFRPAGE
  uint8_t SFRPAGE_save = SFRPAGE;
  WDT_0_enter_DefaultMode_from_RESET();
  CIP51_0_enter_DefaultMode_from_RESET();
  CLOCK_0_enter_DefaultMode_from_RESET();
  TIMER01_0_enter_DefaultMode_from_RESET();
  TIMER_SETUP_0_enter_DefaultMode_from_RESET();
  SMBUS_0_enter_DefaultMode_from_RESET();
  UARTE_1_enter_DefaultMode_from_RESET();
  INTERRUPT_0_enter_DefaultMode_from_RESET();
  // Restore the SFRPAGE
  SFRPAGE = SFRPAGE_save;
  // [Config Calls]$

}

extern void WDT_0_enter_DefaultMode_from_RESET(void)
{
  // $[WDTCN - Watchdog Timer Control]
  SFRPAGE = 0x00;
  //Disable Watchdog with key sequence
  WDTCN = 0xDE; //First key
  WDTCN = 0xAD; //Second key
  // [WDTCN - Watchdog Timer Control]$

}

extern void CIP51_0_enter_DefaultMode_from_RESET(void)
{
  // $[PFE0CN - Prefetch Engine Control]
  /***********************************************************************
   - SYSCLK < 50 MHz
   ***********************************************************************/
  SFRPAGE = 0x10;
  PFE0CN = PFE0CN_FLRT__SYSCLK_BELOW_50_MHZ;
  // [PFE0CN - Prefetch Engine Control]$

}

extern void CLOCK_0_enter_DefaultMode_from_RESET(void)
{
  // $[HFOSC1 Setup]
  // Ensure SYSCLK is > 24 MHz before switching to HFOSC1
  SFRPAGE = 0x00;
  CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
    ;
  // [HFOSC1 Setup]$

  // $[CLKSEL - Clock Select]
  /***********************************************************************
   - Clock derived from the Internal High Frequency Oscillator 1
   - SYSCLK is equal to selected clock source divided by 1
   ***********************************************************************/
  CLKSEL = CLKSEL_CLKSL__HFOSC1 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
    ;
  // [CLKSEL - Clock Select]$

}

extern void UARTE_1_enter_DefaultMode_from_RESET(void)
{
  // $[SBCON1 - UART1 Baud Rate Generator Control]
  /***********************************************************************
   - Enable the baud rate generator
   - Prescaler = 1
   ***********************************************************************/
  SFRPAGE = 0x20;
  SBCON1 = SBCON1_BREN__ENABLED | SBCON1_BPS__DIV_BY_1;
  // [SBCON1 - UART1 Baud Rate Generator Control]$

  // $[SMOD1 - UART1 Mode]
  // [SMOD1 - UART1 Mode]$

  // $[UART1FCN0 - UART1 FIFO Control 0]
  // [UART1FCN0 - UART1 FIFO Control 0]$

  // $[SBRLH1 - UART1 Baud Rate Generator High Byte]
  /***********************************************************************
   - UART1 Baud Rate Reload High = 0xFF
   ***********************************************************************/
  SBRLH1 = (0xFF << SBRLH1_BRH__SHIFT);
  // [SBRLH1 - UART1 Baud Rate Generator High Byte]$

  // $[SBRLL1 - UART1 Baud Rate Generator Low Byte]
  /***********************************************************************
   - UART1 Baud Rate Reload Low = 0xE6
   ***********************************************************************/
  SBRLL1 = (0xE6 << SBRLL1_BRL__SHIFT);
  // [SBRLL1 - UART1 Baud Rate Generator Low Byte]$

  // $[UART1LIN - UART1 LIN Configuration]
  // [UART1LIN - UART1 LIN Configuration]$

  // $[SCON1 - UART1 Serial Port Control]
  // [SCON1 - UART1 Serial Port Control]$

  // $[UART1FCN1 - UART1 FIFO Control 1]
  // [UART1FCN1 - UART1 FIFO Control 1]$

}

extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void)
{
  // $[EIE1 - Extended Interrupt Enable 1]
  /***********************************************************************
   - Disable ADC0 Conversion Complete interrupt
   - Disable ADC0 Window Comparison interrupt
   - Disable CP0 interrupts
   - Disable CP1 interrupts
   - Disable all Port Match interrupts
   - Disable all PCA0 interrupts
   - Enable interrupt requests generated by SMB0
   - Disable Timer 3 interrupts
   ***********************************************************************/
  SFRPAGE = 0x00;
  EIE1 = EIE1_EADC0__DISABLED | EIE1_EWADC0__DISABLED | EIE1_ECP0__DISABLED
      | EIE1_ECP1__DISABLED | EIE1_EMAT__DISABLED | EIE1_EPCA0__DISABLED
      | EIE1_ESMB0__ENABLED | EIE1_ET3__DISABLED;
  // [EIE1 - Extended Interrupt Enable 1]$

  // $[EIE2 - Extended Interrupt Enable 2]
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
  IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__DISABLED
      | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__DISABLED
      | IE_ES0__DISABLED;
  // [IE - Interrupt Enable]$

  // $[IP - Interrupt Priority]
  // [IP - Interrupt Priority]$

  // $[IPH - Interrupt Priority High]
  // [IPH - Interrupt Priority High]$

}

extern void SMBUS_0_enter_DefaultMode_from_RESET(void)
{
  // $[SMB0FCN0 - SMBus0 FIFO Control 0]
  // [SMB0FCN0 - SMBus0 FIFO Control 0]$

  // $[SMB0RXLN - SMBus0 Receive Length Counter]
  // [SMB0RXLN - SMBus0 Receive Length Counter]$

  // $[SMB0ADR - SMBus 0 Slave Address]
  // [SMB0ADR - SMBus 0 Slave Address]$

  // $[SMB0ADM - SMBus 0 Slave Address Mask]
  /***********************************************************************
   - Automatic slave address recognition and hardware acknowledge is
   enabled
   - SMBus Slave Address Mask = 0x7F
   ***********************************************************************/
  SMB0ADM = SMB0ADM_EHACK__ADR_ACK_AUTOMATIC | (0x7F << SMB0ADM_SLVM__SHIFT);
  // [SMB0ADM - SMBus 0 Slave Address Mask]$

  // $[SMB0TC - SMBus 0 Timing and Pin Control]
  // [SMB0TC - SMBus 0 Timing and Pin Control]$

  // $[SMB0CF - SMBus 0 Configuration]
  /***********************************************************************
   - Timer 1 Overflow
   - Enable the SMBus module
   ***********************************************************************/
  SMB0CF &= ~SMB0CF_SMBCS__FMASK;
  SMB0CF |= SMB0CF_SMBCS__TIMER1 | SMB0CF_ENSMB__ENABLED;
  // [SMB0CF - SMBus 0 Configuration]$

}

extern void TIMER01_0_enter_DefaultMode_from_RESET(void)
{
  // $[Timer Initialization]
  //Save Timer Configuration
  uint8_t TCON_save;
  TCON_save = TCON;
  //Stop Timers
  TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;

  // [Timer Initialization]$

  // $[TH0 - Timer 0 High Byte]
  // [TH0 - Timer 0 High Byte]$

  // $[TL0 - Timer 0 Low Byte]
  // [TL0 - Timer 0 Low Byte]$

  // $[TH1 - Timer 1 High Byte]
  /***********************************************************************
   - Timer 1 High Byte = 0xD7
   ***********************************************************************/
  TH1 = (0xD7 << TH1_TH1__SHIFT);
  // [TH1 - Timer 1 High Byte]$

  // $[TL1 - Timer 1 Low Byte]
  // [TL1 - Timer 1 Low Byte]$

  // $[Timer Restoration]
  //Restore Timer Configuration
  TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

  // [Timer Restoration]$

}

extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void)
{
  // $[CKCON0 - Clock Control 0]
  /***********************************************************************
   - System clock divided by 4
   - Counter/Timer 0 uses the clock defined by the prescale field, SCA
   - Timer 2 high byte uses the clock defined by T2XCLK in TMR2CN0
   - Timer 2 low byte uses the clock defined by T2XCLK in TMR2CN0
   - Timer 3 high byte uses the clock defined by T3XCLK in TMR3CN0
   - Timer 3 low byte uses the clock defined by T3XCLK in TMR3CN0
   - Timer 1 uses the clock defined by the prescale field, SCA
   ***********************************************************************/
  CKCON0 = CKCON0_SCA__SYSCLK_DIV_4 | CKCON0_T0M__PRESCALE
      | CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__EXTERNAL_CLOCK
      | CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
      | CKCON0_T1M__PRESCALE;
  // [CKCON0 - Clock Control 0]$

  // $[CKCON1 - Clock Control 1]
  // [CKCON1 - Clock Control 1]$

  // $[TMOD - Timer 0/1 Mode]
  /***********************************************************************
   - Mode 1, 16-bit Counter/Timer
   - Mode 2, 8-bit Counter/Timer with Auto-Reload
   - Timer Mode
   - Timer 0 enabled when TR0 = 1 irrespective of INT0 logic level
   - Timer Mode
   - Timer 1 enabled when TR1 = 1 irrespective of INT1 logic level
   ***********************************************************************/
  TMOD = TMOD_T0M__MODE1 | TMOD_T1M__MODE2 | TMOD_CT0__TIMER
      | TMOD_GATE0__DISABLED | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
  // [TMOD - Timer 0/1 Mode]$

  // $[TCON - Timer 0/1 Control]
  /***********************************************************************
   - Start Timer 1 running
   ***********************************************************************/
  TCON |= TCON_TR1__RUN;
  // [TCON - Timer 0/1 Control]$

}
