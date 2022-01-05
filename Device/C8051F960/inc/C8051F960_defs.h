//-----------------------------------------------------------------------------
// C8051F960_defs.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Register/bit definitions for the C8051F96x family.
//
//
// Target:         C8051F96x
// Tool chain:     Keil, Raisonance, SDCC
// Command Line:   None
//
// Version 0.5 - 17 APR 2013 (BW)
//    -Corrected comments for SFR Page 0x0 SFR 0xB3 (PMU0MD)
// Version 0.4 - 20 AUG 2012 (TP)
//    -Added #define for _XPAGE to provide support for SDCC memory paging
//     (pdata)
// Version 0.5
//    - Corrections to ADC0PWR and ADC0TK - KB
// Version 0.3
//    - Initial Release - KB
//

//-----------------------------------------------------------------------------
// Bracketed to avoid multiple inclusion errors.
//-----------------------------------------------------------------------------

#ifndef C8051F960_DEFS_H
#define C8051F960_DEFS_H

//-----------------------------------------------------------------------------
// Conditional inclusion of compiler_defs.h to avoid inclusion errors.
//-----------------------------------------------------------------------------

#ifndef COMPILER_DEFS_H
#include "si_toolchain.h"
#endif

//-----------------------------------------------------------------------------
// Page 0x0 sfrs
//-----------------------------------------------------------------------------

SI_SFR(P0, 0x80);                        // Port 0 Latch
SI_SFR(SP, 0x81);                        // Stack Pointer
SI_SFR(DPL, 0x82);                       // Data Pointer Low
SI_SFR(DPH, 0x83);                       // Data Pointer High
SI_SFR(PSBANK, 0x84);                    // Flash Page Switch Bank SFR
SI_SFR(SFRNEXT, 0x85);                   // SFR Page Stack Next
SI_SFR(SFRLAST, 0x86);                   // SFR Page Stack Last
SI_SFR(PCON, 0x87);                      // Power Control
SI_SFR(TCON, 0x88);                      // Timer/Counter Control
SI_SFR(TMOD, 0x89);                      // Timer/Counter Mode
SI_SFR(TL0, 0x8A);                       // Timer/Counter 0 Low
SI_SFR(TL1, 0x8B);                       // Timer/Counter 1 Low
SI_SFR(TH0, 0x8C);                       // Timer/Counter 0 High
SI_SFR(TH1, 0x8D);                       // Timer/Counter 1 High
SI_SFR(CKCON, 0x8E);                     // Clock Control
SI_SFR(PSCTL, 0x8F);                     // Program Store R/W Control
SI_SFR(P1, 0x90);                        // Port 1 Latch
SI_SFR(TMR3CN, 0x91);                    // Timer/Counter 3 Control
SI_SFR(TMR3RLL, 0x92);                   // Timer/Counter 3 Reload Low
SI_SFR(TMR3RLH, 0x93);                   // Timer/Counter 3 Reload High
SI_SFR(TMR3L, 0x94);                     // Timer/Counter 3 Low
SI_SFR(TMR3H, 0x95);                     // Timer/Counter 3 High
SI_SFR(SCON0, 0x98);                     // UART0 Control
SI_SFR(SBUF0, 0x99);                     // UART0 Data Buffer
SI_SFR(CPT1CN, 0x9A);                    // Comparator1 Control
SI_SFR(CPT0CN, 0x9B);                    // Comparator0 Control
SI_SFR(CPT1MD, 0x9C);                    // Comparator1 Mode Selection
SI_SFR(CPT0MD, 0x9D);                    // Comparator0 Mode Selection
SI_SFR(CPT1MX, 0x9E);                    // Comparator1 Mux Selection
SI_SFR(CPT0MX, 0x9F);                    // Comparator0 Mux Selection
SI_SFR(P2, 0xA0);                        // Port 2 Latch
SI_SFR(SPI0CFG, 0xA1);                   // SPI0 Configuration
SI_SFR(SPI0CKR, 0xA2);                   // SPI0 Clock Rate Control
SI_SFR(SPI0DAT, 0xA3);                   // SPI0 Data
SI_SFR(P0MDOUT, 0xA4);                   // Port 0 Output Mode Configuration
SI_SFR(P1MDOUT, 0xA5);                   // Port 1 Output Mode Configuration
SI_SFR(P2MDOUT, 0xA6);                   // Port 2 Output Mode Configuration
SI_SFR(SFRPAGE, 0xA7);                   // SFR Page
SI_SFR(IE, 0xA8);                        // Interrupt Enable
SI_SFR(CLKSEL, 0xA9);                    // Clock Select
SI_SFR(EMI0CN, 0xAA);                    // EMIF Control
SI_SFR(EMI0CF, 0xAB);                    // EMIF Configuration
SI_SFR(RTC0ADR, 0xAC);                   // RTC0 Address
SI_SFR(RTC0DAT, 0xAD);                   // RTC0 Data
SI_SFR(RTC0KEY, 0xAE);                   // RTC0 Key
SI_SFR(EMI0TC, 0xAF);                    // EMIF Timing Control
SI_SFR(P3, 0xB0);                        // Port 3
SI_SFR(OSCXCN, 0xB1);                    // External Oscillator Control
SI_SFR(OSCICN, 0xB2);                    // Internal Oscillator Control
SI_SFR(PMU0MD, 0xB3);                    // Power Management Unit Mode
SI_SFR(PMU0CF, 0xB5);                    // PMU0 Configuration 0
SI_SFR(PMU0FL, 0xB6);                    // PMU0 flag
SI_SFR(FLKEY, 0xB7);                     // Flash Lock And Key
SI_SFR(IP, 0xB8);                        // Interrupt Priority
SI_SFR(IREF0CN, 0xB9);                   // Current Reference IREF0 Configuration
SI_SFR(ADC0AC, 0xBA);                    // ADC0 Accumulator Configuration
SI_SFR(ADC0MX, 0xBB);                    // ADC0 MUX
SI_SFR(ADC0CF, 0xBC);                    // ADC0 Configuration
SI_SFR(ADC0L, 0xBD);                     // ADC0 Low
SI_SFR(ADC0H, 0xBE);                     // ADC0 High
SI_SFR(P1MASK, 0xBF);                    // Port 1 Mask
SI_SFR(SMB0CN, 0xC0);                    // SMBus0 Control
SI_SFR(SMB0CF, 0xC1);                    // SMBus0 Configuration
SI_SFR(SMB0DAT, 0xC2);                   // SMBus0 Data
SI_SFR(ADC0GTL, 0xC3);                   // ADC0 Greater-Than Compare Low
SI_SFR(ADC0GTH, 0xC4);                   // ADC0 Greater-Than Compare High
SI_SFR(ADC0LTL, 0xC5);                   // ADC0 Less-Than Compare Word Low
SI_SFR(ADC0LTH, 0xC6);                   // ADC0 Less-Than Compare Word High
SI_SFR(P0MASK, 0xC7);                    // Port 0 Mask
SI_SFR(TMR2CN, 0xC8);                    // Timer/Counter 2 Control
SI_SFR(REG0CN, 0xC9);                    // Voltage Regulator (REG0) Control
SI_SFR(TMR2RLL, 0xCA);                   // Timer/Counter 2 Reload Low
SI_SFR(TMR2RLH, 0xCB);                   // Timer/Counter 2 Reload High
SI_SFR(TMR2L, 0xCC);                     // Timer/Counter 2 Low
SI_SFR(TMR2H, 0xCD);                     // Timer/Counter 2 High
SI_SFR(PCA0CPM5, 0xCE);                  // PCA0 Module 5 Mode Register
SI_SFR(P1MAT, 0xCF);                     // Port 1 Match
SI_SFR(PSW, 0xD0);                       // Program Status Word
SI_SFR(REF0CN, 0xD1);                    // Voltage Reference Control
SI_SFR(PCA0CPL5, 0xD2);                  // PCA0 Capture 5 Low
SI_SFR(PCA0CPH5, 0xD3);                  // PCA0 Capture 5 High
SI_SFR(P0SKIP, 0xD4);                    // Port 0 Skip
SI_SFR(P1SKIP, 0xD5);                    // Port 1 Skip
SI_SFR(P2SKIP, 0xD6);                    // Port 2 Skip
SI_SFR(P0MAT, 0xD7);                     // Port 0 Match
SI_SFR(PCA0CN, 0xD8);                    // PCA0 Control
SI_SFR(PCA0MD, 0xD9);                    // PCA0 Mode
SI_SFR(PCA0CPM0, 0xDA);                  // PCA0 Module 0 Mode Register
SI_SFR(PCA0CPM1, 0xDB);                  // PCA0 Module 1 Mode Register
SI_SFR(PCA0CPM2, 0xDC);                  // PCA0 Module 2 Mode Register
SI_SFR(PCA0CPM3, 0xDD);                  // PCA0 Module 3 Mode Register
SI_SFR(PCA0CPM4, 0xDE);                  // PCA0 Module 4 Mode Register
SI_SFR(PCA0PWM, 0xDF);                   // PCA0 PWM Configuration
SI_SFR(ACC, 0xE0);                       // Accumulator
SI_SFR(XBR0, 0xE1);                      // Port I/O Crossbar Control 0
SI_SFR(XBR1, 0xE2);                      // Port I/O Crossbar Control 1
SI_SFR(XBR2, 0xE3);                      // Port I/O Crossbar Control 2
SI_SFR(IT01CF, 0xE4);                    // INT0/INT1 Configuration
SI_SFR(FLWR, 0xE5);                      // Flash Write Only Register
SI_SFR(EIE1, 0xE6);                      // Extended Interrupt Enable 1
SI_SFR(EIE2, 0xE7);                      // Extended Interrupt Enable 2
SI_SFR(ADC0CN, 0xE8);                    // ADC0 Control
SI_SFR(PCA0CPL1, 0xE9);                  // PCA0 Capture 1 Low
SI_SFR(PCA0CPH1, 0xEA);                  // PCA0 Capture 1 High
SI_SFR(PCA0CPL2, 0xEB);                  // PCA0 Capture 2 Low
SI_SFR(PCA0CPH2, 0xEC);                  // PCA0 Capture 2 High
SI_SFR(PCA0CPL3, 0xED);                  // PCA0 Capture 3 Low
SI_SFR(PCA0CPH3, 0xEE);                  // PCA0 Capture 3 High
SI_SFR(RSTSRC, 0xEF);                    // Reset Source Configuration/Status
SI_SFR(B, 0xF0);                         // B Register
SI_SFR(P0MDIN, 0xF1);                    // Port 0 Input Mode Configuration
SI_SFR(P1MDIN, 0xF2);                    // Port 1 Input Mode Configuration
SI_SFR(P2MDIN, 0xF3);                    // Port 2 Input Mode Configuration
SI_SFR(SMB0ADR, 0xF4);                   // SMBus Slave Address
SI_SFR(SMB0ADM, 0xF5);                   // SMBus Slave Address Mask
SI_SFR(EIP1, 0xF6);                      // Extended Interrupt Priority 1
SI_SFR(EIP2, 0xF7);                      // Extended Interrupt Priority 2
SI_SFR(SPI0CN, 0xF8);                    // SPI0 Control
SI_SFR(PCA0L, 0xF9);                     // PCA0 Counter Low
SI_SFR(PCA0H, 0xFA);                     // PCA0 Counter High
SI_SFR(PCA0CPL0, 0xFB);                  // PCA0 Capture 0 Low
SI_SFR(PCA0CPH0, 0xFC);                  // PCA0 Capture 0 High
SI_SFR(PCA0CPL4, 0xFD);                  // PCA0 Capture 4 Low
SI_SFR(PCA0CPH4, 0xFE);                  // PCA0 Capture 4 High
SI_SFR(VDM0CN, 0xFF);                    // VDD Monitor Control

//-----------------------------------------------------------------------------
// Page 0x2 sfrs
//-----------------------------------------------------------------------------

SI_SFR(LCD0D0, 0x89);                    // LCD0 Data 0
SI_SFR(LCD0D1, 0x8A);                    // LCD0 Data 1
SI_SFR(LCD0D2, 0x8B);                    // LCD0 Data 2
SI_SFR(LCD0D3, 0x8C);                    // LCD0 Data 3
SI_SFR(LCD0D4, 0x8D);                    // LCD0 Data 4
SI_SFR(LCD0D5, 0x8E);                    // LCD0 Data 5
SI_SFR(LCD0D6, 0x91);                    // LCD0 Data 6
SI_SFR(LCD0D7, 0x92);                    // LCD0 Data 7
SI_SFR(LCD0D8, 0x93);                    // LCD0 Data 8
SI_SFR(LCD0D9, 0x94);                    // LCD0 Data 9
SI_SFR(LCD0DA, 0x95);                    // LCD0 Data A
SI_SFR(LCD0DB, 0x96);                    // LCD0 Data B
SI_SFR(LCD0DC, 0x97);                    // LCD0 Data C
SI_SFR(LCD0DD, 0x99);                    // LCD0 Data D
SI_SFR(LCD0DE, 0x9A);                    // LCD0 Data E
SI_SFR(LCD0DF, 0x9B);                    // LCD0 Data F
SI_SFR(LCD0CNTRST, 0x9C);                // LCD0 Contrast
SI_SFR(LCD0CN, 0x9D);                    // LCD0 Control
SI_SFR(LCD0BLINK, 0x9E);                 // LCD0 Blink Mask
SI_SFR(LCD0TOGR, 0x9F);                  // LCD0 Toggle Rate
SI_SFR(SPI1CFG, 0xA1);                   // SPI1 Configuration
SI_SFR(SPI1CKR, 0xA2);                   // SPI1 Clock Rate Control
SI_SFR(SPI1DAT, 0xA3);                   // SPI1 Data
SI_SFR(LCD0PWR, 0xA4);                   // LCD0 Power
SI_SFR(LCD0CF, 0xA5);                    // LCD0 Configuration
SI_SFR(LCD0VBMCN, 0xA6);                 // LCD0 VBAT Monitor Control
SI_SFR(LCD0CLKDIVL, 0xA9);               // LCD0 Clock Divider Low
SI_SFR(LCD0CLKDIVH, 0xAA);               // LCD0 Clock Divider High
SI_SFR(LCD0MSCN, 0xAB);                  // LCD0 Master Control
SI_SFR(LCD0MSCF, 0xAC);                  // LCD0 Master Configuration
SI_SFR(LCD0CHPCF, 0xAD);                 // LCD0 Charge Pump Configuration
SI_SFR(LCD0CHPMD, 0xAE);                 // LCD0 Charge Pump Mode
SI_SFR(LCD0VBMCF, 0xAF);                 // LCD0 VBAT Monitor Configuration
SI_SFR(DC0CN, 0xB1);                     // DC0 Control
SI_SFR(DC0CF, 0xB2);                     // DC0 Configuration
SI_SFR(DC0MD, 0xB3);                     // DC0 Mode
SI_SFR(LCD0CHPCN, 0xB5);                 // LCD0 Charge Pump Control
SI_SFR(LCD0BUFMD, 0xB6);                 // LCD0 Buffer Mode
SI_SFR(CRC1IN, 0xB9);                    // CRC1 In
SI_SFR(CRC1OUTL, 0xBA);                  // CRC1 Out Low
SI_SFR(CRC1OUTH, 0xBB);                  // CRC1 Out High
SI_SFR(CRC1POLL, 0xBC);                  // CRC1 Polynomial Low
SI_SFR(CRC1POLH, 0xBD);                  // CRC1 Polynomial High
SI_SFR(CRC1CN, 0xBE);                    // CRC1 Control
SI_SFR(PC0STAT, 0xC1);                   // PC0 Status
SI_SFR(ENC0L, 0xC2);                     // ENC0 Low
SI_SFR(ENC0M, 0xC3);                     // ENC0 Middle
SI_SFR(ENC0H, 0xC4);                     // ENC0 High
SI_SFR(ENC0CN, 0xC5);                    // ENC0 Control
SI_SFR(VREGINSDL, 0xC6);                 // VREGIN Switch Delay Low
SI_SFR(VREGINSDH, 0xC7);                 // VREGIN Switch Delay High
SI_SFR(DMA0NCF, 0xC9);                   // DMA0 Configuration
SI_SFR(DMA0NBAL, 0xCA);                  // DMA0 Base Address Low (Selected Channel)
SI_SFR(DMA0NBAH, 0xCB);                  // DMA0 Base Address High (Selected Channel)
SI_SFR(DMA0NAOL, 0xCC);                  // DMA0 Address Offset Low (Selected Channel)
SI_SFR(DMA0NAOH, 0xCD);                  // DMA0 Address Offset High (Selected Channel)
SI_SFR(DMA0NSZL, 0xCE);                  // DMA0 Size Low (Selected Channel)
SI_SFR(DMA0NSZH, 0xCF);                  // DMA0 Size High (Selected Channel)
SI_SFR(DMA0SEL, 0xD1);                   // DMA0 Channel Select
SI_SFR(DMA0EN, 0xD2);                    // DMA0 Enable
SI_SFR(DMA0INT, 0xD3);                   // DMA0 Interrupt
SI_SFR(DMA0MINT, 0xD4);                  // DMA0 Middle Interrupt
SI_SFR(DMA0BUSY, 0xD5);                  // DMA0 Busy
SI_SFR(DMA0NMD, 0xD6);                   // DMA0 Mode (Selected Channel)
SI_SFR(PC0PCF, 0xD7);                    // PC0 Pull-up Configuration
SI_SFR(PC0MD, 0xD9);                     // PC0 Mode
SI_SFR(PC0CTR0L, 0xDA);                  // PC0 Counter 0 Low
SI_SFR(PC0CTR0M, 0xDB);                  // PC0 Counter 0 Middle
SI_SFR(PC0CTR0H, 0xDC);                  // PC0 Counter 0 High
SI_SFR(PC0CTR1L, 0xDD);                  // PC0 Counter 1 Low
SI_SFR(PC0CTR1M, 0xDE);                  // PC0 Counter 1 Middle
SI_SFR(PC0CTR1H, 0xDF);                  // PC0 Counter 1 High
SI_SFR(PC0CMP0L, 0xE1);                  // PC0 Comparator 0 Low
SI_SFR(PC0CMP0M, 0xE2);                  // PC0 Comparator 0 Middle
SI_SFR(PC0CMP0H, 0xE3);                  // PC0 Comparator 0 High
SI_SFR(PC0TH, 0xE4);                     // PC0 Threshold
SI_SFR(AES0BCFG, 0xE9);                  // AES0 Block Configuration
SI_SFR(AES0DCFG, 0xEA);                  // AES0 Data Configuration
SI_SFR(AES0BIN, 0xEB);                   // AES0 Block Input
SI_SFR(AES0XIN, 0xEC);                   // AES0 XOR Input
SI_SFR(AES0KIN, 0xED);                   // AES0 Key Input
SI_SFR(AES0DBA, 0xEE);                   // AES0 Data Byte Address
SI_SFR(AES0KBA, 0xEF);                   // AES0 Key Byte Address
SI_SFR(PC0CMP1L, 0xF1);                  // PC0 Comparator 1 Low
SI_SFR(PC0CMP1M, 0xF2);                  // PC0 Comparator 1 Middle
SI_SFR(PC0CMP1H, 0xF3);                  // PC0 Comparator 1 High
SI_SFR(PC0HIST, 0xF4);                   // PC0 History
SI_SFR(AES0YOUT, 0xF5);                  // AES Y Out
SI_SFR(SPI1CN, 0xF8);                    // SPI1 Control
SI_SFR(PC0DCL, 0xF9);                    // PC0 Debounce Configuration Low
SI_SFR(PC0DCH, 0xFA);                    // PC0 Debounce Configuration high
SI_SFR(PC0INT0, 0xFB);                   // PC0 Interrupt 0
SI_SFR(PC0INT1, 0xFC);                   // PC0 Interrupt 1
SI_SFR(DC0RDY, 0xFD);                    // DC0 Ready

//-----------------------------------------------------------------------------
// Page 0xF sfrs
//-----------------------------------------------------------------------------

SI_SFR(SFRPGCN, 0x8E);                   // SFR Page Control
SI_SFR(CRC0DAT, 0x91);                   // CRC0 Data
SI_SFR(CRC0CN, 0x92);                    // CRC0 Control
SI_SFR(CRC0IN, 0x93);                    // CRC0 Input
SI_SFR(CRC0FLIP, 0x94);                  // CRC0 Flip
SI_SFR(CRC0AUTO, 0x96);                  // CRC0 Automatic Control
SI_SFR(CRC0CNT, 0x97);                   // CRC0 Automatic Flash Sector Count
SI_SFR(LCD0BUFCN, 0x9C);                 // LCD0 Buffer Control
SI_SFR(P3DRV, 0xA1);                     // Port 3 Drive Strength
SI_SFR(P4DRV, 0xA2);                     // Port 4 Drive Strength
SI_SFR(P5DRV, 0xA3);                     // Port 5 Drive Strength
SI_SFR(P0DRV, 0xA4);                     // Port 0 Drive Strength
SI_SFR(P1DRV, 0xA5);                     // Port 1 Drive Strength
SI_SFR(P2DRV, 0xA6);                     // Port 2 Drive Strength
SI_SFR(P6DRV, 0xAA);                     // Port 6 Drive Strength
SI_SFR(P7DRV, 0xAB);                     // Port 7 Drive Strength
SI_SFR(LCD0BUFCF, 0xAC);                 // LCD0 Buffer Configuration
SI_SFR(P3MDOUT, 0xB1);                   // P3 Mode Out
SI_SFR(OSCIFL, 0xB2);                    // Internal Oscillator Flash
SI_SFR(OSCICL, 0xB3);                    // Internal Oscillator Calibration
SI_SFR(FLSCL, 0xB6);                     // Flash Scale Register
SI_SFR(IREF0CF, 0xB9);                   // Current Reference IREF0 Configuration
SI_SFR(ADC0PWR, 0xBA);                   // ADC0 Burst Mode Power-Up Time
SI_SFR(ADC0TK, 0xBB);                    // ADC0 Tracking Control
SI_SFR(TOFFL, 0xBD);                     // Temperature Offset Low
SI_SFR(TOFFH, 0xBE);                     // Temperature Offset High
SI_SFR(P4, 0xD9);                        // Port 4 Latch
SI_SFR(P5, 0xDA);                        // Port 4 Latch
SI_SFR(P6, 0xDB);                        // Port 4 Latch
SI_SFR(P7, 0xDC);                        // Port 4 Latch
SI_SFR(HWID, 0xE9);                      // Hardware ID
SI_SFR(REVID, 0xEA);                     // Revision ID
SI_SFR(DEVICEID, 0xEB);                  // Device ID
SI_SFR(P3MDIN, 0xF1);                    // Port 3 Input Mode Configuration
SI_SFR(P4MDIN, 0xF2);                    // Port 4 Input Mode Configuration
SI_SFR(P5MDIN, 0xF3);                    // Port 5 Input Mode Configuration
SI_SFR(P6MDIN, 0xF4);                    // Port 6 Input Mode Configuration
SI_SFR(PCLKACT, 0xF5);                   // Peripheral Clock Enable Active Mode
SI_SFR(P4MDOUT, 0xF9);                   // P4 Mode Out
SI_SFR(P5MDOUT, 0xFA);                   // P5 Mode Out
SI_SFR(P6MDOUT, 0xFB);                   // P6 Mode Out
SI_SFR(P7MDOUT, 0xFC);                   // P7 Mode Out
SI_SFR(CLKMODE, 0xFD);                   // Clock Mode
SI_SFR(PCLKEN, 0xFE);                    // Peripheral Clock Enables (LP Idle)

//-----------------------------------------------------------------------------
// 16-bit Register Definitions (might not be supported by all compilers)
//-----------------------------------------------------------------------------

SI_SFR16(DP, 0x82);                      // Data Pointer
SI_SFR16(TOFF, 0x85);                    // Temperature Sensor Offset
SI_SFR16(TMR3RL, 0x92);                  // Timer 3 Reload
SI_SFR16(TMR3, 0x94);                    // Timer 3 Counter
SI_SFR16(ADC0, 0xBD);                    // ADC0 Data
SI_SFR16(ADC0GT, 0xC3);                  // ADC0 Greater-Than Compare
SI_SFR16(ADC0LT, 0xC5);                  // ADC0 Less-Than Compare
SI_SFR16(TMR2RL, 0xCA);                  // Timer 2 Reload
SI_SFR16(TMR2, 0xCC);                    // Timer 2 Counter
SI_SFR16(PCA0CP5, 0xD2);                 // PCA0 Module 5 Capture/Compare
SI_SFR16(PCA0CP1, 0xE9);                 // PCA0 Module 1 Capture/Compare
SI_SFR16(PCA0CP2, 0xEB);                 // PCA0 Module 2 Capture/Compare
SI_SFR16(PCA0CP3, 0xED);                 // PCA0 Module 3 Capture/Compare
SI_SFR16(PCA0, 0xF9);                    // PCA0 Counter
SI_SFR16(PCA0CP0, 0xFB);                 // PCA0 Module 0 Capture/Compare
SI_SFR16(PCA0CP4, 0xFD);                 // PCA0 Module 4 Capture/Compare

//-----------------------------------------------------------------------------
// Indirect RTC Register Addresses
//-----------------------------------------------------------------------------

#define CAPTURE0  0x00                 // RTC address of CAPTURE0 register
#define CAPTURE1  0x01                 // RTC address of CAPTURE1 register
#define CAPTURE2  0x02                 // RTC address of CAPTURE2 register
#define CAPTURE3  0x03                 // RTC address of CAPTURE3 register
#define RTC0CN    0x04                 // RTC address of RTC0CN register
#define RTC0XCN   0x05                 // RTC address of RTC0XCN register
#define RTC0XCF   0x06                 // RTC address of RTC0XCF register
#define RTC0CF    0x07                 // RTC address of RTC0CF register
#define ALARM0B0  0x08                 // RTC address of ALARM0 Byte 0
#define ALARM0B1  0x09                 // RTC address of ALARM0 Byte 1
#define ALARM0B2  0x0A                 // RTC address of ALARM0 Byte 2
#define ALARM0B3  0x0B                 // RTC address of ALARM0 Byte 3
#define ALARM1B0  0x0C                 // RTC address of ALARM1 Byte 0
#define ALARM1B1  0x0D                 // RTC address of ALARM1 Byte 1
#define ALARM1B2  0x0E                 // RTC address of ALARM1 Byte 2
#define ALARM1B3  0x0F                 // RTC address of ALARM1 Byte 3
#define ALARM2B0  0x10                 // RTC address of ALARM2 Byte 0
#define ALARM2B1  0x11                 // RTC address of ALARM2 Byte 1
#define ALARM2B2  0x12                 // RTC address of ALARM2 Byte 2
#define ALARM2B3  0x13                 // RTC address of ALARM2 Byte 3

//-----------------------------------------------------------------------------
// Address Definitions for Bit-addressable Registers
//-----------------------------------------------------------------------------

#define SFR_P0       0x80
#define SFR_TCON     0x88
#define SFR_P1       0x90
#define SFR_SCON0    0x98
#define SFR_P2       0xA0
#define SFR_IE       0xA8
#define SFR_P3       0xB0
#define SFR_IP       0xB8
#define SFR_SMB0CN   0xC0
#define SFR_TMR2CN   0xC8
#define SFR_PSW      0xD0
#define SFR_PCA0CN   0xD8
#define SFR_ACC      0xE0
#define SFR_ADC0CN   0xE8
#define SFR_B        0xF0
#define SFR_SPI0CN   0xF8
#define SFR_SPI1CN   0xF8

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

// TCON 0x88
SI_SBIT(TF1, SFR_TCON, 7);               // Timer 1 Overflow Flag
SI_SBIT(TR1, SFR_TCON, 6);               // Timer 1 On/Off Control
SI_SBIT(TF0, SFR_TCON, 5);               // Timer 0 Overflow Flag
SI_SBIT(TR0, SFR_TCON, 4);               // Timer 0 On/Off Control
SI_SBIT(IE1, SFR_TCON, 3);               // External Interrupt 1 Edge Flag
SI_SBIT(IT1, SFR_TCON, 2);               // External Interrupt 1 Type
SI_SBIT(IE0, SFR_TCON, 1);               // External Interrupt 0 Edge Flag
SI_SBIT(IT0, SFR_TCON, 0);               // External Interrupt 0 Type

// SCON0 0x98
SI_SBIT(S0MODE, SFR_SCON0, 7);           // UART0 Mode
                                       // Bit6 UNUSED
SI_SBIT(MCE0, SFR_SCON0, 5);             // UART0 MCE
SI_SBIT(REN0, SFR_SCON0, 4);             // UART0 RX Enable
SI_SBIT(TB80, SFR_SCON0, 3);             // UART0 TX Bit 8
SI_SBIT(RB80, SFR_SCON0, 2);             // UART0 RX Bit 8
SI_SBIT(TI0, SFR_SCON0, 1);              // UART0 TX Interrupt Flag
SI_SBIT(RI0, SFR_SCON0, 0);              // UART0 RX Interrupt Flag

// IE 0xA8
SI_SBIT(EA, SFR_IE, 7);                  // Global Interrupt Enable
SI_SBIT(ESPI0, SFR_IE, 6);               // SPI0 Interrupt Enable
SI_SBIT(ET2, SFR_IE, 5);                 // Timer 2 Interrupt Enable
SI_SBIT(ES0, SFR_IE, 4);                 // UART0 Interrupt Enable
SI_SBIT(ET1, SFR_IE, 3);                 // Timer 1 Interrupt Enable
SI_SBIT(EX1, SFR_IE, 2);                 // External Interrupt 1 Enable
SI_SBIT(ET0, SFR_IE, 1);                 // Timer 0 Interrupt Enable
SI_SBIT(EX0, SFR_IE, 0);                 // External Interrupt 0 Enable


// IP 0xB8
                                       // Bit7 UNUSED
SI_SBIT(PSPI0, SFR_IP, 6);               // SPI0 Priority
SI_SBIT(PT2, SFR_IP, 5);                 // Timer 2 Priority
SI_SBIT(PS0, SFR_IP, 4);                 // UART0 Priority
SI_SBIT(PT1, SFR_IP, 3);                 // Timer 1 Priority
SI_SBIT(PX1, SFR_IP, 2);                 // External Interrupt 1 Priority
SI_SBIT(PT0, SFR_IP, 1);                 // Timer 0 Priority
SI_SBIT(PX0, SFR_IP, 0);                 // External Interrupt 0 Priority

// SMB0CN 0xC0
SI_SBIT(MASTER, SFR_SMB0CN, 7);          // SMBus0 Master/Slave
SI_SBIT(TXMODE, SFR_SMB0CN, 6);          // SMBus0 Transmit Mode
SI_SBIT(STA, SFR_SMB0CN, 5);             // SMBus0 Start Flag
SI_SBIT(STO, SFR_SMB0CN, 4);             // SMBus0 Stop Flag
SI_SBIT(ACKRQ, SFR_SMB0CN, 3);           // SMBus0 Acknowledge Request
SI_SBIT(ARBLOST, SFR_SMB0CN, 2);         // SMBus0 Arbitration Lost
SI_SBIT(ACK, SFR_SMB0CN, 1);             // SMBus0 Acknowledge Flag
SI_SBIT(SI, SFR_SMB0CN, 0);              // SMBus0 Interrupt Pending Flag

// TMR2CN 0xC8
SI_SBIT(TF2H, SFR_TMR2CN, 7);            // Timer 2 High Byte Overflow Flag
SI_SBIT(TF2L, SFR_TMR2CN, 6);            // Timer 2 Low Byte Overflow Flag
SI_SBIT(TF2LEN, SFR_TMR2CN, 5);          // Timer 2 Low Byte Interrupt Enable
SI_SBIT(TF2CEN, SFR_TMR2CN, 4);          // Timer 2 LFO Capture Enable
SI_SBIT(T2SPLIT, SFR_TMR2CN, 3);         // Timer 2 Split Mode Enable
SI_SBIT(TR2, SFR_TMR2CN, 2);             // Timer 2 On/Off Control
SI_SBIT(T2RCLK, SFR_TMR2CN, 1);          // Timer 2 Capture Mode
SI_SBIT(T2XCLK, SFR_TMR2CN, 0);          // Timer 2 External Clock Select

// PSW 0xD0
SI_SBIT(CY, SFR_PSW, 7);                 // Carry Flag
SI_SBIT(AC, SFR_PSW, 6);                 // Auxiliary Carry Flag
SI_SBIT(F0, SFR_PSW, 5);                 // User Flag 0
SI_SBIT(RS1, SFR_PSW, 4);                // Register Bank Select 1
SI_SBIT(RS0, SFR_PSW, 3);                // Register Bank Select 0
SI_SBIT(OV, SFR_PSW, 2);                 // Overflow Flag
SI_SBIT(F1, SFR_PSW, 1);                 // User Flag 1
SI_SBIT(P, SFR_PSW, 0);                  // Accumulator Parity Flag

// PCA0CN 0xD8
SI_SBIT(CF, SFR_PCA0CN, 7);              // PCA0 Counter Overflow Flag
SI_SBIT(CR, SFR_PCA0CN, 6);              // PCA0 Counter Run Control Bit
SI_SBIT(CCF5, SFR_PCA0CN, 5);            // PCA0 Module 5 Interrupt Flag
SI_SBIT(CCF4, SFR_PCA0CN, 4);            // PCA0 Module 4 Interrupt Flag
SI_SBIT(CCF3, SFR_PCA0CN, 3);            // PCA0 Module 3 Interrupt Flag
SI_SBIT(CCF2, SFR_PCA0CN, 2);            // PCA0 Module 2 Interrupt Flag
SI_SBIT(CCF1, SFR_PCA0CN, 1);            // PCA0 Module 1 Interrupt Flag
SI_SBIT(CCF0, SFR_PCA0CN, 0);            // PCA0 Module 0 Interrupt Flag

// ADC0CN 0xE8
SI_SBIT(AD0EN, SFR_ADC0CN, 7);           // ADC0 Enable
SI_SBIT(BURSTEN, SFR_ADC0CN, 6);         // ADC0 Burst Enable
SI_SBIT(AD0INT, SFR_ADC0CN, 5);          // ADC0 EOC Interrupt Flag
SI_SBIT(AD0BUSY, SFR_ADC0CN, 4);         // ADC0 Busy Flag
SI_SBIT(AD0WINT, SFR_ADC0CN, 3);         // ADC0 Window Interrupt Flag
SI_SBIT(AD0CM2, SFR_ADC0CN, 2);          // ADC0 Convert Start Mode Bit 2
SI_SBIT(AD0CM1, SFR_ADC0CN, 1);          // ADC0 Convert Start Mode Bit 1
SI_SBIT(AD0CM0, SFR_ADC0CN, 0);          // ADC0 Convert Start Mode Bit 0

// SPI0CN 0xF8
SI_SBIT(SPIF0, SFR_SPI0CN, 7);           // SPI0 Interrupt Flag
SI_SBIT(WCOL0, SFR_SPI0CN, 6);           // SPI0 Write Collision Flag
SI_SBIT(MODF0, SFR_SPI0CN, 5);           // SPI0 Mode Fault Flag
SI_SBIT(RXOVRN0, SFR_SPI0CN, 4);         // SPI0 RX Overrun Flag
SI_SBIT(NSS0MD1, SFR_SPI0CN, 3);         // SPI0 Slave Select Mode 1
SI_SBIT(NSS0MD0, SFR_SPI0CN, 2);         // SPI0 Slave Select Mode 0
SI_SBIT(TXBMT0, SFR_SPI0CN, 1);          // SPI0 TX Buffer Empty Flag
SI_SBIT(SPI0EN, SFR_SPI0CN, 0);          // SPI0 Enable

// SPI1CN 0xF8
SI_SBIT(SPIF1, SFR_SPI1CN, 7);           // SPI1 Interrupt Flag
SI_SBIT(WCOL1, SFR_SPI1CN, 6);           // SPI1 Write Collision Flag
SI_SBIT(MODF1, SFR_SPI1CN, 5);           // SPI1 Mode Fault Flag
SI_SBIT(RXOVRN1, SFR_SPI1CN, 4);         // SPI1 RX Overrun Flag
SI_SBIT(NSS1MD1, SFR_SPI1CN, 3);         // SPI1 Slave Select Mode 1
SI_SBIT(NSS1MD0, SFR_SPI1CN, 2);         // SPI1 Slave Select Mode 0
SI_SBIT(TXBMT1, SFR_SPI1CN, 1);          // SPI1 TX Buffer Empty Flag
SI_SBIT(SPI1EN, SFR_SPI1CN, 0);          // SPI1 Enable

//-----------------------------------------------------------------------------
// Interrupt Priorities
//-----------------------------------------------------------------------------

#define INTERRUPT_INT0             0   // External Interrupt 0
#define INTERRUPT_TIMER0           1   // Timer0 Overflow
#define INTERRUPT_INT1             2   // External Interrupt 1
#define INTERRUPT_TIMER1           3   // Timer1 Overflow
#define INTERRUPT_UART0            4   // Serial Port 0
#define INTERRUPT_TIMER2           5   // Timer2 Overflow
#define INTERRUPT_SPI0             6   // Serial Peripheral Interface 0
#define INTERRUPT_SMBUS0           7   // SMBus0 Interface
#define INTERRUPT_RTC0ALARM        8   // RTC0 (SmaRTClock) Alarm
#define INTERRUPT_ADC0_WINDOW      9   // ADC0 Window Comparison
#define INTERRUPT_ADC0_EOC         10  // ADC0 End Of Conversion
#define INTERRUPT_PCA0             11  // PCA0 Peripheral
#define INTERRUPT_COMPARATOR0      12  // Comparator0
#define INTERRUPT_COMPARATOR1      13  // Comparator1
#define INTERRUPT_TIMER3           14  // Timer3 Overflow
#define INTERRUPT_VDDMON           15  // VDD Monitor Early Warning
#define INTERRUPT_PORT_MATCH       16  // Port Match
#define INTERRUPT_RTC0_OSC_FAIL    17  // RTC0 (smaRTClock) Oscillator Fail
#define INTERRUPT_SPI1             18  // Serial Peripheral Interface 1
#define INTERRUPT_PC0              19  // Pulse Counter 0
#define INTERRUPT_DMA0             20  // Direct Memory Access 0
#define INTERRUPT_ENC0             21  // Encoder/Decoder 0
#define INTERRUPT_AES0             22  // Advanced Encryption Standard 0

//-----------------------------------------------------------------------------
// SFR Page Definitions
//-----------------------------------------------------------------------------

#define LEGACY_PAGE       0x00         // LEGACY SFR PAGE
#define DPPE_PAGE         0x02         // DPPE SFR PAGE
#define DMA0_PAGE         0x02         // DMA0 SFR PAGE
#define AES0_PAGE         0x02         // AES0 SFR PAGE
#define ENC0_PAGE         0x02         // ENC0 SFR PAGE
#define CRC1_PAGE         0x02         // CRC1 SFR PAGE
#define SPI1_PAGE         0x02         // SPI1 SFR PAGE
#define LCD0_PAGE         0x02         // LCD0 SFR PAGE
#define CONFIG_PAGE       0x0F         // SYSTEM AND PORT CONFIGURATION PAGE
#define CRC0_PAGE         0x0F         // CRC0
#define TOFF_PAGE         0x0F         // TEMPERATURE SENSOR OFFSET PAGE

//-----------------------------------------------------------------------------
// SDCC PDATA External Memory Paging Support
//-----------------------------------------------------------------------------

#if defined SDCC

SI_SFR(_XPAGE, 0xAA); // Point to the EMI0CN register

#endif

//-----------------------------------------------------------------------------
// Header File PreProcessor Directive
//-----------------------------------------------------------------------------

#endif                                 // #define C8051F960_DEFS_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------