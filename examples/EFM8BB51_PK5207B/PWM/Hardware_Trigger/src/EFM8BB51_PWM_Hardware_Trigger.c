//-----------------------------------------------------------------------------
// EFM8BB51_PWM_Hardware_Trigger.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program demonstrates using a hardware trigger to start the PWM, and how
// to enable and handle PWM interrupts.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 8
//   P1.5   - LED0
//   P1.6   - LED1
//   P2.0   - Display enable, shared by C2D pin
//            See AN127 for C2D pin sharing information
//
//   BTN0 - P0.2
//   BTN1 - P0.3
//
//   PWM0-CH0X - P1.5 (LED0)
//   PWM0-CH1X - P1.6 (LED1)
//
//   CLU2 - CLU logic to start PWM when BTN0 is pressed
//   CLU3 - CLU logic to stop PWM when BTN1 is pressed
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB51 PK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB51 PK board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB51 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Initially, the PWM will wait for a hardware trigger to start the counter
// 6) Press BTN0 to drive the CLU2 output high and trigger the PWM. LED 0 and
//    LED1 should both turn on with a 50% duty cycle.
// 7) Press BTN1 to drive the CLU3 output high and kill the PWM output. This 
//    triggers a PWM Halt interrupt. In this example the interrupt routine 
//    re-arms the PWM for the next hardware trigger event.
// 8) In addition to visually viewing the 50% duty cycle on LED0 and LED1,
//    connect oscilloscope probes to P1.5 and P1.6 to view the PWM waveforms
//    on a scope.
//
// Target:         EFM8BB51
// Tool chain:     Generic
//
// Release 1.0 (DS)
//    - Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB51_Register_Enums.h>
#include <InitDevice.h>

//-----------------------------------------------------------------------------
// Pin Definitions and General Defines
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P2, 0);          // Display Enable, shared by C2D pin
                                       // See AN127 for C2D pin sharing info
#define DISP_BC_DRIVEN   0             // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1             // 1 = EFM8 drives display

SI_SBIT (BTN0, SFR_P0, 2);          // BTN0
SI_SBIT (BTN1, SFR_P0, 3);          // BTN1

#define CH0                   0
#define CH1                   1

/* Configurable defines below */

/* Change SYSCLK_PREDIV_FREQ if you change the system clock source */
#define SYSCLK_PREDIV_FREQ    24500000UL   // in Hz

/* PWMCLKDIVval specifies the PWM clock divider
 * The PWM clock freq equals SYSCLK_PREDIV_FREQ / (2 ^ PWMCKDIVval)
 * */
#define PWMCKDIVval           3

/* Use PWM_FREQ_DESIRED to specify the desired EDGE aligned frequency
 * NOTE: If using center-alignment, the frequency will be approximately
 * half that of the edge-aligned frequency.
 * The relationship of center-aligned period (Pc) to edge-aligned period (Pe)
 * when using the same PWMLIM and PWMCPx values is given by:
 * Pc = (2 * Pe) - 2
 *  */
#define PWM_FREQ_DESIRED      100UL        // edge aligned frequency, in Hz

/* PWMLIMval is used to set the PWM counter overflow value, and is
 * calulated from the configurable constants above */
#define PWMLIMval             ((SYSCLK_PREDIV_FREQ/(1<<PWMCKDIVval))/PWM_FREQ_DESIRED)

/* PWMUD_STEP configures the step value used when adjusting the PWM
 * duty cycle - initial step is 5% */
#define PWMUD_STEP            PWMLIMval / 20UL//PWMLIMval / 100UL

/* Change PWMALIGN to configure the alignment of the PWM signal.
 * Choose one of the following values:
 * PWMCFG0_PWMMODE__EDGE
 * PWMCFG0_PWMMODE__CENTER
 * */
#define PWMALIGN              PWMCFG0_PWMMODE__EDGE

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
  // Disable the watchdog here
}

//-----------------------------------------------------------------------------
// initPWM
//-----------------------------------------------------------------------------
//
// This routine initializes the PWM for single ended output on channels 0 and 1.
//
//-----------------------------------------------------------------------------
void initPWM(void)
{
  uint8_t SFRPAGE_save = SFRPAGE;
  SFRPAGE = PG2_PAGE;

  /* Configure PWMCFG0:
   * 1) Set compile-time configurable alignment (PWMALIGN)
   * 2) Update CH0 and CH1 from update register on OF
   * 3) Enable kill signal from CLU3 output (BTN0)
   * 4) Configure PWM hardware trigger on rising edge of trigger source
   * 5) Enable PWM0 clock
   */
  PWMCFG0 = PWMALIGN | PWMCFG0_SYNCUPD__CH0CH1 | PWMCFG0_KILL0EN__ENABLE
      | PWMCFG0_TRGESEL__REDGE | PWMCFG0_PWMCLKEN__ENABLE;

  /* Configure PWMCFG1:
   * 1) Use single ended mode on CH0 and CH1
   * 2) Configure for f_PWM >= f_SYSCLK
   */
  PWMCFG1 = PWMCFG1_DIFFMODE0__DISABLE | PWMCFG1_DIFFMODE1__DISABLE
      | PWMCFG1_PWMLTSYS__GTSYSCK;

  /* Configure PWMCFG2:
   * 1) Clear the counter
   * 2) Configure CLU2 as hardware trigger source
   * 3) Enable CH0 and CH1
   */
  PWMCFG2 = PWMCFG2_CNTRZERO__ZERO | PWMCFG2_TRGSEL__CLU2OUTA
      | PWMCFG2_CH1EN__ENABLE | PWMCFG2_CH0EN__ENABLE;

  /* Configure PWMCFG3:
   * Set CH0x and CH0y safe state when kill signal is asserted, safe state is
   * CH0x = 0 and CH1x = 0
   */
  PWMCFG3 = PWMCFG3_CH0XSAFST__SAFE0 | PWMCFG3_CH1XSAFST__SAFE0;

  /* Configure PWM clock frequency based on calculated PWMCKDIVval */
  PWMCKDIV = PWMCKDIVval;

  /* Configure PWM Interrupts
   * Enable PWM halt interrupts when kill signal asserted
   */
  PWMIE = PWMIE_HALTIE__ENABLE;

  /* Configure PWM frequency based on calculated PWMLIMval definition */
  PWMLIM = PWMLIMval;

  /* Set duty cycle, CH0 = 50% */
  PWMCP0 = PWMLIMval >> 1;
  PWMCPUD0 = PWMLIMval >> 1;

  /* Set duty cycle, CH1 = 50% */
  PWMCP1 = PWMLIMval >> 1;
  PWMCPUD1 = PWMLIMval >> 1;

  /* Enable PWM, hardware trigger (CLU2) will start the PWM counter,
   * do not overwrite previous PWMCFG1 settings
   */
  PWMCFG1 |= PWMCFG1_PWMEN__ENABLE;

  SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{
  enter_DefaultMode_from_RESET();

  DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8

  /* Initialize the PWM for single ended output on CH0 and CH1 */
  initPWM();

  IE_EA = 1;                          // Enable global interrupts

  while (1);
}


