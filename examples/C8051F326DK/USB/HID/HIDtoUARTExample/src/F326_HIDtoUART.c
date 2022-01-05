//-----------------------------------------------------------------------------
// F326_HIDtoUART.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how the Firmware Template can be used to
// create a USB HID to UART bridge that communicates with a PC application.
// The PC application is a simple terminal-like program that sends data to and
// receives data from the Target Board.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F326-TB are place on the following:
//    J2: VDD  - +3VD
//    J3: P0.4 - TX
//    J3: P0.5 - RX
//    J7: VDD  - VIO
//    J8: Power selection, P1 9V or J4 Debugger
// 2) Connect a serial cable to the J5 RS232 connector on the target board.
// 3) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1.
// 4) Compile and download code to C8051F326 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 6) Run the GUI application HIDtoUART that comes with the firmware
//    and test the features.
//
//
// Target:         C8051F326/7
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
// Project Name:   HIDtoUART
//
//
// Release 1.1
//    -All changes by BW
//    -1 SEP 2010
//    -Updated USB0_Init() to write 0x89 to CLKREC instead of 0x80
// Release 1.0
//    -Initial Revision (PD)
//    -04 JUN 2008
//

//-----------------------------------------------------------------------------
// Header Files
//-----------------------------------------------------------------------------
#include "c8051f3xx.h"
#include "F3xx_USB0_InterruptServiceRoutine.h"
#include "F3xx_USB0_Register.h"
#include "F3xx_HIDtoUART.h"
#include <intrins.h>
//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------
#define SYSCLK             24000000    // SYSCLK frequency in Hz

// USB clock selections (SFR CLKSEL)
#define USB_4X_CLOCK       0x00        // Select 4x clock multiplier, for USB
#define USB_INT_OSC_DIV_2  0x10        // Full Speed
#define USB_EXT_OSC        0x20
#define USB_EXT_OSC_DIV_2  0x30
#define USB_EXT_OSC_DIV_3  0x40
#define USB_EXT_OSC_DIV_4  0x50

// System clock selections (SFR CLKSEL)
#define SYS_INT_OSC        0x00        // Select to use internal oscillator
#define SYS_EXT_OSC        0x01        // Select to use an external oscillator
#define SYS_4X_DIV_2       0x02


//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F326
//-----------------------------------------------------------------------------
SI_SFR16(SBRL1, 0x93);

//-----------------------------------------------------------------------------
// Local Function Prototypes
//-----------------------------------------------------------------------------
// Initialization Routines
void Sysclk_Init (void);               // Initialize the system clock
void Port_Init (void);                 // Configure ports
void Usb0_Init (void);                 // Configure USB core
void Delay (void);                     // Approximately 80 us/1 ms on
                                       // Full/Low Speed
void UART0_Init(void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
#ifndef BAUDRATE_HARDCODED
unsigned long BaudRate;
#endif

// IN_PACKET and OUT_PACKEt buffer bytes immediately before and
// after they are transferred across USB inside the report handlers
unsigned char xdata IN_PACKET[64];
unsigned char xdata OUT_PACKET[64];

// UART_INPUT and UART_OUTPUT buffer bytes immediately before
// and after they are transferred across the UART interface
unsigned char xdata UART_INPUT[UART_INPUT_BUFFERSIZE];
unsigned char xdata UART_OUTPUT[UART_OUTPUT_BUFFERSIZE];

// UART_OUTPUT_OVERFLOW_BOUNDARY holds a calculation showing how
// many bytes can be transmitted out of the buffer in the span of time
// before another USB report is received
// If the buffer size crosses this boundary, USB communication should be
// suspended until the buffer shrinks below the boundary
unsigned char UART_OUTPUT_OVERFLOW_BOUNDARY;

unsigned char USB_OUT_SUSPENDED;       // Flag set when buffer size crosses
                                       // boundary

unsigned char UART_INPUT_SIZE = 0;     // Maintains size of input buffer
unsigned char UART_OUTPUT_SIZE = 0;    // Maintains size of output buffer

unsigned char UART_INPUT_LAST = 0;     // Points to oldest byte received
unsigned char UART_INPUT_FIRST = 0;    // Points to newest byte received

unsigned char UART_OUTPUT_LAST = 0;    // Points to oldest byte received
unsigned char UART_OUTPUT_FIRST = 0;   // Points to newest byte received
unsigned char TX_Ready;                // Flag used to initiate UART transfer


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// UART0 Interrupt Service Routine
//-----------------------------------------------------------------------------
// Routine transmits a byte by pulling LAST byte from UART_OUTPUT buffer.
// Routine receives a byte by pushing it as FIRST byte on UART_INPUT buffer.
//
void UART0_Interrupt (void) interrupt 4
{
   if (SCON0_RI == 1)                  // Received byte flag
   {
      SCON0_RI = 0;                    // Acknowledge flag

      // Note: the code below only works correctly if the UART buffers
      // are 127 bytes (0x7F) in length
      // This allows buffer wrapping to be accomplished by simply
      // clearing the MSB
      UART_INPUT_FIRST++;              // Move pointer
      UART_INPUT_FIRST &= ~0x80;       // Wrap pointer if necessary

      // Save received byte onto buffer
      UART_INPUT[UART_INPUT_FIRST] = SBUF0;

      UART_INPUT_SIZE++;               // Increment buffer size
   }

   if (SCON0_TI == 1)                  // Transmit complete flag
   {
      SCON0_TI = 0;                    // Acknowledge flag
      if (UART_OUTPUT_SIZE != 0)       // If buffer is not empty
      {
         UART_OUTPUT_LAST++;           // Move buffer pointer

         UART_OUTPUT_LAST &= ~0x80;    // Wrap pointer

         // Transmit byte from buffer
         SBUF0 = UART_OUTPUT[UART_OUTPUT_LAST];
         UART_OUTPUT_SIZE--;           // Decrement buffer size
      }
      else
      {
         TX_Ready = 1;                 // Buffer is empty, signal that
                                       // UART TX is free and ready to
                                       // transmit
      }
   }

}
//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DEFAULT_InitRoutine
//-----------------------------------------------------------------------------
// This function is declared in the header file F3xx_HIDtoUART.h and is
// called in the main(void) function.  It calls initialization routines
// local to this file.
//
//-----------------------------------------------------------------------------
void System_Init (void)
{
   Sysclk_Init ();                     // Initialize oscillator
   Port_Init ();                       // Initialize crossbar and GPIO

#ifndef BAUDRATE_HARDCODED
   BaudRate = 115200;
#endif
   UART0_Init();

}

//-----------------------------------------------------------------------------
// USB_Init
//-----------------------------------------------------------------------------
// USB Initialization performs the following:
// - Initialize USB0
// - Enable USB0 interrupts
// - Enable USB0 transceiver
// - Enable USB0 with suspend detection
//
//-----------------------------------------------------------------------------
void USB_Init (void)
{

   POLL_WRITE_BYTE (POWER,  0x08);     // Force Asynchronous USB Reset
   POLL_WRITE_BYTE (IN1IE,  0x07);     // Enable Endpoint 0-1 in interrupts
   POLL_WRITE_BYTE (OUT1IE, 0x07);     // Enable Endpoint 0-1 out interrupts
   POLL_WRITE_BYTE (CMIE,   0x07);     // Enable Reset, Resume, and Suspend
                                       // interrupts

   USB0XCN = 0xE0;                     // Enable transceiver; select full speed
   POLL_WRITE_BYTE (CLKREC, 0x89);     // Enable clock recovery, single-step
                                       // mode disabled

   EIE1 |= 0x02;                       // Enable USB0 Interrupts

                                       // Enable USB0 by clearing the USB
   POLL_WRITE_BYTE (POWER, 0x01);      // Inhibit Bit and enable suspend
                                       // detection

}


//-----------------------------------------------------------------------------
// Sysclk_Init(void)
//-----------------------------------------------------------------------------
// This function initializes the system clock and the USB clock.
//
//-----------------------------------------------------------------------------
void Sysclk_Init(void)
{
   OSCICN |= 0x83;
   CLKMUL  = 0x00;
   CLKMUL |= 0x80;

   Delay();

   CLKMUL |= 0xC0;                     // Initialize the clock multiplier

   while(!(CLKMUL & 0x20));            // Wait for multiplier to lock

   CLKSEL = 0x02;                      // Use Clock Multiplier/2 as
                                       // system clock
}

//-----------------------------------------------------------------------------
// Port_Init(void)
//-----------------------------------------------------------------------------
// Port Initialization routine that configures the Crossbar and GPIO ports.
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   P0MDOUT |= 0x10;                    // Port 0 pins 0-3 set high impedence
   GPIOCN  |= 0x40;                    // Enable inputs
}


//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//


//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Baudrate generator, for <BAUDRATE1> and 8-N-1.
//-----------------------------------------------------------------------------

void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       // clear SCON0_RI and SCON0_TI bits

   // Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

   if (SYSCLK/BaudRate/2/0xFFFF < 1) {
      SBRL1 = -(SYSCLK/BaudRate/2);
      SBCON0 |= 0x03;                  // set prescaler to 1
   } else if (SYSCLK/BaudRate/2/0xFFFF < 4) {
      SBRL1 = -(SYSCLK/BaudRate/2/4);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x01;                  // set prescaler to 4

   } else if (SYSCLK/BaudRate/2/0xFFFF < 12) {
      SBRL1 = -(SYSCLK/BaudRate/2/12);
      SBCON0 &= ~0x03;                 // set prescaler to 12
   } else {
      SBRL1 = -(SYSCLK/BaudRate/2/48);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x02;                  // set prescaler to 4
   }

   SBCON0 |= 0x40;                     // enable baud rate generator
   TX_Ready = 1;                       // Flag showing that UART can transmit
   IE = 0x10;                          // Enable UART0 interrupts

   IP |= 0x10;                         // Make UART high priority

   // The following code computes the size the UART_INPUT buffer, which
   // stores bytes received from the USB, can reach before one more
   // USB packet will overflow the buffer.
   // This is to guard against the case where the UART is transmitting
   // bytes out at a speed that is much lower than the speed at which
   // bytes are received.
   UART_OUTPUT_OVERFLOW_BOUNDARY = UART_OUTPUT_BUFFERSIZE - OUT_DATA_SIZE;
}




//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Delay(void)
//-----------------------------------------------------------------------------
// Used for a small pause, approximately 80 us in Full Speed,
// and 1 ms when clock is configured for Low Speed
//
//-----------------------------------------------------------------------------
void Delay (void)
{
   int x;
   for(x = 0;x < 500;x)
      x++;
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
