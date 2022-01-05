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
This program demonstrates how to check the Current Reference full scale error 
and Absolute current error in source current mode on datasheet for the IREF0 
in Table 4.15, in this program SW3 to switch current reference output with
different mode. 

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
Table 4.15 IREF0 source mode
   - Full scale error (Low current)
      + Min: --
      + Typ: --
      + Max: +/- 5%
      
   - Full scale error (High current)
      + Min: --
      + Typ: --
      + Max: +/- 6%
      
   - Absolute current error (low power 20 uA)
      + Min: --
      + Typ: +/- 1%
      + Max: +/- 3%
   
Notes On Example and Modes:
--------------------------------------------------------------------------------
High Current full scale Mode (Default): 
   LPOSC divided by 1
   IREF set to 504 uA
   P0.7 set as analog output

Low Current full scale Mode : 
   lPOSC divided by 1
   IREF set to 63 uA
   P0.7 set as analog output

Absolute Current Mode : 
   lPOSC divided by 1
   IREF set to 20 uA (Low power)
   P0.7 set as analog output
   
How to Use:
--------------------------------------------------------------------------------
Refer to F96x_IREF0_Error.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -11 OCT 2013

---eof--------------------------------------------------------------------------
