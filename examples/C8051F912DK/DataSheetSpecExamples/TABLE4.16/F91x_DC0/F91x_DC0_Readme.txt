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
This example demonstrates the Output Voltage range, Current ,Power consumption
and clocking frequency on datasheet for the DC-DC in Table 4.16, in this program
SW0_2 to switch Output voltage Range.

Resources Used:
--------------------------------------------------------------------------------
LPOSC
PORT0
PORT1
WDT0 (Disabled)
DC0

Part Family:
--------------------------------------------------------------------------------
C8051F91x/90x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.2

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.16 DC-DC Output Voltage Range
   - Target Output 1.8 V
      + Min: 1.73  V
      + Typ: 1.80  V
      + Max: 1.87  V
      
   - Target Output 1.9 V
      + Min: 1.83  V
      + Typ: 1.90  V
      + Max: 1.97  V
      
   - Target Output 2.0 V
      + Min: 1.93  V
      + Typ: 2.00  V
      + Max: 2.07  V
      
   - Target Output 2.1 V
      + Min: 2.03  V
      + Typ: 2.10  V
      + Max: 2.17  V  
      
   - Target Output 2.4 V
      + Min: 2.30  V
      + Typ: 2.40  V
      + Max: 2.50  V  
      
  - Target Output 2.7 V
      + Min: 2.60  V
      + Typ: 2.70  V
      + Max: 2.80  V      
      
  - Target Output 3.0 V
      + Min: 2.90  V
      + Typ: 3.00  V
      + Max: 3.10  V       
      
   - Target Output 3.3 V
      + Min: 3.18  V
      + Typ: 3.30  V
      + Max: 3.42  V         

Table 4.16 DC-DC Output Current
   - Target Output 1.8 V
      + Min: --    
      + Typ: --
      + Max: 36    mA
      
   - Target Output 1.9 V
      + Min: --    
      + Typ: --
      + Max: 34    mA
      
   - Target Output 2.0 V
      + Min: --    
      + Typ: --
      + Max: 32    mA
      
   - Target Output 2.1 V
      + Min: --    
      + Typ: --
      + Max: 30    mA
      
   - Target Output 2.4 V
      + Min: --    
      + Typ: --
      + Max: 27    mA
      
  - Target Output 2.7 V
      + Min: --    
      + Typ: --
      + Max: 24    mA
      
  - Target Output 3.0 V
      + Min: --    
      + Typ: --
      + Max: 21    mA
      
   - Target Output 3.3 V
      + Min: --    
      + Typ: --
      + Max: 19    mA
      
Table 4.16 DC-DC Output Power      
      + Min: --    
      + Typ: --
      + Max: 65    mW

Table 4.16 DC-DC Clocking frequency 
      + Min: 1.6   MHz    
      + Typ: 2.4   MHz
      + Max: 3.2   MHz
      
Notes On Example and Modes:
--------------------------------------------------------------------------------
   
How to Use:
--------------------------------------------------------------------------------
Refer to F91x_DC0.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -02 NOV 2013

---eof--------------------------------------------------------------------------
