//-----------------------------------------------------------------------------
// F35x_SPI0_EEPROM_Polled_Mode.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
// This program accesses a SPI EEPROM using polled mode access. The 'F35x MCU
// is configured in 4-wire Single Master Mode, and the EEPROM is the only
// slave device connected to the SPI bus. The read/write operations are
// tailored to access a Microchip 4 kB EEPROM 25LC320. The relevant hardware
// connections of the 'F35x MCU are shown here:
//
// P0.0 - SPI SCK    (digital output, push-pull)
// P0.1 - SPI MISO   (digital input,  open-drain)
// P0.2 - SPI MOSI   (digital output, push-pull)
// P0.3 - SPI NSS    (digital output, push-pull)
// P0.4 - UART TXD   (digital output, push-pull)
// P0.5 - UART RXD   (digital input,  open-drain)
// P0.6 - LED        (digital output, push-pull)
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are installed on the following:
//    - J8 : Power selection, P1 9V or J4 Debugger
//    - J3 : P0.4 - TX
//    - J3 : P0.5 - RX
//    - J3 : P0.6 - P0.6_LED
//    - J9 : P0.2
//    - J10: P0.3
// 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
//    connect the 9V power adapter to P1. After power the board, the red LED D3
//    is on.
// 3) Connect SPI bus to a Microchip 4 kB EEPROM 25LC320 SPI EEPROM.
// 4) Connect serial cable from the RS232 connector J5 to a PC.
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the virtual COM port at <BAUDRATE> (115200) and 8-N-1.
// 6) Compile and download code to the C8051F350-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 7) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 8) HyperTerminal will print the progress of the write/read operation, and in
//    the end will print the test result as pass or fail. Additionally, if the
//    verification passes, the LED will blink slowly. If it fails, the LED will
//    be OFF.
//
//
// Target:         C8051F35x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 26 JUN 2014
//
// Release 1.0
//    -Initial Revision (PKC)
//    -18 MAY 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F350_Register_Enums.h>
#include <stdio.h>                     // printf is declared here

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define BAUDRATE           115200      // Baud rate of UART in bps
#define SYSCLK             24500000    // Internal oscillator frequency in Hz

// Microchip 25LC320 Slave EEPROM Parameters
#define  F_SCK_MAX         2000000     // Max SCK freq (Hz)
#define  T_NSS_DISABLE_MIN 500         // Min NSS disable time (ns)
#define  EEPROM_CAPACITY   4096        // EEPROM capacity (bytes)

// EEPROM Instruction Set
#define  EEPROM_CMD_READ   0x03        // Read Command
#define  EEPROM_CMD_WRITE  0x02        // Write Command
#define  EEPROM_CMD_WRDI   0x04        // Reset Write Enable Latch Command
#define  EEPROM_CMD_WREN   0x06        // Set Write Enable Latch Command
#define  EEPROM_CMD_RDSR   0x05        // Read Status Register Command
#define  EEPROM_CMD_WRSR   0x01        // Write Status Register Command

SI_SBIT(LED, SFR_P0, 6);                 // LED0='1' means ON

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

void Delay_us (uint8_t time_us);
void Delay_ms (uint8_t time_ms);
void EEPROM_Write (uint16_t address, uint8_t value);
uint8_t EEPROM_Read (uint16_t address);

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
   PCA0MD &= ~PCA0MD_WDTE__ENABLED;    // Disable the Watchdog Timer
}
 
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
void main (void)
{
   uint16_t address;                        // EEPROM address
   uint8_t  test_byte;                      // Used as a temporary variable

   Init_Device ();                     // Initializes hardware peripherals


   // The following code will test the EEPROM by performing write/read/verify
   // operations. The first test will write 0xFFs to the EEPROM, and the
   // second test will write the LSBs of the EEPROM addresses.

   // Fill EEPROM with 0xFF's
   LED = 1;
   printf("Filling with 0xFF's...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = 0xFF;
      EEPROM_Write (address, test_byte);

      // Print status to UART0
      if ((address % 16) == 0)
      {
         printf ("\nWriting 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED = ~LED;
      }
      else
      {
         printf ("%02x ", (uint16_t)test_byte);
      }
   }

   // Verify EEPROM with 0xFF's
   printf("\n\nVerifying 0xFF's...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = EEPROM_Read (address);

      // Print status to UART0
      if ((address % 16) == 0)
      {
         printf ("\nVerifying 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED = ~LED;
      }
      else
         printf ("%02x ", (uint16_t)test_byte);
      if (test_byte != 0xFF)
      {
         LED = 0;
         printf ("Error at %u\n", address);
         while (1);                    // Stop here on error (for debugging)
      }
   }

   // Fill EEPROM with LSB of EEPROM addresses
   printf("\n\nFilling with LSB of EEPROM addresses...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = address & 0xFF;
      EEPROM_Write (address, test_byte);

      // Print status to UART0
      if ((address % 16) == 0)
      {
         printf ("\nWriting 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED = ~LED;
      }
      else
      {
         printf ("%02x ", (uint16_t)test_byte);
      }
   }

   // Verify EEPROM with LSB of EEPROM addresses
   printf("\n\nVerifying LSB of EEPROM addresses...\n");
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = EEPROM_Read (address);

      // print status to UART0
      if ((address % 16) == 0)
      {
         printf ("\nVerifying 0x%04x: %02x ", address, (uint16_t)test_byte);
         LED = ~LED;
      }
      else
         printf ("%02x ", (uint16_t)test_byte);

      if (test_byte != (address & 0xFF))
      {
         LED = 0;
         printf ("Error at %u\n", address);
         while (1);                    // Stop here on error (for debugging)
      }
   }

   printf ("\n\nVerification success!\n");

   while (1)                           // Loop forever
   {
      LED = ~LED;                      // Flash LED when done (all verified)
      Delay_ms (200);
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
   // Set the internal oscillator to 24.5 MHz
   OSCICN = OSCICN_IOSCEN__ENABLED | OSCICN_IFCN__HFOSC_DIV_1;
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
// P0.0  -  SCK  (SPI0), Push-Pull,  Digital
// P0.1  -  MISO (SPI0), Open-Drain, Digital
// P0.2  -  MOSI (SPI0), Push-Pull,  Digital
// P0.3  -  NSS  (SPI0), Push-Pull,  Digital
// P0.4  -  TX0 (UART0), Push-Pull,  Digital
// P0.5  -  RX0 (UART0), Open-Drain, Digital
// P0.6  -  Skipped,     Push-Pull,  Digital (LED D2 on Target Board)
// P0.7  -  Skipped,     Push-Pull,  Digital (LED D1 on Target Board)

// P1.0  -  Skipped,     Open-Drain, Digital (Switch S2 on Target Board)
// P1.1  -  Unassigned,  Open-Drain, Digital
// P1.2  -  Unassigned,  Open-Drain, Digital
// P1.3  -  Unassigned,  Open-Drain, Digital
// P1.4  -  Unassigned,  Open-Drain, Digital
// P1.5  -  Unassigned,  Open-Drain, Digital
// P1.6  -  Unassigned,  Open-Drain, Digital
// P1.7  -  Unassigned,  Open-Drain, Digital
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   P0MDOUT = 0xDD;

   // Enable the SPI and UART on the XBAR
   XBR0 = XBR0_SPI0E__ENABLED | XBR0_URT0E__ENABLED;

   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;
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
   CKCON |= CKCON_T2ML__SYSCLK;
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

   TL1 = TH1;                             // Init Timer1
   TMOD &= ~(TMOD_GATE1__BMASK |
             TMOD_CT1__BMASK   |
             TMOD_T1M__FMASK);
   // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  TMOD_T1M__MODE2;

   TCON_TR1 = 1;                          // START Timer1

   SCON0_TI = 1;                          // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// SPI0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configures SPI0 to use 4-wire Single-Master mode. The SPI timing is
// configured for Mode 0,0 (data centered on first edge of clock phase and
// SCK line low in idle state). The SPI clock is set to 1.75 MHz. The NSS pin
// is set to 1.
//
//-----------------------------------------------------------------------------
void SPI0_Init()
{
   // Enable the SPI as a Master, CKPHA = '0', CKPOL = '0'
   SPI0CFG = SPI0CFG_MSTEN__MASTER_ENABLED |
             SPI0CFG_CKPHA__DATA_CENTERED_FIRST |
             SPI0CFG_CKPOL__IDLE_LOW;

   // 4-wire Single Master, SPI enabled
   SPI0CN = SPI0CN_NSSMD__4_WIRE_MASTER_NSS_HIGH |
            SPI0CN_SPIEN__ENABLED;

   // The equation for SPI0CKR is (SYSCLK/(2*F_SCK_MAX))-1, but this yields
   // a SPI frequency that is slightly more than 2 MHz. But, 2 MHz is the max
   // frequency spec of the EEPROM used here. So, the "-1" term is omitted
   // in the following usage:
   SPI0CKR = (SYSCLK/(2*F_SCK_MAX));
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
   PCA0_Init ();
   OSCILLATOR_Init ();
   PORT_Init ();
   TIMER2_Init ();
   UART0_Init ();
   SPI0_Init ();
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
   TMR2CN_TR2  = 0;                  // Stop timer

   TMR2CN_TF2H = 0;                  // Clear timer overflow flag

   TMR2 = -((uint16_t)(SYSCLK/1000000) * (uint16_t)(time_us));

   TMR2CN_TR2 = 1;                   // Start timer

   while (!TMR2CN_TF2H);             // Wait till timer overflow occurs

   TMR2CN_TR2 = 0;                   // Stop timer
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
   {
      for(i = 0; i< 10; i++)           // 10 * 100 microsecond delay
      {
         Delay_us (100);
      }
   }
}

//-----------------------------------------------------------------------------
// EEPROM_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : 1. address - the destination EEPROM address.
//                   range: 0 to EEPROM_CAPACITY
//                2. value - the value to write.
//                   range: 0x00 to 0xFF
//
// Writes one byte to the specified address in the EEPROM. This function polls
// the EEPROM status register after the write operation, and returns only after
// the status register indicates that the write cycle is complete. This is to
// prevent from having to check the status register before a read operation.
//
//-----------------------------------------------------------------------------
void EEPROM_Write (uint16_t address, uint8_t value)
{
   // Writing a byte to the EEPROM is a five-step operation.

   // Step1: Set the Write Enable Latch to 1
   SPI0CN_NSSMD0 = 0;                  // Step1.1: Activate Slave Select
   SPI0DAT = EEPROM_CMD_WREN;          // Step1.2: Send the WREN command
   while (!SPI0CN_SPIF);               // Step1.3: Wait for end of transfer
   SPI0CN_SPIF = 0;                    // Step1.4: Clear the SPI intr. flag
   SPI0CN_NSSMD0 = 1;                  // Step1.5: Deactivate Slave Select
   Delay_us (1);                       // Step1.6: Wait for at least
                                       //          T_NSS_DISABLE_MIN
   // Step2: Send the WRITE command
   SPI0CN_NSSMD0 = 0;
   SPI0DAT = EEPROM_CMD_WRITE;
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;

   // Step3: Send the EEPROM destination address (MSB first)
   SPI0DAT = (uint8_t)((address >> 8) & 0x00FF);
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;
   SPI0DAT = (uint8_t)(address & 0x00FF);
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;

   // Step4: Send the value to write
   SPI0DAT = value;
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;
   SPI0CN_NSSMD0 = 1;
   Delay_us (1);

   // Step5: Poll on the Write In Progress (WIP) bit in Read Status Register
   do
   {
      SPI0CN_NSSMD0 = 0;               // Activate Slave Select
      SPI0DAT = EEPROM_CMD_RDSR;       // Send the Read Status Register command
      while (!SPI0CN_SPIF);            // Wait for the command to be sent out
      SPI0CN_SPIF = 0;
      SPI0DAT = 0;                     // Dummy write to output serial clock
      while (!SPI0CN_SPIF);            // Wait for the register to be read
      SPI0CN_SPIF = 0;
      SPI0CN_NSSMD0 = 1;               // Deactivate Slave Select after read
      Delay_us (1);
   } while((SPI0DAT & 0x01) == 0x01 );
}

//-----------------------------------------------------------------------------
// EEPROM_Read
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
uint8_t EEPROM_Read (uint16_t address)
{
   // Reading a byte from the EEPROM is a three-step operation.

   // Step1: Send the READ command
   SPI0CN_NSSMD0 = 0;                  // Activate Slave Select
   SPI0DAT = EEPROM_CMD_READ;
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;

   // Step2: Send the EEPROM source address (MSB first)
   SPI0DAT = (uint8_t)((address >> 8) & 0x00FF);
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;
   SPI0DAT = (uint8_t)(address & 0x00FF);
   while (!SPI0CN_SPIF);
   SPI0CN_SPIF = 0;

   // Step3: Read the value returned
   SPI0DAT = 0;                        // Dummy write to output serial clock
   while (!SPI0CN_SPIF);               // Wait for the value to be read
   SPI0CN_SPIF = 0;
   SPI0CN_NSSMD0 = 1;                  // Deactivate Slave Select
   Delay_us (1);

   return SPI0DAT;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------