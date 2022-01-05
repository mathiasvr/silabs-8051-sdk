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
This program demonstrates the power consumption of the Internal ADC Reference 
listed in Table 1.2 of the datasheet. The internal reference is enabled when it 
is selected as a voltage reference. Switch 1 (S1) triggers a port match 
interrupt event that toggles the voltage reference selection between VDD and the
internal voltage reference; The internal reference is selected by default. Low 
power mode is toggled by using switch 2 (S2).

Resources Used:
--------------------------------------------------------------------------------
ADC0
HFOSC
PORT1
PORT2
TMR2
WDT0  (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F85x/86x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.2 Power Consumption - Internal ADC0 Reference
   - Normal Power Mode
      + Min: --
      + Typ: 680 uA
      + Max: 750 uA
   - Low Power Mode
      + Min: --
      + Typ: 160 uA
      + Max: 190 uA
   
Notes On Example and Modes:
--------------------------------------------------------------------------------
- Normal Power Mode (Default): 
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2

- Low Power Mode:
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2
   
How to Use:
--------------------------------------------------------------------------------
Refer to F85x_ADC_Reference_Supply_Current.c for detailed directions on using 
this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (BGD)
   -17 JUL 2013

---eof--------------------------------------------------------------------------
