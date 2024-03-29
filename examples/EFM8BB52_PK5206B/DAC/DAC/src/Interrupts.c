//=========================================================
// src/Interrupts.c: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================
         
// USER INCLUDES			
#include <SI_EFM8BB52_Register_Enums.h>

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK          24500000       // SYSCLK frequency in Hz

#define SAMPLE_RATE_DAC 200000L        // DAC sampling rate in Hz

#define PHASE_PRECISION 65536          // Range of phase accumulator

#define FREQUENCY       1000L          // Frequency of output in Hz

#define PHASE_ADD       (FREQUENCY * PHASE_PRECISION / SAMPLE_RATE_DAC)

// A full cycle, 10-bit, unsigned sine wave lookup table
SI_SEGMENT_VARIABLE(SINE_TABLE[256], uint16_t, const SI_SEG_CODE) = 
{
  0x200,0x20C,0x219,0x225,0x232,0x23E,0x24B,0x257,
  0x263,0x270,0x27C,0x288,0x294,0x2A0,0x2AC,0x2B8,
  0x2C3,0x2CF,0x2DA,0x2E5,0x2F1,0x2FC,0x306,0x311,
  0x31C,0x326,0x330,0x33A,0x344,0x34E,0x357,0x360,
  0x369,0x372,0x37A,0x383,0x38B,0x393,0x39A,0x3A2,
  0x3A9,0x3B0,0x3B6,0x3BD,0x3C3,0x3C8,0x3CE,0x3D3,
  0x3D8,0x3DD,0x3E1,0x3E5,0x3E9,0x3EC,0x3F0,0x3F3,
  0x3F5,0x3F7,0x3F9,0x3FB,0x3FD,0x3FE,0x3FE,0x3FF,
  0x3FF,0x3FF,0x3FE,0x3FE,0x3FD,0x3FB,0x3F9,0x3F7,
  0x3F5,0x3F3,0x3F0,0x3EC,0x3E9,0x3E5,0x3E1,0x3DD,
  0x3D8,0x3D3,0x3CE,0x3C8,0x3C3,0x3BD,0x3B6,0x3B0,
  0x3A9,0x3A2,0x39A,0x393,0x38B,0x383,0x37A,0x372,
  0x369,0x360,0x357,0x34E,0x344,0x33A,0x330,0x326,
  0x31C,0x311,0x306,0x2FC,0x2F1,0x2E5,0x2DA,0x2CF,
  0x2C3,0x2B8,0x2AC,0x2A0,0x294,0x288,0x27C,0x270,
  0x263,0x257,0x24B,0x23E,0x232,0x225,0x219,0x20C,
  0x200,0x1F3,0x1E6,0x1DA,0x1CD,0x1C1,0x1B4,0x1A8,
  0x19C,0x18F,0x183,0x177,0x16B,0x15F,0x153,0x147,
  0x13C,0x130,0x125,0x11A,0x10E,0x103,0x0F9,0x0EE,
  0x0E3,0x0D9,0x0CF,0x0C5,0x0BB,0x0B1,0x0A8,0x09F,
  0x096,0x08D,0x085,0x07C,0x074,0x06C,0x065,0x05D,
  0x056,0x04F,0x049,0x042,0x03C,0x037,0x031,0x02C,
  0x027,0x022,0x01E,0x01A,0x016,0x013,0x00F,0x00C,
  0x00A,0x008,0x006,0x004,0x002,0x001,0x001,0x000,
  0x000,0x000,0x001,0x001,0x002,0x004,0x006,0x008,
  0x00A,0x00C,0x00F,0x013,0x016,0x01A,0x01E,0x022,
  0x027,0x02C,0x031,0x037,0x03C,0x042,0x049,0x04F,
  0x056,0x05D,0x065,0x06C,0x074,0x07C,0x085,0x08D,
  0x096,0x09F,0x0A8,0x0B1,0x0BB,0x0C5,0x0CF,0x0D9,
  0x0E3,0x0EE,0x0F9,0x103,0x10E,0x11A,0x125,0x130,
  0x13C,0x147,0x153,0x15F,0x16B,0x177,0x183,0x18F,
  0x19C,0x1A8,0x1B4,0x1C1,0x1CD,0x1DA,0x1E6,0x1F3,
};

//-----------------------------------------------------------------------------
// TIMER3_ISR
//-----------------------------------------------------------------------------
//
// TIMER3 ISR Content goes here. Remember to clear flag bits:
// TMR3CN::TF2H (Timer # High Byte Overflow Flag)
// TMR3CN::TF2L (Timer # Low Byte Overflow Flag)
//
// This routine changes the state of the LED whenever Timer3 overflows.
//
//-----------------------------------------------------------------------------

SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
{
  uint8_t SFRPAGE_save = SFRPAGE;

  static uint16_t phase_acc = 0;      // Holds phase accumulator

  SI_UU16_t temp;                     // The temporary value that holds
                                     // value before being written
                                     // to the DAC

  SI_VARIABLE_SEGMENT_POINTER(table_ptr, uint16_t, const SI_SEG_CODE);  // Pointer to the lookup table


  TMR3CN0 &= ~0x80;                   // Clear Timer3 overflow flag

  table_ptr = SINE_TABLE;

  phase_acc += PHASE_ADD;             // Increment phase accumulator


  // Read the table value
  temp.u16 = *(table_ptr + (phase_acc >> 8));

  // Set the value of <temp> to the next output of DAC at full-scale
  // amplitude. The rails are 0x000 and 0xFFF. DAC low byte must be
  // written first.

  SFRPAGE = PG4_PAGE;

  DAC0L = temp.u8[LSB];
  DAC0H = temp.u8[MSB];

  SFRPAGE = SFRPAGE_save;
}
