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
This program demonstrates the Power consumption of different Current Reference
mode and different IREF0DAT on datasheet for the IREF0 in Table 4.15, in this
program SW3 to switch current output mode. push SW4 to cycle Current
Reference ON / OFF.

Resources Used:
--------------------------------------------------------------------------------
LPOSC
PORT0
PORT1
TMR0
WDT0 (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F96x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.15 IREF0 Power Consumption (source mode)
   - Low Power 
      + Min: --
      + Typ: 10  uA
      + Max: --
      
   - Low Power 
      + Min: --
      + Typ: 10  uA
      + Max: --
      
   - High Current 
      + Min: --
      + Typ: 10  uA
      + Max: --
      
   - High Current 
      + Min: --
      + Typ: 10  uA
      + Max: --
      
Table 4.15 IREF0 Power Consumption (sink mode)
   - Low Power 
      + Min: --
      + Typ: 1   uA
      + Max: --
      
   - Low Power 
      + Min: --
      + Typ: 11  uA
      + Max: --
      
   - High Current 
      + Min: --
      + Typ: 12  uA
      + Max: --
      
   - High Current 
      + Min: --
      + Typ: 81  uA
      + Max: --
         
      
      
Notes On Example and Modes:
--------------------------------------------------------------------------------
Low Power Minimum Mode (Default): 
   LPOSC divided by 1
   IREF0DAT set to 000001

Low Power Maximum Mode : 
   LPOSC divided by 1
   IREF0DAT set to 111111
   
High Current Minimum Mode : 
   LPOSC divided by 1
   IREF0DAT set to 000001

High Current Maximum Mode : 
   LPOSC divided by 1
   IREF0DAT set to 111111
   
How to Use:
--------------------------------------------------------------------------------
Refer to F96x_IREF0_Power.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -05 NOV 2013

---eof--------------------------------------------------------------------------
