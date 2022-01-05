/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "ProjectConfig.h"
#include "CSLibrary.h"
#include "HardwareConfig.h"

#include "SI_C8051F990_Register_Enums.h"




//-----------------------------------------------------------------------------
// Local function prototypes
//-----------------------------------------------------------------------------
void ConfigureCS0ActiveMode(void);
void ConfigurePortsActiveMode(void);
//-----------------------------------------------------------------------------
// SetMux
//-----------------------------------------------------------------------------
//
// Low level access to the CS0MUX register.  This is called by ScanSensor().
//
void SetMux(uint8_t mux_value)
{
   CS0MX = mux_value;
}

//-----------------------------------------------------------------------------
// SetGain
//-----------------------------------------------------------------------------
//
// Low level access to the gain bits.  This is called by ScanSensor().
//
void SetGain(uint8_t gain_value)
{
   CS0MD1 = 0x07 & gain_value;         // clear all bits except gain bits
}

//-----------------------------------------------------------------------------
// ReadGain
//-----------------------------------------------------------------------------
//
// Returns the gain bits of the sensor.  
//
uint8_t ReadGain(void)
{
   return CS0MD1 & 0x07;
}

//-----------------------------------------------------------------------------
// ReadMux
//-----------------------------------------------------------------------------
//
// Returns the mux value from the sensor peripheral.
//
uint8_t ReadMux(void)
{
   return CS0MX;
}

//-----------------------------------------------------------------------------
// SetAccumulation
//-----------------------------------------------------------------------------
//
// Sets the accumulation bits of the sensor register.
//
void SetAccumulation(uint8_t accumulation)
{
   CS0CF = 0x07 & accumulation;        // Clear all bits except accumulation
}


//-----------------------------------------------------------------------------
// ExecuteConversion
//-----------------------------------------------------------------------------
//
// Assumes that the performance characteristics of the sensor have already
// been configured.  Enables the sensor, starts a scan, blocks until
// the scan is complete.
// Note that this version of ExecuteConversion() allows for a retrieval
// of sensor data from the serial port through a call to get_U16().  This
// is a compile-time switch.
//
uint16_t ExecuteConversion(void)
{
 
      SI_UU16_t ScanResult;

      CS0CN = 0x88;                       // Enable CS0, Enable Digital Comparator

      CS0CN &= ~0x20;                     // Clear the CS0 INT flag

      CS0CN |= 0x10;                      // Set CS0BUSY to begin conversion

      while (!(CS0CN & 0x20));            // Wait in foreground


      ScanResult.u8[MSB] = CS0DH;         // Read Result
      ScanResult.u8[LSB] = CS0DL;
   
      CS0CN = 0x00;                       // Disable CS0

      return ScanResult.u16;

   
}




//-----------------------------------------------------------------------------
// ConfigureSensorForActiveMode
//-----------------------------------------------------------------------------
//
// This is a top-level call to configure the sensor to its operational state
// during active mode.
//
void ConfigureSensorForActiveMode(void)
{
   ConfigurePortsActiveMode();
   ConfigureCS0ActiveMode();
}

//-----------------------------------------------------------------------------
// ScanSensor
//-----------------------------------------------------------------------------
//
// This is a top-level function that assumes there is a sensor node struct
// with CS0-related controls.  This is called by the library to execute
// one scan of one sensor node.  Note that the sensor output is returned
// and not saved to buffers in this routine.  Saving is the responsibility
// of the library routines.
//
uint16_t ScanSensor(uint8_t node_index)
{
   SetMux(tech_spec[node_index].mux);
   SetGain(tech_spec[node_index].gain);
   SetAccumulation(tech_spec[node_index].accumulation);
   return ExecuteConversion();
}




//-----------------------------------------------------------------------------
// Local hardware access functions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// ConfigureCS0ActiveMode
//-----------------------------------------------------------------------------
//
// This is a low-level local routine to handle configuration of the CS0 block.
//
void ConfigureCS0ActiveMode(void)
{
	
   CS0CF = 0x00;                         // MODE: CS0BUSY
   CS0MD2 &= 0xC0;                       // 12-bit mode
   CS0MD2 |= 0x40;
   CS0THH = 0;
   CS0THL = 0;

   CS0CN = 0x88;                       // Enable CS0, Enable Digital Comparator

   CS0CN &= ~0x20;

   CS0CN |= 0x10;                      // Set CS0BUSY to begin conversion
   while (!(CS0CN & 0x20));            // Wait in foreground
   
   CS0CN = 0x00;                       // Disable CS0

}


//-----------------------------------------------------------------------------
// ConfigurePortsActiveMode
//-----------------------------------------------------------------------------
//
// This is a low-level local routine to handle configuration of the ports
// for active mode.
//
void ConfigurePortsActiveMode(void)
{
   P0MDIN &= ~0x08;            // Set TOUCH_SENSE_SWITCH P0.3 to analog
}



void TechSpecificNodeInit(uint8_t sensor_index)
{
  tech_spec[sensor_index].gain = gain_values[sensor_index];
  tech_spec[sensor_index].mux = mux_values[sensor_index];
  tech_spec[sensor_index].accumulation = accumulation_values[sensor_index];
}
