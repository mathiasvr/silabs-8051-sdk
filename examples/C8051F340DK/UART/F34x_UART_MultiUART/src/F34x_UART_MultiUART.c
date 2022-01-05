//-----------------------------------------------------------------------------
// F34x_UART_MultiUART.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program configures UART0 and UART1 to operate in polled mode, suitable
// for use with the stdio.h functions printf() and scanf().
//
// Test code is included for printf() and getchar(), and shows an example
// of how the putchar() and _getkey() library routines can be overridden to
// allow for multiple UARTs to be selected by the functions (in this example, 
// the global variable UART is used to select between UART0 and UART1).
//
// The example sets system clock to maximum frequency of 48 MHz.
//
// The system clock frequency is stored in a global constant SYSCLK.  The
// target UART0 baud rate is stored in a global constant BAUDRATE0, and the
// target UART1 baud rate is stored in a global constant BAUDRATE1.
//
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J19: Power - P1_PWR or SER_PWR
//    - J7: +3VD - VDD
//    - J12: P0.4 TX or connect TX to P0.0 (see below)
//           P0.5 RX or connect RX to P0.1 (see below)
//           P2.2 - P2.2_LED
// 2) Connect the USB Debug Adapter to J9. If P1_PWR power is selected on J19,
//    connect the 9V power adapter to P1.
// 3) Ensure that the serial cable is connected to the RS232 connector
//    on the target board.
// 4) Compile and download code to a 'F340 device on a C8051F340-TB development
//    board selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 6) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the COM port at <BAUDRATE0>/<BAUDRATE1> and 8-N-1
// 7) To test UART 0, place jumpers connecting P0.4 to TX and P0.5 to RX on 
//    C8051F340 Target Board header J12.  To test UART 1, run wires from
//    P0.0 on J2 to the TX pin of J12, and P0.1 on J2 to the RX pin of J12.
// 8) Upon reset, each UART will print out a hello message. Additionally,
//    sending a '1' or '0' to the appropriately selected UART (chosen on
//    line 127) will turn the LED on and off.
//
//
// Target:         C8051F340
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 08 JAN 2014
//
// Release 1.0
//    -Initial Revision (PD)
//    -17 JUL 2006
//    -Initial Release
//
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F340_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F34x
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK       48000000          // SYSCLK frequency in Hz
#define BAUDRATE0     115200           // Baud rate of UART0 in bps
#define BAUDRATE1     115200           // Baud rate of UART1 in bps

SI_SBIT(LED, SFR_P2, 2);                 // LED='1' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSTEMCLOCK_Init (void);
void PORT_Init (void);
void UART0_Init (void);
void UART1_Init (void);
void Delay (void);

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

bit UART = 0;                          // '0 is UART0; '1' is UART1

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
   PCA0MD &= ~0x40;                    // Disable Watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void) {
   char input_char;


   SYSTEMCLOCK_Init ();                // initialize oscillator
   PORT_Init ();                       // initialize crossbar and GPIO
   UART0_Init ();                      // initialize UART0
   UART1_Init ();                      // initialize UART1

   // transmit example UART0
   UART = 0;   // select UART0
   printf ("Hello, from UART0!\n");

   // transmit example UART1
   UART = 1;                           // select UART1
   printf ("Hello, from UART1!\n");

   // receive example: a '1' turns LED on; a '0' turns LED off.
   // select which UART to receive on by changing the next line of code.

   UART = 0;                           // select UART: 0 = UART0, 1 = UART1

   while (1) {
      input_char = getchar();
      printf (" '%c', 0x%02x\n", (uint8_t) input_char, 
                                 (unsigned) input_char);

      switch (input_char) {
         case '0':
            LED = 0;
            break;
         case '1':
            LED = 1;
            break;
         default:
            break;
      }

   }

}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSTEMCLOCK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None

// This routine initializes the system clock to use the internal system clock
// routed through the clock multiplier as its clock source.
//
//-----------------------------------------------------------------------------

void SYSTEMCLOCK_Init (void)
{
   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency and enable
                                       // missing clock detector

   CLKMUL  = 0x00;                     // Select internal oscillator as
                                       // input to clock multiplier

   CLKMUL |= 0x80;                     // Enable clock multiplier
   Delay();                            // Delay for clock multiplier to begin
   CLKMUL |= 0xC0;                     // Initialize the clock multiplier
   Delay();                            // Delay for clock multiplier to begin

   while(!(CLKMUL & 0x20));            // Wait for multiplier to lock
   CLKSEL  = 0x03;                     // Select system clock
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None

// Configure the Crossbar and GPIO ports
//
// P0.0     TX1 (UART 1)
// P0.1     RX1 (UART 1)
// P0.2     
// P0.3
// P0.4     TX0 (UART 0)
// P0.5     RX0 (UART 0)
// P0.6
// P0.7

// P1       not used in this example

// P2.0
// P2.1
// P2.2     LED
// P2.3    
// P2.4
// P2.5
// P2.6
// P2.7
//-----------------------------------------------------------------------------

void PORT_Init (void)
{  
   XBR0 = 0x01;                        // route UART 0 to crossbar
   XBR2 = 0x01;                        // route UART 1 to crossbar
   XBR1 = 0x40;                        // enable crossbar
   P0MDOUT |= 0x11;                    // set P0.4 to push-pull output
   P2MDOUT |= 0x04;                    // set LED to push-pull
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for baudrate; and 8-N-1.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON0_RI and SCON0_TI bits

   if (SYSCLK/BAUDRATE0/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE0/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } else if (SYSCLK/BAUDRATE0/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE0/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01                 
      CKCON |=  0x09;
   } else if (SYSCLK/BAUDRATE0/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE0/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSCLK/BAUDRATE0/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

   TL1 = TH1;                          // init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;                       
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// UART1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure UART1 for baudrate1 and 8-N-1.
//
//-----------------------------------------------------------------------------

void UART1_Init (void)
{
   SMOD1 = 0x0C;                       // set to disable parity, 8-data bit,
                                       // disable extra bit, 
                                       // stop bit 1 bit wide

   SCON1 = 0x10;                       // SCON1: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON0_RI and SCON0_TI bits

   if (SYSCLK/BAUDRATE1/2/0xFFFF < 1) {
      SBRL1 = -(SYSCLK/BAUDRATE1/2);
      SBCON1 |= 0x03;                  // set prescaler to 1
   } else if (SYSCLK/BAUDRATE1/2/0xFFFF < 4) {
      SBRL1 = -(SYSCLK/BAUDRATE1/2/4);
      SBCON1 &= ~0x03;
      SBCON1 |= 0x01;                  // set prescaler to 4

   } else if (SYSCLK/BAUDRATE1/2/0xFFFF < 12) {
      SBRL1 = -(SYSCLK/BAUDRATE1/2/12);
      SBCON1 &= ~0x03;                 // set prescaler to 12
   } else {
      SBRL1 = -(SYSCLK/BAUDRATE1/2/48);
      SBCON1 &= ~0x03;
      SBCON1 |= 0x02;                  // set prescaler to 4
   }

   SCON1 |= 0x02;                      // indicate ready for TX
   SBCON1 |= 0x40;                     // enable baud rate generator
}

//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
// Return Value : UART0/1 buffer value
// Parameters   : character to be transmitted across UART0/1
//
// This is an overloaded fuction found in the stdio library.  When the
// function putchar is called, either by user code or through calls to stdio
// routines such as printf, the following routine will be executed instead 
// of the function located in the stdio library.
//
// The function checks the UART global variable to determine which UART to 
// use to receive a character.
//
// The routine expands '\n' to include a carriage return as well as a 
// new line character by first checking to see whether the character  
// passed into the routine equals '\n'.  If it is, the routine waits for 
// SCON0_TI/TI1 to be set, indicating that UART 0/1 is ready to transmit another 
// byte.  The routine then clears SCON0_TI/TI1 bit, and sets the UART0/1 output 
// buffer to '0x0d', which is the ASCII character for carriage return.
//
// The routine the waits for SCON0_TI/TI1 to be set, clears SCON0_TI/TI1, and sets
// the UART output buffer to <c>.  
//
//-----------------------------------------------------------------------------

char putchar (char c)  {

   if (UART == 0) {

      if (c == '\n')  {                // check for newline character
         while (!SCON0_TI);                 // wait until UART0 is ready to transmit
         SCON0_TI = 0;                      // clear interrupt flag
         SBUF0 = 0x0d;                 // output carriage return command
      }
      while (!SCON0_TI);                    // wait until UART0 is ready to transmit
      SCON0_TI = 0;                         // clear interrupt flag
      return (SBUF0 = c);              // output <c> using UART 0
   }

   else if (UART == 1) {
      if (c == '\n')  {                // check for newline character
         while (!(SCON1 & 0x02));      // wait until UART1 is ready to transmit
         SCON1 &= ~0x02;               // clear TI1 interrupt flag
         SBUF1 = 0x0d;                 // output carriage return
      }
      while (!(SCON1 & 0x02));         // wait until UART1 is ready to transmit
      SCON1 &= ~0x02;                  // clear TI1 interrupt flag
      return (SBUF1 = c);              // output <c> using UART 1
   }
   else {
	   return 0xff;
   }
}

//-----------------------------------------------------------------------------
// _getkey
//-----------------------------------------------------------------------------
//
// Return Value : byte received from UART0/1
// Parameters   : none

// This is an overloaded fuction found in the stdio library.  When the
// function _getkey is called, either by user code or through calls to stdio
// routines such as scanf, the following routine will be executed instead 
// of the function located in the stdio library.
//
// The function checks the UART global variable to determine which UART to 
// use to receive a character.
//
// The routine waits for SCON0_RI/RI1 to be set, indicating that a byte has
// been received across the UART0/UART1 RX line.  The routine saves the 
// received character into a local variable, clears the SCON0_RI/RI1 interrupt
// flag, and returns the received character value.
//
//-----------------------------------------------------------------------------
char _getkey ()  {
  char c;

  if (UART == 0) {
    while (!SCON0_RI);                      // wait until UART0 receives a character
    c = SBUF0;                         // save character to local variable
    SCON0_RI = 0;                           // clear UART0 receive interrupt flag
    return (c);                        // return value received through UART0
  }

  else if (UART == 1) {
    while (!(SCON1 & 0x01));           // wait until UART1 receives a character
    c = SBUF1;                         // save character to local variable
    SCON1 &= ~0x01;                    // clear UART1 receive interrupt flag
    return (c);                        // return value received through UART1
  }
  else {
	  return 0xff;
  }
}

//-----------------------------------------------------------------------------
// Delay
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : none
//
// Used for a small pause of approximately 40 us.
//
//-----------------------------------------------------------------------------

void Delay(void)
{
   int16_t x;
   for(x = 0;x < 500;x)
      x++;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
