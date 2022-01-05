/**************************************************************************//**
 * (C) Copyright 2014 Silicon Labs Inc,
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 * 
 *****************************************************************************/

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB2_Register_Enums.h>                // SI_SFR declarations
#include "initialization.h"
#include "buffers.h"
#include "uart_0.h"
#include "display.h"

/**************************************************************************//**
 *
 * Main thread for VCPXpress UART demo.
 * 
 * This example implements a USB to uart bridge using a ping-pong buffering 
 * scheme for both Rx and Tx paths. It includes no flow control and has a 
 * fixed baud-rate of 115200 baud (ignores change requests from the host).
 *
 * The MCU enumerates as a COM port and connects to the board controllers COM port providing
 * bidirectional communication between the two COM ports.
 *
 * This example also displays characters received and transmitted on the LCD of the STK.
 *
 *****************************************************************************/


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
   PCA0MD &= ~0x40;                               
}
 
/**************************************************************************//**
 * @brief
 *   Main thread. Loops forever servicing usb->uart copies
 *   
 *****************************************************************************/
int main (void)
{  
   uint8_t i;
   uint8_t result = VCP_STATUS_OK;

   // Disable watchdog timer
   

   System_Init ();   // Call top-level initialization routine
   resetState();     // reset buffer statues and init uart
   UART0_readBuffer(uartRxBuf, USB_BLOCK_SIZE);// init first uart receive
   IE_EA = 1;        // enable global interrupts
   initDisplay();    //initialize display


   //Main Thread. Here we monitor the current buffer for each
   //  transfer and kick the transfer when appropriate.
   while(1)
   {
     // if usb is ready for the next Tx packet and the current Tx
     // buffer has data then send the current buffer.
     if(usbTxReady && *usbTxLen){

       //Copy data to display
    	 // This must happen first to get data on the LCD before
    	 // USB flushe's it.
       for (i=0; i<*usbTxLen; i++){
       	 printRx(usbTxBuf[i]);
       }

    	 //VCPXpress library is not reentrant so we disable
       //  the USB interrupt to ensure it doesn't fire during
       //  the block write call
       EIE1 &= ~EIE1_EUSB0__BMASK;
       usbTxReady = 0;
       result = Block_Write(usbTxBuf, *usbTxLen, usbTxLen);
       EIE1 |= EIE1_EUSB0__BMASK;

       //If our write request failed retry
       if(result != VCP_STATUS_OK)
       {
      	 usbTxReady = 1;
       }

       //Buffer will be switched by callback when transfer completes

     }
      
      //If USB receive has completed and the current buffer
      // is empty. Prime for next receive.
      if(usbRxReady && !*usbRxLen)
      {
        //VCPXpress library is not reentrant so we disable
        //  the USB interrupt to ensure it doesn't fire during
        //  the block read call
        EIE1 &= ~EIE1_EUSB0__BMASK;
        usbRxReady = 0;
        Block_Read(usbRxBuf, USB_BLOCK_SIZE, usbRxLen);
        EIE1 |= EIE1_EUSB0__BMASK;

        //If our read request failed retry
        if(result != VCP_STATUS_OK)
        {
        	usbRxReady = 1;
        }

        //Buffer will be switched by callback when transfer completes.

      }// If usb ready for next receive

      //If uart tx has completed and the current buffer
      // has data then start next uart transmit
      if(uartTxReady && *uartTxLen)
      {
    	  //Copy data to display
      	// Do this first so UART doesn't flush data before
      	// display update is finished.
      	for (i=0; i<*uartTxLen; i++){
    	  	printTx(uartTxBuf[i]);
    	  }

      	//We need to avoid having the UART ISR fire when we are
    	  // modifying the library. However we also don't want lower
    	  // priority interrupts to fire so we turn off all interrupts
    	  IE_EA = 0;
    	  UART0_writeBuffer(uartTxBuf, *uartTxLen);
    	  uartTxReady = 0;
    	  IE_EA = 1;

     	  //Buffer will be switched by callback when transfer completes.
      }

      //If uart rx has completed and the current buffer
      // is empty then start receiving new data.
      if(uartRxReady && !*uartRxLen)
      {
    	  //Block UART (and lower priority) interrupts
    	  IE_EA = 0;
    	  UART0_readBuffer(uartRxBuf, USB_BLOCK_SIZE);
    	  uartRxReady = 0;
    	  IE_EA = 1;

    	  //Buffer will be switched by callback when transfer completes.
      }

   }// while(forever)
}//main()
