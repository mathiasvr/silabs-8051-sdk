//-----------------------------------------------------------------------------
// F500_UART0_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program demonstrates how to configure the C8051F500 to write to and 
// read from the UART interface. The program reads a word using the UART 
// interrupts and outputs that word to the screen, with all characters in 
// uppercase.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    J17: P0.4 - UART_TX
//    J17: P0.5 - UART_RX
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 2) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 3) Connect a USB cable from the USB port P5 on development board to a PC.
//    Specify the target baudrate in the constant <BAUDRATE>
// 4) On the PC, open Hyperterminal, or a similar program, and connect to the
//    target board's serial port using <BAUDRATE> and 8-N-1 (CP210x USB-to-UART
//    VCP drivers must be installed).
// 5) Compile and download code to C8051F500 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 7) Please type up to 64 characters into the Terminal and press Enter.
//    The MCU will then print back the characters that were typed
//
//
// Target:         C8051F50x/1x (Side A of a C8051F500-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.2 / 11 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.1 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.0 / 10 MAR 2008 (GP)
//    -Initial Revision


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK           24000000      // SYSCLK frequency in Hz
#define BAUDRATE           115200      // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void PORT_Init (void);

SI_INTERRUPT_PROTO(UART0_ISR, UART0_IRQn);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

#define UART_BUFFERSIZE 64
uint8_t UART_Buffer[UART_BUFFERSIZE];
uint8_t UART_Buffer_Size = 0;
uint8_t UART_Input_First = 0;
uint8_t UART_Output_First = 0;
uint8_t TX_Ready = 1;
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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SFRPAGE = LEGACY_PAGE;              // Change for PCA0MD


   PORT_Init ();                       // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init ();                      // Initialize UART

   IE_EA = 1;                          // Enable global interrutpts

   while (1)
   {
      // If the complete word has been entered via the terminal followed by
      // carriage return

      if((TX_Ready == 1) && (UART_Buffer_Size != 0) && (Byte == 13))
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
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE; 

   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output
   XBR0    = XBR0_URT0E__ENABLED;      // Enable UART on P0.4(TX) and P0.5(RX)
   // Enable crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_save;
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
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE; 

   // Configure internal oscillator for its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;

   SFRPAGE = LEGACY_PAGE;
   RSTSRC  = 0x04;                     // Enable missing clock detector

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
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE; 

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       // clear RI0 and TI0 bits

 // Baud Rate = [BRG Clock / (65536 - (SBRLH0:SBRLL0))] x 1/2 x 1/Prescaler

#if   ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 1)
      SBRL0 = -(SYSCLK / BAUDRATE / 2);
      SBCON0 |= 0x03;                  // Set prescaler to 1
#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 4)
      SBRL0 = -(SYSCLK / BAUDRATE / 2/ 4);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x01;                  // Set prescaler to 4
#elif ((SYSCLK / BAUDRATE / 2 / 0xFFFF) < 12)
      SBRL0 = -(SYSCLK / BAUDRATE / 2 / 12);
      SBCON0 &= ~0x03;                 // Set prescaler to 12
#else
      SBRL0 = -(SYSCLK / BAUDRATE / 2 / 48);
      SBCON0 &= ~0x03;
      SBCON0 |= 0x02;                  // Set prescaler to 48
#endif

   SBCON0 |= SBCON0_SBRUN__ENABLED;    // Enable baud rate generator
   IE_ES0 = 1;                         // Enable UART0 interrupts

   TX_Ready = 1;                       // Flag showing that UART can transmit

   SFRPAGE = SFRPAGE_save;
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
         if (UART_Buffer_Size == UART_Input_First) 
         {
              UART_Output_First = 0;  
         }

         // Store a character in the variable byte
         Byte = UART_Buffer[UART_Output_First];

         if ((Byte >= 0x61) && (Byte <= 0x7A))  // If upper case letter
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