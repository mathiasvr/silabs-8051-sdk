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
  PBCFG_0_enter_DefaultMode_from_RESET();
  VREF_0_enter_DefaultMode_from_RESET();
  HFOSC_0_enter_DefaultMode_from_RESET();
  CLOCK_0_enter_DefaultMode_from_RESET();
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
  SFRPAGE = 0x0F;
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

