//-----------------------------------------------------------------------------
// F85x_Flash_Access.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// How to Use (Measure Current):
//
// 1) Remove the JP2 (Imeasure) shorting block and attach an ammeter between
//    the JP2 (Imeasure) headers.
// 2) Remove all other shorting blocks.
// 3) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Check the current initially
// 6) Hit S1 to write to flash or S2 to erase from flash.
// 7) Check the new current value.
// 7) The difference is the flash current.
//
// How to Use (Measure Time):
//
// 1) Remove all the shorting blocks except for those of JP2 (Imeasure), 
//    J27-S1 (P1.7), J27-S2 (P2.1), and LED1 (P1.0).
// 2) Connect an oscilloscope probe to GND and P1.0
// 3) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Use S1 (P1.7) for Flash Write or S2 (P2.1) for Flash Erase.
// 5a) When measuring for Flash Write, look at the time of a shift between 
//     high and low.
// 5b) When measuring for Flash Erase, divide the time of the pin staying low 
//     by 5 (the time accounts for all 5 pages erased).
//
// Target:        C8051F85x
// Target Board:   UPMU-F850 MCU-100
// Tool chain:    Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - 5 FEB 2014
//
// Release 1.0
//    - Initial Revision 
//    - 11 JULY 2013
//
//
// Refer to F85x_Flash_Access_Readme.txt for more information.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------

#include <SI_C8051F850_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24500000          // Clock speed in Hz
#define SW_PRESSED           0
#define SW_NOT_PRESSED       1
SI_SBIT(S1, SFR_P1, 7);
SI_SBIT(S2, SFR_P2, 1);
SI_SBIT(PIN00, SFR_P1, 0);

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);               // Configure the system clock
void Flash_Write(void);                // Flash Write Instruction
void Flash_Erase(void);                // Flash Erase Instruction
void Port_Init(void);                  // Initializing the Switches

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

void main (void)
{  
   uint8_t inputlatch = 0;
   uint8_t inputlatch2 = 0;
   Port_Init();
   SYSCLK_Init();
   while(1) 
   {
      if (S1 == SW_PRESSED)   // Flash Write Initialization Switch
      {
         inputlatch = 1;
      }

      if ((S1 == SW_NOT_PRESSED) && (inputlatch == 1))
      {
         Flash_Write();
         inputlatch = 0;
      }

      if (S2 == SW_PRESSED)   // Flash Erase Initialization Switch
      {
         inputlatch2 = 1;
      }

      if ((S2 == SW_NOT_PRESSED) && (inputlatch2 == 1))
      {
         Flash_Erase();
         inputlatch2 = 0;
      }

   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5 MHz
// oscillator divided by 1 as its clock source.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   CLKSEL = 0x00;                      // Set system clock to 24.5 MHz
}

//-----------------------------------------------------------------------------
// Port_Init
//
// Return Value : None
// Parameters   : None
//
//  This function configures the crossbar and GPIO ports.
//
// P1.7  digital  push-pull      Flash Write Initialization Switch
// P2.1  digital  push-pull      Flash Erase Initialization Switch
//-----------------------------------------------------------------------------

void Port_Init (void)
{
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   P1MDOUT &= ~0x80;
   P1 |= 0x80;
   P2MDOUT &= ~0x02;
   P2 |= 0x80;
}

//-----------------------------------------------------------------------------
// Runtime Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Flash_Erase
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function is used to erase data from flash with enough cycles for a user
// to properly measure the current/time.
//
//-----------------------------------------------------------------------------

void Flash_Erase (void)
{
   int16_t counter = 0;
   bit EA_SAVE = IE_EA;                   // Preserve IE_EA
   char xdata * data pwrite;           // Flash write pointer
   PIN00 = 1;                          // Initialize LED as off
   SYSCLK_Init();                      // Initialize the System Clock
   P1MDOUT |= 0x01;                    // Enable Pin 1.1 as an output
   XBR2 = 0x40;                        // Enable Crossbbar

   while(counter < 5) 
   {
   PIN00 = 0;                          // Turn LED on while flash is being erased
   counter++;
   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   pwrite = (char xdata *) 0x0F00;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = 0;                        // Initiate page erase

   PSCTL &= ~0x03;                     // PSWE = 0; PSEE = 0

   IE_EA = EA_SAVE;                       // Restore interrupts
   }
   PIN00 = !PIN00;                     // Turn LED Off when done
}

//-----------------------------------------------------------------------------
// Flash_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function is used to write data to flash with enough cycles for a user
// to properly measure the current/time.
//
//-----------------------------------------------------------------------------

void Flash_Write (void)
{
   int16_t counter = 0;
   bit EA_SAVE = IE_EA;                   // Preserve IE_EA
   char xdata * data pwrite;           // Flash write pointer
   PIN00 = 0;
   P1MDOUT |= 0x01;
   XBR2 = 0x40;

   while(counter < 25000) 
   {
   counter++;
   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   pwrite = (char xdata *) 0x0800;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x01;                      // PSWE = 1 which enables writes

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = 0xA5;                     // Write the byte

   PSCTL &= ~0x01;                     // PSWE = 0 which disable writes

   IE_EA = EA_SAVE;                       // Restore interrupts
   PIN00 = !PIN00;
   }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------