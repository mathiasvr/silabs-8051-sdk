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
This program demonstrates the Low Frequency Oscillator (LFOSC) power consumption
and oscillator frequency listed in Tables 1.2 and 1.5 of the datasheet, 
respectively. For power consumption measurements, the high frequency oscillator
(HFOSC) divided by 128 is selected as the system clock while the LFOSC operates 
as a peripheral at 80 kHz. The LFOSC is enabled at the start of the program and
the processor is put into idle mode. The processor is woken when a port match 
interrupt occurs. When the processor is woken, the LFOSC enable is toggled and 
the processor returns to an idle state. The processor is kept in an idle state 
to aid in measuring the low current of the LFOSC. During oscillator frequency 
measurements, the LFOSC is selected as the system clock and routed to an 
external pin.

Resources Used:
--------------------------------------------------------------------------------
HFOSC
LFOSC
PORT0
PORT1
WDT0  (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F85x/86x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.2 Power Consumption - Low Frequency Oscillator
   - Operating at 80 KHz, T_A = 25 C
      + Min: --
      + Typ: 3.5 uA
      + Max: --

Table 1.5 Internal Oscillators - Low Frequency Oscillator (80 kHz)
   - Full Temperature and Supply Range
      + Min: 75 kHz
      + Typ: 80 kHz
      + Max: 85 kHz

Notes On Example and Modes:
--------------------------------------------------------------------------------
- Low Frequency Oscillator Measure Current (Default): 
   + HFOSC
   + LFOSC
   + PORT1

- Low Frequency Oscillator Measure Frequency:
   + LFOSC
   + PORT0

How to Use:
--------------------------------------------------------------------------------
Refer to F85x_LF_Oscillator.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (BGD)
   -17 JUL 2013

---eof--------------------------------------------------------------------------
