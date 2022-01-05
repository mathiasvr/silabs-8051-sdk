//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB1_Register_Enums.h>
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
  WDT_0_enter_DefaultMode_from_RESET();
  PORTS_0_enter_DefaultMode_from_RESET();
  PORTS_1_enter_DefaultMode_from_RESET();
  PBCFG_0_enter_DefaultMode_from_RESET();
  CLOCK_0_enter_DefaultMode_from_RESET();
  // [Config Calls]$

}

//================================================================================
// WDT_0_enter_DefaultMode_from_RESET
//================================================================================
extern void WDT_0_enter_DefaultMode_from_RESET(void)
{
  // $[WDTCN - Watchdog Timer Control]
  //Disable Watchdog with key sequence
  WDTCN = 0xDE; //First key
  WDTCN = 0xAD; //Second key
  // [WDTCN - Watchdog Timer Control]$

}

//================================================================================
// PORTS_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_0_enter_DefaultMode_from_RESET(void)
{
  // $[P0 - Port 0 Pin Latch]
  // [P0 - Port 0 Pin Latch]$

  // $[P0MDOUT - Port 0 Output Mode]
  /*
   // P0.0 output is push-pull
   // P0.1 output is open-drain
   // P0.2 output is open-drain
   // P0.3 output is open-drain
   // P0.4 output is open-drain
   // P0.5 output is open-drain
   // P0.6 output is open-drain
   // P0.7 output is push-pull
   */
  P0MDOUT = P0MDOUT_B0__PUSH_PULL
            | P0MDOUT_B1__OPEN_DRAIN
            | P0MDOUT_B2__OPEN_DRAIN
            | P0MDOUT_B3__OPEN_DRAIN
            | P0MDOUT_B4__OPEN_DRAIN
            | P0MDOUT_B5__OPEN_DRAIN
            | P0MDOUT_B6__OPEN_DRAIN
            | P0MDOUT_B7__PUSH_PULL;
  // [P0MDOUT - Port 0 Output Mode]$

  // $[P0MDIN - Port 0 Input Mode]
  // [P0MDIN - Port 0 Input Mode]$

  // $[P0SKIP - Port 0 Skip]
  /*
   // P0.0 pin is skipped by the crossbar
   // P0.1 pin is skipped by the crossbar
   // P0.2 pin is skipped by the crossbar
   // P0.3 pin is skipped by the crossbar
   // P0.4 pin is skipped by the crossbar
   // P0.5 pin is skipped by the crossbar
   // P0.6 pin is skipped by the crossbar
   // P0.7 pin is not skipped by the crossbar
   */
  P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED | P0SKIP_B2__SKIPPED
           | P0SKIP_B3__SKIPPED
           | P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
           | P0SKIP_B6__SKIPPED
           | P0SKIP_B7__NOT_SKIPPED;
  // [P0SKIP - Port 0 Skip]$

  // $[P0MASK - Port 0 Mask]
  /*
   // P0.0 pin logic value is ignored and will not cause a port mismatch
   //     event
   // P0.1 pin logic value is ignored and will not cause a port mismatch
   //     event
   // P0.2 pin logic value is compared to P0MAT.2
   // P0.3 pin logic value is compared to P0MAT.3
   // P0.4 pin logic value is ignored and will not cause a port mismatch
   //     event
   // P0.5 pin logic value is ignored and will not cause a port mismatch
   //     event
   // P0.6 pin logic value is ignored and will not cause a port mismatch
   //     event
   // P0.7 pin logic value is ignored and will not cause a port mismatch
   //     event
   */
  P0MASK = P0MASK_B0__IGNORED | P0MASK_B1__IGNORED | P0MASK_B2__COMPARED
           | P0MASK_B3__COMPARED
           | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
           | P0MASK_B6__IGNORED
           | P0MASK_B7__IGNORED;
  // [P0MASK - Port 0 Mask]$

  // $[P0MAT - Port 0 Match]
  // [P0MAT - Port 0 Match]$

}

//================================================================================
// PORTS_1_enter_DefaultMode_from_RESET
//================================================================================
extern void PORTS_1_enter_DefaultMode_from_RESET(void)
{
  // $[P1 - Port 1 Pin Latch]
  // [P1 - Port 1 Pin Latch]$

  // $[P1MDOUT - Port 1 Output Mode]
  /*
   // P1.0 output is open-drain
   // P1.1 output is open-drain
   // P1.2 output is open-drain
   // P1.3 output is open-drain
   // P1.4 output is push-pull
   // P1.5 output is push-pull
   // P1.6 output is push-pull
   // P1.7 output is open-drain
   */
  P1MDOUT = P1MDOUT_B0__OPEN_DRAIN
            | P1MDOUT_B1__OPEN_DRAIN
            | P1MDOUT_B2__OPEN_DRAIN
            | P1MDOUT_B3__OPEN_DRAIN
            | P1MDOUT_B4__PUSH_PULL
            | P1MDOUT_B5__PUSH_PULL
            | P1MDOUT_B6__PUSH_PULL
            | P1MDOUT_B7__OPEN_DRAIN;
  // [P1MDOUT - Port 1 Output Mode]$

  // $[P1MDIN - Port 1 Input Mode]
  // [P1MDIN - Port 1 Input Mode]$

  // $[P1SKIP - Port 1 Skip]
  /*
   // P1.0 pin is not skipped by the crossbar
   // P1.1 pin is not skipped by the crossbar
   // P1.2 pin is not skipped by the crossbar
   // P1.3 pin is not skipped by the crossbar
   // P1.4 pin is skipped by the crossbar
   // P1.5 pin is skipped by the crossbar
   // P1.6 pin is skipped by the crossbar
   // P1.7 pin is not skipped by the crossbar
   */
  P1SKIP = P1SKIP_B0__NOT_SKIPPED
           | P1SKIP_B1__NOT_SKIPPED
           | P1SKIP_B2__NOT_SKIPPED
           | P1SKIP_B3__NOT_SKIPPED
           | P1SKIP_B4__SKIPPED
           | P1SKIP_B5__SKIPPED
           | P1SKIP_B6__SKIPPED
           | P1SKIP_B7__NOT_SKIPPED;
  // [P1SKIP - Port 1 Skip]$

  // $[P1MASK - Port 1 Mask]
  // [P1MASK - Port 1 Mask]$

  // $[P1MAT - Port 1 Match]
  // [P1MAT - Port 1 Match]$

}

//================================================================================
// PBCFG_0_enter_DefaultMode_from_RESET
//================================================================================
extern void PBCFG_0_enter_DefaultMode_from_RESET(void)
{
  // $[XBR2 - Port I/O Crossbar 2]
  /*
   // Weak Pullups enabled 
   // Crossbar enabled
   */
  XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;
  // [XBR2 - Port I/O Crossbar 2]$

  // $[PRTDRV - Port Drive Strength]
  // [PRTDRV - Port Drive Strength]$

  // $[XBR0 - Port I/O Crossbar 0]
  /*
   // UART I/O unavailable at Port pin
   // SPI I/O unavailable at Port pins
   // SMBus 0 I/O unavailable at Port pins
   // CP0 unavailable at Port pin
   // Asynchronous CP0 unavailable at Port pin
   // CP1 unavailable at Port pin
   // Asynchronous CP1 unavailable at Port pin
   // SYSCLK output routed to Port pin
   */
  XBR0 = XBR0_URT0E__DISABLED | XBR0_SPI0E__DISABLED | XBR0_SMB0E__DISABLED
         | XBR0_CP0E__DISABLED
         | XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
         | XBR0_CP1AE__DISABLED
         | XBR0_SYSCKE__ENABLED;
  // [XBR0 - Port I/O Crossbar 0]$

  // $[XBR1 - Port I/O Crossbar 1]
  // [XBR1 - Port I/O Crossbar 1]$

}

//================================================================================
// CLOCK_0_enter_DefaultMode_from_RESET
//================================================================================
extern void CLOCK_0_enter_DefaultMode_from_RESET(void)
{
  // $[CLKSEL - Clock Select]
  /*
   // Clock derived from the Internal High-Frequency Oscillator
   // SYSCLK is equal to selected clock source divided by 1
   */
  CLKSEL = CLKSEL_CLKSL__HFOSC | CLKSEL_CLKDIV__SYSCLK_DIV_1;
  // [CLKSEL - Clock Select]$

}

