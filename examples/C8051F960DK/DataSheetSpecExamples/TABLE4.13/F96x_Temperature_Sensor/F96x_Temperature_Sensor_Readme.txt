--------------------------------------------------------------------------------
 Copyright 2014 Silicon Laboratories, Inc.
 All rights reserved. This program and the accompanying materials
 are made available under the terms of the Silicon Laboratories End User 
 License Agreement which accompanies this distribution, and is available at
 http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 Original content and implementation provided by Silicon Laboratories.
--------------------------------------------------------------------------------

Program Description:
--------------------------------------------------------------------------------
This example code demonstrates the Temperature Sensor power consumption and 
performance, such as Offset , Offset Error, temperature. A UART connection 
from the MCU is required to print test result in UART terminal.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
ADC0
Temperature Sensor
P0.4 (UTX - UART Transmit Pin)
P0.5 (URX - UART Receive Pin)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.13 Temperature Sensor
   - Linearity
      + Min: --
	  + Typ: +/- 1 degree Celsius
	  + Max: --
  	  
   - Offset 
      + Min: --
	  + Typ: 1025 mV (Temp 25 C)
	  + Max: --

   - Offset Error
      + Min: --
	  + Typ: 18 mV
	  + Max: --
  
   - Supply Current
      + Min: --
	  + Typ: 35 uA
	  + Max: --
      
Notes On Example and Modes:
--------------------------------------------------------------------------------
   
F96x ADC Settings:
   - Number of ADC Samples: 64 (Default)
   - ADC Sampling Rate: 10 kHz in 10-Bit Mode
   		 
How to Use:
--------------------------------------------------------------------------------
Refer to F96x_Temperature_Sensor.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 18 OCT 2013

---eof--------------------------------------------------------------------------

