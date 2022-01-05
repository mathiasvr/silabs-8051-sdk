///////////////////////////////////////////////////////////////////////////////
// logic.h
///////////////////////////////////////////////////////////////////////////////

#ifndef LOGIC_H_
#define LOGIC_H_

///////////////////////////////////////////////////////////////////////////////
// Configuration
///////////////////////////////////////////////////////////////////////////////

// LCD refresh rate in Hz
#define DEMO_FRAME_RATE                50

///////////////////////////////////////////////////////////////////////////////
// Defines
///////////////////////////////////////////////////////////////////////////////

// Center column number for x-axis centering
#define X_CENTER                       128/2

// Center row number for y-axis centering
#define Y_CENTER                       128/2

// Beginning row number for input/output name and state labels
#define OUTPUT_LABEL_ROW               22
#define INPUT1_LABEL_ROW               9
#define INPUT2_LABEL_ROW               35

// Beginning column number for input name labels
#define OUTPUT_NAME_LABEL_COL          70
#define INPUT_NAME_LABEL_COL           40

// Beginning column number for input state labels
#define OUTPUT_STATE_LABEL_COL         118
#define INPUT_STATE_LABEL_COL          5

// Look up tables for logic functions
#define LUT_AND                        0xC0
#define LUT_NAND                       0x3F
#define LUT_OR                         0xFC
#define LUT_NOR                        0x03
#define LUT_XOR                        0x3C
#define LUT_XNOR                       0xC3

// Parameters for logic_write() and logic_read()
#define OUTPUT 0
#define INPUT1 1
#define INPUT2 2

#define FONT_SCALE_DEFAULT 1

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// logic_readState
//-----------------------------------------------------------------------------
//
// Returns pointer to state label. Used by addLabel()
//
// io - specifies input/output to retrieve. Valid arguments are:
//      OUTPUT, INPUT1, INPUT2
//
char * logic_readState(uint8_t io);

//-----------------------------------------------------------------------------
// logic_writeState
//-----------------------------------------------------------------------------
//
// Update the state labels
//
// io    - specifies input/output to update. Valid arguments are:
//         OUTPUT, INPUT1, INPUT2
// state - hex value 0x00 or 0x01 to indicate the input's or ouput's state
//
void logic_writeState(uint8_t io, uint8_t state);

//-----------------------------------------------------------------------------
// logic_main
//-----------------------------------------------------------------------------
//
// Main driver function containing demo loop
//
void logic_main(void);


#endif /* LOGIC_H_ */
