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
This program demonstrates the response time with different mode on datasheet
for the Comparator in Table 4.14, in this program SW0_2 to switch
Comparator work mode. push SW0_3 to cycle CP0 Negative Input VDD/GND.

Resources Used:
--------------------------------------------------------------------------------
LPOSC
PORT0
PORT1
TIMER0
CP0
WDT0 (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F91x/90x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.2

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.14 Response time
   - Mode 0, CP0+ – CP0– = 100 mV
      + Min: --
      + Typ: 130  ns
      + Max: --

   - Mode 0, CP0+ – CP0– = -100 mV
      + Min: --
      + Typ: 200  ns
      + Max: --

   - Mode 1, CP0+ – CP0– = 100 mV
      + Min: --
      + Typ: 210  ns
      + Max: --

    - Mode 1, CP0+ – CP0– = -100 mV
      + Min: --
      + Typ: 410  ns
      + Max: --

    - Mode 2, CP0+ – CP0– = 100 mV
      + Min: --
      + Typ: 420  ns
      + Max: --

    - Mode 2, CP0+ – CP0– = -100 mV
      + Min: --
      + Typ: 1200 ns
      + Max: --

    - Mode 3, CP0+ – CP0– = 100 mV
      + Min: --
      + Typ: 1750 ns
      + Max: --

    - Mode 3, CP0+ – CP0– = -100 mV
      + Min: --
      + Typ: 6200 ns
      + Max: --


Notes On Example and Modes:
--------------------------------------------------------------------------------

How to Use:
--------------------------------------------------------------------------------
Refer to F91x_Comparator_Response.c for detailed directions on using this
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   - 27 OCT 2013

---eof--------------------------------------------------------------------------
