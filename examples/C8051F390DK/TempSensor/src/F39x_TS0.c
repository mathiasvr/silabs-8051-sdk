//-----------------------------------------------------------------------------
// F39x_TS0.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program continuously displays the current die temperature in degrees  
// Celsius obtained by the precision temperature sensor.
//
// The temperature sensor output is a 16-bit, two's complement number
// (1 sign bit, 8 integer bits, and 7 decimals bits) stored in
// TS0DATH:TS0DATL. 
//
// Note: only 4 of the 7 decimal bits are used in this code example.
//       Usually we'll see about 0.3~1C error to ambient temperature.
//       The error will be small if the sensor of thermometer is put on 
//       cover of MCU.
//
// 
// How To Test:
//
// 1) Ensure that jumpers are placed on J14 of the C8051F390/370 UDP MCU
//    board that connect P0.4 to the MCU_TX, and P0.5 to the MCU_RX.
// 2) Connect USB cable from the development board to a PC
// 3) Specify the target baudrate in the constant <BAUDRATE>.
// 4) Open Hyperterminal, or a similar program, and connect to the target
//    board's USB port (virtual com port).
// 5) Download and execute code to the 'F390/370 UDP MCU board.
//
//
// Target:         C8051F39x/37x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.3
//    - Put MCU into low power mode(0.1mA) before starting TS0 (SY)
//    - 15 Oct 2014
// Release 1.2
//    - Using 4 of the 7 decimal bits of the TS0 output (ST)
//    - 15 JUL 2012
//
// Release 1.1
//    - Added comments and formatting (ST)
//    - 15 JUN 2012
//
// Release 1.0
//    - Initial Revision (BD)
//

//-----------------------------------------------------------------------------
// Includes and Device-Specific Parameters
//-----------------------------------------------------------------------------

#include <SI_C8051F390_Register_Enums.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK    49000000/2           // SYSCLK frequency in Hz
#define BAUDRATE      115200           // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void PORT_Init (void);
void TS0_Init (void);
void EnterLowPowerMode(void);


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
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   SI_UU32_t temp_reading;                  
   uint16_t hexval;                         // Holds raw temperature sensor output      
   uint8_t temp_int;                        // Holds integer portion of temperature
                                       //   in decimal (degrees C)
   uint16_t temp_frac;                      // Holds decimal portion of temperature 
                                       //   in decimal (degrees C)
   uint8_t sign_char;                       // '-' if the temperature is negative

                                       // enable)

   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init();
   TS0_Init();

   while(1)
   {
      //-----------------------
      // 1. Obtain temperature
      //-----------------------      
      // Put MCU into low power mode (about 0.1mA) for a few second
	  // If MCU works at 24.5MHz, Idd is about 5mA, the temperature is about 1.5C higher
      EnterLowPowerMode();
      hexval = 4000;
	  while(--hexval) ;



      SFRPAGE = PTEMPCFG_PAGE;

      TS0CN |= 0x80;    
      TS0CN &= ~0x80;                  // Start temp sensor measurement

      while ((TS0CN & 0x40)!= 0x40);   // Wait conversion to complete



      SYSCLK_Init ();                   // Initialize Oscillator
      TCON_TR1 = 1;                     // Resume Timer1 for UART

      TS0CN &= ~0x40;

      SFRPAGE = PTEMPDAT_PAGE;

      hexval = TS0DAT;                 // Store raw temperature sensor output

      //--------------------------------------
      // 2. Format temperture for UART output
      //--------------------------------------
      temp_reading.u32 = hexval;  

      temp_frac = temp_reading.u8[B0]>>3;    // Store only 4 of 7 decimal bits
      temp_reading.u32 = temp_reading.u32 << 1;

      // Convert raw temperature sensor value to decimal for UART output
      if (temp_reading.u8[B2])
      {
         sign_char = '-';                    // Temperature is negative
         temp_int = -(temp_reading.s8[B1]);  // Decimal value of integer
         temp_frac = 0x0F & (-temp_frac);    
         temp_frac = 625 * (temp_frac);      // Decimal value of fraction
         if (temp_frac != 0)
         {
            temp_int--;
         }
      }
      else
      {
         sign_char = ' ';                    // Temperature is postive
         temp_int = temp_reading.u8[B1];     // Decimal value of integer
         temp_frac = 0x0F & temp_frac;
         temp_frac = 625 * (temp_frac);      // Decimal value of fraction 
      }

      //--------------------------------------------------------
      // 3. Print temperature in Celsius and raw hex value to UART
      //--------------------------------------------------------
      printf("%c%d.%04u\t0x%04X\n", sign_char, (uint16_t)temp_int, temp_frac,
             hexval);
      SFRPAGE = LEGACY_PAGE;
   }
}

void EnterLowPowerMode(void)
{
    // Stop Timer 0
    TCON_TR1 = 0;
    // Change SYSCLK to LF_OSC/8(=10KHz)
    OSCLCN    |= 0x80;
    while ((OSCLCN & 0x40) == 0);
    CLKSEL    = 0x02;
	// Disable high frequency oscillator
	OSCICN = 0x00;
}


//-----------------------------------------------------------------------------
// TS0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the precision temperature sensor
//
//-----------------------------------------------------------------------------

void TS0_Init (void)
{
   unsigned char SFRPAGE_save = SFRPAGE;
   SFRPAGE = PTEMPCFG_PAGE;

   TS0CN = 0x07;                       // TS length = max

   SFRPAGE = SFRPAGE_save;
}


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
   XBR0    = 0x01;                     // Enable UART on P0.4(TX) and P0.5(RX)
   XBR1    = 0x40;                     // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 49 MHz
// oscillator divided by 2 as its clock source.  Also enables missing clock 
// detector reset.
//
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   unsigned char SFRPAGE_save = SFRPAGE;
   SFRPAGE = LEGACY_PAGE;
   OSCICN = 0x83;                      // Set system clock to 24.5 MHz
   RSTSRC = 0x04;                      // Enable missing clock detector
   CLKSEL = 0;                         // Select Internal High Frequency OSC
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
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON0_RI and SCON0_TI bits
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
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
