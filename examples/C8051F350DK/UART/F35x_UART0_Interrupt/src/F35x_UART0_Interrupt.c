//-----------------------------------------------------------------------------
// F35x_UART0_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// This program demonstrates how to configure the C8051F35x to write to and read
// from the UART interface. The program reads a word using the UART interrupts
// and outputs that word to the screen, with all characters in uppercase.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J3 : P0.4 - TX
//    - J3 : P0.5 - RX
//    - J8 : Power selection, P1 9V or J4 Debugger
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 3) Connect serial cable from the RS232 connector J5 to a PC.
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the virtual COM port at <BAUDRATE> (9600) and 8-N-1.
// 5) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 6) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 7) Type up to 64 characters into the Terminal and press Enter. The MCU
//    will then print back the characters that were typed.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1 (JL)
//    - Updated descriptions
//    - 24 JUN 2014
//
// Release 1.0
//    -Initial Revision (SM)
//    -5 JUN 2007
//


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F350_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK          24500000       // SYSCLK frequency in Hz
#define BAUDRATE        9600           // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void PORT_Init (void);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

#define UART_BUFFERSIZE 64
uint8_t UART_Buffer[UART_BUFFERSIZE];
uint8_t UART_Buffer_Size = 0;
uint8_t UART_Input_First = 0;
uint8_t UART_Output_First = 0;
uint8_t TX_Ready =1;
static uint8_t Byte;

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   // Disable Watchdog timer

   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init();

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
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P0.4   digital   push-pull    UART TX
// P0.5   digital   open-drain   UART RX
//
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output
   XBR0     = XBR0_URT0E__ENABLED;           // Enable UART0
   XBR1     = XBR1_XBARE__ENABLED |          // Enable crossbar
             XBR1_WEAKPUD__PULL_UPS_ENABLED; // with weak pullups
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
   OSCICN |= OSCICN_IFCN__HFOSC_DIV_1; // Configure internal oscillator for
                                       // its maximum frequency
   RSTSRC  = 0x04;                     // Enable missing clock detector
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
   SCON0 = SCON0_SMODE__8_BIT         |   // 8-bit variable bit rate
           SCON0_MCE__MULTI_DISABLED  |   // level of STOP bit is ignored
           SCON0_REN__RECEIVE_ENABLED |   // RX enabled
           SCON0_TB8__CLEARED_TO_0    |   // ninth bits are zeros
           SCON0_RB8__CLEARED_TO_0    |
           SCON0_TI__NOT_SET          |   // clear RI0 and TI0 bits
           SCON0_RI__NOT_SET;

   CKCON &= ~(CKCON_T1M__BMASK |
              CKCON_SCA__FMASK);
   #if (SYSCLK/BAUDRATE/2/256 < 1)
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON |=  CKCON_T1M__SYSCLK;        // T1M = 1; SCA1:0 = xx
   #elif (SYSCLK/BAUDRATE/2/256 < 4)
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON |=  CKCON_SCA__SYSCLK_DIV_4;  // T1M = 0; SCA1:0 = 01
   #elif (SYSCLK/BAUDRATE/2/256 < 12)
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON |= CKCON_SCA__SYSCLK_DIV_12;  // T1M = 0; SCA1:0 = 00
   #else
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON |=  CKCON_SCA__SYSCLK_DIV_48; // T1M = 0; SCA1:0 = 10
   #endif

   TL1 = TH1;                          // init Timer1
   TMOD &= ~(TMOD_GATE1__BMASK |
             TMOD_CT1__BMASK   |
             TMOD_T1M__FMASK);
   // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  TMOD_T1M__MODE2;
   TCON_TR1 = 1;                       // START Timer1
   TX_Ready = 1;                       // Flag showing that UART can transmit
   IP |= IP_PS0__HIGH;                 // Make UART high priority
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
      if( UART_Buffer_Size == 0)  {      // If new word is entered
         UART_Input_First = 0;
      }

      SCON0_RI = 0;                      // Clear interrupt flag

      Byte = SBUF0;                      // Read a character from UART

      if (UART_Buffer_Size < UART_BUFFERSIZE)
      {
         UART_Buffer[UART_Input_First] = Byte; // Store in array

         UART_Buffer_Size++;             // Update array's size

         UART_Input_First++;             // Update counter
      }
   }

   if (SCON0_TI == 1)                   // Check if transmit flag is set
   {
      SCON0_TI = 0;                     // Clear interrupt flag

      if (UART_Buffer_Size != 1)         // If buffer not empty
      {
         // If a new word is being output
         if ( UART_Buffer_Size == UART_Input_First ) {
            UART_Output_First = 0;
         }

         // Store a character in the variable byte
         Byte = UART_Buffer[UART_Output_First];

         if ((Byte >= 0x61) && (Byte <= 0x7A)) { // If upper case letter
            Byte -= 32;
         }

         SBUF0 = Byte;                   // Transmit to Hyperterminal

         UART_Output_First++;            // Update counter

         UART_Buffer_Size--;             // Decrease array size

      }
      else
      {
         UART_Buffer_Size = 0;            // Set the array size to 0
         TX_Ready = 1;                    // Indicate transmission complete
      }
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------