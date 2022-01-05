/**************************************************************************//**
 * @file
 * @brief   Hardware initialization functions.
 * @author  Silicon Laboratories
 * @version 1.0.0 (DM: July 14, 2014)
 *
 *******************************************************************************
 * @section License
 * (C) Copyright 2014 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *******************************************************************************
 *
 * initialization routines for VCPXpress UART example
 * 
 *****************************************************************************/

#include <SI_C8051F320_Register_Enums.h>
#include "descriptor.h"
#include "callback.h"
#include "initialization.h"

void Delay(void);

/**************************************************************************//**
 *  @brief
 *    Init entire system
 *   
 *  Top-level initialization routine to call all other module initialization
 *  subroutines. 
  
 *****************************************************************************/
void System_Init (void)
{
   SYSCLK_Init ();                     // Initialize system clock to
                                       // 48MHz
   PORT_Init ();                       // Initialize crossbar and GPIO
   USB_Init(&InitStruct);              // VCPXpress initialization
   API_Callback_Enable();              // Enable VCPXpress API interrupts
   UART0_Init();                       // Initialize UART0 for printf's
}



/**************************************************************************//**
 *  @brief
 *    Init Sysclk
 *  
 *  This routine initializes the system clock to use the internal 12MHz
 *  oscillator as its clock source.  Also enables missing clock detector reset.
 *   
 *****************************************************************************/
void SYSCLK_Init (void)
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
 *  @brief
 *    Init Ports and Xbar
 *  
 *  Configure the Crossbar and GPIO ports.
 *    * P0.4 - UART TX (push-pull)
 *    * P0.5 - UART RX
 *   
 *****************************************************************************/
void PORT_Init (void)
{

  XBR0     = XBR0_URT0E__ENABLED;     // Enable UART0
  XBR1     = XBR1_XBARE__ENABLED      // Enable crossbar and disable weak pull-ups
             | XBR1_WEAKPUD__PULL_UPS_DISABLED;     
  P0MDOUT |= 0x10;                    // Set TX to push-pull.
}


/**************************************************************************//**
 *  @brief
 *    Init Uart and it's timer.
 *  
 *  Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
 *   
 *****************************************************************************/
void UART0_Init (void)
{
  //UART to 8-bit bidirectional
  SCON0 = SCON0_SMODE__8_BIT
           | SCON0_REN__RECEIVE_ENABLED
           | SCON0_RB8__CLEARED_TO_0
           | SCON0_TB8__CLEARED_TO_0
           | SCON0_MCE__MULTI_DISABLED
           | SCON0_RI__NOT_SET
           | SCON0_TI__NOT_SET;
       
  if (SYSCLK/BAUDRATE/2/256 < 1) {
     TH1 = -(SYSCLK/BAUDRATE/2);
     CKCON |=  0x08;                  // T1M = 1; SCA1:0 = xx
  } else if (SYSCLK/BAUDRATE/2/256 < 4) {
     TH1 = -(SYSCLK/BAUDRATE/2/4);
     CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
     CKCON |=  0x01;
  } else if (SYSCLK/BAUDRATE/2/256 < 12) {
     TH1 = -(SYSCLK/BAUDRATE/2/12);
     CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
  } else if (SYSCLK/BAUDRATE/2/256 < 48) {
     TH1 = -(SYSCLK/BAUDRATE/2/48);
     CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
     CKCON |=  0x02;
  } else {
     while (1);                       // Error.  Unsupported baud rate
  }
  TL1 = TH1;                          // init Timer1 reload

   //Setup timer 2 as 1ms tick for timeout.
   CKCON |= CKCON_T0M__SYSCLK; //select sysclk for timer0
   TH0 = -(SYSCLK/1000)>>8;     
   TL0 = -(SYSCLK/1000) & 0x00FF;
   
   TMOD = TMOD_CT0__TIMER
          | TMOD_T0M__MODE1
          | TMOD_CT1__TIMER
          | TMOD_T1M__MODE2;
   
   TCON_TR1 = 1;                       // START Timer1
   TCON_TR0 = 1;                       // START Timer0

   IP   = IP_PS0__HIGH;                //Make UART0 high priority so we don't drop bytes
   IE_ES0 = 1;                         // Enable UART0 interrupts
   TCON_IT0 = 0;                       // clear timer flag;
   IE_ET0 = 1;                         // Enable TIMER0 interrupts;
}

/**************************************************************************//**
 * @brief delay for approximately 5us @ 24Mhz
 *
 *****************************************************************************/
void Delay (void)
{
   int16_t x;

   for (x = 0; x < 250; x)
   {
      x++;
   }
}

