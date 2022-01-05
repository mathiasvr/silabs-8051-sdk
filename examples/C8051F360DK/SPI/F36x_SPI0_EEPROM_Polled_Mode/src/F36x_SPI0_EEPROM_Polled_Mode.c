//-----------------------------------------------------------------------------
// F36x_SPI0_EEPROM_Polled_Mode.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// This program accesses a SPI EEPROM using polled mode access. The 'F36x MCU
// is configured in 4-wire Single Master Mode, and the EEPROM is the only
// slave device connected to the SPI bus. The read/write operations are
// tailored to access a Microchip 4 kB EEPROM 25LC320. The relevant hardware
// connections of the 'F36x MCU are shown here:
//
// P0.0 - SPI SCK    (digital output, push-pull)
// P0.1 - SPI MISO   (digital input,  open-drain)
// P0.2 - SPI MOSI   (digital output, push-pull)
// P0.3 - SPI NSS    (digital output, push-pull)
//
// P3.2 - LED        (digital output, push-pull)
//
//
// How To Test:
// ------------
// 1) Ensure shorting blocks on C8051F360 TB are placed on the following:
//    J12: P3.2 - P3.2_LED
//    J1: Power selection, P2 9V, P4 USB or J9 Debugger
// 2) Connect the USB Debug Adapter to J9. If 9V power is selected on J1,
//    connect the 9V power adapter to P2. If VBUS is selected on J1, connect
//    USB cable to P4.
// 3) Verify J15 is NOT populated on the Target Board.
// 4) Connect SPI bus to a Microchip 4 kB EEPROM 25LC320 SPI EEPROM.
// 5) Compile and download code to C8051F360 target board with Simplicity
//    IDE by selecting Run -> Debug from the menus,
//    or clicking the Debug button in the quick menu,
//    or pressing F11.
// 6) Run the code by selecting Run -> Resume from the menus,
//    or clicking the Resume button in the quick menu,
//    or pressing F8.
// 7) The LED will blink fast during the write/read/verify operations.
//    If the verification passes, the LED will blink slowly. If it fails,
//    the LED will be OFF.
//
//
// Target:         C8051F36x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Release 1.1
//    - Updated to use with Simplicity Studio (JL)
//    - 29 JUL 2014
//
// Release 1.0
//    -Initial Revision (PKC / TP)
//    -16 OCT 2006


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F360_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

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

SI_SBIT(LED, SFR_P3, 2);                  // LED='1' means ON

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void Oscillator_Init (void);
void Port_Init (void);
void Timer2_Init (void);
void SPI0_Init (void);

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


   Oscillator_Init ();
   Port_Init ();
   Timer2_Init ();
   SPI0_Init ();


   // The following code will test the EEPROM by performing write/read/verify
   // operations. The first test will write 0xFFs to the EEPROM, and the
   // second test will write the LSBs of the EEPROM addresses.

   // Fill EEPROM with 0xFF's
   LED = 1;
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = 0xFF;
      EEPROM_Write (address, test_byte);

      if ((address % 16) == 0)
      {
         LED = !LED;
      }
   }

   // Verify EEPROM with 0xFF's
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = EEPROM_Read (address);

      if ((address % 16) == 0)
      {
         LED = !LED;
      }

      if (test_byte != 0xFF)
      {
         LED = 0;
         while (1);                    // Stop here on error (for debugging)
      }
   }

   // Fill EEPROM with LSB of EEPROM addresses
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = address & 0xFF;
      EEPROM_Write (address, test_byte);

      if ((address % 16) == 0)
      {
         LED = !LED;
      }
   }

   // Verify EEPROM with LSB of EEPROM addresses
   for (address = 0; address < EEPROM_CAPACITY; address++)
   {
      test_byte = EEPROM_Read (address);

      if ((address % 16) == 0)
      {
         LED = !LED;

         Delay_ms(1000);
      }

      if (test_byte != (address & 0xFF))
      {
         LED = 0;
         while (1);                    // Stop here on error (for debugging)
      }
   }

   while (1)                           // Loop forever
   {
      LED = !LED;                      // Flash LED when done (all verified)
      Delay_ms (150);
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   // Set the internal oscillator to 24.5 MHz
   OSCICN = OSCICN_IOSCEN__ENABLED | OSCICN_IFCN__HFOSC_DIV_1;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
// P0.0  -  SCK  (SPI0), Push-Pull,  Digital
// P0.1  -  MISO (SPI0), Open-Drain, Digital
// P0.2  -  MOSI (SPI0), Push-Pull,  Digital
// P0.3  -  NSS  (SPI0), Push-Pull,  Digital
//
// P3.2  -  Skipped,     Push-Pull,  Digital (LED on Target Board)
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   P0MDOUT  = 0x0D;                    // Enable SCK, MOSI, NSS as push-pull
   P3MDOUT  = 0x04;                    // Enable LED (P3.2) as push-pull

   P3SKIP   = 0x04;                    // Skip LED (P3.2)

   // Enable the SPI on the XBAR
   XBR0 = XBR0_SPI0E__ENABLED;
   // Enable crossbar and weak pull-ups
   XBR1 = XBR1_WEAKPUD__PULL_UPS_ENABLED | XBR1_XBARE__ENABLED;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initializes Timer2 to be clocked by SYSCLK for use as a delay timer.
//
//-----------------------------------------------------------------------------
void Timer2_Init (void)
{
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

   CKCON  |= CKCON_T2ML__SYSCLK;

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
   uint8_t SFRPAGE_save = SFRPAGE;          // Save the current SFRPAGE

   SFRPAGE = CONFIG_PAGE;              // Switch to the necessary SFRPAGE

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

   SFRPAGE = SFRPAGE_save;             // Restore the SFRPAGE
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
// Creates a delay for the specified time (in microseconds) using Timer2. The
// time tolerance is approximately +/-50 ns (1/SYSCLK + function call time).
//
//-----------------------------------------------------------------------------
void Delay_us (uint8_t time_us)
{
   TMR2CN_TR2  = 0;                  // Stop timer

   TMR2CN_TF2H = 0;                  // Clear timer overflow flag

   TMR2 = -((uint16_t)(SYSCLK/1000000) * (uint16_t)(time_us) );

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
// Creates a delay for the specified time (in milliseconds) using Timer2. The
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