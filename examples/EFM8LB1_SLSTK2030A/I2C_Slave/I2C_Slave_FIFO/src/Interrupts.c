//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================


// USER INCLUDES
#include <SI_EFM8LB1_Register_Enums.h>
#include "EFM8LB1_I2C_Slave.h"

//-----------------------------------------------------------------------------
// I2C0_ISR
//-----------------------------------------------------------------------------
//
// I2C0 ISR Content goes here. Remember to clear flag bits:
// I2C0STAT::I2C0INT (I2C Interrupt)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(I2C0_ISR, I2C0_IRQn)
{
	//  Number of bytes in the transmit FIFO and shift register
	uint8_t txCnt = 0;

	if((I2C0FCN1 & I2C0FCN1_TFRQ__BMASK) && (txDataReady == 1))
	{
		// Firmware fill the TX FIFO until the TX FIFO full, and it should
		// locate at the beginning of the ISR, because the switch below may
		// consume so much system cycle.
		while((I2C0FCN1 & I2C0FCN1_TXNF__BMASK) == I2C0FCN1_TXNF__NOT_FULL)
		{
			if ((I2C0STAT & I2C0STAT_NACK__BMASK) == I2C0STAT_NACK__SET)
			{
				// Master did not NACK
				// Stopping transfer of data
				// Flush FIFO if there is data in it
				I2C0STAT &= ~I2C0STAT_NACK__BMASK;
			}
			else
			{
				I2C0DOUT = sendDataValue++;
				sendDataCnt++;
			}
		}
		// The TFRQ will not set I2C0INT bit, so don't need to clear
		// the I2C0INT when TFRQ happen.
		// Other interrupts may be generated while dealing with the TFRQ,
		// so just return to avoid clearing other interrupts.
		return;
	}

	if(I2C0FCN1 & I2C0FCN1_RFRQ__SET)
	{
		while((I2C0FCN1 & I2C0FCN1_RXE__BMASK) == I2C0FCN1_RXE__NOT_EMPTY)
		{ // Read data out
			i2cReceivedData   = I2C0DIN;
			// This example only supports host write 1 byte to slave
			// If want slave to accept more than 1 byte, use a buffer to save the data from FIFO
			dataReady = 1;
		}
		// Other interrupts may be generated while dealing with the RFRQ,
		// so just return to avoid clearing other interrupts.
		return;
	}

	switch (I2C0STAT & I2C_STATUS_VECTOR_MASK)
	{

		case I2C_ADDR_RD:  // START+ADDR+R
			I2C0STAT &= ~(I2C0STAT_START__BMASK | I2C0STAT_RD__BMASK);
			// The data be written to FIFO within Transmit FIFO Request (TFRQ) service routines.
			break;

		case I2C_ADDR_WR: // START+ADDR+W
			I2C0STAT &= ~(I2C0STAT_START__BMASK | I2C0STAT_WR__BMASK);
			break;

		case I2C_RD_DATA:
			// Check for NACK
			if ((I2C0STAT & I2C0STAT_NACK__BMASK) == I2C0STAT_NACK__SET)
			{
				// Master did not NACK
				// Stopping transfer of data
				// Flush FIFO if there is data in it
				I2C0STAT &= ~I2C0STAT_NACK__BMASK;
			}
			I2C0STAT &= ~I2C0STAT_RD__BMASK;
			break;

		case I2C_WR_DATA:  // FIFO is full, whatever slave ACK/NACK master
			I2C0STAT &= ~I2C0STAT_WR__BMASK;
			break;

		case I2C_STOSTA:
		case I2C_STOSTARD:
			I2C0STAT &= ~I2C0STAT_STOP__BMASK;
			txCnt = (I2C0FCT & I2C0FCT_TXCNT__FMASK) >> I2C0FCT_TXCNT__SHIFT;
			// One byte be shifted to the Shift register, also need to reload it.
			if(sendDataCnt > 2)
			{
				txCnt += 1;
			}

			if(txCnt > 0)
			{
				I2C0FCN0 |= I2C0FCN0_TFLSH__FLUSH;
				if(txCnt >= 2)
				{
					I2C0DOUT = sendDataValue-txCnt;
					I2C0DOUT = sendDataValue-txCnt+1;
					sendDataValue = sendDataValue-(txCnt-2);
					sendDataCnt = 2;
				}
				else if(txCnt == 1)
				{
					I2C0DOUT = sendDataValue-txCnt;
					sendDataCnt = 1;
				}
			}
			//
			//	STOP Condition received.
			//
			return;		// START Flag set, so re-enter ISR
			break;

		case I2C_STO:
			I2C0STAT &= ~I2C0STAT_STOP__BMASK;
			txCnt = (I2C0FCT & I2C0FCT_TXCNT__FMASK) >> I2C0FCT_TXCNT__SHIFT;
			// One byte be shifted to the Shift register, also need to reload it.
			if(sendDataCnt > 2)
			{
				txCnt += 1;
			}

			if(txCnt > 0)
			{
				I2C0FCN0 |= I2C0FCN0_TFLSH__FLUSH;
				if(txCnt >= 2)
				{
					I2C0DOUT = sendDataValue-txCnt;
					I2C0DOUT = sendDataValue-txCnt+1;
					sendDataValue = sendDataValue-(txCnt-2);
					sendDataCnt = 2;
				}
				else if(txCnt == 1)
				{
					I2C0DOUT = sendDataValue-txCnt;
					sendDataCnt = 1;
				}
			}
			//
			//	STOP Condition received.
			//
			break;
		default:
			break;
	}

	// Clear I2C interrupt flag
	I2C0STAT &= ~I2C0STAT_I2C0INT__BMASK;
}

//-----------------------------------------------------------------------------
// TIMER4_ISR
//-----------------------------------------------------------------------------
//
// TIMER4 ISR Content goes here. Remember to clear flag bits:
// TMR4CN0::TF4H (Timer # High Byte Overflow Flag)
// TMR4CN0::TF4L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(TIMER4_ISR, TIMER4_IRQn)
{
	SFRPAGE = PG3_PAGE;

	I2C0CN0 &= ~I2C0CN0_I2C0EN__BMASK;			// Disable I2C module
	I2C0CN0 |= I2C0CN0_I2C0EN__ENABLED;			// Re-enable I2C module

	SFRPAGE = PG2_PAGE;
	TMR4CN0 &= ~TMR4CN0_TF4H__BMASK;			// Clear Timer3 interrupt-pending flag
}


