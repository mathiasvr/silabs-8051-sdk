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
extern void enter_DefaultMode_from_RESET(void);
extern void enter_RTC_SelfOsc_from_DefaultMode(void);
extern void enter_RTC_Crystal_from_DefaultMode(void);
// [Mode Transition Prototypes]$

// $[Config(Per-Module Mode)Transition Prototypes]
extern void PCA_0_enter_DefaultMode_from_RESET(void);
extern void CLOCK_0_enter_DefaultMode_from_RESET(void);
extern void FLASH_0_enter_DefaultMode_from_RESET(void);
extern void PORTS_1_enter_DefaultMode_from_RESET(void);
extern void PBCFG_0_enter_DefaultMode_from_RESET(void);
extern void TIMER16_2_enter_DefaultMode_from_RESET(void);
extern void TIMER_SETUP_0_enter_DefaultMode_from_RESET(void);
extern void DCDC_0_enter_DefaultMode_from_RESET(void);
extern void RSTSRC_0_enter_DefaultMode_from_RESET(void);
extern void INTERRUPT_0_enter_DefaultMode_from_RESET(void);
extern void RTC_0_enter_RTC_SelfOsc_from_DefaultMode(void);
extern void INTERRUPT_0_enter_RTC_SelfOsc_from_DefaultMode(void);
extern void RTC_0_enter_RTC_Crystal_from_DefaultMode(void);
extern void INTERRUPT_0_enter_RTC_Crystal_from_DefaultMode(void);
// [Config(Per-Module Mode)Transition Prototypes]$


#endif

