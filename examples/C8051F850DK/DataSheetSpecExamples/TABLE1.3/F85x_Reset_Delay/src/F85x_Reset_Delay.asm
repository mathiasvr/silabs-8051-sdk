//-----------------------------------------------------------------------------
// F85x_Reset_Delay.asm
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// Refer to F85x_Reset_Delay_Readme.txt for more information on this example.
//
// ** NOTE: Instructions differ depending on whether the Reset Delay from POR
// or Reset Delay from non-POR source is being measured. **
//
// How to Test (Reset Delay from Power-On Reset - Minimum Time):
// -------------------------------------------------------------
//    1) Ensure that the LED2-P1.1 (J27) and JP2 (Imeasure) headers have
//       shorting blocks attached to them. Remove all other shorting blocks.
//    2) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//       development board by selecting Run -> Debug from the menus, clicking the
//       Debug button in the quick menu, or pressing F11.
//    3) Disconnect both the power adapter and debug adapter from the board.
//    4) Using a signal generator, set up a square-wave signal with
//       an amplitude of 3.5V (rises from 0 to 3.5V) and a frequency of
//       150 Hz or less.
//
//       NOTE: A 150 Hz signal or less is needed to be able to detect the
//             3 ms Power-On Reset Delay. If the frequency is set too high,
//             the square wave will turn low before fully powering on.
//
//    5) Attach this signal to TP5 (VDD). In order to take accurate
//       measurements, it may be necessary to also attach this signal
//       to an oscilloscope by means of a splitter connected to the
//       waveform generator.
//    6) Attach an oscilloscope probe to TP3 (RST). 
//    7) Using the oscilloscope, determine the length of time between
//       the square wave VDD source going high and the peak of the RST
//       signal. 
//      
// How to Test (Delay from Non-"Power-On" Reset and Code Execution):
// -----------------------------------------------------------------
//    1) Ensure that the JP2 (Imeasure) header has the associate shorting 
//       block attached to it. Remove the LED1-P1.0 (J27) shorting block
//    2) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//       development board by selecting Run -> Debug from the menus, clicking the
//       Debug button in the quick menu, or pressing F11.
//    3) Attach oscilloscope probes to TP3 (RST) and P1.0. 
//    4) Hold down the reset switch, and configure the oscilloscope to
//       read the trigger event upon releasing the switch.
//    5) After releasing the reset switch, use the oscilloscope to
//       determine the length of time between the effective startup of
//       the MCU and the falling edge of P1.0 (LED1 activated).
//       
//
// Target:         C8051F85x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.1 (BL)
//    - Updated Description / How to Test
//    - 5 FEB 2014
//
// Release 1.0
//    - Initial Revision (PMD)
//    - 8 JUL 2013
//
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "SI_C8051F850_Defs.inc"

org 0x00
  jmp MAIN


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

org 0B3h

MAIN:

  mov P1MDOUT, #0x03                   ; enable P1.0 and P1.1 push-pull output
  mov XBR2, #0x40                      ; enable crossbar

  anl P1, #0xFE                        ; activate LED1

  clr IE_EA                               ; disable all interrupts
  mov WDTCN,#0DEh                      ; disable software watchdog timer
  mov WDTCN,#0ADh                      
  setb IE_EA                              ; re-enable interrupts

  mov A, RSTSRC
  anl A, #0x02                         ; check current status of RSTSRC

  ; turn on LED2 only if Power-On RSTSRC bit set
  cjne A, #0x02, LOOP                
  anl P1, #0xFD                        ; activate LED2
	                     

LOOP:
  jmp LOOP                             ; wait forever
 
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

END
