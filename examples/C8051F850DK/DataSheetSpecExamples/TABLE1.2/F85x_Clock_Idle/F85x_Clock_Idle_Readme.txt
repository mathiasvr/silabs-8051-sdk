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
This example code demonstrates the power consumption and use of different 
internal oscillator frequencies in idle operation. A switch with port match
interrupts enabled will wake the processor, switch mode, then put the processor
back to sleep when pushed. Each operating mode represents a different oscillator
frequency.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P1.7 (S1 - Oscillator Mode Cycle Switch)

Product Family:
--------------------------------------------------------------------------------
C8051F85x/86x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.2 Digital Core (SYSCLK) - Idle Mode
   - Frequency = 24.5 MHz (HFOSC)
      + Min: --
      + Typ: 2.05 mA
      + Max: --
   - Frequency = 1.53 MHz (HFOSC)
      + Min: --
      + Typ: 550 uA
      + Max: --
   - Frequency = 24.5 MHz (LFOSC)
      + Min: --
      + Typ: 125 uA
      + Max: --

Notes On Example and Modes:
--------------------------------------------------------------------------------
Available Oscillator Settings:
   - 24.5 MHz (High Frequency Oscillator) (Default Mode)
      + HFOSC (Divided by 1)
   - 1.53 MHz (High Frequency Oscillator)
      + HFOSC (Divided by 16)
   - 80 kHz  (Low Frequency Oscillator)
      + HLOSC (Divided by 1)
	 
How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Clock_Idle.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (PMD)
	- 25 JUNE 2013

---eof--------------------------------------------------------------------------

