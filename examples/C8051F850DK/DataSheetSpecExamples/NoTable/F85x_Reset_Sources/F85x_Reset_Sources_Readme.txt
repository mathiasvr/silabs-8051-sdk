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
This example code demonstrates the use of various reset sources avaliable to
the F85x and F86x MCUs.

Resources Used:
--------------------------------------------------------------------------------
WDT0   (Watchdog Timer - Enabled in certain configurations)
VMON0  (Voltage Supply Monitor - Enabled in certain configurations)
CMP0   (Comparator0 - Enabled in certain configurations)
P0.0   (Comparator0 - Negative Input (CP0N.0))
LDO    (Comparator0 - Positive Internal Input)
LFOSC0 (Low-Frequency Oscillator - Enabled as peripheral if using WDT0)
P1.0   (LED1 - Device Awaiting Reset Status Monitor)
P1.1   (LED2 - Power Supply Reset Status Monitor)

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5 6/13

Specs Demonstrated:
--------------------------------------------------------------------------------
This code example implements the functionality of various MCU features. As
such, it does not explicitly demonstrate any numerical specifications.

Notes On Example and Modes:
--------------------------------------------------------------------------------
Available Reset Source Modes:
   - Voltage Supply Monitor
      + HFOSC (Divided by 1)
      + VMON0
   - Missing Clock Detector
      + LFOSC (Divided by 128)
   - Watchdog Timer
      + HFOSC (Divided by 1)
      + WDT0
   - Software Reset
      + HFOSC (Divided by 1)
   - Comparator0 Reset
      + HFOSC (Divided by 1)
      + CMP0


NOTE: Modes are selected by modifying a constant preprocessor definition prior
      to compilation. See the "How to Use" section for more information.
   
How to Use:
--------------------------------------------------------------------------------
Refer to F85x_Reset_Sources.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (PMD)
	- 2 JUL 2013

---eof--------------------------------------------------------------------------

