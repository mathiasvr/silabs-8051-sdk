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
LCD0 configurations in sleep mode. A switch, SW2, will disable/enable the 
LCD driver. Note: the datasheet figures assume a device without an LCD 
connected. The current consumed by an LCD is dependent on the LCD used.

Resources Used:
--------------------------------------------------------------------------------
WDT0 (Watchdog Timer - Disabled)
P0.1 (SW2 - MCU Mode Cycle Switch)

Product Family:
--------------------------------------------------------------------------------
C8051F96x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.4 Digital Core (SYSCLK) -  Sleep Mode, LCD Enable.
    (60Hz refresh rate, driving 32 segment) 
   - RTC-LFO, Contrast Mode1, charge pump disabled. 
     - static LCD
      + Min: --
      + Typ: 0.6 uA
      + Max: --	
      
     - 2-Mux LCD
      + Min: --
      + Typ: 1.0 uA
      + Max: --	
      
     - 4-Mux LCD
      + Min: --
      + Typ: 1.1 uA
      + Max: --	
      
   - RTC-32kHz Crystal, Contrast Mode1, charge pump disabled. 
     - static LCD
      + Min: --
      + Typ: 1.1 uA
      + Max: --	
      
     - 2-Mux LCD
      + Min: --
      + Typ: 1.5 uA
      + Max: --	
      
     - 4-Mux LCD
      + Min: --
      + Typ: 1.6 uA
      + Max: --	      
      
   - RTC-LFO, Contrast Mode3, charge pump enable 2.7 V. 
     - static LCD
      + Min: --
      + Typ: 1.2 uA
      + Max: --	
      
     - 2-Mux LCD
      + Min: --
      + Typ: 1.6 uA
      + Max: --	
      
     - 3-Mux LCD
      + Min: --
      + Typ: 1.8 uA
      + Max: --	 
      
     - 4-Mux LCD
      + Min: --
      + Typ: 2.0 uA
      + Max: --	    
      
   - RTC-32kHz Crystal, Contrast Mode3, charge pump enable 2.7 V. 
     - static LCD
      + Min: --
      + Typ: 1.3 uA
      + Max: --	
      
     - 2-Mux LCD
      + Min: --
      + Typ: 1.8 uA
      + Max: --	
      
     - 3-Mux LCD
      + Min: --
      + Typ: 1.8 uA
      + Max: --	 
      
     - 4-Mux LCD
      + Min: --
      + Typ: 2.0 uA
      + Max: --	
      
How to Use:
--------------------------------------------------------------------------------
Refer to F96x_Clock_Sleep_LCD0.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
	- Initial Revision (MT)
	- 09 NOV 2013

---eof--------------------------------------------------------------------------

