//-----------------------------------------------------------------------------
// EFM8_IOEXP_common.c
//-----------------------------------------------------------------------------
// Copyright 2020 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// Example software to demonstrate I2C/SMBus to GPIO interface on EFM8 devices:
// - 8-16 GPIOs via an I2C Bus
// - Input and output selectable
// - Drive strength selectable
// - Push/pull/OD selectable
// - Interrupt capable
// - Polarity inversion
// - Auto increment addressing
// - Sleep mode
// - Address extension
//
// Insert LINK to application note here
//
// Resources:
//   SYSCLK - 24.5 MHz
//   SMBus  - SMBus Slave
//   Timer1 - SMBus clock rate (used only for free timeout detection)
//   Timer3 - SMBus SCL low timeout detection
//   P1.2   - SMBus SDA
//   P1.3   - SMBus SCL
//   P1.0   - Address extension (default address increased by 1 if connected to
//            GND)
//   P1.1   - IRQ (goes to GND if state of input pins are changing)
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB51 PK (SMBus GPIO Expander) + EFR32 STK (Command
// Interpreter/SMBus Master)
//-----------------------------------------------------------------------------
// 1) Connect the device EFM8BB51 PK to EFR32 STK running the corresponding
//    I2C CLI code.
// 2) Place the switch in "AEM" mode.
// 3) Connect the EFM8BB51 PK board to a PC using a USB cable.
// 4) Compile and download code to the EFM8BB51 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 5) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 6) Dependently what kind of commands arrive via SMBus the code will drive
//    the appropriate GPIOs or mirrors their state in response.
// 7) Details of command set and expected behavior can be found in AN1304
//
//
// Target:         EFM8BB51
// Tool chain:     Generic
//
// Release 1.0 (CsM)
//    - Initial Revision
//    - 15 FEB 2020
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <string.h>

#include <InitDevice.h>
#include <EFM8_IOEXP_common.h>

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
ioexp_info_t dev_id;
uint8_t id_index = 0;

uint8_t P0IPM = 0; // Internal IOEXP variable for INVERT_POLARITY commands
uint8_t P1IPM = 0; // Internal IOEXP variable for INVERT_POLARITY commands
uint8_t P2IPM = 0; // Internal IOEXP variable for INVERT_POLARITY commands

uint8_t unchecked_port_after_irq = 0;

ioexp_status IOEXP_STATUS = IOEXP_ST_READY;
uint8_t ioexp_cmd = 0;
uint8_t ioexp_data = 0;

static
void parse (uint8_t received_data)
{
  // Parse incoming data and prepare out_buffer
  switch (IOEXP_STATUS)
  {
    case IOEXP_ST_READY:
      // store cmd. If it is a read session, the generat_output() will
      // generate the answer and set the status to READY state else
      // the state-machine is waiting for the incoming data byte(s)
      ioexp_cmd = received_data;
      IOEXP_STATUS = IOEXP_ST_WAITNG_FOR_DATA;
      break;
    case IOEXP_ST_WAITNG_FOR_DATA:
      ioexp_data = received_data;
      IOEXP_STATUS = IOEXP_ST_DATA_RECEIVED;
      break;
    default:
      break;
  }
}

static
uint8_t generate_output (void)
{
  uint8_t *pdev_id = &dev_id;
  uint8_t return_value = IOEXP_NO_DATA_ERR;
  uint8_t SFRPAGE_save;

  SFRPAGE_save = SFRPAGE;
  SFRPAGE = LEGACY_PAGE;
  switch (ioexp_cmd & IOEXP_CMD_MASK)
  {
    case IOEXP_CMD_DEVID:
      if ((ioexp_cmd & IOEXP_AUTO_INCREMENT) && !(ioexp_cmd & IOEXP_PORT_MASK))
      {
        return_value = *(pdev_id + (id_index++ % sizeof(ioexp_info_t)));
      }
      else
      {
        return_value = *(pdev_id + (ioexp_cmd & IOEXP_PORT_MASK));
      }
      break;
    case IOEXP_CMD_INPUT:
      READ_INPUT_CMD(ioexp_cmd & IOEXP_PORT_MASK, return_value);
      READ_CHECK(ioexp_cmd & IOEXP_PORT_MASK);
      if ((IOEXP_INP_CHANGED_IRQ == SET_IRQ) && (unchecked_port_after_irq == 0))
      {
        // The current state will be the new match value
        P0MAT = P0;
        P1MAT = P1;
        IOEXP_INP_CHANGED_IRQ = CLEAR_IRQ;
        EMAT_ENABLE();         // Enable Port Match interrupts
      }
      goto AIL;
    case IOEXP_CMD_OUTPUT:
      READ_OUTPUT_CMD(ioexp_cmd & IOEXP_PORT_MASK, return_value);
      goto AIL;
    case IOEXP_CMD_INVERT:
      READ_INVERT_CMD(ioexp_cmd & IOEXP_PORT_MASK, return_value);
      goto AIL;
    case IOEXP_CMD_CONFIG:
      READ_CONFIG_CMD(ioexp_cmd & IOEXP_PORT_MASK, return_value);
      goto AIL;
    case IOEXP_CMD_DRIVE:
      READ_DRIVE_CMD(ioexp_cmd & IOEXP_PORT_MASK, return_value);
AIL:  CMD_AUTO_INCREMENT(ioexp_cmd);
      break;
    default:
      break;
  }
  SFRPAGE = SFRPAGE_save;
  return return_value;
}


//-----------------------------------------------------------------------------
// SI_INTERRUPT(SMBUS0_ISR, SMBUS0_IRQn)
//-----------------------------------------------------------------------------
void __ioexp_SMBUS_ISR(void)
{
  if (SMB0CN0_ARBLOST == 0)
  {
    // Decode the SMBus status vector
    switch (SMB0CN0 & 0xF0)
    {
      // Slave Receiver: Start+Address received
      case SMB_SRADD:
        SMB0CN0_STA = 0;// Clear SMB0CN0_STA bit
        // If the transfer is a master READ,
        if ((SMB0DAT & 0x01) == READ)
        {
        // Prepare outgoing byte
          SMB0DAT = generate_output();
        }
        else
        {
          SMB0CN0_ACK = 1;            // Auto-SMB0CN_ACK the next master WRITE
        }
        break;
      // Slave Receiver: Data received
      case SMB_SRDB:
        // Parse incoming data and prepare out_buffer if needed
        parse(SMB0DAT);
        SMB0CN0_ACK = 1;// SMB0CN0_ACK received data
        break;
      // Slave Receiver: Stop received while either a Slave Receiver or
      // Slave Transmitter
      case SMB_SRSTO:
        IOEXP_STATUS = IOEXP_ST_READY;
        id_index = 0;
        SMB0CN0_STO = 0;// SMB0CN0_STO must be cleared by software when
                        // a STOP is detected as a slave
        break;
      // Slave Transmitter: Data byte transmitted
      case SMB_STDB:
        // If Master SMB0CN0_ACK's, send the next byte
        if (SMB0CN0_ACK == 1)
        {
          // Prepare next outgoing byte
          SMB0DAT = generate_output();
        }                         // Otherwise, do nothing
        break;
      // Slave Transmitter: Arbitration lost, Stop detected
      //
      // This state will only be entered on a bus error condition.
      // In normal operation, the slave is no longer sending data or has
      // data pending when a STOP is received from the master, so the
      // SMB0CN0_TXMODE bit is cleared and the slave goes to the SRSTO state.
      case SMB_STSTO:
        SMB0CN0_STO = 0;// SMB0CN0_STO must be cleared by software when
                        // a STOP is detected as a slave
        break;
      // Default: all other cases undefined
      default:
        SMB0CF &= ~0x80;// Reset communication
        SMB0CF |= 0x80;
        SMB0CN0_STA = 0;
        SMB0CN0_STO = 0;
        SMB0CN0_ACK = 1;
        break;
    }
  }
  // SMB0CN0_ARBLOST == 1, Abort failed transfer
  else
  {
    SMB0CN0_STA = 0;
    SMB0CN0_STO = 0;
    SMB0CN0_ACK = 1;
  }

  SMB0CN0_SI = 0;                     // Clear SMBus interrupt flag
}

//-----------------------------------------------------------------------------
// SI_INTERRUPT(TIMER3_ISR, TIMER3_IRQn)
//-----------------------------------------------------------------------------
void __ioexp_SMBUS_TIMER_ISR(void)
{
  SMB0CF  &= ~0x80;                   // Disable SMBus
  SMB0CF  |=  0x80;                   // Re-enable SMBus
  TMR3CN0 &= ~0x80;                   // Clear Timer3 interrupt-pending flag
}

//-----------------------------------------------------------------------------
// SI_INTERRUPT (PMATCH_ISR, PMATCH_IRQn)
//-----------------------------------------------------------------------------
void __ioexp_PMATCH_ISR(void)
{
  SFRPAGE = LEGACY_PAGE;              // EIE on SFRPAGE 0x00
  EMAT_DISABLE();        // Disable Port Match interrupts to prevent multiply interrupts
  IOEXP_INP_CHANGED_IRQ = SET_IRQ;
  unchecked_port_after_irq = IOEXP_PORT_MASK;
  // The new match set and enable the interrupt can happen when the input
  // have been read. The IOEXP_INP_CHANGED_IRQ pin have to release the same
  // time as well.
}

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then waits for SMBus
// communication.
//
//-----------------------------------------------------------------------------
void ioexp_main (void)
{
  uint8_t SFRPAGE_save;
  uint8_t i;
  uint16_t addr;
  bool IE_SAVE;

  IE_SAVE = IE_EA;
  IE_EA = 0;

  SFRPAGE = LEGACY_PAGE;

  // Initialize things around match IRQ
  IOEXP_INP_CHANGED_IRQ = CLEAR_IRQ;
  P0MASK = (P0MDIN & ~P0MDOUT) & P0DEVMASK;
  P1MASK = (P1MDIN & ~P1MDOUT) & P1DEVMASK;
  EMAT_ENABLE();  // Enable Port Match interrupts
  // Initialize SMB address
  // default addr: 0xF0 (0x78)
  SMB0ADR = SLAVE_ADDR;
  if (IOEXP_ADDR_EXT == 0)
  {
    // If IOEXP_ADDR_EXT is connected to GND, use extended address : 0xF2 (0x79)
    SMB0ADR = SLAVE_ADDR_EXT;
  }
  // Collect device information
  memset(&dev_id, 0, sizeof(ioexp_info_t));
  dev_id.signum = IOEXP_DEVICE_ID;
  dev_id.deviceId = DEVICEID;
  dev_id.derivId = DERIVID;
  dev_id.revId = REVID;
  SFRPAGE = LEGACY_PAGE;
  dev_id.smbAddr = SMB0ADR;
  for (i=0, addr=UID_ST_ADDR; addr >= UID_SP_ADDR; i++, addr--)
  {
    SI_VARIABLE_SEGMENT_POINTER(pread, uint8_t, const UID_SEG); // Read pointer
    pread = (SI_VARIABLE_SEGMENT_POINTER(, uint8_t, const UID_SEG)) addr;
    dev_id.uid[i] = *pread;
  }

  IE_EA = IE_SAVE;

  while(1)
  {
    switch (IOEXP_STATUS)
    {
      case IOEXP_ST_DATA_RECEIVED:
        SFRPAGE_save = SFRPAGE;
        SFRPAGE = LEGACY_PAGE;
        switch (ioexp_cmd & IOEXP_CMD_MASK)
        {
          case IOEXP_CMD_INPUT:
            break;
          case IOEXP_CMD_OUTPUT:
            WRITE_OUTPUT_CMD(ioexp_cmd & IOEXP_PORT_MASK, ioexp_data);
            goto AI_LBL;
          case IOEXP_CMD_INVERT:
            WRITE_INVERT_CMD(ioexp_cmd & IOEXP_PORT_MASK, ioexp_data);
            goto AI_LBL;
          case IOEXP_CMD_CONFIG:
            WRITE_CONFIG_CMD(ioexp_cmd & IOEXP_PORT_MASK, ioexp_data);
            goto AI_LBL;
          case IOEXP_CMD_DRIVE:
            WRITE_DRIVE_CMD(ioexp_cmd & IOEXP_PORT_MASK, ioexp_data);
AI_LBL:     CMD_AUTO_INCREMENT(ioexp_cmd);
            break;
          case IOEXP_CMD_SLEEP:
            if (ioexp_data == 1)
            {
              PCON0 |= 0x02; // Stop the SoC
            }
            break;
        }
        SFRPAGE = SFRPAGE_save;
        IOEXP_STATUS = IOEXP_ST_WAITNG_FOR_DATA;
        break;
      default:      // IOEXP_ST_READY
        break;
    }
  }
}

