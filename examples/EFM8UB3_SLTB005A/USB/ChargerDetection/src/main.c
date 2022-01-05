//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates the USB charger detection hardware feature.
// The device will detect whether it's connected to a standard downstream
// port (SDP) or dedicated charging port (DCP) and display "SDP Detected"
// or "DCP Detected" over UART.  If a USB host, or SDP, is detected,
// the device will enumerate as a HID keyboard.  Pressing either push
// button will send the characters in the string "HID Keyboard " repeatedly.
//
//-----------------------------------------------------------------------------
// How To Test: EFM8UB3 STK
//-----------------------------------------------------------------------------
// 1) Place the "Power Source" switch in "DBG USB" mode.
// 2) Connect the EFM8UB3 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8UB3 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the JLink CDC UART Port at 115200 baud rate and 8-N-1.
// 6) Connect the STK board to a USB host via micro USB cable.
// 7) The terminal should display "SDP Detected" and the device should
//    enumerate as a HID keyboard.
// 8) Pressing either push button will send the characters in "HID Keyboard "
//    to the host as key presses.
// 9) Disconnect the micro USB cable.
// 10) Reset the device.
// 11) Connect the device to a dedicated charging port (DCP compatible USB
//     charger).
// 12) The terminal should display "DCP Detected".
//
// Target:         EFM8UB3
// Tool chain:     Generic
//
// Release 0.1 (CM)
//    - Initial Revision
//    - 10 OCT 2014
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 24.5 MHz HFOSC1 / 1
// USB0   - Full speed
// Timer3 - 1 kHz (1 ms tick)
// P0.2 - push button
// P0.3 - push button
// P1.4 - LED Green
// P1.5 - LED Blue
// P1.6 - LED Red
// P2.1 - VBUS sense

/////////////////////////////////////////////////////////////////////////////
// main.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////
#include <bsp.h>
#include <InitDevice.h>
#include <efm8_usb.h>
#include <descriptors.h>
#include <idle.h>
#include <tick.h>
#include <button.h>
#include "retargetserial.h"

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

typedef SI_SEGMENT_VARIABLE(, char, const SI_SEG_CODE) codeStr_t;

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

SI_SEGMENT_VARIABLE_SEGMENT_POINTER(chargerDescription[], codeStr_t, const SI_SEG_CODE, const SI_SEG_CODE) = {
	"              ",
	"  SDP Detected",
	"  DCP Detected",
	"  CDP Detected"
};


//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define MESSAGE_DISP_ROW  		 88

/* Charger type definition */
#define NO_CHARGER			0
#define SDP_CHARGER			1
#define DCP_CHARGER			2
#define CDP_CHARGER			3
#define LAST_CHARGER_TYPE	CDP_CHARGER


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
static uint8_t USB_Detect_Charger(void);

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
  // Disable the watchdog here
}
 
//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int16_t main(void)
{
  uint8_t chargerType = 0;
  
	enter_DefaultMode_from_RESET();

	// Enable VBUS detection on VBUS/P2.1 pin
	USB_VbusDetectEnable();

	IE_EA = 1;       // Enable global interrupts

	SCON1_TI = 1;                 // Set transmit flag to 1

	while (1) {
		/* Wait for VBUS signal high*/
		while (BSP_VBUS == 0);
		chargerType = USB_Detect_Charger();
		if (chargerType <= LAST_CHARGER_TYPE) {
			RETARGET_PRINTF("%s",chargerDescription[chargerType]);
		}

		/* Initial USB for Standard Downstream Port */
		if (chargerType == SDP_CHARGER) {
			USBD_Init(&initstruct);
		}

		/* Wait for VBUS signal LOW*/
		while (BSP_VBUS);

		/* Disable USB physical layer Transceiver */
		USBD_Disconnect();

		chargerType = NO_CHARGER;
		if (chargerType <= LAST_CHARGER_TYPE) {
			RETARGET_PRINTF("%s",chargerDescription[chargerType]);
		}
	}
}

//-----------------------------------------------------------------------------
// USB_Detect_Charger
//-----------------------------------------------------------------------------
//
// Description - Detect charger type and return it to caller.
//
//-----------------------------------------------------------------------------
static uint8_t USB_Detect_Charger(void)
{
  uint8_t sfrPageSave = SFRPAGE;
	uint8_t ret = NO_CHARGER;

	if (BSP_VBUS == 0) {
		return ret;
	}

	SFRPAGE = PG3_PAGE;

	USB0CDSTA = 0;

	// Enable charger detection using primary detection and data contact detection
	USB0CDCN = USB0CDCN_CHDEN__ENABLED | USB0CDCN_PDEN__ENABLED | USB0CDCN_DCDEN__ENABLED;

	/* Wait for primary detection to complete */
	while ((USB0CDSTA & USB0CDSTA_PDI__SET) == 0);

	/* SDP bit will indicate if a Standard Downstream Port is detected */
	if (USB0CDSTA & USB0CDSTA_SDP__DETECTED) {
		ret = SDP_CHARGER;
	} else {
	    USB0CDSTA = 0;

		/* Do Secondary Detection for DCP and CDP */
		USB0CDCN = USB0CDCN_CHDEN__ENABLED | USB0CDCN_SDEN__ENABLED;

		/* Wait for Secondary Detection to complete */
		while ((USB0CDSTA & USB0CDSTA_SDI__SET) == 0);

		if (USB0CDSTA & USB0CDSTA_CDP__DETECTED) {
			ret = CDP_CHARGER;
		} else if (USB0CDSTA & USB0CDSTA_DCP__DETECTED) {
			ret = DCP_CHARGER;
		}
	}

	SFRPAGE = sfrPageSave;

	return ret;
}

/*
1. Enable VBUS detection on the VBUS pin (must be connected to USB VBUS).
2. Optionally, enable the PD and/or SD interrupts with the corresponding enable bits.
3. Set DCDEN to "Full Detection" (0x3) to enable data contact detection and the
	associated timeout circuit.
4. Set PDEN to enable Primary Detection.
5. Set CHDEN to begin the charge detect sequence.
6. Wait for Primary Detection to complete (PDI = 1, or service the interrupt).
7. The SDP bit will indicate if a Standard Downstream Port is detected.
8. If the application requires further differentiation between DCP and CDP, set
	SDEN to enable Secondary Detection, and set CHDEN to begin this sequence.
9. Wait for Secondary Detection to complete (SDI = 1, or service the interrupt).
10. The DCP and CDP bits will indicate if a Dedicated Charging Port or Charging
	Downstream Port has been detected
*/

