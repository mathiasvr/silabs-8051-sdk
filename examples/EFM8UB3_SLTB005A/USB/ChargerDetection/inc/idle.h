/////////////////////////////////////////////////////////////////////////////
// idle.h
/////////////////////////////////////////////////////////////////////////////

#ifndef __IDLE_H__
#define __IDLE_H__
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <si_toolchain.h>
#include <stdint.h>
#include <stdbool.h>

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////
#define DEFAULT_IDLE_RATE       500     // Rate defined in the HID class doc.
#define POLL_RATE               24      // The bInterval reported with the
                                        // interrupt IN endpoint descriptor.

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef struct
{
  uint8_t rate;
  uint16_t timer;
} idleTimer_TypeDef;

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////
void idleTimerStart ( void );
bool isIdleTimerIndefinite( void );
bool isIdleTimerExpired ( void );
void idleTimerSet ( uint8_t rate );
void idleTimerTick ( void );
uint8_t idleGetRate ( void );

#endif
