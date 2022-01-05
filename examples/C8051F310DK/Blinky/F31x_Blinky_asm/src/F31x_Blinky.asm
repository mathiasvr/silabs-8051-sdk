$NOMOD51
;-----------------------------------------------------------------------------
; F31x_Blinky.asm
;-----------------------------------------------------------------------------
; Copyright 2014 Silicon Laboratories, Inc.
; http:;www.silabs.com
;
; Program Description:
;
; This program flashes the green LED on the C8051F33x target board about
; five times a second using a simple while loop in assembly.
;
;
; How To Test:
;
; 1) Ensure shorting blocks are place on the following:
;    - J3: P3.3_LED
;    - J8: Power - 9V or Debugger
; 2) Connect the USB Debug Adapter to J4. If 9V power is selected on J8,
;    connect the 9V power adapter to P1.
; 3) Compile and download code to the C8051F310-TB development board by selecting 
;		Run -> Debug from the menus, clicking the Debug button in the quick menu,
;		or pressing F11.
; 4) Run the code by selecting Run -> Resume from the menus, clicking the 
;		Resume button in the quick menu, or pressing F8.
;
;
; Target:         C8051F310
; Tool chain:     Silicon Labs Studio / Keil C51 9.51
; Command Line:   None
;
; Release 1.1 (BL)
;		- Updated Description / How to Test
;		- 16 JAN 2014
;
; Release 1.0
;    - Initial Revision (HF)
;    - 04 FEB 2003
;

$include (SI_C8051F310_Defs.inc)

;-----------------------------------------------------------------------------
; EQUATES
;-----------------------------------------------------------------------------

GREEN_LED   equ   P3.3                    ; Green LED: '1' is ON	

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
            orl   P0SKIP, #04h            ; skip LED pin in crossbar
                                          ; assignments
            mov   XBR1, #40h              ; enable Crossbar
            orl   P3MDOUT, #08h           ; make LED pin output push-pull
            orl   P3MDIN, #08h            ; make LED pin input mode digital

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

