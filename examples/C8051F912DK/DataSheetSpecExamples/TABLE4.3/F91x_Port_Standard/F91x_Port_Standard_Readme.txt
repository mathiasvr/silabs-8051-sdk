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
This program demonstrates the Port I/O voltage levels listed in Table 4.3 of
the datasheet. Port 0 is used for low drive port output voltage tests. Port 1
is used for high drive port output voltage tests and input voltage level tests.
DS3 and DS4 are setup as visual aids for the input voltage level tests.

Resources Used:
--------------------------------------------------------------------------------
PROSC
PORT0
PORT1
WDT0 (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F91x/90x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.2

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.3 Port I/O - Output High Voltage (High Drive)
   - IOH = -3 mA
      + Min: VDD – 0.7 V
      + Typ: --
      + Max: --

   - IOH = -10 uA
      + Min: VDD – 0.1 V
      + Typ: --
      + Max: --

   - IOH = -10 mA
      + Min: --
      + Typ: See Chart
      + Max: --

Table 4.3 Port I/O - Output High Voltage (Low Drive)
   - IOH = -1 mA
      + Min: VDD – 0.7 V
      + Typ: --
      + Max: --

   - IOH = -10 uA
      + Min: VDD – 0.1 V
      + Typ: --
      + Max: --

   - IOH = -3 mA
      + Min: --
      + Typ: See Chart
      + Max: --

Table 4.3 Port I/O - Output Low Voltage (High Drive)
   - IOH = 8.5 mA
      + Min: --
      + Typ: --
      + Max: 0.6 V

   - IOH = 10 uA
      + Min: --
      + Typ: --
      + Max: 0.1 V

   - IOH = 25 mA
      + Min: --
      + Typ: See Chart
      + Max: --

Table 4.3 Port I/O - Output Low Voltage (Low Drive)
   - IOH = 1.4 mA
      + Min: --
      + Typ: --
      + Max: 0.6 V

   - IOH = 10 uA
      + Min: --
      + Typ: --
      + Max: 0.1 V

   - IOH = 4 mA
      + Min: --
      + Typ: See Chart
      + Max: --

Table 4.3 Port I/O - Input High Voltage
   - VDD = 2.0 to 3.6 V
      + Min: VDD – 0.6 V
      + Typ: --
      + Max: --

Table 4.3 Port I/O - Input Low Voltage
   - VDD = 2.0 to 3.6 V
      + Min: --
      + Typ: --
      + Max: 0.6 V
	
Notes On Example and Modes:
--------------------------------------------------------------------------------
- Port Standard:
   + PROSC
   + Port I/O

How to Use:
--------------------------------------------------------------------------------
Refer to F91x_Port_Standard.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -22 OCT 2013

---eof--------------------------------------------------------------------------
