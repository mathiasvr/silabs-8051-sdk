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
internal oscillator frequencies in idle mode. A switch with port match
interrupts enabled will wake the processor, switch mode, then put the processor
back to sleep when pushed. Each operating mode represents a different oscillator
frequency.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.2 (SW0.2 - Oscillator Mode Cycle Switch)

Product Family:
--------------------------------------------------------------------------------
C8051F99x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.1 

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.2 Digital Core (SYSCLK) - Idle Mode
   - Frequency = 24.5 MHz (PROSC)
      + Min: --
      + Typ: 1.8 mA
      + Max: 3.0 mA
   
   - Frequency = 20.0 MHz (LPOSC)
      + Min: --
      + Typ: 1.4 mA
      + Max: --

   - Frequency = 32.768 kHz (SMARTC)
      + Min: --
      + Typ: 82 uA
      + Max: --

Notes On Example and Modes:
--------------------------------------------------------------------------------
Available Oscillator Settings:
   - 24.5 MHz (Precision internal Oscillator) (Default Mode)
      + PROSC (Divided by 1)
   - 20.0 MHz (lOW POWER Frequency Oscillator)
      + LPOSC(Divided by 1)
   - 32.768 kHz (SmaRTClock Oscillator)
      + SMARTC (Divided by 1)
	 
  
How to Use:
--------------------------------------------------------------------------------
Refer to F99x_Clock_Idle.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (PD)
	- 28 OCT 2013

---eof--------------------------------------------------------------------------

