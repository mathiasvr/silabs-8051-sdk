//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!!
//=========================================================

// USER INCLUDES
#include <SI_C8051F970_Register_Enums.h>
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
  PCA_0_enter_DefaultMode_from_RESET();
  PORTS_0_enter_DefaultMode_from_RESET();
  PORTS_1_enter_DefaultMode_from_RESET();
  PBCFG_0_enter_DefaultMode_from_RESET();
  VREF_0_enter_DefaultMode_from_RESET();
  TIMER01_0_enter_DefaultMode_from_RESET();
  TIMER_SETUP_0_enter_DefaultMode_from_RESET();
  INTERRUPT_0_enter_DefaultMode_from_RESET();
  // Restore the SFRPAGE
  SFRPAGE = SFRPAGE_save;
  // [Config Calls]$

}

extern void PCA_0_enter_DefaultMode_from_RESET(void)
{
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

extern void VREG_0_enter_DefaultMode_from_RESET(void)
{
  // $[REG0CN - Voltage Regulator Control]
  /***********************************************************************
   - Enable the precision High Frequency Oscillator bias
   ***********************************************************************/
  REG0CN = REG0CN_OSCBIAS__ENABLED;
  // [REG0CN - Voltage Regulator Control]$

}

extern void PBCFG_0_enter_DefaultMode_from_RESET(void)
{
  // $[XBR1 - Port I/O Crossbar 1]
  /***********************************************************************
   - Weak Pullups enabled 
   - Crossbar enabled
   - T0 unavailable at Port pin
   - T1 unavailable at Port pin
   ***********************************************************************/
  XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED
      | XBR1_T0E__DISABLED | XBR1_T1E__DISABLED;
  // [XBR1 - Port I/O Crossbar 1]$

  // $[XBR0 - Port I/O Crossbar 0]
  // [XBR0 - Port I/O Crossbar 0]$

}

extern void VREF_0_enter_DefaultMode_from_RESET(void)
{
  // $[REF0CN - Voltage Reference Control]
  /***********************************************************************
   - Disable the Temperature Sensor
   - The ADC0 voltage reference is the internal 1.65 V high speed voltage
   reference
   ***********************************************************************/
  SFRPAGE = 0x00;
  REF0CN = REF0CN_TEMPE__TEMP_DISABLED | REF0CN_REFSL__HIGH_SPEED_VREF;
  // [REF0CN - Voltage Reference Control]$

}

extern void HFOSC_0_enter_DefaultMode_from_RESET(void)
{
  // $[OSCICL - High Frequency Oscillator Calibration]
  // [OSCICL - High Frequency Oscillator Calibration]$

  // $[OSCICN - High Frequency Oscillator Control]
  /***********************************************************************
   - High Frequency Oscillator enabled
   ***********************************************************************/
  OSCICN |= OSCICN_IOSCEN__ENABLED;
  // [OSCICN - High Frequency Oscillator Control]$

}

extern void CLOCK_0_enter_DefaultMode_from_RESET(void)
{
  // $[CLKSEL - Clock Select]
  /***********************************************************************
   - SYSCLK is equal to selected clock source divided by 1
   - Clock derived from the internal precision High-Frequency Oscillator
   ***********************************************************************/
  CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__HFOSC;
  // Wait for the clock to be ready
  while ((CLKSEL & CLKSEL_CLKRDY__BMASK) != CLKSEL_CLKRDY__SET)
    ;
  // [CLKSEL - Clock Select]$

  // $[PCLKEN - Low Power Peripheral Clock Enable]
  // [PCLKEN - Low Power Peripheral Clock Enable]$

  // $[CLKMODE -  Clock Mode]
  // [CLKMODE -  Clock Mode]$

}

extern void PORTS_0_enter_DefaultMode_from_RESET(void)
{
  // $[P0 - Port 0 Pin Latch]
  // [P0 - Port 0 Pin Latch]$

  // $[P0MDOUT - Port 0 Output Mode]
  /***********************************************************************
   - P0.0 output is open-drain
   - P0.1 output is open-drain
   - P0.2 output is open-drain
   - P0.3 output is open-drain
   - P0.4 output is open-drain
   - P0.5 output is push-pull
   - P0.6 output is open-drain
   - P0.7 output is open-drain
   ***********************************************************************/
  SFRPAGE = 0x0F;
  P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN
      | P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__OPEN_DRAIN | P0MDOUT_B4__OPEN_DRAIN
      | P0MDOUT_B5__PUSH_PULL | P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__OPEN_DRAIN;
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

extern void PORTS_1_enter_DefaultMode_from_RESET(void)
{
  // $[P1 - Port 1 Pin Latch]
  // [P1 - Port 1 Pin Latch]$

  // $[P1MDOUT - Port 1 Output Mode]
  /***********************************************************************
   - P1.0 output is open-drain
   - P1.1 output is open-drain
   - P1.2 output is open-drain
   - P1.3 output is push-pull
   - P1.4 output is open-drain
   - P1.5 output is open-drain
   - P1.6 output is open-drain
   - P1.7 output is open-drain
   ***********************************************************************/
  P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
      | P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__PUSH_PULL | P1MDOUT_B4__OPEN_DRAIN
      | P1MDOUT_B5__OPEN_DRAIN | P1MDOUT_B6__OPEN_DRAIN
      | P1MDOUT_B7__OPEN_DRAIN;
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
  /***********************************************************************
   - Timer 0 High Byte = 0x5D
   ***********************************************************************/
  TH0 = (0x5D << TH0_TH0__SHIFT);
  // [TH0 - Timer 0 High Byte]$

  // $[TL0 - Timer 0 Low Byte]
  /***********************************************************************
   - Timer 0 Low Byte = 0x0F
   ***********************************************************************/
  TL0 = (0x0F << TL0_TL0__SHIFT);
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

extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void)
{
  // $[CKCON - Clock Control]
  /***********************************************************************
   - System clock divided by 48
   - Counter/Timer 0 uses the clock defined by the prescale field, SCA
   - Timer 2 high byte uses the clock defined by the T2XCLK bit in TMR2CN
   - Timer 2 low byte uses the clock defined by the T2XCLK bit in TMR2CN
   - Timer 3 high byte uses the clock defined by the T3XCLK bit in TMR3CN
   - Timer 3 low byte uses the clock defined by the T3XCLK bit in TMR3CN
   - Timer 1 uses the clock defined by the prescale field, SCA
   ***********************************************************************/
  CKCON = CKCON_SCA__SYSCLK_DIV_48 | CKCON_T0M__PRESCALE
      | CKCON_T2MH__EXTERNAL_CLOCK | CKCON_T2ML__EXTERNAL_CLOCK
      | CKCON_T3MH__EXTERNAL_CLOCK | CKCON_T3ML__EXTERNAL_CLOCK
      | CKCON_T1M__PRESCALE;
  // [CKCON - Clock Control]$

  // $[TMOD - Timer 0/1 Mode]
  // [TMOD - Timer 0/1 Mode]$

  // $[TCON - Timer 0/1 Control]
  /***********************************************************************
   - Start Timer 0 running
   ***********************************************************************/
  TCON |= TCON_TR0__RUN;
  // [TCON - Timer 0/1 Control]$

}

extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void)
{
  // $[EIE1 - Extended Interrupt Enable 1]
  // [EIE1 - Extended Interrupt Enable 1]$

  // $[EIP1 - Extended Interrupt Priority 1]
  // [EIP1 - Extended Interrupt Priority 1]$

  // $[IE - Interrupt Enable]
  /***********************************************************************
   - Enable each interrupt according to its individual mask setting
   - Disable external interrupt 0
   - Disable external interrupt 1
   - Disable all SPI0 interrupts
   - Enable interrupt requests generated by the TF0 flag
   - Disable all Timer 1 interrupt
   - Disable Timer 2 interrupt
   - Disable UART0 interrupt
   ***********************************************************************/
  IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__DISABLED
      | IE_ET0__ENABLED | IE_ET1__DISABLED | IE_ET2__DISABLED
      | IE_ES0__DISABLED;
  // [IE - Interrupt Enable]$

  // $[IP - Interrupt Priority]
  // [IP - Interrupt Priority]$

  // $[EIE2 - Extended Interrupt Enable 2]
  // [EIE2 - Extended Interrupt Enable 2]$

  // $[EIP2 - Extended Interrupt Priority 2]
  // [EIP2 - Extended Interrupt Priority 2]$

}

