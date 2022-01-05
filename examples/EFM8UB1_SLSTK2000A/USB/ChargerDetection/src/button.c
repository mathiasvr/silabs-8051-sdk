/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// button.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include <bsp.h>
#include <button.h>

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Buttons_GetStatus() Routine
// ----------------------------------------------------------------------------
//
// Description - Get mouse buttons status.
//
// Return - Which button has been pushed
//-----------------------------------------------------------------------------
uint8_t Buttons_GetStatus(void)
{
	uint8_t buttonStatus = 0;
	if (!BSP_PB1) {
		buttonStatus = LEFT_BUTTON;
	} else if (!BSP_PB0) {
		buttonStatus = RIGHT_BUTTON;
	}
	return buttonStatus;
}
