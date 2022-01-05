//-----------------------------------------------------------------------------
// F912DC_Data_Logger.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program implements a ToolStick based data logger on the 'F912DC.
// The device will measure the ambient temperature (using the integrated
// temperature sensor) as well as the battery voltage every hour and store
// that information in flash. This information can then be retrieved by
// observing the flash contents starting at <LOG_START_ADDR>, or by
// using ToolStick terminal to interface with the device.
//
//
// ToolStick Terminal is required for this example and can be found at:
// http://www.silabs.com/products/mcu/Pages/8-bit-microcontroller-software.aspx#toolstick
//
//
// How To Test:
//
// 1) Connect the ToolStick912DC to a ToolStick Base Adapter.
// 2) Compile and download code to the ToolStick Daughter Card by selecting
//    Run -> Debug from the menus, clicking the Debug button in the quick menu,
//    or pressing F11.
// 3) Disconnect the device from Simplicity Studio by clicking the "Disconnect"
//    button in the quick menu, or selecting Run -> Disconnect from the menus.
// 4) Open ToolStick Terminal and connect to the device by pressing "Connect".
// 5) Follow the terminal prompts in ToolStick Terminal.
//
// Resources Used:
// Timer 0: Wait_MS() or screen refresh interval in Interactive Mode
// Timer 1: UART
// Timer 2: TouchSense
// Timer 3: Blinking LED
//  
//
// Target:         ToolStick912DC
// Tool chain:     Simplicity Studio / Keil C51 9.51
//
// Release 1.2 (BL)
//    - Updated Description / How to Test
//    - 21 MAR 2014
//
// Release 1.1
//    -Initial Revision (JH)
//    -21 JULY 2009
//
// Release 1.0
//    -Initial Revision (FB)
//    -5 OCT 2007
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#define DEBUG_MODE 0

#include "SI_C8051F912_Register_Enums.h"
#include "stdio.h"

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(RED_LED,      SFR_P1, 5);        // '0' means ON, '1' means OFF
SI_SBIT(YELLOW_LED,   SFR_P1, 6);        // '0' means ON, '1' means OFF
SI_SBIT(SW2,          SFR_P0, 2);        // SW2 == 0 means switch pressed
SI_SBIT(POWER_SOURCE, SFR_P1, 3);        // '0' means TSPWR, '1' means BATPWR
SI_SBIT(POT_ENABLE,   SFR_P1, 4);        // Open-Drain output. Write '0' to 
                                       // enable the potentiometer.

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK          24500000L      // SYSCLK frequency in Hz
#define RTCCLK           32768         // smaRTClock frequency in Hz
#define BAUDRATE         115200        // Baud rate of UART in bps
#define T0_SW_DIV         2048L        // Timer0 Software Divide Value
#define T3_SW_DIV          10L         // Timer3 Software Divide Value

#define LED_BLINK_RATE      10L        // LED blink rate in Hz 
#define DEBOUNCE_INTERVAL    5         // Debounce interval in milliseconds
#define UART_REFRESH_RATE   10L        // Screen Refresh Rate in Hz 

#define WAKE_INTERVAL_LONG  3600L      // Wakeup-interval in seconds
                                       // Typically set to 1 hour
#define WAKE_INTERVAL_SHORT  1L        // Wakeup-interval used in interactive mode
                                       // Typically set to 1 second 

#define VREF              (1650L)      // Voltage reference in millivolts
                                                                          
#define LED_ON              0          // Macros to turn LED on and off
#define LED_OFF             1

#define SWITCH_PRESSED      0          // Macros to determine switch state
#define SWITCH_NOT_PRESSED  1    

#define TSPWR               0          // Macros to determine power source
#define BATPWR              1

#define LOGONLY             0          // Macros to determine the system state
#define LOGUART             1          // See the <System_State> global variable
#define INTERACTIVE         2          
#define UNDEFINED          255

//-------------------------------------
// Calculated Constants
//-------------------------------------
#define T3_DEBOUNCE_TICKS   DEBOUNCE_INTERVAL*(SYSCLK/12/1000)

// Calculate the alarm intervals in smaRTClock ticks
//                                       
//  interval [ticks] = interval [sec] x RTCCLK [ticks/sec]
//
#define WAKE_INTERVAL_LONG_TICKS (WAKE_INTERVAL_LONG * RTCCLK)
#define WAKE_INTERVAL_SHORT_TICKS (WAKE_INTERVAL_SHORT * RTCCLK)

//-------------------------------------
// Data Logger Constants
//-------------------------------------
#define SCRATCHPAD          1          // Passed to the Flash access routines
                                       // to indicate that the Flash operation
                                       // should target the scratchpad
               
#define PROGRAMAREA         0          // Passed to the Flash access routines
                                       // to indicate that the Flash operation
                                       // should target the program area

#define LOG_START_ADDR     0x1800      // First byte of the Flash memory
                                       // allocated for logging

#define LOG_END_ADDR       0x39FF      // Last byte of the Flash memory 
                                       // allocated for logging
                                       
#define LOG_ENTRY_SIZE       4         // Number of bytes in each log entry

#define FLASH_PAGE_SIZE     512       // Number of bytes in each Flash page

typedef struct LOGSTAT_T               // Structure representing the contents
{                                      // of the scratchpad area
   uint8_t max_voltage;
   uint8_t min_voltage;
   int8_t max_temperature;
   int8_t min_temperature;
   uint8_t voltage_at_mfg;

} LOGSTAT;

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// The state of the system. Can be one of the following:
// 
//    LOGONLY - Entered after each POR. This is the lowest power mode where only
//              the logging function is enabled. If the switch is pressed, the 
//              Yellow LED will blink.
//
//    LOGUART - Entered after a pin reset when powered from the battery. This is
//              a low power mode, with some UART interaction. The primary purpose
//              of the UART is to alert the user that they need to switch over 
//              to ToolStick power. If the switch is pressed, the Red LED will 
//              blink.
//
//    INTERACTIVE - Entered after a pin reset when powered from ToolStick power.
//              This is a high power mode in which the MCU does not "Sleep", 
//              but rather continues to run an interactive demonstration using
//              ToolStick terminal as the primary interface. In this mode, 
//              both Yellow and Red LEDs will blink when the switch is pressed.
//
uint8_t System_State;

// Address of the last "logged" entry. Must be initialized to 0x0000 and low
// byte written first to allow software to detect an invalid address caused
// by a power-on reset quickly followed by a pin reset.
SI_UU16_t Current_Address = { 0x0000 };

// Indicates that a int32_t alarm (typically hourly) is pending
bit Long_Alarm_Pending = 0;

// The state of the LED(s). A value of '1' indicates LED(s) are blinking. A
// value of '0' indicates LED(s) are not blinking.
bit LED_Blinking = 0;
                      
// Flag to indicate that it is time to refresh the screen when connected to 
// a UART terminal in interactive mode. This bit is set by the Timer0 ISR.
bit Screen_Refresh = 1;

// Flag used in interactive mode to switch between "Summary View" and 
// "Interactive Content" view.
bit Display_Interactive_Content = 0;

// Software flags to indicate smaRTClock events are pending
bit Alarm_Pending = 0;
bit smaRTClock_Fail = 0;

//-------------------------------------
// Logged Variables  
//-------------------------------------

// Indicates the current time in units of Data Log Entries. For example, if
// a data sample is logged every hour, then <Current_Time> contains the number
// of hours since the the device was manufactured. This variable overflows
// after 65536 Data Log Entries (or every 7.4 years).
SI_UU16_t Current_Time = { 0x0000 }; 

// The current "VBAT" battery voltage in hundredth of a volt
// A value of 0 indicates TS_PWR.
// A value of 125 indicates 1.25 volts.
// A value of 254 indicates 2.54 volts.
// A value of 255 is un-initialized.
uint8_t Current_Voltage;

// The current temperature in Degrees C
int8_t Current_Temperature;

//-------------------------------------
// Interactive Mode Variables  
//-------------------------------------

// Timer2 count of SW03 relaxation oscillator period
SI_UU16_t SW03_Timer_Count;                     

// The current potentiometer value in ADC codes
// A value of 0 indicates potentiometer wiper is at GND.
// A value of 255 indicates potentiometer wiper is at VDD/DC+.
uint8_t Potentiometer_Value;

// The number of seconds remaining until the next log is taken.
SI_UU16_t Seconds_Remaining;

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
SI_INTERRUPT_PROTO(RTC0_OSC_FAIL_ISR, RTC0FAIL_IRQn);
SI_INTERRUPT_PROTO(RTC0ALARM_ISR, RTC0ALARM_IRQn);
SI_INTERRUPT_PROTO(ADC0_ISR, ADC0EOC_IRQn);
SI_INTERRUPT_PROTO(Timer3_ISR, TIMER3_IRQn);
SI_INTERRUPT_PROTO(Timer0_ISR, TIMER0_IRQn);

// Initialization Routines
void PORT_Init (void);
void SYSCLK_Init (void);
void RTC_Init (void);
void ADC0_Init (void);
void TouchSense_Init (void);
void Timer0_Init (int16_t counts);
void Timer3_Init (int16_t counts);
void UART0_Init (void);

// Interrupt Service Routines

// Application Level Support Routines
void Update_Voltage(void);
void Update_Temperature(void);
void TouchSense_Update(void);
void Potentiometer_Update(void);
void Seconds_Update (void);

// Data Log Access Routines
void Find_Last_Record(void);
void Add_New_Record(void);
void Update_Scratchpad (void);

// Low Level Support Routines
void Wait_MS(uint16_t ms);
uint8_t RTC_Read (uint8_t);
void RTC_Write (uint8_t, uint8_t);
void RTC_SetAlarmInterval (uint32_t interval);

// FLASH read/write/erase routines
void FLASH_ByteWrite (uint16_t addr, uint8_t byte, uint8_t SFLE);
uint8_t   FLASH_ByteRead  (uint16_t addr, uint8_t SFLE);
void FLASH_PageErase (uint16_t addr, uint8_t SFLE);

// Terminal Display Routines
void Print_Summary(void);
void Print_Interactive(void);
void Print_Log (void);
void Print_Entry (uint16_t addr);

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
   static bit wakeup_pending;

   uint8_t wakeup_source;                   // Variable used for decoding the 
                                       // wake-up source
   
                                       // enable)
                                       
   //DC0CF = 0x04;                       // Set Peak Current Limit to 500mA
   DC0CN = 0x03;                       // Set Supply Voltage to 2.1 Volts   

   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     // Initialize Oscillator
   RTC_Init ();                        // Initialize Real Time Clock
   ADC0_Init ();                       // Initialize ADC0

   // Initalize the Timer3 reload value to overflow at 2x the blink rate
   Timer3_Init (SYSCLK/12/T3_SW_DIV/LED_BLINK_RATE/2);  
   
   PMU0CF = 0x20;
 
   //----------------------------------
   // Initialize the system state
   //----------------------------------
   
   System_State = UNDEFINED;
   
   //----------------------------------
   // Enable Interrupts 
   //----------------------------------
   IE_EA = 1;

   //----------------------------------
   // Search the log 
   //----------------------------------
   
   // Find the last record and update the <Current_Address> to point
   // to the start of the last valid entry. <Current_Address> will
   // be initialized to 0x0000 if the log is empty.
   Find_Last_Record();
   
   // If Log is empty
   if(Current_Address.u16 == 0x0000)
   {
      // Add the first entry and initialize the scratchpad.
      Add_New_Record();
      Update_Scratchpad();
   }

   //----------------------------------
   // Main Application Loop 
   //----------------------------------
   while (1)
   {
      //--------------------------------
      // Task #1
      // Check if the power source has
      // changed, update <System_State>
      //--------------------------------  
      
      if((POWER_SOURCE == TSPWR) && (System_State != INTERACTIVE))
      {
         
         System_State = INTERACTIVE;   // Set the system state to INTERACTIVE

         // Insert a delay
         Wait_MS(5);                   // If device enters this mode, allow the
                                       // power supply to settle down before 
                                       // attempting to send any UART. 
         
         // Adjust Alarm Interval for interactive mode
         RTC_SetAlarmInterval(WAKE_INTERVAL_SHORT_TICKS);

         // Set the number of seconds remaining
         Seconds_Remaining.u16 = WAKE_INTERVAL_LONG;
         
         // Initalize TouchSense. This turns on the relaxation oscillator 
         // and configures Timer2 into capture mode 
         TouchSense_Init();            
         
         // Initialize UART0 for printf()
         UART0_Init();      

         // Initalize the Timer0 to set the screen refresh rate
         Timer0_Init (SYSCLK/48/T0_SW_DIV/UART_REFRESH_RATE); 
         
         // Switch to "Summary View"
         Display_Interactive_Content = 0;

         // Display a "log summary"
         printf("\f\f");      // Clear Screen
         Print_Summary();            
      
      } else 
      
      if((POWER_SOURCE == BATPWR) && ((RSTSRC & 0x03) == 0x01) && (System_State != LOGUART))
      {
         System_State = LOGUART;          // Set the system state to LOGUART
         
         // Adjust Alarm Interval for non-interactive mode
         RTC_SetAlarmInterval(WAKE_INTERVAL_LONG_TICKS);

         // Initialize UART0 for printf()
         UART0_Init(); 
         
         // Disable all un-necessary peripherals
         TCON_TR0 = 0;
         TMR2CN = 0x00;
         CPT0CN = 0x00;
         
         // Switch to "Summary View"
         Display_Interactive_Content = 0;

         // Display a "log summary"
         printf("\f\f");                     // Clear Screen
         Print_Summary();    
            
      } else

      if((POWER_SOURCE == BATPWR) && ((RSTSRC & 0x03) != 0x01) &&(System_State != LOGONLY))
      {
         System_State = LOGONLY;          // Set the system state to LOGONLY

         // Adjust Alarm Interval for non-interactive mode
         RTC_SetAlarmInterval(WAKE_INTERVAL_LONG_TICKS);

         // Print out a message in case user is still connected to Toolstick Terminal
         UART0_Init();
         printf("\f\f");                     // Clear Screen
         printf("A power on reset has occured and the device is now in Logging Only Mode.\n\n");
         printf("Please disconnect, then re-connect (or switch power source to TS PWR) to \n");
         printf("view a summary of the log contents.\n");

         #if(DEBUG_MODE == 0)
         // Disable all un-necessary peripherals
         TCON_TR0 = 0;
         TCON_TR1 = 0;
         TMR2CN = 0x00;
         CPT0CN = 0x00;
         #endif
      } 
      
      //--------------------------------
      // Task #2
      // Handle Switch Press/Release
      //--------------------------------
      
      // Check if switch is pressed and LED is not blinking
      if( (SW2 == SWITCH_PRESSED) && !LED_Blinking)
      {
         
         // Debounce the Switch
         TMR3 = 0;                     // Initialize T3 to zero
         TMR3CN |= 0x04;               // Start Timer (TR3 = 1)
         while(TMR3 < T3_DEBOUNCE_TICKS);
         TMR3CN &= ~0x04;              // Stop Timer (TR3 = 0)  

         // Check if the switch is still pressed
         if(SW2 == SWITCH_PRESSED)
         {
            // A valid switch press has been detected
            // Enable Timer 3 and Timer 3 interrupts to allow the 
            // proper LED(s) to blink. See the Timer3 ISR for details        
            TMR3 = TMR3RL;                // Initialize T3 to reload value
            TMR3CN |= 0x04;               // Start Timer (TR3 = 1)
            EIE1 |= 0x80;                 // Enable T3 Interrupts
            LED_Blinking = 1;             // Set status flag
 
            // Toggle the <Display_Interactive_Content> flag
            Display_Interactive_Content = !Display_Interactive_Content;
            
            // Check if a summary should be printed
            if((System_State == LOGUART) || 
               (System_State == INTERACTIVE && !Display_Interactive_Content))
            {
               // Display a "log summary"
               Print_Summary();    
            }

         } else
         {
            // Switch is bouncing, take no action
         }        

      } 
      
      // Check for switch not pressed and LED blinking
      if( (SW2 == SWITCH_NOT_PRESSED) && LED_Blinking)
      {
         // Disable Timer 3 and Timer 3 interrupts to prevent the 
         // LED(s) from blinking upon wake-up. 
         TMR3CN &= ~0x04;              // Stop Timer (TR3 = 0)
         EIE1 &= ~0x80;                // Disable T3 Interrupts
         LED_Blinking = 0;             // Clear status flag

         RED_LED = LED_OFF;            // Turn off the LEDs
         YELLOW_LED = LED_OFF;
         
      } 

      //--------------------------------
      // Task #3
      // Check for a clock failure
      //--------------------------------
      if(PMU0CF & 0x08)
      {
         smaRTClock_Fail = 1;
        
      } 
      
      if(smaRTClock_Fail)
      {
         // Re-Initialize the smaRTClock
         RTC_Init (); 
         
         System_State = UNDEFINED;

         smaRTClock_Fail = 0;

         // Clear the PMU wake-up flags
         // Interrupts for all enabled sources that are not persistent
         // should be enabled when clearing the wake-up source flags.
         // This is a precautionary step in case these events occur
         // during the same clock cycle that clears the PMU0CF flags.
         EIE1 |= 0x02;                 // Enable the Alarm interrupt
         EIE2 |= 0x04;                 // Enable the Clock Fail interrupt
         PMU0CF = 0x20;
         EIE1 &= ~0x02;                // Disable the Alarm interrupt
         EIE2 &= ~0x04;                // Disable the Clock Fail interrupt   

      }

      //--------------------------------
      // Task #4
      // Check for a pending
      // smaRTClock alarm
      //--------------------------------
      
      // Read the Power Mangement Flag and update the Alarm_Pending
      // software flag
      if(PMU0CF & 0x04)      
      {
         Alarm_Pending = 1;            // Indicate that an alarm is pending
      }

      if( Alarm_Pending )
      {
         // Clear the software flag
         Alarm_Pending = 0;
         
         // Clear the ALRM flag
         RTC_Write(RTC0CN, 0xD4);      // Disable Alarm, clear ALRM flag
         RTC_Write(RTC0CN, 0xDC);      // Re-Enable the Alarm
         
         // Clear the PMU wake-up flags
         // Interrupts for all enabled sources that are not persistent
         // should be enabled when clearing the wake-up source flags.
         // This is a precautionary step in case these events occur
         // during the same clock cycle that clears the PMU0CF flags.
         EIE1 |= 0x02;                 // Enable the Alarm interrupt
         EIE2 |= 0x04;                 // Enable the Clock Fail interrupt
         PMU0CF = 0x20;
         EIE1 &= ~0x02;                // Disable the Alarm interrupt
         EIE2 &= ~0x04;                // Disable the Clock Fail interrupt          
         
         #if(DEBUG_MODE)
         printf("Normal Alarm\n");         
         #endif

         // Handle smaRTClock alarm based on the current System State
         if(System_State == INTERACTIVE)
         {
            Seconds_Remaining.u16--;
            
            if(Seconds_Remaining.u16 == 0)
            {
               Long_Alarm_Pending = 1;
               Seconds_Remaining.u16 = WAKE_INTERVAL_LONG;
            }
         
         } else 
         
         if( (System_State == LOGONLY) || (System_State == LOGUART) )
         {
            Long_Alarm_Pending = 1;
         }
         
      }

      //--------------------------------
      // Task #5
      // Check if a int32_t alarm is 
      // pending 
      //--------------------------------
            
      if(Long_Alarm_Pending)
      {      
         Long_Alarm_Pending = 0;
         
         // Update global variables and write to Flash
         Add_New_Record();             // Add a new log entry
         
         // Update the statistics stored in Flash
         Update_Scratchpad();
		   
         // Switch to "Summary View"
         Display_Interactive_Content = 0;

         // Display a "log summary"
         if((System_State == INTERACTIVE) || (System_State == LOGUART))
         {
            Print_Summary();
         }
      }

      //--------------------------------
      // Task #6
      // Check if the screen needs
      // to be periodically updated
      //--------------------------------
      
      if( (System_State == INTERACTIVE) &&
           Screen_Refresh && Display_Interactive_Content
        )
      {     
           Screen_Refresh = 0;        // Clear Flag
           Print_Interactive();       // Interactive Terminal Display
       
      }
      
      //--------------------------------
      // Task #7
      // Check if the log should be 
      // printed
      //--------------------------------
      
      if( SCON0_RI )
      {
         if((SBUF0 == 'p') || (SBUF0 == 'P'))
         {
            Print_Log();
         }

         SCON0_RI = 0;

      }

      //--------------------------------
      // Task #8
      // Place the device in Sleep Mode
      //--------------------------------
      
      // Place the device in Sleep Mode if the following condtions
      // are met:
      //    A. There is no pending smaRTClock alarm
      //    B. There is no pending Clock Failure
      //    B. The LED is not currently blinking (SW2 is not pressed)
      //    C. We are powered from the battery
      //    D. The system state is not INTERACTIVE
      //    E. The system state is not UNDEFINED
      //    
      if( (!Alarm_Pending) && (!smaRTClock_Fail) && (!LED_Blinking) && 
          (POWER_SOURCE == BATPWR) && (System_State != INTERACTIVE) &&
          (System_State != UNDEFINED))
      {
         
         wakeup_pending = 0;           // Clear software flag

         // Clear the PMU wake-up flags
         // Interrupts for all enabled sources that are not persistent
         // should be enabled when clearing the wake-up source flags.
         // This is a precautionary step in case these events occur
         // during the same clock cycle that clears the PMU0CF flags.
         EIE1 |= 0x02;                 // Enable the Alarm interrupt
         EIE2 |= 0x04;                 // Enable the Clock Fail interrupt
         PMU0CF = 0x20;
         EIE1 &= ~0x02;                // Disable the Alarm interrupt
         EIE2 &= ~0x04;                // Disable the Clock Fail interrupt     
         
         // Check for non-persistent wake-up sources
         if((Alarm_Pending) || (smaRTClock_Fail) || (POWER_SOURCE != BATPWR) ||
             (SW2 != SWITCH_NOT_PRESSED))
         {
            
            wakeup_pending = 1;        // Set software flag
         
         }        

         // Configure the Port I/O for Sleep Mode 
         // No configuration necessary

         // Configure clock to be used during wake-up
         CLKSEL = 0x04;             // Select Low Power Oscillator
         
         // Enable wake-up sources and place the device in Sleep Mode
         // Wake-Up sources enabled are:
         // A. smaRTClock Alarm
         // B. smaRTClock Oscillator Fail
         // C. Port Match (wake-up on switch press)
         // D. Pulse on Reset Pin (always enabled)
         if(!wakeup_pending)
         {
            PMU0CF = 0x8E;
         }
            
         //--------------------------------------------------------------------
         // ~~~~~~~~~~~~~~~~~~~~ Device Sleeping ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         //--------------------------------------------------------------------
         
         // Configure clock to be used after waking up
         CLKSEL = 0x00;             // Select Precision Oscillator

         // Read the wake-up source flags 
         wakeup_source = PMU0CF & 0x1F;   
      
         // Check for a reset pin wake-up
         if(wakeup_source & 0x10)
         {
            // Take no action. Device will loop once and go back to sleep.      
         }         

         // Check for smaRTClock Oscillator failure
         if(wakeup_source & 0x08)
         {
            // Take no action. Clock Failure handled by main application loop.            
         }    
      
         // Check for smaRTClock alarm
         if(wakeup_source & 0x04)
         {
            // Take no action. Alarm will be handled by main application loop.
         }

         // Check for Port Match event
         if(wakeup_source & 0x02)
         {
            // Take no action. Switch press is handled by main application loop.
         }

      } 
      
      #if(DEBUG_MODE == 1)
      else
      {  
         printf("Unable to enter Sleep\n");
      }
      #endif

   }
}

//-----------------------------------------------------------------------------
// Terminal Display Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Print_Summary
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Prints a summary to the Terminal
//
//
//-----------------------------------------------------------------------------
void Print_Summary (void)
{  
   LOGSTAT log_stat;
   
   uint8_t voltage_at_mfg_integer;
   uint8_t voltage_at_mfg_fraction;
   uint8_t max_voltage_integer;
   uint8_t max_voltage_fraction;
   uint8_t min_voltage_integer;
   uint8_t min_voltage_fraction;
   uint8_t current_voltage_integer;
   uint8_t current_voltage_fraction;

   //----------------------------------
   // Make a copy of the scratchpad
   //----------------------------------
   
   log_stat.max_voltage = FLASH_ByteRead(0x00, SCRATCHPAD);
   log_stat.min_voltage = FLASH_ByteRead(0x01, SCRATCHPAD);     
   log_stat.max_temperature = FLASH_ByteRead(0x02, SCRATCHPAD);
   log_stat.min_temperature = FLASH_ByteRead(0x03, SCRATCHPAD);     
   log_stat.voltage_at_mfg = FLASH_ByteRead(0x04, SCRATCHPAD);     
   
   //----------------------------------
   // Update the current variables
   //----------------------------------
   Update_Voltage();    
   Update_Temperature();  

   //----------------------------------
   // Separate Voltages into fraction
   // and integer components
   //----------------------------------
   
   voltage_at_mfg_integer = log_stat.voltage_at_mfg / 100;
   voltage_at_mfg_fraction = log_stat.voltage_at_mfg - (voltage_at_mfg_integer * 100);

   max_voltage_integer = log_stat.max_voltage / 100;
   max_voltage_fraction = log_stat.max_voltage - (max_voltage_integer * 100);

   min_voltage_integer = log_stat.min_voltage / 100;
   min_voltage_fraction = log_stat.min_voltage - (min_voltage_integer * 100);

   current_voltage_integer = Current_Voltage / 100;
   current_voltage_fraction = Current_Voltage - (current_voltage_integer * 100);

   //----------------------------------
   // Clear Screen and Print Summary
   //----------------------------------

   printf("\f");                       // Clear Screen
  
   printf("\n    *** Log Summary ***\n\n");   

   printf("Running Time: %u hours\n\n", 
                              (uint16_t) Current_Time.u16);

   printf("Initial Battery Voltage: %u.%u Volts\n", 
                              (uint16_t) voltage_at_mfg_integer,
                              (uint16_t) voltage_at_mfg_fraction
         );

   if(POWER_SOURCE == BATPWR)
   {
      printf("Current Battery Voltage: %u.%u Volts\n", 
                                 (uint16_t) current_voltage_integer,
                                 (uint16_t) current_voltage_fraction
            );
   
   }

   printf("Max/Min Battery Voltage: %u.%u / %u.%u Volts\n\n",
                              (uint16_t) max_voltage_integer,
                              (uint16_t) max_voltage_fraction,
                              (uint16_t) min_voltage_integer,
                              (uint16_t) min_voltage_fraction
         );

   printf("Current Temperature: %d degrees C\n", 
                              (uint16_t) Current_Temperature                              
         );

   printf("Max/Min Temperature: %d / %d degrees C\n\n", 
                              (uint16_t) log_stat.max_temperature,
                              (uint16_t) log_stat.min_temperature
         );

   printf("    *** End Summary ***\n\n"); 

   //----------------------------------
   // Print Current Voltage and Temp
   //----------------------------------
   
   // Print Battery Voltage and "Reminder" to switch to ToolStick power
   if(POWER_SOURCE == BATPWR)
   {
      
      printf("Please switch to ToolStick Power to preserve battery life.\n\n");

   }  else 

   {
      printf("Powered from the ToolStick Base Adapter!\n\n");
   }

   // Print a reminder that the user can press "P" to print the log
   if(System_State == INTERACTIVE)
   {
      printf("Transfer 'P' to print the entire log.\n");
      printf("Press the P0.2 switch to toggle interactive display.\n");
   }

}

//-----------------------------------------------------------------------------
// Print_Interactive
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Prints interactive content to the Terminal
//
//
//-----------------------------------------------------------------------------
void Print_Interactive (void)
{  
 
   //----------------------------------
   // Update Interactive Variables
   //----------------------------------
   TouchSense_Update();
   Potentiometer_Update();
   Update_Temperature();

   //----------------------------------
   // Clear Screen and Print 
   // Interactive Content Menu
   //----------------------------------
   
   printf("\f");                       // Clear Screen

   printf("  Time Remaining   Potentiometer   TouchSense      Current    \n");
   printf("  to Next Update       Ratio         Period      Temperature  \n");
   printf("    [Seconds]        [Percent]      [SYSCLK]       [deg C]    \n");

   //----------------------------------
   // Print Interactive Variables
   //----------------------------------

   printf("%9i%17i%15i%14i\n",(uint16_t) Seconds_Remaining.u16,
                              (uint16_t) ((((uint16_t) Potentiometer_Value) * 100) / 255),
                              (uint16_t) SW03_Timer_Count.u16, 
                              (uint16_t) Current_Temperature
         );  
   
}

//-----------------------------------------------------------------------------
// Print_Log
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Prints the Log to the Terminal
//
//
//-----------------------------------------------------------------------------
void Print_Log (void)
{  
 
   SI_UU16_t address;
   SI_UU16_t value;
   static bit continue_printing;

   continue_printing = 0;

   printf("\n\nNote: Voltage equals 0.0 indicates data point taken while\n");
   printf("connected to ToolStick Power\n\n\n");

   printf("Time [Hours], Supply Voltage [Volts], Temperature [Degrees C]\n"); 

   //----------------------------------
   // Print from current address to 
   // the start of the log
   //----------------------------------
   for(address.u16 = Current_Address.u16;  address.u16 > LOG_START_ADDR; 
                                           address.u16 -= LOG_ENTRY_SIZE)
   {
      Print_Entry(address.u16);
   }

   //----------------------------------
   // Check end of log
   // Read 2 entries to ensure that 
   // the timestamp has not rolled over 
   //----------------------------------
   
   // Read timestamp of next to last record
   value.u8[MSB] = FLASH_ByteRead(LOG_END_ADDR-LOG_ENTRY_SIZE-3, PROGRAMAREA);
   value.u8[LSB] = FLASH_ByteRead(LOG_END_ADDR-LOG_ENTRY_SIZE-2, PROGRAMAREA);
   if(value.u16 != 0xFFFF)
   {
      continue_printing = 1;
   }

   // Read timestamp of last record
   value.u8[MSB] = FLASH_ByteRead(LOG_END_ADDR-3, PROGRAMAREA);
   value.u8[LSB] = FLASH_ByteRead(LOG_END_ADDR-2, PROGRAMAREA);
   if(value.u16 != 0xFFFF)
   {
      continue_printing = 1;
   }

   //----------------------------------
   // Print the rest of the log
   //----------------------------------
   if(continue_printing)
   {  
      for(address.u16 = LOG_END_ADDR-3;  address.u16 > Current_Address.u16; 
                                         address.u16 -= LOG_ENTRY_SIZE)
      {
         Print_Entry(address.u16);
      }
   } 
}

//-----------------------------------------------------------------------------
// Print_Entry
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Prints the Log to the Terminal
//
//
//-----------------------------------------------------------------------------
void Print_Entry (uint16_t addr)
{  
   SI_UU16_t address;
   SI_UU16_t value;

   uint8_t voltage_integer;
   uint8_t voltage_fraction;   
   
   // Initialize Address
   address.u16 = addr;

   // Read the time
   value.u8[MSB] = FLASH_ByteRead(address.u16, PROGRAMAREA);
   value.u8[LSB] = FLASH_ByteRead(address.u16+1, PROGRAMAREA);
   
   if(value.u16 != 0xFFFF)
   { 
      // Print the time
      printf("%u, ", (uint16_t) value.u16);  

      // Print the voltage
   
      value.u8[LSB] = FLASH_ByteRead(address.u16+2, PROGRAMAREA);
      voltage_integer = value.u8[LSB] / 100;
      voltage_fraction = value.u8[LSB] - (voltage_integer * 100);

      printf("%u.%u, ", (uint16_t) voltage_integer,
             (uint16_t) voltage_fraction);  

      // Print the Temperature
   
      value.u8[LSB] = FLASH_ByteRead(address.u16+3, PROGRAMAREA);
      printf("%u\n", (uint16_t) value.u8[LSB]);  
   }

}

//-----------------------------------------------------------------------------
// Application Level Support Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Update_Voltage
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Takes an ADC measurement and updates the <Current_Voltage> variable.
//
//
//-----------------------------------------------------------------------------
void Update_Voltage (void)
{  
   SI_UU32_t sample;   
   
   if(POWER_SOURCE == TSPWR)
   {
      Current_Voltage = 0x00;
      return;
   }

   // Configure the ADC input MUX
   ADC0MX = 0x1C;                      // Select VBAT as the ADC input

   // Start a Burst Mode conversion. ADC tracking is automatically handled
   // by the ADC when it is in Burst Mode. The default values can be 
   // adjusted by modifying the ADC0PWR and ADC0TK registers. For most 
   // applications, they may be left at their default values.
   ADC0CN_ADINT = 0;                         // clear ADC0 conv. complete flag
   EIE1 |= 0x08;                       // enable ADC0 conversion complete int.
   ADC0CN_ADBUSY = 1;                        // initiate conversion
   
   // Wait for conversion to complete
   while(!ADC0CN_ADINT)
   {
      PCON = 0x01;                     // Place device in Idle Mode
   }
   
   // Convert the ADC code to "hundredth" of a volt using the following
   // converstion equation:
   // 
   //    Voltage = (ADC_CODE / 1024) x VREF
   // 
   //    Note: The Voltage takes the same unit as VREF.
   //
   sample.u32 = ADC0;                  // Copy the 10-bit ADC Result
   sample.u32 *= (VREF/10);            // Perform multiplication by
                                       // VREF first. Take VREF in 
                                       // units of "hundredth" of a volt

   // Shift right by 10 (divide by 1024)
   sample.u32 >>= 10;

   // Update the <Current_Voltage> variable
   Current_Voltage = sample.u8[B0];    // Copy the LSB
   
}

//-----------------------------------------------------------------------------
// Update_Temperature
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Takes an ADC measurement and updates the <Current_Temperature> variable.
//
//
//-----------------------------------------------------------------------------
void Update_Temperature (void)
{  
   SI_UU32_t sample;
   uint8_t ADC0AC_SAVE = ADC0AC;

   // Configure the ADC input MUX
   ADC0MX = 0x1B;                      // Select Temp Sensor as the ADC input
   
   // Configure the ADC to take 8 samples and shift right by 3
   ADC0AC = 0x1A;

   // Start a Burst Mode conversion. ADC tracking is automatically handled
   // by the ADC when it is in Burst Mode. The default values can be 
   // adjusted by modifying the ADC0PWR and ADC0TK registers. For most 
   // applications, they may be left at their default values.
   ADC0CN_ADINT = 0;                         // clear ADC0 conv. complete flag
   EIE1 |= 0x08;                       // enable ADC0 conversion complete int.
   ADC0CN_ADBUSY = 1;                        // initiate conversion
   
   // Wait for conversion to complete
   while(!ADC0CN_ADINT)
   {
      PCON = 0x01;                     // Place device in Idle Mode
   }
   
   // Convert the ADC code to "degrees" using the following
   // converstion equation:
   // 
   //    Voltage [mV] = (ADC_CODE / 1024) x VREF [mV]
   // 
   //    Temperature [ºC] = (Voltage [mV] * 0.248 [ºC/mV]) - 231 [ºC]
   // 
   // Combining the two equations and assuming all units are in  mV or ºC,
   // we have:
   // 
   //    Temperature = ADC_CODE * VREF * 0.248 
   //                  -----------------------   -  231
   //                          1024
   // 
   // Making this equation "fixed-point" friendly, we get:
   //
   //                             VREF * 64 * [(0.248*1000)/1000] 
   //    Temperature = ADC_CODE x --------------------------------  -  231
   //                                    1024 * 64 (= 2^16)  
   //
   sample.u32 = ADC0;                  // Copy the 10-bit ADC Result
   sample.u32 *= ((VREF*64*248)/1000); // Perform the required multiplication
   sample.s16[MSB] -= 231;             // Subtract the offset. 
                                       // Note: An implicit divide by 2^16 
                                       // is performed by truncating (ignoring)
                                       // the least significant 16 bits.

   // Restore ADC0AC
   ADC0AC = ADC0AC_SAVE;   

   // Update the <Current_Temperature> variable
   Current_Temperature = sample.s8[B2];// Copy the LSB of the upper 16-bits.
}

//-----------------------------------------------------------------------------
// Potentiometer_Update
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Takes an ADC measurement and updates the <Potentiometer_Value> variable.
//
//
//-----------------------------------------------------------------------------
void Potentiometer_Update (void)
{  
   // Enable the Potentiometer
   POT_ENABLE = 0;                     // Enable the Potentiometer (active-low)
   
   // Configure the ADC input MUX
   ADC0MX = 0x06;                      // Select Potentiometer as the ADC input
   
   // Configure the Voltage Reference
   REF0CN &= ~0x10;                    // Select VDD/DC+ as the reference

   // Configure the ADC conversion mode
   ADC0AC |= 0x10;                     // Right-Shift by 2
   
   // Start a Burst Mode conversion. ADC tracking is automatically handled
   // by the ADC when it is in Burst Mode. The default values can be 
   // adjusted by modifying the ADC0PWR and ADC0TK registers. For most 
   // applications, they may be left at their default values.
   ADC0CN_ADINT = 0;                         // clear ADC0 conv. complete flag
   EIE1 |= 0x08;                       // enable ADC0 conversion complete int.
   ADC0CN_ADBUSY = 1;                        // initiate conversion
   
   // Wait for conversion to complete
   while(!ADC0CN_ADINT)
   {
      PCON = 0x01;                     // Place device in Idle Mode
   }
   
   // Update the <Potentiometer_Value> variable
    Potentiometer_Value = ADC0L;       // Copy the conversion result.
   
   // Restore the Voltage Reference
   REF0CN |= 0x10;                    // Select High-Speed Reference

   // Restore the ADC conversion mode
   ADC0AC &= ~0x10;                   // Right-Shift by 0

   // Disable the Potentiometer
   POT_ENABLE = 1;                    // Disable the Potentiometer (active-low)

}

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
// Data Log Access Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Find_Last_Record ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function finds the last written record and updates the global
// <Current_Address> and <Current_Time> variable
//
//-----------------------------------------------------------------------------
void Find_Last_Record (void)
{
   SI_UU16_t address;
   SI_UU16_t value;

   //----------------------------------
   // Find the first empty Flash page
   //----------------------------------

   for( address.u16 = LOG_START_ADDR;
        address.u16 < LOG_END_ADDR;
        address.u16 += FLASH_PAGE_SIZE
      )
   {
      value.u8[MSB] = FLASH_ByteRead(address.u16, PROGRAMAREA);
      if(value.u8[MSB] != 0xFF)  { continue; }

      value.u8[LSB] = FLASH_ByteRead(address.u16+1, PROGRAMAREA);
      if(value.u8[LSB] == 0xFF)  { break; }
   }
   
   //----------------------------------
   // Jump to the beginning of the log
   // if we are past the end
   //----------------------------------
   if(address.u16 >= LOG_END_ADDR)
   {
      address.u16 = LOG_START_ADDR;
   } 

   //----------------------------------
   // Jump to the beginning of the 
   // previous page unless we are at
   // the first page.
   //----------------------------------
   
   if(address.u16 != LOG_START_ADDR)
   {
      address.u16 -= FLASH_PAGE_SIZE;
   } 

   //----------------------------------
   // Linearly search each record to
   // determine the first empty record
   //----------------------------------
   
   for( ; address.u16 < LOG_END_ADDR; address.u16 += LOG_ENTRY_SIZE)
   {
      value.u8[MSB] = FLASH_ByteRead(address.u16, PROGRAMAREA);
      if(value.u8[MSB] != 0xFF)  { continue; }

      value.u8[LSB] = FLASH_ByteRead(address.u16+1, PROGRAMAREA);
      if(value.u8[LSB] == 0xFF)  { break; }
   }

   //----------------------------------
   // Jump to the previous record 
   // unless the log is empty 
   //----------------------------------
   if(address.u16 != LOG_START_ADDR)
   {
      address.u16 -= LOG_ENTRY_SIZE;
   } else
   
   // Determine if the first "Entry" is blank becasue the entire log is empty
   // or because we have looped around, erased the first page, and have not 
   // had a chance to write the first entry.
   {
      // Read the timestamp of the very last record
      value.u8[MSB] = FLASH_ByteRead((LOG_END_ADDR+1)-(LOG_ENTRY_SIZE)   , PROGRAMAREA);
      value.u8[LSB] = FLASH_ByteRead((LOG_END_ADDR+1)-(LOG_ENTRY_SIZE)+1 , PROGRAMAREA);
      
      // If the record is empty
      if(value.u16 == 0xFFFF)
      {
         // Initialize the current address to 0x0000 to indicate that
         // the LOG is completely empty.
         address.u16 = 0x0000;
      } else
      {
         // Initialize the current address to the address of the very last entry
         address.u16 = (LOG_END_ADDR+1)-(LOG_ENTRY_SIZE);
      }
   }

   //----------------------------------
   // Update the <Current_Address>
   //----------------------------------
   Current_Address.u16 = address.u16;
   
   //----------------------------------
   // Update the <Current_Time>
   //----------------------------------
   if(Current_Address.u16 != 0x0000)
   {
      value.u8[MSB] = FLASH_ByteRead(Current_Address.u16, PROGRAMAREA);
      value.u8[LSB] = FLASH_ByteRead(Current_Address.u16+1, PROGRAMAREA);
      
      Current_Time.u16 = value.u16;
   }  

}

//-----------------------------------------------------------------------------
// Add_New_Record ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function adds a record to the data log and increments the 
// <Current_Address> and <Current_Time> variables. 
//
//-----------------------------------------------------------------------------
void Add_New_Record (void)
{
   SI_UU32_t log_entry;

   //----------------------------------
   // Advance Time and Address
   //----------------------------------
         
   // Increment the <Current_Time>
   // If the log is empty, do not increment the time.
   if(Current_Address.u16 != 0x0000)
   {
      Current_Time.u16 += 1;
   }

   // Increment the <Current_Address>
   if(Current_Address.u16 == 0x0000)
   {
      Current_Address.u16 = LOG_START_ADDR;
   } else
   {
      Current_Address.u16 += LOG_ENTRY_SIZE;
   }
   
   // Wrap the <Current_Address>
   if(Current_Address.u16 > LOG_END_ADDR)
   {
      Current_Address.u16 = LOG_START_ADDR;
   }
   
   //----------------------------------
   // Update Voltage and Temperature
   //----------------------------------
   Update_Voltage();
   Update_Temperature();   

   //----------------------------------
   // Verify that all bytes of the 
   // current address are 0xFF
   //----------------------------------
   
   log_entry.u8[B3] = FLASH_ByteRead(Current_Address.u16, PROGRAMAREA);
   log_entry.u8[B2] = FLASH_ByteRead(Current_Address.u16+1, PROGRAMAREA);
   log_entry.u8[B1] = FLASH_ByteRead(Current_Address.u16+2, PROGRAMAREA);   
   log_entry.u8[B0] = FLASH_ByteRead(Current_Address.u16+3, PROGRAMAREA);      

   if(log_entry.u32 != 0xFFFFFFFF)
   {
      FLASH_PageErase(Current_Address.u16, PROGRAMAREA);
   }

   //----------------------------------
   // Fill in the new timestamp, 
   // voltage and temperature
   //----------------------------------
   
   log_entry.u8[B3] = Current_Time.u8[MSB];
   log_entry.u8[B2] = Current_Time.u8[LSB];
   log_entry.u8[B1] = Current_Voltage;
   log_entry.u8[B0] = Current_Temperature;

   //----------------------------------
   // Transfer the new log entry to
   // Flash memory
   //----------------------------------
   
   FLASH_ByteWrite(Current_Address.u16+0, log_entry.u8[B3], PROGRAMAREA);
   FLASH_ByteWrite(Current_Address.u16+1, log_entry.u8[B2], PROGRAMAREA);
   FLASH_ByteWrite(Current_Address.u16+2, log_entry.u8[B1], PROGRAMAREA);   
   FLASH_ByteWrite(Current_Address.u16+3, log_entry.u8[B0], PROGRAMAREA);      

}

//-----------------------------------------------------------------------------
// Update_Scratchpad ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function adds a record to the data log and increments the 
// <Current_Address> and <Current_Time> variables. 
//
//-----------------------------------------------------------------------------
void Update_Scratchpad (void)
{
   LOGSTAT log_stat;
   uint8_t update_required = 0;

   //----------------------------------
   // Make a copy of the scratchpad
   //----------------------------------
   
   log_stat.max_voltage = FLASH_ByteRead(0x00, SCRATCHPAD);
   log_stat.min_voltage = FLASH_ByteRead(0x01, SCRATCHPAD);     
   log_stat.max_temperature = FLASH_ByteRead(0x02, SCRATCHPAD);
   log_stat.min_temperature = FLASH_ByteRead(0x03, SCRATCHPAD);     
   log_stat.voltage_at_mfg = FLASH_ByteRead(0x04, SCRATCHPAD);     
   
   //----------------------------------
   // Update max and min voltage
   //----------------------------------
   
   if(Current_Voltage != 0x00)
   {
      if((log_stat.max_voltage == 0xFF) || (log_stat.max_voltage < Current_Voltage))
      {
         log_stat.max_voltage = Current_Voltage;
         update_required = 1;
      }

      if((log_stat.min_voltage == 0xFF) || (log_stat.min_voltage > Current_Voltage))
      {
         log_stat.min_voltage = Current_Voltage;
         update_required = 1;
      }
         
   }

   //----------------------------------
   // Update max and min temperature
   //----------------------------------
   
   if(Current_Temperature != 0x00)
   {
      if((log_stat.max_temperature == (int8_t)0xFF) || (log_stat.max_temperature < Current_Temperature))
      {
         log_stat.max_temperature = Current_Temperature;
         update_required = 1;
      }

      if((log_stat.min_temperature == (int8_t)0xFF) || (log_stat.min_temperature > Current_Temperature))
      {
         log_stat.min_temperature = Current_Temperature;
         update_required = 1;
      }
         
   }

   //----------------------------------
   // Update Voltage at MFG
   //----------------------------------
   if( (log_stat.voltage_at_mfg == 0xFF) && (Current_Voltage != 0x00) )
   {
      log_stat.voltage_at_mfg = Current_Voltage;
      update_required = 1;
   }

   //----------------------------------
   // Update Scratchpad 
   //----------------------------------
   
   if(update_required)
   {
      // Erase the Scratchpad
      FLASH_PageErase(0x0000, SCRATCHPAD);
      
      // Update the Scratchpad
      FLASH_ByteWrite(0x00, log_stat.max_voltage,     SCRATCHPAD);
      FLASH_ByteWrite(0x01, log_stat.min_voltage,     SCRATCHPAD);     
      FLASH_ByteWrite(0x02, log_stat.max_temperature, SCRATCHPAD);
      FLASH_ByteWrite(0x03, log_stat.min_temperature, SCRATCHPAD);     
      FLASH_ByteWrite(0x04, log_stat.voltage_at_mfg,  SCRATCHPAD);     
   }

}
//-----------------------------------------------------------------------------
// Application Level Support Routines
//-----------------------------------------------------------------------------

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
   
   CKCON &= ~0x07;                     // Timer0 uses a 1:48 prescaler
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
   RTC0ADR  = reg ;                    // pick register
   RTC0DAT = value;                    // write data
   while ((RTC0ADR & 0x80) == 0x80);   // poll on the BUSY bit
}

//-----------------------------------------------------------------------------
// Flash Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FLASH_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value :
//      uint8_t - byte read from Flash
// Parameters   :
//   1) FLADDR addr - address of the byte to read to
//                    valid range is from 0x0000 to 0xFBFF for 64K devices
//                    valid range is from 0x0000 to 0x7FFF for 32K devices
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
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of the byte to write to
//                    valid range is from 0x0000 to 0xFBFF for 64K devices
//                    valid range is from 0x0000 to 0x7FFF for 32K devices
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
// FLASH_PageErase
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of any byte in the page to erase
//                    valid range is from 0x0000 to 0xFBFF for 64K devices
//                    valid range is from 0x0000 to 0x7FFF for 32K devices
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
   P1MDIN |= 0x08;                     // P1.3 is digital
   P0MDOUT &= ~0x08;                   // P1.3 is open-drain
   P0     |= 0x08;                     // Set P1.3 latch -> '1'

   // Configure LEDs
   P1MDIN |= 0x60;                     // P1.5, P1.6 are digital
   P1MDOUT |= 0x60;                    // P1.5, P1.6 are push-pull

   // Configure UART   
   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output

   // Configure Crossbar
   XBR0    = 0x01;                     // Enable UART on P0.4(TX) and P0.5(RX)
   XBR2    = 0x40;                     // Enable crossbar and weak pull-ups

   // Configure Port Match
   P0MASK |= 0x04;                     // Allow P0.2 to generate port match
                                       // wake-up events
   P0MAT |= 0x04;                      // Wake-up event occurs on the falling
                                       // edge of P0.2
   P1MASK |= 0x08;                     // Allow P1.3 to generate port match
                                       // wake-up events
   P1MAT |= 0x08;                      // Wake-up event occurs on the falling
                                       // edge of P0.2

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
   
   //OSCICN &= ~0x01;					   // Divide the clock by 2   
   //OSCICN &= ~0x03;

   while(!(OSCICN & 0x40));            // Poll IFRDY
   
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   CLKSEL = 0x00;                      // Select precision internal osc. 
                                       // divided by 1 as the system clock
   
}

//-----------------------------------------------------------------------------
// RTC_Init ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function will initialize the smaRTClock. 
//
//-----------------------------------------------------------------------------
void RTC_Init (void)
{
   uint8_t i;
   
   uint8_t CLKSEL_SAVE = CLKSEL;  

   // If the interface is locked
   if(RTC0KEY == 0x00)
   {
      RTC0KEY = 0xA5;                  // Unlock the smaRTClock interface
      RTC0KEY = 0xF1;
   }
    
   RTC_Write (RTC0XCN, 0xC0);          // Enable Automatic Gain Control
                                       // and configure the smaRTClock
                                       // oscillator for crystal mode

   //RTC_Write (RTC0XCN, 0xE0);          // Enable Automatic Gain Control
                                       // and configure the smaRTClock
                                       // oscillator for crystal mode
                                       // Enable Bias Doubling
   
   RTC_Write (RTC0XCF, 0x83);          // Enable Automatic Load Capacitiance
                                       // stepping and set the desired 
                                       // load capacitance to 4.5pF plus
                                       // the stray PCB capacitance

   RTC_Write (RTC0CN, 0x80);           // Enable smaRTClock oscillator
      
   //----------------------------------
   // Wait for smaRTClock to start
   //----------------------------------
   
   CLKSEL    =  0x74;                  // Switch to 156 kHz low power
                                       // internal oscillator
   // Wait > 2 ms
   for (i=0x150;i!=0;i--);  

   // Wait for smaRTClock valid
   while ((RTC_Read (RTC0XCN) & 0x10)== 0x00); 
   
   // Wait for Load Capacitance Ready
   while ((RTC_Read (RTC0XCF) & 0x40)== 0x00); 
   
   //----------------------------------
   // smaRTClock has been started
   //----------------------------------
 
   RTC_Write (RTC0CN, 0xC0);           // Enable missing smaRTClock detector
                                       // and leave smaRTClock oscillator
                                       // enabled. 

   CLKSEL    =  0x74;                  // Switch to 156 kHz low power
                                       // internal oscillator
   
   // Wait > 2 ms
   for (i=0x340; i!=0; i--);

   RTC_Write (RTC0CN, 0xC0);           // Clear the clock fail flag (precautionary).
   
   PMU0CF = 0x20;                      // Clear PMU wake-up source flags

   CLKSEL = CLKSEL_SAVE;               // Restore system clock
   while(!(CLKSEL & 0x80));            // Poll CLKRDY
}

//-----------------------------------------------------------------------------
// RTC_SetAlarmInterval ()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//    uint32_t interval - the new smaRTClock alarm interval
//
// This function will set the smaRTClock alarm interval and set an 
// auto-repeating alarm. 
//
//-----------------------------------------------------------------------------
void RTC_SetAlarmInterval (uint32_t interval)
{
   
   SI_UU32_t alarm_interval;
   
   alarm_interval.u32 = interval;   

   //----------------------------------
   // Set the smaRTClock Alarm
   //----------------------------------
   
   // Stop the RTC
   RTC_Write (RTC0CN, 0xC0);     // Disable Timer
   
   // Clear the main timer
   RTC_Write (CAPTURE0, 0x00);   // Least significant byte
   RTC_Write (CAPTURE1, 0x00);
   RTC_Write (CAPTURE2, 0x00);
   RTC_Write (CAPTURE3, 0x00);   // Most significant byte
   
   RTC_Write(RTC0CN, 0xC2);            // Write '1' to RTC0SET
   while((RTC_Read(RTC0CN) & 0x02));   // Wait for RTC0SET -> 0
   
   // Copy the alarm interval to the alarm registers
   RTC_Write (ALARM0, alarm_interval.u8[B0]);   // Least significant byte
   RTC_Write (ALARM1, alarm_interval.u8[B1]);
   RTC_Write (ALARM2, alarm_interval.u8[B2]);
   RTC_Write (ALARM3, alarm_interval.u8[B3]);   // Most significant byte

   // Enable the smaRTClock timer and alarm with auto-reset
   RTC_Write (RTC0CN, 0xDC);

}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures ADC0 for Burst Mode
//
//-----------------------------------------------------------------------------
void ADC0_Init (void)
{
   ADC0CN = 0x40;                      // ADC0 disabled, Burst Mode enabled,
                                       // conversion triggered on writing '1'
                                       // to ADC0CN_ADBUSY

   REF0CN = 0x1C;                      // Select internal high speed voltage
                                       // reference and enable the temperature
                                       // sensor

   ADC0CF = ((SYSCLK/8300000))<<3;   // Set SAR clock to 8.3MHz

   ADC0CF |= 0x01;                     // Select Gain of 1
   
   ADC0AC = 0x00;                      // Right-justify results, shifted right
                                       // by 0 bits. Accumulate 1 sample for
                                       // an output word of 10-bits.

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
      CKCON |=  0x08;                  // T1M = 1; SCA1:0 = xx
   #elif (SYSCLK/BAUDRATE/2/256 < 4) 
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
      #error "Timer0 sets prescaler to divide by 48"
   #elif (SYSCLK/BAUDRATE/2/256 < 12) 
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
      #error "Timer0 sets prescaler to divide by 48"
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
   SCON0_RI = 0;                            // Indicate No Data Waiting
}

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

}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Configure Timer0 to 8-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/48 as its time base.
//
void Timer0_Init (int16_t counts)
{
   TH0 = (int8_t)(-counts);                // Init Timer0 High register
   TL0 = TH0;                          // Set the intial Timer0 value

   TMOD &= ~0x0F;                      // Clear Timer0 related bits
   TMOD |= 0x02;                       // Timer0 in 8-bit reload mode
   
   CKCON &= ~0x07;                     // Clear Prescaler and Timer0 
                                       // Clock Select Bit
   CKCON |= 0x02;                      // Set prescaler to SYSCLK/48
                                       // Note: This may affect Timer1
                                       // if it is also using the 
                                       // prescaler. See the UART0_Init()
                                       // routine for details.
   
   TCON_TF0 = 0;                            // Clear Timer0 Interrupt Flag
   IE_ET0=1;                              // Timer0 interrupt enabled
   TCON_TR0 = 1;                            // Start Timer0

}

//-----------------------------------------------------------------------------
// Timer3_Init
//-----------------------------------------------------------------------------
//
// Configure Timer3 to 16-bit auto-reload and generate an interrupt at
// interval specified by <counts> using SYSCLK/12 as its time base.
//
void Timer3_Init (int16_t counts)
{
   TMR3CN  = 0x00;                        // Stop Timer2; Clear TF2;
                                          // TMR2CN_T2XCLK is SYSCLK/12 
   CKCON  &= ~0xC0;                       // Timer2 clocked based on TMR2CN_T2XCLK;

   TMR3RL  = -counts;                     // Init reload values
   TMR3    = TMR3RL;                      // initalize timer to reload value
      
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0_ISR
//-----------------------------------------------------------------------------
//
// This routine sets the <Screen_Refresh> flag when a screen update is needed.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(Timer0_ISR, TIMER0_IRQn)
{
   static uint16_t clock_divider = T0_SW_DIV;

   // Implement Software Clock Divider
   clock_divider--;  
   if(clock_divider == 0) clock_divider = T0_SW_DIV;
   else return;
   
   // Set the <Screen_Refresh> flag
   Screen_Refresh = 1;

}

//-----------------------------------------------------------------------------
// Timer3_ISR
//-----------------------------------------------------------------------------
// This routine changes the state of the LED(s) whenever Timer3 overflows.
//
SI_INTERRUPT(Timer3_ISR, TIMER3_IRQn)
{
   static uint16_t clock_divider = T3_SW_DIV;

   // Clear Timer3 interrupt flag
   TMR3CN &= ~0x80;                    
  
   // Implement Software Clock Divider
   clock_divider--;  
   if(clock_divider == 0) clock_divider = T3_SW_DIV;
   else return;
    
   // Toggle the approprate LED(s)   
   switch(System_State)
   {
      case LOGONLY:
      YELLOW_LED = !YELLOW_LED;        // change state of Yellow LED
      break;

      case LOGUART:
      RED_LED = !RED_LED;              // change state of Red LED
      break;

      case INTERACTIVE:
      YELLOW_LED = !YELLOW_LED;        // change state of Yellow LED
      RED_LED = !RED_LED;              // change state of Red LED
      break;
   }
}

//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// This ISR allows the device to wake from Idle mode once an ADC
// conversion is complete.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(ADC0_ISR, ADC0EOC_IRQn)
{
   
   EIE1 &= ~0x08;                      // Disable ADC0 conversion complete int.
 
}

//-----------------------------------------------------------------------------
// RTC0ALARM_ISR
//-----------------------------------------------------------------------------
//
// This ISR sets a software flag to indicate that the ALRM flag was set to 1.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(RTC0ALARM_ISR, RTC0ALARM_IRQn)
{
   EIE1 &= ~0x02;                      // Disable the Alarm interrupt

   Alarm_Pending = 1;                  // Set the software flag
}

//-----------------------------------------------------------------------------
// RTC0_OSC_FAIL_ISR
//-----------------------------------------------------------------------------
//
// This ISR sets a software flag to indicate that the ALRM flag was set to 1.
//
//-----------------------------------------------------------------------------
SI_INTERRUPT(RTC0_OSC_FAIL_ISR, RTC0FAIL_IRQn)
{
   EIE2 &= ~0x04;                      // Disable the Clock Fail interrupt

   smaRTClock_Fail = 1;                // Set the software flag
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
