//-----------------------------------------------------------------------------
// log.c
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main.h"
#include "disp.h"
#include "render.h"
#include "log.h"
#include "smbus.h"
#include "demo_config.h"
#include "flash.h"
#include "deque.h"

// Standard library
#include <string.h>
#include "retargetserial.h"

//-----------------------------------------------------------------------------
// Globals Variables
//-----------------------------------------------------------------------------
// Store the latest measurement result
LOG currentMeasurement;

// Current log size in FLASH
uint8_t currentLogSize;
// Address in FLASH of the next address to store a entry.
uint16_t nextAddr  = LOG_START_ADDR;

// The last entry number of log that prints on LCD for Logger mode
uint8_t endEntryNum;

// Array of temperature or humidity values scaled to LCD y locations
uint8_t idata scaleArray[MAX_NUM_ENTRY];

uint8_t maxHumidity, minHumidity;
uint16_t maxTemperature, minTemperature;

// This bit determines if need to clear LCD
bool fullDisplay = false;

uint8_t logPageCount = 0;
//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
void UpdateCurrentMeasurement (void);
void DisplayLogger(uint8_t endLogNumToPrint);
void displayGraph(void);
void DisplayWaveform(void);
int16_t CelsiusToFahrenheit (int16_t celsiusVaue);
bool IsBetween(int16_t a, int16_t b, uint8_t x);
void PrintString(SI_VARIABLE_SEGMENT_POINTER(str, char, RENDER_STR_SEG), uint8_t posx, uint8_t posy);
void GetRecord (uint16_t* addr, LOG_U* record);
void AddRecord (LOG_U* entry);

//-----------------------------------------------------------------------------
// Routines
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// UpdateCurrentMeasurement
//---------------------------------------------------------------------------
//
// Description - measure current temperature and humidity.
//
//        125 * RH_Code
// %RH = --------------- - 6
//          65536
//
//                    175.72 * Temp_Code
// Temperature (C) = -------------------- - 46.85
//                         65536
//
void UpdateCurrentMeasurement (void)
{
    uint32_t calculationHolder;

    // Reset the Temperature & Humidity Sensor.
    //ResetSensor();

    calculationHolder = (((uint32_t)125) * MeasureAndReadHumidity());
    // Covert read value to percent humidity following formula in si7021 data sheet
    currentMeasurement.percentHumidity =  (uint8_t)((calculationHolder >> 16) - 6);

    // Covert read value to celsius temperature following formula in si7021 data sheet
    calculationHolder = (((uint32_t)17572) * ReadTemperature());
    calculationHolder = ((calculationHolder >> 16) - 4685);
    currentMeasurement.celsiusTemperature = (int16_t)(calculationHolder / 10);

    currentMeasurement.hours = hours;
    currentMeasurement.minutes = minutes;
    currentMeasurement.seconds = seconds;

    // Reset the Temperature & Humidity Sensor.
    //ResetSensor();

    // Insert the currentMeasurement to FLASH
    AddRecord((LOG_U*)&currentMeasurement);

    // Update Max and Min values
    UpdateMaxAndMin((LOG_U*)&currentMeasurement);
}
//-----------------------------------------------------------------------------
// Display Functions
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// DisplayLogger
//---------------------------------------------------------------------------
//
// Description - display Celsius, Fahrenheit, or Humidity data in logger form.
//
//
void DisplayLogger(uint8_t endLogNumToPrint)
{
    SI_SEGMENT_VARIABLE(tempStr[18], uint8_t, RENDER_STR_SEG);
    uint8_t sizeToPrint;
    uint8_t index, i;
    uint8_t posy = 0;
    int16_t tempData;
    LOG_U prevRecord;
    uint16_t getAddr;

    if((endLogNumToPrint == 0) || (endLogNumToPrint > MAX_NUM_ENTRY))
    {
        return;
    }

    // Set index to the most recent 16 samples.
    if(endLogNumToPrint >= LCD_PAGE_SIZE)
    {
        sizeToPrint = LCD_PAGE_SIZE;
        index = endLogNumToPrint - LCD_PAGE_SIZE;

        fullDisplay = true;
    }
    else
    {
        sizeToPrint = endLogNumToPrint;
        index = 0;

        if(fullDisplay == true)
        {
            // Clear LCD
            DISP_ClearAll();
            fullDisplay = false;
        }
    }

    getAddr = LOG_START_ADDR + index * LOG_ENTRY_SIZE;

    // This section of code prints out the logger based on SW1_State and sizeToPrint
    for(i = 0; i < sizeToPrint; i++)
    {
        GetRecord(&getAddr, &prevRecord);

        switch(displayItem)
        {
        case DISPLAY_ITEM_CELSIUS:
            tempData = prevRecord.log.celsiusTemperature;
            sprintf(tempStr, "%d.%d C, %um %us", tempData / 10, tempData % 10
                            , (uint16_t)(prevRecord.log.minutes)
                            , (uint16_t)(prevRecord.log.seconds));
            break;

        case DISPLAY_ITEM_FAHRENHEIT:
            tempData = CelsiusToFahrenheit(prevRecord.log.celsiusTemperature);
            sprintf(tempStr, "%d.%d F, %um %us", tempData / 10, tempData % 10
                            , (uint16_t)(prevRecord.log.minutes)
                            , (uint16_t)(prevRecord.log.seconds));
            break;

        case DISPLAY_ITEM_HUMIDITY:
            sprintf(tempStr, "%d%%, %um %us", (uint16_t)(prevRecord.log.percentHumidity)
                            , (uint16_t)(prevRecord.log.minutes)
                            , (uint16_t)(prevRecord.log.seconds));
            break;
        }
        PrintString(tempStr, 0, posy);
        posy += FONT_HEIGHT;
    }
}

//---------------------------------------------------------------------------
// DisplayGraph
//---------------------------------------------------------------------------
//
// Description - display the RAMLog of Celsius, Fahrenheit, or Humidity data in graph.
//
//
void displayGraph(void)
{
    int16_t min_value, max_value;

    SI_SEGMENT_VARIABLE(str[21], char, RENDER_STR_SEG);
    uint8_t index = currentLogSize - 1;

    int16_t tempData;

    // Displays the title and the most recent reading to the right of the title.
    // The posx constants passed to PrintString are so that the printed line is centered.
    switch(displayItem)
    {
        case DISPLAY_ITEM_CELSIUS:
        {
            min_value = minTemperature / 10;
            max_value = maxTemperature / 10 + 1;

            tempData = currentMeasurement.celsiusTemperature;
            sprintf(str, "Celsius %d.%d C", tempData / 10, tempData % 10);
        }
        break;
        case DISPLAY_ITEM_FAHRENHEIT:
        {
            min_value = CelsiusToFahrenheit(minTemperature) / 10;
            max_value = CelsiusToFahrenheit(maxTemperature) / 10 + 1;

            tempData = CelsiusToFahrenheit(currentMeasurement.celsiusTemperature);
            sprintf(str, "Fahrenheit %d.%d F", tempData / 10, tempData % 10);
        }
        break;
        case DISPLAY_ITEM_HUMIDITY:
        {
            min_value = (uint16_t)minHumidity;
            max_value = (uint16_t)maxHumidity;

            sprintf(str, "Humidity %d%%", (uint16_t)(currentMeasurement.percentHumidity));
        }
        break;
    }

    PrintString(str, 15, 0);

    // This prints the maximum and minimum value that could appear on the LCD on the side axis
    sprintf(str, "%d", max_value);
    PrintString(str, 0, 8);

    sprintf(str, "%d", min_value);
    PrintString(str, 0, 112);

    // Displays the bottom axis to the LCD.
    // The posx constants passed to PrintString are so that the printed line is centered.
    #if MEASUREMENT_INTERVAL == HOURS
    sprintf(str, "1 sample per hour");

    #elif MEASUREMENT_INTERVAL == MINUTES
    sprintf(str, "1 sample per minute");

    #elif MEASUREMENT_INTERVAL == SECONDS
    sprintf(str, "1 sample per sec");

    #else // MEASUREMENT_INTERVAL == TWO_SECONDS
    sprintf(str, "1 sample two secs");
    #endif

    PrintString(str, 15, 120);
}

//---------------------------------------------------------------------------
// DisplayWaveform
//---------------------------------------------------------------------------
//
// Description - displays data log on the LCD
//
//
void DisplayWaveform(void)
{
    uint8_t i;
    uint8_t j, index;
    uint8_t prevIndex;
    SI_SEGMENT_VARIABLE(line[DISP_BUF_SIZE], char, RENDER_LINE_SEG);

    // Print only the lines that the data locations should appear.
    // Since there is a title, maximum value on the top of the screen,
    // and minimum value, a bottom axis (each of height 8 pixels)
    // the range of LCD y location for data points is 16 - 111.
    for (i = 111; i >= 16; i--)
    {
        memset(line, 0x00, DISP_BUF_SIZE);

        index = 0;
        prevIndex = 0;

        // For every x location available on the LCD, plot a sensor reading value.
        // This algorithm also smoothes the jaggedness of the plot by drawing a vertical line
        // from the current location to the location of the previous point - 1.
        // This for loop starts at 15 because the side axis takes up the first 15 x-pixels of the LCD
        for (j = 15; j < currentLogSize + 15; j++)
        {
            // For the first point, only need to draw the dot
            // because there is no previous value before.
            if(scaleArray[index] == i)
            {
                RENDER_PixelLine(line, j);
            }
           else
            {
                // Render temperature into line buffer
                // This section implements the line smoothness. It checks to see if the current
                // and prev values are apart by more than 1 and checks if current line is between these
                // two values.
                if (index != 0)
                {
                    if(IsBetween(scaleArray[prevIndex], scaleArray[index], i))
                    {
                        RENDER_PixelLine(line, j);
                    }
                }
            }

            index ++;
            prevIndex = index - 1;
        }

        DISP_WriteLine(i, line);
    }
}

//---------------------------------------------------------------------------
// IsBetween
//---------------------------------------------------------------------------
//
// Description - determines whether x is between the values of a and b
//
// a - an endpoint value.
// b - other endpoint value.
// x - the value to check to see if in between a and b.
//
//
// return - returns true if x is between a and b, otherwise returns false.
bool IsBetween(int16_t a, int16_t b, uint8_t x)
{
    if (x <= b && x > a)
        return 1;

    if (x < a && x >= b)
        return 1;

    return 0;
}

//---------------------------------------------------------------------------
// CelsiusToFahrenheit
//---------------------------------------------------------------------------
//
// Description - this function implements this following equation:
//
//      f(x) = x * 9 / 5 + 320
//
int16_t CelsiusToFahrenheit (int16_t celsiusVaue)
{
    return (celsiusVaue * 9 / 5 + 320);
}

//---------------------------------------------------------------------------
// PrintString
//---------------------------------------------------------------------------
//
// Description - prints a string to the screen
//
// str - the string to print out in a char array
// posx - the x position on the LCD to start printing the string.
// posy - the y position on the LCD to start printing the string.
//
//
// return - returns true if x is between a and b, otherwise returns false.
void PrintString(SI_VARIABLE_SEGMENT_POINTER(str, char, RENDER_STR_SEG), uint8_t posx, uint8_t posy)
{
    uint8_t i;

    SI_SEGMENT_VARIABLE(line[DISP_BUF_SIZE], char, RENDER_LINE_SEG);

    for(i = 0; i < FONT_HEIGHT; i++)
    {
        RENDER_ClrLine(line);
        RENDER_StrLine(line, posx, i, str);
        DISP_WriteLine(posy, line);
        posy++;
    }
}

//---------------------------------------------------------------------------
// GetRecord
//---------------------------------------------------------------------------
//
// Description - this function retrieves a entry from FLASH and places it into a LOG variable.
//
// addr -the address to retrieve the entry.
// record - the LOG variable to store the entry data retrieved from FLASH.
//
void GetRecord (uint16_t* addr, LOG_U* record)
{
    uint8_t i;
    for (i = 0; i < LOG_ENTRY_SIZE; i++)
    {
        record->c[i] = FLASH_ByteRead(*addr);
        *addr += 1;
    }
}

//---------------------------------------------------------------------------
// AddRecord
//---------------------------------------------------------------------------
//
// Description - this function adds a LOG variable as an entry to the FLASH.
//
//
void AddRecord (LOG_U* entry)
{
    uint8_t i;

    // Erase Flash before writing
    if(nextAddr % PAGE_SIZE == 0 )
    {
        FLASH_PageErase(nextAddr);

        currentLogSize = 0;
        maxHumidity = minHumidity = 0;
        maxTemperature = minTemperature = 0;
    }

    for(i = 0; i < LOG_ENTRY_SIZE; i++)
    {
        // Store entry into FLASH and then increment nextAddr
        FLASH_ByteWrite(nextAddr, (uint8_t)(entry->c[i]));
        nextAddr++;
    }

    currentLogSize++;
    if(currentLogSize >= MAX_NUM_ENTRY)
    {
        nextAddr = LOG_START_ADDR;

        logPageCount++;
    }
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
