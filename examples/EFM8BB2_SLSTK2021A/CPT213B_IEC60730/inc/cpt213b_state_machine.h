/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef INC_CPT213B_STATE_MACHINE_H_
#define INC_CPT213B_STATE_MACHINE_H_

#include "project_config.h"

extern uint8_t PB0_previous, PB0_current;
extern uint8_t PB1_previous, PB1_current;
extern bool firstStatusPkt;
extern uint8_t pkt_count;

uint8_t CPT_state_machine(void);
extern uint8_t cpt_state;

#define ENTER_CONFIG_MODE 		0
#define ENTER_SENSING_MODE		1
#define UPDATE_SMBUS			2
#define ENTER_SAFE_STATE		3

#endif /* INC_CPT213B_STATE_MACHINE_H_ */
