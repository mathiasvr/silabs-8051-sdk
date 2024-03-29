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
  PORTS_1_enter_DefaultMode_from_RESET();
  PBCFG_0_enter_DefaultMode_from_RESET();
  VREF_0_enter_DefaultMode_from_RESET();
  EXTOSC_0_enter_DefaultMode_from_RESET();
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
  XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED
      | XBR1_T0E__DISABLED | XBR1_T1E__DISABLED;
  // [XBR1 - Port I/O Crossbar 1]$

  // $[XBR0 - Port I/O Crossbar 0]
  /***********************************************************************
   - SMBus0 I/O unavailable at Port pins
   - UART I/O unavailable at Port pin
   - SPI I/O unavailable at Port pins
   - SYSCLK output routed to Port pin
   - All PCA I/O unavailable at Port pins
   - ECI unavailable at Port pin
   ***********************************************************************/
  XBR0 = XBR0_SMB0E__DISABLED | XBR0_URT0E__DISABLED | XBR0_SPI0E__DISABLED
      | XBR0_SYSCKE__ENABLED | XBR0_PCA0ME__DISABLED | XBR0_ECIE__DISABLED;
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
   - Clock derived from the External Oscillator circuit
   ***********************************************************************/
  CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__EXTOSC;
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
   - P0.1 output is push-pull
   - P0.2 output is open-drain
   - P0.3 output is open-drain
   - P0.4 output is open-drain
   - P0.5 output is open-drain
   - P0.6 output is open-drain
   - P0.7 output is open-drain
   ***********************************************************************/
  SFRPAGE = 0x0F;
  P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__PUSH_PULL
      | P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__OPEN_DRAIN | P0MDOUT_B4__OPEN_DRAIN
      | P0MDOUT_B5__OPEN_DRAIN | P0MDOUT_B6__OPEN_DRAIN
      | P0MDOUT_B7__OPEN_DRAIN;
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
   - P0.1 output has high output drive strength
   - P0.2 output has low output drive strength
   - P0.3 output has low output drive strength
   - P0.4 output has low output drive strength
   - P0.5 output has low output drive strength
   - P0.6 output has low output drive strength
   - P0.7 output has low output drive strength
   ***********************************************************************/
  P0DRV = P0DRV_B0__LOW_DRIVE | P0DRV_B1__HIGH_DRIVE | P0DRV_B2__LOW_DRIVE
      | P0DRV_B3__LOW_DRIVE | P0DRV_B4__LOW_DRIVE | P0DRV_B5__LOW_DRIVE
      | P0DRV_B6__LOW_DRIVE | P0DRV_B7__LOW_DRIVE;
  // [P0DRV - Port 0 Drive Strength]$

}

extern void PORTS_1_enter_DefaultMode_from_RESET(void)
{
  // $[P1 - Port 1 Pin Latch]
  // [P1 - Port 1 Pin Latch]$

  // $[P1MDOUT - Port 1 Output Mode]
  // [P1MDOUT - Port 1 Output Mode]$

  // $[P1MDIN - Port 1 Input Mode]
  /***********************************************************************
   - P1.0 pin is configured for digital mode
   - P1.1 pin is configured for analog mode
   - P1.2 pin is configured for digital mode
   - P1.3 pin is configured for digital mode
   - P1.4 pin is configured for digital mode
   - P1.5 pin is configured for digital mode
   - P1.6 pin is configured for digital mode
   - P1.7 pin is configured for digital mode
   ***********************************************************************/
  P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__ANALOG | P1MDIN_B2__DIGITAL
      | P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__DIGITAL
      | P1MDIN_B6__DIGITAL | P1MDIN_B7__DIGITAL;
  // [P1MDIN - Port 1 Input Mode]$

  // $[P1SKIP - Port 1 Skip]
  /***********************************************************************
   - P1.0 pin is not skipped by the crossbar
   - P1.1 pin is skipped by the crossbar
   - P1.2 pin is not skipped by the crossbar
   - P1.3 pin is not skipped by the crossbar
   - P1.4 pin is not skipped by the crossbar
   - P1.5 pin is not skipped by the crossbar
   - P1.6 pin is not skipped by the crossbar
   - P1.7 pin is not skipped by the crossbar
   ***********************************************************************/
  P1SKIP = P1SKIP_B0__NOT_SKIPPED | P1SKIP_B1__SKIPPED | P1SKIP_B2__NOT_SKIPPED
      | P1SKIP_B3__NOT_SKIPPED | P1SKIP_B4__NOT_SKIPPED | P1SKIP_B5__NOT_SKIPPED
      | P1SKIP_B6__NOT_SKIPPED | P1SKIP_B7__NOT_SKIPPED;
  // [P1SKIP - Port 1 Skip]$

  // $[P1MASK - Port 1 Mask]
  // [P1MASK - Port 1 Mask]$

  // $[P1MAT - Port 1 Match]
  // [P1MAT - Port 1 Match]$

  // $[P1DRV - Port 1 Drive Strength]
  // [P1DRV - Port 1 Drive Strength]$

}

extern void EXTOSC_0_enter_DefaultMode_from_RESET(void)
{
  // $[Delay counter definition]
  uint16_t delayCounter;
  // [Delay counter definition]$

  // $[OSCXCN - External Oscillator Control]
  /***********************************************************************
   - Select external oscillator mode 3: 155 kHz < Crystal frequency <= 415
   kHz, 100 kHz < RC/C frequency <= 200 kHz, C mode K factor = 22
   - RC Oscillator Mode
   ***********************************************************************/
  OSCXCN = OSCXCN_XFCN__MODE3 | OSCXCN_XOSCMD__RC;

  // Delay for at least 1ms
  for (delayCounter = 0x1000; delayCounter > 0; delayCounter--)
    ;

  // Poll the clock valid bit until the clock is ready

  while ((OSCXCN & OSCXCN_XCLKVLD__BMASK) != OSCXCN_XCLKVLD__SET)
    ;
  // [OSCXCN - External Oscillator Control]$

}

