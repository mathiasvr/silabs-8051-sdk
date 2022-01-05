//-----------------------------------------------------------------------------
// F93x_Reset_Delay.asm
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
// 
// This example demonstrates the reset delay from power-on and non-power-on, as
// specified in table 4.6 of the datasheet.
// 
// How to Use:
//
// ** NOTE: Instructions differ depending on whether the Reset Delay from POR
// or Reset Delay from non-POR source is being measured. **
//
// A) Reset Delay from Power-On Reset (Minimum Time)
//    1) Remove all of the shorting blocks on the board except a shorting
//       block is placed across to J15.
//    2) Place VDD Select (SW5) in the VREG position and power the board
//       using the P1 power connector.
//    3) Download and run the code on the target board.
//    4) Disconnect both the power adapter and debug adapter from the board.
//    5) Using a signal generator, set up a square-wave signal with
//       an amplitude of 3.5V (rises from 0 to 3.5V) and a frequency of
//       150 Hz or less.
//
//       NOTE: A 150 Hz signal or less is needed to be able to detect the
//             3 ms Power-On Reset Delay. If the frequency is set too high,
//             the square wave will turn low before fully powering on.
//
//    6) Attach this signal to VBAT_PIN (VDD). In order to take accurate
//       measurements, it may be necessary to also attach this signal
//       to an oscilloscope by means of a splitter connected to the
//       waveform generator.
//    7) Attach an oscilloscope probe to /RST TP2. 
//    8) Using the oscilloscope, determine the length of time between
//       the square wave VDD source going high and the peak of the RST
//       signal. 
//      
// B) Delay from Non-"Power-On" Reset and Code Execution
//    1) Ensure that the J15 (Imeasure) header has the associate shorting 
//       block attached to it.
//    2) Download and run the code on the target board.
//    3) Attach oscilloscope probes to /RST TP2 and P0.0. 
//    4) Hold down the reset switch, and configure the oscilloscope to
//       read the trigger event upon releasing the switch.
//    5) After releasing the reset switch, use the oscilloscope to
//       determine the length of time between the effective startup of
//       the MCU and the falling edge of P0.0 (LED1 activated).
//       
//
// Target:         C8051F96x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
//
// Revision History:
//
// Release 1.0
//    - Initial Revision (MT)
//    - 16 OCT 2013
//
//
// Refer to F93x_Reset_Delay_Readme.txt for more information.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F960_Defs.inc"

org 0x00
  jmp MAIN


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

org 0D3h

MAIN:

  mov P0MDOUT, #0x03                    ; enable P0.0 and P0.1 push-pull output
  mov XBR2, #0x40                       ; enable crossbar

  anl P0, #0xFE                         ; activate LED1

  mov PCA0MD, #0x00                     ; disable software watchdog timer

  mov A, RSTSRC
  anl A, #0x02                          ; check current status of RSTSRC

  ; turn on LED2 only if Power-On RSTSRC bit set
  cjne A, #0x02, LOOP                
  anl P1, #0xFD                         ; activate LED2
	                     

LOOP:
  jmp LOOP                              ; wait forever
 
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

END
