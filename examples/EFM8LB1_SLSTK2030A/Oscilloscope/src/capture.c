/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// capture.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "InitDevice.h"
#include "capture.h"

/////////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////////

SI_SEGMENT_VARIABLE(AdcSamples[ADC_BUFFER_SIZE], uint8_t, ADC_BUFFER_SEG);
volatile SI_VARIABLE_SEGMENT_POINTER(AdcSamplesPtr, uint8_t, ADC_BUFFER_SEG) = AdcSamples;
volatile bool CaptureInProgress = false;
volatile LEVEL_THR CompareLevel = LEVEL_THR_LT;
volatile TRIGGER_SLOPE TriggerSlope = TRIGGER_SLOPE_RISING;

uint16_t TriggerLevel = 512; // Default trigger level (L / 1024 * 3.3 V)

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

static void EnableWindowCompareIRQ(LEVEL_THR level, uint16_t threshold);
static SI_FUNCTION_PROTO_USING(EnableWindowCompareIRQ_2, void, (LEVEL_THR level, uint16_t threshold), 2);

/////////////////////////////////////////////////////////////////////////////
// Static Functions
/////////////////////////////////////////////////////////////////////////////

// Configure the threshold and level.
// Enable window compare interrupts.
static void EnableWindowCompareIRQ(LEVEL_THR level, uint16_t threshold)
{
    uint8_t SFRPAGE_save = SFRPAGE;

    SFRPAGE = LEGACY_PAGE;

    if (level == LEVEL_THR_LT)
    {
        // Setup window compare for less than threshold
        ADC0GT = 0xFFFF;
        ADC0LT = (threshold >> 2)+1;
    }
    else
    {
        // Setup window compare for greater than threshold
        ADC0GT = threshold >> 2;
        ADC0LT = 0xFFFF;
    }

    // Enable ADC window compare interrupt
    // for trigger level detect
    ADC0CN0_ADWINT = 0;
    EIE1 |= EIE1_EWADC0__BMASK;

    SFRPAGE = SFRPAGE_save;
}

// Configure the threshold and level.
// Enable window compare interrupts.
// (Use register bank 2)
//
// NOTE: Functions/ISRs that use a non-default register bank must
// call functions that also use the same register bank otherwise
// the "Don't use absolute register accesses" compiler option must be used
//
// See http://www.keil.com/support/docs/598.htm for more information
static SI_FUNCTION_USING(EnableWindowCompareIRQ_2, void, (LEVEL_THR level, uint16_t threshold), 2)
{
    uint8_t SFRPAGE_save = SFRPAGE;

    SFRPAGE = LEGACY_PAGE;

    if (level == LEVEL_THR_LT)
    {
        // Setup window compare for less than threshold
        ADC0GT = 0xFFFF;
        ADC0LT = (threshold >> 2)+1;
    }
    else
    {
        // Setup window compare for greater than threshold
        ADC0GT = threshold >> 2;
        ADC0LT = 0xFFFF;
    }

    // Enable ADC window compare interrupt
    // for trigger level detect
    ADC0CN0_ADWINT = 0;
    EIE1 |= EIE1_EWADC0__BMASK;

    SFRPAGE = SFRPAGE_save;
}

/////////////////////////////////////////////////////////////////////////////
// Global Functions
/////////////////////////////////////////////////////////////////////////////

// Return true if capture is currently in progress
bool IsCaptureInProgress()
{
    return CaptureInProgress;
}

// Enable trigger detect to start ADC capture
void TriggerCapture()
{
    IE_EA = 0;

    CaptureInProgress = true;

    // Reset ADC sample buffer
    AdcSamplesPtr = AdcSamples;

    if (TriggerSlope == TRIGGER_SLOPE_RISING)
    {
        CompareLevel = LEVEL_THR_LT;
    }
    else
    {
        CompareLevel = LEVEL_THR_GT;
    }

    // Setup window compare level and threshold
    EnableWindowCompareIRQ(CompareLevel, TriggerLevel);

    IE_EA = 1;
}

// Start capturing samples (ignoring trigger level)
void ForceCapture()
{
    uint8_t SFRPAGE_save;

    IE_EA = 0;

    CaptureInProgress = true;

    SFRPAGE_save = SFRPAGE;
    SFRPAGE = LEGACY_PAGE;
    
    // Turn on ADC interrupts
    ADC0CN0_ADINT = 0;
    EIE1 |= EIE1_EADC0__BMASK;

    SFRPAGE = SFRPAGE_save;
    
    IE_EA = 1;
}

// Stop a capture still in progress.
// Turn off ADC and window compare interrupts
void AbortCapture()
{
    uint8_t SFRPAGE_save;
  
    IE_EA = 0;

    SFRPAGE_save = SFRPAGE;
    SFRPAGE = LEGACY_PAGE;
    
    // Turn off ADC window compare interrupts
    EIE1 &= ~EIE1_EWADC0__BMASK;

    // Turn off ADC interrupts
    EIE1 &= ~EIE1_EADC0__BMASK;
    
    SFRPAGE = SFRPAGE_save;

    CaptureInProgress = false;

    IE_EA = 1;
}

//-----------------------------------------------------------------------------
// ADC0EOC_ISR
//-----------------------------------------------------------------------------
//
// ADC0EOC ISR Content goes here. Remember to clear flag bits:
// ADC0CN0::ADINT (Conversion Complete Interrupt Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT_USING(ADC0EOC_ISR, ADC0EOC_IRQn, 1)
{
    uint8_t SFRPAGE_save;

    ADC0CN0_ADINT = 0;

    // Read 8-bit ADC value
    // 10-bit ADC value is right shifted twice by hardware
    *AdcSamplesPtr = ADC0L;
    AdcSamplesPtr++;

    // Capture complete/buffer is full
    if (AdcSamplesPtr == AdcSamples + ADC_BUFFER_SIZE)
    {
        SFRPAGE_save = SFRPAGE;
        SFRPAGE = LEGACY_PAGE;

        // Turn off ADC interrupts
        EIE1 &= ~EIE1_EADC0__BMASK;

        SFRPAGE = SFRPAGE_save;

        CaptureInProgress = false;
    }
}

//-----------------------------------------------------------------------------
// ADC0WC_ISR
//-----------------------------------------------------------------------------
//
// ADC0WC ISR Content goes here. Remember to clear flag bits:
// ADC0CN0::ADWINT (Window Compare Interrupt Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT_USING(ADC0WC_ISR, ADC0WC_IRQn, 2)
{
    uint8_t SFRPAGE_save;

    ADC0CN0_ADWINT = 0;

    // Start capturing ADC samples on the correct rising/falling edge
    if ((TriggerSlope == TRIGGER_SLOPE_RISING && CompareLevel == LEVEL_THR_GT) ||
        (TriggerSlope == TRIGGER_SLOPE_FALLING && CompareLevel == LEVEL_THR_LT))
    {
        // Read 8-bit ADC value
        // 10-bit ADC value is right shifted twice by hardware
        *AdcSamplesPtr = ADC0L;
        AdcSamplesPtr++;

        SFRPAGE_save = SFRPAGE;
        SFRPAGE = LEGACY_PAGE;

        // Turn off ADC window compare interrupts
        EIE1 &= ~EIE1_EWADC0__BMASK;

        // Turn on ADC interrupts
        ADC0CN0_ADINT = 0;
        EIE1 |= EIE1_EADC0__BMASK;

        SFRPAGE = SFRPAGE_save;
    }
    else
    {
        // Toggle window compare for next interrupt
        CompareLevel = (LEVEL_THR)!CompareLevel;
        EnableWindowCompareIRQ_2(CompareLevel, TriggerLevel);
    }
}
