/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// oscilloscope.c
/////////////////////////////////////////////////////////////////////////////

// Oscilloscope Demo
// =================
//
// Description:
// Capture ADC samples and plot on the LCD
//
// Parameters:
// Max ADC Sample Rate: 500 ksps
// Max ADC Input Voltage: 3.3 V
// Trigger Options: Rising/Falling Edge Trigger using ADC window compare
//                  (Auto-capture mode)
// LCD Orientation: 90 degrees CCW

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "InitDevice.h"
#include "joystick.h"
#include "disp.h"
#include "render.h"
#include "tick.h"
#include "utils.h"
#include "oscilloscope.h"
#include "capture.h"
#include "rgb_led.h"
#include "splash.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
// Typedefs
/////////////////////////////////////////////////////////////////////////////

typedef SI_VARIABLE_SEGMENT_POINTER(, char, const SI_SEG_CODE) codeStr_t;

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

#define COLOR_RED       Colors[0]
#define COLOR_GREEN     Colors[1]
#define COLOR_BLUE      Colors[2]

const SI_SEGMENT_VARIABLE(Colors[], Color, SI_SEG_CODE) = {
    {255, 0, 0},  // Red
    {0, 255, 0},  // Green
    {0, 0, 255}   // Blue
};

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////

static SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

bool IsJoystickEnabled = false;
bool SingleSeqOneShot = false;

bool IsRunning = true;
bool IsMenuActive = false;
MENU_ITEM ActiveMenuItem = MENU_DISPLAY_TYPE;
DISPLAY_TYPE DisplayType = DISPLAY_TYPE_VECTORS;
SHOW_LABELS ShowLabels = SHOW_LABELS_ON;
TRIGGER_MODE TriggerMode = TRIGGER_MODE_AUTO;
INPUT AnalogInput = INPUT_JOY;
RATE SampleRate = RATE_500K;

uint8_t HorizontalPosX = 0;

/////////////////////////////////////////////////////////////////////////////
// Global Variables - Menu String Constants
/////////////////////////////////////////////////////////////////////////////

SI_SEGMENT_VARIABLE(MenuTextDisplay[], codeStr_t, const SI_SEG_CODE) = {
    "Display:  Vectors",
    "Display:  Dots"
};

SI_SEGMENT_VARIABLE(MenuTextShowLabels[], codeStr_t, const SI_SEG_CODE) = {
    "Labels:   Off",
    "Labels:   On"
};

SI_SEGMENT_VARIABLE(MenuTextTriggerMode[], codeStr_t, const SI_SEG_CODE) = {
    "Mode:     Auto",
    "Mode:     Normal"
};

SI_SEGMENT_VARIABLE(MenuTextTriggerSlope[], codeStr_t, const SI_SEG_CODE) = {
    "Slope:    Rising",
    "Slope:    Falling"
};

SI_SEGMENT_VARIABLE(MenuTextInput[], codeStr_t, const SI_SEG_CODE) = {
    "Input:    Joystick",
    "Input:    P1.1"
};

SI_SEGMENT_VARIABLE(MenuTextSampleRate[], codeStr_t, const SI_SEG_CODE) = {
    "Rate:     31.25 kHz",
    "Rate:     62.5 kHz",
    "Rate:     125 kHz",
    "Rate:     250 kHz",
    "Rate:     500 kHz",
    "Rate:     500x2 kHz"
};

SI_SEGMENT_VARIABLE(MenuTextWindowPeriod[], codeStr_t, const SI_SEG_CODE) = {
    "4096us",
    "2048us",
    "1024us",
    "512us",
    "256us",
    "128us"
};

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

uint8_t GetJoystickDirection();
bool IsJoystickInputEnabled();
void EnableJoystickInput();
void DisableJoystickInput();

void ApplyMenuSettings();

void HandleInput();
void HandleMenuModeInput();
void HandleRunModeInput();
void HandleHaltModeInput();

void DrawSplash();
void DrawMenu();
void DrawWaveform();

void SynchFrame();

void Oscilloscope_Init();
void Oscilloscope_main();

/////////////////////////////////////////////////////////////////////////////
// Jostick Functions
/////////////////////////////////////////////////////////////////////////////

uint8_t GetJoystickDirection()
{
    uint32_t mv;
    uint8_t dir;

    // Wait for conversion to complete
    ADC0CN0_ADINT = 0;
    while (!ADC0CN0_ADINT);
    ADC0CN0_ADINT = 0;

    mv = ((uint32_t)ADC0H) * 3300 / 256;
    dir = JOYSTICK_convert_mv_to_direction(mv);

    return dir;
}

bool IsJoystickInputEnabled()
{
    return IsJoystickEnabled;
}

void EnableJoystickInput()
{
    // Already enabled
    if (IsJoystickEnabled)
    {
        return;
    }

    if (IsCaptureInProgress())
    {
        AbortCapture();
    }

    IsJoystickEnabled = true;

    // Switch VREF to VDD (instead of 1.65V Vref)
    REF0CN = (REF0CN & ~REF0CN_REFSL__FMASK) | REF0CN_REFSL__VDD_PIN;

    // Switch ADC gain to 1x
    ADC0CF |= ADC0CF_ADGN__GAIN_1;

    // Switch ADC input to joystick
    ADC0MX = ADC0MX_ADC0MX__ADC0P15;

    // Some EFM8 STK joysticks have very large (weak) resistors,
    // so reduce the ADC sampling rate to reduce the voltage
    // drop at the analog input due to ADC sample capacitor charging.
    //
    // Set ADC conversion rate to 31.25 KHz
    CKCON0 &= ~CKCON0_T0M__SYSCLK;
    TH0 = 60;
}

void DisableJoystickInput()
{
    // Already disabled
    if (!IsJoystickEnabled)
    {
        return;
    }

    // Switch VREF back to 1.65V internal Vref
    REF0CN = (REF0CN & ~REF0CN_REFSL__FMASK) | REF0CN_REFSL__INTERNAL_VREF;

    // Switch ADC gain back to 0.5x
    ADC0CF &= ~ADC0CF_ADGN__GAIN_1;

    ApplyMenuSettings();

    IsJoystickEnabled = false;
}

/////////////////////////////////////////////////////////////////////////////
// Menu Functions
/////////////////////////////////////////////////////////////////////////////

void ApplyMenuSettings()
{
    // Select analog input pin
    switch (AnalogInput)
    {
    case INPUT_JOY:     ADC0MX = ADC0MX_ADC0MX__ADC0P15;    break;
    case INPUT_EXT:     ADC0MX = ADC0MX_ADC0MX__ADC0P9;     break;
    }

    // Use Prescaler for SYSCLK/4
    // or
    // Use SYSCLK for higher sample rates
    switch (SampleRate)
    {
    case RATE_31K25:    CKCON0 &= ~CKCON0_T0M__SYSCLK; TH0 = 60;   break;
    case RATE_62K5:     CKCON0 &= ~CKCON0_T0M__SYSCLK; TH0 = 158;  break;
    case RATE_125K:     CKCON0 |=  CKCON0_T0M__SYSCLK; TH0 = 60;   break;
    case RATE_250K:     CKCON0 |=  CKCON0_T0M__SYSCLK; TH0 = 158;  break;

    case RATE_500K:
    case RATE_500KX2:   CKCON0 |=  CKCON0_T0M__SYSCLK; TH0 = 207;  break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Input Handler Functions
/////////////////////////////////////////////////////////////////////////////

// Handle user input
// Return what type of input was handled
void HandleInput()
{
    if (IsRunning || SingleSeqOneShot)
    {
        DisableJoystickInput();
    }
    else
    {
        EnableJoystickInput();
    }

    if (IsMenuActive)
    {
        HandleMenuModeInput();
    }
    else if (IsRunning)
    {
        HandleRunModeInput();
    }
    else
    {
        HandleHaltModeInput();
    }

    if (IsRunning || SingleSeqOneShot)
    {
        DisableJoystickInput();
    }
    else
    {
        EnableJoystickInput();
    }
}

// Handle input from user while in the menu
// Return what type of input was handled
void HandleMenuModeInput()
{
    uint8_t dir;

    // Get current joystick direction
    dir = GetJoystickDirection();

    // Wait for user to release the joystick from current direction
    if (dir != JOYSTICK_NONE)
    {
        while (GetJoystickDirection() == dir);
    }

    // "Up"
    if (dir == JOYSTICK_W)
    {
        ActiveMenuItem = (ActiveMenuItem == (MENU_ITEM)0) ? (MENU_ITEM)(MENU_COUNT - 1) : (MENU_ITEM)(ActiveMenuItem - 1);
    }
    // "Down"
    else if (dir == JOYSTICK_E)
    {
        ActiveMenuItem = (MENU_ITEM)((ActiveMenuItem + 1) % MENU_COUNT);
    }
    // "Left"
    else if (dir == JOYSTICK_S)
    {
        switch (ActiveMenuItem)
        {
        case MENU_DISPLAY_TYPE:     DisplayType = (DisplayType == 0) ? (DISPLAY_TYPE)(DISPLAY_TYPE_COUNT - 1) : (DISPLAY_TYPE)(DisplayType - 1);                break;
        case MENU_SHOW_LABELS:      ShowLabels = (ShowLabels == 0) ? (SHOW_LABELS)(SHOW_LABELS_COUNT - 1) : (SHOW_LABELS)(ShowLabels - 1);                      break;
        case MENU_TRIGGER_MODE:     TriggerMode = (TRIGGER_MODE)(TriggerMode == 0) ? (TRIGGER_MODE)(TRIGGER_MODE_COUNT - 1) : (TRIGGER_MODE)(TriggerMode - 1);  break;
        case MENU_TRIGGER_SLOPE:    TriggerSlope = (TriggerSlope == 0) ? (TRIGGER_SLOPE)(TRIGGER_SLOPE_COUNT - 1) : (TRIGGER_SLOPE)(TriggerSlope - 1);          break;
        case MENU_INPUT:            AnalogInput = (AnalogInput == 0) ? (INPUT)(INPUT_COUNT - 1) : (INPUT)(AnalogInput - 1);                                     break;
        case MENU_SAMPLE_RATE:      SampleRate = (SampleRate == 0) ? (RATE)(RATE_COUNT - 1) : (RATE)(SampleRate - 1);                                           break;
        }
    }
    // "Right"
    else if (dir == JOYSTICK_N)
    {
        switch (ActiveMenuItem)
        {
        case MENU_DISPLAY_TYPE:     DisplayType = (DISPLAY_TYPE)((DisplayType + 1) % DISPLAY_TYPE_COUNT);     break;
        case MENU_SHOW_LABELS:      ShowLabels = (SHOW_LABELS)((ShowLabels + 1) % SHOW_LABELS_COUNT);         break;
        case MENU_TRIGGER_MODE:     TriggerMode = (TRIGGER_MODE)((TriggerMode + 1) % TRIGGER_MODE_COUNT);     break;
        case MENU_TRIGGER_SLOPE:    TriggerSlope = (TRIGGER_SLOPE)((TriggerSlope + 1) % TRIGGER_SLOPE_COUNT); break;
        case MENU_INPUT:            AnalogInput = (INPUT)((AnalogInput + 1) % INPUT_COUNT);                   break;
        case MENU_SAMPLE_RATE:      SampleRate = (RATE)((SampleRate + 1) % RATE_COUNT);                       break;
        }
    }
    // "Center"
    else if (dir == JOYSTICK_C)
    {
        IsMenuActive = false;
    }
}

// Handle input from push buttons while running
void HandleRunModeInput()
{
    // Run/Halt
    if (BSP_PB1 == BSP_PB_PRESSED)
    {
        while (BSP_PB1 == BSP_PB_PRESSED);

        IsRunning = false;
    }
    // Single sequence
    else if (BSP_PB0 == BSP_PB_PRESSED)
    {
        while (BSP_PB0 == BSP_PB_PRESSED);

        // Turn on single sequence mode
        SingleSeqOneShot = true;
    }
}

// Handle input from joystick while halted
void HandleHaltModeInput()
{
    uint8_t dir;

    // Run/Halt
    if (BSP_PB1 == BSP_PB_PRESSED)
    {
        while (BSP_PB1 == BSP_PB_PRESSED);

        IsRunning = true;

        ApplyMenuSettings();
    }
    // Single sequence
    else if (BSP_PB0 == BSP_PB_PRESSED)
    {
        while (BSP_PB0 == BSP_PB_PRESSED);

        // Turn on single sequence mode
        SingleSeqOneShot = true;
    }
    // Joystick navigation
    else if (IsJoystickInputEnabled())
    {
        dir = GetJoystickDirection();

        switch (dir)
        {
        // "Center"/Menu button
        case JOYSTICK_C:
            // Wait for user to release joystick
            while (GetJoystickDirection() != JOYSTICK_NONE);

            // Open the menu
            IsMenuActive = true;
            break;
        // "Up"
        case JOYSTICK_W:
            if (TriggerLevel < (TPOS_MAX - TRIGGER_INC_SIZE))
            {
                TriggerLevel += TRIGGER_INC_SIZE;
            }
            break;
        // "Down"
        case JOYSTICK_E:
            if (TriggerLevel > TRIGGER_INC_SIZE)
            {
                TriggerLevel -= TRIGGER_INC_SIZE;
            }
            break;
        // "Left"
        case JOYSTICK_S:
            if (HorizontalPosX >= HPOS_INC_SIZE)
            {
                HorizontalPosX -= HPOS_INC_SIZE;
            }
            break;
        // "Right"
        case JOYSTICK_N:
            if (HorizontalPosX <= (HPOS_MAX - HPOS_INC_SIZE))
            {
                HorizontalPosX += HPOS_INC_SIZE;
            }
            break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Draw Functions
/////////////////////////////////////////////////////////////////////////////

void DrawSplash()
{
    uint8_t y;

    for (y = 0; y < DISP_HEIGHT; y++)
    {
        DISP_WriteLine(y, (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, SI_SEG_GENERIC))&splash_bits[y * DISP_BUF_SIZE]);
    }
}

// Draw the menu system
//
// - Display: Vectors/Dots
// - Labels:  On/Off
// - Mode:    Auto/Normal
// - Slope:   Rising/Falling
// - Input:   Joystick/P1.1
// - Rate:    31.25k/62.5k/125k/250k/500k
void DrawMenu()
{
    uint8_t i;
    uint8_t y;
    uint8_t activeMenuItemX = 0;

    for (y = 0; y < DISP_HEIGHT - 8; y++)
    {
        for (i = 0; i < DISP_BUF_SIZE; i++)
        {
            Line[i] = 0x00;
        }

        RENDER_VerticalStrLine(Line, 0, y, (SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))MenuTextDisplay[DisplayType]);
        RENDER_VerticalStrLine(Line, 10, y, (SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))MenuTextShowLabels[ShowLabels]);
        RENDER_VerticalStrLine(Line, 20, y, (SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))MenuTextTriggerMode[TriggerMode]);
        RENDER_VerticalStrLine(Line, 30, y, (SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))MenuTextTriggerSlope[TriggerSlope]);
        RENDER_VerticalStrLine(Line, 40, y, (SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))MenuTextInput[AnalogInput]);
        RENDER_VerticalStrLine(Line, 50, y, (SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))MenuTextSampleRate[SampleRate]);

        DISP_WriteLine((DISP_HEIGHT-1) - FONT_MAP_SPACING - y, Line);
    }

    activeMenuItemX = ActiveMenuItem * 10;

    for (y = 0; y < FONT_MAP_SPACING; y++)
    {
        for (i = 0; i < DISP_BUF_SIZE; i++)
        {
            Line[i] = 0x00;
        }
        RENDER_VerticalStrLine(Line, activeMenuItemX, y, "*");
        DISP_WriteLine((DISP_HEIGHT-1) - y, Line);
    }
}

// Draw ADC samples on the LCD
//
// Smoothing algorithm:
// For each column, draw all pixels from the last point to the current point
//
//    +
//   + +
//   |  |
//  +   +
//  |    |
//  |    |
// +     +
//
// + - ADC sample
// | - Line from previous sample
//
void DrawWaveform()
{
    uint8_t i;
    uint8_t j;
    uint8_t sample;
    uint8_t lastSample;
    uint8_t row;
    SI_VARIABLE_SEGMENT_POINTER(SamplePtr, uint8_t, ADC_BUFFER_SEG);

    // No zoom
    if (SampleRate != RATE_500KX2)
    {
        SamplePtr = AdcSamples + HorizontalPosX;
    }
    // Scale HPOS for 2x digital zoom
    else
    {
        // Scale HPOS from [0, 128] to [0, 192], since we only display
        // half the number of samples (64 instead of 128)
        SamplePtr = AdcSamples + ((uint16_t)HorizontalPosX*3/2);
    }

    // Plot each ADC sample value
    // (LCD orientation is rotated 90 degrees for easier rendering)
    for (i = 0; i < DISP_HEIGHT; i++)
    {
        // Invert row number:
        // Draw x =   0 at row = 127 (bottom)
        // Draw x = 127 at row =   0 (top)
        row = (DISP_HEIGHT-1) - i;

        for (j = 0; j < DISP_BUF_SIZE; j++)
        {
            Line[j] = 0x00;
        }

        // No zoom
        if (SampleRate != RATE_500KX2)
        {
            // Scale 8-bit sample to a value between 0 and 127
            sample = SamplePtr[i] / 2;
        }
        // Interpolate samples for 2x digital zoom
        else
        {
            // Display ADC sample
            if ((i % 2) == 0)
            {
                // Scale 8-bit sample to a value between 0 and 127
                sample = SamplePtr[i/2] / 2;
            }
            // Interpolate sample by averaging current and next sample
            else
            {
                // Handle the interpolated sample after the last ADC sample,
                // where there is no next sample
                if (HorizontalPosX == HPOS_MAX && i == (DISP_HEIGHT - 1))
                {
                    // Repeat the last ADC sample for the interpolated sample
                    sample = (SamplePtr[i/2]) / 2;
                }
                else
                {
                    // Interpolate the samples between actual ADC samples
                    sample = ((uint16_t)SamplePtr[i/2] + SamplePtr[(i+1)/2]) / 2 / 2;
                }
            }
        }

        if (i == 0)
        {
            // First sample doesn't have a previous sample,
            // so set it equal to itself.
            lastSample = sample;
        }

        if (DisplayType == DISPLAY_TYPE_VECTORS)
        {
            if (sample > lastSample)
            {
                lastSample++;
            }
            else if (sample < lastSample)
            {
                lastSample--;
            }

            // Invert column number:
            // Draw y =   0 (0.0V) at pixel 127 (right)
            // Draw y = 127 (3.3V) at pixel   0 (left)
            RENDER_LineSegmentLine(Line, (DISP_HEIGHT-1) - lastSample, (DISP_HEIGHT-1) - sample);
        }
        else
        {
            // Invert column number:
            // Draw y =   0 (0.0V) at pixel 127 (right)
            // Draw y = 127 (3.3V) at pixel   0 (left)
            RENDER_PixelLine(Line, (DISP_HEIGHT-1) - sample);
        }

        // Draw dotted trigger level line
        if ((i % 4) == 0)
        {
            RENDER_PixelLine(Line, (DISP_HEIGHT-1) - ((uint8_t)(TriggerLevel / 8)));
        }

        if (ShowLabels == SHOW_LABELS_ON)
        {
            // Render 3.3V
            if (i < RENDER_GetStrSize(LABEL_V_MAX))
            {
                RENDER_VerticalStrLine(Line, 0, i, LABEL_V_MAX);
            }

            // Render 0
            if (i < RENDER_GetStrSize(LABEL_ORIGIN))
            {
                RENDER_VerticalStrLine(Line, DISP_WIDTH - FONT_HEIGHT, i, LABEL_ORIGIN);
            }

            // Render window period (128us - 4096us)
            if (row < RENDER_GetStrSize((SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))MenuTextWindowPeriod[SampleRate]))
            {
                RENDER_VerticalStrLine(Line, DISP_WIDTH - FONT_HEIGHT, RENDER_GetStrSize((SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))MenuTextWindowPeriod[SampleRate]) - 1 - row, (SI_VARIABLE_SEGMENT_POINTER(, char, RENDER_STR_SEG))MenuTextWindowPeriod[SampleRate]);
            }
        }

        DISP_WriteLine(row, Line);

        lastSample = sample;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////////////////////////////

// Delay until start of next frame
void SynchFrame()
{
    static uint16_t lastTick = 0;
    uint16_t tick;

    // Render at 30 Hz
    while (((tick = GetTickCount()) - lastTick) < HZ_TO_MS(FRAME_RATE));
    lastTick = tick;
}

// Apply default settings
void Oscilloscope_Init()
{
    ApplyMenuSettings();

    // Display the splash screen with instructions
    DrawSplash();

    // Wait for user to press a push button
    while (BSP_PB1 == BSP_PB_UNPRESSED && BSP_PB0 == BSP_PB_UNPRESSED);

    // Wait for user to release both push buttons
    while (BSP_PB1 == BSP_PB_PRESSED || BSP_PB0 == BSP_PB_PRESSED);
}

// Call from main loop
void Oscilloscope_main()
{
    HandleInput();

    // Menu
    if (IsMenuActive)
    {
        RGB_SetColor(COLOR_RED, 32);

        DrawMenu();
    }
    // Single Sequence
    else if (SingleSeqOneShot)
    {
        RGB_SetColor(COLOR_BLUE, 32);

        SingleSeqOneShot = false;
        IsRunning = false;

        if (IsCaptureInProgress())
        {
            AbortCapture();
        }

        TriggerCapture();

        while (IsCaptureInProgress())
        {
            DrawWaveform();
            SynchFrame();

            // Press PB0/PB1 to abort single sequence
            if (BSP_PB0 == BSP_PB_PRESSED || BSP_PB1 == BSP_PB_PRESSED)
            {
                while (BSP_PB0 == BSP_PB_PRESSED || BSP_PB1 == BSP_PB_PRESSED);
                AbortCapture();

                // Restart capture after aborting single sequence
                IsRunning = true;
            }
        }
    }
    // Running
    else if (IsRunning)
    {
        RGB_SetColor(COLOR_GREEN, 32);

        if (IsCaptureInProgress())
        {
            // In normal mode, keep waiting for the conversion to complete
            // and display the intermediate status of the waveform (usually
            // the last waveform captured)
            if (TriggerMode == TRIGGER_MODE_NORMAL)
            {
                DrawWaveform();
                SynchFrame();
                return;
            }
            // In auto mode:
            // If the capture hasn't finished by the next frame start time,
            // then force a auto-capture (ignore trigger)
            else
            {
                AbortCapture();
                ForceCapture();

                // Wait until capture is complete
                while (IsCaptureInProgress());
            }
        }

        DrawWaveform();
        TriggerCapture();
    }
    // Halted
    else
    {
        RGB_SetColor(COLOR_RED, 32);

        if (IsCaptureInProgress())
        {
            AbortCapture();
        }

        DrawWaveform();
    }

    SynchFrame();
}
