//-----------------------------------------------------------------------------
// F970_I2C_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Example software to demonstrate the C8051F970 I2C Slav interface
// - Interrupt-driven I2C Slave implementation
// - Only slave states defined
// - 1-byte I2C Slave data holder used for both transmit and receive
// - Timer3 used by I2C Slave for SCL low timeout detection
// - ARBLOST support included
// - Pinout:
//    I2C Slave Pins
//
//    P0.5 -> LED05
//
//    all other port pins unused
//
// How To Test:
// ------------
// 1) Ensure shorting blocks are place on the following:
//    - J1:    3.3v <> VREG (Power)
//    - JP3:    VDD <> VMCU
//    - JP4:    VDD <> VLED (LED power)
// 2) Place the VDD SELECT switch in the VREG position and power the board.
// 3) Compile and download code to the C8051F970-TB by selecting
//      Run -> Debug from the menus,
//      or clicking the Debug button in the quick menu,
//      or pressing F11.
// 4) Connect the I2C Slave to a SMBus master using the H7 header.
// 5) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
//      The slave code will write data and read data from the
//      same data byte, so a successive write and read will effectively echo the
//      data written. To verify that the code is working properly, verify on the
//      master that the data written is the same as the data received.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2)
//    - 31 JUL 2014
//
// Release 1.1
//    - Update to use Configurator (BML)
//    - 3 Jun 2019
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  SYSCLK         24500000       // System clock frequency in Hz

#define LED_ON           0             // Turns the LED on
#define LED_OFF          1             // Turns the LED off

#define  SLAVE_ADDR     0xF0           // Device addresses (7 bits,
									   // lsb is a don't care)

// I2C States
#define  I2C_ADDR_RD    0x09			// Valid Slave Address + Master Read Request
#define  I2C_ADDR_WR    0x0A			// Valid Slave Address + Master Write Request
#define  I2C_RD_DATA    0x01			// Transfer data from Slave
#define  I2C_WR_DATA    0x02			// Write data to Slave
#define  I2C_STO        0x04
#define  I2C_STOSTA     0x0C
#define  I2C_STOSTARD   0x0D

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

uint8_t I2C_DATA;                            // Global holder for I2CSlave data.

bit DATA_READY = 0;                     // Set to '1' by the SMBus ISR
										// when a new data byte has been
										// received.

SI_SBIT(LED05, P0, 5);                    // LED05==LED_ON means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void I2CSlave_Init (void);
void Timer3_Init (void);
void Port_Init (void);
void OSCILLATOR_Init (void);

SI_INTERRUPT_PROTO(TIMER3_ISR, TIMER3_IRQn);
SI_INTERRUPT_PROTO(I2CSlave_ISR, I2C0_IRQn);

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
  SFRPAGE = PCA0_PAGE;                 // Disable the watchdog timer
  PCA0MD &= ~PCA0MD_WDTE__ENABLED;
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then waits for SMBus
// communication.
//
//-----------------------------------------------------------------------------
void main (void)
{
  enter_DefaultMode_from_RESET();

	LED05 = LED_OFF;

	IE_EA = 1;                          // Global interrupt enable

	while(1)
	{
		while(!DATA_READY);              // New SMBus data received?
		DATA_READY = 0;

		LED05 = !LED05;
	}
}

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal
// oscillator.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
	SFRPAGE = LEGACY_PAGE;

	REG0CN |= 0x10;                     // Enable bias of the precision internal oscillator

	OSCICN |= 0x80;                     // Enable the precision internal osc.

	RSTSRC = 0x06;                      // Enable missing clock detector and
										// leave VDD Monitor enabled.

	CLKSEL = 0x00;                      // Select precision internal osc.
										// divided by 1 as the system clock

}
//-----------------------------------------------------------------------------
// SMBus_Init()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// SMBus configured as follows:
// - SMBus enabled
// - Slave mode not inhibited
// - Setup and hold time extensions enabled
// - Bus Free and SCL Low timeout detection enabled
//
//-----------------------------------------------------------------------------
void I2CSlave_Init (void)
{

	SFRPAGE = I2C0_PAGE;

	I2C0CN &= ~I2C0CN_BUSY__BMASK;		// Clear BUSY bit
	I2C0SLAD = (SLAVE_ADDR >> 1);

	I2C0CN |= I2C0CN_PINMD__I2C_MODE;	// Set pads in I2C mode
	I2C0CN |= I2C0CN_PUEN__ENABLED;		// Enable I2C pad pullup

	I2C0CN |= I2C0CN_I2C0EN__ENABLED;	// Enable I2C module

	I2C0CN |= I2C0CN_TIMEOUT__ENABLED;	// Enable SCL Low Timeout detection
	I2C0CN |= I2C0CN_PRELOAD__DISABLED;	// Data byte need not be preloaded for I2C read operation

	SFRPAGE = LEGACY_PAGE;
}

//-----------------------------------------------------------------------------
// Timer3_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer3 configured for use by the SMBus low timeout detect feature as
// follows:
// - Timer3 in 16-bit auto-reload mode
// - SYSCLK/12 as Timer3 clock source
// - Timer3 reload registers loaded for a 25ms overflow period
// - Timer3 pre-loaded to overflow after 25ms
// - Timer3 enabled
//
//-----------------------------------------------------------------------------
void Timer3_Init (void)
{

	SFRPAGE = TIMER3_PAGE;

	TMR3CN = 0x00;                      // Timer3 configured for 16-bit auto-
										// reload, low-byte interrupt disabled

	CKCON &= ~0x40;                     // Timer3 uses SYSCLK/12

	TMR3RL = (uint16_t) -(SYSCLK/12/40);     // Timer3 configured to overflow after
	TMR3 = TMR3RL;                      // ~25ms (for SMBus low timeout detect):
										// 1/.025 = 40

	EIE1 |= 0x80;                       // Timer3 interrupt enable

	TMR3CN |= 0x04;                     // Start Timer3
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P1.0   digital   push-pull     LED
//
// all other port pins unused
//
//-----------------------------------------------------------------------------
void Port_Init (void)
{

	SFRPAGE = CONFIG_PAGE;

	P0SKIP = 0x01;
	P0MDOUT |= 0x02;                    // Make the LED05 (P0.2) a
										// push-pull output

	XBR0 = 0x04;                        // Enable SMBus pins
	XBR1 = 0x40;                        // Enable crossbar and weak pull-ups

	P0 = 0xFF;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// I2C Slave Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// I2C Slave ISR state machine
// - Slave only implementation - no master states defined
// - All incoming data is written to global variable <I2C_DATA_IN>
// - All outgoing data is read from global variable <I2C_DATA_OUT>
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(I2CSlave_ISR, I2C0_IRQn)
{
	SFRPAGE = I2C0_PAGE;

	switch (I2C0STAT & 0x0F) {

		case I2C_ADDR_RD:
			I2C0STAT_START = 0;
			I2C0STAT_RD = 0;

			//
			//	Data sent to Master after START Condition
			//

			I2C0DOUT = I2C_DATA;
			break;

		case I2C_ADDR_WR:
			I2C0STAT_START = 0;
			I2C0STAT_WR = 0;

			//
			//	Data read from Master after START Condition
			//

			I2C_DATA = I2C0DIN;
			break;


		case I2C_RD_DATA:
			// Check for NACK
			if (I2C0STAT_NACK) {
				// Master did not NACK
				// Stopping transfer of data
				I2C0STAT_NACK = 0;
			} else {

				//
				//	Data sent to Master
				//

				I2C0DOUT = I2C_DATA;
			}
			I2C0STAT_RD = 0;
			break;
		case I2C_WR_DATA:
			I2C0STAT_WR = 0;

			//
			//	Data read from Master
			//

			I2C_DATA = I2C0DIN;

			break;
		case I2C_STOSTA:
		case I2C_STOSTARD:
			I2C0STAT_STOP = 0;
			//
			//	STOP Condition received.
			//

			return;		// START Flag set, so re-enter ISR

			break;
		case I2C_STO:
			I2C0STAT_STOP = 0;
			//
			//	STOP Condition received.
			//
			break;
		default:
			break;
	}

	// Clear I2C interrupt flag
	I2C0STAT_I2C0INT = 0;
}

//-----------------------------------------------------------------------------
// Timer3 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// A Timer3 interrupt indicates an SMBus SCL low timeout.
// The I2C Slave is disabled and re-enabled here
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
{
	SFRPAGE = I2C0_PAGE;

	I2C0CN &= ~0x02;			// Disable I2C module
	I2C0CN |= 0x02;			// Re-enable I2C module

	SFRPAGE = TIMER3_PAGE;
	TMR3CN &= ~0x80;			// Clear Timer3 interrupt-pending flag
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
