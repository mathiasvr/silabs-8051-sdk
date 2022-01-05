/*
 * USBXpress_Echo_main.c
 *
 * Main routine for USBXpress Echo example.
 *
 * This example echos any data sent over usb back up to the host.
 * It also blinks an led at 1Hz to indicate that the core is running.
 *
 */

//------------------------------------------------------------------------------
// Includes

#include <SI_C8051F320_Register_Enums.h>                // SFR declarations
#include <si_toolchain.h>
#include "efm8_usbxpress.h"
#include "descriptor.h"
#include <stdint.h>

// -----------------------------------------------------------------------------
// Global Constants

#define USB_BUFFER_SIZE    512          /// Size of USB buffer

SI_SBIT (LED, SFR_P2, 2);               // LED='1' means ON

#define BLINK_RATE         500          // ms per blink
#define SYSCLK             24000000     // SYSCLK frequency in Hz
#define PRESCALE           48           //Timer prescaler
#define TIMER_RELOAD -(SYSCLK / PRESCALE / 1000)

#define TIMER_RELOAD_HIGH ((TIMER_RELOAD & 0xFF00)>>8)
#define TIMER_RELOAD_LOW (TIMER_RELOAD & 0x00FF)

// -----------------------------------------------------------------------------
// Function Prototypes

void Delay (void);
void Sysclk_Init (void);
void Port_Init (void);
void Timer0_Init ();
void my_usbxp_callback(void);

// -----------------------------------------------------------------------------
// Variable Declarations

SI_SEGMENT_VARIABLE(timerCounter, uint16_t, SI_SEG_XDATA);

/// Buffer for holding USB data
SI_SEGMENT_VARIABLE(usbBuffer[USB_BUFFER_SIZE], uint8_t, SI_SEG_XDATA);

// -----------------------------------------------------------------------------
// Functions

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
  PCA0MD &= ~PCA0MD_WDTE__BMASK;         // Disable watchdog timer
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


  VDM0CN = VDM0CN_VDMEN__ENABLED;        // Enable VDD Monitor
  Delay ();                              // Wait for VDD Monitor to stabilize
  RSTSRC = RSTSRC_PORSF__SET;            // Enable VDD Monitor as a reset source

  Sysclk_Init ();                        // Initialize system clock
  Port_Init ();                          // Initialize crossbar and GPIO
  Timer0_Init();                         // Initialize Timer0

  // USBXpress Initialization
  USBX_init(&initStruct);

  // Enable USBXpress API interrupts
  USBX_apiCallbackEnable(my_usbxp_callback);

  IE_EA = 1;       // Enable global interrupts

  while (1)
  {
  }                                // Spin forever
}


// -------------------------------
// Interrupt Service Routines

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

    timerCounter++;
    if(timerCounter >= BLINK_RATE)
    {
      LED = !LED;                       // Change state of LED
      timerCounter = 0;
    }
}

/**************************************************************************//**
 * @brief USBXpress call-back
 *
 * This function is called by USBXpress. In this example any received data
 * sent back up to the host.
 *
 *****************************************************************************/
void my_usbxp_callback(void)
{
  SI_SEGMENT_VARIABLE(readLen, uint16_t, SI_SEG_IDATA);
  SI_SEGMENT_VARIABLE(writeLen, uint16_t, SI_SEG_IDATA);
  uint32_t intval = USBX_getCallbackSource();

  // Suspend
  if (intval & USBX_DEV_SUSPEND)
  {
    // Turn off LED
    LED = 0;

    // Enter suspend mode to save power
    USBX_suspend();
  }

  // Device opened
  if (intval & USBX_DEV_OPEN)
  {
    // Prime first read
    USBX_blockRead(usbBuffer, USB_BUFFER_SIZE, &readLen);
  }

  // USB read complete
  if (intval & USBX_RX_COMPLETE)
  {
    // If data was received, echo it back to the host
    if (readLen)
    {
      USBX_blockWrite(usbBuffer, readLen, &writeLen);
    }
    // If data of zero-length was received, start a new read
    else
    {
      USBX_blockRead(usbBuffer, USB_BUFFER_SIZE, &readLen);
    }
  }

  // USB write complete
  if (intval & USBX_TX_COMPLETE)
  {
    // The data was sent to the host, so start a new read
    USBX_blockRead(usbBuffer, USB_BUFFER_SIZE, &readLen);
  }
}

// -------------------------------
// Initialization Functions

/**************************************************************************//**
 * @brief clock initialization
 *
 * Set fastest system clock (48Mhz)
 *****************************************************************************/
void Sysclk_Init (void)
{
  OSCICN  |= OSCICN_IOSCEN__ENABLED
             | OSCICN_IFCN__HFOSC_DIV_1;        // Select full speed HFOSC

  CLKMUL = 0x00;                                //clear multiplier
  CLKMUL |= CLKMUL_MULEN__ENABLED;              //enable multiplier
  Delay();
  CLKMUL |= CLKMUL_MULINIT__SET;                //Initialize multiplier
  Delay();

  while(!(CLKMUL & CLKMUL_MULRDY__BMASK));      // Wait for multiplier to lock

  CLKSEL = CLKSEL_CLKSL__CLKMUL_DIV_2;          // select max sysclk
}

/**************************************************************************//**
 * @brief Port initialization
 *
 * P2.2   digital   push-pull    LED1
 *
 *****************************************************************************/
static void Port_Init (void)
{
   P2MDOUT   = P2MDOUT_B2__PUSH_PULL;   // P2.2 is push-pull
   XBR1      = XBR1_XBARE__ENABLED;     // Enable the crossbar
}

/**************************************************************************//**
 * @brief Timer initialization
 *
 * @param counts:
 *   calculated Timer overflow rate range is positive range of integer: 0 to 32767
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
   CKCON = CKCON_SCA__SYSCLK_DIV_48;   // Timer0 uses a 1:48 prescaler
   IE_ET0 = 1;                         // Timer0 interrupt enabled
   TCON = TCON_TR0__RUN;               // Timer0 ON
}


/**************************************************************************//**
 * @breif delay for approximately 1ms @ 48Mhz
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
