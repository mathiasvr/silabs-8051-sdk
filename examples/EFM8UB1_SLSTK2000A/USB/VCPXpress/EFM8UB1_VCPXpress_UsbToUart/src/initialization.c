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

#include <SI_EFM8UB1_Register_Enums.h>
#include "descriptor.h"
#include "initialization.h"


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
   SPI0_Init();                        // Initialize SPI0 for LCD
   TIMER2_Init();                      // Initialize timer 2 for SPI CS timing
   TIMER3_Init();                      // Initialize tick timer
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
  SFRPAGE = 0x10;
  HFOCN  = HFOCN_HFO1EN__ENABLED;
  CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 // Select hfosc (need 24Mhz before switching to 48Mhz)
         | CLKSEL_CLKSL__HFOSC0;
  CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 // need to write register twice
         | CLKSEL_CLKSL__HFOSC0;
  while(!(CLKSEL & CLKSEL_DIVRDY__READY));
  CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 // Select hfosc/1
         | CLKSEL_CLKSL__HFOSC1;
  CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 // need to write register twice
         | CLKSEL_CLKSL__HFOSC1;
  while(!(CLKSEL & CLKSEL_DIVRDY__READY));
  SFRPAGE = 0x0;
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

  XBR0     = XBR0_URT0E__ENABLED      // Enable UART0
		         | XBR0_SPI0E__ENABLED;   // Enable SPI
  XBR2     = XBR2_XBARE__ENABLED      // Enable crossbar and disable weak pull-ups
             | XBR2_WEAKPUD__PULL_UPS_DISABLED;
  P0MDOUT |= 0x52;                    // Set TX + SCK + NSS pin to push-pull.
  P1MDOUT |= 0x01;                    // Set MOSI pin to push-pull.
  P2MDOUT |= 0x09;                    // BC_EN to push-pull

  P0SKIP  = 0x0F;                     //Place SPI @ P06

  P2 |= 0x09;                         // BC enable and display power

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
           | SCON0_RB8__NOT_SET
           | SCON0_TB8__NOT_SET
           | SCON0_MCE__MULTI_DISABLED
           | SCON0_RI__NOT_SET
           | SCON0_TI__NOT_SET;

  CKCON0 &= ~CKCON0_T1M__BMASK; //select prescaler

  if (SYSCLK/BAUDRATE/2/256 < 1) {
    TH1 = -(SYSCLK/BAUDRATE/2/1);
    CKCON0 |=  CKCON0_T1M__SYSCLK;                 // T1M = 1;
  }else if (SYSCLK/BAUDRATE/2/256 < 4) {
    TH1 = -(SYSCLK/BAUDRATE/2/4);
    CKCON0 &= ~CKCON0_SCA__FMASK;                  // T1M = 0; SCA1:0 = 01
    CKCON0 |=  CKCON0_SCA__SYSCLK_DIV_4;
  } else if (SYSCLK/BAUDRATE/2/256 < 12) {
    TH1 = -(SYSCLK/BAUDRATE/2/12);
    CKCON0 &= ~CKCON0_SCA__FMASK;                  // T1M = 0; SCA1:0 = 01
  } else if (SYSCLK/BAUDRATE/2/256 < 48) {
    TH1 = -(SYSCLK/BAUDRATE/2/48);
    CKCON0 &= ~CKCON0_SCA__FMASK;                  // T1M = 0; SCA1:0 = 01
    CKCON0 |=  CKCON0_SCA__SYSCLK_DIV_48;
  } else {
     while (1);                       // Error.  Unsupported baud rate
  }
  TL1 = TH1;                          // init Timer1 reload

   //Setup timer 0 as 1ms tick for timeout.
   CKCON0 |=  CKCON0_T0M__SYSCLK; // T0M = 1;
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

void SPI0_Init(void) {
	SPI0CKR = (23 << SPI0CKR_SPI0CKR__SHIFT); //SYSCLK/24*2 = 12Mhz

	SPI0CFG = SPI0CFG_CKPHA__DATA_CENTERED_FIRST
			  | SPI0CFG_CKPOL__IDLE_LOW
		      | SPI0CFG_MSTEN__MASTER_ENABLED;

	SPI0CN0 &= ~SPI0CN0_NSSMD__FMASK;
	SPI0CN0 |= SPI0CN0_SPIEN__ENABLED;

	IE |= IE_ESPI0__ENABLED;
}

void TIMER2_Init(void) {
  IE |= IE_ET2__ENABLED;
}

void TIMER3_Init(void) {
	uint8_t TMR3CN0_TR3_save = TMR3CN0 & TMR3CN0_TR3__BMASK;
	// Stop Timer
	TMR3CN0 &= ~(TMR3CN0_TR3__BMASK);

	TMR3RLH = (0xF0 << TMR3RLH_TMR3RLH__SHIFT);
	TMR3RLL = (0x60 << TMR3RLL_TMR3RLL__SHIFT);
	TMR3CN0 |= TMR3CN0_TR3__RUN;

	// Restore Timer Configuration
	TMR3CN0 |= TMR3CN0_TR3_save;

  SFRPAGE = 0x10;
	EIE1 |= EIE1_ET3__ENABLED;
  SFRPAGE = 0x00;

}


