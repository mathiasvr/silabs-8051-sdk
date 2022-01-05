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
This program demonstrates the Voltage Supply Monitor power consumption listed 
in Table 4.6 of the datasheet. This program also demonstrates the VDD Supply 
Monitor Threshold listed in Table 4.6 of the datasheet. The supply monitor is 
enabled at the start of the program and switch SW2 is used to toggle the 
enable bit. The power consumption can be tested by measuring the current of 
both modes and taking the difference. The supply monitor is selected as a reset
source when the supply monitor is enabled. When the supply monitor is disabled,
it is deselected as a reset source.The supply monitor can be chosen digital or 
analog mode through set #define ANALOG_MODE. 

Resources Used:
--------------------------------------------------------------------------------
WDT0  (Watchdog Timer - Disabled)
VDMON (Voltage Supply Monitor)
P0.1  (SW2 - Voltage Supply Enable/Disable)
P0.1  (LED1 - VDD above monitor warning threshold ON/OFF)

Part Family:
--------------------------------------------------------------------------------
C8051F96x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.6 Power Consumption - Voltage Monitor Supply Current (VDMEN)
   - Digital or Analog
      + Min: --
      + Typ: 14 uA
      + Max: --

Table 4.6 Reset and Supply Monitor - VDD Supply Monitor Threshold
	- No Test Conditions Specified
      + Min: 1.7 V
      + Typ: 1.75 V
      + Max: 1.8 V
   
Notes On Example and Modes:
--------------------------------------------------------------------------------
VDMON On Mode (Default): 
   PROSC divided by 1
   VDMEN enabled and selected as reset source
   P0.0 set as open-drain input
   P0.1 set as push pull output

VDMON Off Mode:
   HFOSC divided by 1
   VDMEN disabled
   P0.0 set as open-drain input
   P0.1 set as push pull output
   
SW2 (P0.1) is used to switch between the monitor on / off. 
set #define ANALOG_MODE to switch digital an analog mode.

How to Use:
--------------------------------------------------------------------------------
See top of F96x_Voltage_Monitor.c.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision
   -04 NOV 2013

---eof--------------------------------------------------------------------------
