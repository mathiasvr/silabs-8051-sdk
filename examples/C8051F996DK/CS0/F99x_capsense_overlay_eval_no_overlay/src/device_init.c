/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <si_toolchain.h>             // compiler declarations
#include "SI_C8051F990_Defs.h"
#include "cslib_config.h"
#include "device_init.h"
#include "comm_routines.h"

#include "cslib.h"

//-----------------------------------------------------------------------------
// OSCILLATOR_init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal precision
// oscillator
//
//-----------------------------------------------------------------------------
void OSCILLATOR_init (void)
{
   REG0CN |= 0x10;                     // Enable the precision osc. bias
   OSCICN |= 0x80;                     // Enable the precision internal osc.

   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock

}

//-----------------------------------------------------------------------------
// PORT_init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P1.2 CS0 Switch 4
// P1.4 UART TX
// P1.5 UART RX

//-----------------------------------------------------------------------------
void PORT_init (void)
{

   P1MDOUT |= 0x28;
   XBR2    = 0x40;                     // Enable crossbar and weak pull-ups



}


void systemInit(void)
{
   OSCILLATOR_init();
   PORT_init();

}
