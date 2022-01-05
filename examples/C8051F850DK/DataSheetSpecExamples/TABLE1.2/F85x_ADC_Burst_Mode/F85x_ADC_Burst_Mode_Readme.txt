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
This program demonstrates the following power consumption specifications for the
Analog to Digital Converter (ADC) in burst mode from Table 1.2:

 - 10-bit single conversions, external reference, 200 ksps
 - 10-bit single conversions, external reference, 100 ksps
 - 10-bit single conversions, external reference, 10 ksps
 - 10-bit single conversions, internal reference, Low power bias, 200 ksps
 - 10-bit single conversions, internal reference, Low power bias, 100 ksps
 - 10-bit single conversions, internal reference, Low power bias, 10 ksps
 - 12-bit single conversions, external reference, 100 ksps
 - 12-bit single conversions, external reference, 50 ksps
 - 12-bit single conversions, external reference, 10 ksps
 - 12-bit single conversions, internal reference, Normal bias, 100 ksps
 - 12-bit single conversions, internal reference, Low power bias, 50 ksps
 - 12-bit single conversions, internal reference, Low power bias, 10 ksps

The ADC is initialized to the 10-bit single conversions burst mode using the 
external reference and sampling at 200 ksps. Use switch 1 (S1) on P1.7 to cycle
through the ADC burst mode settings. Use switch 2 (S2) on P2.1 to toggle the ADC
burst mode enable bit.

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
Table 1.2 Power Consumption - ADC0 Burst Mode, 10-bit single conversions, 
external reference
   - 200 ksps, VDD = 3.0 V
      + Min: --
      + Typ: 370 uA
      + Max: --
   - 100 ksps, VDD = 3.0 V
      + Min: --
      + Typ: 185 uA
      + Max: --
   - 10 ksps, VDD = 3.0 V
      + Min: --
      + Typ: 19 uA
      + Max: --

Table 1.2 Power Consumption - ADC0 Burst Mode, 10-bit single conversions, 
internal reference, Low power bias settings
   - 200 ksps, VDD = 3.0 V
      + Min: --
      + Typ: 490 uA
      + Max: --
   - 100 ksps, VDD = 3.0 V
      + Min: --
      + Typ: 245 uA
      + Max: --
   - 10 ksps, VDD = 3.0 V
      + Min: --
      + Typ: 23 uA
      + Max: --
      
Table 1.2 Power Consumption - ADC0 Burst Mode, 12-bit single conversions, 
external reference
   - 100 ksps, VDD = 3.0 V
      + Min: --
      + Typ: 530 uA
      + Max: --
   - 50 ksps, VDD = 3.0 V
      + Min: --
      + Typ: 265 uA
      + Max: --
   - 10 ksps, VDD = 3.0 V
      + Min: --
      + Typ: 53 uA
      + Max: --
   
Table 1.2 Power Consumption - ADC0 Burst Mode, 12-bit single conversions, 
internal reference
   - 100 ksps, VDD = 3.0 V, Normal bias
      + Min: --
      + Typ: 950 uA
      + Max: --
   - 50 ksps, VDD = 3.0 V, Low power bias
      + Min: --
      + Typ: 420 uA
      + Max: --
   - 10 ksps, VDD = 3.0 V, Low power bias
      + Min: --
      + Typ: 85 uA
      + Max: --
   
Notes On Example and Modes:
--------------------------------------------------------------------------------
- ADC0_BURST_10_BIT_EXT_REF_200_KSPS (Default)
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2

- ADC0_BURST_10_BIT_EXT_REF_100_KSPS
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2
   
- ADC0_BURST_10_BIT_EXT_REF_10_KSPS
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2
      
- ADC0_BURST_10_BIT_INT_REF_200_KSPS
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2
   
- ADC0_BURST_10_BIT_INT_REF_100_KSPS
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2

- ADC0_BURST_10_BIT_INT_REF_10_KSPS
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2   

- ADC0_BURST_12_BIT_EXT_REF_100_KSPS
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2

- ADC0_BURST_12_BIT_EXT_REF_50_KSPS
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2
   
- ADC0_BURST_12_BIT_EXT_REF_10_KSPS
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2
      
- ADC0_BURST_12_BIT_INT_REF_100_KSPS
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2
   
- ADC0_BURST_12_BIT_INT_REF_50_KSPS
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2

- ADC0_BURST_12_BIT_INT_REF_10_KSPS
   + ADC0
   + HFOSC
   + PORT1
   + PORT2
   + TMR2

How to Use:
--------------------------------------------------------------------------------
Refer to F85x_ADC_Burst_Mode.c for detailed directions on using this example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (BGD)
   -25 JUL 2013

---eof--------------------------------------------------------------------------
