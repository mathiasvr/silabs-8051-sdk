//-----------------------------------------------------------------------------
// F36x_UART_STDIO.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the C8051F360 to use routines
// in STDIO.h to write to and read from the UART interface. The program
// reads a character using the STDIO routine getchar(), outputs that character
// to the screen, and then outputs the ASCII hex value of that character.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P0.1 - TX
//    J12: P0.2 - RX
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Ensure that a USB cable is connected to the USB connector
//    on the target board.
// 4) Specify the target baudrate in the constant <BAUDRATE>.
// 5) Open Hyperterminal, or a similar program, and connect to the target
//    board's serial port using <BAUDRATE> and 8-N-1 (CP210x USB-to-UART
//    VCP drivers must be installed).
// 6) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 8) Type a character in your terminal.
//
//
// Target:         C8051F36x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.2
//    - Updated to use with Simplicity Studio (JL)
//    - 29 JUL 2014
//
// Release 1.1
//    -01 MAR 2010 (BD)
//    -Replaced call to "getkey" with "getchar"
//
// Release 1.0
//    -Initial Revision (PD / TP)
//    -16 OCT 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK      24500000           // SYSCLK frequency in Hz
#define BAUDRATE        9600           // Baud rate of UART in bps


//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void Port_Init (void);
void Timer2_Init (int);

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uint8_t input_character;                 // Used to store character from UART


   Port_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init();

   SFRPAGE = LEGACY_PAGE;

   while (1)
   {
      printf ("\nEnter character: ");
      input_character = getchar ();
      printf ("\nCharacter entered : %c",input_character);
      printf ("\n      Value in Hex: %bx",input_character);

   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P0.1   digital   push-pull    UART TX
// P0.2   digital   open-drain   UART RX
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P0MDOUT |= 0x02;                    // Enable UTX as push-pull output
   XBR0    = XBR0_URT0E__ENABLED;      // Enable UART on P0.1(TX) and P0.2(RX)
   XBR1    = XBR1_XBARE__ENABLED;      // Enable crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal oscillator
// at its maximum frequency.
// Also enables the Missing Clock Detector.
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   OSCICN |= 0x03;                     // Configure internal oscillator for
                                       // its maximum frequency
   RSTSRC  = 0x04;                     // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
   CKCON &= ~(CKCON_T1M__BMASK |
              CKCON_SCA__FMASK);
   #if (SYSCLK/BAUDRATE/2/256 < 1)
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON |= CKCON_T1M__SYSCLK;
   #elif (SYSCLK/BAUDRATE/2/256 < 4)
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON |= CKCON_SCA__SYSCLK_DIV_4;
   #elif (SYSCLK/BAUDRATE/2/256 < 12)
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON |= CKCON_SCA__SYSCLK_DIV_12;
   #else
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON |= CKCON_SCA__SYSCLK_DIV_48;
   #endif

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xF0;                      // TMOD: Timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                       // START Timer1
   SCON0_TI = 1;                       // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------