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
P0.2 (SW0.2 - Oscillator Mode Cycle Switch)
P0.3 (SW0.3 - Measurement Mode Cycle Switch)
RTC  (SmaRTClock - Real Time Clock)

Product Family:
--------------------------------------------------------------------------------
C8051F99x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.1

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.2 Digital Core (SYSCLK) - Normal Mode
   - Frequency = 24.5 MHz (Precision Oscillator)
      + Min: --
      + Typ: 3.6 mA
      + Max: 4.5 mA
   
   - Frequency = 20.0 MHz (Low-Power Oscillator)
      + Min: --
      + Typ: 3.1 mA
      + Max: --

   - Frequency = 32.768 kHz (SmaRTClock Real Time Clock)
      + Min: --
      + Typ: 84 uA
      + Max: --

Notes On Example and Modes:
--------------------------------------------------------------------------------
Available Oscillator Settings:
   - 24.5 MHz (High Precision internal Oscillator) (Default Mode)
      + HFOSC (Divided by 1)
   - 20.0 MHz (Low-Power Frequency Oscillator)
      + LFOSC (Divided by 1)
   - 32.768 kHz (SmaRTClock Oscillator)
      + SMARTC (Divided by 1)
	 
Available Measurement Settings:
   - Oscilloscope Mode (P0.0 Enabled)
   - Current Mode (P0.0 Disabled)

Note: Accurate current readings cannot be made while using oscilloscope mode,
      because a significant current draw is made by the P0.0 push-pull output.
  
How to Use:
--------------------------------------------------------------------------------
Refer to F99x_Clock_Normal.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 28 OCT 2013

---eof--------------------------------------------------------------------------

