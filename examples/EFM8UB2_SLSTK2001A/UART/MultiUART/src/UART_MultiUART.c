//-----------------------------------------------------------------------------
// UART_MultiUART.c
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
//
// How To Test:
//
//
// 1) Place the switch in "AEM" mode.
// 2) For UART1, Make the following connections from the EFM8UB1 STK to a
//    CP210x-EK:
//    EFM8UB2 P0.6 (TX) - CP210x RX
//    EFM8UB2 P0.7 (RX) - CP210x TX
//    EFM8UB2 GND       - CP210x GND
// 3) Connect the EFM8UB2 STK board to a PC using a mini USB cable.
// 4) Connect the CP210x-EK board to the PC using a USB cable.
// 5) Compile and download code to the EFM8UB2 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 6) For UART0: On the PC, open HyperTerminal (or any other terminal program)
//    and connect to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 7) For UART1: On the PC, open HyperTerminal (or any other terminal program)
//    and connect to the CP210x virtual COM port at 115200 baud rate and 8-N-1.
// 8) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 9) Upon reset, each UART will print out a hello message. Additionally,
//    sending a '1' or '0' to the appropriately selected UART (chosen on
//    line 127) will turn the LED on and off.
//
//
// Target:         EFM8UB2
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0 (BL)
//    - Initial Release
//    - 14 JAN 2015
//
//
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_EFM8UB2_Register_Enums.h>
#include <stdio.h>
#include "InitDevice.h"

#if defined __ICC8051__
#include <pgmspace.h>
#endif

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------


SI_SBIT (LED, SFR_P1, 6);                 // LED='0' means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void Delay (void);

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

bool UART = 0;                          // '0 is UART0; '1' is UART1

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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void) {
   char input_char;

   enter_DefaultMode_from_RESET();

   // Indicate that UART0 and UART1 are ready to transmit
   SCON0_TI = 1;
   SCON1 |= SCON1_TI__SET;

   // transmit example UART0
   UART = 0;   // select UART0
#if defined __C51__
   printf ("Hello, from UART0!\n");
#elif defined __ICC8051__
   printf_P ("Hello, from UART0!\n");
#endif

   // transmit example UART1
   UART = 1;                           // select UART1
#if defined __C51__
   printf ("Hello, from UART1!\n");
#elif defined __ICC8051__
   printf_P ("Hello, from UART1!\n");
#endif

   // receive example: a '1' turns LED on; a '0' turns LED off.
   // select which UART to receive on by changing the next line of code.

   UART = 0;                           // select UART: 0 = UART0, 1 = UART1

   while (1) {
      input_char = getchar();
#if defined __C51__
      printf (" '%c', 0x%x\n", (uint8_t) input_char, (uint16_t) input_char);
#elif defined __ICC8051__
      printf_P (" '%c', 0x%x\n", (uint8_t) input_char, (uint16_t) input_char);
#endif

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

#if defined __C51__

char putchar (char c)
{
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

#elif defined __ICC8051__

int putchar(int c)
{
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

#endif

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

#if defined __C51__

char getchar ()  {
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

#elif defined __ICC8051__

int getchar(void){
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

#endif


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
   volatile int16_t x;
   for(x = 0;x < 500;x)
      x++;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
