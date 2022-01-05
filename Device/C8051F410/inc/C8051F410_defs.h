//-----------------------------------------------------------------------------
// C8051F410_defs.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Register/bit definitions for the C8051F41x family.
// **Important Note**: The compiler_defs.h header file should be included
// before including this header file.
//
// Target:         C8051F410, 'F411, 'F412, 'F413
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.4 - 20 AUG 2012 (TP)
//    -Added #define for _XPAGE to provide support for SDCC memory paging
//     (pdata)
// Release 1.3 - 22 JULY 2008 (ES)
//    -Deleted MAC0ACC2, MAC0ACC3 from byte register definitions
// Release 1.2 - 08 NOV 2007 (BW)
//    -Changed SFR16 for IDA1 from 0x96 to 0xF4
//    -Added line break after last line in file
// Release 1.1 - 07 AUG 2007 (PKC)
//    -Removed #include "si_toolchain.h". The C source file should include it.
//    -Removed FID and fixed formatting.
// Release 1.0 - 14 MAY 2007 (BW)
//    -Ported from 'F360 DEFS rev 1.1

//-----------------------------------------------------------------------------
// Header File Preprocessor Directive
//-----------------------------------------------------------------------------

#ifndef C8051F410_DEFS_H
#define C8051F410_DEFS_H

//-----------------------------------------------------------------------------
// Byte Registers
//-----------------------------------------------------------------------------

SI_SFR(P0, 0x80);                        // Port 0 Latch
SI_SFR(SP, 0x81);                        // Stack Pointer
SI_SFR(DPL, 0x82);                       // Data Pointer Low
SI_SFR(DPH, 0x83);                       // Data Pointer High
SI_SFR(CRC0CN, 0x84);                    // CRC0 Control
SI_SFR(CRC0IN, 0x85);                    // CRC0 Input
SI_SFR(CRC0DAT, 0x86);                   // CRC0 Data
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
SI_SFR(IDA0L, 0x96);                     // Current Mode Dac0 Low
SI_SFR(IDA0H, 0x97);                     // Current Mode Dac0 High
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
SI_SFR(IE, 0xA8);                        // Interrupt Enable
SI_SFR(CLKSEL, 0xA9);                    // Clock Select
SI_SFR(EMI0CN, 0xAA);                    // External Memory Interface Control
SI_SFR(CLKMUL, 0xAB);                    // Clock Multiplier
SI_SFR(RTC0ADR, 0xAC);                   // RTC0 Address
SI_SFR(RTC0DAT, 0xAD);                   // RTC0 Data
SI_SFR(RTC0KEY, 0xAE);                   // RTC0 Key
SI_SFR(ONESHOT, 0xAF);                   // Flash Oneshot Control
SI_SFR(P0ODEN, 0xB0);                    // Port 0 Overdrive Enable
SI_SFR(OSCXCN, 0xB1);                    // External Oscillator Control
SI_SFR(OSCICN, 0xB2);                    // Internal Oscillator Control
SI_SFR(OSCICL, 0xB3);                    // Internal Oscillator Calibration
SI_SFR(IDA1CN, 0xB5);                    // Current Mode DAC1 Control
SI_SFR(FLSCL, 0xB6);                     // Flash Scale
SI_SFR(FLKEY, 0xB7);                     // Flash Lock And Key
SI_SFR(IP, 0xB8);                        // Interrupt Priority
SI_SFR(IDA0CN, 0xB9);                    // Current Mode DAC0 Control
SI_SFR(ADC0TK, 0xBA);                    // ADC0 Tracking Control
SI_SFR(ADC0MX, 0xBB);                    // AMUX0 Channel Select
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
SI_SFR(REG0CN, 0xC9);                    // Voltage Regulator Control
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
SI_SFR(CRC0FLIP, 0xDF);                  // CRC Flip
SI_SFR(ACC, 0xE0);                       // Accumulator
SI_SFR(XBR0, 0xE1);                      // Port I/O Crossbar Control 0
SI_SFR(XBR1, 0xE2);                      // Port I/O Crossbar Control 1
SI_SFR(PFE0CN, 0xE3);                    // Prefetch Control
SI_SFR(IT01CF, 0xE4);                    // INT0/INT1 Configuration
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
SI_SFR(IDA1L, 0xF4);                     // Current Mode Dac1 Low
SI_SFR(IDA1H, 0xF5);                     // Current Mode Dac1 High
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
// 16-bit Register Definitions (might not be supported by all compilers)
//-----------------------------------------------------------------------------

SI_SFR16(DP, 0x82);                      // Data Pointer
SI_SFR16(TMR3RL, 0x92);                  // Timer 3 Reload
SI_SFR16(TMR3, 0x94);                    // Timer 3 Counter
SI_SFR16(IDA0, 0x96);                    // IDAC0 Data
SI_SFR16(ADC0, 0xBD);                    // ADC0 Data
SI_SFR16(ADC0GT, 0xC3);                  // ADC0 Greater-Than Compare
SI_SFR16(ADC0LT, 0xC5);                  // ADC0 Less-Than Compare
SI_SFR16(TMR2RL, 0xCA);                  // Timer 2 Reload
SI_SFR16(TMR2, 0xCC);                    // Timer 2 Counter
SI_SFR16(PCA0CP5, 0xD2);                 // PCA0 Module 5 Capture/Compare
SI_SFR16(PCA0CP1, 0xE9);                 // PCA0 Module 1 Capture/Compare
SI_SFR16(PCA0CP2, 0xEB);                 // PCA0 Module 2 Capture/Compare
SI_SFR16(PCA0CP3, 0xED);                 // PCA0 Module 3 Capture/Compare
SI_SFR16(IDA1, 0xF4);                    // IDAC1 Data
SI_SFR16(PCA0, 0xF9);                    // PCA0 Counter
SI_SFR16(PCA0CP0, 0xFB);                 // PCA0 Module 0 Capture/Compare
SI_SFR16(PCA0CP4, 0xFD);                 // PCA0 Module 4 Capture/Compare

//-----------------------------------------------------------------------------
// Address Definitions for Bit-addressable Registers
//-----------------------------------------------------------------------------

#define SFR_P0       0x80
#define SFR_TCON     0x88
#define SFR_P1       0x90
#define SFR_SCON0    0x98
#define SFR_P2       0xA0
#define SFR_IE       0xA8
#define SFR_P0ODEN   0xB0
#define SFR_IP       0xB8
#define SFR_SMB0CN   0xC0
#define SFR_TMR2CN   0xC8
#define SFR_PSW      0xD0
#define SFR_PCA0CN   0xD8
#define SFR_ACC      0xE0
#define SFR_ADC0CN   0xE8
#define SFR_B        0xF0
#define SFR_SPI0CN   0xF8

//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

// TCON 0x88
SI_SBIT(TF1, SFR_TCON, 7);               // Timer 1 Overflow Flag
SI_SBIT(TR1, SFR_TCON, 6);               // Timer 1 On/Off Control
SI_SBIT(TF0, SFR_TCON, 5);               // Timer 0 Overflow Flag
SI_SBIT(TR0, SFR_TCON, 4);               // Timer 0 On/Off Control
SI_SBIT(IE1, SFR_TCON, 3);               // Ext. Interrupt 1 Edge Flag
SI_SBIT(IT1, SFR_TCON, 2);               // Ext. Interrupt 1 Type
SI_SBIT(IE0, SFR_TCON, 1);               // Ext. Interrupt 0 Edge Flag
SI_SBIT(IT0, SFR_TCON, 0);               // Ext. Interrupt 0 Type

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
SI_SBIT(TF2CEN, SFR_TMR2CN, 4);          // Timer 2 Lfo Capture Enable
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
SI_SBIT(AD0LJST, SFR_ADC0CN, 2);         // ADC0 Left Justify Select
SI_SBIT(AD0CM1, SFR_ADC0CN, 1);          // ADC0 Convert Start Mode Bit 1
SI_SBIT(AD0CM0, SFR_ADC0CN, 0);          // ADC0 Convert Start Mode Bit 0

// SPI0CN 0xF8
SI_SBIT(SPIF, SFR_SPI0CN, 7);            // SPI0 Interrupt Flag
SI_SBIT(WCOL, SFR_SPI0CN, 6);            // SPI0 Write Collision Flag
SI_SBIT(MODF, SFR_SPI0CN, 5);            // SPI0 Mode Fault Flag
SI_SBIT(RXOVRN, SFR_SPI0CN, 4);          // SPI0 RX Overrun Flag
SI_SBIT(NSSMD1, SFR_SPI0CN, 3);          // SPI0 Slave Select Mode 1
SI_SBIT(NSSMD0, SFR_SPI0CN, 2);          // SPI0 Slave Select Mode 0
SI_SBIT(TXBMT, SFR_SPI0CN, 1);           // SPI0 TX Buffer Empty Flag
SI_SBIT(SPIEN, SFR_SPI0CN, 0);           // SPI0 SPI0 Enable

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
#define INTERRUPT_RTC0             8   // RTC0 (SmaRTClock)
#define INTERRUPT_ADC0_WINDOW      9   // ADC0 Window Comparison
#define INTERRUPT_ADC0_EOC         10  // ADC0 End Of Conversion
#define INTERRUPT_PCA0             11  // PCA0 Peripheral
#define INTERRUPT_COMPARATOR0      12  // Comparator0
#define INTERRUPT_COMPARATOR1      13  // Comparator1
#define INTERRUPT_TIMER3           14  // Timer3 Overflow
#define INTERRUPT_VREG0            15  // VREG0 Dropout
#define INTERRUPT_PORT_MATCH       16  // Port Match

//-----------------------------------------------------------------------------
// SFR Page Definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// SDCC PDATA External Memory Paging Support
//-----------------------------------------------------------------------------

#if defined SDCC

SI_SFR(_XPAGE, 0xAA); // Point to the EMI0CN register

#endif

//-----------------------------------------------------------------------------
// Header File PreProcessor Directive
//-----------------------------------------------------------------------------

#endif                                 // #define C8051F410_DEFS_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
