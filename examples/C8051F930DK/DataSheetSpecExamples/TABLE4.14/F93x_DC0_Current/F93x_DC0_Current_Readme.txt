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
This program demonstrates the Bias current on datasheet for the DC-DC in
Table 4.14, a method to measure the DC/DC bias current directly is to connect a
power supply to the VBAT pin with the DC/DC running and drive the voltage
higher than the DC/DC target voltage. For example, set the DC/DC for 2.4 volts
and apply 3.0 V to the VBAT pin. This will force the DC/DC duty cycle to go
to 0%. Pulse skipping should be enabled for this measurement.

Resources Used:
--------------------------------------------------------------------------------
LPOSC
PORT0
PORT1
WDT0 (Disabled)
DC0

Part Family:
--------------------------------------------------------------------------------
C8051F93x/92x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 1.3

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.14 DC-DC Bias Current
   - VBAT supply
      + Min: --
      + Typ: 80   uA
      + Max: --

   - VDD/DC+ supply
      + Min: --
      + Typ: 100  uA
      + Max: --


Notes On Example and Modes:
--------------------------------------------------------------------------------

How to Use:
--------------------------------------------------------------------------------
Refer to F93x_DC0_Current.c for detailed directions on using this
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   -03 NOV 2013

---eof--------------------------------------------------------------------------
