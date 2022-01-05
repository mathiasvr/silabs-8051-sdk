//-----------------------------------------------------------------------------
// F931DC_CapTouchSense_Tune.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program can be used to determine the the sensitivity of the
// capacitive touch sense switch on the 'F931 ToolStick Daughter Card.
// It prints the oscillation period of the P0.3 Capacitive Touch Sense 
// switch to a UART terminal.
//
// When using touch sense switches, the oscillation period of the RC
// relaxation oscillator can be used to determine when the switch 
// has been pressed. The oscillation period will increase when the 
// switch capacitance increases (indicating a user's finger is placed
// on the switch). 
//
//
// How To Test:
//
// 1) Connect the ToolStick931DC to a ToolStick Base Adapter.
// 2) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 3) Set a break-point within the while(1) loop within the main function,
//    after TouchSense_Update() (line 137).
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) After the code has halted, add "SW03_Timer_Count.s16" to the expressions
//    view by selecting "Add new expression" in the expressions view, or by
//    highlighting SW03_Timer_Count, right clicking, and selecting
//    "Add Watch Expression..."
// 6) Observe the period given in SW03_Timer_Count. This period is given in
//    system clocks.
// 7) Touch the "TouchSense Switch" connected to P0.3 and resume the code
//    again. Observe the change in period given by SW03_Timer_Count.
//
//
// Target:         ToolStick931DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 21 MAR 2014
//
// Release 1.0
//    -Initial Revision (FB)
//    -16 NOV 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F930_Register_Enums.h"
#include "stdio.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,      SFR_P1, 5);        // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED,   SFR_P1, 6);        // '0' means ON, '1' means OFF
SI_SBIT(SW2,          SFR_P0, 2);        // SW2 == 0 means switch pressed
SI_SBIT(POWER_SOURCE, SFR_P1, 3);        // '0' means TS Power, '1' means battery

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK      24500000L          // SYSCLK frequency in Hz
#define BAUDRATE      115200           // Baud rate of UART in bps

#define SW03_THRESHOLD  1000           // Threshold for SW03

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// Timer2 count of SW03 relaxation oscillator transitions
SI_UU16_t SW03_Timer_Count;                     

// Touch Switch: SW03==0 means finger sensed; SW03==1 means finger not sensed.
uint8_t SW03_Status;     

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART0_Init (void);
void PORT_Init (void);
void TouchSense_Init (void);

void Wait_MS(uint16_t ms);
void TouchSense_Update(void);

//-----------------------------------------------------------------------------
// Generic UART definitions to support both Keil and SDCC
//-----------------------------------------------------------------------------
#ifdef SDCC
#include "sdcc_stdio.h"
#endif

char *GETS (char *, uint16_t);

#ifdef __C51__
#define GETS gets
#endif

#ifdef SDCC
#include "sdcc_stdio.c"
#endif

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
   PCA0MD &= ~0x40;                    // WDTE = 0 (clear watchdog timer
}
 
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{

                                       // enable)

   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   UART0_Init();                       // Initialize UART0
   
   TouchSense_Init();                  // Initialize Comparator0 and 
                                       // Timer2 for use with TouchSense
   
   while(1)
   {
      Wait_MS(100);

      TouchSense_Update();
   
      printf("\f");                    // Clear Screen
      printf("Oscillation Period:\n"); // Print oscillation period   
      printf("SW03: %5d\n\n",  SW03_Timer_Count.u16 );

      YELLOW_LED = SW03_Status;        // Update LED

      printf("Power Source: ");        // Write Label   
      
      if(!POWER_SOURCE)
      {
         printf("ToolStick Power");
      } else
      {
         printf("Battery Power");
      }

   }

}

//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TouchSense_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
//
//-----------------------------------------------------------------------------
void TouchSense_Update (void)
{  
   
   SI_UU16_t timer_count_A, timer_count_B;

   // Prepare Timer2 for the first TouchSense reading
   TMR2CN &= ~0x80;                    // Clear overflow flag
   while(!(TMR2CN & 0x80));            // Wait for overflow
   timer_count_A.u16 = TMR2RL;         // Record value
   
   // Prepare Timer2 for the second TouchSense reading
   TMR2CN &= ~0x80;                    // Clear overflow flag
   while(!(TMR2CN & 0x80));            // Wait for overflow
   timer_count_B.u16 = TMR2RL;         // Record value
   
   // Calculate the oscillation period
   SW03_Timer_Count.u16 = timer_count_B.u16 - timer_count_A.u16;
   
}

//-----------------------------------------------------------------------------
// Wait_MS
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters:
//   1) uint16_t ms - number of milliseconds of delay
//                        range is full range of integer: 0 to 65335
//
// This routine inserts a delay of <ms> milliseconds.
//-----------------------------------------------------------------------------
void Wait_MS(uint16_t ms)
{
   char i;

   TCON_TR0 = 0;                            // Stop Timer0
   
   TMOD &= ~0x0F;                      // Timer0 in 8-bit mode
   TMOD |= 0x02;
   
   CKCON &= ~0x03;                     // Timer0 uses a 1:48 prescaler
   CKCON |= 0x02;                   
    
   TH0 = -SYSCLK/48/10000;             // Set Timer0 Reload Value to 
                                       // overflow at a rate of 10kHz
   
   TL0 = TH0;                          // Init Timer0 low byte to the
                                       // reload value
   
   TCON_TF0 = 0;                            // Clear Timer0 Interrupt Flag
   IE_ET0 = 0;                            // Timer0 interrupt disabled
   TCON_TR0 = 1;                            // Timer0 on

   while(ms--)
   {
      for(i = 0; i < 10; i++)
      {
         TCON_TF0 = 0;
         while(!TCON_TF0);
      }
   }

   TCON_TF0 = 0;

}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TouchSense_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure Comparator 0 and Timer 2 for use with Touchsense.
//-----------------------------------------------------------------------------
void TouchSense_Init (void)
{
   // Initialize Comparator0
   CPT0CN = 0x8F;                      // Enable Comparator0; clear flags
                                       // select maximum hysterisis
   CPT0MD = 0x0F;                      // Comparator interrupts disabled, 
                                       // lowest power mode

   CPT0MX = 0x1C;                      // Positive Mux: TouchSense Compare
                                       // Negative Mux: P0.3 - TouchSense Switch
   
   // Initialize Timer2
   CKCON |= 0x10;                      // Timer2 counts system clocks
   TMR2CN = 0x16;                      // Capture mode enabled, capture 
                                       // trigger is Comparator0. 
                                       // Start timer (TMR2CN_TR2 = 1).

   SW03_Status = 1;                    // Set switch status to finger not 
                                       // detected.

}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P0.2   digital   open-drain    Switch P0.2
// P0.3   analog    open-drain    Touch Sense Switch
// P0.4   digital   push-pull     UART TX
// P0.5   digital   open-drain    UART RX
//
// P1.3   digital   open-drain    Power Source
// P1.5   digital   push-pull     Red LED
// P1.6   digital   push-pull     Yellow LED
//
// P2.0   analog    open-drain    Touch Sense Switch
// P2.1   analog    open-drain    Touch Sense Switch
//

//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   
   // Configure TouchSense switches
   P0MDIN &= ~0x08;                    // P0.3 is analog
   P0MDOUT &= ~0x08;                   // P0.3 is open-drain
   P0      |=  0x08;                   // P0.3 latch -> '1'
   
   // Configure Hardware Switch
   P0MDIN |= 0x04;                     // P0.2 is digital
   P0MDOUT &= ~0x04;                   // P0.2 is open-drain
   P0     |= 0x04;                     // Set P0.2 latch -> '1'
   
   // Configure Power Source Input   
   P1MDIN  |=  0x08;                   // P1.3 is digital
   P1MDOUT &= ~0x08;                   // P1.3 is open-drain
   P1      |=  0x08;                   // Set P1.3 latch -> '1'

   // Configure LEDs
   P1MDIN |= 0x60;                     // P1.5, P1.6 are digital
   P1MDOUT |= 0x60;                    // P1.5, P1.6 are push-pull

   // Configure UART   
   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output

   // Configure Crossbar
   XBR0    = 0x01;                     // Enable UART on P0.4(TX) and P0.5(RX)
   XBR2    = 0x40;                     // Enable crossbar and weak pull-ups
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal precision 
// oscillator at its maximum frequency and enables the missing clock 
// detector.
// 
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   
   OSCICN |= 0x80;                     // Enable the precision internal osc.
   
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc. 
                                       // divided by 1 as the system clock
   
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear SCON0_RI and SCON0_TI bits
   #if (SYSCLK/BAUDRATE/2/256 < 1) 
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   #elif (SYSCLK/BAUDRATE/2/256 < 4) 
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   #elif (SYSCLK/BAUDRATE/2/256 < 12) 
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   #else 
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   #endif

   TL1 = TH1;                          // Init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TCON_TR1 = 1;                            // START Timer1
   SCON0_TI = 1;                            // Indicate TX0 ready
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
