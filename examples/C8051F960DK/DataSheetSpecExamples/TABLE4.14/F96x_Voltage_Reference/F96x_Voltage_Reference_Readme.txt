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
voltage of the ADC internal voltage reference given in Table 4.14 of the 
datasheet for the Normal and Low Power mode settings. In this program, the ADC 
reference is set to a default Normal mode . SW1 to switch to the Low Power 
mode setting. SW2 to turn on/off voltage Reference .

Resources Used:
--------------------------------------------------------------------------------
ADC0
LPOSC
PORT0
PORT1
TMR2
WDT0 (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F96x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.14 Voltage Reference - Output Voltage (Full Temperature and Supply Range)
   - No special condition
      + Min: 1.62 V
      + Typ: 1.65 V
      + Max: 1.68 V
   
Table 4.12 Voltage Reference - Supple current
   - Normal mode
      + Min: --
      + Typ: 260  uA
      + Max: --
      
   - Low Power mode
      + Min: --
      + Typ: 140  uA
      + Max: --
       
Notes On Example and Modes:
--------------------------------------------------------------------------------
Voltage Reference Normal Mode (Default): 
   LPOSC divided by 1
   ADC enabled, internal VREF set to 1.65V
   P0.6 set as analog input

Voltage Reference Low Power mode:
   lPOSC divided by 1
   ADC enabled, internal VREF set to 1.65V
   P0.6 set as analog input

How to Use:
--------------------------------------------------------------------------------
Refer to F96x_Voltage_Reference.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -30 OCT 2013

---eof--------------------------------------------------------------------------
