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
This program demonstrates the Voltage Reference power consumption and output
voltage of the ADC internal voltage reference given in Table 4.11 of the
datasheet for the 1.65V and 1.68V settings. In this program, the ADC reference
is set to a default 1.65V. SW0_2 to switch to the 1.68V setting. SW0_3 to
turn on/off voltage Reference .

Resources Used:
--------------------------------------------------------------------------------
ADC0
LPOSC
PORT0
PORT1
TIMER2
WDT0 (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F93x/92x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.3

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.11 Voltage Reference - Output Voltage (Full Temperature and Supply Range)
   - High Speed Setting
      + Min: 1.60 V
      + Typ: 1.65 V
      + Max: 1.70 V

   - Precision Setting
      + Min: 1.645 V
      + Typ: 1.680  V
      + Max: 1.715 V

Notes On Example and Modes:
--------------------------------------------------------------------------------
Voltage Reference High Speed Mode (Default):
   LPOSC divided by 1
   ADC enabled, internal VREF set to 1.65V
   P0.6 set as analog input

Voltage Reference 1.65V Mode:
   lPOSC divided by 1
   ADC enabled, internal VREF set to 1.68V
   P0.6 set as analog input

How to Use:
--------------------------------------------------------------------------------
Refer to F93x_Voltage_Reference.c for detailed directions on using this
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -10 OCT 2013

---eof--------------------------------------------------------------------------
