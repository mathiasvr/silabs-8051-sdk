EFM8UB1 WGX Bootloader

This example demonstrates a bootloader that can load firmware images from a
WGX module via UART0 or UART1.


-----------------------------------------------------------------------------
 Bootloader Operation:
-----------------------------------------------------------------------------

The bootloader makes use of the 'hboot' command on the WGX module's UART 
command line interface. It is based on the EFM8 Factory Bootloader described
in AN945.

The bootloader will run if any of the following conditions are true:

- Flash address 0x0000 is 0xFF (reset vector is unprogrammed).
- Register R0 is set to the bootloader signature (0xA5) and a software reset
was requested using the RSTSRC register (application requesting bootloader).
- A power on reset or pin reset has occurred while the boot pin is set to 0.
The boot pin is P2.0 on the QFN20, QFN24, and QSOP24 packages, and P3.0 on the
QFN28 package.
- The LOAD_DEFAULT_ON_STARTUP compile option has been selected and a power on
or pin reset has occurred.

If none of these conditions are satisfied, the bootloader will jump to address
0x0000 to execute the application.

Once the bootloader has started, it will periodically check the UART interface
for a connected WGX module by sending a carriage return and line feed sequence.
When the WGX module responds, the bootloader will send a 'hboot' command to 
begin the loading process.

The application can request a specific image by setting several signature 
bytes and the filename in the XDATA memory as follows:

0x02 = '@' (0x40)
0x03 = '#' (0x43)
0x04 = String containing filename. Filename must be null terminated.

If no file is specified, the bootloader will request a default file. See 
configuration options below to change the default file.

Once the loading process has begun, the watchdog timer will be set for a 13
second timeout. If the load has not completed after the timeout, the device
will reset and try again.

When the image has loaded successfully, the bootloader will disable the 
watchdog and perform a software reset to begin the application.



-----------------------------------------------------------------------------
 Configuration Options:
-----------------------------------------------------------------------------

Several compile time options are available in hboot_config.h:

EFM8_UART - select UART0 or UART1 (default is UART1).

ENABLE_BOARD_CONTROLLER - enables the board controller on the SLSTK2000A
evaluation board. This makes UART0 available on the board's virtual COM port.
This option is only needed when using UART0 on the SLSTK2001A boar0.

DISABLE_BOARD_CONTROLLER - disables the board controller on the SLSTK2000A
evaluation board. This makes UART0 available on the board's external IO
connector. This option is only needed when using UART0 on the SLSTK2000A
board (this is the default setting).

Note ENABLE_BOARD_CONTROLLER and DISABLE_BOARD_CONTROLLER are mutually 
exclusive.  If neither is selected, the board control pin will be left 
floating.

ENABLE_LEDS - enables LED status indication (default is enabled). If disabled,
the LED pins will be left floating.

LOAD_DEFAULT_ON_STARTUP - selecting this option will cause the bootloader to
request loading BOOT_FILE every time the device starts from a power on
or pin reset. Be aware that if the device is power cycled frequently, using
this option will cause heavy wear on the flash memory.

BOOT_FILE - sets the default image filename.


Target:         EFM8UB1
Tool chain:     Generic

Release 1.0 (JB)
   - Initial Revision
   - 6 SEP 2018

Release 1.1 (JB)
   - Add LOAD_DEFAULT_ON_STARTUP option
   - 13 NOV 2018


-----------------------------------------------------------------------------
 Resources:
-----------------------------------------------------------------------------
 SYSCLK - 24.5 MHz HFOSC0
 WDT  - 13 second timeout
 P1.4 - LED0
 P1.5 - LED1
 P1.6 - LED2
 P2.0 - Board controller enable (if configured)
 P2.1 - UART1 TX (115200 bps)
 P2.2 - UART1 RX
-----------------------------------------------------------------------------