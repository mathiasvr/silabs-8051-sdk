$NOMOD51
;-----------------------------------------------------------------------------
; F41x_Blinky.asm
;-----------------------------------------------------------------------------
; Copyright 2014 Silicon Laboratories, Inc.
; http:;www.silabs.com
;
; Program Description:
;
; This program illustrates how to disable the watchdog timer,
; configure the Crossbar, configure a port and write to a port
; I/O pin.
;
;
; How To Test:
;
; 1) Ensure shorting blocks are place on the following:
;    J9:   VUNREG (9V adapter) or 5VEC3 (Debugger) 
;    J10:  +3VD/+3VD
;    J12:  +3VD/VREGIN
;          VREGIN_EN
;    J17:  5VEC3/VIO or +3VD/VIO
;          VIO_EN
;    J5:   P2.1/D3
; 2) Connect the USB Debug Adapter to J4. If VUNREG is selected on J9,
;    connect the 9V power adapter to P1.
; 3) Compile and download code to the C8051F410-TB development board by
;    selecting Run -> Debug from the menus, clicking the Debug button in the
;    quick menu, or pressing F11.
; 4) Run the code by selecting Run -> Resume from the menus, clicking the
;    Resume button in the quick menu, or pressing F8.
; 5) The P2.1 LED (D3) should be flashing at approximately 2 Hz.
;
;
; Target:         C8051F41x
; Tool chain:     Silicon Labs Studio / Keil C51 9.51
; Command Line:   None
;
; Release 1.2 (BL)
;    - Updated Description / How to Test
;    - 21 JAN 2014
;
; Release 1.1 / 11 MAR 2010 (GP)
;    -Tested with Raisonance
;
; Release 1.0
;    -Initial Revision (GP)
;    -03 JAN 2006
;

$include (SI_C8051F410_Defs.inc)

;------------------------------------------------------------------------------
; EQUATES
;------------------------------------------------------------------------------

GREEN_LED   equ   P2.1                 ; Green LED: '1' is ON	

;------------------------------------------------------------------------------
; RESET and INTERRUPT VECTORS
;------------------------------------------------------------------------------

            ; Reset Vector
            cseg AT 0
            ljmp Main                  ; Locate a jump to the start of
                                       ; code at the reset vector.

;------------------------------------------------------------------------------
; CODE SEGMENT
;------------------------------------------------------------------------------


Blink       segment  CODE

            rseg     Blink             ; Switch to this code segment.
            using    0                 ; Specify register bank for the
                                       ; following program code.

Main:
            ; Disable the WDT.
            anl   PCA0MD, #NOT(040h)   ; Clear Watchdog Enable bit

						; Configure the Oscillator
            orl   OSCICN, #04h         ; sysclk = 24.5 Mhz / 8

            ; Enable the Port I/O Crossbar

            mov   XBR1, #40h           ; Enable Crossbar
            orl   P2MDOUT, #02h        ; Make LED pin output push-pull

            ; Initialize LED to OFF
            clr   GREEN_LED

            ; Simple delay loop.
Loop2:      mov   R7, #03h			
Loop1:      mov   R6, #00h
Loop0:      mov   R5, #00h
            djnz  R5, $
            djnz  R6, Loop0
            djnz  R7, Loop1
            cpl   GREEN_LED            ; Toggle LED.
            jmp   Loop2

;------------------------------------------------------------------------------
; End of file.

END

