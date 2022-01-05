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
This example demonstrates how to check the Current Reference full scale error
and Absolute current error in source current mode on datasheet for the IREF0
in Table 4.13, in this program SW0_2 to switch current reference output with
different mode. The default 10K Ohm resistor(R14) that between P0.7 and GND
should be replaced with 4.7K Ohm to support high current full scale output
504uA, in this mode, the voltage should be 2.4V.

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
C8051F91x/90x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.2

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.13 IREF0 source mode
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
Refer to F91x_IREF0_Error.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -26 OCT 2013

---eof--------------------------------------------------------------------------
