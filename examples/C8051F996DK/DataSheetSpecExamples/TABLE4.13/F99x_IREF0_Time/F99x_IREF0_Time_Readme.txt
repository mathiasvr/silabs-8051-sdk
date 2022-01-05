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
This program demonstrates the Dynamic Performance of IREF on datasheet for
the IREF0 in Table 4.13,in this program SW0_2 to switch Reference ON/OFF. 


Resources Used:
--------------------------------------------------------------------------------
LPOSC
PORT0
PORT1
TIMER0
IREF0
WDT0 (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F99x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.1

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.13 IREF Dynamic Performance  
   - Output Settling Time to 1/2 LSB
      + Min: --
      + Typ: 300  ns
      + Max: --
      
   - Startup Time
      + Min: --
      + Typ: 1    us
      + Max: --
 
      
Notes On Example and Modes:
--------------------------------------------------------------------------------
Low Power Maximum Mode (Default): 
   LPOSC divided by 1
   IREF0DAT set to 111111
   P0.7 analog  IERF0
   
How to Use:
--------------------------------------------------------------------------------
Refer to F99x_IREF0_Time.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -31 OCT 2013

---eof--------------------------------------------------------------------------
