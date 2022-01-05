//-----------------------------------------------------------------------------
// F85x_Flash_LockByte.asm
//-----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the Silicon Laboratories End User 
// License Agreement which accompanies this distribution, and is available at
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
// Original content and implementation provided by Silicon Laboratories.
//
// Refer to F85x_Flash_LockByte_Readme.txt for more information regarding this
// code example's functionality.
//
//
// How to Test (Flash - Lock Byte Functionality):
// ----------------------------------------------
// 1) Uncomment the series of #define directives which correspond to your board.
// 2) Add a breakpoint to the _nop_ line featuring the comment "RUN TO HERE".
// 3) Compile and download code to a 'F85x device on a UPMU-F850 MCU 
//    development board by selecting Run -> Debug from the menus, clicking the
//    Debug button in the quick menu, or pressing F11.
// 4) Run the code by selecting Run -> Resume from the menus, clicking the
//    Resume button in the quick menu, or pressing F8.
// 5) Upon reaching the breakpoint, access Code Memory by 
//	  selecting Window -> Show View -> Memory Browser.
// 6) Input the address of the last byte in code memory (e.g. 0x1FFF for the F850)
//    and input the value corresponding to the desired number of locked pages.
//    This number must be written as a one's complement. For more information
//    on how to input this value, see the section titled "Flash Memory" in the
//    reference documentation for your device.
// 7) After inputting the lock byte value, click the "Refresh Values", and
//    then click the "Reset" button.
// 8) Run the code again. Then, halt the code. If the page lock was performed
//    correctly, the program execution will halt on the "PASSED" label.
//
//    NOTE: This program tests the locked pages by attempting to read all pages
//          one by one, and stopping upon locating a page which has been locked.
//          The top page number that has been locked is recorded in register R4.
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

#include "SI_C8051F850_Defs.inc"

;------------------------------------------------------------------------------
; DEFINE TEST PARAMETERS
;------------------------------------------------------------------------------

;--------------------------------16K FLASH Test Parameters---------------------
; Uncomment for 8kB device (F850/3, F860/3)
#define ADDRESS_UNLOCKED_CODE 01dd0h   ; Change this number to a value above
                                       ; the locked FLASH pages.
#define ADDRESS_HIGH  01Ah
#define PAGE_NUMBER 00DH

; Uncomment for 4kB device  (F851/4, F861/4)
//#define ADDRESS_UNLOCKED_CODE 00dd0h ; Change this number to a value above 
                                       ; the locked FLASH pages.
//#define ADDRESS_HIGH  00Ah
//#define PAGE_NUMBER 005H

; Uncomment for 2kB device  (F852/5, F862/5)
//#define ADDRESS_UNLOCKED_CODE 005d0h ; Change this number to a value above
                                       ; the locked FLASH pages.
//#define ADDRESS_HIGH  002h
//#define PAGE_NUMBER 001H

;------------------------------------------------------------------------------

FLARDL       DATA 0B3H

#define ADDRESS_LOW    000h   ; Low byte of FLASH read memory location

org  00h
MAIN:
  clr IE_EA          ; disable all interrupts
  mov WDTCN,#0DEh ; disable software watchdog timer
  mov WDTCN,#0ADh
  setb IE_EA         ; re-enable interrupts

  nop  ; RUN TO HERE (use breakpoint)
  ; Configure the Lock Byte Address
  ; Click refresh button.
  ; Click the reset button before continuing.

  mov A, RSTSRC
  cjne A, #040h, TESTJMP      ; Test if FERROR has not occured


  jmp PASSED

TESTJMP:
  jmp TEST

org ADDRESS_UNLOCKED_CODE
TEST:
  mov R4, #PAGE_NUMBER        ; Check R4 at end of execution to see 
                              ; which page caused FERROR.
  mov R6, #ADDRESS_HIGH
  mov R7, #ADDRESS_LOW
LOOP:
  mov DPH, R6
  mov DPL, R7
  clr A
  movc A, @A+DPTR             ; Attempt to read FLASH page memory
  mov A, R0
  dec R6
  dec R6
  dec R4
  cjne R4, #0FFh, LOOP        ; Test 256-byte pages from ADDRESS_HIGH to 0x0000

  jmp $                       ; TEST ERROR.  Locked FLASH was read by unlocked
                              ; code or lock byte/addresses were not set
                              ; correctly.
PASSED:
  jmp $                       ; TEST FINISHED.  Trying to read locked FLASH
                              ; from unlocked code resulted in FLASH error
                              ; reset.  Read the value in R4 and compare with
                              ; test procedure.

;--------------------------------END OF PROGRAM--------------------------------
END
