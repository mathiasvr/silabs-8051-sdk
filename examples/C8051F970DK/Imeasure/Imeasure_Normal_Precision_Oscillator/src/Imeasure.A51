; -----------------------------------------------------------------------------
;  Imeasure.a51
; -----------------------------------------------------------------------------
;  Copyright 2014 Silicon Laboratories, Inc.
;  www.silabs.com
; 
;  Program Description:
; 
;  This routine inserts an infinite loop that may be used to measure current.
;  The loop has been located within a 64-byte boundary to represent average
;  supply current.
; 
; 
;  Target:         C8051F97x
;  Tool chain:     Keil C51
;  Command Line:   None
; 
;  Release 1.0
;     -Initial Revision (TP)
;     -21 MAY 2014
; 
#include "Imeasure.h"

; -----------------------------------------------------------------------------
;  Start of Module
; -----------------------------------------------------------------------------
NAME	IMEASURE                          

; -----------------------------------------------------------------------------
;  Imeasure_Infinite_Loop
; -----------------------------------------------------------------------------
; 
;  Return Value : None
;  Parameters   : None
; 
;  Infinite Loop used for measuring current.
; 
; -----------------------------------------------------------------------------
   CSEG AT IMEASURE_FUNCTION_LOCATION

   SJMP 	$
   RET   


; -----------------------------------------------------------------------------
;  End of Module
; -----------------------------------------------------------------------------
END                                    ; End of Module

; -----------------------------------------------------------------------------
;  End of File
; -----------------------------------------------------------------------------
