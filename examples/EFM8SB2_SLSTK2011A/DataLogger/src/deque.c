//-----------------------------------------------------------------------------
// deque.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"
#include "log.h"
#include "deque.h"

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Routines
// ----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// UpdateMaxAndMin
//---------------------------------------------------------------------------
void UpdateMaxAndMin(LOG_U* entry)
{
    uint16_t currentTemp;
    currentTemp = (entry->c[MSB]);
    currentTemp = (currentTemp << 8) | (entry->c[LSB]);

    // Update Max and Min Temperature
    if (currentTemp > maxTemperature)
    {
        maxTemperature = currentTemp;
    }

    if (currentTemp < minTemperature)
    {
        minTemperature = currentTemp;
    }

    // Update Max and Min Humidity
    if ((entry->c[HUMIDITY]) > maxHumidity)
    {
        maxHumidity = (entry->c[HUMIDITY]);
    }

    if ((entry->c[HUMIDITY]) < minHumidity)
    {
        minHumidity = (entry->c[HUMIDITY]);
    }
}

//---------------------------------------------------------------------------
// ScaleLocation
//---------------------------------------------------------------------------
//
// Description - this function scales each measurement value in RAMLog into a LCD y value.
// Note- It uses this formula to do the scaling:
//
//              (b-a)(x - min)
//      f(x) = --------------  + a
//                max - min
//
// RAMLog - the deque that holds the most recent [ARRAY_SIZE] number of readings.
// array - the array that is to hold the scaled LCD y values of each measurement value in RAMLog.
// max - the max measurement value that should be displayed onto the LCD to auto scale the plot.
// min - the min measurement value that should be displayed onto the LCD to auto scale the plot.
// measurementSelect - determines whether to scale humidity or temperature data.
//
//
void ScaleLocation(void)
{
    uint8_t i;

    LOG_U prevRecord;
    uint16_t getAddr = LOG_START_ADDR;

    for(i = 0; i < currentLogSize; i++)
    {
        // The getAddr will be updated after reading data from Flash
        GetRecord(&getAddr, &prevRecord);

        if(displayItem == HUMIDITY)
        {
            scaleArray[i] = ConvertToPixelLocation(prevRecord.log.percentHumidity, maxHumidity, minHumidity);
        }
        else
        {
            scaleArray[i] = ConvertToPixelLocation(prevRecord.log.celsiusTemperature, maxTemperature, minTemperature);
        }
    }

}

//---------------------------------------------------------------------------
// ConvertToPixelLocation
//---------------------------------------------------------------------------
//
// Description - this function implements this following equation:
//
//              (b-a)(x - min)
//      f(x) = --------------  + a
//                max - min
//
// x - the value to convert.
// maxValue - the max measurement value that should be displayed onto the LCD to auto scale the plot.
// minValue - the min measurement value that should be displayed onto the LCD to auto scale the plot.
//
//
uint8_t ConvertToPixelLocation(int16_t x, int16_t maxValue, int16_t minValue)
{
    int32_t pixelLocation = (((int32_t)(B - A) * (x - minValue))/ (maxValue - minValue)) + A;
    return (uint8_t) pixelLocation;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------