//-----------------------------------------------------------------------------
// log.h
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//

#ifndef LOG_H_
#define LOG_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "log_config.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define RECOVERY 0
#define NORMAL 1

#define PAGE_SIZE 512

typedef struct LOG_T
{
    int16_t celsiusTemperature;
    uint8_t percentHumidity;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} LOG;

typedef union LOG_U
{
    LOG log;
    uint8_t c[6];
} LOG_U;

#define HUMIDITY        2
#define HOUR            3
#define MINUTE          4
#define SECONDE         5

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
// Store the latest measurement result
extern LOG currentMeasurement;

// Current log size in FLASH
extern uint8_t currentLogSize;

// Address in FLASH of the next address to store a entry.
extern uint16_t nextAddr;

// The last entry number log that prints on LCD for Logger mode
extern uint8_t endEntryNum;

// Array of temperature or humidity values scaled to LCD y locations
extern SI_SEGMENT_VARIABLE(scaleArray[MAX_NUM_ENTRY], uint8_t, SI_SEG_IDATA);

extern uint8_t maxHumidity, minHumidity;
extern uint16_t maxTemperature, minTemperature;

// This bit determines if need to clear LCD
extern bool fullDisplay;
//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
//void Log_Main();
void UpdateCurrentMeasurement (void);
void DisplayLogger(uint8_t endLogNumToPrint);
void displayGraph(void);
void DisplayWaveform(void);
void GetRecord (uint16_t* addr, LOG_U* record);
void AddRecord (LOG_U* entry);

#endif /* LOG_H_ */