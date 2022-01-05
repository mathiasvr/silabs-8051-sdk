//-----------------------------------------------------------------------------
// EFM8_IOEXP_common.h
//-----------------------------------------------------------------------------
// Copyright 2020 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header for common ioexp file:

#ifndef EFM8_IOEXP_COMMON_H_
#define EFM8_IOEXP_COMMON_H_

//-----------------------------------------------------------------------------
// Device Dependent Definitions
//-----------------------------------------------------------------------------
#define IOEXP_PORT_MAX              1
#include <SI_EFM8BB52_Register_Enums.h>
// P0.0 - P0.7 delegated as port 0;
// P1.0 - P1.7 delegated as port 1;
// P2.4 - P2.6 delegated as port 2 (port2.5-port2.7);
// P3.0 - P3.4 delegated as port 2 (port2.0-port2.4);
// MCU  BRD5206B  Function
// port port      description
// P2.0 EXP12     SMB SDA;
// P2.1 EXP14     SMB SCL;
// P2.2 -----     ADDR_EXT;
// P2.3 EXP03     INP_CHANGED_IRQ;
SI_SBIT(IOEXP_INP_CHANGED_IRQ, SFR_P2, 3);
SI_SBIT(IOEXP_ADDR_EXT, SFR_P2, 2);
#define P0DEVMASK                0xFF
#define P1DEVMASK                0xFF
#define P2DEVMASK                0x70
#define P3DEVMASK                0x1F
#undef IOEXP_PORT_MAX
#define IOEXP_PORT_MAX              2 // redefine for port 2
#define UID_ST_ADDR 0xFFCF
#define UID_SP_ADDR 0xFFC0
#define UID_SEG           SI_SEG_CODE

#ifndef LEGACY_PAGE
#define LEGACY_PAGE              0x00
#endif
#define SET_IRQ                     0
#define CLEAR_IRQ                   1

#define IOEXP_UID_LENGTH UID_ST_ADDR - UID_SP_ADDR + 1
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#define WRITE                    0x00 // SMBus WRITE command
#define READ                     0x01 // SMBus READ command

#define SLAVE_ADDR               0xF0 // Device addr(7 bits, lsb is GC:Ignored)
#define SLAVE_ADDR_EXT           0xF2 // Device addr(7 bits, lsb is GC:Ignored)
#define SLAVE_ADDR_MASK          0xFF // Address mask(7 bits, lsb is AUT_ACK)

// Status vector - top 4 bits only
#define SMB_SRADD                0x20 // (SR) slave address received
                                      //    (also could be a lost arbitration)
#define SMB_SRSTO                0x10 // (SR) STOP detected while SR or ST,
                                      //    or lost arbitration
#define SMB_SRDB                 0x00 // (SR) data byte received, or
                                      //    lost arbitration
#define SMB_STDB                 0x40 // (ST) data byte transmitted
#define SMB_STSTO                0x50 // (ST) STOP detected during a
                                      //    transaction; bus error
// End status vector definition
#define SMB_MAX_BUFF_LEN            4

#define IOEXP_DEVICE_ID    0x494F5850 // Signature:'IOXP'
#define IOEXP_NO_DATA_ERR        0xFF
// Definitions for IOEXP instruction handling
#define IOEXP_CMD_MASK           0x7C
#define IOEXP_CMD_INPUT          0x00
#define IOEXP_CMD_OUTPUT         0x04
#define IOEXP_CMD_INVERT         0x08
#define IOEXP_CMD_CONFIG         0x0C
#define IOEXP_CMD_DRIVE          0x10
#define IOEXP_CMD_SLEEP          0x70
#define IOEXP_CMD_DEVID          0x78

#define IOEXP_PORT_MASK          0x03
#define IOEXP_PORT_0                0
#define IOEXP_PORT_1                1
#define IOEXP_PORT_2                2

#define IOEXP_AUTO_INCREMENT     0x80
#define CMD_AUTO_INCREMENT(cmd) {\
  if (\
    ((cmd) & IOEXP_AUTO_INCREMENT) &&\
    (((cmd) & IOEXP_PORT_MASK) < IOEXP_PORT_MAX)) {\
      cmd++;\
  }}

#define EMAT_ENABLE(void) ( EIE1 |= EIE1_EMAT__ENABLED )         // Enable Port Match interrupts
#define EMAT_DISABLE(void) ( EIE1 &= ~EIE1_EMAT__ENABLED )       // Disable Port Match interrupts to prevent multiply interrupts

#define WRITE_OUTPUT_CMD(port, data_out) {\
  switch (port) {\
    case 0: P0 = (((data_out)^P0IPM) & P0DEVMASK) | (P0 & ~P0DEVMASK); break;\
    case 1: P1 = (((data_out)^P1IPM) & P1DEVMASK) | (P1 & ~P1DEVMASK); break;\
    case 2: P2 = ((((data_out)^P2IPM)>>1) & P2DEVMASK) | (P2 & ~P2DEVMASK);\
            P3 = (((data_out)^P2IPM) & P3DEVMASK) | (P3 & ~P3DEVMASK); break;\
  }}

#define WRITE_INVERT_CMD(port, data_out) {\
  switch (port) {\
    case 0: P0IPM = (data_out) & P0DEVMASK; break;\
    case 1: P1IPM = (data_out) & P1DEVMASK; break;\
    case 2: P2IPM = (data_out); break;\
  }}

#define WRITE_CONFIG_CMD(port, data_out) {\
  /* Disable Port Match IRQs to prevent interrupts while configuration changes */ \
  EMAT_DISABLE(); \
  switch (port) {\
    case 0: P0MDOUT = ((data_out) & P0DEVMASK) | (P0MDOUT & ~P0DEVMASK);\
            P0MASK = (P0MDIN & ~P0MDOUT) & P0DEVMASK; break;\
    case 1: P1MDOUT = ((data_out) & P1DEVMASK) | (P1MDOUT & ~P1DEVMASK);\
            P1MASK = (P1MDIN & ~P1MDOUT) & P1DEVMASK; break;\
    case 2: P2MDOUT = (((data_out)>>1) & P2DEVMASK) | (P2MDOUT & ~P2DEVMASK);\
            P3MDOUT = ((data_out) & P3DEVMASK) | (P3MDOUT & ~P3DEVMASK); break;\
    }\
  /* if IOEXP_INP_CHANGED_IRQ is not cleared must not enable the interrupt */ \
  /* to avoid unexpected behavior */ \
  if (IOEXP_INP_CHANGED_IRQ == CLEAR_IRQ) {\
    EMAT_ENABLE();  /* Enable Port Match interrupts */ \
  }}

#define WRITE_DRIVE_CMD(port, data_out) {\
  switch (data_out) {\
    case 0: PRTDRV &= ~(1 << (port)); break;\
    case 1: PRTDRV |=  (1 << (port)); break;\
  }}

#define READ_INPUT_CMD(port, retval) {\
  switch (port) {\
    case 0: (retval) = ((P0^P0IPM) & P0DEVMASK); break;\
    case 1: (retval) = ((P1^P1IPM) & P1DEVMASK); break;\
    case 2: (retval) = ((((P2 & P2DEVMASK) << 1 ) | (P3 & P3DEVMASK))^P2IPM); break;\
  };}

#define READ_CHECK(port) ( unchecked_port_after_irq &= ~(1 << (port)) )

#define READ_OUTPUT_CMD(port, retval) {\
  switch (port) {\
    case 0: (retval) = ((P0) & P0DEVMASK); break;\
    case 1: (retval) = ((P1) & P1DEVMASK); break;\
    case 2: (retval) = (((P2 & P2DEVMASK) << 1 ) | (P3 & P3DEVMASK)); break;\
  };}

#define READ_INVERT_CMD(port, retval) {\
  switch (port) {\
    case 0: (retval) = (P0IPM & P0DEVMASK); break;\
    case 1: (retval) = (P1IPM & P1DEVMASK); break;\
    case 2: (retval) = (P2IPM); break;\
  };}

#define READ_CONFIG_CMD(port, retval) {\
  switch (port) {\
    case 0: (retval) = (P0MDOUT & P0DEVMASK); break;\
    case 1: (retval) = (P1MDOUT & P1DEVMASK); break;\
    case 2: (retval) = (((P2MDOUT & P2DEVMASK) << 1 ) | (P3MDOUT & P3DEVMASK)); break;\
  };}

#define READ_DRIVE_CMD(port, retval) ( (retval) = PRTDRV & (1 << (port)) )

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct {
  uint32_t signum;
  uint8_t deviceId;
  uint8_t derivId;
  uint8_t revId;
  uint8_t smbAddr;
  uint8_t uid[IOEXP_UID_LENGTH];
} ioexp_info_t;

typedef enum {
  IOEXP_ST_READY = 0,
  IOEXP_ST_CMD_RECEIVED,
  IOEXP_ST_WAITNG_FOR_DATA,
  IOEXP_ST_DATA_RECEIVED,
} ioexp_status;

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
extern ioexp_info_t dev_id;
extern uint8_t id_index;
extern uint8_t P0IPM; // Internal IOEXP variable for INVERT_POLARITY commands
extern uint8_t P1IPM; // Internal IOEXP variable for INVERT_POLARITY commands
extern uint8_t P2IPM; // Internal IOEXP variable for INVERT_POLARITY commands

extern uint8_t unchecked_port_after_irq;

extern ioexp_status IOEXP_STATUS;
extern uint8_t ioexp_cmd;
extern uint8_t ioexp_data;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void __ioexp_SMBUS_ISR(void);
void __ioexp_SMBUS_TIMER_ISR(void);
void __ioexp_PMATCH_ISR(void);
void ioexp_main (void);

#endif // EFM8_IOEXP_COMMON_H_
