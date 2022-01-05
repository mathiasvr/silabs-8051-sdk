//-----------------------------------------------------------------------------
// F70x_EEPROM.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program accesses the F70x's on-chip EEPROM.  It uses low level
// read, write, and erase routines to write test values to EEPROM and then
// read those values back out.  If firmware detects that switch SW1 is
// pressed when the device comes out of reset, then a pattern is 
// written to EEPROM.  If the switch is not pressed when the device comes
// out of reset, the EEPROM is read.  The UART outputs the bytes written to
// or read from the EEPROM.
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J8:  P1.1 - SW1
//    - J12: P0.4 - RXD
//           P0.5 - TXD
//	  - J15: VDD - power source
// 2) Connect the USB Debug Adapter to P3.
// 3) Connect the USB cable to P4.
// 4) Specify the target baudrate in the constant <BAUDRATE>.
// 5) Open Hyperterminal, or a similar program, and connect to the target
//    board's serial port.
// 6) Compile and download code to the C8051F70x development board by
//    selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Verify the output.
//
// Target:         C8051F70x
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (PD)
//    -10 JUL 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "si_toolchain.h"
#include <SI_C8051F700_Defs.h>         // SFR declarations
#include <stdio.h>                     // printf is declared here


//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define BAUDRATE           230400      // Baud rate of UART in bps
#define SYSCLK             24500000    // Internal oscillator frequency in Hz

#define EEPROM_CAPACITY    32

#define  LED_ON             0
#define  LED_OFF            1

SI_SBIT(LED, SFR_P1, 0);                 // LED==LED_ON means ON
SI_SBIT(SW, SFR_P1, 1);                


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void PCA0_Init (void);
void OSCILLATOR_Init (void);
void PORT_Init (void);
void TIMER2_Init (void);
void UART0_Init (void);
void SPI0_Init (void);
void Init_Device (void);
void EEPROM_Init(void);
void SYSCLK_Init (void);               // Configure the system clock

void Delay_us (uint8_t time_us);
void Delay_ms (uint8_t time_ms);

void EEPROM_RAMWrite (uint8_t address, uint8_t value);
uint8_t EEPROM_RAMRead (uint8_t address);
uint8_t EEPROM_CopyRAMToEEPROM (void);
uint8_t EEPROM_CopyEEPROMToRAM (void);


//-----------------------------------------------------------------------------
// Generic UART definitions to support both Keil and SDCC
//-----------------------------------------------------------------------------

#ifdef SDCC
#include <sdcc_stdio.h>
#endif

char *GETS (char *, unsigned int);

#ifdef __C51__
#define GETS gets
#endif

#ifdef SDCC
#include <sdcc_stdio.c>
#endif


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
   WDTCN = 0xDE;                       // Disable watchdog timer
   WDTCN = 0xAD;
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   uint8_t  address;                        // EEPROM address
   uint8_t  test_byte;                      // Used as a temporary variable

   Init_Device ();                     // Initializes hardware peripherals

   Delay_ms (200);

   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = address;
      EEPROM_RAMWrite (address, test_byte);

      // Print status to UART0
      if ((address % 16) == 0)
      {
         printf ("\nWriting 0x%02bx: %02bx ", address, (uint8_t)test_byte);
         LED = !LED;
      }
      else
         printf ("%02bx ", (uint8_t)test_byte);

   }
   EEPROM_CopyRAMToEEPROM();        // Writes RAM values to EEPROM


   EEPROM_CopyEEPROMToRAM();        // Moves EEPROM values into RAM

   // Verify EEPROM with LSB of EEPROM addresses
   printf("\n\nVerifying LSB of EEPROM addresses...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = EEPROM_RAMRead (address);

      // print status to UART0
      if ((address % 16) == 0)
      {
         printf ("\nVerifying 0x%02bx: %02bx ", (uint8_t)address,
                 (uint8_t)test_byte);
         LED = !LED;
      }
      else
      {
         printf ("%02bx ", (uint8_t)test_byte);
      }            
   }

   while (1)
   {
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
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   PCA0MD    = 0x00;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal oscillator
// at 24.5 MHz.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x80;                     // Enable the precision internal osc.
      
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc. 
                                       // divided by 1 as the system clock

   SFRPAGE = SFRPAGE_save;
}
//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P0.4  -  TX0  (UART0), Push-Pull, Digital
// P0.5  -  RX0  (UART0), Open-Drain, Digital 
// P1.0  -  Skipped,     Push-Pull,  Digital (LED on Target Board)
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   P0MDOUT |= 0x10;                    // Make TX0 push-pull

   P1MDOUT = 0x01;                     // Make the LED push-pull
   P1SKIP = 0x01;                      // Skip the LED (P1.0)

   XBR0 |= 0x01;                       // Enable UART on P0.4(TX) and P0.5(RX)
   XBR1 = 0x40;                        // Enable the XBAR and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// TIMER2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initializes Timer2 to be clocked by SYSCLK for use as a delay timer.
//
//-----------------------------------------------------------------------------
void TIMER2_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   CKCON    |= 0x10;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configures the UART0 using Timer1, for <BAUDRATE> and 8-N-1. Once this is
// set up, the standard printf function can be used to output data.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
   #if (SYSCLK/BAUDRATE/2/256 < 1) 
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   #elif (SYSCLK/BAUDRATE/2/256 < 4) 
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   #elif (SYSCLK/BAUDRATE/2/256 < 12) 
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   #else 
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   #endif

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                       // START Timer1
   SCON0_TI = 1;                       // Indicate TX0 ready

   SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// This routine initializes the system clock to use the internal 24.5MHz / 8
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void SYSCLK_Init (void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x83;                      // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// EEPROM_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// 
//
//-----------------------------------------------------------------------------
void EEPROM_Init(void)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   EECN = 0x81;

   SFRPAGE = SFRPAGE_save;
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

   SYSCLK_Init();                      // Initialize the system clock
   EEPROM_Init();
   PCA0_Init ();
   OSCILLATOR_Init ();
   PORT_Init ();
   TIMER2_Init ();
   UART0_Init ();
}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Delay_us
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 1. time_us - time delay in microseconds
//                   range: 1 to 255
//
// Creates a delay for the specified time (in microseconds) using TIMER2. The
// time tolerance is approximately +/-50 ns (1/SYSCLK + function call time).
//
//-----------------------------------------------------------------------------
void Delay_us (uint8_t time_us)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   TMR2CN_TR2   = 0;                   // Stop timer
   TMR2CN_TF2H  = 0;                   // Clear timer overflow flag
   SFRPAGE = LEGACY_PAGE;
   TMR2  = -( (uint16_t)(SYSCLK/1000000) * (uint16_t)(time_us) );
   SFRPAGE = CONFIG_PAGE;
   TMR2CN_TR2   = 1;                   // Start timer
   while (!TMR2CN_TF2H);               // Wait till timer overflow occurs
   TMR2CN_TR2   = 0;                   // Stop timer

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Delay_ms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 1. time_ms - time delay in milliseconds
//                   range: 1 to 255
//
// Creates a delay for the specified time (in milliseconds) using TIMER2. The
// time tolerance is approximately +/-50 ns (1/SYSCLK + function call time).
//
//-----------------------------------------------------------------------------
void Delay_ms (uint8_t time_ms)
{
   uint8_t i;

   while(time_ms--)
      for(i = 0; i< 10; i++)           // 10 * 100 microsecond delay
         Delay_us (100);
}

//-----------------------------------------------------------------------------
// EEPROM_RAMWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 1. address - the destination EEPROM address.
//                   range: 0 to EEPROM_CAPACITY
//                2. value - the value to write.
//                   range: 0x00 to 0xFF
//
// 
//
//-----------------------------------------------------------------------------
void EEPROM_RAMWrite (uint8_t address, uint8_t value)
{
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;

   EEADR = address;
   EEDAT = value;

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// EEPROM_RAMRead
//-----------------------------------------------------------------------------
//
// Return Value : The value that was read from the EEPROM
//                   range: 0x00 to 0xFF
// Parameters   : 1. address - the source EEPROM address.
//                   range: 0 to EEPROM_CAPACITY
//
// Reads one byte from the specified EEPROM address.
//
//-----------------------------------------------------------------------------
uint8_t EEPROM_RAMRead (uint8_t address)
{
   uint8_t retval;
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = LEGACY_PAGE;

   EEADR = address;

   retval = EEDAT;

   SFRPAGE = SFRPAGE_save;

   return retval;
}

//-----------------------------------------------------------------------------
// EEPROM_CopyRAMToEEPROM
//-----------------------------------------------------------------------------
//
// Return Value : EEPSTATE value
//                00b = Write/Erase disabled
//                11b = Write/Erase locked
// Parameters   : None
//
// Writes 32 bytes of EEPROM-space RAM to non-volatile EEPROM.
//
//-----------------------------------------------------------------------------
uint8_t EEPROM_CopyRAMToEEPROM (void)
{
   uint8_t retval;
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   // Reading EEKEY gives the EESTATE value in bits 0 and 1
   // Writing two values to EEKEY unlocks EEPROM to writes
   // and erases

   switch (EEKEY & 0x03)
   {
      case (0x00): EEKEY = 0x55;       // Write/Erase is not enabled, write
                                       // first key byte and fall through
                                       // to next case

      case (0x01): EEKEY = 0xAA;       // First key byte has been written,
                                       // write second key and fall through
                                       // to next case
               
      case (0x02): EECN = 0x82;        // Write/Erase enabled and break;
                   break;


      case (0x03): break;              // Write/Erase locked, do nothing
         
   } // end switch

   retval = (EEKEY & 0x03);            // Read EESTATE

   SFRPAGE = SFRPAGE_save;

   return retval;
}

//-----------------------------------------------------------------------------
// EEPROM_CopyEEPROMToRAM
//-----------------------------------------------------------------------------
//
// Return Value : EEPSTATE value
//                00b = Write/Erase disabled
//                11b = Write/Erase locked
// Parameters   : None
//
// Copies non-volatile 32-byte on-chip EEPROM into EEPROM-space RAM.
//
//-----------------------------------------------------------------------------
uint8_t EEPROM_CopyEEPROMToRAM (void)
{

   uint8_t retval;
   // Save the current SFRPAGE
   uint8_t SFRPAGE_save = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   EECN |= 0x04;                       // Initiate 32-byte read from EEPROM
      
   retval = (EEKEY & 0x03);            // Read EESTATE

   SFRPAGE = SFRPAGE_save;

   return retval;
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
