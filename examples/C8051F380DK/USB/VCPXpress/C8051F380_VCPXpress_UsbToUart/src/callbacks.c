/**************************************************************************//**
 * @file
 * @brief   Callbacks and ISR's.
 * @author  Silicon Laboratories
 * @version 1.0.0 (DM: July 14, 2014)
 *
 *******************************************************************************
 * @section License
 * (C) Copyright 2014 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *******************************************************************************
 *
 * Definitions and external declarations for VCPXpress_callback.c
 * We also place the UART and TIMER ISR here.
 * 
 *****************************************************************************/

#include <SI_C8051F380_Register_Enums.h>    // SI_SFR declarations
#include "callback.h"
#include "buffers.h"
#include "initialization.h"

//Here we say if we are idle for 10 flush.
#define RX_FLUSH_TIME 10      //<! Number of milliseconds the uart needs
                              //   to be idle before we flush whatever
                              //   data we have.
static uint16_t rxIdleTime;   //!< Number of bit times since last byte
                              //   received.

//These variables track the current level of the uart buffers. We need to do this
// since making live changes to *uartRxLen will trigger the USB to send after the
// first byte is received. Instead we keep uartRxLen at 0 untill the buffer is full
// and then update it. We do th same for the tx side.
SI_SEGMENT_VARIABLE(rxCount, uint8_t, SI_SEG_XDATA); //tracks bytes REMAINING in the current uart rx buffer.
SI_SEGMENT_VARIABLE(txCount, uint8_t, SI_SEG_XDATA); //tracks the bytes REMAINING in the current uart tx buffer.

// These are pointers to track the current active position in the uart buffers. We can not modify
// uartTxBuf/uartRxBuf directly since they need to point to the start of the buffers for the switch operation
// to work properly.
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(uartTxCurrent, uint8_t, SI_SEG_XDATA, SI_SEG_XDATA);// tracks current tx byte.
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(uartRxCurrent, uint8_t, SI_SEG_XDATA, SI_SEG_XDATA);// tracks current rx byte.

/**************************************************************************//**
 *  @brief
 *    Starts transmiting the data in the current uartTxBuf
 *
 *****************************************************************************/
void startUartTx()
{
	//Setup a transmission and kick the transfer
  txCount = *uartTxLen -1;
  uartTxCurrent = uartTxBuf+1;
  uartTxReady = 0;
  SBUF0 = *uartTxBuf;
}

/**************************************************************************//**
 *  @brief
 *    Starts to receive data in the current uartRxBuf
 *
 *****************************************************************************/
void startUartRx()
{
	//Setup the uart to receive a block
	rxCount = USB_BLOCK_SIZE;
	uartRxCurrent = uartRxBuf;
	uartRxReady = 0;
}

/**************************************************************************//**
 *  @brief
 *    VCPXpress callback
 *    
 *  Called by VCPXpresss. We only need to handle TX/RX complete and device_open but
 *  we could extend functionatly by handling other events (like baud rate change
 *  requests)
 *   
 *****************************************************************************/
void VCP_Callback(void)
{
   uint32_t INTVAL = Get_Callback_Source();

   // Device was opened so reset
   if (INTVAL & DEVICE_OPEN)
   {

     //Reset variable state
     resetState();
     rxIdleTime = 0;

   }// if DEVICE_OPEN

   // Rx compete ready for next RX buffer.
   if (INTVAL & RX_COMPLETE)
   {
  	 if(*usbRxLen != 0)
  	 {
  		 //if we received a packet of non-zero length switch to next
  		 // buffer. *usbRxLen is already set by usb library.
       usbSwitchRxbuffer();
  	 }

  	 // Flag main that next transfer can start
  	 // in the case of a zlp this will restart the receive.
  	 usbRxReady = 1;

   }// if RX_COMPLETE

   // Rx compete ready for next TX buffer.
   if (INTVAL & TX_COMPLETE)
   {
  	 //Only switch buffer if we completed a packet of non-zero length
  	 if(*usbTxLen != 0)
  	 {
    	 // When TX complete perform buffer switch
       *usbTxLen = 0;       // mark current buffer empty now that TX is done
       usbSwitchTxbuffer(); // switch usb buffer
  	 }

  	 // Flag main while loop that it can send data up
  	 usbTxReady = 1;
   
   }// if TX_COMPLETE
}//VCPXpress_API_CALLBACK()


/**************************************************************************//**
 *  @brief
 *    TIMER0 Interrupt Service Routine
 *    
 *  The UART ISR only causes data to be sent to the host on a buffer
 *  overflow. If a small number of bytes is received followed by a long
 *  pause we want to flush those bytes in a reasonable time frame. This 
 *  ISR measures the time since the last bye received. If that time exceeds
 *  a preset limit then the rx buffer is flushed immediately.
 *  
 *  Here we use TIMER0 to setup a static timeout. However for lower baud rates
 *  we could turn on the TIMER0 (baud rate generation) interrupt and make our 
 *  timeout in terms of uart bit widths. For high baud rates this does not work
 *  since the timer fires often and the ISR ends up eating a lot of cpu cycles.
 *   
 *****************************************************************************/
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
  static uint8_t previousBytes;  //!< Previous number of bytes remaining
  uint8_t numBytes;              //!< Num bytes in the uartRx buffer

  //Reload time for next 1ms.
  TH0 = -(SYSCLK/1000)>>8;
  TL0 = -(SYSCLK/1000) & 0x0FF;

  numBytes = USB_BLOCK_SIZE - rxCount;

  //if the uart is ready for RX (no transfer pending) then we can't be
  // waiting on data. If the buffer is empty then we don't need to flush
  if (uartRxReady || !numBytes)
  {
  	return;
  }

  //if we see a tick reset our idle time
  if (previousBytes != numBytes)
  {
	  rxIdleTime = 0;
	  previousBytes = numBytes;
  }

  //if timeout occurs then flush RX buffer.
  if(rxIdleTime > RX_FLUSH_TIME)
  {
	  //Block UART (and lower priority) interrupts
	  IE_EA = 0;

	  //Set length of Rx buffer to the number of bytes received.
	  *uartRxLen = USB_BLOCK_SIZE - rxCount;
    switchRxBuffer();      // Switch buffer

    //If next buffer is empty then start the read immediately.
	  //Otherwise mark ready so main loop ill start read when buffer is ready.
		if(!*uartRxLen)
		{
			startUartRx();
		}
		else
		{
			uartRxReady = 1;              // Flag main that next Read can start
		}

		IE_EA = 1;
		//END interrupt blocking

	  //reset timeout
	  rxIdleTime = 0;
  }
  else
  {
    //Increment idle time
    rxIdleTime++;
  }

}// timer0 IST

SI_INTERRUPT(UART0_ISR, UART0_IRQn)
{
  uint8_t flags;

  //Buffer and clear flags immediately so we don't miss an interrupt while processing
  flags = SCON0 & (SCON0_RI__BMASK | SCON0_TI__BMASK);
	SCON0 &= ~flags;

	if (rxCount && (flags &  SCON0_RI__SET))
  {
		//Read byte
		*uartRxCurrent = SBUF0;
		++uartRxCurrent;
		--rxCount;

		//handle buffer overflow
		if (!rxCount)
		{
			// When Rx completes (full) perform buffer switch
			*uartRxLen  = USB_BLOCK_SIZE; // mark current buffer full
		  switchRxBuffer();             // switch to next buffer

			//If next buffer is empty then start the read immediately.
		  //Otherwise mark ready so main loop ill start read when buffer is ready.
			if(!*uartRxLen)
			{
			  // If you do not do this and always use the main loop to start the next
				// receive it may take to long since the main loop can be bogged down in
				// starting USB transfers.
				startUartRx();
			}
			else
			{
				uartRxReady = 1;              // Flag main that next Read can start
			}
		}
  }

  if ((flags & SCON0_TI__SET))
  {
  	//If space remains in uartTx buffer.
  	if (txCount){
      SBUF0 = *uartTxCurrent;
      ++uartTxCurrent;
      --txCount;
    }
    else
    {
      // When TX complete perform buffer switch
    	*uartTxLen = 0;   // Update buffer len to trigger usb to receive
    	switchTxBuffer(); // Switch to next buffer

    	//If next buffer has data start sending else signal main loop to kick us
    	//when data arrives
    	if (*uartTxLen)
    	{
    		startUartTx();
    	}
    	else
    	{
      	uartTxReady = 1;  // Flag main while loop that it can start transfer
    	}
    }
  }
}



