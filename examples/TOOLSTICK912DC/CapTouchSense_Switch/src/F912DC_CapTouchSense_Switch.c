//-----------------------------------------------------------------------------
// F912DC_CapTouchSense_Switch.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program lights up the yellow LED on the F912 ToolStick Daughter Card
// when the capacitive touch sense (contact-less) switch (P0.3) is touched.
//
// A relaxation oscillator is implemented using an on-chip analog comparator 
// with dedicated TouchSense inputs. The high-to-low transitions of the 
// relaxation oscillator are counted by Timer2. The relaxation oscillator 
// frequency depends on the capacitance of the touch sense trace capacitor. 
// Timer2 is operated in capture mode to determine the oscillation period.
//
// How To Test:
//
// Setup:
// 1) Connect the ToolStick912DC to a ToolStick Base Adapter.
// 2) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 3) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
//
// One Time Calibration (stored in non-volatile flash):
// 1) The Yellow LED will be lit. Place a finger on Cap Touch Sense Pad (P0.3
//    and hold it there. Do not remove this finger till step 3.
// 2) Press and hold switch SW2 (P0.2). The Red LED will be lit.
//    Release the switch after 1 second. The Red LED will turn off.
// 3) The Yellow LED will continue to be lit. The LED should switch off
//    once you remove your finger from the touch pad.
//
// Usage:
// 1) Touch SW03. The Yellow LED (P1.5) should light up in response.
//
// Target:         ToolStick912DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 21 MAR 2014
//
// Release 1.0
//    -Initial Revision (JH)
//    -27 JULY 2009
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F912_Register_Enums.h"
#include "stdio.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,    SFR_P1, 5);          // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED, SFR_P1, 6);          // '0' means ON, '1' means OFF
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK      24500000           // SYSCLK frequency in Hz

#define LED_ON              0          // Macros to turn LED on and off
#define LED_OFF             1

#define SW03_SENSITIVITY   300         // Sensitivity value used for both
                                       // Touch Sense switches. Larger values
                                       // make sthe switches more sensitive

#define CAL_ADDRESS         0          // Address in the scratchpad to store
                                       // the calibrated switch value

#define SCRATCHPAD          1          // Passed to the Flash access routines
                                       // to indicate that the calibration
                                       // constant is located in the scratchpad
               
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// Timer2 count of SW20 relaxation oscillator transitions
SI_UU16_t SW03_Timer_Count;                     

// Touch Switch: SW20==0 means finger sensed; SW20==1 means finger not sensed.
uint8_t SW03_Status;     
 
//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void PORT_Init (void);
void TouchSense_Init (void);

void Wait_MS(uint16_t ms);
void TouchSense_Update(void);
void Calibrate (void);
uint16_t Get_Calibration(void);

// FLASH read/write/erase routines
void FLASH_ByteWrite (uint16_t addr, uint8_t byte, uint8_t SFLE);
uint8_t   FLASH_ByteRead  (uint16_t addr, uint8_t SFLE);
void FLASH_PageErase (uint16_t addr, uint8_t SFLE);

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
   
   TouchSense_Init();                  // Initialize Comparator0 and 
                                       // Timer2 for use with TouchSense
   
   if( Get_Calibration() == 0xFFFF)
   {
      Calibrate ();
   }   

   while(1)
   {
      Wait_MS(25);                     // Polling Interval 
      
      if(!SW2)                         // If the P0.2 Switch is Pressed
      {
         Calibrate();                  // Calibrate the Touch Pads
      }

      TouchSense_Update();             // Update switch variables

      YELLOW_LED = SW03_Status;        // Set LED states based on the 
                                       // switch states
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
   
   // Update the status variable for SW20
   if(SW03_Timer_Count.u16 > (Get_Calibration() - SW03_SENSITIVITY))
   {
      SW03_Status = 0;
   
   } else
   {
      SW03_Status = 1;
   }

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
// Calibrate
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine will store the touch value in non-volatile Flash memory as
// the calibration threshold for the switch SW21.
//
// 1. Place a finger on the touch switch SW21, and hold it there.
// 2. Press switch SW2 (P0.7) for >1 sec and release using another finger.
// 3. Remove finger from SW21.
//-----------------------------------------------------------------------------
void Calibrate (void)
{
   uint8_t  EA_Save; 
   
   YELLOW_LED = LED_ON;         
   RED_LED = LED_ON;         
        
   while (SW2);                        // Wait till switch is pressed
   
   while (!SW2);                       // Wait till switch released   

   EA_Save = IE;                       // Preserve IE_EA
   IE_EA = 0;                             // Disable interrupts
   
   TouchSense_Update();                // Update switch variables
   
   // Erase the scratchpad
   FLASH_PageErase (CAL_ADDRESS, SCRATCHPAD);   
   
   // Write the expected switch value to the scratchpad
   FLASH_ByteWrite ((uint16_t)(CAL_ADDRESS+MSB), SW03_Timer_Count.u8[MSB],
                    SCRATCHPAD);
   FLASH_ByteWrite ((uint16_t)(CAL_ADDRESS+LSB), SW03_Timer_Count.u8[LSB],
                    SCRATCHPAD);
   
   RED_LED = LED_OFF; 
   
   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }
}

//-----------------------------------------------------------------------------
// Get_Calibration
//-----------------------------------------------------------------------------
//
// Return Value : 
//   1) uint16_t   -  calibration value
//                        range is full range of integer: 0 to 65335
// Parameters: None
//
// This routine reads the calibration value from Flash
//-----------------------------------------------------------------------------
uint16_t Get_Calibration(void)
{
   SI_UU16_t cal_value;   

   cal_value.u8[MSB] = FLASH_ByteRead ((uint16_t)(CAL_ADDRESS+MSB),
                                         SCRATCHPAD);
   cal_value.u8[LSB] = FLASH_ByteRead ((uint16_t)(CAL_ADDRESS+LSB),
                                         SCRATCHPAD);
   
   return cal_value.u16;

}

//-----------------------------------------------------------------------------
// Flash Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of the byte to write to
//   2) char byte - byte to write to Flash.
//
// This routine writes <byte> to the linear FLASH address <addr>.
//-----------------------------------------------------------------------------
void FLASH_ByteWrite (uint16_t addr, uint8_t byte, uint8_t SFLE)
{
   uint8_t EA_Save = IE;                    // Preserve IE_EA
   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, xdata, data); 
   //unsigned char xdata * data pwrite;// FLASH write pointer

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (char xdata *) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x01;                      // PSWE = 1
   
   if(SFLE)
   {
      PSCTL |= 0x04;                   // Access Scratchpad
   }

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   *pwrite = byte;                     // Write the byte

   PSCTL &= ~0x05;                     // SFLE = 0; PSWE = 0

   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }

}

//-----------------------------------------------------------------------------
// FLASH_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value :
//      uint8_t - byte read from Flash
// Parameters   :
//   1) FLADDR addr - address of the byte to read to
//
// This routine reads a <byte> from the linear FLASH address <addr>.
//-----------------------------------------------------------------------------
uint8_t FLASH_ByteRead (uint16_t addr, uint8_t SFLE)
{
   uint8_t EA_Save = IE;                    // Preserve IE_EA
   
   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pread, uint8_t, code, data); 
   //char code * data pread;             // FLASH read pointer

   uint8_t byte;

   IE_EA = 0;                             // Disable interrupts

   pread = (char code *) addr;

   if(SFLE)
   {
      PSCTL |= 0x04;                   // Access Scratchpad
   }

   byte = *pread;                      // Read the byte

   PSCTL &= ~0x04;                     // SFLE = 0

   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }

   return byte;

}

//-----------------------------------------------------------------------------
// FLASH_PageErase
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of any byte in the page to erase
//                    
//
// This routine erases the FLASH page containing the linear FLASH address
// <addr>.  Note that the page of Flash containing the Lock Byte cannot be
// erased if the Lock Byte is set.
//
//-----------------------------------------------------------------------------
void FLASH_PageErase (uint16_t addr, uint8_t SFLE)
{
   uint8_t EA_Save = IE;                    // Preserve IE_EA
   
   SI_SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, uint8_t, xdata, data); 
   //unsigned char xdata * data pwrite;// FLASH write pointer

   IE_EA = 0;                             // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x06;                      // Enable VDD monitor as a reset source
                                       // Leave missing clock detector enabled

   pwrite = (char xdata *) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1
   
   if(SFLE)
   {
      PSCTL |= 0x04;                   // Access Scratchpad
   }

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = 0;                        // Initiate page erase

   PSCTL &= ~0x07;                     // SFLE = 0; PSWE = 0; PSEE = 0

   if ((EA_Save & 0x80) != 0)          // Restore IE_EA
   {
      IE_EA = 1;
   }
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
// End Of File
//-----------------------------------------------------------------------------
