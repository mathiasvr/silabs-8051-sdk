//-----------------------------------------------------------------------------
// F53xA_UART0_Interrupts.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the C8051F530A to write to and
// read from the UART interface. The program reads a word using the UART
// interrupts and outputs that word to the screen, with all characters in
// uppercase.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F530A TB are placed on the following:
//    "A" SIDE
//    HDR4: TX_A - P0.4_A
//    HDR4: RX_A - P0.5_A
// 2) Connect the USB Debug Adapter to HDR2 for "A" SIDE.
//    Connect the 9V power adapter to P5.
// 3) Connect a USB cable from P1 on the TB to the PC. (The USB ACTIVE
//    LED should light up if the CP210x drivers are properly installed).
//    Specify the target baudrate in the constant <BAUDRATE>.
// 4) Open Hyperterminal, or a similar terminalprogram, and connect to the
//    target board's serial port using <BAUDRATE> 8-N-1. (CP210x USB-to-UART
//    VCP drivers must be installed).
// 5) Compile and download code to C8051F530A target board "A" SIDE with
//    Simplicity IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 7) Type up to 64 characters into the Terminal and press Enter. The MCU
//    will then print back the characters that were typed.
//
// Target:         C8051F52xA/F53xA (C8051F530A TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.3 / 09 SEP 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.2 / 11 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1
//    -Updated for 'F530A TB (TP)
//    -31 JAN 2008
//
// Release 1.0
//    -Initial Revision (SM)
//    -10 JULY 2007


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F530_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK             24500000    // Oscillator frequency in Hz
#define BAUDRATE             115200    // Baud rate of UART in bps
#define UART_BUFFERSIZE          64

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void PCA0_Init (void);
void Oscillator_Init (void);
void Port_Init (void);
void UART0_Init (void);
void Init_Device (void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

uint8_t UART_Buffer[UART_BUFFERSIZE];
uint8_t UART_Buffer_Size = 0;
uint8_t UART_Input_First = 0;
uint8_t UART_Output_First = 0;
bit TX_Ready = 1;
static int8_t Byte;

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   Init_Device ();                     // Initializes hardware peripherals

   IE_EA = 1;

   while(1)
   {
      // If the complete word has been entered via the terminal followed by
      // carriage return
      if(TX_Ready == 1 && UART_Buffer_Size != 0 && Byte == 13)
      {
         TX_Ready = 0;                 // Set the flag to zero
         SCON0_TI = 1;                 // Set transmit flag to 1
      }
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function disables the watchdog timer.
//
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
}

//-----------------------------------------------------------------------------
// Oscillator_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void Oscillator_Init (void)
{
   // Set internal oscillator to run at its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P0.4   digital, push-pull  TX (UART0)
// P0.5   digital, open-drain RX (UART0)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
    P0MDOUT = 0x10;                    // Set the TX pin to push-pull
    XBR0 = XBR0_URT0E__ENABLED;        // Enable UART0 on the crossbar
    // Enable crossbar and weak pull-ups
    XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED |
           XBR1_XBARE__ENABLED;
}


//-----------------------------------------------------------------------------
// Init_Device
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Calls all device initialization functions.
//
//-----------------------------------------------------------------------------
void Init_Device (void)
{
   PCA0_Init ();                       // Disable the Watchdog Timer first
   Oscillator_Init ();
   Port_Init ();
   UART0_Init ();
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
   if (SYSCLK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } else if (SYSCLK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   } else if (SYSCLK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else {
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                       // START Timer1
   TX_Ready = 1;                       // Flag showing that UART can transmit
   IP |= 0x10;                         // Make UART high priority
   IE_ES0 = 1;                         // Enable UART0 interrupts
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// UART0_Interrupt
//-----------------------------------------------------------------------------
//
// This routine is invoked whenever a character is entered or displayed on the
// Hyperterminal.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(UART0_ISR, UART0_IRQn)
{
   if (SCON0_RI == 1)
   {
      if( UART_Buffer_Size == 0)       // If new word is entered
      {
         UART_Input_First = 0;
      }

      SCON0_RI = 0;                    // Clear interrupt flag

      Byte = SBUF0;                    // Read a character from UART

      if (UART_Buffer_Size < UART_BUFFERSIZE)
      {
         UART_Buffer[UART_Input_First] = Byte; // Store in array

         UART_Buffer_Size++;           // Update array's size

         UART_Input_First++;           // Update counter
      }
   }

   if (SCON0_TI == 1)                  // Check if transmit flag is set
   {
      SCON0_TI = 0;                    // Clear interrupt flag

      if (UART_Buffer_Size != 1)       // If buffer not empty
      {
         // If a new word is being output
         if ( UART_Buffer_Size == UART_Input_First )
         {
            UART_Output_First = 0;
         }

         // Store a character in the variable byte
         Byte = UART_Buffer[UART_Output_First];

         if ((Byte >= 0x61) && (Byte <= 0x7A)) // If upper case letter
         {
            Byte -= 32;
         }

         SBUF0 = Byte;                 // Transmit to Hyperterminal

         UART_Output_First++;          // Update counter

         UART_Buffer_Size--;           // Decrease array size

      }
      else
      {
         UART_Buffer_Size = 0;         // Set the array size to 0
         TX_Ready = 1;                 // Indicate transmission complete
      }
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------