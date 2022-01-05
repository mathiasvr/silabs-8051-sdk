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
This program demonstrates the CS0 module bias supply current and conversion 
time with different conversion bit length on datasheet for the CS0 in Table 
4.16, in this program SW0_2 to switch conversion bit mode. push SW0_3 to cycle
CS0 ON / OFF.

Resources Used:
--------------------------------------------------------------------------------
LPOSC
PORT0
PORT1
TIMER0
WDT0 (Disabled)
CS0

Part Family:
--------------------------------------------------------------------------------
C8051F99x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.1

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.16 Capacitive Sense power supply current
   -  Bias current
      + Min: --
      + Typ: 50  uA
      + Max: 60  uA
      
Table 4.16 Capacitive Sense conversion time
   - 12-bit mode
      + Min: 20   us
      + Typ: 25   us
      + Max: 40   us
      
   - 13-bit mode
      + Min: 21   us
      + Typ: 27   us
      + Max: 42.5 us
      
   - 14-bit mode
      + Min: 23   us
      + Typ: 29   us
      + Max: 45   us
      
   - 16-bit mode
      + Min: 26   us
      + Typ: 33   us
      + Max: 50   us
         
      
Notes On Example and Modes:
--------------------------------------------------------------------------------
Conversion channel P1.0 
    CS0MX = 0x08;                   
    P1.0 analog input
   
How to Use:
--------------------------------------------------------------------------------
Refer to F99x_CS0.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -01 NOV 2013

---eof--------------------------------------------------------------------------
