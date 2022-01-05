EFM8BB2 CPT112S Demo
========================

 Program Description:
 This demo demonstrates the capacitive button function of CPT007B fixed
 function board. This demo runs on the EFM8BB2 STK, and communicates
 with CPT007B through the GPIO pins on the EXP header. Virtual buttons
 on STK's LCD screen indicate whether the capacitive sensing buttons on
 CPT007B are pressed or released.
 This demo supports up to 7 capacitive buttons on CPT007B

 How To Test: EFM8BB2 STK
-----------------------------------------------------------------------------
 1) Place the switch in "AEM" mode.
 2) Attach CPT007B board to the STK through EXP headers
 3) Connect the EFM8BB2 STK board to a PC using a mini USB cable.
 4) Compile and download code to the EFM8BB2 STK board.
    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
    click the Debug button in the quick menu, or press F11.
 4) Run the code.
    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
    click the Resume button in the quick menu, or press F8.
 5) The demo program should start with a set of circles on display and by
    pressing/releasing capacitive buttons on CPT007B, the circles turns
    ON/OFF.

 Target:         EFM8BB2
 Tool chain:     Generic

 Release 0.1 (MR/CC)
    - Initial Revision
    - 28 Oct 2015

 Resources
-----------------------------------------------------------------------------
 SYSCLK - 24.5 MHz HFOSC / 1
 UART0  - 115200 baud, 8-N-1
 SPI0   - 1 MHz
 Timer1 - 231 kHz (UART0)
 Timer2 - 2 MHz (SPI0 CS delay)
 Timer3 - 1 kHz (1 ms tick)
 P0.4 - UART0_TX
 P0.5 - UART0_RX
 P0.6 - LCD SCK
 P1.0 - LCD MOSI
 P0.1 - LCD CS (Active High)
 P2.6 - Display enable
 P0.7 - CS00 GPIO input
 P1.1 - CS01 GPIO input
 P1.2 - CS02 GPIO input
 P1.3 - CS03 GPIO input
 P1.4 - CS04 GPIO input
 P1.5 - CS05 GPIO input
 P1.6 - CS06 GPIO input