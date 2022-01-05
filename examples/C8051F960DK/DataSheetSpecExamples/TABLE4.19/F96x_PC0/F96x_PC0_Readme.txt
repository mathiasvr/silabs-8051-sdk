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
This example code demonstrates the power consumption of Pulse Counter and a 
simple button debounce function that Pulse Counter works in sleep mode. The 
PC0 comparator match event will wake the processor and toggled LED1 state, then
put the processor back to sleep.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P1.0 (SW4 - PC0 analog input)
P0.0 (LED1 - digital output)

Product Family:
--------------------------------------------------------------------------------
C8051F96x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.19 Supply Current 2ms sample rate
   - VBAT 1.8 V
      + Min: --
      + Typ: 145 uA
      + Max: --	
   
   - VBAT 2.2 V
      + Min: --
      + Typ: 175 uA
      + Max: --	
 
   - VBAT 3.0 V
      + Min: --
      + Typ: 235 uA
      + Max: --	
      
   - VBAT 3.8 V
      + Min: --
      + Typ: 285 uA
      + Max: --	      
      
      
How to Use:
--------------------------------------------------------------------------------
Refer to F96x_PC0.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 08 NOV 2013

---eof--------------------------------------------------------------------------

