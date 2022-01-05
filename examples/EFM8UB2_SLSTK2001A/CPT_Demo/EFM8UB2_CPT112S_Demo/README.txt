EFM8UB2 CPT112S Demo
========================

1. Introduction
This demo demonstrates the capacitive button function of CPT112S 
fixed function board. This demo runs on the EFM8UB2 STK,
and communicates with CPT112S through the SMBUS pins on the EXP header. 
Virtual buttons on STK's LCD screen indicate whether the capacitive 
sensing buttons on CPT112S are pressed or released.

This demo supports 11 buttons on the CPT112S device.
(The CS11 button is disabled in the default configuration of the board 
to enable the buzzer)

2. Hardware
EFM8UB2 STK Board (Mother board)
CPT112S FFD (Daughter board attached through EXP)

Board:  Silicon Labs SLSTK2001A-EFM8UB2 Starter Kit
Device: EFM8UB20F64G