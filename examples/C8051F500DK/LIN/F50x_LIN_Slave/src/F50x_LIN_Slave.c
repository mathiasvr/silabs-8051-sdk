//-----------------------------------------------------------------------------
// F50x_LIN_Slave.c
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
// This example is intended to be used with the LIN_Master example.
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
//    global constant after disabling AUTO_BAUD constant: LIN_BAUD_RATE
// 4) Compile and download code to C8051F500-TB (either device A or device B)
//    with Simplicity IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 5) Make sure the another device A or B is running the LIN0_Master code.
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
// Release 1.3 / 18 AUG 2014 (JL)
//    - Updated to use with Simplicity Studio
//
// Release 1.2 / 02 MAR 2010 (GP)
//    -Tested with Raisonance
//
// Release 1.1 / 03 SEP 2008 (GP)
//    -Fixed baud rate calculations
//
// Release 1.0 / 09 JUNE 2008 (ADT)
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

#define  AUTO_BAUD   0                 // 0 = disable autobaud
                                       // 1 = enable autobaud
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

#if (AUTO_BAUD == 0)                   // If running in manual baud rate mode

 #define LIN_MULTIPLIER (uint8_t) ((20000.0 / LIN_BAUD_RATE) - 1)

 #if (SYSCLK >= 16000000)
   #define LIN_PRESCALAR 1
   #define LIN_DIVIDER (uint16_t) (SYSCLK / ((uint32_t) 4 * (LIN_MULTIPLIER + 1) * (uint32_t) LIN_BAUD_RATE))
 #else
   #define LIN_PRESCALAR 0
   #define LIN_DIVIDER (uint16_t) (SYSCLK / ((uint32_t) 2 * (LIN_MULTIPLIER + 1) * (uint32_t) LIN_BAUD_RATE))
 #endif

#else                                  // If running in auto baud rate mode

   #if (SYSCLK >= 16000000)
      #define LIN_PRESCALAR 1
      #define LIN_DIVIDER (uint16_t)((uint32_t) SYSCLK / ((uint32_t) 4 * (uint32_t) 20000))
   #else
      #define LIN_PRESCALAR 0
      #define LIN_DIVIDER (uint16_t)((uint32_t) SYSCLK / ((uint32_t) 2 * (uint32_t) 20000))
   #endif

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

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);           // Configure the system clock
void Port_Init (void);                 // Configure the Crossbar and GPIO
void LIN0_Slave_Init (void);           // Configure LIN for slave

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
   LIN0_Slave_Init ();                 // Initialize LIN

   IE_EA = 1;                          // Enable Interrupts

   LED = 0;                            // Initialize LED to be off


   while (1);                          // Infinite while loop waiting for
                                       // instructions from Master
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

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
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
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
// P1.3  -  LED    ,     Push-Pull,  Digital
// P1.4  -  SW1   ,      Open-Drain, Digital
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
// LIN0_Slave_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Intializes the LIN peripheral to slave mode.
//
// LIN is an asynchronous, serial communications interface. All LIN registers
// besides the control mode register (LIN0CF) are indirectly accessed;
// the address is specified in the LINADDR register which points to a specific
// data register which can be accessed through the LINDATA register.
// Waveforms and additional specifications can be found in the LIN consortium
// (http:\\www.lin-subbus.com).
//-----------------------------------------------------------------------------
void LIN0_Slave_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

// Enable interface, configure the device for LIN Slave, maunal baud rate
   LIN0CF = 0x80;
#if (AUTO_BAUD == 1)
   LIN0CF |= 0x20;                     // Automatic baud rate
#endif

   SFRPAGE = LEGACY_PAGE;
   // Load the least significant byte of the divider.
   LIN0ADR = LIN0DIV;
   LIN0DAT = (uint8_t) LIN_DIVIDER;

   LIN0ADR = LIN0MUL;

#if (AUTO_BAUD == 0)
   // Load the multiplier, prescalar, and most significant bit of the divider.
   LIN0DAT = (uint8_t) ((LIN_PRESCALAR << 6) | (LIN_MULTIPLIER << 1)
               | (LIN_DIVIDER >> 8));
#else
   // Load the prescalar and most significant bit of the divider
   LIN0DAT = (uint8_t) ((LIN_PRESCALAR << 6) | (LIN_DIVIDER >> 8));
#endif

   LIN0ADR = LIN0SIZE;
   LIN0DAT = 0x80;                     // Enhanced checksum

   EIE1 |= EIE1_ELIN0__ENABLED;        // Enable LIN interrupts

   LIN0ADR = LIN0CTRL;
   LIN0DAT = 0x0C;                     // Reset LININT and LIN0ERR

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// LIN0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This ISR triggers when recieving a transmission from the master. Depending
// on what message is sent, LED P1^4_B will turn on/off. If the master sends
// a request frame, it will check the status of SW1 and send back the
// information.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(LIN0_ISR, LIN0_IRQn)
{
   uint8_t ID, status, error;
   static uint8_t num_error = 0;
   uint8_t SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;

   LIN0ADR = LIN0ST;
   status = LIN0DAT;

   if (status & LININTREQ)             // If LIN interrupt request
   {
      if(status & DTREQ)               // If ID field received
      {
         LIN0ADR = LIN0ID;
         ID = LIN0DAT;

         if ((ID & ID_LED_SLAVE) == ID_LED_SLAVE)  // ID is to mess with slave
         {
            LIN0ADR = LIN0CTRL;
            LIN0DAT &= ~TXRX;          // Select to receive data

            if (ID == ID_LED_SLAVE_ON)
            {
               LED = 1;
            }
            else if (ID == ID_LED_SLAVE_OFF)
            {
               LED = 0;
            }
         }
         else                          // ID is to mess with master LED
         {

            LIN0ADR = LIN0CTRL;
            LIN0DAT |= TXRX;           // Select to transmit data

            LIN0ADR = LIN0SIZE;
            LIN0DAT = (LIN0DAT & 0xF0) | 0x01;  //transmit 1 data byte

            LIN0ADR = LIN0DT1;

            if (SW1 == 0)              // Switch is pressed
            {
               LIN0DAT = LED_MASTER_ON;
            }
            else                       // Switch is not pressed
            {
               LIN0DAT = LED_MASTER_OFF;
            }

         }
      }
      else if (status & ERROR)         // If an error was detected
      {

         LIN0ADR = LIN0ERR;
         error = LIN0DAT;              // Check the error register
         // Keep track of the number of errors received
         num_error++;
         // Handle your errors here, application dependent

      }
      else if(status & DONE)           // If reception completed
      {
         // Transmission complete
      }
   }

   LIN0ADR = LIN0CTRL;
   LIN0DAT |= DTACK;                   // Acknowledge data

   LIN0ADR = LIN0CTRL;
   LIN0DAT |= 0x0C;                    // Reset interrupt request, error flag

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------