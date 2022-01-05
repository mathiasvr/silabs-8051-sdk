/////////////////////////////////////////////////////////////////////////////
// function_gerator.h
/////////////////////////////////////////////////////////////////////////////

#ifndef __FUNCTION_GENERATOR_H__
#define __FUNCTION_GENERATOR_H__

/////////////////////////////////////////////////////////////////////////////
// Configuration
/////////////////////////////////////////////////////////////////////////////

// LCD refresh rate in Hz
#define DEMO_FRAME_RATE                50

/////////////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////////////

// SYSCLK frequency in Hz
#define SYSCLK                         24500000

// DAC sampling rate in Hz
#define SAMPLE_RATE_DAC                60000L

// Range of phase accumulator
#define PHASE_PRECISION                65536

// Center column number for x-axis centering
#define X_CENTER                       128/2

// Beginning x and y position for waveform sprite
#define X_POS_WAVEFORM                 (X_CENTER - 88/2 - 1)
#define Y_POS_WAVEFORM                 4

// Beginning y position for frequency
#define Y_POS_FREQ                     108

// Beginning x and y position for nav left/right arrow sprites
#define X_POS_NAV_ARROW_LEFT           3
#define X_POS_NAV_ARROW_RIGHT          117
#define Y_POS_NAV_ARROW_LEFT_RIGHT     41

// Beginning x and y position for nav up/down arrow sprites
#define X_POS_NAV_ARROW_UP_DOWN        (X_CENTER - 4)
#define Y_POS_NAV_ARROW_UP             (Y_POS_FREQ - 7)
#define Y_POS_NAV_ARROW_DOWN           (Y_POS_FREQ + 10)

// Enum for demo states
typedef enum {
	DEMO_SINE,
	DEMO_SQUARE,
	DEMO_TRIANGLE,
	DEMO_SAWTOOTH,
	DEMO_WINDOWED_SINE
} DemoState;

// Splash timeout in ms
#define SPASH_TIMEOUT                  4000

// Enum for kill splash flag
typedef enum {
	KILL_SPLASH,
	SHOW_SPLASH
} KillSpash;

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

void       FunctionGenerator_main(void);

#endif /* __FUNCTION_GENERATOR_H__ */
