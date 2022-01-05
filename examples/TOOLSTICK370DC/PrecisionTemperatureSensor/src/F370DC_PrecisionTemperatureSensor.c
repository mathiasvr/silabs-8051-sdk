//-----------------------------------------------------------------------------
// F370DC_PrecisionTemperatureSensor.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
//
// This program continuously measures the current die temperature in degrees
// Celsius obtained by the precision temperature sensor.
//
// The temperature sensor output is a 16-bit, two's complement number
// (1 sign bit, 8 integer bits, and 7 decimals bits) stored in
// TS0DATH:TS0DATL. 
//
// Note: only 4 of the 7 decimal bits are used in this code example
//
// How To Test:
//
// 1) Connect the ToolStick370DC to a ToolStick Base Adapter.
// 2) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 3) Set a break-point within the while(1) loop within the main function
//    after a temperature measurement is taken (line 142 or greater).
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8. The program should halt
//    at the set break-point.
// 5) Check the variables "temperature" for the measured temperature. The
//    temperature is held in this variable in thousandths of a degree:
//    for example, a value of 25000 would be equal to 25.000 degrees C.
// 6) Resume the code again to take another measurement.
//
// Target:         ToolStick370DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
//
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 20 MAY 2014
//
// Release 1.0
//    -Initial Revision (ST)
//    -27 JUL 2012
//

//-----------------------------------------------------------------------------
// Includes and Device-Specific Parameters
//-----------------------------------------------------------------------------

#include "SI_C8051F390_Register_Enums.h"
#include "stdio.h"

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
   int32_t temperature;
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
      SFRPAGE = PTEMPCFG_PAGE;

      TS0CN |= 0x80;    
      TS0CN &= ~0x80;                  // Start temp sensor measurement

      while ((TS0CN & 0x40)!= 0x40);   // Wait conversion to complete
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

      temperature = (temp_int * 1000) + (temp_frac / 10);
      if (sign_char == '-') {
         temperature *= -1;
      }

      //----------------------------------------
      // 3. Print temperature in Celsius to UART
      //----------------------------------------
      printf("%c%d.%04u degrees C\n", sign_char, (uint16_t)temp_int,
             temp_frac);
   }
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
   uint8_t SFRPAGE_save = SFRPAGE;
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
   OSCICN = 0x83;                      // Set system clock to 24.5 MHz
   RSTSRC = 0x04;                      // Enable missing clock detector
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
