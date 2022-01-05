//=========================================================
// inc/InitDevice.h: generated by Hardware Configurator
//
// This file will be regenerated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!
//=========================================================
#ifndef __INIT_DEVICE_H__
#define __INIT_DEVICE_H__

// USER CONSTANTS
// USER PROTOTYPES

// $[Mode Transition Prototypes]
extern void enter_SlowBlink_from_RESET(void);
extern void enter_FastBlink_from_RESET(void);
extern void enter_SlowBlink_WD_from_SlowBlink(void);
// [Mode Transition Prototypes]$

// $[Config(Per-Module Mode)Transition Prototypes]
extern void PCA_0_enter_SlowBlink_from_RESET(void);
extern void PORTS_2_enter_SlowBlink_from_RESET(void);
extern void PBCFG_0_enter_SlowBlink_from_RESET(void);
extern void RSTSRC_0_enter_SlowBlink_from_RESET(void);
extern void HFOSC_0_enter_SlowBlink_from_RESET(void);
extern void CLOCK_0_enter_SlowBlink_from_RESET(void);
extern void TIMER16_2_enter_SlowBlink_from_RESET(void);
extern void INTERRUPT_0_enter_SlowBlink_from_RESET(void);
extern void PCA_0_enter_FastBlink_from_RESET(void);
extern void PORTS_2_enter_FastBlink_from_RESET(void);
extern void PBCFG_0_enter_FastBlink_from_RESET(void);
extern void RSTSRC_0_enter_FastBlink_from_RESET(void);
extern void HFOSC_0_enter_FastBlink_from_RESET(void);
extern void CLOCK_0_enter_FastBlink_from_RESET(void);
extern void TIMER16_2_enter_FastBlink_from_RESET(void);
extern void INTERRUPT_0_enter_FastBlink_from_RESET(void);
extern void PCA_0_enter_SlowBlink_WD_from_SlowBlink(void);
extern void PCACH_2_enter_SlowBlink_WD_from_SlowBlink(void);
extern void PCACH_5_enter_SlowBlink_WD_from_SlowBlink(void);
// [Config(Per-Module Mode)Transition Prototypes]$


#endif

