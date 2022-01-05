//-----------------------------------------------------------------------------
// F50x_LIN_Master.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program's purpose is to communicate between the A and B sides of the
// C8051F50x target board through the Local Interconnect Network (LIN).
//
// Pressing the P1.4_A switch will trigger the P1.3_B LED to light up.
// Releasing the P1.4_A switch will turn off the P1.3_B LED.
// Alternatively, pressing the P1.4_B switch will turn on the P1.3_A LED
// while releasing the P1.4_B switch will turn off the P1.3_A LED.
//
// This example is intended to be used with the LIN_Slave example.
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F500-TB are placed on the following:
//    Side A:
//    - J17: P1.0 - LIN_TX
//    - J17: P1.1 - LIN_RX
//    - J19: P1.3 LED
//    - J19: P1.4 - P1.4_SW
//    Side B:
//    - J26: P1.0 - LIN_TX
//    - J26: P1.1 - LIN_RX
//    - J11: P1.3 LED
//    - J11: P1.4 - P1.4_SW
//    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
// 2) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
//    connect the 9V power adapter to P2.
// 3) If you wish to change the baud rate of the LIN, change the following
//    global constant: LIN_BAUD_RATE
// 4) Compile and download code to C8051F500-TB (either device A or device B)
//    with Simplicity IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Make sure the another device A or B is running the LIN0_Slave code.
// 6) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 7) If the communication passes, when a switch on a side of the target board
//    is pressed, the LED on the opposite side will light up. When said switch
//    is released, the LED will turn off.
//
//
// Target:         C8051F50x/1x (C8051F500-TB)
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.4 / 18 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.3 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.2 / 10 SEP 2008 (GP)
//    -Changed LIN transmission to transmit at least one byte
//
// Release 1.1 / 03 SEP 2008 (GP)
//    -Fixed baud rate calculations
//
// Release 1.0 / 09 JUN 2008 (ADT)
//    -Initial Revision
//
// Obs1: When the master sends a request frame, the ID sent from the master
//       is the same as the ID read by the master after a slave transmit frame,
//       even if the slave tries to edit the ID before transmitting.
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F500_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

SI_SBIT(SW1, SFR_P1, 4);                 // SW1 = '0' means switch pressed
SI_SBIT(LED, SFR_P1, 3);                 // LED = '1' means LED on

#define SYSCLK 24000000L               // Clock speed in Hz

#if (SYSCLK < 8000000)                 // For the slowest baud rate
#error Minimum system clock needed to operate LIN peripheral is 8Mhz.
#endif

#define TWENTY_MS (uint16_t)((.02/(1.0/SYSCLK)) / 12.0) // Clock cycles for 20ms

#define  LIN_BAUD_RATE 19200           // Baud Rate to run LIN peripheral
                                       // 625 < LIN_BAUD_RATE < 20000

//-----------------------------------------------------------------------------
// LIN #defines
//-----------------------------------------------------------------------------

//------ Control register ------
#define  STREQ       0x01              // LIN Master Start transmission request
#define  WUPREQ      0x02              // Wake-Up request
#define  RSTERR      0x04              // Error reset request
#define  RSTINT      0x08              // Interrupt reset request
#define  DTACK       0x10              // Slave Data ACK
#define  TXRX        0x20              // Master selection of TX or RX mode
#define  SLEEP       0x40              // Slave flag set by master to inform
                                       // the peripheral that either a sleep
                                       // mode frame or bus idle timeout
                                       // was reached
#define  STOP        0x80              // Slave flag, to be set by the
                                       // application so no processing is to be
                                       // done until next SYNCH BREAK
//------ Status Register ------
#define  DONE        0x01              // Msg. processing completed
#define  WAKEUP      0x02              // Wake-Up signal
#define  ERROR       0x04              // Error detected
#define  LININTREQ   0x08              // Interrupt request
#define  DTREQ       0x10              // ID detected
#define  ABORT       0x20              // Abort requested
#define  IDLTOUT     0x40              // Time-out detected
#define  ACTIVE      0x80              // Interface active (communicating)

#define LIN_MULTIPLIER (uint8_t)((20000.0 / LIN_BAUD_RATE) - 1)

#if (SYSCLK >= 16000000)
 #define LIN_PRESCALAR 1
 #define LIN_DIVIDER (uint16_t) (SYSCLK / ((uint32_t) 4 * (LIN_MULTIPLIER + 1) * (uint32_t) LIN_BAUD_RATE))
#else
 #define LIN_PRESCALAR 0
 #define LIN_DIVIDER (uint16_t) (SYSCLK / ((uint32_t) 2 * (LIN_MULTIPLIER + 1) * (uint32_t) LIN_BAUD_RATE))
#endif

// Instruction Set
#define ID_SW1_SLAVE_QUERY 0x01
#define ID_LED_SLAVE       0x02        //LED on the slave side
#define ID_LED_SLAVE_OFF   0x02
#define ID_LED_SLAVE_ON    0x03

#define LED_MASTER_OFF     0x04
#define LED_MASTER_ON      0x05

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

SI_UU16_t Next_Interrupt_Time;              // Contains the next interrupt time that
                                       // PCA module 2 will try to match

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void PCA0_Init (void);                 // Configure the PCA
void LIN0_Master_Init (void);          // Configure LIN for master

SI_INTERRUPT_PROTO(PCA0_ISR, PCA0_IRQn);
SI_INTERRUPT_PROTO(LIN0_ISR, LIN0_IRQn);


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
   SFRPAGE = LEGACY_PAGE;


   Oscillator_Init ();                 // Initialize the system clock
   Port_Init ();                       // Initialize crossbar and GPIO
   PCA0_Init ();                       // Initialize PCA0
   LIN0_Master_Init ();                // Initialize LIN0

   IE_EA = 1;                          // Enable Interrupts

   LED = 0;                            // Initialize LED to be off

   while (1);
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Module 2 is configured as a match interrupt to be used as a periodic
// interrupt every 20 ms.
//-----------------------------------------------------------------------------
void PCA0_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   // Configure PCA time base; overflow interrupt disabled
   PCA0CN = 0x00;                      // Stop counter; clear all flags
   PCA0MD = 0x00;                      // Use SYSCLK / 12 as time base

   PCA0CPM2 = 0x49;                    // Configure PCA0 module 2 to interrupt
                                       // periodically

   PCA0L = 0x00;                       // Set PCA0 counter to 0
   PCA0H = 0x00;

   // Initialize PCA0 module 2 to interrupt in 20 ms
   Next_Interrupt_Time.u16 = TWENTY_MS;
   PCA0CPL2 = Next_Interrupt_Time.u8[LSB];
   PCA0CPH2 = Next_Interrupt_Time.u8[MSB];

   EIE1 |= 0x08;                       // Enable PCA interrupts

   PCA0CN_CR = 1;                      // Start PCA counter

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configure the internal oscillator to maximum internal frequency of 24 Mhz
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   // Set internal oscillator to run at its maximum frequency
   OSCICN = OSCICN_IOSCEN__ENABLED |
            OSCICN_IFCN__HFOSC_DIV_1;

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// Port_Init *EDIT THIS SECTION*
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.

// P0.0  -  Skipped,     Open-Drain, Digital
// P0.1  -  Skipped,     Open-Drain, Digital
// P0.2  -  Skipped,     Open-Drain, Digital
// P0.3  -  Skipped,     Open-Drain, Digital
// P0.4  -  Skipped,     Open-Drain, Digital
// P0.5  -  Skipped,     Open-Drain, Digital
// P0.6  -  Skipped,     Open-Drain, Digital
// P0.7  -  Skipped,     Open-Drain, Digital
// P1.0  -  TX    (LIN), Push-Pull,  Digital
// P1.1  -  RX    (LIN), Open-Drain, Digital
// P1.2  -  Unassigned,  Open-Drain, Digital
// P1.3  -  LED,         Push-Pull,  Digital
// P1.4  -  SW1,         Open-Drain, Digital
// P1.5  -  Unassigned,  Open-Drain, Digital
// P1.6  -  Unassigned,  Open-Drain, Digital
// P1.7  -  Unassigned,  Open-Drain, Digital
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P1MDOUT   = 0x09;                   // Set P1.0 to push-pull
                                       // Set P1.1 to open-drain
                                       // Set P1.3 to push-pull
                                       // Set P1.4 to open-drain

   P0SKIP    = 0xFF;                   // Skip to place LIN on 1.0 and 1.1

   // Enable LIN0, crossbar and weak pull-ups
   XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED |
          XBR2_XBARE__ENABLED |
          XBR2_LIN0E__ENABLED;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// LIN0_Master_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Intializes the LIN peripheral to master mode.
//
// LIN is an asynchronous, serial communications interface. All LIN registers
// besides the control mode register (LIN0CF) are indirectly accessed;
// the address is specified in the LINADDR register which points to a specific
// data register which can be accessed through the LINDATA register.
// Waveforms and additional specifications can be found in the LIN consortium
// (http:\\www.lin-subbus.com).
//-----------------------------------------------------------------------------
void LIN0_Master_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   // Enable interface, configure the device for LIN Master, manual baud rate
   LIN0CF = 0x80;
   LIN0CF |= 0x40;

   SFRPAGE = LEGACY_PAGE;
   // Load the least significant byte of the divider.
   LIN0ADR = LIN0DIV;
   LIN0DAT = (uint8_t) LIN_DIVIDER;

   // Load the multiplier, prescalar, and most significant bit of the divider.
   LIN0ADR = LIN0MUL;
   LIN0DAT = (uint8_t) ((LIN_PRESCALAR << 6) | (LIN_MULTIPLIER << 1)
               | (LIN_DIVIDER >> 8));

   LIN0ADR = LIN0SIZE;
   LIN0DAT = 0x80;                     // Enhanced checksum

   LIN0ADR = LIN0CTRL;
   LIN0DAT = 0x0C;                     // Reset LININT and LIN0ERR

   EIE1 |= EIE1_ELIN0__ENABLED;        // Enable  LIN interrupts

   SFRPAGE = SFRPAGE_save;

}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// This is the ISR for the PCA0. Module 2 will send a LIN transmit or request
// frame every 20ms to the slave. If there is a falling edge on the master
// switch, meaning that the switch was just pressed, send a LED on transfer.
// If there is a rising edige on the master switch, meaning that the switch
// was just released, send a LED off transfer. If the switch was in the same
// state as it was previously, meaning that the switch is either not pressed
// or is being held down, send a request frame to retreive the status of the
// slave switch.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(PCA0_ISR, PCA0_IRQn)
{
   static uint8_t sw1prev = 0;              // Status of the switch during the last
                                       // interrupt
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   if (PCA0CN_CCF2)
   {
      PCA0CN_CCF2 = 0;                 // Clear other interrupt flags for PCA

      // Interrupt every 20 ms
      Next_Interrupt_Time.u16 = Next_Interrupt_Time.u16 + TWENTY_MS;

      PCA0CPL2 = Next_Interrupt_Time.u8[LSB];
      PCA0CPH2 = Next_Interrupt_Time.u8[MSB];

      if ((sw1prev == 0) && (SW1 == 1))
      {
         // Button was down and switch is now off = rising edge
         // Turn off LED
         LIN0ADR = LIN0CTRL;
         LIN0DAT |= TXRX;              // Select to transmit data

         LIN0ADR = LIN0ID;
         LIN0DAT = ID_LED_SLAVE_OFF;   // Tell slave to turn off LED

         // Data byte not used, but necessary to prevent slave error
         LIN0ADR = LIN0SIZE;
         LIN0DAT = (LIN0DAT & 0xF0) | 0x01;   // Transmitting 1 byte

         LIN0ADR = LIN0CTRL;
         LIN0DAT |= STREQ;             // Start Request
      }
      else if ((sw1prev == 1) && (SW1 == 0))
      {
         // Button was up and switch is now pressed = falling edge
         // Turn on LED
         LIN0ADR = LIN0CTRL;
         LIN0DAT |= TXRX;              // Select to transmit data

         LIN0ADR = LIN0ID;
         LIN0DAT = ID_LED_SLAVE_ON;    // Tell slave to turn on LED

         // Data byte not used, but necessary to prevent slave error
         LIN0ADR = LIN0SIZE;
         LIN0DAT = (LIN0DAT & 0xF0) | 0x01;   // Transmitting 1 byte

         LIN0ADR = LIN0CTRL;
         LIN0DAT |= STREQ;             // Start Request
      }
      else if ((sw1prev == SW1))
      {
         // Same state as last state, covers 00 and 11
         // Poll slave switch
         LIN0ADR = LIN0CTRL;
         LIN0DAT &= ~TXRX;             // Create a master receive operation

         LIN0ADR = LIN0ID;
         LIN0DAT = ID_SW1_SLAVE_QUERY; // Want to know status of slave switch

         LIN0ADR = LIN0SIZE;
         LIN0DAT = (LIN0DAT & 0xF0) | 0x01;   // Transmitting 1 byte

         LIN0ADR = LIN0CTRL;
         LIN0DAT |= STREQ;             // Start Request
      }

   }
   else
   {
      // Clear other interrupt flags for PCA
      PCA0CN &= ~( PCA0CN_CF__BMASK   |
                   PCA0CN_CCF5__BMASK |
                   PCA0CN_CCF4__BMASK |
                   PCA0CN_CCF3__BMASK |
                   PCA0CN_CCF1__BMASK |
                   PCA0CN_CCF0__BMASK );
   }

   sw1prev = SW1;                      // Save value of switch
   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// LIN0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This ISR triggers when completing/receiving a transmission from the slave.
// Depending on what message is sent, LED (P1.3) on the slave will turn on/off.
// When receiving a transmission from the slave, the information in data byte
// 1 will tell the master the state of the switch on the slave side. The master
// will then toggle the LED (P1.3) on the master side accordingly.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(LIN0_ISR, LIN0_IRQn)
{
   uint8_t ctrl, ID, command, status, error;
   static uint8_t num_error = 0;
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   LIN0ADR = LIN0ST;                   // Retrieve status
   status = LIN0DAT;

   LIN0ADR = LIN0CTRL;
   ctrl = LIN0DAT;

   LIN0ADR = LIN0ID;
   ID = LIN0DAT;

   if (status & LININTREQ)
   {
      if (status & ERROR)              // If an error was detected
      {
         LIN0ADR = LIN0ERR;
         error = LIN0DAT;              // Check the error register
         // Keep track of the number of errors received
         num_error++;
         // Handle your errors here, application dependent
      }
      else if(status & DONE)           // If transmission/reception completed
      {
         // If in recieve mode and ID is correct
         if (((ctrl & TXRX) == 0) && (ID == ID_SW1_SLAVE_QUERY))
         {
            LIN0ADR = LIN0DT1;
            command = LIN0DAT;         // Get info from transmission

            if (command == LED_MASTER_ON)
             {
                LED = 1;
             }
            if (command == LED_MASTER_OFF)
            {
                LED = 0;
            }
         }
      }
   }

   LIN0ADR = LIN0CTRL;
   LIN0DAT |= 0x0C;                    // Reset LININT and LIN0ERR

   SFRPAGE = SFRPAGE_save;

}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------