//-----------------------------------------------------------------------------
// F9xx_smaRTClock_Date_Tracker.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program will keep track of the date and time and output both to the
// toolstick terminal. The initial date and time can be set by changing
// YEAR_INIT, DAY_INIT, HOUR_INIT, MINUTE_INIT under the global constants
// section. Each time P0.2 is pressed, the current date and time will be output
// to the toolstick terminal. If a smaRTClock error occurs, it will be output
// to the terminal.
//
// The program works by setting an RTC alarm to trigger at the end of each day,
// or after the 24th hour. Each alarm trigger increments a global variable
// which stores the the day number. Each time SW2 is pressed, the day (ranging
// from 1-366) is decoded into a month number (ranging from 1-12) and a day of
// the month number (ranging from 1-31). The RTC counter value is captured and
// and decoded into hours, minutes, seconds, and hundreths of a second. To
// handle leap years, the section of the code that decodes the day of the year
// to a month and day of the month is broken up into two sections: leap year
// and non leap year. The first RTC alarm is set to trigger at the end of the
// day based on the initial time settings in the global constants section.
// After the first alarm, a software flag is set (the variable 'second_alarm')
// and the RTC alarm is reconfigured to trigger after every 24 hours.
//
// To switch between SLEEP and SUSPEND mode, change the definition of
// POWER_MODE in the global constants section of the code to either SLEEP
// or SUSPEND
//
// Note: At the begining of Main(), there is a trap while loop. If the device
//       is stuck in SLEEP mode and you are unable to connect to the device,
//       unplug the daughter card from the toolstick. Then, while holding down
//       SW2, plug the daughter card back into the toolstick and then try to
//       connect to the device. After you are connected to the device, you can
//       release SW2.
//
// How To Test:
//
// 1) Ensure that jumpers are placed on the following:
//       J12:  P0.4/TX
//             P0.5/RX
//             VDD/VIO
//             P0.2/SW0.2
//       J11:  PWR/WALL_PWR
//       J17:  VDD_PIN/PWR
// 2) Connect a USB cable from the development board P3 to a PC
// 3) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the USB port (virtual com port) at 115200 and 8-N-1
// 4) Connect the USB Debug Adapter to J9.
// 5) Turn the POWER switch (SW5) to the "ON" position.
// 6) Compile and download code to a 'F93x device on a C8051F93x-TB development
//    board by selecting Run -> Debug from the menus, clicking the Debug button 
//    in the quick menu, or pressing F11.
// 7) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 8) Each time SW2 (P0.2) is pressed, the current time will be output to
//    the terminal screen
//
//
// Target:         C8051F92x-C8051F93x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.3 (BL)
//    - Updated Description / How to Test
//    - 15 JAN 2014
//
// Release 1.2 
//    - Updated to be compatible with Raisonance Toolchain (FB)
//    - 14 APR 2010
//
// Release 1.1
//    - Compiled and tested for TOOLSTICK F931 DC (JH)
//    - 06 JULY 2009
//
// Release 1.0
//    - Initial Revision (ES)
//    - 22 SEP 2008
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "si_toolchain.h"
#include <SI_C8051F930_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
#include <stdio.h>                     // For printf()

//-----------------------------------------------------------------------------
// Pin Declarations
//-----------------------------------------------------------------------------

SI_SBIT(SW2, SFR_P0, 2);                 // SW2 == 0 means switch pressed

//-----------------------------------------------------------------------------
// Type Definitions
//-----------------------------------------------------------------------------
// Made output_month, output_days, hours, minutes, seconds, hundreths all INTs
// because printf will only print INTs in decimal form

typedef struct DATE_INFO {
   uint16_t years;
   uint16_t output_month;
   uint16_t days;
   uint16_t output_days;
   uint16_t hours;
   uint16_t minutes;
   uint16_t seconds;
   uint16_t hundreths;
} DATE_INFO;

//-----------------------------------------------------------------------------
// Global Constants and Variables
//-----------------------------------------------------------------------------

#define SYSCLK         24500000        // SYSCLK frequency in Hz

#define BAUDRATE         115200        // Baud rate of UART in bps

#define RTCCLK            32768        // SmaRTClock frequency in Hz

#define RTC_DAY      2831155200        // Number of RTC counts in a day
#define RTC_HOUR      117964800        // Number of RTC counts in an hour
#define RTC_MINUTE      1966080        // Number of RTC counts in a minute
#define RTC_SECOND        32768        // Number of RTC counts in a second
#define RTC_HUNDRETH        328        // Number of RTC counts in a hundreth
                                       // of a second (rounded up)

#define SUSPEND            0x40        // Value to write to PMU0CF to place
                                       // the device in Suspend mode

#define SLEEP              0x80        // Value to write to PMU0CF to place
                                       // the device in Sleep Mode

#define POWER_MODE      SUSPEND        // Select between Suspend and Sleep
                                       // mode. When debugging, if the
                                       // MCU is stopped/halted while
                                       // in Sleep Mode, the connection
                                       // to the IDE will be lost. The
                                       // IDE connection will not be lost
                                       // if the MCU is stopped/halted
                                       // while in suspend mode.

#define SWITCH_PRESSED        0        // Macros to determine switch state
#define SWITCH_NOT_PRESSED    1

#define FIVE_MS    5*(SYSCLK/12/1000)  // 5 ms delay constant
#define US_DELAY   (SYSCLK/12/1000)/10 // 500 us delay constant

// Used in debounce section of code
#define DEBOUNCE_INTERVAL  10          // Debounce interval in milliseconds
#define T3_DEBOUNCE_TICKS  DEBOUNCE_INTERVAL*(SYSCLK/12/1000)

// Initial settings for time and date
// Current date setting would be September 22, 2008
#define YEAR_INIT            14        // Output will be 2000+YEAR_INIT
#define DAY_INIT            150        // Valid range of DAY_INIT is 1-366
#define HOUR_INIT            15        // Valid range of HOUR_INIT is 0-23
#define MINUTE_INIT           0        // Valid range of MINUTE_INIT is 0-59
#define SECOND_INIT           0        // Valid range of SECOND_INIT is 0-59

// Convert initial time settings to RTC alarm counts. Used in RTC_Init()
// (note: '\' is in following line to allow code to continue on next line)
#define TIME_INIT    ((RTC_HOUR*HOUR_INIT)+(RTC_MINUTE*MINUTE_INIT)+   \
                     (RTC_SECOND*SECOND_INIT))

DATE_INFO event1;                      // This will keep the runnnig date
                                       // information

// Software flags to indicate smaRTClock events are pending
bit Alarm_Pending = 0;
bit smaRTClock_Fail = 0;

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

uint8_t RTC_Read (uint8_t reg);
void RTC_Write (uint8_t reg, uint8_t value);
uint8_t Leap_Year (uint16_t year);
void Compute_Month (uint16_t * month, uint16_t * days, uint16_t total,
		    uint16_t year);

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
  // Disable the watchdog here
}
 
#if defined SDCC
   void putchar (char output);
#endif
//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
	static bit wakeup_pending;          // This is checked before putting the
									   // device to sleep to ensure a wake-up
									   // wasn't triggered during a clear of
									   // PMU0CF

	volatile bit second_alarm;          // Software flag used to signify that
									   // the first alarm has already occured.
	uint8_t wakeup_source;

	SI_UU32_t RTC_Capture;                   // Will hold contents of RTC timer when
									   // button is pushed

	SI_UU32_t alarm_interval_second;         // Will hold new alarm interval after
									   // second_alarm bit set

	// Trap in case device gets stuck in SLEEP
	if (!SW2)
	{
	  while (1);
	}

	//Enter default mode
	enter_DefaultMode_from_RESET();
	SCON0_TI = 1;                            // Indicate TX0 ready

	printf("\nsmaRTClock Date Tracker\n");

	// Initialize DATE_INFO struct
	event1.seconds = SECOND_INIT;
	event1.minutes = MINUTE_INIT;
	event1.hours   = HOUR_INIT;
	event1.days    = DAY_INIT;
	event1.years   = (2000+YEAR_INIT);

	second_alarm = 0;                   // Initialize software flag

	// Update output_month and output_days for output
	Compute_Month(&event1.output_month, &event1.output_days, event1.days,
				  event1.years);

	// Output initial time
	printf("Current Time: %d-%d-%d %d:%d:%d", event1.output_month,
			event1.output_days, event1.years, event1.hours, event1.minutes,
			event1.seconds);

	printf("\nPress P0.2 to Output Time\n");

	//----------------------------------
	// Main Application Loop
	//----------------------------------
	while (1)
	{
		 wakeup_pending = 0;           // Clear software flag

		 // Clear the PMU wake-up flags
		 // Interrupts for all enabled sources that are not persistent
		 // should be enabled when clearing the wake-up source flags.
		 // This is a precautionary step in case these events occur
		 // during the same clock cycle that clears the PMU0CF flags.
		 //Enter default mode
		 enter_Enable_Interrupts_from_DefaultMode();
		 PMU0CF = 0x20;
		 EIE1 &= ~0x02;                // Disable the alarm interrupt
		 EIE2 &= ~0x04;                // Disable the clock fail interrupt

		 // Check for non-persistent wake-up sources
		 if ((Alarm_Pending) || (smaRTClock_Fail))
		 {
			wakeup_pending = 1;        // Set software flag
		 }

		 // Enable wake-up sources and place the device in Sleep Mode
		 // Wake-Up sources enabled are:
		 // A. smaRTClock Alarm
		 // B. smaRTClock Oscillator Fail
		 // C. Port Match (wake-up on switch press)
		 // D. Pulse on Reset Pin (always enabled)
		 if ((!wakeup_pending) || (SW2))
		 {
		  RSTSRC = 0x04;               // Disable VDDMON, leave missing clock
									   // detector enabled
		  // Put device to sleep
		  PMU0CF = 0x0E | POWER_MODE;  // To change between SUSPEND or SLEEP
		 }                             // modes, change the POWER_MODE
									   // definition in the global constants

		 //--------------------------------------------------------------------
		 // ~~~~~~~~~~~~~~~~~~~~ Device Sleeping ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 //--------------------------------------------------------------------

		 while (!(VDM0CN & 0x20));     // Wait for VDDOK to be set to 1

		 RSTSRC = 0x06;                // Enable VDDMON, missing clock detector

		 // Read the wake-up source flags
		 wakeup_source = PMU0CF & 0x1F;

		 // Clear the wake-up source flags
		 PMU0CF = 0x20;

		 // Check for a reset pin wake-up (wait about 5 ms)
		 if (wakeup_source & 0x10)
		 {
			   TMR3 = 0;               // Initialize T3 to zero
			   TMR3CN |= 0x04;         // Start timer (TR3 = 1)
			   while (TMR3 < FIVE_MS);
			   TMR3CN &= ~0x04;        // Stop timer (TR3 = 0)
		 }

		 // Check for smaRTClock Oscillator failure
		 if ((wakeup_source & 0x08) || smaRTClock_Fail)
		 {
			IE_EA = 0;
			smaRTClock_Fail = 0;       // Clear the software flag
			printf("\nsmaRTClock Failure\n");
			while (1);

		 }

		 // Check for smaRTClock alarm
		 if ((wakeup_source & 0x04) || Alarm_Pending)
		 {
			Alarm_Pending = 0;         // Clear the software flag

			if ((event1.days == 365) & (Leap_Year(event1.years)))
			{
				  event1.days++;       // Check for leap year, increment days
			}
			else if (event1.days >= 365)
			{
				  event1.days = 1;     // If non-leap year, reset days,
				  event1.years ++;     // increment years
			}
			else
			{
				  event1.days ++;
			}

			// 5 ms wait
			TMR3 = 0;                  // Initialize T3 to zero
			TMR3CN |= 0x04;            // Start timer (TR3 = 1)
			while (TMR3 < FIVE_MS);
			TMR3CN &= ~0x04;           // Stop timer (TR3 = 0)

			if (!second_alarm)
			{
			   second_alarm = 1;       // Set software flag, only want to come
									   // into this section of code once

			   alarm_interval_second.u32 = (RTC_DAY); // Set alarm to trigger
													  // every 24 hours
			   // Copy the alarm interval to the alarm registers
			   RTC_Write (ALARM0, alarm_interval_second.u8[B0]);     // LSB
			   RTC_Write (ALARM1, alarm_interval_second.u8[B1]);
			   RTC_Write (ALARM2, alarm_interval_second.u8[B2]);
			   RTC_Write (ALARM3, alarm_interval_second.u8[B3]);     // MSB
			}
		 }

		 // Check for Port Match event
		 if (wakeup_source & 0x02)
		 {
			if (SW2 == SWITCH_PRESSED)
			{
			   // Debounce the Switch (wait > 5 ms)
			   TMR3 = 0;               // Initialize T3 to zero
			   TMR3CN |= 0x04;         // Start timer (TR3 = 1)
			   while (TMR3 < T3_DEBOUNCE_TICKS);
			   TMR3CN &= ~0x04;        // Stop timer (TR3 = 0)

			   // Check if the switch is still pressed
			   if (SW2 == SWITCH_PRESSED)
			   {
				  // Compute month and days for output
					Compute_Month(&event1.output_month, &event1.output_days,
						   event1.days, event1.years);

				  RTC_Write (RTC0CN, 0xDD);        // Initiate smaRTClock timer
												   // capture
				  // Wait for smaRTClock timer capture valid
				  while ((RTC_Read (RTC0CN) & 0x01)== 0x01);

				  // Copy the RTC capture timer to the RTC capture variable
				  RTC_Capture.u8[B0] = RTC_Read (CAPTURE0);    // LSB
				  RTC_Capture.u8[B1] = RTC_Read (CAPTURE1);
				  RTC_Capture.u8[B2] = RTC_Read (CAPTURE2);
				  RTC_Capture.u8[B3] = RTC_Read (CAPTURE3);    // MSB

				  if (!second_alarm){
					 // Adjust captured time with initial time settings if
					 // frst day has not passed
					 (RTC_Capture.u32) = (RTC_Capture.u32)+TIME_INIT;
				  }

				  event1.hours = (RTC_Capture.u32)/RTC_HOUR;
				  (RTC_Capture.u32) = (RTC_Capture.u32)%RTC_HOUR;
				  event1.minutes = (RTC_Capture.u32)/RTC_MINUTE;
				  (RTC_Capture.u32) = (RTC_Capture.u32)%RTC_MINUTE;
				  event1.seconds = (RTC_Capture.u32)/RTC_SECOND;
				  (RTC_Capture.u32) = (RTC_Capture.u32)%RTC_SECOND;
				  event1.hundreths = (RTC_Capture.u32)/RTC_HUNDRETH;

				  // Output date
				  printf("\nCurrent Time: %d-%d-%d ", event1.output_month,
					 event1.output_days, event1.years);

				  printf("%d:%d:%d.%d\n",event1.hours, event1.minutes,
					 event1.seconds, event1.hundreths);

				  // If device is configured to enter SLEEP mode, delay for
				  // a short time (~500 us) to let the UART finish
				  // finish transmitting before looping and going
				  // back into SLEEP mode
				  #if POWER_MODE == SLEEP
					 // 500 us wait
					 TMR3 = 0;                  // Initialize T3 to zero
					 TMR3CN |= 0x04;            // Start timer (TR3 = 1)
					 while (TMR3 < US_DELAY);
					 TMR3CN &= ~0x04;           // Stop timer (TR3 = 0)
				  #endif
			   }
			   else
			   {
				  // Switch is bouncing, take no action
			   }
			}
		 }
	}

	// NOTREACHED
	return 0;
}

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// RTC_Read
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
   reg &= 0x0F;                        // Mask low nibble
   RTC0ADR  = reg;                     // Pick register
   RTC0ADR |= 0x80;                    // Set BUSY bit to read
   while ((RTC0ADR & 0x80) == 0x80);   // Poll on the BUSY bit
   return RTC0DAT;                     // Return value
}

//-----------------------------------------------------------------------------
// RTC_Write
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint8_t reg - address of RTC register to write
//                2) uint8_t value - the value to write to <reg>
//
// This function will Write one byte to the specified RTC register.
// Using a register number greater that 0x0F is not permited.
//
//-----------------------------------------------------------------------------

void RTC_Write (uint8_t reg, uint8_t value)
{
   reg &= 0x0F;                        // Mask low nibble
   RTC0ADR  = reg;                     // Pick register
   RTC0DAT = value;                    // Write data
   while ((RTC0ADR & 0x80) == 0x80);   // Poll on the BUSY bit
}

//-----------------------------------------------------------------------------
// Leap_Year
//-----------------------------------------------------------------------------
//
// Return Value : '1' if a leap year, '0' if not a leap year
// Parameters   : uint16_t year - the current year in the counter
//
// This function will return a whether a year is a leap year or not
//
//-----------------------------------------------------------------------------

uint8_t Leap_Year (uint16_t year)
{
   if ((((year%4)==0) && ((year%100)!=0)) || ((year%400)==0))
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

//-----------------------------------------------------------------------------
// Compute_Month
//-----------------------------------------------------------------------------
//
// Return Value : none
// Parameters   : 1) uint16_t* month - pointer to month number variable (1-12)
//                2) uint16_t* days - pointer to number of days  variable (1-31)
//                3) uint16_t total - total number of days to compute
//                   month and days from
//                4) uint16_t total - current year (in 20xx format)
//
// This function will calculate the month number and day number based on the
// total number of days in a given year that have passed. This function is
// split into a leap year section and non-leap year section.
//
//-----------------------------------------------------------------------------

void Compute_Month (uint16_t * month, uint16_t * days, uint16_t total,
                    uint16_t year)
{
   if (total < 32)                     // First month does not depend on
   {                                   // leap year
      (*month) = 1;                    // January
      (*days) = total;
   }
   else if (Leap_Year(year))
   {
      // Calculate month based on leap year
      if (total < 61)
      {
         (*month) = 2;                 // February
         (*days) = total-31;
      }
      else if (total < 92)
      {
         (*month) = 3;                 // March
         (*days) = total-60;
      }
      else if (total < 122)
      {
         (*month) = 4;                 // April
         (*days) = total - 91;
      }
      else if (total < 153)
      {
         (*month) = 5;                 // May
         (*days) = total - 121;
      }
      else if (total < 183)
      {
         (*month) = 6;                 // June
         (*days) = total-152;
      }
      else if (total < 214)
      {
         (*month) = 7;                 // July
         (*days) = total - 182;
      }
      else if (total < 245)
      {
         (*month) = 8;                 // August
         (*days) = total - 213;
      }
      else if (total < 275)
      {
         (*month) = 9;                 // September
         (*days) = total - 244;
      }
      else if (total < 306)
      {
         (*month) = 10;                // October
         (*days) = total-274;
      }
      else if (total < 336)
      {
         (*month) = 11;                // November
         (*days) = total - 305;
      }
      else
      {
         (*month) = 12;                // December
         (*days) = total - 335;
      }

   }
   else
   {
      // Calculate month based on non leap year
      if (total < 60)
      {
         (*month) = 2;                 // February
         (*days) = total-31;
      }
      else if (total < 91)
      {
         (*month) = 3;                 // March
         (*days) = total-59;
      }
      else if (total < 121)
      {
         (*month) = 4;                 // April
         (*days) = total - 90;
      }
      else if (total < 152)
      {
         (*month) = 5;                 // May
         (*days) = total - 120;
      }
      else if (total < 182)
      {
         (*month) = 6;                 // June
         (*days) = total-151;
      }
      else if (total < 213)
      {
         (*month) = 7;                 // July
         (*days) = total - 181;
      }
      else if (total < 244)
      {
         (*month) = 8;                 // August
         (*days) = total - 212;
      }
      else if (total < 274)
      {
         (*month) = 9;                 // September
         (*days) = total - 243;
      }
      else if (total < 305)
      {
         (*month) = 10;                // October
         (*days) = total-273;
      }
      else if (total < 335)
      {
         (*month) = 11;                // November
         (*days) = total - 304;
      }
      else
      {
         (*month) = 12;                // December
         (*days) = total - 334;
      }
   }
}

#if defined SDCC

// SDCC does not include a definition for putchar(), which is used in printf()
// and so it is defined here.  The prototype does not need to be explicitly
// defined because it is provided in stdio.h

//-----------------------------------------------------------------------------
// putchar
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : character to send to UART
//
// This function outputs a character to the UART.
//
//-----------------------------------------------------------------------------

void putchar (char output)
{
   if (output == '\n')
   {
      while (!SCON0_TI);
      SCON0_TI = 0;
      SBUF0 = 0x0D;
   }
   while (!SCON0_TI);
   SCON0_TI = 0;
   SBUF0 = output;
}

#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
