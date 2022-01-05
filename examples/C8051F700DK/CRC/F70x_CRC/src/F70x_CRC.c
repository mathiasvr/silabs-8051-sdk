//-----------------------------------------------------------------------------
// F70x_CRC.c
//-----------------------------------------------------------------------------
// Copyright (C) 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This firmware calculates a CRC on data with a software-implemented CRC and
// the C8051F700's hardware-implemented CRC.  It measures the cycles spent
// performing both tasks and outputs this information through the UART.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J12: P0.4 - RXD
//           P0.5 - TXD
//	  - J15: VDD - power source
// 2) Connect the USB Debug Adapter to P3.
// 3) Connect the USB cable to P4.
// 4) Specify the target baudrate in the constant <BAUDRATE>.
// 5) Open Hyperterminal, or a similar program, and connect to the target
//    board's serial port.
// 6) Compile and download code to the C8051F70x development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Verify the output.
//
// Target:         C8051F700
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (PD)
//    -7 JUL 2008
//


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F700_Defs.h>          // SFR declarations
#include <stdio.h>
//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F41x
//-----------------------------------------------------------------------------

//sfr16 TMR2RL   = 0xca;                // Timer2 reload value
//sfr16 TMR2     = 0xcc;                // Timer2 counter

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // SYSCLK frequency in Hz
#define BAUDRATE     230400

sbit LED = P2^1;                       // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void Timer2_Init (void);
void Timer2_ISR (void);
void UART0_Init (void);
void OSCILLATOR_Init (void);
uint32_t UpdateCRC(uint32_t CRC_acc, uint8_t CRC_input);

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
   WDTCN = 0xDE;                       // Disable watchdog timer
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void) {

   uint32_t CRC_Val;                        // Stores software-implemented CRC
   uint16_t SoftwareCRC_NumCycles;          
   uint16_t HardwareCRC_NumCycles;          

   SI_UU32_t CRCHW;                         // Saves hardware-generated CRC


   SYSCLK_Init ();                     // Initialize system clock
   OSCILLATOR_Init ();
   PORT_Init ();                       // Initialize crossbar and GPIO
   Timer2_Init ();                     // Init timer 2 to count system
                                       // clocks when enabled
   UART0_Init ();                      // Init UART to output CRC test results

   SFRPAGE = CONFIG_PAGE;
   TMR2CN    = 0x04;                   // Enable timer 2
   SFRPAGE = LEGACY_PAGE;

   // First, generate a 32-bit CRC in software, using a sequence
   // of characters as inputs, and save the number of cycles used
   // to perform the calculations
   CRC_Val = 0xFFFFFFFF;

   CRC_Val = UpdateCRC(CRC_Val, '1');
   CRC_Val = UpdateCRC(CRC_Val, '2');
   CRC_Val = UpdateCRC(CRC_Val, '3');
   CRC_Val = UpdateCRC(CRC_Val, '4');
   CRC_Val = UpdateCRC(CRC_Val, '5');
   CRC_Val = UpdateCRC(CRC_Val, '6');
   CRC_Val = UpdateCRC(CRC_Val, '7');
   CRC_Val = UpdateCRC(CRC_Val, '8');
   CRC_Val = UpdateCRC(CRC_Val, '9');

   SFRPAGE = CONFIG_PAGE;
   TMR2CN    &= ~0x04;                 // Disable timer 2
   SFRPAGE = LEGACY_PAGE;

   SoftwareCRC_NumCycles = TMR2;       // Save number of cycles
   TMR2 = 0;                           // Clear timer

   SFRPAGE = CONFIG_PAGE;
   TMR2CN    = 0x04;                   // Enable timer 2
   SFRPAGE = LEGACY_PAGE;

   // Now use the C8051F700's CRC to generate the same CRC and save
   // the number of cycles used to perform the calculations
   SFRPAGE = CONFIG_PAGE;

   CRC0CN |= 0x00;                     // 32-bit CRC
   CRC0CN |= 0x04;                     // Initializes CRC to 0xFFFF
   CRC0CN |= 0x08;                     // Initializes result based on bit 2's
                                       // value

   CRC0IN = '1';                       // Write values into the CRC
   CRC0IN = '2';
   CRC0IN = '3';
   CRC0IN = '4';
   CRC0IN = '5';
   CRC0IN = '6';
   CRC0IN = '7';
   CRC0IN = '8';
   CRC0IN = '9';

   CRC0CN &= ~0x03;                    // First CRC0DAT access reads bits
                                       // 7-0 of CRC
   CRCHW.u8[3] = CRC0DAT;              // Read CRC value from hardware
   CRCHW.u8[2] = CRC0DAT;
   CRCHW.u8[1] = CRC0DAT;
   CRCHW.u8[0] = CRC0DAT;

   SFRPAGE = CONFIG_PAGE;
   TMR2CN   &= ~0x04;                  // Disable timer 2
   SFRPAGE = LEGACY_PAGE;
   
   HardwareCRC_NumCycles = TMR2;       // Save number of cycles

   // Output results
   printf("\nCycles used for software-generated CRC: %d",SoftwareCRC_NumCycles);
   printf("\nCycles used for hardware-generated CRC: %d",HardwareCRC_NumCycles);
   while (1) {                         // Spin forever
   }
}


unsigned long UpdateCRC (unsigned long CRC_acc, unsigned char CRC_input)
{
   unsigned char i; // loop counter
   #define POLY 0xEDB88320 // bit-reversed version of the poly 0x04C11DB7
   // Create the CRC "dividend" for polynomial arithmetic (binary arithmetic
   // with no carries)

   CRC_acc = CRC_acc ^ CRC_input;

   // "Divide" the poly into the dividend using CRC XOR subtraction
   // CRC_acc holds the "remainder" of each divide
   //
   // Only complete this division for 8 bits since input is 1 byte
   for (i = 0; i < 8; i++)
   {
      // Check if the MSB is set (if MSB is 1, then the POLY can "divide"
      // into the "dividend")
      if ((CRC_acc & 0x00000001) == 0x00000001)
      {
         // if so, shift the CRC value, and XOR "subtract" the poly
         CRC_acc = CRC_acc >> 1;
         CRC_acc ^= POLY;
      }
      else
      {
         // if not, just shift the CRC value
         CRC_acc = CRC_acc >> 1;
      }
   }
   // Return the final remainder (CRC value)
   return CRC_acc;
}




//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x80;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 24.5MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x83;                      // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P0.4   digital   push-pull     UART TX
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;
   
   SFRPAGE = CONFIG_PAGE;
   P0MDOUT |= 0x10;                    // Set P0.4 to push-pull output

   XBR0     = 0x01;                    // No digital peripherals selected
   XBR1     = 0x40;                    // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1)  int counts - calculated Timer overflow rate
//                    range is postive range of integer: 0 to 32767
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK as its time base.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON   |= 0x10;                    // Timer2 clocked based on SYSCLK

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
   #if (SYSCLK/BAUDRATE/2/256 < 1) 
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   #elif (SYSCLK/BAUDRATE/2/256 < 4) 
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   #elif (SYSCLK/BAUDRATE/2/256 < 12) 
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   #else 
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   #endif

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                       // START Timer1
   SCON0_TI = 1;                       // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
