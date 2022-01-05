//-----------------------------------------------------------------------------
// F9xx_Main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates the current consumption of the device in different
// power modes. The power modes available are Normal, Idle, Legacy Stop, Suspend,
// Sleep, and Sleep with RTC. See F9xx_Config.h for more details.
//
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect the USB Debug Adapter to J4.
// 3) Turn the POWER switch (SW5) to the "ON" position.
// 4) In "F9xx_Config.h", define POWER_MODE to the desired mode.
// 5) Compile and download code to a 'F91x device on a C8051F91x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 6) Remove all jumpers except for J11.
// 7) Disconnect the USB Debug Adapter from the target board. 
// 8) Power Cycle the board (or press the reset switch).
// 9) Measure the current flowing through J17 or H2.
//
//
// Target:         C8051F90x-C8051F91x
// Tool chain:     Silicon Labs Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 14 JAN 2014
//
// Release 1.2 
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.1
//    - Added support for 'F91x-'F90x devices (FB)
//    - Added options to disable VDD and VBAT Supply Monitors
//    -15 JUL 2009
//
// Release 1.0
//    -Initial Revision (FB)
//    -02 OCT 2008
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <SI_C8051F912_Register_Enums.h>

#include "Global_Const.h"
#include "Imeasure.h"
#include "F9xx_Config.h"

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
SI_SBIT(SW2,        SFR_P0, 2);          // SW2 == 0 means switch pressed
SI_SBIT(SW3,        SFR_P0, 3);          // SW3 == 0 means switch pressed

#define RTC0CN    0x04                 // RTC address of RTC0CN register                
#define RTC0XCN   0x05                 // RTC address of RTC0XCN register 
#define RTC0XCF   0x06                 // RTC address of RTC0XCF register

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

volatile SI_LOCATED_VARIABLE_NO_INIT(DEVICE_ID_16K, uint8_t, code, 0x3FFE);
volatile SI_LOCATED_VARIABLE_NO_INIT(DEVICE_ID_64K, uint8_t, code, 0xFFFE);

#define C8051F930  0x56
#define C8051F931  0x5E
#define C8051F920  0xB1
#define C8051F921  0xB3

#define C8051F912  0xD3
#define C8051F911  0xD2
#define C8051F902  0xD1
#define C8051F901  0xD0

uint8_t Device_ID = 0x0000;

//-----------------------------------------------------------------------------
// Function Pointer
//-----------------------------------------------------------------------------
#if(COMPILER == KEIL)
void (*Imeasure_Function_Pointer)(void);
#elif(COMPILER == RAISONANCE)
void Imeasure(void);
#endif
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void Wait_US(uint16_t us);
void Wait_MS(uint16_t ms);

void RTC_Init (void);
uint8_t RTC_Read (uint8_t);
void RTC_Write (uint8_t, uint8_t);

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
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{


                                       // enable)
   
   //--------------------------------------------------------------------------
   // Low Power Startup Delay
   //--------------------------------------------------------------------------
   // In one-cell mode, decreasing the MCU power consumption for a short period
   // after Power-On Reset allows the decoupling capacitors time to recharge.
   // This allows the MCU to successfully start up at lower voltages when 
   // the source impedance is very high. Examples of high source impedance 
   // include a battery which is near its end of life or when starting up 
   // while being connected to a current meter.
   
   if(RSTSRC & 0x02 || PMU0CF & 0x10)
   {
      Wait_MS (100);                   // Insert low power delay 
   }

   //--------------------------------------------------------------------------
   // Safety Trap
   //--------------------------------------------------------------------------
   // Create a trap to catch code execution in case we place the device in
   // a state in which we are unable to connect. Holding SW2 down during a 
   // reset will cause the device to enter this trap.

   if( (SW2 == 0) )
   {
      while(1);
   }

   //--------------------------------------------------------------------------
   // Disable VBAT Supply Monitor
   //--------------------------------------------------------------------------
   // The IDE cannot connect to the device when the VBAT supply monitor is 
   // disabled. Insert a 3-5 second delay before disabling the VBAT supply
   // monitor to enable debugging. The proper way to connect to the device
   // when the VBAT supply monitor is disabled is:
   //
   //    1. Trigger any device reset (e.g. reset pin, etc).
   //
   //    2. Within 3-5 seconds (based on delay below), initiate a connection
   //       using the IDE.
    
   #if (DISABLE_VBAT_SUPPLY_MONITOR)

   Wait_MS (3000);

   SFRPAGE = 0x0F;
   PMU0MD |= 0x20; 
   SFRPAGE = 0x00;
   #endif

   //--------------------------------------------------------------------------
   // Disable VDD/DC+ Supply Monitor
   //--------------------------------------------------------------------------
   #if (DISABLE_VDD_SUPPLY_MONITOR)
   VDM0CN = 0x00;
   RSTSRC = 0x00;
   #endif

   //--------------------------------------------------------------------------
   // Determine the Device ID
   //--------------------------------------------------------------------------
   
   // Attempt to read the device ID from address 0xFFFE (for a 64KB device).
   // If the read is successful, the device ID is copied from the Flash location
   // to the <Device_ID> variable. If a Flash Error Reset occurs, we are on 
   // a 16KB or smaller device. The device ID will be available from 0x3FFE.
   if(RSTSRC == 0x40)
   {
      Device_ID = DEVICE_ID_16K;
   } 
   else
   {
      Device_ID = DEVICE_ID_64K;
   }

   //--------------------------------------------------------------------------
   // Optimize One-Cell Mode Operation
   //--------------------------------------------------------------------------
   // These instructions have no effect when operating in two-cell mode
   
   DC0CN = 0x81;                       // Use the large switch and enable
                                       // minimum pulse width

   //DC0CF |= 0x04;                    // Enable 500mA peak current

   if(Device_ID == C8051F912 || Device_ID == C8051F902)
   {
      // Enable Low Power Mode on the DC-DC Converter
      DC0CF |= 0x80;
   } 

   //--------------------------------------------------------------------------
   // MCU Initialization
   //--------------------------------------------------------------------------
   PORT_Init();                        // Initialize Port I/O
   OSCILLATOR_Init ();                 // Initialize Oscillator

   //--------------------------------------------------------------------------
   // Sleep or Suspend Mode
   //--------------------------------------------------------------------------
   #if ((POWER_MODE == SLEEP) || (POWER_MODE == RTCSLEEP) || (POWER_MODE == SUSPEND))
      while(1)
      {
         CLKSEL = 0x14;                // Force the clock select to
                                       // low power oscillator / 2 per
                                       // 'F930/31/20/21 errata recommendation
                                       
         if((CLOCK_SELECTION & 0x07) == PRECISION_OSC)
         {
            REG0CN &= ~0x10;           // Disable precision oscillator bias   
         }   


         while ((CLKSEL & 0x80) == 0); // Wait for divider to be applied                       
         
         FLSCL &= ~0x40;               // Clear the BYPASS bit
         FLWR  =   0x01;               // to enable the one-shot
         
         #if((POWER_MODE == RTCSLEEP) && ((CLOCK_SELECTION & 0x0F) != SMARTCLOCK_OSC))
         RTC_Init();
         #endif
         
         PMU0CF = 0x20;                // Clear all wake-up source flags
         PMU0CF = POWER_MODE;          // Enter the low power mode
      
         #if(POWER_MODE == SLEEP || POWER_MODE == RTCSLEEP)
            NOP();                     // Do not place a breakpoint on
                                       // this instruction if waking up
                                       // from Sleep Mode. Placing a 
                                       // breakpoint here will cause
                                       // the IDE to disconnect.
            
            NOP(); NOP(); NOP();       // For 'F91x-'F90x devices, 4 NOPs
                                       // are required after waking up
                                       // from Sleep mode.
         #endif
         
         if((CLOCK_SELECTION & 0x07) == PRECISION_OSC)
         {
            REG0CN |= 0x10;            // Enable precision oscillator bias   
            Wait_US(4);                // Insert 4uS  delay
         }  
      
         CLKSEL = CLOCK_SELECTION;     // Restore selected system clock
         while ((CLKSEL & 0x80) == 0); // Wait for divider to be applied         

         // Restore the oneshot setting
         #if((SYSCLK > 10000000))              
         FLSCL |= 0x40;                // Set the BYPASS bit
         #endif         

         // If the wake-up source was a falling edge on /RST
         #if(POWER_MODE == SLEEP || POWER_MODE == SUSPEND)
         if(PMU0CF & 0x10)
         {
            Wait_US(15);               // Wait at least 15 us before 
                                       // allowing the MCU to go back
                                       // into the low power mode. 
                                       // This gives the IDE/Debug 
                                       // Adapter time to synchronize
                                       // with the MCU and allows the MCU
                                       // to respond to a Pin Reset.
         }
         #endif
      }
   #endif

   //--------------------------------------------------------------------------
   // Idle Mode
   //--------------------------------------------------------------------------
   #if (POWER_MODE == IDLE)

      while(1)
      {  
         FLSCL &= ~0x40;               // Clear the BYPASS bit
         FLWR  =   0x01;               // to enable the one-shot
         
         PCON = POWER_MODE;

         // Restore the oneshot setting
         #if(SYSCLK > 10000000)              
         FLSCL |= 0x40;                // Set the BYPASS bit
         #endif

      }
   #endif

   //--------------------------------------------------------------------------
   // Stop Mode
   //--------------------------------------------------------------------------
   #if (POWER_MODE == STOP)

      if((CLOCK_SELECTION & 0x07) != PRECISION_OSC)
      {
         REG0CN |= 0x10;               // Enable precision oscillator bias   
         Wait_US(4);                   // Insert 4uS  delay
      }  

      OSCICN |= 0x80;                  // Select precision oscillator as 
      CLKSEL = 0x00;                   // clock source
      while(!(CLKSEL & 0x80));         // Wait for CLKRDY


      PMU0CF = 0x20;                   // Clear wake-up flags to disable
                                       // the low power oscillator

      if(Device_ID == C8051F930 || Device_ID == C8051F931 ||
         Device_ID == C8051F920 || Device_ID == C8051F921   )
      {
         REG0CN &= ~0x10;              // Disable precision oscillator bias
      }                                // if enabled only on 'F93x devices
                                       // Not needed for 'F91x devices
      
      FLSCL &= ~0x40;                  // Clear the BYPASS bit
      FLWR  =   0x01;                  // to enable the one-shot
         
      PCON = POWER_MODE;               // A reset is required to exit this mode

   #endif
      
   //--------------------------------------------------------------------------
   // Normal Mode
   //--------------------------------------------------------------------------
   // Infinite Loop For Measuring Current in Normal Mode
   // Note: Ensure that this loop does not bridge a 64-byte flash row
   // boundary. To ensure the while(1) statement is located on a single
   // row, manually assign its location in Flash.
   #if(COMPILER == KEIL)
   Imeasure_Function_Pointer = IMEASURE_FUNCTION_LOCATION;
   Imeasure_Function_Pointer();
   #elif(COMPILER == RAISONANCE)
   Imeasure();
   #endif

   //--------------------------------------------------------------------------
   // Unreachable Code
   //--------------------------------------------------------------------------
   // Code execution should never reach this point.
   Wait_US(0);                         // Prevent Uncalled Function Warning
   RTC_Init();                         // Prevent Uncalled Function Warning
   while(1);
                                          
}                                    

#if(COMPILER == RAISONANCE)
at IMEASURE_FUNCTION_LOCATION void Imeasure(void) 
{
   while(1);
}
#endif

//-----------------------------------------------------------------------------
// Wait_US
//-----------------------------------------------------------------------------
// Insert <us> microsecond delays using an on-chip timer.
// 
void Wait_US(uint16_t us)
{

   TMR2CN = 0x00;                      // Set Timer2 control register to its
                                       // reset value

   IE_ET2 = 0;                            // Disable Timer 2 interrupts   

   CKCON |= 0x10;                      // Timer2 uses SYSCLK
   
   // Setup Timer2 to overflow at a 1 MHz Rate
   TMR2RL = -(SYSCLK/1000000);         // Reload value to be used in Timer2
   TMR2 = TMR2RL;                      // Init the Timer2 register

   TMR2CN = 0x04;                      // Enable Timer2 in auto-reload mode
   
   while(us)
   {
      TMR2CN &= ~0x80;                 // Clear the Timer2 overflow flag
      while(!(TMR2CN & 0x80));         // Wait for Timer2 overflow
      us--;                            // Decrement remaining time      
   }

   TMR2CN = 0x00;                      // Disable Timer2
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

   char CLKSEL_save = CLKSEL; 

   TCON_TR0 = 0;                            // Stop Timer0
   
   TMOD &= ~0x0F;                      // Timer0 in 8-bit mode
   TMOD |= 0x02;
   
   CKCON &= ~0x07;                     // Timer0 uses a 1:48 prescaler
   CKCON |= 0x02;                   
    

   
   TH0 = -20000000/128/48/1000;        // Set Timer0 Reload Value to 
                                       // overflow at a rate of 1kHz
   
   TL0 = TH0;                          // Init Timer0 low byte to the
                                       // reload value
   
   TCON_TF0 = 0;                            // Clear Timer0 Interrupt Flag
   IE_ET0 = 0;                            // Timer0 interrupt disabled
   TCON_TR0 = 1;                            // Timer0 on
   
   CLKSEL = 0x74;
   
   while(ms--)
   {
     
      TCON_TF0 = 0;
      while(!TCON_TF0);

   }
   
   CLKSEL = CLKSEL_save;   

   TCON_TF0 = 0;

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
// This function initializes the system clock to use the selected oscillator
// and divide value.
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   
   #if(ENABLE_MISSING_CLOCK_DETECTOR)   
      
      #if (DISABLE_VDD_SUPPLY_MONITOR)
      RSTSRC = 0x04;                   // Enable missing clock detector and
                                       // leave VDD Monitor disabled.
      #else                                                   

      RSTSRC = 0x06;                   // Enable missing clock detector and
                                       // leave VDD Monitor enabled.
      #endif
   #endif

   // If system clock is > 10 MHz, bypass (disable) the one-shot timer
   // Otherwise, system clock is <= 10 MHz, enable the one-shot timer
   #if(SYSCLK > 10000000)              
      FLSCL |= 0x40;                   // Set the BYPASS bit
   #else
      FLSCL &= ~0x40;                  // Clear the BYPASS bit
      FLWR  =   0x01;                  // to enable the one-shot
   #endif
   

   #if  ((CLOCK_SELECTION & 0x0F) == PRECISION_OSC)
      OSCICN |= 0x80;                  // Enable the precision internal osc.
  
      PMU0CF = 0x20;                   // Clear all wake-up source flags
                                       // This allows low power oscillator
                                       // to be turned off
   
   #elif((CLOCK_SELECTION & 0x0F) == LOW_POWER_OSC)
      REG0CN &= ~0x10;                 // Disable precision oscillator bias
   
   #elif((CLOCK_SELECTION & 0x0F) == SMARTCLOCK_OSC)
      REG0CN &= ~0x10;                 // Disable precision oscillator bias

      PMU0CF = 0x20;                   // Clear all wake-up source flags
                                       // This allows low power oscillator
                                       // to be turned off

      RTC_Init();                      // Initialize SmaRTClock Oscillator
   
   #elif((CLOCK_SELECTION & 0x0F) == EXTERNAL_OSC)
      REG0CN &= ~0x10;                 // Disable precision oscillator bias

      PMU0CF = 0x20;                   // Clear all wake-up source flags
                                       // This allows low power oscillator
                                       // to be turned off
      
      OSCXCN = 0x20;                   // Start external oscillator 
                                       // in CMOS clock mode
   #endif
     
   
   CLKSEL = CLOCK_SELECTION;           // Set the system clock to the 
                                       // selected oscillator and 
                                       // divide value

   while((CLKSEL & 0x80) == 0);        // Wait for new divide value to be
                                       // applied 



}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
//-----------------------------------------------------------------------------
void PORT_Init (void)
{
   // Crossbar Initialization
   XBR2    = 0x40;                     // Enable Crossbar and weak pull-ups
   
   //P2MDOUT |= 0x40;                    // Drive P2.6 High

}



//-----------------------------------------------------------------------------
// RTC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the RTC. First it unlocks the RTC interface,
// enables the RTC, clears ALRMn and CAPTUREn bits. Then it sets up the RTC
// to operate using a 32.768khZ crystal. Lastly it enables the alarm and
// interrupt. This function uses the RTC primitive functions to access 
// the internal RTC registers.
//
//-----------------------------------------------------------------------------
void RTC_Init (void)
{
   uint16_t i;
   uint8_t CLKSEL_SAVE = CLKSEL;   
   

   RTC0KEY = 0xA5;                     // Unlock the smaRTClock interface
   RTC0KEY = 0xF1;
   
   // Configure the RTC to use the internal LFO
   if(USE_SMARTCLOCK_LFO && (Device_ID == C8051F912 || Device_ID == C8051F902))
   {
      RTC_Write (RTC0XCN, 0x08);          // Configure the smaRTClock
                                          // to use the LFO
   } else
   
   // Configure the RTC to use self-oscillate mode
   if(USE_SMARTCLOCK_LFO)
   {     

      // Configure smaRTClock to self-oscillate mode with bias X2 disabled
      RTC_Write (RTC0XCN, 0x00);   
                                      
      // Disable Auto Load Cap Stepping
      RTC_Write (RTC0XCF, (0x00 | 0x03));  
    
      // Short XTAL3 and XTAL4 together
      RTC_Write(RTC0PIN, 0xE7);
      

   } else
   // Configure RTC to use crystal mode
   {
      RTC_Write (RTC0XCN, 0x60);          // Configure the smaRTClock
                                          // oscillator for crystal mode
                                          // Bias Doubling Enabled
                                          // AGC Disabled
   
   
      RTC_Write (RTC0XCF, 0x83);          // Enable Automatic Load Capacitance
                                          // stepping and set the desired 
                                          // load capacitance to 4.5pF plus
                                          // the stray PCB capacitance

      RTC_Write (RTC0CN, 0x80);           // Enable smaRTClock oscillator
      

      //----------------------------------
      // Wait for smaRTClock to start
      //----------------------------------
   
      CLKSEL    =  0x74;                  // Switch to 156 kHz low power
                                          // internal oscillator
      // Wait > 20 ms

      for (i=0x150;i!=0;i--);  

      // Wait for smaRTClock valid
      while ((RTC_Read (RTC0XCN) & 0x10)== 0x00); 
   

      // Wait for Load Capacitance Ready
      while ((RTC_Read (RTC0XCF) & 0x40)== 0x00); 
   
      //----------------------------------
      // smaRTClock has been started
      //----------------------------------
 
      RTC_Write (RTC0XCN, 0xC0);          // Enable Automatic Gain Control
                                          // and disable bias doubling
   }


   RTC_Write (RTC0CN, 0xC0);           // Enable missing smaRTClock detector
                                       // and leave smaRTClock oscillator
                                       // enabled. 
   // Wait > 2 ms
   for (i=0x40;i!=0;i--);  
   
   PMU0CF = 0x20;                      // Clear PMU wake-up source flags

   CLKSEL = CLKSEL_SAVE;               // Restore system clock
   while(!(CLKSEL & 0x80));            // Poll CLKRDY
  
}



//-----------------------------------------------------------------------------
// Support Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// RTC_Read ()
//-----------------------------------------------------------------------------
//
// Return Value : RTC0DAT
// Parameters   : 1) uint8_t reg - address of RTC register to read
//
// This function will read one byte from the specified RTC register.
// Using a register number greater that 0x0F is not permited.
//
//-----------------------------------------------------------------------------

uint8_t RTC_Read (uint8_t reg)
{
   reg &= 0x0F;                        // mask low nibble
   RTC0ADR  = reg;                     // pick register
   RTC0ADR |= 0x80;                    // set BUSY bit to read
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
   return RTC0DAT;                     // return value
}

//-----------------------------------------------------------------------------
// RTC_Write ()
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint8_t reg - address of RTC register to write
//                2) uint8_t value - the value to write to <reg>
//
// This function will Write one byte to the specified RTC register.
// Using a register number greater that 0x0F is not permited.
//-----------------------------------------------------------------------------
void RTC_Write (uint8_t reg, uint8_t value)
{
   reg &= 0x0F;                        // mask low nibble
   RTC0ADR  = reg;                     // pick register
   RTC0DAT = value;                    // write data
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
