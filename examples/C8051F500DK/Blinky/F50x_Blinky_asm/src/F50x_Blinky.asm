$NOMOD51
;-----------------------------------------------------------------------------
; F50x_Blinky.asm
;-----------------------------------------------------------------------------
; Copyright 2014 Silicon Laboratories, Inc.
; http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
;
; Program Description:
; --------------------
;
; This program illustrates how to disable the watchdog timer,
; configure the Crossbar, configure a port and write to a port
; I/O pin.
;
;
; How To Test:
; ------------
; 1) Ensure shorting blocks on C8051F500-TB are placed on the following:
;    J19: P1.3 LED
;    J7: Power selection, P4 9V (+5VREG) or P2 Debugger (SER_PWR)
; 2) Connect the USB Debug Adapter to P2. If +5VREG is selected on J7,
;    connect the 9V power adapter to P2.
; 3) Compile and download code to C8051F500 target board with Simplicity
;    IDE by selecting Run -> Debug from the menus,
;    or clicking the Debug button in the quick menu,
;    or pressing F11.
; 4) Run the code by selecting Run -> Resume from the menus,
;    or clicking the Resume button in the quick menu,
;    or pressing F8.
; 5) If the LED flashes, the program is working
;
;
; Target:         C8051F50x/1x
; Tool chain:     Simplicity Studio / Keil C51 9.51
; Command Line:   None
;
;
; Release 1.2 / 11 AUG 2014 (JL)
;    - Updated to use with Simplicity Studio
;
; Release 1.1 / 10 MAR 2010 (GP)
;    -Tested with Raisonance
;
; Release 1.0
;    -Initial Revision (GP)
;    -14 JUL 2008
;

$include (SI_C8051F500_Defs.inc); Include register definition file.

;------------------------------------------------------------------------------
; EQUATES
;------------------------------------------------------------------------------

GREEN_LED   equ   P1.3                 ; Green LED: '0' is OFF	

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
            ; Set SFR Page for PCA0MD
            mov   SFRPAGE, #LEGACY_PAGE

            ; Disable the WDT.
            anl   PCA0MD, #NOT(040h)   ; Clear Watchdog Enable bit

            ; Set SFR Page for Oscillator and Port config
            mov   SFRPAGE, #CONFIG_PAGE

            ; Configure the Oscillator
            orl   OSCICN, #84h         ; System Clock = 24 Mhz / 8

            ; Enable the Port I/O Crossbar

            mov   XBR2, #40h           ; Enable Crossbar
            orl   P1MDOUT, #08h        ; Make LED pin output push-pull

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
;------------------------------------------------------------------------------

END
