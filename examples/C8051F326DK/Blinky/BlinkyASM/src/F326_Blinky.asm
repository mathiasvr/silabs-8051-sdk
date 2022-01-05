$NOMOD51
;-----------------------------------------------------------------------------
; F326_Blinky.asm
;-----------------------------------------------------------------------------
; Copyright 2014 Silicon Laboratories, Inc.
; All rights reserved.
;
; Program Description:
;
; This program flashes the green LED on the C8051F326 target board using a
; simple while loop in assembly.
;
;
; How To Test:
;
; 1) Ensure shorting blocks on C8051F326-TB are placed on the following:
;    J2: VDD  - +3VD
;    J3: P2.2 - P2.2_LED
;    J7: VDD  - VIO
;    J8: Power selection, 9V or Debugger
; 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
;    connect the 9V power adapter to P1.
; 3) Compile and download code to the C8051F326 target board with Simplicity
;    IDE by selecting Run -> Debug from the menus,
;    or clicking the Debug button in the quick menu,
;    or pressing F11.
; 4) Run the code by selecting Run -> Resume from the menus,
;    or clicking the Resume button in the quick menu,
;    or pressing F8.
;
;
; Target:         C8051F326/7
; Tool chain:     Silicon Labs Studio / Keil C51 9.51
; Command Line:   None
;
; Release 1.1 (JL)
;    - Updated Description
;    - 02 APR 2014
;
;
;-----------------------------------------------------------------------------

$include (SI_C8051F326_Defs.inc)

;-----------------------------------------------------------------------------
; EQUATES
;-----------------------------------------------------------------------------

GREEN_LED   equ   P2.2                    ; Green LED: '1' is ON

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
            ; Configure the Oscillator to 6 Mhz
            mov   OSCICN, #81h

            ; Configure the LED pin
            orl   P2MDOUT, #04h           ; make LED pin output push-pull

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

