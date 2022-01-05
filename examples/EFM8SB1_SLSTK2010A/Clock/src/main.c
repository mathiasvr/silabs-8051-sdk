//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates a Clock on the EFM8SB1 STK using the
// LCD, joystick, push buttons, and RTC (crystal).
//
//-----------------------------------------------------------------------------
// How To Test: EFM8SB1 STK
//-----------------------------------------------------------------------------
// 1) Place the SW104 switch in "AEM" mode.
// 2) Connect the EFM8SB1 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8SB1 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The clock demo should start.
// 6) By default, the analog clock will show the default time using
//    #defines in demo_config.h. The LED will turn on when the MCU
//    is in active mode, and the LED will turn off when the MCU is
//    in low power mode (or the LCD is turned off).
// 7) Press PB1 to toggle between analog clock and digital clock mode.
// 8) Press PB0 to turn the LCD off to conserve power. Press PB0 again
//    to turn the LCD back on.
// 9) Press the joystick center button to adjust the current time.
// 10) Press the joystick up/down to increase/decrease the currently
//     selected hours, minutes, or seconds.
// 11) Press the joystick left/right to select hours, minutes, or seconds
//     to adjust. The selected time unit will flash.
// 12) Press the joystick center button to apply the new time and return
//     to the previous clock mode, analog or digital.
//
// Target:         EFM8SB1
// Tool chain:     Generic
//
// Release 0.1 (CM)
//    - Initial Revision
//    - 10 OCT 2014
//
//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 24.5 MHz HFOSC / 1
// ADC0   - 10-bit
// SPI0   - 1 MHz
// Timer0 - 2 MHz (4 ms tick for time adjust blinking)
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P0.1 - LCD CS (Active High)
// P0.6 - LCD SCK
// P1.0 - LCD MOSI
// P1.1 - LED
// P1.2 - push button
// P1.3 - push button
// P1.4 - Joystick (analog voltage divider)
// P2.7 - Display enable
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "bsp.h"
#include "main.h"
#include "InitDevice.h"
#include "clock.h"
#include "disp.h"
#include "tick.h"
#include "joystick.h"
#include "adc.h"
#include "demo_config.h"

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// This will keep the current hour
SI_SEGMENT_VARIABLE(Hours, uint8_t, SI_SEG_XDATA);
// This will keep the current minute
SI_SEGMENT_VARIABLE(Minutes, uint8_t, SI_SEG_XDATA);
// This will keep the current second
SI_SEGMENT_VARIABLE(Seconds, uint8_t, SI_SEG_XDATA);

// Variable used to blink the digital time in set time mode
volatile bool Blink;

// Controls whether LCD drawing code is executed or not
bool LCD_State = LCD_ON;

// Controls whether displaying digital or analog mode
bool Clock_Mode = ANALOG_CLOCK;

//-----------------------------------------------------------------------------
// Global Functions
// ----------------------------------------------------------------------------

// Increment time by 1 second
void IncrementTime()
{
    if (Seconds == 59)
    {
        Seconds = 0;
        if (Minutes == 59)
        {
            Minutes = 0;
            if (Hours == 23)
            {
                Hours = 0;
            }
            else
            {
                Hours++;
            }

        }
        else
        {
            Minutes++;
        }
    }
    else
    {
        Seconds++;
    }
}

void AdjustTime()
{
    uint8_t setSelect = 0;
    uint8_t joystick;
    uint16_t lastTick;
    uint16_t elapsedTick;
    bool isJoystickHeld = false;

    // Set P1.4 joystick to analog input
    // to read joystick positions
    P1MASK &= ~P1MASK_B4__COMPARED;
    P1MDIN &= ~P1MDIN_B4__BMASK;

    // Turn on Timer 0, used for blinking the time
    // and joystick ADC start of conversion
    TCON |= 0x10;
    IE |= 0x02;

    DigitalTimeDraw(Hours, Minutes, Seconds);

    // Wait for first ADC conversion to complete
    while (!ADC0CN0_ADINT);

    // Wait for user to release the joystick
    while ((joystick = GetJoystickDirection()) != JOYSTICK_NONE);

    while (1)
    {
        // Draw set time, blinking the current unit being set
        if (!isJoystickHeld && Blink)
        {
            switch (setSelect)
            {
            case 0: DigitalTimeDraw(BLANK, Minutes, Seconds);   break;
            case 1: DigitalTimeDraw(Hours, BLANK, Seconds);     break;
            case 2: DigitalTimeDraw(Hours, Minutes, BLANK);     break;
            }
        }
        else
        {
            DigitalTimeDraw(Hours, Minutes, Seconds);
        }

        // Get joystick direction
        joystick = GetJoystickDirection();
        lastTick = GetTickCount();

        // Wait for user to release joystick or start repeating
        while (1)
        {
            // Joystick was release
            if (GetJoystickDirection() == JOYSTICK_NONE)
            {
                isJoystickHeld = false;
                break;
            }
            // Joystick is being held (repeat initial delay)
            else if (!isJoystickHeld)
            {
                if ((elapsedTick = (GetTickCount() - lastTick)) >= JOYSTICK_REPEAT_INITIAL_DELAY)
                {
                    // Joystick is being held
                    isJoystickHeld = true;
                    break;
                }
            }
            // Joystick is being held (repeat delay)
            else if ((elapsedTick = (GetTickCount() - lastTick)) >= JOYSTICK_REPEAT_DELAY)
            {
                break;
            }
        }

        // Done setting the time
        if (joystick == JOYSTICK_C)
        {
            // User must release the joystick to leave the
            // adjust time mode
            while (GetJoystickDirection() != JOYSTICK_NONE);
            break;
        }
        // Setting time: increments the time interval
        else if (joystick == JOYSTICK_N)
        {
            if (setSelect == 0)
            {
                if (Hours == 23)
                {
                    Hours = 0;
                }
                else
                {
                    Hours++;
                }
            }
            else if (setSelect == 1)
            {
                if (Minutes == 59)
                {
                    Minutes = 0;
                }
                else
                {
                    Minutes++;
                }
            }
            else if (setSelect == 2)
            {
                if (Seconds == 59)
                {
                    Seconds = 0;
                }
                else
                {
                    Seconds++;
                }
            }
        }
        // Setting time: decrements the time interval
        else if (joystick == JOYSTICK_S)
        {
            if (setSelect == 0)
            {
                if (Hours == 0)
                {
                    Hours = 23;
                }
                else
                {
                    Hours--;
                }
            }
            else if (setSelect == 1)
            {
                if (Minutes == 0)
                {
                    Minutes = 59;
                }
                else
                {
                    Minutes--;
                }
            }
            else if (setSelect == 2)
            {
                if (Seconds == 0)
                {
                    Seconds = 59;
                }
                else
                {
                    Seconds--;
                }
            }
        }
        // Setting time: move to next time units
        else if (joystick == JOYSTICK_E)
        {
            if (setSelect == 2)
            {
                setSelect = 0;
            }
            else
            {
                setSelect++;
            }
        }
        // Setting time: move to previous time units
        else if (joystick == JOYSTICK_W)
        {
            if (setSelect == 0)
            {
                setSelect = 2;
            }
            else
            {
                setSelect--;
            }
        }
    }

    // Turn off Timer 0 since done setting time,
    // no more blinking and no more analog
    // joystick sampling
    TCON &= ~0x10;
    IE &= ~0x02;

    // Set P1.4 joystick back to digital input
    // for port match detection
    P1MASK |= P1MASK_B4__COMPARED;
    P1MDIN |= P1MDIN_B4__DIGITAL;
}

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
 
//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main(void)
{
    uint8_t wakeup_source;

    //Enter default mode
    enter_DefaultMode_from_RESET();
    DISP_Init();

    // Initialize current time
    Seconds = SECOND_INIT;
    Minutes = MINUTE_INIT;
    Hours = HOURS_INIT;

    // Draw the analog clock by default
    AnalogTimeDraw(Hours, Minutes, Seconds);

    // Allow SPI transmissions to LCD to finish before entering sleep
    Wait(5);

    //----------------------------------
    // Main Application Loop
    //----------------------------------
    while (1)
    {
        // Clear the PMU wake-up flags
        // Interrupts for all enabled sources that are not persistent
        // should be enabled when clearing the wake-up source flags.
        // This is a precautionary step in case these events occur
        // during the same clock cycle that clears the PMU0CF flags.
        //EIE2 |= 0x04;                 // Enable the clock fail interrupt
        PMU0CF = 0x20;
        //EIE2 &= ~0x04;                // Disable the clock fail interrupt

        // Enable wake-up sources and place the device in Sleep Mode
        // Wake-Up sources enabled are:
        // A. smaRTClock Alarm
        // B. smaRTClock Oscillator Fail
        // C. Port Match (wake-up on switch press)
        // D. Pulse on Reset Pin (always enabled)

        RSTSRC = 0x04;                // Disable VDDMON, leave missing clock
                                      // detector enabled
        // Put device to sleep
        BSP_LED0 = BSP_LED_OFF;
        PMU0CF = 0x0E | POWER_MODE;   // To change between SUSPEND or SLEEP
                                      // modes, change the POWER_MODE
                                      // definition in the global constants

        // Turn the LED on while the device is awake, but not if
        // the LCD is turned off (to save power)
        if (LCD_State == LCD_ON)
        {
            BSP_LED0 = BSP_LED_ON;
        }

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
            Wait(5);
        }

        // Check for smaRTClock Oscillator failure
        if (wakeup_source & 0x08)
        {
            IE_EA = 0;
            while (1);
        }

        // Check for smaRTClock alarm
        if (wakeup_source & 0x04)
        {
            IncrementTime();

            if (LCD_State == LCD_ON)
            {
                // Display updated time
                if (Clock_Mode == DIGITAL_CLOCK)
                {
                    DigitalTimeDraw(Hours, Minutes, Seconds);
                }
                else
                {
                    AnalogTimeDraw(Hours, Minutes, Seconds);
                }
            }
#if POWER_MODE == SLEEP
            Wait(5); 	// Wait 5 ms to let SPI to finish
#endif
        }

        // Check for Port Match event
        if (wakeup_source & 0x02)
        {
            if (LCD_State == LCD_ON)
            {
                if (PB_TIME_SET == BSP_PB_PRESSED)
                {
                    AdjustTime();

                    // Display updated time
                    if (Clock_Mode == DIGITAL_CLOCK)
                    {
                        DigitalTimeDraw(Hours, Minutes, Seconds);
                    }
                    else
                    {
                        AnalogTimeDraw(Hours, Minutes, Seconds);
                    }
                }

                // If device is configured to enter SLEEP mode, delay for
                // a short time (~5 ms) to let the SPI finish
                // finish transmitting before looping and going
                // back into SLEEP mode
#if POWER_MODE == SLEEP
                Wait(5);
#endif
            }

            if (PB_LCD_OFF == BSP_PB_PRESSED)
            {
                // Debounce the Switch (wait > 5 ms)
                Wait(5);

                // Check if the switch is still pressed

                // This routine allows the user to set the time after SW1 is pressed
                // There is a time limit from the last time a button was pressed
                if (PB_LCD_OFF == BSP_PB_PRESSED)
                {
                    if (LCD_State == LCD_OFF)
                    {
                        LCD_State = LCD_ON;
                    }
                    else if (LCD_State == LCD_ON)
                    {
                        LCD_State = LCD_OFF;
                        ClearBackground();
                    }
                }
            }

            if (LCD_State == LCD_ON)
            {
                // This section allows user to change between analog and digital clock modes
                if (PB_ANALOG_DIGITAL == BSP_PB_PRESSED)
                {
                    // Debounce the Switch (wait > 5 ms)
                    Wait(5);

                    // Check if the switch is still pressed
                    if (PB_ANALOG_DIGITAL == BSP_PB_PRESSED)
                    {
                        // Toggle between analog and digital mode every time SW2 is pressed
                        Clock_Mode ^= 1;

                        if (Clock_Mode == DIGITAL_CLOCK)
                        {
                            DigitalTimeDraw(Hours, Minutes, Seconds);
                        }
                        else
                        {
                            AnalogTimeDraw(Hours, Minutes, Seconds);
                        }
                    }
                }
                // If device is configured to enter SLEEP mode, delay for
                // a short time (5 ms) to let the SPI finish
                // finish transmitting before looping and going
                // back into SLEEP mode
#if POWER_MODE == SLEEP
                Wait(5);
#endif
            }
        }
    }
}

