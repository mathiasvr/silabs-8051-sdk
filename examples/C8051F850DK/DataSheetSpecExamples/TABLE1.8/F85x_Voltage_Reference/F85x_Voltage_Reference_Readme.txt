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
This program demonstrates the output voltage of the ADC internal voltage 
reference given in Table 1.8 of the datasheet for the 1.65V and 2.4V settings. 
The internal ADC reference cannot be measured directly; it is not routed to an 
external pin. Instead, the reference voltage can be measured by sweeping the 
input to the ADC. When the ADC is at the maximum reading (0x03FF for a 10 bit 
conversion), the LED labeled DS5 will turn on and the input voltage can be used
to determine the internal ADC reference voltage. Switch 1 is used to toggle the 
reference levels. The LED labeled DS6 will turn on when the reference level is 
set to 1.65V.

Resources Used:
--------------------------------------------------------------------------------
ADC0
HFOSC
PORT1
TMR2
WDT0 (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F85x/86x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.8 Voltage Reference - Output Voltage (Full Temperature and Supply Range)
   - 1.65V Setting
      + Min: 1.62 V
      + Typ: 1.65 V
      + Max: 1.68 V
   - 2.4V Setting
      + Min: 2.35 V
      + Typ: 2.4  V
      + Max: 2.45 V
   
Notes On Example and Modes:
--------------------------------------------------------------------------------
- Voltage Reference 2.4V Mode (Default): 
   + ADC0
   + HFOSC
   + PORT1
   + TMR2

- Voltage Reference 1.65V Mode:
   + ADC0
   + HFOSC
   + PORT1
   + TMR2

How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Voltage_Reference.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (BGD)
   -16 JUL 2013

---eof--------------------------------------------------------------------------
