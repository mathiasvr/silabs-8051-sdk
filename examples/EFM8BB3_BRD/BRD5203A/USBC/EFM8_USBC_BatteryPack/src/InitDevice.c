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
  /*
   // SYSCLK < 50 MHz
   */
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
  /*
   // Clock derived from the Internal High Frequency Oscillator 1
   // SYSCLK is equal to selected clock source divided by 1
   */
  CLKSEL = CLKSEL_CLKSL__HFOSC1 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
    ;
  // [CLKSEL - Clock Select]$

}

extern void UARTE_1_enter_DefaultMode_from_RESET(void)
{
  // $[SBCON1 - UART1 Baud Rate Generator Control]
  /*
   // Enable the baud rate generator
   // Prescaler = 1
   */
  SFRPAGE = 0x20;
  SBCON1 = SBCON1_BREN__ENABLED | SBCON1_BPS__DIV_BY_1;
  // [SBCON1 - UART1 Baud Rate Generator Control]$

  // $[SMOD1 - UART1 Mode]
  // [SMOD1 - UART1 Mode]$

  // $[UART1FCN0 - UART1 FIFO Control 0]
  // [UART1FCN0 - UART1 FIFO Control 0]$

  // $[SBRLH1 - UART1 Baud Rate Generator High Byte]
  /*
   // UART1 Baud Rate Reload High = 0xFF
   */
  SBRLH1 = (0xFF << SBRLH1_BRH__SHIFT);
  // [SBRLH1 - UART1 Baud Rate Generator High Byte]$

  // $[SBRLL1 - UART1 Baud Rate Generator Low Byte]
  /*
   // UART1 Baud Rate Reload Low = 0xE6
   */
  SBRLL1 = (0xE5 << SBRLL1_BRL__SHIFT);
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
  /*
   // Enable each interrupt according to its individual mask setting
   // Disable external interrupt 0
   // Disable external interrupt 1
   // Disable all SPI0 interrupts
   // Disable all Timer 0 interrupt
   // Disable all Timer 1 interrupt
   // Disable Timer 2 interrupt
   // Disable UART0 interrupt
   */
  SFRPAGE = 0x00;
  IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED | IE_ESPI0__DISABLED
      | IE_ET0__DISABLED | IE_ET1__DISABLED | IE_ET2__DISABLED
      | IE_ES0__DISABLED;
  // [IE - Interrupt Enable]$

  // $[IP - Interrupt Priority]
  // [IP - Interrupt Priority]$

  // $[IPH - Interrupt Priority High]
  // [IPH - Interrupt Priority High]$

}
