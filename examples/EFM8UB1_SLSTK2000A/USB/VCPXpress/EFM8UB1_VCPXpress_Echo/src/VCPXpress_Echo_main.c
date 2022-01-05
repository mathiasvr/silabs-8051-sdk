/**************************************************************************//**
 * (C) Copyright 2014 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 * 
 *****************************************************************************/

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB1_Register_Enums.h>                // SI_SFR declarations
#include "VCPXpress.h"
#include "descriptor.h"
#include <stdint.h>


/**************************************************************************//**
 * VCPXpress_Echo_main.c
 *
 * Main routine for VCPXpress Echo example.
 *
 * This example simply echos any data received over USB back up to the PC.
 *
 *****************************************************************************/

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------


SI_SBIT (LED1, SFR_P1, 4);                // LED='1' means ON

#define BLINK_RATE         2              // Timer2 Interrupts per second
#define SYSCLK             6000000        // SYSCLK frequency in Hz
#define PRESCALE           48             //Timer prescaler
#define TIMER_RELOAD -(SYSCLK / PRESCALE / BLINK_RATE) // this must be < 2^16

#define TIMER_RELOAD_HIGH ((TIMER_RELOAD & 0xFF00)>>8)
#define TIMER_RELOAD_LOW (TIMER_RELOAD & 0x00FF)

#define PACKET_SIZE 64

uint16_t xdata InCount;                   // Holds size of received packet
uint16_t xdata OutCount;                  // Holds size of transmitted packet
uint8_t xdata RX_Packet[PACKET_SIZE];     // Packet received from host
uint8_t xdata TX_Packet[PACKET_SIZE];     // Packet to transmit to host


// Function Prototypes
//-----------------------------------------------------------------------------
void Delay (void);
void Sysclk_Init (void);
static void Port_Init (void);
void Timer0_Init ();


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
   WDTCN = 0xDE;
   WDTCN = 0xAD;
}
 
/**************************************************************************//**
 * @brief Main loop
 *
 * The main loop sets up the device and then waits forever. All active tasks
 * are ISR driven.
 *
 *****************************************************************************/
void main (void)
{
   //Disable WDT

   VDM0CN = VDM0CN_VDMEN__ENABLED;            // Enable VDD Monitor
   Delay ();                                  // Wait for VDD Monitor to stabilize
   RSTSRC = RSTSRC_PORSF__SET;                // Enable VDD Monitor as a reset source

   Sysclk_Init ();                            // Initialize system clock
   Port_Init ();                              // Initialize crossbar and GPIO
   Timer0_Init ();                            // Initialize Timer2

   // VCPXpress Initialization
   USB_Init(&InitStruct);

   // Enable VCPXpress API interrupts
   API_Callback_Enable();

   IE_EA = 1;       // Enable global interrupts

   while (1)
   {
   }                                // Spin forever
}


// Interrupt Service Routines
//-----------------------------------------------------------------------------

/**************************************************************************//**
 * @brief Timer0_ISR
 *
 * This routine changes the state of the LED whenever Timer0 overflows.
 *
 *****************************************************************************/
SI_INTERRUPT(Timer0_ISR, TIMER0_IRQn)
{
  TH0 = TIMER_RELOAD_HIGH;            // Reload Timer0 High register
  TL0 = TIMER_RELOAD_LOW;             // Reload Timer0 Low register
  
  LED1 = !LED1;                       // Change state of LED
}


/**************************************************************************//**
 * @brief VCPXpress callback
 *
 * This function is called by VCPXpress. In this example any received data
 * is immediately transmitted to the UART. On completion of each write the
 * next read is primed.
 *
 *****************************************************************************/
void VCP_Callback(void)
{
   uint32_t INTVAL = Get_Callback_Source();
   uint8_t i;


   if (INTVAL & DEVICE_OPEN)
   {
      Block_Read(RX_Packet, PACKET_SIZE, &InCount);   // Start first USB Read
   }

   if (INTVAL & RX_COMPLETE)                          // USB Read complete
   {
      for (i=0; i<InCount;i++)                        // Copy received packet to output buffer
      {
         TX_Packet[i] = RX_Packet[i];
      }
      Block_Write(TX_Packet, InCount, &OutCount);     // Start USB Write
   }

   if (INTVAL & TX_COMPLETE)                          // USB Write complete
   {
      Block_Read(RX_Packet, PACKET_SIZE, &InCount);   // Start next USB Read
   }
}

/**************************************************************************//**
 * @brief Clock initialization
 *
 * Setting SYSCLK to 6 MHz
 *****************************************************************************/
void Sysclk_Init (void)
{
  SFRPAGE = 0x10;
  HFOCN  = HFOCN_HFO1EN__ENABLED;      // Enable 48Mhz osc
  // need to set SYSCLK to > 24 MHz before switching to HFOSC1
  CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__HFOSC0;
  // wait for clock divider ready
  while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
  {}
  // now switch to HFOSC1 with divider of HFOSC1/8 for 6 MHz
  CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_8 | CLKSEL_CLKSL__HFOSC1;
  while ((CLKSEL & CLKSEL_DIVRDY__BMASK) == CLKSEL_DIVRDY__NOT_READY)
  {}
  SFRPAGE = 0x0;
}

/**************************************************************************//**
 * @brief Port initialization
 *
 * Enable port output pin for LED
 *****************************************************************************/
static void Port_Init (void)
{
   P1MDOUT = P1MDOUT_B4__PUSH_PULL;
   P1SKIP = P1SKIP_B4__SKIPPED;
   XBR2 = XBR2_XBARE__ENABLED;      // Enable crossbar
}

/**************************************************************************//**
 * @brief Timer initialization
 *
 * Configure Timer0 to 16-bit auto-reload and generate an interrupt at
 * interval specified by <counts> using SYSCLK/48 as its time base.
 *
 *****************************************************************************/
void Timer0_Init()
{
   TH0 = TIMER_RELOAD_HIGH;            // Init Timer0 High register
   TL0 = TIMER_RELOAD_LOW;             // Init Timer0 Low register
   TMOD = TMOD_T0M__MODE1;             // Timer0 in 16-bit mode
   CKCON0 = CKCON0_SCA__SYSCLK_DIV_48; // Timer0 uses a 1:48 prescaler
   IE_ET0 = 1;                         // Timer0 interrupt enabled
   TCON = TCON_TR0__RUN;               // Timer0 ON
}

/**************************************************************************//**
 * @brief delay for approximately 1ms
 *
 *****************************************************************************/
void Delay (void)
{
   int16_t x;

   for (x = 0; x < 500; x)
   {
      x++;
   }
}
