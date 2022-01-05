//-----------------------------------------------------------------------------
// C8051F340_defs.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Register/bit definitions for the C8051F34x family.
// **Important Note**: The compiler_defs.h header file should be included
// before including this header file.
//
// Target:         C8051F340, 'F341, 'F342, 'F343, 'F344, 'F345, 'F346, 'F347,
//                 'F348, 'F349, 'F34A, 'F34B, 'F34C, 'F34D
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.6 - 20 AUG 2012 (TP)
//    -Added #define for _XPAGE to provide support for SDCC memory paging
//     (pdata)
// Release 1.5 - 21 SEP 2009 (BD)
//    -Added supported target part numbers 'F34C/D.
// Release 1.4 - 15 SEP 2008 (PKC)
//    -Added supported target part numbers 'F348/9/A/B.
// Release 1.3 - 07 AUG 2007 (PKC)
//    -Removed #include "si_toolchain.h". The C source file should include it.
//    -Removed FID and fixed formatting.
// Release 1.2 - (PKC/BW)
//    -Added SFR_ address definitions for bit-addressable SFRs
// Release 1.1 - 15 JUN 2006 (PKC)
//    -Reformatted header file to enable portable SFR definitions
// Release 1.0 - 13 DEC 2005 (GP/PKC)
//    -Initial Release

//-----------------------------------------------------------------------------
// Header File Preprocessor Directive
//-----------------------------------------------------------------------------

#ifndef C8051F340_DEFS_H
#define C8051F340_DEFS_H

//-----------------------------------------------------------------------------
// Byte Registers
//-----------------------------------------------------------------------------

SI_SFR(P0, 0x80);                        // Port 0 Latch
SI_SFR(SP, 0x81);                        // Stack Pointer
SI_SFR(DPL, 0x82);                       // Data Pointer Low
SI_SFR(DPH, 0x83);                       // Data Pointer High
SI_SFR(EMI0TC, 0x84);                    // EMIF Timing
SI_SFR(EMI0CF, 0x85);                    // EMIF Configuration
SI_SFR(OSCLCN, 0x86);                    // Internal Low-Freq Oscillator Control
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
SI_SFR(TMR3CN, 0x91);                    // Timer/Counter 3Control
SI_SFR(TMR3RLL, 0x92);                   // Timer/Counter 3 Reload Low
SI_SFR(TMR3RLH, 0x93);                   // Timer/Counter 3 Reload High
SI_SFR(TMR3L, 0x94);                     // Timer/Counter 3Low
SI_SFR(TMR3H, 0x95);                     // Timer/Counter 3 High
SI_SFR(USB0ADR, 0x96);                   // USB0 Indirect Address Register
SI_SFR(USB0DAT, 0x97);                   // USB0 Data Register
SI_SFR(SCON0, 0x98);                     // UART0 Control
SI_SFR(SBUF0, 0x99);                     // UART0 Data Buffer
SI_SFR(CPT1CN, 0x9A);                    // Comparator1 Control
SI_SFR(CPT0CN, 0x9B);                    // Comparator0 Control
SI_SFR(CPT1MD, 0x9C);                    // Comparator1 Mode Selection
SI_SFR(CPT0MD, 0x9D);                    // Comparator0 Mode Selection
SI_SFR(CPT1MX, 0x9E);                    // Comparator1 MUX Selection
SI_SFR(CPT0MX, 0x9F);                    // Comparator0 MUX Selection
SI_SFR(P2, 0xA0);                        // Port 2 Latch
SI_SFR(SPI0CFG, 0xA1);                   // SPI Configuration
SI_SFR(SPI0CKR, 0xA2);                   // SPI Clock Rate Control
SI_SFR(SPI0DAT, 0xA3);                   // SPI Data
SI_SFR(P0MDOUT, 0xA4);                   // Port 0 Output Mode Configuration
SI_SFR(P1MDOUT, 0xA5);                   // Port 1 Output Mode Configuration
SI_SFR(P2MDOUT, 0xA6);                   // Port 2 Output Mode Configuration
SI_SFR(P3MDOUT, 0xA7);                   // Port 3 Output Mode Configuration
SI_SFR(IE, 0xA8);                        // Interrupt Enable
SI_SFR(CLKSEL, 0xA9);                    // Clock Select
SI_SFR(EMI0CN, 0xAA);                    // External Memory Interface Control
SI_SFR(SBCON1, 0xAC);                    // UART1 Baud Rate Generator Control
SI_SFR(P4MDOUT, 0xAE);                   // Port 4 Output Mode Configuration
SI_SFR(PFE0CN, 0xAF);                    // Prefetch Engine Control
SI_SFR(P3, 0xB0);                        // Port 3 Latch
SI_SFR(OSCXCN, 0xB1);                    // External Oscillator Control
SI_SFR(OSCICN, 0xB2);                    // Internal Oscillator Control
SI_SFR(OSCICL, 0xB3);                    // Internal Oscillator Calibration
SI_SFR(SBRLL1, 0xB4);                    // UART1 Baud Rate Generator Low
SI_SFR(SBRLH1, 0xB5);                    // UART1 Baud Rate Generator High
SI_SFR(FLSCL, 0xB6);                     // Flash Scale
SI_SFR(FLKEY, 0xB7);                     // Flash Lock and Key
SI_SFR(IP, 0xB8);                        // Interrupt Priority
SI_SFR(CLKMUL, 0xB9);                    // Clock Multiplier
SI_SFR(AMX0N, 0xBA);                     // AMUX0 Negative Channel Select
SI_SFR(AMX0P, 0xBB);                     // AMUX0 Positive Channel Select
SI_SFR(ADC0CF, 0xBC);                    // ADC0 Configuration
SI_SFR(ADC0L, 0xBD);                     // ADC0 Low
SI_SFR(ADC0H, 0xBE);                     // ADC0 High
SI_SFR(SMB0CN, 0xC0);                    // SMBus Control
SI_SFR(SMB0CF, 0xC1);                    // SMBus Configuration
SI_SFR(SMB0DAT, 0xC2);                   // SMBus Data
SI_SFR(ADC0GTL, 0xC3);                   // ADC0 Greater-Than Compare Low
SI_SFR(ADC0GTH, 0xC4);                   // ADC0 Greater-Than Compare High
SI_SFR(ADC0LTL, 0xC5);                   // ADC0 Less-Than Compare Word Low
SI_SFR(ADC0LTH, 0xC6);                   // ADC0 Less-Than Compare Word High
SI_SFR(P4, 0xC7);                        // Port 4 Latch
SI_SFR(TMR2CN, 0xC8);                    // Timer/Counter 2 Control
SI_SFR(REG0CN, 0xC9);                    // Voltage Regulator Control
SI_SFR(TMR2RLL, 0xCA);                   // Timer/Counter 2 Reload Low
SI_SFR(TMR2RLH, 0xCB);                   // Timer/Counter 2 Reload High
SI_SFR(TMR2L, 0xCC);                     // Timer/Counter 2 Low
SI_SFR(TMR2H, 0xCD);                     // Timer/Counter 2 High
SI_SFR(PSW, 0xD0);                       // Program Status Word
SI_SFR(REF0CN, 0xD1);                    // Voltage Reference Control
SI_SFR(SCON1, 0xD2);                     // UART1 Control
SI_SFR(SBUF1, 0xD3);                     // UART1 Data Buffer
SI_SFR(P0SKIP, 0xD4);                    // Port 0 Skip
SI_SFR(P1SKIP, 0xD5);                    // Port 1 Skip
SI_SFR(P2SKIP, 0xD6);                    // Port 2 Skip
SI_SFR(USB0XCN, 0xD7);                   // USB0 Transceiver Control
SI_SFR(PCA0CN, 0xD8);                    // PCA0 Control
SI_SFR(PCA0MD, 0xD9);                    // PCA0 Mode
SI_SFR(PCA0CPM0, 0xDA);                  // PCA0 Module 0 Mode Register
SI_SFR(PCA0CPM1, 0xDB);                  // PCA0 Module 1 Mode Register
SI_SFR(PCA0CPM2, 0xDC);                  // PCA0 Module 2 Mode Register
SI_SFR(PCA0CPM3, 0xDD);                  // PCA0 Module 3 Mode Register
SI_SFR(PCA0CPM4, 0xDE);                  // PCA0 Module 4 Mode Register
SI_SFR(P3SKIP, 0xDF);                    // Port 3 Skip
SI_SFR(ACC, 0xE0);                       // Accumulator
SI_SFR(XBR0, 0xE1);                      // Port I/O Crossbar Control 0
SI_SFR(XBR1, 0xE2);                      // Port I/O Crossbar Control 1
SI_SFR(XBR2, 0xE3);                      // Port I/O Crossbar Control 2
SI_SFR(IT01CF, 0xE4);                    // INT0/INT1 Configuration
SI_SFR(SMOD1, 0xE5);                     // UART1 Mode
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
SI_SFR(P3MDIN, 0xF4);                    // Port 3 Input Mode Configuration
SI_SFR(P4MDIN, 0xF5);                    // Port 4 Input Mode Configuration
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

SI_SFR16(DP, 0x82);
SI_SFR16(TMR3RL, 0x92);
SI_SFR16(TMR3, 0x94);
SI_SFR16(SBRL1, 0xB4);
SI_SFR16(ADC0, 0xBD);
SI_SFR16(ADC0GT, 0xC3);
SI_SFR16(ADC0LT, 0xC5);
SI_SFR16(TMR2RL, 0xCA);
SI_SFR16(TMR2, 0xCC);
SI_SFR16(PCA0CP1, 0xE9);
SI_SFR16(PCA0CP2, 0xEB);
SI_SFR16(PCA0CP3, 0xED);
SI_SFR16(PCA0, 0xF9);
SI_SFR16(PCA0CP0, 0xFB);
SI_SFR16(PCA0CP4, 0xFD);

//-----------------------------------------------------------------------------
// Address Definitions for Bit-addressable Registers
//-----------------------------------------------------------------------------

#define SFR_P0     0x80
#define SFR_TCON   0x88
#define SFR_P1     0x90
#define SFR_SCON0  0x98
#define SFR_P2     0xA0
#define SFR_IE     0xA8
#define SFR_P3     0xB0
#define SFR_IP     0xB8
#define SFR_SMB0CN 0xC0
#define SFR_TMR2CN 0xC8
#define SFR_PSW    0xD0
#define SFR_PCA0CN 0xD8
#define SFR_ACC    0xE0
#define SFR_ADC0CN 0xE8
#define SFR_B      0xF0
#define SFR_SPI0CN 0xF8


//-----------------------------------------------------------------------------
// Bit Definitions
//-----------------------------------------------------------------------------

// TCON 0x88
SI_SBIT(TF1, SFR_TCON, 7);               // Timer1 overflow flag
SI_SBIT(TR1, SFR_TCON, 6);               // Timer1 on/off control
SI_SBIT(TF0, SFR_TCON, 5);               // Timer0 overflow flag
SI_SBIT(TR0, SFR_TCON, 4);               // Timer0 on/off control
SI_SBIT(IE1, SFR_TCON, 3);               // Ext interrupt 1 edge flag
SI_SBIT(IT1, SFR_TCON, 2);               // Ext interrupt 1 type
SI_SBIT(IE0, SFR_TCON, 1);               // Ext interrupt 0 edge flag
SI_SBIT(IT0, SFR_TCON, 0);               // Ext interrupt 0 type

// SCON0 0x98
SI_SBIT(S0MODE, SFR_SCON0, 7);           // Serial mode control bit 0
                                       // Bit6 UNUSED
SI_SBIT(MCE0, SFR_SCON0, 5);             // Multiprocessor communication enable
SI_SBIT(REN0, SFR_SCON0, 4);             // Receive enable
SI_SBIT(TB80, SFR_SCON0, 3);             // Transmit bit 8
SI_SBIT(RB80, SFR_SCON0, 2);             // Receive bit 8
SI_SBIT(TI0,  SFR_SCON0, 1);             // Transmit interrupt flag
SI_SBIT(RI0,  SFR_SCON0, 0);             // Receive interrupt flag

// IE 0xA8
SI_SBIT(EA,    SFR_IE, 7);               // Global interrupt enable
SI_SBIT(ESPI0, SFR_IE, 6);               // SPI0 interrupt enable
SI_SBIT(ET2,   SFR_IE, 5);               // Timer2 interrupt enable
SI_SBIT(ES0,   SFR_IE, 4);               // UART0 interrupt enable
SI_SBIT(ET1,   SFR_IE, 3);               // Timer1 interrupt enable
SI_SBIT(EX1,   SFR_IE, 2);               // External interrupt 1 enable
SI_SBIT(ET0,   SFR_IE, 1);               // Timer0 interrupt enable
SI_SBIT(EX0,   SFR_IE, 0);               // External interrupt 0 enable

// IP 0xB8
                                       // Bit7 UNUSED
SI_SBIT(PSPI0, SFR_IP, 6);               // SPI0 interrupt priority
SI_SBIT(PT2,   SFR_IP, 5);               // Timer2 priority
SI_SBIT(PS0,   SFR_IP, 4);               // UART0 priority
SI_SBIT(PT1,   SFR_IP, 3);               // Timer1 priority
SI_SBIT(PX1,   SFR_IP, 2);               // External interrupt 1 priority
SI_SBIT(PT0,   SFR_IP, 1);               // Timer0 priority
SI_SBIT(PX0,   SFR_IP, 0);               // External interrupt 0 priority

// SMB0CN 0xC0
SI_SBIT(MASTER,  SFR_SMB0CN, 7);         // Master/slave indicator
SI_SBIT(TXMODE,  SFR_SMB0CN, 6);         // Transmit mode indicator
SI_SBIT(STA,     SFR_SMB0CN, 5);         // Start flag
SI_SBIT(STO,     SFR_SMB0CN, 4);         // Stop flag
SI_SBIT(ACKRQ,   SFR_SMB0CN, 3);         // Acknowledge request
SI_SBIT(ARBLOST, SFR_SMB0CN, 2);         // Arbitration lost indicator
SI_SBIT(ACK,     SFR_SMB0CN, 1);         // Acknowledge flag
SI_SBIT(SI,      SFR_SMB0CN, 0);         // SMBus interrupt flag

// TMR2CN 0xC8
SI_SBIT(TF2H,    SFR_TMR2CN, 7);         // Timer2 high byte overflow flag
SI_SBIT(TF2L,    SFR_TMR2CN, 6);         // Timer2 low byte overflow flag
SI_SBIT(TF2LEN,  SFR_TMR2CN, 5);         // Timer2 low byte interrupt enable
SI_SBIT(T2CE,    SFR_TMR2CN, 4);         // Timer2 capture enable
SI_SBIT(T2SPLIT, SFR_TMR2CN, 3);         // Timer2 split mode enable
SI_SBIT(TR2,     SFR_TMR2CN, 2);         // Timer2 on/off control
SI_SBIT(T2CSS,   SFR_TMR2CN, 1);         // Timer 2 Capture Source select
SI_SBIT(T2XCLK,  SFR_TMR2CN, 0);         // Timer2 external clock select

// PSW 0xD0
SI_SBIT(CY,  SFR_PSW, 7);                // Carry flag
SI_SBIT(AC,  SFR_PSW, 6);                // Auxiliary carry flag
SI_SBIT(F0,  SFR_PSW, 5);                // User flag 0
SI_SBIT(RS1, SFR_PSW, 4);                // Register bank select 1
SI_SBIT(RS0, SFR_PSW, 3);                // Register bank select 0
SI_SBIT(OV,  SFR_PSW, 2);                // Overflow flag
SI_SBIT(F1,  SFR_PSW, 1);                // User flag 1
SI_SBIT(P,   SFR_PSW, 0);                // Accumulator parity flag

// PCA0CN 0xD8
SI_SBIT(CF, SFR_PCA0CN, 7);              // PCA0 counter overflow flag
SI_SBIT(CR, SFR_PCA0CN, 6);              // PCA0 counter run control
                                       // Bit5 UNUSED
SI_SBIT(CCF4, SFR_PCA0CN, 4);            // PCA0 module4 capture/compare flag
SI_SBIT(CCF3, SFR_PCA0CN, 3);            // PCA0 module3 capture/compare flag
SI_SBIT(CCF2, SFR_PCA0CN, 2);            // PCA0 module2 capture/compare flag
SI_SBIT(CCF1, SFR_PCA0CN, 1);            // PCA0 module1 capture/compare flag
SI_SBIT(CCF0, SFR_PCA0CN, 0);            // PCA0 module0 capture/compare flag

// ADC0CN 0xE8
SI_SBIT(AD0EN,   SFR_ADC0CN, 7);         // ADC0 enable
SI_SBIT(AD0TM,   SFR_ADC0CN, 6);         // ADC0 track mode
SI_SBIT(AD0INT,  SFR_ADC0CN, 5);         // ADC0 conversion complete interrupt flag
SI_SBIT(AD0BUSY, SFR_ADC0CN, 4);         // ADC0 busy flag
SI_SBIT(AD0WINT, SFR_ADC0CN, 3);         // ADC0 window compare interrupt flag
SI_SBIT(AD0CM2,  SFR_ADC0CN, 2);         // ADC0 conversion mode select 2
SI_SBIT(AD0CM1,  SFR_ADC0CN, 1);         // ADC0 conversion mode select 1
SI_SBIT(AD0CM0,  SFR_ADC0CN, 0);         // ADC0 conversion mode select 0

// SPI0CN 0xF8
SI_SBIT(SPIF,   SFR_SPI0CN, 7);          // SPI0 interrupt flag
SI_SBIT(WCOL,   SFR_SPI0CN, 6);          // SPI0 write collision flag
SI_SBIT(MODF,   SFR_SPI0CN, 5);          // SPI0 mode fault flag
SI_SBIT(RXOVRN, SFR_SPI0CN, 4);          // SPI0 rx overrun flag
SI_SBIT(NSSMD1, SFR_SPI0CN, 3);          // SPI0 slave select mode 1
SI_SBIT(NSSMD0, SFR_SPI0CN, 2);          // SPI0 slave select mode 0
SI_SBIT(TXBMT,  SFR_SPI0CN, 1);          // SPI0 transmit buffer empty
SI_SBIT(SPIEN,  SFR_SPI0CN, 0);          // SPI0 SPI enable


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
#define INTERRUPT_USB0             8   // USB Interface
#define INTERRUPT_ADC0_WINDOW      9   // ADC0 Window Comparison
#define INTERRUPT_ADC0_EOC         10  // ADC0 End Of Conversion
#define INTERRUPT_PCA0             11  // PCA0 Peripheral
#define INTERRUPT_COMPARATOR0      12  // Comparator0
#define INTERRUPT_COMPARATOR1      13  // Comparator1
#define INTERRUPT_TIMER3           14  // Timer3 Overflow
#define INTERRUPT_VBUS_LEVEL       15  // VBUS level-triggered interrupt
#define INTERRUPT_UART1            16  // Serial Port 1

//-----------------------------------------------------------------------------
// SDCC PDATA External Memory Paging Support
//-----------------------------------------------------------------------------

#if defined SDCC

SI_SFR(_XPAGE, 0xAA); // Point to the EMI0CN register

#endif

//-----------------------------------------------------------------------------
// Header File PreProcessor Directive
//-----------------------------------------------------------------------------

#endif                                 // #define C8051F340_DEFS_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
