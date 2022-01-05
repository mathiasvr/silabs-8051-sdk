//-----------------------------------------------------------------------------
// EFM8BB51_PWM_Differential_DTI_Kill.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program configures the PWM module to produce differential output on PWM
// channel 0 with configurable alignment and dead time insertion. Additionally,
// kill signals from both CLU3 and CMP1 are enabled to stop the PWM output when
// P0.2 (BTN0) or P1.3 (EXP Header pin 15) are driven low. The PWM can be 
// restarted by software after being stopped by driving P0.3 (BTN1) low.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 8
//   P1.5   - LED0
//   P1.6   - LED1
//   P2.0   - Display enable, shared by C2D pin
//            See AN127 for C2D pin sharing information
//
//   P0.2   - BTN0, kill PWM signal when pressed
//   P0.3   - BTN1, restart PWM after killed
//
//   PWM0-CH0X - P1.5 (LED0)
//   PWM0-CH0Y - P1.6 (LED1)
//
//   CLU3 - Asserts PWM Kill signal when BTN1 is pressed
//   CMP1 - Asserts PWM Kill signal when P1.3 (EXP Header 15) is driven low
//
//   TIMER2 - Timer used to update LED duty cycle
//            (T2 interrupt frequency = 20 Hz)
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
// 5) Initially, LED0 and LED1 will alternate brightness as the CH0 PWM duty
//    cycle is updated at a 20 Hz interval.
// 6) Press BTN0 to kill the PWM via CLU3 output. This stops the PWM and sets
//    the CH0X and CH0Y to specified safe states, which in this example
//    corresponds to LED0 on and LED1 off.
// 7) Alternatively, drive P1.3 (EXP Header pin 15) low to kill the PWM via
//    CMP1 output. This stops the PWM in the same manner as step 6.
// 8) Press BTN1 to restart the PWM via software. To restart the PWM after the
//    kill signal is de-asserted, the PWMEN bit is toggled off, then on again.
// 9) In addition to visually viewing the changing duty cycle on LED0 and
//    LED1, connect oscilloscope probes to P1.5 and P1.6 to view the PWM
//    waveforms and observe the effects of any changes to the PWMALIGN,
//    PWMCH0X_DTILIMval, PWMCH0Y_DTILIMval, or other configurable defines.
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

/* Configurable defines below */

/* Change SYSCLK_PREDIV_FREQ if you change the system clock source */
#define SYSCLK_PREDIV_FREQ    24500000UL   // in Hz

/* PWMCLKDIVval specifies the PWM clock divider
 * The PWM clock freq equals SYSCLK_PREDIV_FREQ / (2 ^ PWMCKDIVval)
 */
#define PWMCKDIVval           3

/* Use PWM_FREQ_DESIRED to specify the desired EDGE aligned frequency
 * NOTE: If using center-alignment, the frequency will be approximately
 * half that of the edge-aligned frequency.
 * The relationship of center-aligned period (Pc) to edge-aligned period (Pe)
 * when using the same PWMLIM and PWMCPx values is given by:
 * Pc = (2 * Pe) - 2
 */
#define PWM_FREQ_DESIRED      100UL        // edge aligned frequency, in Hz

/* PWMLIMval is used to set the PWM counter overflow value, and is
 * calulated from the configurable constants above
 */
#define PWMLIMval             ((SYSCLK_PREDIV_FREQ/(1<<PWMCKDIVval))/PWM_FREQ_DESIRED)

/* PWMUD_STEP configures the step value used when adjusting the PWM
 * duty cycle - initial step is 5%
 */
#define PWMUD_STEP            PWMLIMval / 20UL

/* Change PWMALIGN to configure the alignment of the PWM signal.
 * Choose one of the following values:
 * PWMCFG0_PWMMODE__EDGE
 * PWMCFG0_PWMMODE__CENTER
 * */
#define PWMALIGN              PWMCFG0_PWMMODE__EDGE

/* Change PWMCH0X_DTILIMval to configure the offset of the rising overflow edge
 * on the PWM CH0x output in edge-aligned mode, and the rising up-count edge in
 * center-aligned mode.
 */
#define PWMCH0X_DTILIMval      0x2

/* Change PWMCH0Y_DTILIMval to configure the offset of the rising match edge on
 * the PWM CH0y output in edge-aligned mode, and the rising down-count edge in
 * center-aligned mode.
 */
#define PWMCH0Y_DTILIMval        0x3

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
volatile uint8_t updateDuty;

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
   * 2) Update CH0 from update register on OF
   * 3) Enable kill signal from CLU3 output (BTN0)
   * 4) Enable kill signal from CMP1 output (EXP Header
   * 4) Enable PWM0 clock
  */
  PWMCFG0 = PWMALIGN | PWMCFG0_SYNCUPD__CH0 | PWMCFG0_KILL0EN__ENABLE
    | PWMCFG0_KILL1EN__ENABLE | PWMCFG0_PWMCLKEN__ENABLE;

  /* Configure PWMCFG1:
   * 1) Enable dead time insertion
   * 2) Enable differential mode on CH0
   * 3) Configure DTI mode to add dead time before rising PWM edges
   * 4) Configure for f_PWM >= f_SYSCLK
   */
  PWMCFG1 = PWMCFG1_DTIEN__ENABLE | PWMCFG1_DIFFMODE0__ENABLE
    | PWMCFG1_DTIMODE__RISING | PWMCFG1_PWMLTSYS__GTSYSCK;

  /* Configure PWMCFG2:
   * 1) clear the counter
   * 2) no external trigger (default)
   * 3) enable CH0
   */
  PWMCFG2 = PWMCFG2_CNTRZERO__ZERO | PWMCFG2_CH0EN__ENABLE;

  /* Configure PWMCFG3:
   * Set CH0x and CH0y safe state when kill signal is asserted, safe state is
   * CH0x = 1 and CH0y = 0
   */
  PWMCFG3 = PWMCFG3_CH0XSAFST__SAFE1 | PWMCFG3_CH0YSAFST__SAFE0;

  /* Configure PWM clock frequency based on calculated PWMCKDIVval */
  PWMCKDIV = PWMCKDIVval;

  /* Configure PWM frequency based on calculated PWMLIMval definition */
  PWMLIM = PWMLIMval;

  /* Set initial duty cycle, CH0 = 50% */
  PWMCP0 = PWMLIMval >> 1;
  PWMCPUD0 = PWMLIMval >> 1;

  /* Set PWM DTI limit for CH0x */
  PWMDTIPLIM = PWMCH0X_DTILIMval;

  /* Set PWM DTI limit for CH0y */
  PWMDTIPLIM = PWMCH0Y_DTILIMval;

  /* Enable PWM, do not overwrite previous PWMCFG1 settings */
  PWMCFG1 |= PWMCFG1_PWMEN__ENABLE;

  SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// updateDutyCycle
//-----------------------------------------------------------------------------
//
// This routine changes the PWM CH0 duty cycles in PWMUD_STEP steps,
// alternating direction between 0 and 100 %.  Initial step is ~ 5%.
//
//-----------------------------------------------------------------------------
void updateDutyCycle(void)
{
  uint16_t PWMCH0cmpval;
  static uint8_t CH0dir = 0;  // 0 == down, 1 == up

  uint8_t SFRPAGE_save = SFRPAGE;
  SFRPAGE = PG2_PAGE;
  // take a snapshot of the CH0 compare values
  PWMCFG3 |= PWMCFG3_PWMSNP__SNPCH0CTR;
  PWMCH0cmpval = PWMCP0;

  // update CH0 duty cycle
  if (CH0dir == 1) {   // duty cycle is increasing
    if ((PWMCH0cmpval + PWMUD_STEP) >= (PWMLIMval - 1)) {
      PWMCPUD0 = PWMLIMval-1;
      CH0dir = !CH0dir;
    }
    else {
      PWMCPUD0 = PWMCH0cmpval + PWMUD_STEP;
    }
  }
  else {  // duty cycle is decreasing
    if ((PWMCH0cmpval < PWMUD_STEP)) {
      PWMCPUD0 = 1;
      CH0dir = !CH0dir;  // change direction
    }
    else {
      PWMCPUD0 = PWMCH0cmpval - PWMUD_STEP;
    }
  }
  SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// restart
//-----------------------------------------------------------------------------
//
// This routine restarts the PWM by toggling the PWMEN bit off, then on again.
// This is required to restart the PWM from software after a kill signal stops 
// the PWM.
//
//-----------------------------------------------------------------------------
void restart(void) {
  uint8_t SFRPAGE_save = SFRPAGE;
  SFRPAGE = PG2_PAGE;
  PWMCFG1 &= ~PWMCFG1_PWMEN__ENABLE;
  PWMCFG1 |= PWMCFG1_PWMEN__ENABLE;
  SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main (void)
{

  // initialize the flag that triggers a PWM duty cycle update
  updateDuty = 0;

  enter_DefaultMode_from_RESET();

  DISP_EN = DISP_BC_DRIVEN;           // Display not driven by EFM8

  // Initialize the PWM for differential output with DTI and kill signals*/
  initPWM();

  IE_EA = 1;                          // Enable global interrupts

  while (1)
  {
    // The variable 'updateDuty' is set in the TIMER2 ISR at a frequency of
    // ~ 20 Hz */
    if (updateDuty == 1) {
      updateDutyCycle();

      if (BTN1 == 0) {
        restart();
      }
      updateDuty = 0;
    }
  }
}
