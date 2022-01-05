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

#include <SI_C8051F326_Register_Enums.h>
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
  P0MDOUT |= 0x10;                    // Set TX to push-pull.
  GPIOCN  = GPIOCN_INPUTEN__ENABLED;  // Enable input path
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
	uint16_t divCount;
	//UART to 8-bit bidirectional
  SCON0 = SCON0_REN__RECEIVE_ENABLED
           | SCON0_RI__NOT_SET
           | SCON0_TI__NOT_SET;

  SBCON0 |= SBCON0_SBCLK__USBCLK;

  if (2*SYSCLK/BAUDRATE/2/0xFFFF < 1) {
  	divCount= -(2*SYSCLK/BAUDRATE/2);
     //No need to clear bits to write all 1
     SBCON0 |=  SBCON0_SBPS__DIV_BY_1;
  } else if (2*SYSCLK/BAUDRATE/2/0xFFFF < 4) {
  	divCount = -(2*SYSCLK/BAUDRATE/2/4);
  	SBCON0 &= ~SBCON0_SBPS__FMASK;
    SBCON0 |=  SBCON0_SBPS__DIV_BY_4;
  } else if (2*SYSCLK/BAUDRATE/2/0xFFFF < 12) {
  	divCount = -(2*SYSCLK/BAUDRATE/2/12);
  	SBCON0 &= ~SBCON0_SBPS__FMASK;
  	//No need to set field to 0
  } else if (2*SYSCLK/BAUDRATE/2/0xFFFF < 48) {
  	divCount = -(2*SYSCLK/BAUDRATE/2/48);
  	SBCON0 &= ~SBCON0_SBPS__FMASK;
    SBCON0 |=  SBCON0_SBPS__DIV_BY_48;
  } else {
     while (1);                       // Error.  Unsupported baud rate
  }

  SBRLL0 = divCount & 0xFF;
  SBRLH0 = (divCount >> 8) & 0xFF;
  SBCON0 |= SBCON0_SBRUN__ENABLED;


   //Setup timer 0 as 1ms tick for timeout.
   CKCON |= CKCON_T0M__SYSCLK; //select sysclk for timer0
   TH0 = -(SYSCLK/1000)>>8;     
   TL0 = -(SYSCLK/1000) & 0x00FF;
   
   TMOD = TMOD_T0M__MODE1
          | TMOD_T1M__MODE2;
   
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

