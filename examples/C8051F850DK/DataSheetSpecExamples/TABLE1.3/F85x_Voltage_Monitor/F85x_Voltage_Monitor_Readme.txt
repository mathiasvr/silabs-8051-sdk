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
in Table 1.2 of the datasheet. This program also demonstrates the VDD Supply 
Monitor Threshold listed in Table 1.3 of the datasheet. The supply monitor is 
enabled at the start of the program and switch 1 (S1) is used to toggle the 
enable bit. The supply monitor is selected as a reset source when the it is 
enabled.

Resources Used:
--------------------------------------------------------------------------------
HFOSC
PORT1
VDMON
WDT0  (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F85x/86x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.2 Power Consumption - Voltage Supply Monitor (VMON0)
   - Min: --
   - Typ: 15 uA
   - Max: 25 uA

Table 1.3 Reset and Supply Monitor - VDD Supply Monitor Threshold
   - Min: 1.85 V
   - Typ: 1.95 V
   - Max: 2.1 V
   
Notes On Example and Modes:
--------------------------------------------------------------------------------
- VDMON On (Default): 
   + HFOSC
   + PORT1
   + VMON0

- VDMON Off:
   + HFOSC
   + PORT1
   + VMON0 (Disabled)

How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Voltage_Monitor.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (BGD)
   -17 JUL 2013

---eof--------------------------------------------------------------------------
