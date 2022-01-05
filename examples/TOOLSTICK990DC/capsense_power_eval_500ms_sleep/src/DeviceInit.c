/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "SI_C8051F990_Register_Enums.h"
#include "ProjectConfig.h"
#include "DeviceInit.h"

#include "CSLibrary.h"


//-----------------------------------------------------------------------------
// SiLabs_Startup
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function is called within SILABS_STARTUP.A51 before any variable
// initialization code (by using the assembler directive SET(SILABS_STARTUP=1)
// in order to disable the watchdog timer.
//
//-----------------------------------------------------------------------------
void SiLabs_Startup(void)
{
	PCA0MD &= ~PCA0MD_WDTE__BMASK;     // WDTE = 0 (clear watchdog timer
									   // enable)
}

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal precision
// oscillator
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   REG0CN |= 0x10;                     // Enable the precision osc. bias
   OSCICN |= 0x80;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P0.2 S2
//-----------------------------------------------------------------------------
void PORT_Init (void)
{

	// Enable portmatch on P0.2 (S2)
	P0MAT |= P0MAT_B2__HIGH;
	P0MASK |= P0MASK_B2__COMPARED;

	XBR2 = XBR2_XBARE__ENABLED;   	// Enable crossbar and weak pull-ups

	EIE2 |= EIE2_EMAT__ENABLED;		// Enable portmatch interrupts
	IE_EA = IE_EA__ENABLED;			// Globally enable interrupts
}


void SystemInit(void)
{
   SiLabs_Startup();
   OSCILLATOR_Init();
   PORT_Init();
}
