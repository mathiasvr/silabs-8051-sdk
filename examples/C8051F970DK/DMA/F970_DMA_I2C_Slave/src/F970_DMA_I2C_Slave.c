//-----------------------------------------------------------------------------
// F970_DMA_I2C_Slave.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
// --------------------
//
// Example software to demonstrate the C8051F970 DMA mode of I2C Slave interface
// - Interrupt-driven I2C Slave implementation
// - Only slave states defined
// - N-bytes I2C Slave data holder used for both transmit and receive
// - Timer3 used by I2C Slave for SCL low timeout detection
// - ARBLOST support included
// - Pinout:
//    I2C Slave Pins on F970-DK(QFN-48 package)
//        SCL  P6.0
//        SDA  P6.1
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
// 4) Connect the I2C Slave to a SMBus/I2C master using the H7 header.
// 5) Run the code by selecting
//      Run -> Resume from the menus,
//      or clicking the Resume button in the quick menu,
//      or pressing F8.
// 
//      LED05 will toggle after a I2C transaction(read or write) is done.
//      The slave code will write data and read data from the same buffer, 
//      so a successive write and read will effectively echo the data written. 
//      To verify that the code is working properly, verify on the master 
//      that the data written is the same as the data received.
//
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.53
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (SHY)
//    - 1 AUG 2015

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F970_Register_Enums.h> // Bit Enums
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  SYSCLK         24500000       // System clock frequency in Hz
#define  IC_BUF_LEN     3              // Bytes of buffer for I2C Slave read/write

#define LED_ON          0              // Turns the LED on
#define LED_OFF         1              // Turns the LED off

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

xdata uint8_t I2C_DMA_buff[IC_BUF_LEN] = {0};     // Global holder for I2C slave data


bit DATA_READY = 0;                     // Set to '1' by the SMBus ISR
										// when a transaction is done(STOP is recived)

SI_SBIT(LED05, P0, 5);                    // LED05==LED_ON means ON

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void DMA_Init(void);

SI_INTERRUPT_PROTO(DMA_FULL_LENGTH_ISR, DMA0FULL_IRQn);
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

	DMA_Init();

	LED05 = LED_OFF;

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
// DMA_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the DMA channel 0 and 1 for I2C slave module
//
//
//-----------------------------------------------------------------------------
void DMA_Init (void)
{
	uint8_t SFRPAGE_save = SFRPAGE;
	SFRPAGE  = DMA0_PAGE;                         // Change Page register to DMA0 Page
	
	// Channel 0 for XRAM -> I2C
	DMA0SEL  = DMA0SEL_SELECT__CH0;               // Select DMA channel 0
  DMA0NBA  = (uint16_t)I2C_DMA_buff + 1;        // XRAM base address

	// Channel 1 for I2C -> XRAM
	DMA0SEL  = DMA0SEL_SELECT__CH1;               // Select DMA channel 1
	DMA0NBA  = (uint16_t)I2C_DMA_buff;            // XRAM base address
}


//-----------------------------------------------------------------------------
// DMA_Reset_Ch0
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Channel 0 is for XRAM -> I2C
// Reset some DMA channel0's setting for a new I2C transaction when START
// is received
//
//-----------------------------------------------------------------------------
void DMA_Reset_Ch0(void)
{
	DMA0SEL  = DMA0SEL_SELECT__CH0;                // Select DMA channel 0
	DMA0EN  &= ((DMA0EN & (~DMA0EN_CH0EN__BMASK))  // Disable DMA channel 0
				| DMA0EN_CH0EN__DISABLED);
	I2C0DOUT = I2C_DMA_buff[0];                    // Copy the first byte to buffer
	DMA0NAO  = 0;                                  // XRAM offset
	DMA0EN  |= DMA0EN_CH0EN__ENABLED;              // Enable DMA channel 0
}

//-----------------------------------------------------------------------------
// DMA_Reset_Ch1
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Channel 1 is for I2C -> XRAM
// Reset some DMA channel1's setting for a new I2C transaction when START
// is received
//
//-----------------------------------------------------------------------------
void DMA_Reset_Ch1(void)
{
	DMA0SEL  = DMA0SEL_SELECT__CH1;               // Select DMA channel 1
	DMA0EN  &= ((DMA0EN & (~DMA0EN_CH1EN__BMASK)) // Disable DMA channel 1
				| DMA0EN_CH1EN__DISABLED);
	DMA0NAO = 0;                                  // XRAM offset
	DMA0EN |= DMA0EN_CH1EN__ENABLED;              // Enable DMA channel 0
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
// - All incoming data is copied to global buffer by DMA
// - All outgoing data is copied from global buffer by DMA
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(I2CSlave_ISR, I2C0_IRQn)
{
	SFRPAGE = I2C0_PAGE;

	switch (I2C0STAT & 0x0F) {

		case I2C_ADDR_RD:
			I2C0STAT_START = 0; // Clear flag
			I2C0STAT_RD = 0;
			DMA_Reset_Ch0();    // Reset ch0's setting to let DMA copy data
			break;

		case I2C_ADDR_WR:
			I2C0STAT_START = 0; // Clear flag
			I2C0STAT_WR = 0;
			DMA_Reset_Ch1();    // Reset ch0's setting to let DMA copy data
			break;


		case I2C_RD_DATA:
			I2C0STAT_RD = 0;    // Clear flag if comes here
			break;
		case I2C_WR_DATA:
			I2C0STAT_WR = 0;    // Clear flag if comes here
			break;
		case I2C_STOSTA:
		case I2C_STOSTARD:
			I2C0STAT_STOP = 0;
			//	STOP Condition received.
			return;		// START Flag set, so re-enter ISR

			break;
		case I2C_STO:
			I2C0STAT_STOP = 0;
			DATA_READY = 1;
			//	STOP Condition received.
			break;
		default:
			break;
	}
	I2C0STAT_I2C0INT = 0;  // Clear I2C interrupt flag
}

//-----------------------------------------------------------------------------
// Timer3 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// A Timer3 interrupt indicates an SCL low timeout.
// The I2C Slave is disabled and re-enabled here
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
{
	SFRPAGE = I2C0_PAGE;

	I2C0CN &= ~0x02;			// Disable I2C module
	I2C0CN |= 0x02;			    // Re-enable I2C module

	SFRPAGE = TIMER3_PAGE;
	TMR3CN &= ~0x80;			// Clear Timer3 interrupt-pending flag
}

//-----------------------------------------------------------------------------
// DMA Full-Length Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// A DMA interrupt indicates a full-length transfer of a DMA channel is done.
//
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(DMA_FULL_LENGTH_ISR, DMA0FULL_IRQn)
{
	SFRPAGE = I2C0_PAGE;
	DMA0EN  &= (DMA0EN & (~(DMA0EN_CH0EN__BMASK|DMA0EN_CH1EN__BMASK))) | DMA0EN_CH0EN__DISABLED | DMA0EN_CH1EN__BMASK;

	// Reset the channel
	if (DMA0INT & DMA0INT_CH0I__BMASK)
	{
		DMA0SEL  = DMA0SEL_SELECT__CH0;             // Select DMA channel 0
		DMA0NAO = 0;                                // XRAM offset
	}

	if (DMA0INT & DMA0INT_CH1I__BMASK)
	{
		DMA0SEL  = DMA0SEL_SELECT__CH1;             // Select DMA channel 1
		DMA0NAO = 0;                                // XRAM offset
	}
	DMA0INT = 0;                                    // Clear flags
	DMA0EN |= DMA0EN_CH0EN__ENABLED | DMA0EN_CH1EN__ENABLED;            // Enable DMA channel 0/1 for next I2C transaction
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
