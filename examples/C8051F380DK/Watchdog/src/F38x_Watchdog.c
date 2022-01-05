//-----------------------------------------------------------------------------
// F38x_Watchdog.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt  
//
// Program Description:
//
// This program helps the user to learn about operating the Watch Dog Timer.
// The WDT is used to generate resets if the times between writes to the WDT 
// update register (PCA0CPH4) exceed a specified limit. The WDT can be disabled 
// and enabled in the software as needed. When enabled the PCA Module 4 acts as
// the WDT. This program resets the MCU when P0.7 switch is pressed. Also upon 
// reset the LED blinks approximately ten times faster when compared to before. 
// The reset is caused due to a WDT overflow and can be confirmed by checking 
// the value of the RSTSRC register where bit 3 is set to indicate a reset
// caused by WDT.
// 
// How to Test:
// 1) Ensure shorting blocks are place on the following:
//    - J19: Power - P1_PWR or SER_PWR
//    - J7: +3VD - VDD
//    - J12: P2.0 - SW2.0
//           P2.2 - P2.2_LED
// 2) Connect the USB Debug Adapter to J9. If P1_PWR power is selected on J19,
//    connect the 9V power adapter to P1.
// 4) Compile and download code to a 'F380 device on a C8051F380-TB development
//    board selecting Run -> Debug from the menus, clicking the Debug button in
//    the quick menu, or pressing F11.
// 5) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8:
//        - The test will blink the LED at a rate of 10Hz until the switch SW2
//          (P0.7) is pressed.
//        - Once the the switch is pressed and held for a int32_t enough time, 
//          it will prevent the WDT from being touched and the WDT will
// 		    cause a reset.
//        - Upon reset the code checks for a WDT reset and blinks the LED ten 
// 		  times faster than before to indicate the same.      
//
// Target:         C8051F380
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - Replaced C51 code with compiler agnostic code
//    - 08 JAN 2014
//
// Release 1.0
//    -Initial Revision (SM)
//    -12 JUL 2006	

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F380_Register_Enums.h>

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F38x
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       12000000         // SYSCLK frequency in Hz

SI_SBIT(LED, SFR_P2, 2);                 // LED='1' means ON
SI_SBIT(SW2, SFR_P2, 0);                 // SW2='0' means switch pressed

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(Timer2_ISR, TIMER2_IRQn);

void OSCILLATOR_Init (void);
void PORT_Init (void);
void PCA_Init (void);
void Timer2_Init (int16_t counts);

//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------
//
// The MAIN routine performs all the intialization, and then loops until the 
// switch is pressed. When SW2 (P2.0) is pressed the code checks the RSTSRC 
// register to make sure if the last reset is because of WDT.
//-----------------------------------------------------------------------------
void main (void) 
{
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer 
                                       // enable)

   OSCILLATOR_Init ();                 // Initialize system clock to 12/8 MHz
   PCA_Init();                         // Intialize the PCA
   PORT_Init();                        // Initialize crossbar and GPIO
      
   if ((RSTSRC & 0x02) == 0x00)        // First check the PORSF bit. if PORSF
   {                                   // is set, all other RSTSRC flags are
                                       // invalid

   	   // Check if the last reset was due to the Watch Dog Timer
   	   if (RSTSRC == 0x08)                    	
   	   { 
	      Timer2_Init (SYSCLK / 12 / 100);    // Enable Timer to make LED blink
       	  IE_EA = 1;                        	  // Enable global interrupts

          while(1);                           // Wait forever
   	   }	
   	   else 
   	   {
	      Timer2_Init (SYSCLK / 12 / 10);    // Init Timer2 to generate
		                                     // interrupts at a 10Hz rate.
	   }                                   
   }

   // Calculate Watchdog Timer Timeout
   // Offset calculated in PCA clocks
   // Offset = ( 256 x PCA0CPL4 ) + 256 - PCA0L
   //        = ( 256 x 255(0xFF)) + 256 - 0
   // Time   = Offset * (12/SYSCLK)   
   //        = 524 ms ( PCA uses SYSCLK/12 as its clock source)		
	   	
   PCA0MD  &= ~0x40;                   // WDTE = 0 (clear watchdog timer enable)
   PCA0L    = 0x00;               	   // Set lower byte of PCA counter to 0  
   PCA0H    = 0x00;               	   // Set higher byte of PCA counter to 0
   PCA0CPL4 = 0xFF;               	   // Write offset for the WDT 
   PCA0MD  |= 0x40;               	   // Enable the WDT
   IE_EA = 1;					    	   // Enable global interrupts	
	
   //--------------------------------------------------------------------------
   // Main Application Loop/Task Scheduler
   //--------------------------------------------------------------------------

   while (1) 
   {   
	  //----------------------------------------------------------------------
	  // Task #1 - Check Port I/O
	  //----------------------------------------------------------------------                              
      while(!SW2);                  // Force the MCU to stay in this task as 
						   			// int32_t as SW2 is pressed. This task must 
						   			// finish before the watchdog timer 
   									// timeout expires.
	
	  //----------------------------------------------------------------------
	  // Task #2 - Reset Watchdog Timer
	  //----------------------------------------------------------------------                  	   
	  PCA0CPH4 = 0x00;              // Write a 'dummy' value to the PCA0CPH4 
						   			// register to reset the watchdog timer 
						   			// timeout. If a delay longer than the 
						   			// watchdog timer delay occurs between 
						   			// successive writes to this register,
	   								// the device will be reset by the watch
	   								// dog timer.                                
    }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 12 MHz / 8 
// oscillator as its clock source.  Also enables missing clock detector reset.
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{ 
   OSCICN = 0x80;                      // Configure internal oscillator for
                                       // its lowest frequency
   RSTSRC = 0x04;                      // Enable missing clock detector
}

//-----------------------------------------------------------------------------
// PCA_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the PCA to use the SYSCLK / 12 
// as its clock source.  It also sets the offset value by writing to PCA0CPL2.
//-----------------------------------------------------------------------------
void PCA_Init()
{
    PCA0CN     =  0x40;                // PCA counter enable
    PCA0MD    &= ~0x40 ;               // Watchdog timer disabled-clearing bit 6
    PCA0MD    &=  0xF1;                // timebase selected - System clock / 12
    PCA0CPL4   =  0xFF;                // Offset value
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the Crossbar and GPIO ports.
// P2.2   digital   push-pull     LED
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   XBR0     = 0x00;                    // No digital peripherals selected
   XBR1     = 0x40;                    // Enable crossbar and weak pull-ups
   P2MDOUT |= 0x04;                    // Enable LED as a push-pull output
}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
// Return Value : None
// Parameters   :
//   1)  int16_t counts - calculated Timer overflow rate
//                    range is positive range of integer: 0 to 32767 
//
// Configure Timer2 to 16-bit auto-reload and generate an interrupt at 
// interval specified by <counts> using SYSCLK/48 as its time base.
//-----------------------------------------------------------------------------
void Timer2_Init (int16_t counts)
{
   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON  &= ~0x60;                    // Timer2 clocked based on TMR2CN_T2XCLK;

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xffff;                   // Set to reload immediately
   IE_ET2     = 1;                        // Enable Timer2 interrupts
   TMR2CN_TR2     = 1;                        // Start Timer2
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
//
// This routine changes the state of the LED whenever Timer2 overflows.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer2_ISR, TIMER2_IRQn)
{
   TMR2CN_TF2H = 0;                           // Clear Timer2 interrupt flag
   LED = ~LED;                         // Change state of LED
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
