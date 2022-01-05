//=========================================================
// src/InitDevice.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================

// USER INCLUDES
#include <SI_EFM8BB51_Register_Enums.h>
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
    PORTS_0_enter_DefaultMode_from_RESET();
    PORTS_1_enter_DefaultMode_from_RESET();
    PBCFG_0_enter_DefaultMode_from_RESET();
    CLU_1_enter_DefaultMode_from_RESET();
    CLU_2_enter_DefaultMode_from_RESET();
    CLOCK_0_enter_DefaultMode_from_RESET();
    TIMER16_2_enter_DefaultMode_from_RESET();
    TIMER16_3_enter_DefaultMode_from_RESET();
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

extern void PORTS_2_enter_DefaultMode_from_RESET(void)
{
    // $[P2 - Port 2 Pin Latch]
    // [P2 - Port 2 Pin Latch]$

    // $[P2MDOUT - Port 2 Output Mode]
    /*
     // B0 (Port 2 Bit 0 Output Mode) = PUSH_PULL (P2.0 output is push-pull.)
     */
    P2MDOUT = P2MDOUT_B0__PUSH_PULL;
    // [P2MDOUT - Port 2 Output Mode]$

    // $[P2MDIN - Port 2 Input Mode]
    // [P2MDIN - Port 2 Input Mode]$
}

extern void PBCFG_0_enter_DefaultMode_from_RESET(void)
{
    // $[XBR2 - Port I/O Crossbar 2]
    /*
     // WEAKPUD (Port I/O Weak Pullup Disable) = PULL_UPS_ENABLED (Weak
     //     Pullups enabled (except for Ports whose I/O are configured for analog
     //     mode).)
     // XBARE (Crossbar Enable) = ENABLED (Crossbar enabled.)
     // URT1E (UART1 I/O Enable) = DISABLED (UART1 I/O unavailable at Port
     //     pin.)
     // URT1RTSE (UART1 RTS Output Enable) = DISABLED (UART1 RTS1 unavailable
     //     at Port pin.)
     // URT1CTSE (UART1 CTS Input Enable) = DISABLED (UART1 CTS1 unavailable
     //     at Port pin.)
     */
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

extern void CLU_2_enter_DefaultMode_from_RESET(void)
{
    // $[CLU2FN - Configurable Logic Unit 2 Function Select]
    // [CLU2FN - Configurable Logic Unit 2 Function Select]$

    // $[CLU2MX - Configurable Logic Unit 2 Multiplexer]
    /*
     // MXA (CLU2 A Input Multiplexer Selection) = CLU2A1 (Select CLU2A.1)
     // MXB (CLU2 B Input Multiplexer Selection) = CLU2B8 (Select CLU2B.8)
     */
    CLU2MX = CLU2MX_MXA__CLU2A1 | CLU2MX_MXB__CLU2B8;
    // [CLU2MX - Configurable Logic Unit 2 Multiplexer]$

    // $[CLU2CF - Configurable Logic Unit 2 Configuration]
    /*
     // OUTSEL (CLU Output Select) = LUT (Select LUT output.)
     */
    CLU2CF |= CLU2CF_OUTSEL__LUT;
    // [CLU2CF - Configurable Logic Unit 2 Configuration]$

}

extern void CL_0_enter_DefaultMode_from_RESET(void)
{
    // $[CLEN0 - Configurable Logic Enable 0]
    /*
     // C0EN (CLU0 Enable) = DISABLE (CLU0 is disabled. The output of the
     //     block will be logic low.)
     // C1EN (CLU1 Enable) = DISABLE (CLU1 is disabled. The output of the
     //     block will be logic low.)
     // C2EN (CLU2 Enable) = ENABLE (CLU2 is enabled.)
     // C3EN (CLU3 Enable) = DISABLE (CLU3 is disabled. The output of the
     //     block will be logic low.)
     */
    CLEN0 = CLEN0_C0EN__DISABLE | CLEN0_C1EN__DISABLE | CLEN0_C2EN__ENABLE
            | CLEN0_C3EN__DISABLE;
    // [CLEN0 - Configurable Logic Enable 0]$

    // $[CLIE0 - Configurable Logic Interrupt Enable 0]
    // [CLIE0 - Configurable Logic Interrupt Enable 0]$

}

extern void CLOCK_0_enter_DefaultMode_from_RESET(void)
{
    // $[HFOSC1 Setup]
    // [HFOSC1 Setup]$

    // $[CLKSEL - Clock Select]
    /*
     // CLKSL (Clock Source Select) = HFOSC0 (Clock derived from the Internal
     //     High Frequency Oscillator 0.)
     // CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
     //     selected clock source divided by 1.)
     // CLKSL (Clock Source Select) = HFOSC0 (Clock derived from the Internal
     //     High Frequency Oscillator 0.)
     // CLKDIV (Clock Source Divider) = SYSCLK_DIV_1 (SYSCLK is equal to
     //     selected clock source divided by 1.)
     */
    SFRPAGE = 0x00;
    CLKSEL = CLKSEL_CLKSL__HFOSC0_clk24p5 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
    while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY);
    // [CLKSEL - Clock Select]$

}

extern void TIMER16_2_enter_DefaultMode_from_RESET(void)
{
    // $[Timer Initialization]
    // Save Timer Configuration
    uint8_t TMR2CN0_TR2_save;
    TMR2CN0_TR2_save = TMR2CN0 & TMR2CN0_TR2__BMASK;
    // Stop Timer
    TMR2CN0 &= ~(TMR2CN0_TR2__BMASK);
    // [Timer Initialization]$

    // $[TMR2CN1 - Timer 2 Control 1]
    /*
     // T2CSEL (Timer 2 Capture Select) = LFOSC (Capture high-to-low
     //     transitions of the LFO oscillator.)
     // RLFSEL (Force Reload Select) = CLU2_OUT (Timer will reload on overflow
     //     events and CLU2 synchronous output high.)
     */
    SFRPAGE = 0x10;
    TMR2CN1 = TMR2CN1_T2CSEL__LFOSC | TMR2CN1_RLFSEL__CLU2_OUT;
    // [TMR2CN1 - Timer 2 Control 1]$

    // $[TMR2CN0 - Timer 2 Control]
    SFRPAGE = 0x00;
    // [TMR2CN0 - Timer 2 Control]$

    // $[TMR2H - Timer 2 High Byte]
    /*
     // TMR2H (Timer 2 High Byte) = 0xB0
     */
    TMR2H = (0xB0 << TMR2H_TMR2H__SHIFT);
    // [TMR2H - Timer 2 High Byte]$

    // $[TMR2L - Timer 2 Low Byte]
    /*
     // TMR2L (Timer 2 Low Byte) = 0x3F
     */
    TMR2L = (0x3F << TMR2L_TMR2L__SHIFT);
    // [TMR2L - Timer 2 Low Byte]$

    // $[TMR2RLH - Timer 2 Reload High Byte]
    /*
     // TMR2RLH (Timer 2 Reload High Byte) = 0xB0
     */
    TMR2RLH = (0xB0 << TMR2RLH_TMR2RLH__SHIFT);
    // [TMR2RLH - Timer 2 Reload High Byte]$

    // $[TMR2RLL - Timer 2 Reload Low Byte]
    /*
     // TMR2RLL (Timer 2 Reload Low Byte) = 0x3F
     */
    TMR2RLL = (0x3F << TMR2RLL_TMR2RLL__SHIFT);
    // [TMR2RLL - Timer 2 Reload Low Byte]$

    // $[TMR2CN0]
    /*
     // TR2 (Timer 2 Run Control) = RUN (Start Timer 2 running.)
     */
    TMR2CN0 |= TMR2CN0_TR2__RUN;
    // [TMR2CN0]$

    // $[Timer Restoration]
    // Restore Timer Configuration
    TMR2CN0 |= TMR2CN0_TR2_save;
    // [Timer Restoration]$

}

extern void TIMER16_3_enter_DefaultMode_from_RESET(void)
{
    // $[Timer Initialization]
    // Save Timer Configuration
    uint8_t TMR3CN0_TR3_save;
    TMR3CN0_TR3_save = TMR3CN0 & TMR3CN0_TR3__BMASK;
    // Stop Timer
    TMR3CN0 &= ~(TMR3CN0_TR3__BMASK);
    // [Timer Initialization]$

    // $[TMR3CN1 - Timer 3 Control 1]
    // [TMR3CN1 - Timer 3 Control 1]$

    // $[TMR3CN0 - Timer 3 Control]
    // [TMR3CN0 - Timer 3 Control]$

    // $[TMR3H - Timer 3 High Byte]
    // [TMR3H - Timer 3 High Byte]$

    // $[TMR3L - Timer 3 Low Byte]
    // [TMR3L - Timer 3 Low Byte]$

    // $[TMR3RLH - Timer 3 Reload High Byte]
    // [TMR3RLH - Timer 3 Reload High Byte]$

    // $[TMR3RLL - Timer 3 Reload Low Byte]
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

extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void)
{

}

extern void PORTS_0_enter_DefaultMode_from_RESET(void)
{
    // $[P0 - Port 0 Pin Latch]
    /*
     // B0 (Port 0 Bit 0 Latch) = HIGH (P0.0 is high. Set P0.0 to drive or
     //     float high.)
     // B1 (Port 0 Bit 1 Latch) = HIGH (P0.1 is high. Set P0.1 to drive or
     //     float high.)
     // B2 (Port 0 Bit 2 Latch) = HIGH (P0.2 is high. Set P0.2 to drive or
     //     float high.)
     // B3 (Port 0 Bit 3 Latch) = HIGH (P0.3 is high. Set P0.3 to drive or
     //     float high.)
     // B4 (Port 0 Bit 4 Latch) = HIGH (P0.4 is high. Set P0.4 to drive or
     //     float high.)
     // B5 (Port 0 Bit 5 Latch) = HIGH (P0.5 is high. Set P0.5 to drive or
     //     float high.)
     // B6 (Port 0 Bit 6 Latch) = HIGH (P0.6 is high. Set P0.6 to drive or
     //     float high.)
     // B7 (Port 0 Bit 7 Latch) = HIGH (P0.7 is high. Set P0.7 to drive or
     //     float high.)
     */
    P0 = P0_B0__LOW | P0_B1__HIGH | P0_B2__HIGH | P0_B3__HIGH | P0_B4__HIGH
            | P0_B5__HIGH | P0_B6__HIGH | P0_B7__HIGH;
    // [P0 - Port 0 Pin Latch]$

    // $[P0MDOUT - Port 0 Output Mode]
    /*
     // B0 (Port 0 Bit 0 Output Mode) = PUSH_PULL (P0.0 output is push-pull.)
     // B1 (Port 0 Bit 1 Output Mode) = OPEN_DRAIN (P0.1 output is open-
     //     drain.)
     // B2 (Port 0 Bit 2 Output Mode) = OPEN_DRAIN (P0.2 output is open-
     //     drain.)
     // B3 (Port 0 Bit 3 Output Mode) = OPEN_DRAIN (P0.3 output is open-
     //     drain.)
     // B4 (Port 0 Bit 4 Output Mode) = OPEN_DRAIN (P0.4 output is open-
     //     drain.)
     // B5 (Port 0 Bit 5 Output Mode) = OPEN_DRAIN (P0.5 output is open-
     //     drain.)
     // B6 (Port 0 Bit 6 Output Mode) = OPEN_DRAIN (P0.6 output is open-
     //     drain.)
     // B7 (Port 0 Bit 7 Output Mode) = OPEN_DRAIN (P0.7 output is open-
     //     drain.)
     */
    P0MDOUT = P0MDOUT_B0__PUSH_PULL | P0MDOUT_B1__OPEN_DRAIN
            | P0MDOUT_B2__OPEN_DRAIN | P0MDOUT_B3__OPEN_DRAIN
            | P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__PUSH_PULL
            | P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__OPEN_DRAIN;
    // [P0MDOUT - Port 0 Output Mode]$

    // $[P0MDIN - Port 0 Input Mode]
    /*
     // B0 (Port 0 Bit 0 Input Mode) = DIGITAL (P0.0 pin is configured for
     //     digital mode.)
     // B1 (Port 0 Bit 1 Input Mode) = DIGITAL (P0.1 pin is configured for
     //     digital mode.)
     // B2 (Port 0 Bit 2 Input Mode) = DIGITAL (P0.2 pin is configured for
     //     digital mode.)
     // B3 (Port 0 Bit 3 Input Mode) = DIGITAL (P0.3 pin is configured for
     //     digital mode.)
     // B4 (Port 0 Bit 4 Input Mode) = DIGITAL (P0.4 pin is configured for
     //     digital mode.)
     // B5 (Port 0 Bit 5 Input Mode) = DIGITAL (P0.5 pin is configured for
     //     digital mode.)
     // B6 (Port 0 Bit 6 Input Mode) = DIGITAL (P0.6 pin is configured for
     //     digital mode.)
     // B7 (Port 0 Bit 7 Input Mode) = DIGITAL (P0.7 pin is configured for
     //     digital mode.)
     */
    P0MDIN = P0MDIN_B0__DIGITAL | P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL
            | P0MDIN_B3__DIGITAL | P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL
            | P0MDIN_B6__DIGITAL | P0MDIN_B7__DIGITAL;
    // [P0MDIN - Port 0 Input Mode]$

    // $[P0SKIP - Port 0 Skip]
    /*
     // B0 (Port 0 Bit 0 Skip) = SKIPPED (P0.0 pin is skipped by the
     //     crossbar.)
     // B1 (Port 0 Bit 1 Skip) = NOT_SKIPPED (P0.1 pin is not skipped by the
     //     crossbar.)
     // B2 (Port 0 Bit 2 Skip) = NOT_SKIPPED (P0.2 pin is not skipped by the
     //     crossbar.)
     // B3 (Port 0 Bit 3 Skip) = NOT_SKIPPED (P0.3 pin is not skipped by the
     //     crossbar.)
     // B4 (Port 0 Bit 4 Skip) = NOT_SKIPPED (P0.4 pin is not skipped by the
     //     crossbar.)
     // B5 (Port 0 Bit 5 Skip) = NOT_SKIPPED (P0.5 pin is not skipped by the
     //     crossbar.)
     // B6 (Port 0 Bit 6 Skip) = NOT_SKIPPED (P0.6 pin is not skipped by the
     //     crossbar.)
     // B7 (Port 0 Bit 7 Skip) = NOT_SKIPPED (P0.7 pin is not skipped by the
     //     crossbar.)
     */
    P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__NOT_SKIPPED
            | P0SKIP_B2__NOT_SKIPPED | P0SKIP_B3__NOT_SKIPPED
            | P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
            | P0SKIP_B6__NOT_SKIPPED | P0SKIP_B7__NOT_SKIPPED;
    // [P0SKIP - Port 0 Skip]$

    // $[P0MASK - Port 0 Mask]
    /*
     // B0 (Port 0 Bit 0 Mask Value) = IGNORED (P0.0 pin logic value is
     //     ignored and will not cause a port mismatch event.)
     // B1 (Port 0 Bit 1 Mask Value) = IGNORED (P0.1 pin logic value is
     //     ignored and will not cause a port mismatch event.)
     // B2 (Port 0 Bit 2 Mask Value) = IGNORED (P0.2 pin logic value is
     //     ignored and will not cause a port mismatch event.)
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
    P0MASK = P0MASK_B0__IGNORED | P0MASK_B1__IGNORED | P0MASK_B2__IGNORED
            | P0MASK_B3__IGNORED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
            | P0MASK_B6__IGNORED | P0MASK_B7__IGNORED;
    // [P0MASK - Port 0 Mask]$

    // $[P0MAT - Port 0 Match]
    /*
     // B0 (Port 0 Bit 0 Match Value) = HIGH (P0.0 pin logic value is compared
     //     with logic HIGH.)
     // B1 (Port 0 Bit 1 Match Value) = HIGH (P0.1 pin logic value is compared
     //     with logic HIGH.)
     // B2 (Port 0 Bit 2 Match Value) = HIGH (P0.2 pin logic value is compared
     //     with logic HIGH.)
     // B3 (Port 0 Bit 3 Match Value) = HIGH (P0.3 pin logic value is compared
     //     with logic HIGH.)
     // B4 (Port 0 Bit 4 Match Value) = HIGH (P0.4 pin logic value is compared
     //     with logic HIGH.)
     // B5 (Port 0 Bit 5 Match Value) = HIGH (P0.5 pin logic value is compared
     //     with logic HIGH.)
     // B6 (Port 0 Bit 6 Match Value) = HIGH (P0.6 pin logic value is compared
     //     with logic HIGH.)
     // B7 (Port 0 Bit 7 Match Value) = HIGH (P0.7 pin logic value is compared
     //     with logic HIGH.)
     */
    P0MAT = P0MAT_B0__HIGH | P0MAT_B1__HIGH | P0MAT_B2__HIGH | P0MAT_B3__HIGH
            | P0MAT_B4__HIGH | P0MAT_B5__HIGH | P0MAT_B6__HIGH | P0MAT_B7__HIGH;
    // [P0MAT - Port 0 Match]$

}

extern void PORTS_1_enter_DefaultMode_from_RESET(void)
{
    // $[P1 - Port 1 Pin Latch]
    /*
     // B0 (Port 1 Bit 0 Latch) = HIGH (P1.0 is high. Set P1.0 to drive or
     //     float high.)
     // B1 (Port 1 Bit 1 Latch) = HIGH (P1.1 is high. Set P1.1 to drive or
     //     float high.)
     // B2 (Port 1 Bit 2 Latch) = HIGH (P1.2 is high. Set P1.2 to drive or
     //     float high.)
     // B3 (Port 1 Bit 3 Latch) = HIGH (P1.3 is high. Set P1.3 to drive or
     //     float high.)
     // B4 (Port 1 Bit 4 Latch) = HIGH (P1.4 is high. Set P1.4 to drive or
     //     float high.)
     // B5 (Port 1 Bit 5 Latch) = HIGH (P1.5 is high. Set P1.5 to drive or
     //     float high.)
     // B6 (Port 1 Bit 6 Latch) = HIGH (P1.6 is high. Set P1.6 to drive or
     //     float high.)
     */
    P1 = P1_B0__HIGH | P1_B1__HIGH | P1_B2__HIGH | P1_B3__HIGH | P1_B4__HIGH
            | P1_B5__HIGH | P1_B6__HIGH;
    // [P1 - Port 1 Pin Latch]$

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
     */
    P1MDOUT = P1MDOUT_B0__PUSH_PULL | P1MDOUT_B1__OPEN_DRAIN
            | P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN
            | P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__OPEN_DRAIN
            | P1MDOUT_B6__OPEN_DRAIN;
    // [P1MDOUT - Port 1 Output Mode]$

    // $[P1MDIN - Port 1 Input Mode]
    /*
     // B0 (Port 1 Bit 0 Input Mode) = DIGITAL (P1.0 pin is configured for
     //     digital mode.)
     // B1 (Port 1 Bit 1 Input Mode) = DIGITAL (P1.1 pin is configured for
     //     digital mode.)
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
     */
    P1MDIN = P1MDIN_B0__DIGITAL | P1MDIN_B1__DIGITAL | P1MDIN_B2__DIGITAL
            | P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__DIGITAL
            | P1MDIN_B6__DIGITAL;
    // [P1MDIN - Port 1 Input Mode]$

    // $[P1SKIP - Port 1 Skip]
    /*
     // B0 (Port 1 Bit 0 Skip) = SKIPPED (P1.0 pin is skipped by the
     //     crossbar.)
     // B1 (Port 1 Bit 1 Skip) = NOT_SKIPPED (P1.1 pin is not skipped by the
     //     crossbar.)
     // B2 (Port 1 Bit 2 Skip) = NOT_SKIPPED (P1.2 pin is not skipped by the
     //     crossbar.)
     // B3 (Port 1 Bit 3 Skip) = NOT_SKIPPED (P1.3 pin is not skipped by the
     //     crossbar.)
     // B4 (Port 1 Bit 4 Skip) = NOT_SKIPPED (P1.4 pin is not skipped by the
     //     crossbar.)
     // B5 (Port 1 Bit 5 Skip) = NOT_SKIPPED (P1.5 pin is not skipped by the
     //     crossbar.)
     // B6 (Port 1 Bit 6 Skip) = NOT_SKIPPED (P1.6 pin is not skipped by the
     //     crossbar.)
     */
    P1SKIP = P1SKIP_B0__SKIPPED | P1SKIP_B1__NOT_SKIPPED
            | P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED
            | P1SKIP_B4__NOT_SKIPPED | P1SKIP_B5__NOT_SKIPPED
            | P1SKIP_B6__NOT_SKIPPED;
    // [P1SKIP - Port 1 Skip]$

    // $[P1MASK - Port 1 Mask]
    /*
     // B0 (Port 1 Bit 0 Mask Value) = IGNORED (P1.0 pin logic value is
     //     ignored and will not cause a port mismatch event.)
     // B1 (Port 1 Bit 1 Mask Value) = IGNORED (P1.1 pin logic value is
     //     ignored and will not cause a port mismatch event.)
     // B2 (Port 1 Bit 2 Mask Value) = IGNORED (P1.2 pin logic value is
     //     ignored and will not cause a port mismatch event.)
     // B3 (Port 1 Bit 3 Mask Value) = IGNORED (P1.3 pin logic value is
     //     ignored and will not cause a port mismatch event.)
     // B4 (Port 1 Bit 4 Mask Value) = IGNORED (P1.4 pin logic value is
     //     ignored and will not cause a port mismatch event.)
     // B5 (Port 1 Bit 5 Mask Value) = IGNORED (P1.5 pin logic value is
     //     ignored and will not cause a port mismatch event.)
     // B6 (Port 1 Bit 6 Mask Value) = IGNORED (P1.6 pin logic value is
     //     ignored and will not cause a port mismatch event.)
     */
    P1MASK = P1MASK_B0__IGNORED | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
            | P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
            | P1MASK_B6__IGNORED;
    // [P1MASK - Port 1 Mask]$

    // $[P1MAT - Port 1 Match]
    /*
     // B0 (Port 1 Bit 0 Match Value) = HIGH (P1.0 pin logic value is compared
     //     with logic HIGH.)
     // B1 (Port 1 Bit 1 Match Value) = HIGH (P1.1 pin logic value is compared
     //     with logic HIGH.)
     // B2 (Port 1 Bit 2 Match Value) = HIGH (P1.2 pin logic value is compared
     //     with logic HIGH.)
     // B3 (Port 1 Bit 3 Match Value) = HIGH (P1.3 pin logic value is compared
     //     with logic HIGH.)
     // B4 (Port 1 Bit 4 Match Value) = HIGH (P1.4 pin logic value is compared
     //     with logic HIGH.)
     // B5 (Port 1 Bit 5 Match Value) = HIGH (P1.5 pin logic value is compared
     //     with logic HIGH.)
     // B6 (Port 1 Bit 6 Match Value) = HIGH (P1.6 pin logic value is compared
     //     with logic HIGH.)
     */
    P1MAT = P1MAT_B0__HIGH | P1MAT_B1__HIGH | P1MAT_B2__HIGH | P1MAT_B3__HIGH
            | P1MAT_B4__HIGH | P1MAT_B5__HIGH | P1MAT_B6__HIGH;
    // [P1MAT - Port 1 Match]$

}

extern void CLU_1_enter_DefaultMode_from_RESET(void)
{
    // $[CLU1FN - Configurable Logic Unit 1 Function Select]
    // [CLU1FN - Configurable Logic Unit 1 Function Select]$

    // $[CLU1MX - Configurable Logic Unit 1 Multiplexer]
    /*
     // MXA (CLU1 A Input Multiplexer Selection) = CLU1A1 (Select CLU1A.1)
     // MXB (CLU1 B Input Multiplexer Selection) = CLU1B1 (Select CLU1B.1)
     */
    SFRPAGE = 0x20;
    CLU1MX = CLU1MX_MXA__CLU1A1 | CLU1MX_MXB__CLU1B1;
    // [CLU1MX - Configurable Logic Unit 1 Multiplexer]$

    // $[CLU1CF - Configurable Logic Unit 1 Configuration]
    /*
     // OEN (CLU Port Output Enable) = ENABLE (Enables asynchronous output to
     //     the selected GPIO pin)
     // CLKSEL (CLU D flip-flop Clock Selection) = ALTCLK (The alternate clock
     //     signal CLU1ALTCLK.)
     */
    CLU1CF |= CLU1CF_OEN__ENABLE | CLU1CF_CLKSEL__ALTCLK0;
    // [CLU1CF - Configurable Logic Unit 1 Configuration]$

}

