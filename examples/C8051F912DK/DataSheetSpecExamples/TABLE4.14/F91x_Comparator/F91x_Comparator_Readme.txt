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
This example demonstrates the power consumption for Comparator's different modes
on datasheet for the Comparator in Table 4.14, in this program SW0_2 to switch 
Comparator work mode. push SW0_3 to cycle Comparator ON/OFF.

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
Table 4.14 Comparator Supply Current
   - Mode 0
      + Min: --
      + Typ: 23  uA
      + Max: --
      
    - Mode 1
      + Min: --
      + Typ: 8.8  uA
      + Max: --
      
    - Mode 2
      + Min: --
      + Typ: 2.6  uA
      + Max: --
   
    - Mode 3
      + Min: --
      + Typ: 0.4  uA
      + Max: --
      
Notes On Example and Modes:
--------------------------------------------------------------------------------
   
How to Use:
--------------------------------------------------------------------------------
Refer to F91x_Comparator.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   - 26 OCT 2013

---eof--------------------------------------------------------------------------
