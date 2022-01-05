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
This program demonstrates the Hysteresis character with different mode 
on datasheet for the Comparator in Table 4.16, in this program SW3 to switch 
Comparator work mode. push SW4 to cycle Hysteresis type. On chip digital 
supply 1.8v (VREG0) as CP0 negative input. P1.6 analog input connect a voltage 
supply as CP0 positive input.

Resources Used:
--------------------------------------------------------------------------------
LPOSC
PORT0
PORT1
TMR0
WDT0 (Disabled)

Part Family:
--------------------------------------------------------------------------------
C8051F96x

Datasheet Version:
--------------------------------------------------------------------------------
Rev 0.5

Specs Demonstrated:
--------------------------------------------------------------------------------
Table 4.16 Hysteresis
   - Mode 0, Hysteresis 1
      + Min: --
      + Typ: 0    mV
      + Max: --
            
   - Mode 0, Hysteresis 2
      + Min: --
      + Typ: 8.5  mV
      + Max: --  
      
   - Mode 0, Hysteresis 3
      + Min: --
      + Typ: 17   mV
      + Max: --     
      
   - Mode 0, Hysteresis 4
      + Min: --
      + Typ: 34   mV
      + Max: --    

   - Mode 1, Hysteresis 1
      + Min: --
      + Typ: 0    mV
      + Max: --
            
   - Mode 1, Hysteresis 2
      + Min: --
      + Typ: 6.5  mV
      + Max: --  
      
   - Mode 1, Hysteresis 3
      + Min: --
      + Typ: 13   mV
      + Max: --     
      
   - Mode 1, Hysteresis 4
      + Min: --
      + Typ: 26   mV
      + Max: --    
           
   - Mode 2, Hysteresis 1
      + Min: --
      + Typ: 0    mV
      + Max: 1    mV
            
   - Mode 2, Hysteresis 2
      + Min: 2    mV
      + Typ: 5    mV
      + Max: 10   mV
      
   - Mode 2, Hysteresis 3
      + Min: 5    mV
      + Typ: 10   mV
      + Max: 20   mV     
      
   - Mode 2, Hysteresis 4
      + Min: 12   mV
      + Typ: 20   mV
      + Max: 30   mV    
         
   - Mode 3, Hysteresis 1
      + Min: --
      + Typ: 0    mV
      + Max: --
            
   - Mode 3, Hysteresis 2
      + Min: --
      + Typ: 4.5  mV
      + Max: --  
      
   - Mode 3, Hysteresis 3
      + Min: --
      + Typ: 9    mV
      + Max: --     
      
   - Mode 3, Hysteresis 4
      + Min: --
      + Typ: 17   mV
      + Max: --    
         
      
Notes On Example and Modes:
--------------------------------------------------------------------------------
   
How to Use:
--------------------------------------------------------------------------------
Refer to F96x_Comparator_Hysteresis.c for detailed directions on using this 
example.

Revision History:
--------------------------------------------------------------------------------
Release 1.0
   -Initial Revision (MT)
   - 05 NOV 2013

---eof--------------------------------------------------------------------------
