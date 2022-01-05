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
This program demonstrates the Port I/O voltage levels listed in Table 1.11 of 
the datasheet. Port 0 is used for low drive port output voltage tests. Port 1 
is used for high drive port output voltage tests and input voltage level tests.
DS5 and DS6 are setup as visual aids for the input voltage level tests.

Resources Used:
--------------------------------------------------------------------------------
HFOSC
PORT0
PORT1
WDT0 (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F85x/86x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.11 Port I/O - Output High Voltage (High Drive)
   - IOH = -3 mA
      + Min: VDD – 0.7 V
      + Typ: --
      + Max: --

Table 1.11 Port I/O - Output Low Voltage (High Drive)
   - IOH = 8.5 mA
      + Min: --
      + Typ: --
      + Max: 0.6 V
      
Table 1.11 Port I/O - Output High Voltage (Low Drive)
   - IOH = -1 mA
      + Min: VDD – 0.7 V
      + Typ: --
      + Max: --

Table 1.11 Port I/O - Output Low Voltage (Low Drive)
   - IOH = 1.4 mA
      + Min: --
      + Typ: --
      + Max: 0.6 V

Table 1.11 Port I/O - Input High Voltage
   - No Test Conditions Specified
      + Min: VDD – 0.6 V
      + Typ: --
      + Max: --

Table 1.11 Port I/O - Input Low Voltage
   - No Test Conditions Specified
      + Min: --
      + Typ: --
      + Max: 0.6 V
   
Notes On Example and Modes:
--------------------------------------------------------------------------------
- Port Standard: 
   + HFOSC
   + PORT0
   + PORT1
   
How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Port_Standard.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (BGD)
   -16 JUL 2013

---eof--------------------------------------------------------------------------
