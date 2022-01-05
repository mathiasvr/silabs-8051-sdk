/////////////////////////////////////////////////////////////////////////////
// oscilloscope.h
/////////////////////////////////////////////////////////////////////////////

#ifndef OSCILLOSCOPE_H_
#define OSCILLOSCOPE_H_

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "oscilloscope_config.h"

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

#define LABEL_V_MAX             "3.3V"
#define LABEL_ORIGIN            "0"

#define TPOS_MAX                (1 << 10)
#define HPOS_MAX                (ADC_BUFFER_SIZE - DISP_WIDTH)

/////////////////////////////////////////////////////////////////////////////
// Enumerations
/////////////////////////////////////////////////////////////////////////////

typedef enum MENU_ITEM
{
    MENU_DISPLAY_TYPE,
    MENU_SHOW_LABELS,
    MENU_TRIGGER_MODE,
    MENU_TRIGGER_SLOPE,
    MENU_INPUT,
    MENU_SAMPLE_RATE,
    MENU_COUNT
} MENU_ITEM;

typedef enum DISPLAY_TYPE
{
    DISPLAY_TYPE_VECTORS,
    DISPLAY_TYPE_DOTS,
    DISPLAY_TYPE_COUNT
} DISPLAY_TYPE;

typedef enum SHOW_LABELS
{
    SHOW_LABELS_OFF,
    SHOW_LABELS_ON,
    SHOW_LABELS_COUNT
} SHOW_LABELS;

typedef enum TRIGGER_MODE
{
    TRIGGER_MODE_AUTO,
    TRIGGER_MODE_NORMAL,
    TRIGGER_MODE_COUNT
} TRIGGER_MODE;

typedef enum INPUT
{
    INPUT_JOY,
    INPUT_EXT,
    INPUT_COUNT
} INPUT;

typedef enum RATE
{
    RATE_31K25,
    RATE_62K5,
    RATE_125K,
    RATE_250K,
    RATE_500K,
    RATE_500KX2,
    RATE_COUNT
} RATE;

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

void Oscilloscope_Init();
void Oscilloscope_main();

#endif /* OSCILLOSCOPE_H_ */
