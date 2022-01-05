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
internal oscillator frequencies in Idle mode and Low Power Idle mode. A switch
with port match interrupts enabled will wake the processor, switch mode, then 
put the processor back to sleep when pushed. Each operating mode represents a 
different oscillator frequency.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.1 (SW2 - Oscillator Mode Cycle Switch)

Product Family:
--------------------------------------------------------------------------------
C8051F96x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.4 Digital Supply Current - Idle Mode 
   - Frequency = 24.5 MHz (PROSC)
      + Min: --
      + Typ: 3.5 mA
      + Max: --	 mA
   
   - Frequency = 20.0 MHz (LPOSC)
      + Min: --
      + Typ: 2.6 mA
      + Max: --

   - Frequency = 32.768 kHz (SMARTC)
      + Min: --
      + Typ: 230 uA
      + Max: --

Table 4.4 Digital Supply Current - Low Power Idle Mode, Peripherals Enabled 
(CLKMODE = 0x04 PCLKEN = 0x0F)
   - Frequency = 24.5 MHz (PROSC)
      + Min: --
      + Typ: 1.5 mA
      + Max: 1.9 mA
   
   - Frequency = 20.0 MHz (LPOSC)
      + Min: --
      + Typ: 1.07 mA
      + Max: -- mA

   - Frequency = 32.768 kHz (SMARTC)
      + Min: --
      + Typ: 232 uA
      + Max: --

Table 4.4 Digital Supply Current - Low Power Idle Mode, Peripherals Disabled
(CLKMODE = 0x04 PCLKEN = 0x00)
   - Frequency = 24.5 MHz (PROSC)
      + Min: --
      + Typ: 487 uA
      + Max: -- 
   
   - Frequency = 20.0 MHz (LPOSC)
      + Min: --
      + Typ: 340 uA
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
Refer to F96x_Clock_Idle.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 24 OCT 2013

---eof--------------------------------------------------------------------------

