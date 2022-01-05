//-----------------------------------------------------------------------------
// EFM8BB52_PWM_Single_Ended_Output.c
//-----------------------------------------------------------------------------
// Copyright 2021 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This program configures the PWM module to produce single ended outputs on
// PWM channels 0 and 1.
//
// Resources:
//   SYSCLK - 24.5 MHz HFOSC0 / 8
//   P1.4   - LED green
//   P1.5   - LED blue (not used)
//   P1.6   - LED red
//   P3.4   - Display enable
//
//   BTN0 - P0.2
//   BTN1 - P0.3 (not used)
//
//   PWM0-CH0X - P1.4 (LED green)
//   PWM0-CH1X - P1.6 (LED red)
//
//   TIMER2 - Timer used to update duty LED duty cycle
//            (T2 interrupt frequency = 20 Hz)
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB52 PK
//-----------------------------------------------------------------------------
// 1) Place the switch in "AEM" mode.
// 2) Connect the EFM8BB52 PK board to a PC using a USB cable.
// 3) Compile and download code to the EFM8BB52 PK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) Initially, the red and green LEDs will each be lit at using a 50% initial
//    duty cycle from PWM0 CH0 (P1.4/LED green) and CH1 (P1.6/LED red).
// 6) Press and hold BTN0 to change the duty cycle of each LED.  When BTN0
//    is pressed, the duty cycle will change every ~ 50 ms in 5 % duty cycle
//    steps.  Duty cycle for each channel will alternate between 0 % and 100 %
//    and will alternate in opposite directions.
// 7) In addition to visually viewing the changing duty cycle on LED green and
//    LED red, connect oscilloscope probes to P1.4 and P1.6 to view the PWM
//    waveforms on a scope.
//
// Target:         EFM8BB52
// Tool chain:     Generic
//
// Release 1.1 (MR)
//    - Minor edits made to the instructions
//    - Removed definitions for LEDs since PWM signaling is handled in hardware
//
// Release 1.0 (MC)
//    - Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8BB52_Register_Enums.h>
#include <InitDevice.h>

//-----------------------------------------------------------------------------
// Pin Definitions and General Defines
//-----------------------------------------------------------------------------
SI_SBIT (DISP_EN, SFR_P3, 4);       // Display Enable
#define DISP_BC_DRIVEN   0          // 0 = Board Controller drives display
#define DISP_EFM8_DRIVEN 1          // 1 = EFM8 drives display

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
   * 1) compile-time configurable alignment (PWMALIGN)
   * 2) Update CH0 and CH1 from update register on OF
   * 3) enable PWM0 clock */
  PWMCFG0 = PWMALIGN | PWMCFG0_SYNCUPD__CH0CH1 |
            PWMCFG0_PWMCLKEN__ENABLE;

  /* Configure PWMCFG2:
   * 1) clear the counter
   * 2) no external trigger (default)
   * 3) enable CH0 and CH1  */
  PWMCFG2 = PWMCFG2_CNTRZERO__ZERO | PWMCFG2_CH1EN__ENABLE |
            PWMCFG2_CH0EN__ENABLE;

  /* Configure PWM clock frequency based on calculated PWMCKDIVval */
  PWMCKDIV = PWMCKDIVval;

  /* Configure PWM frequency based on calculated PWMLIMval definition */
  PWMLIM = PWMLIMval;


  /* Set initial duty cycle, CH0 = 50% */
  PWMCP0 = PWMLIMval >> 1;
  PWMCPUD0 = PWMLIMval >> 1;

  /* Set initial duty cycle, CH1 = 50% */
  PWMCP1 = PWMLIMval >> 1;
  PWMCPUD1 = PWMLIMval >> 1;

  /* Configure PWMCFG1:
   * 1) no DTI, all single ended output, f_PWM >= f_SYSCLK
   * 2) enable PWM0 */
  PWMCFG1 = PWMCFG1_PWMLTSYS__GTSYSCK | PWMCFG1_PWMEN__ENABLE;

  SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// UpdateDutyCycle
//-----------------------------------------------------------------------------
//
// This routine changes the PWM CH0 and CH1 duty cycles in PWMUD_STEP steps,
// alternating direction between 0 and 100 %.  Initial step is ~ 5%.
//
//-----------------------------------------------------------------------------
void UpdateDutyCycle(void)
{
  uint16_t PWMCH0cmpval, PWMCH1cmpval;
  static uint8_t CH0dir = 0;  // 0 == down, 1 == up
  static uint8_t CH1dir = 1;  // 0 == down, 1 == up

  uint8_t SFRPAGE_save = SFRPAGE;
  SFRPAGE = PG2_PAGE;
  if(BTN0 == 0){
      // take a snapshot of the CH0 and CH1 compare values
      PWMCFG3 |= PWMCFG3_PWMSNP__SNPCH0CTR;
      PWMCH0cmpval = PWMCP0;
      PWMCFG3 |= PWMCFG3_PWMSNP__SNPCH1CH2;
      PWMCH1cmpval = PWMCP1;

      // update CH0 duty cycle
      if(CH0dir == 1){   // duty cycle is increasing
          if((PWMCFG2 & PWMCFG2_CH0EN__ENABLE) != PWMCFG2_CH0EN__ENABLE){ // PWM channel previous state was 0% duty cycle (OFF)
              PWMCPUD0 = 1 + PWMUD_STEP;
              PWMCFG2 |= PWMCFG2_CH0EN__ENABLE; //re-enable
          }
          else if((PWMCH0cmpval + PWMUD_STEP) >= (PWMLIMval-1)){
              PWMCPUD0 = 0; // This value corresponds to 100% duty cycle (Full ON)
              CH0dir = !CH0dir;
          }
          else{
              PWMCPUD0 = PWMCH0cmpval + PWMUD_STEP;
          }
      }
      else{  // duty cycle is decreasing
          if(PWMCH0cmpval == 0){ // PWM channel previous state was 100% duty cycle (ON)
              PWMCPUD0 = PWMLIMval - 1 - PWMUD_STEP;
          }
          else if((PWMCH0cmpval < PWMUD_STEP)){
              PWMCFG2 &= ~(PWMCFG2_CH0EN__ENABLE); // disable PWM channel to achieve 0% duty cycle (OFF)
              CH0dir = !CH0dir;  // change direction
          }
          else{
              PWMCPUD0 = PWMCH0cmpval - PWMUD_STEP;
          }
      }

      // update CH1 duty cycle
      if(CH1dir == 1){   // duty cycle is increasing
          if((PWMCFG2 & PWMCFG2_CH1EN__ENABLE) != PWMCFG2_CH1EN__ENABLE){ //PWM channel previous state was 0% duty cycle (OFF)
              PWMCPUD1 = 1 + PWMUD_STEP;
              PWMCFG2 |= PWMCFG2_CH1EN__ENABLE; //re-enable
          }
          else if((PWMCH1cmpval + PWMUD_STEP) >= (PWMLIMval-1)){
              PWMCPUD1 = 0; // This value corresponds to 100% duty cycle (Full ON)
              CH1dir = !CH1dir;
          }
          else{
              PWMCPUD1 = PWMCH1cmpval + PWMUD_STEP;
          }
      }
      else{  // duty cycle is decreasing
          if(PWMCH1cmpval == 0){  // PWM channel previous state was 100% duty cycle (ON)
              PWMCPUD1 = PWMLIMval - 1 - PWMUD_STEP;
          }
          else if((PWMCH1cmpval < PWMUD_STEP)){
              PWMCFG2 &= ~(PWMCFG2_CH1EN__ENABLE); // disable PWM channel to achieve 0% duty cycle (OFF)
              CH1dir = !CH1dir;  // change direction
          }
          else{
              PWMCPUD1 = PWMCH1cmpval - PWMUD_STEP;
          }
      }
  }
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

   /* Initialize the PWM for single ended output on CH0 and CH1 */
   initPWM();

   IE_EA = 1;                          // Enable global interrupts

   while (1)
   {
       /* The variable 'updateDuty' is set in the TIMER2 ISR at a frequency of
        * ~ 20 Hz */
       if (updateDuty == 1){
           UpdateDutyCycle();
           updateDuty = 0;
       }
   }
}


