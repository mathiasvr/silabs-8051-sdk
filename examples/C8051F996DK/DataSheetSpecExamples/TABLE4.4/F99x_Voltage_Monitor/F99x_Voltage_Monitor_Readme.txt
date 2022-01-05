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
in Table 4.4 of the datasheet. This program also demonstrates the VDD Supply 
Monitor Threshold listed in Table 4.4 of the datasheet. The supply monitor is 
enabled at the start of the program and switch SW0_2 is used to toggle the 
enable bit. The power consumption can be tested by measuring the current of 
both modes and taking the difference. The supply monitor is selected as a reset
source when the supply monitor is enabled. When the supply monitor is disabled,
it is deselected as a reset source.

Resources Used:
--------------------------------------------------------------------------------
WDT0  (Watchdog Timer - Disabled)
VDMON (Voltage Supply Monitor)
P0.2  (SW0_2 - Voltage Supply Enable/Disable)
P1.5  (LED1.5 - VDD above monitor warning threshold ON/OFF)

Part Family:
--------------------------------------------------------------------------------
C8051F99x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.1

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.4 VDD Monitor Supply Current 
   - No Test Conditions Specified
      + Min: --
      + Typ: 7  uA
      + Max: --

Table 4.4 VDD/DC+ Monitor Threshold  (VREST)
	- No Test Conditions Specified
      + Min: 1.7 V
      + Typ: 1.75 V
      + Max: 1.8 V
   
Notes On Example and Modes:
--------------------------------------------------------------------------------
VDMON On Mode (Default): 
   PROSC divided by 1
   VDMEN enabled and selected as reset source
   P0.2 set as open-drain input
   P1.5 set as push pull output

VDMON Off Mode:
   PROSC divided by 1
   VDMEN disabled
   P0.2 set as open-drain input
   P1.5 set as push pull output
   
Note: SW0_2 (P0.2) is used to switch between the modes. 

How to Use:
--------------------------------------------------------------------------------
See top of F99x_Voltage_Monitor.c.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -22 OCT 2013

---eof--------------------------------------------------------------------------
