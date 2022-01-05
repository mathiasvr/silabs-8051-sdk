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
This program demonstrates the following datasheet specifications for the Analog
to Digital Converter (ADC) from Table 1.2 (Power Consumption) and Table 1.7 
(ADC):

 - Power Consumption: Sampling at 800 ksps, 10 Bit conversions, normal bias 
   settings at VDD = 3.0 V
 - Power Consumption: Sampling at 250 ksps, 10 Bit conversions, low power bias 
   settings at VDD = 3.0 V
 - Throughput Rate: High Speed, 10 Bit Mode
 - Throughput Rate: High Speed, 12 Bit Mode
 - Throughput Rate: Low Power, 10 Bit Mode
 - Throughput Rate: Low Power, 12 Bit Mode

The ADC is initialized to the high speed 10 bit mode sampling at 800 ksps. Using
switch 2 (S2) on P2.1, the ADC can be cycled through three major testing modes: 
power consumption high speed, power consumption low power, and throughput rate
monitoring. Pressing switch 1 (S1) on P1.7 will toggle the ADC enable bit while 
in a mode testing power consumption. If in the mode testing throughput rate, S1 
will cycle through the throughput rates listed in Table 1.7.

Resources Used:
--------------------------------------------------------------------------------
ADC0
HFOSC
PORT1
PORT2
TMR2
WDT0 (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F85x/86x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.6 7/13

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 1.2 Power Consumption - ADC0
   - 800 ksps, 10-bit conversions (200 ksps, 12-bit conversions), normal bias 
     settings at VDD = 3.0 V
      + Min: --
      + Typ: 845 uA
      + Max: TBD
   - 250 ksps, 10-bit conversions (62.5 ksps 12-bit conversions), low power 
     bias settings at VDD = 3.0 V
      + Min: --
      + Typ: 425 nA
      + Max: TBD

Table 1.7 ADC - Throughput Rate (High Speed Mode)
   - 12 Bit Mode
      + Min: --
      + Typ: --
      + Max: 200 ksps
   - 10 Bit Mode
      + Min: --
      + Typ: --
      + Max: 800 ksps
      
Table 1.7 ADC - Throughput Rate (Low Power Mode)
   - 12 Bit Mode
      + Min: --
      + Typ: --
      + Max: 62.5 ksps
   - 10 Bit Mode
      + Min: --
      + Typ: --
      + Max: 250 ksps
   
Notes On Example and Modes:
--------------------------------------------------------------------------------
- Mode 0: Sampling at 800 ksps, highest power mode settings (Default)
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2

- Mode 1: Sampling at 250 ksps, lowest power mode settings
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2
   
- Mode 2: High Speed, 10 Bit Mode
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2
      
- Mode 3: High Speed, 12 Bit Mode
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2
   
- Mode 4: Low Power, 10 Bit Mode
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2

- Mode 5: High Speed, 12 Bit Mode
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2

How to Use:
--------------------------------------------------------------------------------
Refer to F85x_ADC_Continuous_Sample.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (BGD)
   -22 JUL 2013

---eof--------------------------------------------------------------------------
