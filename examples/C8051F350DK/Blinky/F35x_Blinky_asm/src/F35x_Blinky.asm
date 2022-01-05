$NOMOD51
;-----------------------------------------------------------------------------
; F35x_Blinky.asm
;-----------------------------------------------------------------------------
;  Copyright 2014 Silicon Laboratories, Inc.
;  All rights reserved.
;
; Program Description:
;
; This program flashes the green LED on the C8051F350 target board using a
; simple while loop in assembly.
;
;
; How To Test:
; 1) Ensure shorting blocks are installed on the following:
;    - J3: P0.7 - P0.7_LED
;    - J8: Power selection, P1 9V or J4 Debugger
; 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
;    connect the 9V power adapter to P1. After power the board, the red LED D3
;    is on.
; 3) Compile and download code to the C8051F350-TB by selecting
;      Run -> Debug from the menus,
;      or clicking the Debug button in the quick menu,
;      or pressing F11.
; 4) Run the code by selecting
;      Run -> Resume from the menus,
;      or clicking the Resume button in the quick menu,
;      or pressing F8.
; 5) The code is working if the P0.7 LED D1 blinks.
;
; Target:         C8051F35x
; Tool chain:     Silicon Labs Studio / Keil C51 9.51
; Command Line:   None
;
;
; Release 1.1 (JL)
;    - Updated Description
;    - 23 JUN 2014
;
;-----------------------------------------------------------------------------

$include (SI_C8051f350_Defs.inc)          ; Include register definition file.

;-----------------------------------------------------------------------------
; EQUATES
;-----------------------------------------------------------------------------

GREEN_LED   equ   P0.7                    ; Green LED: '1' is ON

;-----------------------------------------------------------------------------
; RESET and INTERRUPT VECTORS
;-----------------------------------------------------------------------------

            ; Reset Vector
            cseg AT 0
            ljmp Main                     ; Locate a jump to the start of
                                          ; code at the reset vector.

;-----------------------------------------------------------------------------
; CODE SEGMENT
;-----------------------------------------------------------------------------


Blink       segment  CODE

            rseg     Blink                ; Switch to this code segment.
            using    0                    ; Specify register bank for the
                                          ; following program code.

Main:
            ; Disable the WDT.
            anl   PCA0MD, #NOT(040h)      ; clear Watchdog Enable bit

            ; Enable the Port I/O Crossbar
            mov   XBR1, #40h              ; enable Crossbar
            orl   P0MDOUT, #80h           ; make LED pin output push-pull
            orl   P0MDIN, #80h            ; make LED pin input mode digital

            ; Initialize LED to OFF
            clr   GREEN_LED

            ; Simple delay loop.
Loop2:      mov   R7, #03h
Loop1:      mov   R6, #00h
Loop0:      mov   R5, #00h
            djnz  R5, $
            djnz  R6, Loop0
            djnz  R7, Loop1
            cpl   GREEN_LED               ; Toggle LED.
            jmp   Loop2

;-----------------------------------------------------------------------------
; End of file.

END