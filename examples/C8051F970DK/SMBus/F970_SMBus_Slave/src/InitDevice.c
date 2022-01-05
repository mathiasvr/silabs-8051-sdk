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
  VREG_0_enter_DefaultMode_from_RESET();
  PORTS_0_enter_DefaultMode_from_RESET();
  PBCFG_0_enter_DefaultMode_from_RESET();
  VREF_0_enter_DefaultMode_from_RESET();
  HFOSC_0_enter_DefaultMode_from_RESET();
  CLOCK_0_enter_DefaultMode_from_RESET();
  TIMER01_0_enter_DefaultMode_from_RESET();
  TIMER16_3_enter_DefaultMode_from_RESET();
  TIMER_SETUP_0_enter_DefaultMode_from_RESET();
  SMBUS_0_enter_DefaultMode_from_RESET();
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
  /***********************************************************************
   - Disable CCF2 interrupts
   - Enable match function
   - Disable negative edge capture
   - 8 to 11-bit PWM selected
   - Disable positive edge capture
   - Enable comparator function
   - Disable PWM function
   - Disable toggle function
   ***********************************************************************/
  PCA0CPM2 = PCA0CPM2_ECCF__DISABLED | PCA0CPM2_MAT__ENABLED
      | PCA0CPM2_CAPN__DISABLED | PCA0CPM2_PWM16__8_BIT
      | PCA0CPM2_CAPP__DISABLED | PCA0CPM2_ECOM__ENABLED
      | PCA0CPM2_PWM__DISABLED | PCA0CPM2_TOG__DISABLED;
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
  /***********************************************************************
   - SMBus0 I/O routed to Port pins
   - UART I/O unavailable at Port pin
   - SPI I/O unavailable at Port pins
   - SYSCLK unavailable at Port pin
   - All PCA I/O unavailable at Port pins
   - ECI unavailable at Port pin
   ***********************************************************************/
  XBR0 = XBR0_SMB0E__ENABLED | XBR0_URT0E__DISABLED | XBR0_SPI0E__DISABLED
      | XBR0_SYSCKE__DISABLED | XBR0_PCA0ME__DISABLED | XBR0_ECIE__DISABLED;
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
  /***********************************************************************
   - P0.0 pin is skipped by the crossbar
   - P0.1 pin is not skipped by the crossbar
   - P0.2 pin is not skipped by the crossbar
   - P0.3 pin is not skipped by the crossbar
   - P0.4 pin is not skipped by the crossbar
   - P0.5 pin is not skipped by the crossbar
   - P0.6 pin is not skipped by the crossbar
   - P0.7 pin is not skipped by the crossbar
   ***********************************************************************/
  P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__NOT_SKIPPED | P0SKIP_B2__NOT_SKIPPED
      | P0SKIP_B3__NOT_SKIPPED | P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
      | P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__NOT_SKIPPED;
  // [P0SKIP - Port 0 Skip]$

  // $[P0MASK - Port 0 Mask]
  // [P0MASK - Port 0 Mask]$

  // $[P0MAT - Port 0 Match]
  // [P0MAT - Port 0 Match]$

  // $[P0DRV - Port 0 Drive Strength]
  /***********************************************************************
   - P0.0 output has low output drive strength
   - P0.1 output has low output drive strength
   - P0.2 output has low output drive strength
   - P0.3 output has low output drive strength
   - P0.4 output has low output drive strength
   - P0.5 output has high output drive strength
   - P0.6 output has low output drive strength
   - P0.7 output has low output drive strength
   ***********************************************************************/
  P0DRV = P0DRV_B0__LOW_DRIVE | P0DRV_B1__LOW_DRIVE | P0DRV_B2__LOW_DRIVE
      | P0DRV_B3__LOW_DRIVE | P0DRV_B4__LOW_DRIVE | P0DRV_B5__HIGH_DRIVE
      | P0DRV_B6__LOW_DRIVE | P0DRV_B7__LOW_DRIVE;
  // [P0DRV - Port 0 Drive Strength]$

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
   - Timer 1 High Byte = 0xBC
   ***********************************************************************/
  TH1 = (0xBC << TH1_TH1__SHIFT);
  // [TH1 - Timer 1 High Byte]$

  // $[TL1 - Timer 1 Low Byte]
  /***********************************************************************
   - Timer 1 Low Byte = 0xBC
   ***********************************************************************/
  TL1 = (0xBC << TL1_TL1__SHIFT);
  // [TL1 - Timer 1 Low Byte]$

  // $[Timer Restoration]
  //Restore Timer Configuration
  TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

  // [Timer Restoration]$

}

extern void TIMER16_3_enter_DefaultMode_from_RESET(void)
{
  // $[Timer Initialization]
  // Save Timer Configuration
  uint8_t TMR3CN_TR3_save;
  TMR3CN_TR3_save = TMR3CN & TMR3CN_TR3__BMASK;
  // Stop Timer
  TMR3CN &= ~(TMR3CN_TR3__BMASK);
  // [Timer Initialization]$

  // $[TMR3CN - Timer 3 Control]
  // [TMR3CN - Timer 3 Control]$

  // $[TMR3H - Timer 3 High Byte]
  /***********************************************************************
   - Timer 3 High Byte = 0x38
   ***********************************************************************/
  TMR3H = (0x38 << TMR3H_TMR3H__SHIFT);
  // [TMR3H - Timer 3 High Byte]$

  // $[TMR3L - Timer 3 Low Byte]
  /***********************************************************************
   - Timer 3 Low Byte = 0x9E
   ***********************************************************************/
  TMR3L = (0x9E << TMR3L_TMR3L__SHIFT);
  // [TMR3L - Timer 3 Low Byte]$

  // $[TMR3RLH - Timer 3 Reload High Byte]
  /***********************************************************************
   - Timer 3 Reload High Byte = 0x38
   ***********************************************************************/
  TMR3RLH = (0x38 << TMR3RLH_TMR3RLH__SHIFT);
  // [TMR3RLH - Timer 3 Reload High Byte]$

  // $[TMR3RLL - Timer 3 Reload Low Byte]
  /***********************************************************************
   - Timer 3 Reload Low Byte = 0x9E
   ***********************************************************************/
  TMR3RLL = (0x9E << TMR3RLL_TMR3RLL__SHIFT);
  // [TMR3RLL - Timer 3 Reload Low Byte]$

  // $[TMR3CN]
  /***********************************************************************
   - Start Timer 3 running
   ***********************************************************************/
  TMR3CN |= TMR3CN_TR3__RUN;
  // [TMR3CN]$

  // $[Timer Restoration]
  // Restore Timer Configuration
  TMR3CN |= TMR3CN_TR3_save;
  // [Timer Restoration]$

}

extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void)
{
  // $[CKCON - Clock Control]
  // [CKCON - Clock Control]$

  // $[TMOD - Timer 0/1 Mode]
  /***********************************************************************
   - Mode 0, 13-bit Counter/Timer
   - Timer Mode
   - Timer 0 enabled when TR0 = 1 irrespective of /INT0 logic level
   - Mode 2, 8-bit Counter/Timer with Auto-Reload
   - Timer Mode
   - Timer 1 enabled when TR1 = 1 irrespective of /INT1 logic level
   ***********************************************************************/
  TMOD = TMOD_T0M__MODE0 | TMOD_CT0__TIMER | TMOD_GATE0__DISABLED
      | TMOD_T1M__MODE2 | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
  // [TMOD - Timer 0/1 Mode]$

  // $[TCON - Timer 0/1 Control]
  /***********************************************************************
   - Start Timer 1 running
   ***********************************************************************/
  TCON |= TCON_TR1__RUN;
  // [TCON - Timer 0/1 Control]$

}

extern void SMBUS_0_enter_DefaultMode_from_RESET(void)
{
  // $[SMB0CF - SMBus 0 Configuration]
  /***********************************************************************
   - Timer 1 Overflow
   - Slave states are enabled
   - Enable the SMBus module
   - Enable bus free timeouts
   - Enable SCL low timeouts
   - Enable SDA extended setup and hold times
   ***********************************************************************/
  SMB0CF = SMB0CF_SMBCS__TIMER1 | SMB0CF_INH__SLAVE_ENABLED
      | SMB0CF_ENSMB__ENABLED | SMB0CF_SMBFTE__FREE_TO_ENABLED
      | SMB0CF_SMBTOE__SCL_TO_ENABLED | SMB0CF_EXTHOLD__ENABLED;
  // [SMB0CF - SMBus 0 Configuration]$

  // $[SMB0ADR - SMBus 0 Slave Address]
  // [SMB0ADR - SMBus 0 Slave Address]$

  // $[SMB0ADM - SMBus 0 Slave Address Mask]
  // [SMB0ADM - SMBus 0 Slave Address Mask]$

}

extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void)
{
  // $[EIE1 - Extended Interrupt Enable 1]
  /***********************************************************************
   - Disable ADC0 Conversion Complete interrupt
   - Disable DMA0 interrupts
   - Disable DMA0M interrupts
   - Disable all PCA0 interrupts
   - Disable RTC Alarm interrupts
   - Enable interrupt requests generated by SMB0
   - Disable Timer 3 interrupts
   - Disable ADC0 Window Comparison interrupt
   ***********************************************************************/
  EIE1 = EIE1_EADC0__DISABLED | EIE1_EDMA0__DISABLED | EIE1_EDMA0M__DISABLED
      | EIE1_EPCA0__DISABLED | EIE1_ERTC0A__DISABLED | EIE1_ESMB0__ENABLED
      | EIE1_ET3__DISABLED | EIE1_EWADC0__DISABLED;
  // [EIE1 - Extended Interrupt Enable 1]$

  // $[EIP1 - Extended Interrupt Priority 1]
  // [EIP1 - Extended Interrupt Priority 1]$

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

  // $[EIE2 - Extended Interrupt Enable 2]
  // [EIE2 - Extended Interrupt Enable 2]$

  // $[EIP2 - Extended Interrupt Priority 2]
  // [EIP2 - Extended Interrupt Priority 2]$

}

