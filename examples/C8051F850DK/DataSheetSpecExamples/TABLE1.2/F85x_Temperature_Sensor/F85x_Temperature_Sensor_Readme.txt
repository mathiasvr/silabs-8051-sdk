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
This program demonstrates the Temperature Sensor power consumption listed 
in Table 1.2 of the data sheet. Additionally, this program demonstrates the
offset and slope characteristics of the Temperature Sensor in Table 1.9. There 
are three different runtime modes used to demonstrate these characteristics, 
which can be toggled via switch input.

Resources Used:
--------------------------------------------------------------------------------
WDT0   (Watchdog Timer - Disabled)
ADC0   (Analog-to-Digital Converter)
TSENSE (Temperature Sensor - ADC0 Component)
UART   (Universal Asynchronous Receiver/Transmitter)
P0.4   (UTX - UART Transmit Pin)
P1.7   (S1 - Mode Cycle Switch)

Part Family:
--------------------------------------------------------------------------------
C8051F85x/86x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.2 Power Consumption (TSENSE) - Temperature Sensor
      + Min: --   
	   + Typ: 75  uA
	   + Max: 105 uA
	  
Table 1.9 Temperature Sensor (TSENSE) - Voltage Offset
   - Temperature Sensor Setting = 0 °C
      + Min: --   
	   + Typ: 757 mV
	   + Max: --
	  
Table 1.9 Temperature Sensor (TSENSE) - Voltage Slope
      + Min: --   
	   + Typ: 2.85 mV / Deg. Celsius
	   + Max: --
   
Notes On Example and Modes:
--------------------------------------------------------------------------------
Available Temperature Sensor Settings:
   - TSENSE Test Mode 0 (Voltage Slope & Voltage Offset)
      + HFOSC (Divided by 1)
      + ADC0
      + Timer2
      + UART
   - TSENSE Test Mode 1 (Power Consumption - TSENSE ON)
      + HFOSC (Divided by 1)
      + ADC0
   - TSENSE Test Mode 2 (Power Consumption - TSENSE OFF)
      + HFOSC (Divided by 1)
   
Note: S1 (P1.7) is used to switch between the modes. 

How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Temperature_Sensor.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (PMD)
   -2 JUL 2013

---eof--------------------------------------------------------------------------
