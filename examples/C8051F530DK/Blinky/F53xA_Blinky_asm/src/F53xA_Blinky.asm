$NOMOD51
;-----------------------------------------------------------------------------
; Blink.ASM
;-----------------------------------------------------------------------------
;  Copyright 2014 Silicon Laboratories, Inc.
; http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
;
; Program Description:
; --------------------
;
; This program illustrates how to disable the watchdog timer,
; configure the internal oscillator, configure the Crossbar, 
; configure a port and write to a port I/O pin.
;
;
; How to Test:
; ------------
; 1) Ensure shorting blocks on C8051F530A TB are placed on the following:
;    "A" SIDE
;    HDR4: P1.3_A - P1.3_LED_A
;    "B" SIDE
;    HDR3: P1.3_B - P1.3_LED_B
; 2) Connect the USB Debug Adapter to HDR2 for "A" SIDE or HDR1 for "B" SIDE.
;    Connect the 9V power adapter to P5.
; 3) Compile and download code to C8051F530A target board
;    (either "A" SIDE or "B" SIDE) with Simplicity
;    IDE by selecting Run -> Debug from the menus,
;    or clicking the Debug button in the quick menu,
;    or pressing F11.
; 4) Run the code by selecting Run -> Resume from the menus,
;    or clicking the Resume button in the quick menu,
;    or pressing F8.
; 5) The code is running when the LED blinks.
;
;
; Target:         C8051F52xA/3xA (C8051F530A TB)
; Tool chain:     Simplicity Studio / Keil C51 9.51
; Command Line:   None
;
;
; Release 1.2 / 09 SEP 2014 (JL)
;    - Updated to use with Simplicity Studio
;
; Release 1.1 / 11 MAR 2010 (GP)
;    -Tested with Raisonance
;
;
;-----------------------------------------------------------------------------

$include (SI_C8051F530_Defs.inc)       ; Include register definition file.

;-----------------------------------------------------------------------------
; EQUATES
;-----------------------------------------------------------------------------

GREEN_LED   equ   P1.3                 ; Green LED: '1' is ON	

;-----------------------------------------------------------------------------
; RESET and INTERRUPT VECTORS
;-----------------------------------------------------------------------------

            ; Reset Vector
            cseg AT 0
            ljmp Main                  ; Locate a jump to the start of
                                       ; code at the reset vector.

;-----------------------------------------------------------------------------
; CODE SEGMENT
;-----------------------------------------------------------------------------


Blink       segment  CODE

            rseg     Blink             ; Switch to this code segment.
            using    0                 ; Specify register bank for the
                                       ; following program code.

Main:
            ; Disable the WDT.
            anl   PCA0MD, #NOT(040h)   ; clear Watchdog Enable bit

            ; Set the Internal Oscillator to 24.5 MHz
            mov   OSCICN, #87h

            ; Enable the Port I/O Crossbar
            mov   XBR1, #40h
            orl   P1MDOUT, #08h        ; make LED pin output push-pull
            orl   P1MDIN, #08h         ; make LED pin input mode digital

            ; Initialize LED to OFF
            clr   GREEN_LED

            ; Simple delay loop.
Loop2:      mov   R7, #08h			
Loop1:      mov   R6, #00h
Loop0:      mov   R5, #00h
            djnz  R5, $
            djnz  R6, Loop0
            djnz  R7, Loop1
            cpl   GREEN_LED            ; Toggle LED.
            jmp   Loop2

;-----------------------------------------------------------------------------
; End of file.

END
