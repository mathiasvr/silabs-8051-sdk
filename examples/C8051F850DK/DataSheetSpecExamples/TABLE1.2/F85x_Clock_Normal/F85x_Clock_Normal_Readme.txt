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
internal oscillator frequencies in normal mode. This code example allows the
user to both measure the current consumption as well as monitor the oscillator
signal to confirm the oscillator frequency. In order to prevent the signal pin
from drawing additional current and clobbering measurements, there are two
measurement modes which separate runtime into a current mode and an
oscilloscope-friendly mode.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.0 (SYSCLK Output)
P1.7 (S1 - Oscillator Mode Cycle Switch)
P2.1 (S2 - Measurement Mode Cycle Switch)

Product Family:
--------------------------------------------------------------------------------
C8051F85x/86x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.2 Digital Core (SYSCLK) - Normal Mode
   - Frequency = 24.5 MHz (HFOSC)
      + Min: --
      + Typ: 4.45 mA
      + Max: --
   - Frequency = 1.53 MHz (HFOSC)
      + Min: --
      + Typ: 915 uA
      + Max: --
   - Frequency = 24.5 MHz (LFOSC)
      + Min: --
      + Typ: 250 uA
      + Max: --

Notes On Example and Modes:
--------------------------------------------------------------------------------
Available Oscillator Settings:
   - 24.5 MHz (High Frequency Oscillator) (Default Mode)
      + HFOSC (Divided by 1)
   - 1.53 MHz (High Frequency Oscillator)
      + HFOSC (Divided by 16)
   - 80 kHz  (Low Frequency Oscillator)
      + LFOSC (Divided by 1)
	 
Available Measurement Settings:
   - Oscilloscope Mode (P0.0 Enabled)
   - Current Mode (P0.0 Disabled)

Note: Accurate current readings cannot be made while using oscilloscope mode,
      because a significant current draw is made by the P0.0 push-pull output.
  
How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Clock_Normal.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (PMD)
	- 2 AUG 2013

---eof--------------------------------------------------------------------------

